
#include <pipes.h>
#include <memorymanager.h>
#include <scheduler.h>

int pipe(unsigned long pidWriter, unsigned int fdWrite, unsigned long pidReader, unsigned int fdRead ){
    
    Pipe * pipe = m_alloc(PAGE_SIZE);
    
    if(pipe == NULL){
        //tirar error
        return -1;
    }
    
    pipe->buffer = pipe+1;
    pipe->writeIndex=0;
    pipe->readIndex=0;
    pipe->counter = 0;
    pipe->status=READ | WRITE;
    pipe->bufferSize=PAGE_SIZE - sizeof(Pipe);

    createMotive(&pipe->writeIndex);
    createMotive(&pipe->readIndex);


    setProcFD(pidWriter,fdWrite,pipe, WRITE);
    setProcFD(pidReader,fdRead,pipe, READ);
    return 0;
}


