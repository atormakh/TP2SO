#include <ipc.h>
#include <scheduler.h>
#include <pipes.h>
#include <lib.h>

void writePipeProc(unsigned long long pid, int fd,char * buffer,int maxWrite){
    PCB * proc = getProc(pid);
    Pipe * fdWrite;

    while((fdWrite =proc->fd[fd]) == NULL){
        createMotive(proc);
        blockMotive(proc,proc->pid);
        yield();
    }

    //verificar si se puede escribir en el pipe
    while(maxWrite>=fdWrite->bufferSize-fdWrite->counter){
        blockMotive(&fdWrite->writeIndex,proc->pid);
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

void writePipe(int fd,char * buffer,int maxWrite){
    PCB * proc = getCurrentProc();
    Pipe * fdWrite;;

    while((fdWrite =proc->fd[fd]) == NULL){
        createMotive(proc);
        blockMotive(proc,proc->pid);
        yield();
    }

    //verificar si se puede escribir en el pipe
    while(maxWrite>=fdWrite->bufferSize-fdWrite->counter){
        blockMotive(&fdWrite->writeIndex,proc->pid);
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

unsigned long long readPipe(int fd,char * buffer,int maxRead){
    PCB * proc = getCurrentProc();
    Pipe * fdRead;
    
    while((fdRead =proc->fd[fd]) == NULL){
        createMotive(proc);
        blockMotive(proc,proc->pid);
        yield();
    }

    //verificar si se puede escribir en el pipe
    while(fdRead->counter<=0){
       blockMotive(&fdRead->readIndex,proc->pid);
       yield();
    }


    unsigned long long i=0;
    for(i=0; i<fdRead->counter && i<maxRead;i++){
        
        buffer[i]=fdRead->buffer[fdRead->readIndex%fdRead->bufferSize];
        fdRead->readIndex++;
    }
    //buffer[i++]=0;
    

    fdRead->counter-=i;
    //reseteo los valores del readIndex del file descriptor asi no tenemos problemas con un eventual overflow
    fdRead->readIndex = fdRead->readIndex % fdRead->bufferSize;

    //despertar a los writers
    awakeAll(&fdRead->writeIndex);
    return i;
}