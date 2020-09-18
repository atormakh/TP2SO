#include<stdlib.h>


void printmem(int argc, char * args[]){
    if(argc != 1){
        puts("cantidad de argumentos invalida");
        return;
    }
    unsigned char * pointer =(unsigned char *)atoi(args[0]);
    for(int i=0; i<32; i++){
        printf("%h ",pointer[i]);
    }
}