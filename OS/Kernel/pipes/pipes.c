
#include <pipes.h>
#include <memorymanager.h>
#include <scheduler.h>

#define WRITE_OPEN 1
#define READ_OPEN 2

typedef struct Pipe{
    unsigned int writeIndex;
    unsigned char status;
    unsigned int readIndex;
    unsigned int bufferSize;
    unsigned char * buffer;
} Pipe;



int pipe(unsigned long pidWriter, unsigned int fdWrite, unsigned long pidReader, unsigned int fdRead ){
    
    Pipe * pipe = m_alloc(PAGE_SIZE);
    //TODO validar malloc no tire error
    
    pipe->buffer = pipe++;
    pipe->writeIndex=0;
    pipe->readIndex=0;
    pipe->status=READ_OPEN | WRITE_OPEN;
    pipe->bufferSize=PAGE_SIZE - sizeof(Pipe);

    //PCB * writer = getProc(pidWriter);

    // setProcFD(pidWriter,fdWrite,pipe, WRITE);
    // setProcFD(pidReader,fdRead,pipe, READ);
    return 0;
}
