#include "builtin.h"
#include <stdlib.h>
#include <shell.h>
#include <syscalls.h>

void help(){
    char ** builtin = getBuiltInCommands();
    char ** applications = getApplicationCommands();

   int cmd = 0;
   puts("Built-in commands:  \n");   
   while(builtin[cmd]!= NULL){
       printf("%s \n", builtin[cmd++]);
   }
   puts("\nApplication commands:  \n");  
   cmd=0; 
   while(applications[cmd]!= NULL){
       printf("%s \n", applications[cmd++]);
   }
   puts("Con ctrl+C se hace un kill de los procesos en foreground \n");
    puts("Con ctrl+D se cierra el STDIN del proceso en foreground pasandolo a background \n");
//    puts("Para pasar parametros por linea de comandos separarlos por espacio, mismo si se quiere pipear o correr en background algun proceso. \n");
//    puts("Ejemplo: phylo & corre el proceso phylo en background \n");
//    puts("Ejemplo: phylo | filter se pipea el out de phlo al in de filter \n");
   puts("Correr : man nombreComando para mas informacion sobre el mismo\n");
 
}

char * builtin_descriptions[]={
    "Muestra todos los comandos disponibles", //help
    "Muestra todos los procesos, si algun proceso esta muerto y todavía no fue sobreescrito, lo mostrará como DEAD", //ps
    "kill <PID> mata al proceso de identificador PID", //kill
    "block <PID> bloquea al proceso de identificador PID hasta ser desbloqueado por el comando unblock (consultar man unblock)", //block
    "unblock <PID> desbloquea al proceso de identificador PID. Si el proceso no estaba bloqueado no hace nada. Para bloquear un proceso consultar man block", //unblock
    "nice <PID> <PRIORITY> donde el pid debe ser un pid de un proceso VIVO y la priority debe ser un numero del 1 al 10, siendo 10 la prioridad mas alta", //nice
    "Muestra el estado de todos los pipes", //pipe
    "Muestra el estado actual de todos los semaforos", //sem
    "Muestra el estado actual de la memoria", //mem
};
char * application_descriptions[]={
    "Escribe lo que recibe por el STDIN en el STDOUT, recordar que si se escribe por TECLADO, solo se escribe en el STDIN al apretar ENTER", //cat
    "Escribe lo que recibe por el STDIN en el STDOUT, filtrando las vocales, recordar que si se escribe por TECLADO, solo se escribe en el STDIN al apretar ENTER", //filter
    "Cuenta la cantidad de lineas recibidas por el STDIN, solo imprime el resultado luego de cerrar el STDIN. En la shell se puede cerrar el STDIN con ctrl+d", //wc
    "loop <time> Imprime un saludo cada TIME segundos sobre el STDOUT ", //loop
    "CCorre test de sincronizacion con semaforos provisto por la catedra. Recordar que solo es relevante el ultimo resultado y que nuestro printf imprime el valor como un unsigned.", //testSync
    "Corre test de sincronizacion sin semaforos", //testNoSync
    "Corre test de prioridades provisto por la catedra", //testPrio
    "Corre test de memory manager provisto por la catedra Para finalizar el proceso debe matarse con el comando kill. Consultar man kill", //testMm
    "Corre test de procesos provisto por la catedra. Para finalizar el proceso debe matarse con el comando kill. Consultar man kill", //testProc
    "Ejecuta un proceso que simula el problema de los dining philosophers. Si el proceso se corre en foreground pueden agregarse filosofos (a) o quitarse (r) durante la ejecucion, recordar que si se escribe por TECLADO, solo se escribe en el STDIN al apretar ENTER", //phylo
};

void man(int argc, char * args[]){
    if(argc != 1){
        puts("Command not found");
        return;
    }
    int validCommand=checkBuiltInCommand(args[0]);
    if(validCommand>=0){
        puts(builtin_descriptions[validCommand]); 
        puts("\n");
        return;
    }else{
        validCommand=checkNoBuiltInCommand(args[0]);
        if(validCommand>=0){
            puts(application_descriptions[validCommand]); 
            puts("\n");
            return;
        }else{
            puts("Command not found");
        }
    }
   
}


void ps(){
    char buffer[1024];
    sys_ps(buffer);
    puts(buffer);
}

void kill(int argc, char * args[]){
    unsigned long long pid = stringToInt(args[0]);
    sys_kill(pid);
}

void block(int argc, char * args[]){
    unsigned long long pid = stringToInt(args[0]);
    sys_block(pid);
}

void unblock(int argc, char * args[]){
    unsigned long long pid = stringToInt(args[0]);
    sys_unblock(pid);
}

void nice(int argc, char * args[]){
    unsigned long long pid = stringToInt(args[0]);
    unsigned long long prio = stringToInt(args[1]);
    sys_nice(pid,prio);
}

void pipes(){
    char buffer[1024];
    sys_pipesInfo(buffer);
    puts(buffer);
}

void sems(){
    char buffer[1024];
    sys_semsInfo(buffer);
    puts(buffer);
}

void mem(){
    char buffer[1024];
    sys_memInfo(buffer);
    puts(buffer);
}
