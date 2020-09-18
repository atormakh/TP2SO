#include<syscalls.h>
#include "sysInfo.h"
#include<stdlib.h>


char buffer[32];

void getcpuid(char * out);

void cpuid(int argc,char * args[]){
    getcpuid(buffer);
    puts(buffer);
}

void localtime(int argc, char * args[]){
    sys_localtime(buffer);
    puts(buffer);
}

void cputemp( int argc, char * args[]){
    int res=0;
    sys_cputemp(&res);
    printf("%d *C",res);
}
