#include <shell.h>
#include "builtin.h"
#include <stdlib.h>
#include <syscalls.h>
#include<applications.h>


unsigned long long counter = 0;


char * messages[] = {"Command not found", "Wrong number of arguments"};
char * builtin_commands[] = {"help", "ps", "kill",  "block", "unblock", "nice","pipe","sem","mem", 0};
char * application_commands[] = {"cat","filter","wc","loop","testSync", "testNoSync","testPrio", "testMm","phylo", 0};
void  (* builtin_run[])(int,char * * ) = {help,ps, kill, block, unblock, nice,pipes,sems,mem}; 
void * applications[]={cat,filter,wc,loop,test_sync, test_no_sync,test_prio, test_mm,phylo};     //faltan {phylo, test_m}
int waitingPids[MAX_PIPED_PROCS];

int inController(int c, char * in, int inIndex){
	
	if(c==128){
		while(inIndex>0){
			inIndex=inController(8,in,inIndex);
		}
	}
	else{
		if(c==8){
			if(inIndex>0){
				inIndex--;
				in[inIndex]=0;
                putchar(c);
			}
		}
		else{
			in[inIndex++] = c;
			in[inIndex]=0;
            putchar(c);
		}
       

	}
    return inIndex;
}


void shell(){
    unsigned int inIndex=0;
    char in[MAX_INPUT];
    char c;
    while(1){
        puts("shell@convinux>");
        sys_readKeyboard(&c,1);
		while(c !='\n'){
            inIndex=inController(c,in,inIndex);
            sys_readKeyboard(&c,1);
		}

        puts("\n");
		exec(in);
        puts("\n");

		
        inIndex=0;
		in[0]=0;
	}

}



void exec(char * in){
    ARGS cmds [MAX_PIPED_PROCS];
    int procs = processInput(in,cmds);
    int validCommand=0;

    //hola a b | algo
    //nos fijamos si el primer comando  es builtin,
    //en tal caso verificamos que no este intentando pipearlo ni correrlo en background
    if((validCommand=checkBuiltInCommand(cmds[0][COMMAND]))>=0){
        if(procs > 1 || (unsigned long long)cmds[0][((unsigned long long)cmds[0][ARG_C]+1)]=='&'){
            //se esta intentando pipear comando builtin
            
            puts(messages[E_PARAMETROS]);
            return;
        }
        builtin_run[validCommand]((unsigned long long)cmds[0][ARG_C], cmds[0]+ARG_V);
        return;
    }
    
    //verificamos que todos los comandos sean validos(ya sabiendo que no son builtin)
    for(int i=0;i<procs;i++){        
       validCommand=checkNoBuiltInCommand(cmds[i][COMMAND]);
        if(validCommand == -1){
            puts(messages[E_PARAMETROS]);
            return;
        }
        cmds[i][COMMAND]=(char *)(unsigned long long)validCommand;
    }
    
    int prevPid;
    int currentPid;
    int inputHandlerPid; 
    int j = 0;
    int needsWaiting=0;
    //2 loop 1 &
    
    prevPid=inputHandlerPid=sys_createProcess(shellInputHandler,"sh_input_handler",0,0);
    for(int i=0 ;i<procs;i++){
        needsWaiting=0;
        if((unsigned long long)cmds[i][(unsigned long long)(cmds[i][ARG_C])+1][0] != '&'){
            // hay que hacerle un wait
            needsWaiting=1;
        }else{
            cmds[i][ARG_C]--;
        }
        char ** procArgs=(char **)(cmds+i)+ARG_V;
        currentPid = sys_createProcess(applications[(unsigned long long)cmds[i][COMMAND]],
                                    application_commands[(unsigned long long)cmds[i][COMMAND]],
                                    (unsigned long long)cmds[0][ARG_C],
                                    procArgs);
        
        if(needsWaiting){
            waitingPids[j]=currentPid;
            j++;
        }
        
        if(i==0)
            sys_pipe(NULL,prevPid, WRITE+1, currentPid, READ);
        else //crear pipe
            sys_pipe(NULL,prevPid, WRITE, currentPid, READ);
        
        //desbloquear proceso anterior
        sys_unblock(prevPid);
        prevPid = currentPid;
    }
    sys_unblock(currentPid);
    waitingPids[j]=-1;
    for(int i = 0; i<j;i++) sys_wait(waitingPids[i]);
    sys_kill(inputHandlerPid);
        
}

// a b &| c | d e

//  a   2   b   &
//  c   0
//  d   1   e

int processInput(char * in, ARGS * procs){
    int processingWord;
    int count;
    int index = 0;
    int procIndex=0;
    int argIndex;

    while(in[index]){
        processingWord=0;
        count=0;
        argIndex=1;
        while(in[index] && in[index]!='|'){
            if(processingWord && in[index]==' '){
                processingWord=0;
                in[index]= 0;
            }else if(!processingWord && in[index] != ' '){
                procs[procIndex][argIndex]=in+index;
                argIndex++;
                count++;
                processingWord=1;
            }
            index++;
        }        
        procs[procIndex][0]=(char*)(unsigned long long)(count-1);
        if(in[index]=='|')index++;
        procIndex++;

    }
    return procIndex;
}


int checkBuiltInCommand(char * command){
    int cmp=1;
    int cmd = 0;
    while(builtin_commands[cmd] && cmp){
        cmp = strcmp(builtin_commands[cmd++],command);
    }

    return (cmp==0)?cmd-1:-1;

}

int checkNoBuiltInCommand(char * command){
    int cmp=1;
    int cmd = 0;
    while(application_commands[cmd] && cmp){
        cmp = strcmp(application_commands[cmd++],command);
    }

    return (cmp==0)?cmd-1:-1;

}

void shellInputHandler(){
    char c;
    char in[MAX_INPUT];
    int inIndex = 0;
    while(1){
        sys_readKeyboard(&c,1);

        while(c !='\n'){
            if(c==0){
                 sys_readKeyboard(&c,1);
                 if(c=='d'){
                     sys_exit(0);
                 }else if(c=='c'){
                     int i=0;
                     while(waitingPids[i]!=-1) sys_kill(waitingPids[i++]);
                 }
            }else{
                inIndex=inController(c,in,inIndex);
                sys_readKeyboard(&c,1);
            }
            
        }
        putchar('\n');
        in[inIndex++]='\n';
        sys_writePipe(WRITE+1,in,inIndex);
        inIndex=0;
    }
}