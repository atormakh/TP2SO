#define K 1024
#define M 1048576
#define G 1073741824

#define PAGE_SIZE 5

#define MEM_SIZE 100
#define FREE 0
#define TAKEN 1

typedef struct MemoryManager{
    void * base;
    unsigned char * bitmap;
    size_t page_size;
    size_t pages_q;
}MemoryManager;

void initialize_mem_man(MemoryManager * mm, char * memory, unsigned char * bitmap, size_t ps, size_t qty);
void * m_alloc(MemoryManager * mm, size_t size );
void * calc_ptr_from_idx(int block_idx, MemoryManager * mm);
int calc_idx_from_ptr(void * ptr, MemoryManager * mm);
void m_free(MemoryManager * mm, void * ptr);
