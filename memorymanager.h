#define K 1024
#define M 1048576
#define G 1073741824

#define PAGE_SIZE K

#define MEM_SIZE G
#define FREE 0
#define TAKEN 1

typedef struct MemoryManager{
    void * base;
    char * bitmap;
    size_t page_size;
    size_t pages_q;
}MemoryManager;

void initialize_mem_man(MemoryManager * mm, char * memory, char * bitmap, size_t ps, size_t qty);
void * alloc(MemoryManager * mm, void * ptr, size_t size );
void * calc_ptr_from_block(int block_idx, MemoryManager * mm);
void m_free(MemoryManager * mm, void * ptr, size_t size);
