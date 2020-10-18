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
    initSemaphores();
}

unsigned long long createProcess(void * proc, int argc, char * argv[]){
    
    // TODO recorrer para sobreescribir en el KILLED
    PCB * pcb = scheduler.processes + scheduler.size;
    
    pcb->state = BLOCKED;
    pcb->pid = scheduler.size;
    pcb->priority = 1;
    pcb->currentTicks = 1;
    unsigned long long * bp;

    unsigned long long * stack = bp = (unsigned long long *) (c_alloc(PROC_MEM)+PROC_MEM-1);
    
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
    scheduler.size++;


    PCB * current = getCurrentProc();

    for(int i=0;i<MAX_PIPES;i++){ 
        pcb->fd[i]=current->fd[i];
        pcb->role[i]=current->role[i];
        if(pcb->fd[i] != NULL){
            if(pcb->role[i] == WRITE) pcb->fd[i]->writerRefs++;
            else  pcb->fd[i]->readerRefs++;
        }
    }

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

void kill(unsigned long long pid){
    PCB * proc = getProc(pid);
    proc->state = KILLED;
    awakeAll(&proc->pid);
    closeMotive(&proc->pid);
    for(int i = 0; i<MAX_PIPES;i++){
        awakeAll(proc->fd[i]);
        closePipeProc(i,pid);
        
    }
    m_free(proc->rbp);
}

void unblock(unsigned long long pid){
    
    if(getProc(pid)->state != KILLED)getProc(pid)->state=READY;

}

void block(unsigned long long pid){
     if(getProc(pid)->state != KILLED)getProc(pid)->state=BLOCKED;
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
    
    for(int i = 0; i< scheduler.size;i++){
        buffer+=intToString(scheduler.processes[i].pid,buffer);
        *buffer++=' ';
        switch (scheduler.processes[i].state){
        case READY:
            buffer += strcpy(buffer,"RUNNING ");
            break;
        case KILLED:
            buffer += strcpy(buffer,"DEAD ");
            break;
        case BLOCKED:
            buffer += strcpy(buffer,"BLOCKED ");
            break;
        }
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
    return 1;
}




int createMotive(void * id){
    
    Motive * motive = get(scheduler.motives,(unsigned long long)id);
    if(motive!=NULL) return -1;
    
    motive = m_alloc(sizeof(Motive));
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


int awake(void * id){

    Motive * motive = get(scheduler.motives, (unsigned long long) id);
    PCB * proc = pop(motive->processes);
    if(proc==NULL) return -1;
    if(proc->state==KILLED) return -1;
    proc->state=READY;
    return proc->pid;
}

int awakeAll(void * id){  
    Motive * motive = get(scheduler.motives, (unsigned long long) id);
    if(motive == NULL) return 0;
    while(motive->processes->size>0){
        PCB * proc = ((PCB *)pop(motive->processes));
        if(proc->state!=KILLED)
            proc->state=READY;
    }
    return 1;
}





