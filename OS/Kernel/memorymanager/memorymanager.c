
#include <stdlib.h>
#include <memorymanager.h>
#include <video.h>

MemoryManager * mm;


void initialize_mem_man(void * memory, size_t ps, size_t qty){
    mm->bitmap=memory;
    memory += qty*sizeof(unsigned int);
    mm->base=memory;
    mm->page_size = ps;
    mm->pages_q = qty;
    for(int i=0; i < qty ;i++) mm->bitmap[i]=0;
    

    
}

void * m_alloc( size_t size ){

   
    unsigned int blocks_q = (size-1)/mm->page_size + 1;
    
    unsigned int i = 0;
    unsigned int cont = 0;
    
    while( cont < blocks_q && i < mm->pages_q){
        if(mm->bitmap[i]>0) {
            cont = 0;
            i+= mm->bitmap[i];    
        }else{
            cont++;
            i++;
        } 
        
        
    }
    
    if(cont == blocks_q){
        mm->bitmap[i-cont] = cont;
        return calc_ptr_from_idx(i-cont);
    }else{
        //setear erno no sirve aca creo
        //podemos imprimir un log
        return NULL;
    }
}

void m_free( void * ptr){
    unsigned int idx = calc_idx_from_ptr(ptr);
    mm->bitmap[idx] = FREE;    
}

void * calc_ptr_from_idx(unsigned int block_idx){
    return mm->base+block_idx*mm->page_size;
}

unsigned int calc_idx_from_ptr(void * ptr){
    return (unsigned int)(ptr-mm->base)/mm->page_size;
}