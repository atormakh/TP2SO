#include <ipc.h>
#include <scheduler.h>
#include <pipes.h>
#include <lib.h>

void kernelWrite(char * buffer, int n){
    writePipeProc(2,1,buffer,n);
}

void writePipeProc(unsigned long long pid, int fd,char * buffer,int maxWrite){
    PCB * proc = getProc(pid);
    Pipe * fdWrite;;

    if((fdWrite =proc->fd[fd]) == NULL){
        return ;
    }
  
    while(maxWrite>0){
        int toWrite = maxWrite > fdWrite->bufferSize-fdWrite->counter? fdWrite->bufferSize-fdWrite->counter : maxWrite;
        maxWrite-=toWrite;
        //escribimos lo que esta en el buffer en el fd
        for(int i=0;i<toWrite;i++){
            fdWrite->buffer[fdWrite->writeIndex%fdWrite->bufferSize]=buffer[i];
            fdWrite->writeIndex++;
        }
        
    //fdWrite->buffer[fdWrite->writeIndex%fdWrite->bufferSize]=0;
        fdWrite->counter+=toWrite;
        
        //reseteo los valores del writeIndex del file descriptor asi no tenemos problemas con un eventual overflow
        fdWrite->writeIndex = fdWrite->writeIndex % fdWrite->bufferSize;

        awakeAll(&fdWrite->readIndex);
        if(maxWrite > 0){
            buffer+=toWrite;
            blockMotive(&fdWrite->writeIndex,proc->pid);
            yield();
        }
    }
}




void writePipe(int fd,char * buffer,int maxWrite){
    writePipeProc(getCurrentProc()->pid,fd,buffer,maxWrite);
    // PCB * proc = getCurrentProc();
    // Pipe * fdWrite;;

    // if((fdWrite =proc->fd[fd]) == NULL){
    //     return ;
    // }
  
    // while(maxWrite>0){
    //     int toWrite = maxWrite > fdWrite->bufferSize-fdWrite->counter? fdWrite->bufferSize-fdWrite->counter : maxWrite;
    //     maxWrite-=toWrite;
    //     //escribimos lo que esta en el buffer en el fd
    //     for(int i=0;i<toWrite;i++){
    //         fdWrite->buffer[fdWrite->writeIndex%fdWrite->bufferSize]=buffer[i];
    //         fdWrite->writeIndex++;
    //     }
        
    // //fdWrite->buffer[fdWrite->writeIndex%fdWrite->bufferSize]=0;
    //     fdWrite->counter+=toWrite;
        
    //     //reseteo los valores del writeIndex del file descriptor asi no tenemos problemas con un eventual overflow
    //     fdWrite->writeIndex = fdWrite->writeIndex % fdWrite->bufferSize;

    //     awakeAll(&fdWrite->readIndex);
    //     if(maxWrite > 0){
    //         buffer+=toWrite;
    //         blockMotive(&fdWrite->writeIndex,proc->pid);
    //         yield();
    //     }
    // }
    
}

unsigned long long readPipe(int fd,char * buffer,int maxRead){
    PCB * proc = getCurrentProc();
    Pipe * fdRead;
    
    if((fdRead =proc->fd[fd]) == NULL){
        return 0;
    }

    //verificar si se puede escribir en el pipe
    while(fdRead->counter<=0){
        if(fdRead->writerRefs<=0) return 0;
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