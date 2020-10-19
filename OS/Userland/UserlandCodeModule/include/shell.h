#ifndef SHELL_H
#define SHELL_H

#define ARGS_LENGTH 5
#define MAX_INPUT 1024
#define MAX_PIPED_PROCS 10
#define COMMAND 1
#define ARG_C 0
#define ARG_V 2
#define WRITE 1
#define READ 0
#define E_PARAMETROS 1


typedef char * ARGS[ARGS_LENGTH];

int processInput(char * in, ARGS comands[]);
int checkBuiltInCommand(char * command);
int checkNoBuiltInCommand(char * command);
void exec(char * in);
void shellInputHandler();
void shell();
char ** getApplicationCommands();
char ** getBuiltInCommands();

#endif