#include <stdlib.h>
#include <syscalls.h>

int loop(int argc, char * args[]){
    if(argc != 1){
        printf("error cantidad de parametros");
        sys_exit(0);
        return 0;
    }
    int seconds = stringToInt(args[0]);
    unsigned long long pid = sys_getPid();
    while(1){
        sys_sleep(seconds);
        printf("Hola! yo soy %d \n", pid);

    }
    sys_exit(0);
    return 0;
}
