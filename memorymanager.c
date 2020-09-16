#include <stdio.h>
#include <stdlib.h>
#include "memorymanager.h"



char memory[MEM_SIZE]={0};
char bitmap [MEM_SIZE/PAGE_SIZE]={0};

int main(int argc, int * args[]){
    MemoryManager mem;
    initialize_mem_man(&mem, memory, bitmap,PAGE_SIZE, MEM_SIZE/PAGE_SIZE);
    printf("hello world\n");
}

void initialize_mem_man(MemoryManager * mm, char * memory, char * bitmap, size_t ps, size_t qty){
    mm->base=memory;
    mm->bitmap=bitmap;
    mm->page_size = ps;
    mm->pages_q = qty;
}

void * alloc(MemoryManager * mm, void * ptr, size_t size ){

    
    uint blocks_q = (size-1)/mm->page_size + 1;
    if(ptr==NULL){
        int i = 0;
        int cont = 0;
        while( cont < blocks_q && i < mm->pages_q){
            if(mm->bitmap[i]==TAKEN) cont = 0;
            else cont++;
            i++;
        }
        if(cont == blocks_q){
            //SET BLOCKS EN 1
            //RETORNAR PUNTERO BLOQUE
            if(cont > i){
                //tirar error, nos equivocamos en algo
            }
            for(int j=i-cont;j<i;j++){
                mm->bitmap[j] = 1;
            }
            return calc_ptr_from_block(i-cont, mm);
        }else{
            //setear erno no sirve aca creo
            //podemos imprimir un log
            return NULL;
        }
    }
    else{
        int block_index = (int)(ptr-mm->base) / mm->page_size;
        if((ptr-mm->base) % mm->page_size != 0) blocks_q++;

        if(block_index + blocks_q >= mm->pages_q){
            //error
            return NULL;
        }    

        
        for(int i = 0; i<blocks_q;i++){

            if(mm->bitmap[i+block_index]==TAKEN){
                //error
                //return NULL;
                return alloc(mm, NULL, size);
            }   
        }
        
        for(int i = 0; i<blocks_q;i++) mm->bitmap[i+block_index]=TAKEN;


        return ptr;
        
        
    }
    
    
    
    
}

void m_free(MemoryManager * mm, void * ptr, size_t size){
    
}

void * calc_ptr_from_block(int block_idx, MemoryManager * mm){
    return mm->base+block_idx*mm->page_size;
}