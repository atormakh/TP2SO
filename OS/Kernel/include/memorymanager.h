#ifndef MEMORY_MANAGER_H
#define MEMORY_MANAGER_H
#include <memoryDefs.h>
#ifndef BUDDY


typedef struct MemoryManager{
    void * base;
    unsigned int * bitmap;
    size_t page_size;
    size_t pages_q;
    unsigned long long reserved;
}MemoryManager;

void * calc_ptr_from_idx(unsigned long block_idx);
unsigned long calc_idx_from_ptr(void * ptr);


#else


typedef struct BuddySystem{
    unsigned long long memSize;
    unsigned long long minPage;
    unsigned long long virtualSize;
    unsigned long long size;
    unsigned int levels;
    unsigned long long reserved;
    char * base;
}BuddySystem;

unsigned long long computeBuddySize(unsigned long long memSize, unsigned long long minPageSize);
unsigned int getLevel(unsigned long long size);
unsigned long long computeVirtualSize(unsigned long long memSize);
int getValue(unsigned int level, unsigned int offset);
void allocateBranch(unsigned int level, unsigned int offset);
void * getPtr(unsigned int level, unsigned int offset);
void freeBranch(unsigned int level, unsigned int offset);


#endif

void initialize_mem_man(void * memBase, size_t memSize, size_t minPageSize);
void * m_alloc( size_t size );
void * c_alloc(size_t size);
void m_free(void * ptr);
void memInfo(char * buffer);
#endif