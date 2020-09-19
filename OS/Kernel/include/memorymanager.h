#ifndef MEMORY_MANAGER_H
#define MEMORY_MANAGER_H
#define K 1024
#define M 1048576
#define G 1073741824

#define PAGE_SIZE K

#define FREE 0
#define TAKEN 1

typedef struct MemoryManager{
    void * base;
    unsigned char * bitmap;
    size_t page_size;
    size_t pages_q;
}MemoryManager;

void initialize_mem_man( void * memory, size_t ps, size_t qty);
void * m_alloc( size_t size );
void * calc_ptr_from_idx(int block_idx);
int calc_idx_from_ptr(void * ptr);
void m_free(void * ptr);
#endif