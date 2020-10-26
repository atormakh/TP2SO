#ifndef BUILTIN_H
#define BUILTIN_H
void help();
void ps();
void kill(int argc, char * args[]);
void block(int argc, char * args[]);
void unblock(int argc, char * args[]);
void nice(int argc, char * args[]);
void pipes();
void sems();
void mem();
void man(int argc, char * args[]);
#endif