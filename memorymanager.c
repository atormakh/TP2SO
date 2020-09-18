#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "memorymanager.h"



char memory[MEM_SIZE]={0};
char bitmap [MEM_SIZE/PAGE_SIZE]={0};

void printbitmap(){
    for(int i = 0; i<MEM_SIZE/PAGE_SIZE; i++){
        printf("%d ",bitmap[i]);
    }
    printf("\n");
}

int main(int argc, int * args[]){
    MemoryManager mm;
    initialize_mem_man(&mm, memory, bitmap,PAGE_SIZE, MEM_SIZE/PAGE_SIZE);
    char * buffer = (char *)m_alloc(&mm,40);
    char * buffer2 = (char *)m_alloc(&mm,30);
    char * buffer3 = (char *)m_alloc(&mm,6);
    printbitmap();
    strcpy(buffer,"hola cami");
    puts(buffer);
    m_free(&mm,buffer);
    printbitmap();
}

void initialize_mem_man(MemoryManager * mm, char * memory, unsigned char * bitmap, size_t ps, size_t qty){
    mm->base=memory;
    mm->bitmap=bitmap;
    mm->page_size = ps;
    mm->pages_q = qty;
}

void * m_alloc(MemoryManager * mm, size_t size ){

    
    uint blocks_q = (size-1)/mm->page_size + 1;
    
    int i = 0;
    int cont = 0;

    //  5 bloques
    
    //  ---------------------------------
    //  malloc(1)
    
    //  bitmap: 	1 0 0 0 0
    //  bloques:	1 0 0 0 0
    
    //  ---------------------------------
    //  malloc(2)
    
    //  bitmap:	    1 1 1 0 0
    //  bloques:	1 2 0 0 0
    
 
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
        if(cont > i){
            //tirar error, nos equivocamos en algo
        }
        mm->bitmap[i-cont] = cont;
        // for(int j=i-cont;j<i;j++){
        //     mm->bitmap[j] = TAKEN;
        // }
        return calc_ptr_from_idx(i-cont, mm);
    }else{
        //setear erno no sirve aca creo
        //podemos imprimir un log
        return NULL;
    }
    
    
    
    
    
}

void m_free(MemoryManager * mm, void * ptr){
    int idx = calc_idx_from_ptr(ptr, mm);
    mm->bitmap[idx] = FREE;    
}

void * calc_ptr_from_idx(int block_idx, MemoryManager * mm){
    return mm->base+block_idx*mm->page_size;
}

int calc_idx_from_ptr(void * ptr, MemoryManager * mm){
    return (int)(ptr-mm->base)/mm->page_size;
}