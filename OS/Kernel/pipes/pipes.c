
#include <pipes.h>
#include <memorymanager.h>
#include <scheduler.h>
#include<lib.h>

List * pipes;
int pipeQty = 0;


void initPipes(){
    pipes=newList();
}

int pipe(char * pipeId, unsigned long pidWriter, unsigned int fdWrite, unsigned long pidReader, unsigned int fdRead ){
    closePipeProc(fdWrite,pidWriter);
    Pipe * pipe;
    if((pipe=getProc(pidReader)->fd[fdRead]) == NULL){
        pipe = c_alloc(sizeof(Pipe));
        
        if(pipe == NULL){
            return -1;
        }   
        pipe->buffer = c_alloc(PAGE_SIZE);
        pipe->pipeId = c_alloc(MAX_PIPE_NAME);

        if(pipe->buffer == NULL && pipe->pipeId ==NULL)
            return -1;

        pipe->writeIndex=0;
        pipe->readIndex=0;
        pipe->counter = 0;
        pipe->writerRefs = 0;
        pipe->readerRefs = 0;
        pipe->status=READ | WRITE;
        pipe->bufferSize=PAGE_SIZE;

        createMotive(&pipe->writeIndex);
        createMotive(&pipe->readIndex);

        if(pipeId==NULL){
            int n = (int)strcpy(pipe->pipeId,UNNAMED);
            n += intToString(pipeQty,pipe->pipeId + n);
            pipe->pipeId[n]=0;
        }else
            strcpy(pipe->pipeId,pipeId);

        push(pipes, pipe, strHash(pipe->pipeId));

        pipeQty++;

        setProcFD(pidReader,fdRead,pipe, READ);

    }
    setProcFD(pidWriter,fdWrite,pipe, WRITE);
    pipe->writerRefs++;
    pipe->readerRefs++;

    return 0;
}

void closePipe(int fd){
    closePipeProc(fd,getCurrentProc()->pid);
}

void closePipeProc(int fd, unsigned long long pid){
    Pipe * pipePTR = getProc(pid)->fd[fd];
    if(pipePTR == NULL) return;
    int permission = getProc(pid)->role[fd];
    if(permission == WRITE){ 
        pipePTR->writerRefs--;
        awakeAll(&pipePTR->readIndex);
    }
    else{
        pipePTR->readerRefs--;
        awakeAll(&pipePTR->writeIndex);
    } 
    if(pipePTR->readerRefs <=0 && pipePTR->writerRefs<=0){
        remove(pipes, strHash(pipePTR->pipeId));
        closeMotive(&pipePTR->readIndex);
        closeMotive(&pipePTR->writeIndex);
        m_free(pipePTR->buffer);
        m_free(pipePTR->pipeId); 
        m_free(pipePTR);
    }
}

void pipesInfo(char * buffer){
    buffer+=strcpy(buffer, "PIPE_ID | w_state | r_state | w_blocked  | r_blocked  \n");
    iterator(pipes);
    while(hasNext(pipes)){
        Pipe * pipePTR = getNext(pipes);
        buffer += strcpy(buffer, pipePTR->pipeId);
        buffer += strcpy(buffer, "    ");

        if(pipePTR->writerRefs > 0){
            buffer += strcpy(buffer, "true ");
        }else{
            buffer +=strcpy(buffer, "false");
        }
        buffer += strcpy(buffer, "    ");
        if(pipePTR->readerRefs > 0){
            buffer += strcpy(buffer, "true ");
        }else{
            buffer +=strcpy(buffer, "false");
        }
        buffer += strcpy(buffer, "   |   ");

        buffer+=printProcsInMotive(buffer, getMotive(&pipePTR->writeIndex));
        buffer += strcpy(buffer, "   |   ");
        buffer+=printProcsInMotive(buffer, getMotive(&pipePTR->readIndex));
        
        *buffer++='\n';
    }
    *buffer++=0;

}
