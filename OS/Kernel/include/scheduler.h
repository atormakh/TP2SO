#ifndef SCHEDULER_H
#define SCHEDULER_H

#define PROC_Q 200
#define SAVED_REGISTERS 15
#define RSI_POS 6
#define PROC_MEM 5*PAGE_SIZE
typedef enum STATE{READY,BLOCKED,KILLED} STATE;


typedef struct  PCB {
    void * rsp;
    void * rbp;
    unsigned long pid;
    STATE state;

} PCB;

typedef struct Scheduler{
    PCB processes[PROC_Q];
    unsigned long procIndex;
    unsigned long size;
    unsigned char init;
}Scheduler; 

void initialize_scheduler();
void * schedule(void * rsp);
void * fork();
int createProcess(void * proc, int argc, char * argv[]);
void exit(int ret);

#endif