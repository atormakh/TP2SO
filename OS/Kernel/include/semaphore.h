#ifndef SEMAPHORE_H
#define SEMAPHORE_H

typedef struct Sem{
    unsigned int value;
    unsigned int lock;       
}Sem;

Sem * createSem(int value);
void semPost(Sem * sem);
void semWait(Sem * sem);
void closeSem(Sem * sem);
#endif