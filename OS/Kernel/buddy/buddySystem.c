#include <memorymanager.h>
#ifdef BUDDY
#include <lib.h>
BuddySystem buddy;

void initialize_mem_man(void * memBase, size_t memSize, size_t minPageSize){
        
    memSize=memSize-(unsigned long long)memBase;

    unsigned long long virtualSize = computeVirtualSize(memSize);
    unsigned long long size = computeBuddySize(virtualSize, minPageSize);
    int levels = Log2n(virtualSize / minPageSize)+1;
    
    buddy.base=memBase;
    buddy.memSize=memSize;
    buddy.minPage=minPageSize;
    buddy.virtualSize=virtualSize;
    buddy.size=size;
    buddy.levels=levels;
    buddy.reserved = 0;

    for(unsigned long long i=0; i< size;i++) buddy.base[i]=0;

    //reservo zona donde esta el buddy
    int level = getLevel(size);
    allocateBranch(level,0);

    

    //reservo zona de memoria que en realidad no tenemos
    unsigned long long diffSize = virtualSize - memSize;
    if(diffSize != 0){

        level = getLevel(diffSize);
        allocateBranch(level,pow(2,level)-1);

    
    }
    
}



unsigned long long computeBuddySize(unsigned long long memSize, unsigned long long minPageSize){
    int levels = Log2n(memSize / minPageSize)+1;
    return (pow(2, levels)) / 8;
}

unsigned int getLevel(unsigned long long size){
    if (size < buddy.minPage)
        size = buddy.minPage;
    return Log2n(buddy.virtualSize / size);
}

unsigned long long computeVirtualSize(unsigned long long memSize){
     unsigned long long virtualSize = pow(2, Log2n(memSize));
    if(virtualSize < memSize){
        virtualSize = pow(2, Log2n(memSize)+1);
    }
    return virtualSize;
}

int getValue(unsigned int level, unsigned int offset){
    int baseLevel = pow(2, level)-1;
    return getBit(buddy.base + (baseLevel+offset)/8,(baseLevel+offset)%8);
}

int checkUpwards(unsigned int level, unsigned int offset){
    int buddyIndex = (offset%2)? offset-1:offset+1;  
    
    while(level>0 && getValue(level,buddyIndex)==0){        
        offset=offset>>1;
        level--;
        buddyIndex = (offset%2)? offset-1:offset+1;
        if(getValue(level,offset)==1)return 0; 
       

    }
    return 1; 
}

void * m_alloc(size_t size){
    int level = getLevel(size);
    int i;
    for( i = 0 ; i < pow(2, level) ; i++){
        if(getValue(level, i)==0){
            if(checkUpwards(level,i)){
                allocateBranch(level, i);
                buddy.reserved+=buddy.virtualSize/pow(2,level);
                return getPtr(level,i); 
            }
        }
    }
    return NULL;
    
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


void m_free(void * dir){
    
    
    int offset=((char *)dir-(char *)buddy.base)/buddy.minPage;
    int active=0;
    
    int level = buddy.levels-1;
    while(!active){
       int baseLevel = pow(2, level)-1;
       active=getBit(buddy.base + (baseLevel+offset)/8,(baseLevel+offset)%8);
        if(!active){
            offset=offset>>1;
            level--;
        }
    }
    freeBranch(level,offset);
    buddy.reserved-=buddy.virtualSize/pow(2,level);
    return;
}

void freeBranch(unsigned int level, unsigned int offset){

     int bit;
    
    int buddyIndex = (offset%2)? offset+1:offset-1;   
    while(level>0 && getValue(level,buddyIndex)==0){
         
         clearBit(buddy.base+bit/8,bit%8);
         offset=offset>>1;
         level--;
         buddyIndex = (offset%2)? offset+1:offset-1;
         bit =  pow(2, level)-1+offset;

    }
    

    
}

void allocateBranch(unsigned int level, unsigned int offset){


    for(int i = level ; i >= 0 ; i--){
        int bit =  pow(2, i)-1+offset;
        setBit(buddy.base+bit/8,bit%8);
        offset=offset >> 1;
       
    }
    

}



void * getPtr(unsigned int level, unsigned  int offset){

     unsigned int blockSize=buddy.virtualSize/pow(2,level);
     return buddy.base + blockSize*offset;
}

void memInfo(char * buffer){
    buffer+=strcpy(buffer, "Memory state \n");
    buffer+=strcpy(buffer, "Base Address: ");
    buffer+=intToString((unsigned long long) buddy.base,buffer);
    *buffer++='\n';
    buffer+=strcpy(buffer, "Memory Size: ");
    buffer+=intToString(buddy.memSize/M,buffer);
    buffer+=strcpy(buffer, " MB");
    *buffer++='\n';
    buffer+=strcpy(buffer, "Overhead: ");
    buffer+=intToString(buddy.size/K,buffer);
    buffer+=strcpy(buffer, " KB");
    *buffer++='\n';
    buffer+=strcpy(buffer, "Page size: ");
    buffer+=intToString(buddy.minPage/K,buffer);
    buffer+=strcpy(buffer, " KB");
    *buffer++='\n';
    buffer+=strcpy(buffer, "Page quantity: ");
    buffer+=intToString(pow(2,buddy.levels-1),buffer);
    *buffer++='\n';
    buffer+=strcpy(buffer, "Reserved memory: ");
    buffer+=intToString(buddy.reserved/K,buffer);
    buffer+=strcpy(buffer, " KB");
    *buffer++='\n';
    buffer+=strcpy(buffer, "Free memory: ");
    buffer+=intToString((buddy.memSize-buddy.reserved)/K,buffer);
    buffer+=strcpy(buffer, " KB");
    *buffer++='\n';
     *buffer++=0;
}
#endif