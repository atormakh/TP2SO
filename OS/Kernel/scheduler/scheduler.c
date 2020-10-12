#include <scheduler.h>
#include <memorymanager.h>
#include <syscalls.h>
#include <video.h>
#include <lib.h>

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

long motive_cmp(void * motive1, void * motive2){
    return (long)(((Motive *)motive1)->id - ((Motive *)motive2)->id);
}

long proc_cmp(void * proc1, void * proc2){
    return *(unsigned long *)proc1-*(unsigned long *)proc2;
}

void initialize_scheduler(){
    scheduler.motives=newList(motive_cmp);
    scheduler.procIndex=0;
    scheduler.size=0;
    scheduler.init=0;
}

unsigned long long createProcess(void * proc, int argc, char * argv[]){
    
    // TODO recorrer para sobreescribir en el KILLED
    PCB * pcb = scheduler.processes + scheduler.size;
    
    pcb->state = READY;
    pcb->pid = scheduler.size;
    unsigned long long * bp;
    //unsigned long long * stack = bp = 0x700000; 
    
   
    char * tmp = c_alloc(PROC_MEM)+PROC_MEM-1; // TODO, BORRAR CUANDO ANDE

    unsigned long long * stack = bp = (unsigned long long *) tmp;
    
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
    return pcb->pid;

}

void * schedule(void * rsp){
    //writePipe(1,"hola\n",6);
    if(scheduler.init!=0){
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
        if(scheduler.processes[i].state!=KILLED && scheduler.processes[i].pid == pid)
            return scheduler.processes+i;
    }
    return 0;
}

void exit(int ret){
    PCB * proc = scheduler.processes + scheduler.procIndex%scheduler.size;
    proc->state = KILLED;
    m_free(proc->rbp);
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
    return 0;
}


int block(void * id,unsigned long pid){
    Motive searcher={id,NULL};    
    Motive * motive = get(scheduler.motives, &searcher);
    if(motive == NULL){
        return 0;
    }
    PCB * proc = getProc(pid);
    push(motive->processes,proc);
    proc->state=BLOCKED;
    return 1;
}




int createMotive(void * id){
    Motive * motive = m_alloc(sizeof(Motive));
    if(motive == NULL){
        //error
        return -1;
    }
    motive->id=id;
    motive->processes = newList(proc_cmp);
    push(scheduler.motives, motive);
    return 0;
}

void closeMotive(void * id){
    Motive searcher={id,NULL};    
    Motive * motive = get(scheduler.motives, &searcher);
    if(motive==NULL) return;
    remove(scheduler.motives, &searcher);
    
    freeList(motive->processes);
    m_free(motive);
}


int awake(void * id){
    Motive searcher={id,NULL};
    
    Motive * motive = get(scheduler.motives, &searcher);
    PCB * proc = pop(motive->processes);
    if(proc==NULL) return -1;
    proc->state=READY;
    // unsigned long pid=*(unsigned long *)pop(motive->processes);
    // getProc(pid)->state=READY;
    // if(size(motive->processes)==0){
    //     remove(scheduler.motives,motive);
    //     free(motive);
    // }
    return proc->pid;
}

int awakeAll(void * id){
    Motive searcher={id,NULL};    
    Motive * motive = get(scheduler.motives, &searcher);
    if(motive == NULL) return 0;
    while(motive->processes->size>0){
        ((PCB *)pop(motive->processes))->state=READY;
    }
}





