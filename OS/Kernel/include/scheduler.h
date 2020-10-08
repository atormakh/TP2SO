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

typedef enum STATE{READY,BLOCKED,KILLED} STATE;

typedef struct  PCB {
    void * rsp;
    void * rbp;
    unsigned long pid;
    STATE state;
    Pipe * fd[MAX_PIPES];
   

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
int createProcess(void * proc, int argc, char * argv[]);
void exit(int ret);
void ps(char * buffer);
PCB * getCurrentProc();
PCB * getProc(unsigned long pid);
int setProcFD(unsigned long pid,unsigned int fd, Pipe * pipe, unsigned int permission);
int block(void * id,unsigned long pid);
int awake(void * motiveId);
int awakeAll(void * motiveId);
int createMotive(void * id);
void closeMotive(void * id);

#endif
