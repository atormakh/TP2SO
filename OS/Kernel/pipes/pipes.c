
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
        pipe = c_alloc(PAGE_SIZE);
        
        if(pipe == NULL){
            //tirar error
            return -1;
        }   
        pipe->buffer = c_alloc(PAGE_SIZE);
        pipe->pipeId = c_alloc(MAX_PIPE_NAME);

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
            int n = strcpy(pipe->pipeId,UNNAMED);
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
    Pipe * pipe = getProc(pid)->fd[fd];
    if(pipe == NULL) return;
    int permission = getProc(pid)->role[fd];
    if(permission == WRITE){ 
        pipe->writerRefs--;
        awakeAll(&pipe->readIndex);
    }
    else{
        pipe->readerRefs--;
        awakeAll(&pipe->writeIndex);
    } 
    if(pipe->readerRefs <=0 && pipe->writerRefs<=0){
        remove(pipes, strHash(pipe->pipeId));
        closeMotive(&pipe->readIndex);
        closeMotive(&pipe->writeIndex);
        m_free(pipe->buffer);
        m_free(pipe->pipeId); 
        m_free(pipe);
    }
}

void pipesInfo(char * buffer){
    buffer+=strcpy(buffer, "PIPE_ID | w_state | r_state | w_blocked  | r_blocked  \n");
    iterator(pipes);
    while(hasNext(pipes)){
        Pipe * pipe = next(pipes);
        buffer += strcpy(buffer, pipe->pipeId);
        buffer += strcpy(buffer, "    ");

        if(pipe->writerRefs > 0){
            buffer += strcpy(buffer, "true ");
        }else{
            buffer +=strcpy(buffer, "false");
        }
        buffer += strcpy(buffer, "    ");
        if(pipe->readerRefs > 0){
            buffer += strcpy(buffer, "true ");
        }else{
            buffer +=strcpy(buffer, "false");
        }
        buffer += strcpy(buffer, "   |   ");

        buffer+=printProcsInMotive(buffer, getMotive(&pipe->writeIndex));
        buffer += strcpy(buffer, "   |   ");
        buffer+=printProcsInMotive(buffer, getMotive(&pipe->readIndex));
        
        *buffer++='\n';
    }
    *buffer++=0;

}
