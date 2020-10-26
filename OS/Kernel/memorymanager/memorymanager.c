#include <memorymanager.h>
#ifndef BUDDY
#include <lib.h>
#include <scheduler.h>
#include <video.h>

MemoryManager mm;

void initialize_mem_man(void * memory, size_t memSize, size_t ps){

    size_t qty= (memSize-(unsigned long long)memory)/ps;
    
    mm.bitmap=memory;
    memory =(char *)memory+ qty*sizeof(unsigned int);
    mm.base=memory;
    mm.page_size = ps;
    mm.pages_q = qty;
    mm.reserved=0;
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
        mm.reserved+=cont;
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
    if(ptr == NULL) return;
    unsigned long idx = calc_idx_from_ptr(ptr);
    mm.reserved-=mm.bitmap[idx]; 
    mm.bitmap[idx] = FREE;
   
    
}

void * calc_ptr_from_idx(unsigned long block_idx){
    return mm.base+block_idx*mm.page_size;
}

unsigned long calc_idx_from_ptr(void * ptr){
    return (unsigned long)(ptr-mm.base)/mm.page_size;
}

void memInfo(char * buffer){
    buffer+=strcpy(buffer, "Memory state \n");
    buffer+=strcpy(buffer, "Base Address: ");
    buffer+=intToString((unsigned long long) mm.base,buffer);
    *buffer++='\n';
    buffer+=strcpy(buffer, "Memory Size: ");
    buffer+=intToString(mm.page_size*mm.pages_q/M,buffer);
    buffer+=strcpy(buffer, " MB");
    *buffer++='\n';
    buffer+=strcpy(buffer, "Overhead: ");
    buffer+=intToString(mm.pages_q*sizeof(unsigned int)/K,buffer);
    buffer+=strcpy(buffer, " KB");
    *buffer++='\n';
    buffer+=strcpy(buffer, "Page size: ");
    buffer+=intToString(mm.page_size/K,buffer);
    buffer+=strcpy(buffer, " KB");
    *buffer++='\n';
    buffer+=strcpy(buffer, "Page quantity: ");
    buffer+=intToString(mm.pages_q,buffer);
    *buffer++='\n';
    buffer+=strcpy(buffer, "Reserved Pages: ");
    buffer+=intToString(mm.reserved,buffer);
    *buffer++='\n';
    buffer+=strcpy(buffer, "Free Pages: ");
    buffer+=intToString(mm.pages_q-mm.reserved,buffer);
    *buffer++='\n';
    *buffer++=0;
    

}
#endif