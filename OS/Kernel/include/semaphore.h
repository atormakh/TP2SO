#ifndef SEMAPHORE_H
#define SEMAPHORE_H

typedef struct Sem{
    unsigned int value;
    int lock;       
}Sem;

Sem * createSem(int value);
void semPost(Sem * sem);
void semWait(Sem * sem);
void closeSem(Sem * sem);
void acquire(int * lock);
void release(int * lock);
#endif