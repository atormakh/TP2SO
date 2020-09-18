#include "help.h"
#include <stdlib.h>
void help(int argc, char * args[]){
   puts(
        "available commands: \n"
        "    help \n"
        "    inforeg \n"
        "    cpuid \n"
        "    cputemp \n"
        "    printmem \n"
        "    divisionbyzero \n"
        "    invalidopcode \n"
        "    localtime \n"
        "for more information you can use google \n"
    );
}