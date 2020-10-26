#include <scheduler.h>
#include <memorymanager.h>
#include <syscalls.h>
#include <video.h>
#include <lib.h>
#include <semaphore.h>

/*
.Crear y finalizar procesos.
● Obtener el ID del proceso que llama.
● Listar procesos.
● Matar un proceso.
● Modificar la prioridad de un proceso.
● Cambiar el estado de un proceso.
● Renunciar al CPU
*/

//pipe(pid1,fd_write, pid2, fd_read)
//close(fd)

Scheduler scheduler;

void initialize_scheduler(){
    scheduler.motives=newList();
    scheduler.procIndex=0;
    scheduler.size=0;
    scheduler.init=0;
    scheduler.nextPid=0;
    initSemaphores();
}

unsigned long long createProcess(void * proc, char * name, unsigned int argc, char * argv[]){
    
    // TODO recorrer para sobreescribir en el KILLED
    PCB * pcb;
    if(scheduler.size < PROC_Q)scheduler.processes[scheduler.size].state=KILLED;
    int found = 0;
    int i;
    for(i=0; i<=scheduler.size && !found;i++){
        found = (scheduler.processes[i].state == KILLED);
    }
    if(!found) return NULL;
    pcb = scheduler.processes + i - 1;
    if(i==scheduler.size + 1)scheduler.size++;

    pcb->state = BLOCKED;
    pcb->pid = scheduler.nextPid++;
    pcb->priority = 1;
    pcb->currentTicks = 1;
    unsigned long long * bp;

    char * memoryAddress = (char *)c_alloc(PROC_MEM);
    unsigned long long * stack = bp = (unsigned long long *) (memoryAddress+PROC_MEM-1);
    pcb->memoryAddress=memoryAddress;
    
    char ** argvCopy =c_alloc((argc+1)*sizeof(char *));
    char * args = c_alloc((argc+1)*ARGS_LENGTH);
    char * index = args;
    argvCopy[0]=args;

    //copias todo y argv
    for(i=0; i <argc;i++){
        int qty=(int)strcpy(index,argv[i]);
        argvCopy[i]=index;
        index+=qty+1;
    }
    argv=argvCopy;
    pcb->argv=argv;

    pcb->rbp=bp;

    stack --;
    *stack = 0x0;
    stack --;
    *stack = (unsigned long long)bp;
    stack --;
    *stack=0x202;
    stack --;
    *stack = 0x8;
    stack --;
    *stack= (unsigned long long)proc;
    stack -= RSI_POS;
    *stack = argc;
    stack --;
    *stack = (unsigned long long) argv;
    stack -= (SAVED_REGISTERS-RSI_POS-1);
    
    
    // | SS	    | -> 0x0
    // | RSP	| -> BP
    // | RFLAGS | -> 0x202
    // | CS	    | -> 0x8
    // | RIP	| -> main
    // | rax	| -> 0
    // | rbx	| -> 0
    // | rcx	| -> 0
    // | ...	| -> 0
    // | rdi	| -> argc
    // | ...	| -> 0
    // | rsi	| -> (rsi) argv
    // | ...	| -> 0
    //create stack and complete with values
    pcb->rsp = (void *)stack;




    PCB * current = getCurrentProc();

    for(i=0;i<MAX_PIPES;i++){ 
        pcb->fd[i]=current->fd[i];
        pcb->role[i]=current->role[i];
        if(pcb->fd[i] != NULL){
            if(pcb->role[i] == WRITE) pcb->fd[i]->writerRefs++;
            else  pcb->fd[i]->readerRefs++;
        }
    }
    pcb->blockingRefs=1;
    pcb->name=name;
    return pcb->pid;

}

void * schedule(void * rsp){
    if(scheduler.init!=0){

        if(getCurrentProc()->currentTicks<getCurrentProc()->priority){
            getCurrentProc()->currentTicks++;
            return rsp;
        }
        getCurrentProc()->currentTicks=1;

        scheduler.processes[scheduler.procIndex%scheduler.size].rsp=rsp;
        scheduler.procIndex++;
    }
    else scheduler.init =1;
   
    while(scheduler.processes[scheduler.procIndex%scheduler.size].state!=READY){
        scheduler.procIndex++;
    }
    scheduler.procIndex = scheduler.procIndex%scheduler.size;
    return scheduler.processes[scheduler.procIndex%scheduler.size].rsp;
   
}

PCB * getCurrentProc(){
    return scheduler.processes + scheduler.procIndex%scheduler.size;
}
PCB * getProc(unsigned long pid){
    for(int i = 0; i<scheduler.size;i++){
        if(scheduler.processes[i].pid == pid)
            return scheduler.processes+i;
    }
    return 0;
}

int kill(unsigned long long pid){
    PCB * proc = getProc(pid);
    if(proc->state == KILLED) return -1;
    proc->state = KILLED;
    awakeAll(&proc->pid);
    closeMotive(&proc->pid);
    for(int i = 0; i<MAX_PIPES;i++){
        awakeAll(proc->fd[i]);
        closePipeProc(i,pid);
        
    }

    m_free(proc->argv[0]);
    m_free(proc->argv);
    m_free(proc->memoryAddress);

    return 1;
}

int unblock(unsigned long long pid){
    
    if(getProc(pid)->state != KILLED){
        getProc(pid)->state=READY;
        getProc(pid)->blockingRefs--;
        return 1;
    }
    else return -1;



}

int block(unsigned long long pid){
     if(getProc(pid)->state != KILLED){
        getProc(pid)->state=BLOCKED;
        getProc(pid)->blockingRefs++;
        return 1;
     }
     else return -1;
}

unsigned long long getPid(){
    return getCurrentProc()->pid;
}


void nice(unsigned long long pid, unsigned int priority){
    if(priority<MIN_TICKS || priority > MAX_TICKS) return;
    getProc(pid)->priority=priority;
}

void exit(int ret){
    kill(getCurrentProc()->pid);
    getCurrentProc()->currentTicks=getCurrentProc()->priority;
    yield();
}

void wait(unsigned  long long pid){
    PCB * proc = getProc(pid);
    if(proc->state==KILLED) return;
    createMotive(&proc->pid);
    blockMotive(&proc->pid,getCurrentProc()->pid);
    
    yield();
}

void ps(char * buffer){   
    buffer+=strcpy(buffer, "PID | prio |   rbp   |   rsp   |  status  | name \n");
    PCB  proc;
    for(int i = 0; i< scheduler.size;i++){
        proc = scheduler.processes[i];
        buffer+=intToString(proc.pid,buffer);
        buffer += strcpy(buffer, "    ");
        
        buffer += intToString( proc.priority,buffer);
        buffer += strcpy(buffer, "    ");
        buffer += intToString((unsigned long long)proc.rbp, buffer);
        buffer += strcpy(buffer, "    ");
        buffer += intToString((unsigned long long)proc.rsp, buffer);
        buffer += strcpy(buffer, "    ");
        if(proc.pid == getCurrentProc()->pid){
            buffer += strcpy(buffer,"RUNNING ");
        }
        else{
            switch (proc.state){
            case READY:
                buffer += strcpy(buffer,"READY   ");
                break;
            case KILLED:
                buffer += strcpy(buffer,"DEAD    ");
                break;
            case BLOCKED:
                buffer += strcpy(buffer,"BLOCKED ");
                break;
            }
        }
        buffer += strcpy(buffer, "    ");
        buffer += strcpy(buffer, proc.name);        
        *buffer++='\n';
        
       
    }
    *buffer++=0;
}


int setProcFD(unsigned long pid,unsigned int fd, Pipe * pipe, unsigned int permission){
    PCB * proc = getProc(pid);
    proc->fd[fd]=pipe;
    proc->role[fd]=permission;
    return 0;
}


int blockMotive(void * id,unsigned long pid){
    if(getProc(pid)->state == KILLED) return 0; 
    Motive * motive = get(scheduler.motives, (unsigned long long) id);
    if(motive == NULL){
        return 0;
    }
    PCB * proc = getProc(pid);
    push(motive->processes,proc,proc->pid);
    proc->state=BLOCKED;
    proc->blockingRefs++;
    return 1;
}




int createMotive(void * id){
    
    Motive * motive = get(scheduler.motives,(unsigned long long)id);
    if(motive!=NULL) return -1;
    
    motive = c_alloc(sizeof(Motive));
    if(motive == NULL){
        //error
        return -1;
    }
    motive->id=id;
    motive->processes = newList();
    push(scheduler.motives, motive,(unsigned long long)id);
    return 0;
}

void closeMotive(void * id){
    Motive * motive = get(scheduler.motives, (unsigned long long) id);
    if(motive==NULL) return;
    remove(scheduler.motives, (unsigned long long) id);
    
    freeList(motive->processes);
    m_free(motive);
}

List * getMotive(void * id){
    Motive * motive = get(scheduler.motives, (unsigned long long) id);
    if(motive == NULL) return NULL;
    return motive->processes;
}

int printProcsInMotive(char * buffer, List * procs){
     int n = 0;
     if(procs==NULL || procs->size==0){ 
        n=(int)strcpy(buffer,"none");
        return n;
     }
    iterator(procs);
   
    while(hasNext(procs)){
        int pid = ((PCB *)getNext(procs))->pid;
        n+= intToString(pid, buffer+n);
        n+= strcpy(buffer+n, "  ");
    }
    return n;        

}


int awake(void * id){

    Motive * motive = get(scheduler.motives, (unsigned long long) id);
    PCB * proc = pop(motive->processes);
    if(proc==NULL) return -1;
    if(proc->state==KILLED) return -1;
    proc->blockingRefs--;
    if(proc->blockingRefs <=0) proc->blockingRefs=0;
    if(proc->blockingRefs==0)proc->state=READY;
    return proc->pid;
}

int awakeAll(void * id){  
    Motive * motive = get(scheduler.motives, (unsigned long long) id);
    if(motive == NULL) return 0;
    while(motive->processes->size>0){
        PCB * proc = ((PCB *)pop(motive->processes));
        if(proc->state!=KILLED){
            proc->blockingRefs--;
            if(proc->blockingRefs <=0) proc->blockingRefs=0;
            if(proc->blockingRefs==0)proc->state=READY;
        }
    }
    return 1;
}





