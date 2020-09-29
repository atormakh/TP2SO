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

void initialize_scheduler(){
    scheduler.procIndex=0;
    scheduler.size=0;
    scheduler.init=0;
}

int createProcess(void * proc, int argc, char * argv[]){
    
    // TODO recorrer para sobreescribir en el KILLED
    PCB * pcb = scheduler.processes + scheduler.size;
    
    pcb->state = READY;
    pcb->pid = scheduler.size;
    unsigned long long * bp;
    //unsigned long long * stack = bp = 0x700000; 
    
   ;
    char * tmp = m_alloc(PROC_MEM)+PROC_MEM-1; // TODO, BORRAR CUANDO ANDE

    unsigned long long * stack = bp = (unsigned long long *) tmp;
    
    pcb->rbp=bp;

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
    return 1;

}

void * schedule(void * rsp){
    
    if(scheduler.init!=0){
        scheduler.processes[scheduler.procIndex%scheduler.size].rsp=rsp;
        scheduler.procIndex++;
    }
    else scheduler.init =1;
   
    while(scheduler.processes[scheduler.procIndex%scheduler.size].state!=READY){
        scheduler.procIndex++;
    }
    return scheduler.processes[scheduler.procIndex%scheduler.size].rsp;
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
}






