#ifndef SHELL_H
#define SHELL_H

#define ARGS_LENGTH 5
#define MAX_INPUT 1024

int processInput(char * in, char ** args);
int checkCommand(char * command);
void exec(char * in);
void shell();

#endif