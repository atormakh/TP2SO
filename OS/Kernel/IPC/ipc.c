#include <ipc.h>
#include <scheduler.h>
#include <pipes.h>

void writePipe(int fd,char * buffer,int maxWrite){
    PCB * proc = getCurrentProc();
    Pipe * fdWrite =proc->fd[fd];


    //verificar si se puede escribir en el pipe
    while(maxWrite>=fdWrite->buffer-fdWrite->counter){
        //hay que despertarlo
        proc->state = BLOCKED;
    }

    //escribimos lo que esta en el buffer en el fd
    for(int i=0;i<maxWrite;i++){
         fdWrite->buffer[fdWrite->writeIndex%fdWrite->bufferSize]=buffer[i];
         fdWrite->writeIndex++;
    }
   fdWrite->buffer[fdWrite->writeIndex%fdWrite->bufferSize]=0;

    //reseteo los valores del writeIndex del file descriptor asi no tenemos problemas con un eventual overflow
    fdWrite->writeIndex = fdWrite->writeIndex % fdWrite->bufferSize;

    //despertar a los readers
}

void readPipe(int fd,char * buffer,int maxRead,int * qty){
    PCB * proc = getCurrentProc();
    Pipe * fdRead =proc->fd[fd];
    //verificar si hay algo para leer    
    int i=0;
    for(i=0;i<qty && i<maxRead;i++){
        
        buffer[i]=fdRead->buffer[fdRead->readIndex%fdRead->bufferSize];
        fdRead->readIndex++;
    }
    *qty=i;
    buffer[i]=0;

    //reseteo los valores del readIndex del file descriptor asi no tenemos problemas con un eventual overflow
    fdRead->readIndex = fdRead->readIndex % fdRead->bufferSize;

    //despertar a los writers
}