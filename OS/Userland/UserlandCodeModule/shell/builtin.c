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
