#ifndef SEMAPHORE_H
#define SEMAPHORE_H

#define MAX_ID_SIZE

typedef struct Sem{
    unsigned int value;
    int lock;
    unsigned int refs; 
    char id[MAX_ID_SIZE];      
}Sem;

void initSemaphores();
void openSem(char * semId, int value);
void semPost(char * semId);
void semWait(char * semId);
void closeSem(char * semId);
void acquire(int * lock);
void release(int * lock);
void semsInfo(char * buffer);
#endif