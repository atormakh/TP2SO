#include "builtin.h"
#include <stdlib.h>
#include <shell.h>
#include <syscalls.h>

void help(){
    char ** builtin = getBuiltInCommands();
    char ** applications = getApplicationCommands();

   int cmd = 0;
   puts("Built-in commands:  \n");   
   while(builtin[cmd]!= NULL){
       printf("%s \n", builtin[cmd++]);
   }
   puts("\nApplication commands:  \n");  
   cmd=0; 
   while(applications[cmd]!= NULL){
       printf("%s \n", applications[cmd++]);
   }
}

void ps(){
    char buffer[1024];
    sys_ps(buffer);
    puts(buffer);
}

void kill(int argc, char * args[]){
    unsigned long long pid = stringToInt(args[0]);
    sys_kill(pid);
}

void block(int argc, char * args[]){
    unsigned long long pid = stringToInt(args[0]);
    sys_block(pid);
}

void unblock(int argc, char * args[]){
    unsigned long long pid = stringToInt(args[0]);
    sys_unblock(pid);
}

void nice(int argc, char * args[]){
    unsigned long long pid = stringToInt(args[0]);
    unsigned long long prio = stringToInt(args[1]);
    sys_nice(pid,prio);
}

void pipes(){
    char buffer[1024];
    sys_pipesInfo(buffer);
    puts(buffer);
}

void sems(){
    char buffer[1024];
    sys_semsInfo(buffer);
    puts(buffer);
}

void mem(){
    char buffer[1024];
    sys_memInfo(buffer);
    puts(buffer);
}
