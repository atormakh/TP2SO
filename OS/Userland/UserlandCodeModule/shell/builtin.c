#include "builtin.h"
#include <stdlib.h>
#include <syscalls.h>
void help(int argc, char * args[]){
   puts(
        "available commands: \n"
        "    help \n"
        "    ps \n"
        "for more information you can use google \n"
    );
}

void ps(){
    char buffer[1024];
    sys_ps(buffer);
    puts(buffer);
}