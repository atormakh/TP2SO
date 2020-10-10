#include<shell.h>
#include "help.h"
#include "exceptionLauncher.h"
#include <stdlib.h>
#include "printmem.h"
#include "inforeg.h"
#include "sysInfo.h"
#include <syscalls.h>

void stdflush();


void yield();
unsigned long long counter = 0;
void do_nothing(){
    sys_sleep(5);
    sys_exit(0);
}

void dummy(){
    unsigned long long pid;
    sys_createProcess(do_nothing,0,0,&pid);
    return;
}

void ps(){
    char buffer[1024];
    sys_ps(buffer);
    puts(buffer);
}


char * messages[] = {"Command not found"};
char * commands[] = {"help","inforeg","printmem","cputemp", "cpuid", "localtime", "divisionbyzero", "invalidopcode", "ps", "dummy", 0};
void  (* run[])(int,char * * ) = {help, inforeg, printmem, cputemp,cpuid, localtime, divisionbyzero, invalidopcode,ps, dummy};

unsigned int inIndex=0;
char in[MAX_INPUT];

void inController(int c){
	
	if(c==128){
		while(inIndex>0){
			inController(8);
		}
	}
	else{
		if(c==8){
			if(inIndex>0){
				inIndex--;
				in[inIndex]=0;
			}
		}
		else{
			in[inIndex++] = c;
			in[inIndex]=0;
		}

	}
}


void shell(){
    int c;
    int read;
    while(1){
        puts("shell@convinux>");
        sys_readKeyboard(&c,1,&read);
		while(c !='\n'){
            
            inController(c);
            sys_readKeyboard(&c,1,&read);
		}
        
		exec(in);
        puts("\n");
        puts("shell@convinux>");
        //BORRAR
		//currentTab->offsetCurrent = currentTab->current+1;
		
        inIndex=0;
		in[0]=0;
	}

}


void exec(char * in){
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