#ifndef SCHEDULER_H
#define SCHEDULER_H

#include<pipes.h>
#include<list.h>

#define PROC_Q 200
#define SAVED_REGISTERS 15
#define RSI_POS 6
#define PROC_MEM 10*PAGE_SIZE
#define MAX_PIPES 10
#define BLOCKING_MOTIVES 5
#define MIN_TICKS 1
#define MAX_TICKS 10

typedef enum STATE{READY,BLOCKED,KILLED} STATE;

typedef struct  PCB {
    void * rsp;
    void * rbp;
    unsigned int priority;
    unsigned long pid;    
    STATE state;
    int currentTicks;
    Pipe * fd[MAX_PIPES];
    int role[MAX_PIPES];
   

} PCB;

typedef struct Motive{
    void * id;
    List * processes;
}Motive;

typedef struct Scheduler{
    PCB processes[PROC_Q];
    unsigned long procIndex;
    unsigned long size;
    unsigned char init;
    List * motives;
}Scheduler; 

void initialize_scheduler();
void * schedule(void * rsp);
unsigned long long createProcess(void * proc, int argc, char * argv[]);
void exit(int ret);
void ps(char * buffer);
PCB * getCurrentProc();
PCB * getProc(unsigned long pid);
int setProcFD(unsigned long pid,unsigned int fd, Pipe * pipe, unsigned int permission);
int blockMotive(void * id,unsigned long pid);
int awake(void * motiveId);
int awakeAll(void * motiveId);
int createMotive(void * id);
void closeMotive(void * id);
unsigned long long getPid();
void nice(unsigned long long pid, unsigned int priority);
void kill(unsigned  long long pid);
void block(unsigned  long long pid);
void unblock(unsigned  long long pid);
void wait(unsigned  long long pid);
void yield();

#endif
