#include <scheduler.h>

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

int createProcess(void * proc){
    //create stack and complete with values
    //create pcb
    //insert pcb into array mark as ready

}

void * schedule(void * rsp){
    
    while(scheduler.processes[scheduler.procIndex%scheduler.size].state!=READY){
        scheduler.procIndex++;
    }
    return scheduler.processes[scheduler.procIndex++].rsp;
}

void * fork(){
    
}




