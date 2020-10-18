#include <memorymanager.h>
#ifndef BUDDY
#include <lib.h>
#include <video.h>

MemoryManager mm;



void initialize_mem_man(void * memory, size_t ps, size_t qty){
    mm.bitmap=memory;
    memory += qty*sizeof(unsigned int);
    mm.base=memory;
    mm.page_size = ps;
    mm.pages_q = qty;
    for(unsigned long i=0; i < qty ;i++) mm.bitmap[i]=0;
    

    
}

void * m_alloc( size_t size ){
    
   
    unsigned long blocks_q = (size-1)/mm.page_size + 1;
    
    unsigned long i = 0;
    unsigned long cont = 0;
    
    while( cont < blocks_q && i < mm.pages_q){
        if(mm.bitmap[i]>0) {
            cont = 0;
            i+= mm.bitmap[i];    
        }else{
            cont++;
            i++;
        } 
        
        
    }

    
    if(cont == blocks_q){
        mm.bitmap[i-cont] = cont;
        return calc_ptr_from_idx(i-cont);
    }else{
        //setear erno no sirve aca creo
        //podemos imprimir un log
        return NULL;
    }
}

void * c_alloc(size_t size){
    unsigned long long * toRet= m_alloc(size);
    
    if(toRet == NULL){
        return NULL;
    }
    int aux=sizeof(long long);
    int i=0;
    for(i = 0; i<size/aux;i++){
        *(toRet + i)=0;
    }

    for(int j = 0; j<size%aux;j++){
        *((char *)toRet + i+j*aux)=0;
    }

    return toRet;
}

void m_free( void * ptr){
    unsigned long idx = calc_idx_from_ptr(ptr);
    mm.bitmap[idx] = FREE;    
}

void * calc_ptr_from_idx(unsigned long block_idx){
    return mm.base+block_idx*mm.page_size;
}

unsigned long calc_idx_from_ptr(void * ptr){
    return (unsigned long)(ptr-mm.base)/mm.page_size;
}
#endif