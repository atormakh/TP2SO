#ifndef SCHEDULER_H
#define SCHEDULER_H

#define PROC_Q 200

typedef enum STATE{READY,BLOCKED,KILLED} STATE;


typedef struct  PCB {
    void * rsp;
    unsigned long pid;
    STATE state;
} PCB;

typedef struct Scheduler{
    PCB processes[PROC_Q];
    size_t procIndex;
    size_t size;
}Scheduler; 


void * schedule(void * rsp);
#endif