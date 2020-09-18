#include <stdint.h>
#include "inforeg.h"
#include <stdlib.h>
#include <syscalls.h>

char * registersNames[] = {"r15","r14","r13","r12","r11","r10","r9","r8","rsi","rdi",\
                            "rbp","rdx","rcx","rbx","rax","rsp","rip","cs","flags"};

void inforeg( int argc, char * args[]){
    int flag;
    registerArgs reg;
    sys_retrieveReg(&reg, &flag);
    if(flag == 0){
        puts("No se pidio un guardado de registros. Con la tecla CTRL izquierdo puede hacerlo. \n");
        return;
    }
    
    uint64_t * registers =(uint64_t *)&reg;
    puts("registros \n");
        
    for(int i = 0 ; i < REGISTERS ; i++){
        
        printf("%s   %h\n",registersNames[i],registers[i]);
    
    }

    
    

}