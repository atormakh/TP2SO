#include <stdlib.h>
#include <syscalls.h>

#define MAX_SIZE 1024



int cat(){
    char buffer[MAX_SIZE];
    int count;
    while((count=sys_readPipe(0,buffer,MAX_SIZE-1))){
        buffer[count]=0;
        puts(buffer);
    }
    sys_exit(0);
    return 0;
}