#include<shell.h>
#include "help.h"
#include "exceptionLauncher.h"
#include <stdlib.h>
#include "printmem.h"
#include "inforeg.h"
#include "sysInfo.h"

char * messages[] = {"Command not found"};
char * commands[] = {"help","inforeg","printmem","cputemp", "cpuid", "localtime", "divisionbyzero", "invalidopcode", 0};
void  (* run[])(int,char * * ) = {help, inforeg, printmem, cputemp,cpuid, localtime, divisionbyzero, invalidopcode};

void shell(char * in){
    char * args[ARGS_LENGTH];
    int argc = processInput(in,args);
    // strcpy(out, args[1]);
    // return;
    int command = checkCommand(args[0]);
    if(command == -1){
        puts(messages[0]);
    }
    else
        run[command](argc-1,args+1);
}

int processInput(char * in, char ** args){
    int processingWord=0;
    int count=0;
    while(*in){
        if(processingWord && *in==' '){
            processingWord=0;
            *in= 0;
        }else if(!processingWord && *in != ' '){
            *args++=in;
            count++;
            processingWord=1;
        }
        in++;
        
    }
    return count;
}

int checkCommand(char * command){
    int cmp=1;
    int cmd = 0;
    while(commands[cmd] && cmp){
        cmp = strcmp(commands[cmd++],command);
    }

    return (cmp==0)?cmd-1:-1;

}