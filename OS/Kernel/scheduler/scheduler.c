#include <scheduler.h>
#include <memorymanager.h>
#include <video.h>

/*
.Crear y finalizar procesos.
● Obtener el ID del proceso que llama.
● Listar procesos.
● Matar un proceso.
● Modificar la prioridad de un proceso.
● Cambiar el estado de un proceso.
● Renunciar al CPU
*/
Scheduler scheduler;

void initialize_scheduler(){
    scheduler.procIndex=0;
    scheduler.size=0;
}

int createProcess(void * proc, int argc, char * argv[]){
    

    // TODO recorrer para sobreescribir en el KILLED
    PCB * pcb = scheduler.processes + scheduler.size;
    
    pcb->state = READY;
    pcb->pid = scheduler.size;
    unsigned long long * bp;
    //unsigned long long * stack = bp = 0x700000; 
    
    unsigned long long * stack = bp = (unsigned long long *)m_alloc(PAGE_SIZE)+PAGE_SIZE;
    
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
    static int i;
    if(i++>0)scheduler.processes[scheduler.procIndex++%scheduler.size].rsp=rsp;

    while(scheduler.processes[scheduler.procIndex%scheduler.size].state!=READY){
        scheduler.procIndex++;
    }
    return scheduler.processes[scheduler.procIndex%scheduler.size].rsp;
}

void * fork(){
    return 0;
}




