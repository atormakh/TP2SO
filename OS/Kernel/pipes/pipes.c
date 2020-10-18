
#include <pipes.h>
#include <memorymanager.h>
#include <scheduler.h>

int pipe(unsigned long pidWriter, unsigned int fdWrite, unsigned long pidReader, unsigned int fdRead ){
    closePipeProc(fdWrite,pidWriter);
    Pipe * pipe;
    if((pipe=getProc(pidReader)->fd[fdRead]) == NULL){
        pipe = m_alloc(PAGE_SIZE);
        
        if(pipe == NULL){
            //tirar error
            return -1;
        }   
        //[.................................................]
        //p         .
        //          p+1=buffer
        pipe->buffer = m_alloc(PAGE_SIZE);
        pipe->writeIndex=0;
        pipe->readIndex=0;
        pipe->counter = 0;
        pipe->writerRefs = 0;
        pipe->readerRefs = 0;
        pipe->status=READ | WRITE;
        pipe->bufferSize=PAGE_SIZE;

        createMotive(&pipe->writeIndex);
        createMotive(&pipe->readIndex);

        setProcFD(pidReader,fdRead,pipe, READ);

    }
    setProcFD(pidWriter,fdWrite,pipe, WRITE);
    pipe->writerRefs++;
    pipe->readerRefs++;
    PCB * procReader = getProc(pidReader);
    PCB * procWriter = getProc(pidWriter);

    return 0;
}

void closePipe(int fd){
    closePipeProc(fd,getCurrentProc()->pid);
}

void closePipeProc(int fd, unsigned long long pid){
    Pipe * pipe = getProc(pid)->fd[fd];
    if(pipe == NULL) return;
    int permission = getProc(pid)->role[fd];
    if(permission == WRITE) pipe->writerRefs--;
    else pipe->readerRefs--;
    if(pipe->readerRefs <=0 && pipe->writerRefs<=0) m_free(pipe);
}

