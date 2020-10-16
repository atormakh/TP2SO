#ifndef BUDDY_SYSTEM_H
#define BUDDY_SYSTEM_H

typedef struct BuddySystem{
    unsigned long long memSize;
    unsigned long long minPage;
    unsigned long long virtualSize;
    unsigned long long size;
    unsigned int levels;
    char * base;
}BuddySystem;

void initializeBuddySystem(void * memBase, unsigned long long memSize, unsigned long long minPageSize);
unsigned long long computeBuddySize(unsigned long long memSize, unsigned long long minPageSize);
unsigned int getLevel(unsigned long long size);
unsigned long long computeVirtualSize(unsigned long long memSize);
int getValue(unsigned int level, unsigned int offset);
void * m_alloc(unsigned long size);
void allocateBranch(unsigned int level, unsigned int offset);
void allocateRecursivelyDown(unsigned int level, unsigned int offset);
void m_free(void * dir);

#endif

