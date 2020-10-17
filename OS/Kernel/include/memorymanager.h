#include <memoryDefs.h>
#ifndef BUDDY
#ifndef MEMORY_MANAGER_H
#define MEMORY_MANAGER_H

#define NULL 0

#define K 1024
#define M 1048576
#define G 1073741824

#define PAGE_SIZE K

#define FREE 0
#define TAKEN 1

typedef unsigned long size_t;

typedef struct MemoryManager{
    void * base;
    unsigned int * bitmap;
    size_t page_size;
    size_t pages_q;
}MemoryManager;

void initialize_mem_man( void * memory, size_t ps, size_t qty);
void * m_alloc( size_t size );
void * c_alloc(size_t size);
void * calc_ptr_from_idx(unsigned long block_idx);
unsigned long calc_idx_from_ptr(void * ptr);
void m_free(void * ptr);
#endif
#endif