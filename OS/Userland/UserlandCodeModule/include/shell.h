#ifndef SHELL_H
#define SHELL_H

#define ARGS_LENGTH 5

int processInput(char * in, char ** args);
int checkCommand(char * command);
void shell(char * in);

#endif