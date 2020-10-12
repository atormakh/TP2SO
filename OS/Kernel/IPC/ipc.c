#include <ipc.h>
#include <scheduler.h>
#include <pipes.h>
#include <lib.h>

void writePipe(int fd,char * buffer,int maxWrite){
    PCB * proc = getCurrentProc();
    Pipe * fdWrite;;

    while((fdWrite =proc->fd[fd]) == NULL){
        createMotive(proc);
        block(proc,proc->pid);
        yield();
    }

    //verificar si se puede escribir en el pipe
    while(maxWrite>=fdWrite->bufferSize-fdWrite->counter){
        block(&fdWrite->writeIndex,proc->pid);
        yield();
    }
  

    //escribimos lo que esta en el buffer en el fd
    for(int i=0;i<maxWrite;i++){
         fdWrite->buffer[fdWrite->writeIndex%fdWrite->bufferSize]=buffer[i];
         fdWrite->writeIndex++;
    }
    
   //fdWrite->buffer[fdWrite->writeIndex%fdWrite->bufferSize]=0;
    fdWrite->counter+=maxWrite;
    
    //reseteo los valores del writeIndex del file descriptor asi no tenemos problemas con un eventual overflow
    fdWrite->writeIndex = fdWrite->writeIndex % fdWrite->bufferSize;

    awakeAll(&fdWrite->readIndex);
    
}

void readPipe(int fd,char * buffer,int maxRead,int * qty){
    PCB * proc = getCurrentProc();
    Pipe * fdRead;
    
    while((fdRead =proc->fd[fd]) == NULL){
        createMotive(proc);
        block(proc,proc->pid);
        yield();
    }

    //verificar si se puede escribir en el pipe
    while(fdRead->counter<=0){
       block(&fdRead->readIndex,proc->pid);
       yield();
    }


    int i=0;
    for(i=0; i<fdRead->counter && i<maxRead;i++){
        
        buffer[i]=fdRead->buffer[fdRead->readIndex%fdRead->bufferSize];
        fdRead->readIndex++;
    }
    //buffer[i++]=0;
    *qty=i;
    

    fdRead->counter-=i;
    //reseteo los valores del readIndex del file descriptor asi no tenemos problemas con un eventual overflow
    fdRead->readIndex = fdRead->readIndex % fdRead->bufferSize;

    //despertar a los writers
    awakeAll(&fdRead->writeIndex);
}