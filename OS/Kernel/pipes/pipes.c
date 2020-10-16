
#include <pipes.h>
#include <memorymanager.h>
#include <scheduler.h>

int pipe(unsigned long pidWriter, unsigned int fdWrite, unsigned long pidReader, unsigned int fdRead ){
    
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
        pipe->status=READ | WRITE;
        pipe->bufferSize=PAGE_SIZE;

        createMotive(&pipe->writeIndex);
        createMotive(&pipe->readIndex);

        setProcFD(pidReader,fdRead,pipe, READ);

    }
    setProcFD(pidWriter,fdWrite,pipe, WRITE);

    PCB * procReader = getProc(pidReader);
    PCB * procWriter = getProc(pidWriter);
    awakeAll(procWriter);
    closeMotive(procWriter);
    awakeAll(procReader);
    closeMotive(procReader);
    return 0;
}


