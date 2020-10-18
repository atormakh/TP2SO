#include <stdlib.h>
#include <syscalls.h>

#define MAX_SIZE 1024

int wc(){

    char buffer[MAX_SIZE];
    int count;
    int counter=0;
    while((count=sys_readPipe(0,buffer,MAX_SIZE-1))){
        buffer[count]=0;
        for(int i=0;i<count;i++) if(buffer[i]=='\n') counter++;
    }
    printf("input has %d lines\n", counter);

    sys_exit(0);
    return 0;
}