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
    return 2*(pow(2, levels)) / 8;
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
    return getBit(buddy.base + 2*(baseLevel+offset)/8,2*(baseLevel+offset)%8);
}

void * m_alloc(size_t size){
    int level = getLevel(size);
    int i;
    for( i = 0 ; i < pow(2, level) ; i++){
        if(getValue(level, i)==0){
            allocateBranch(level, i);
            return getPtr(level,i); 
        }
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


void m_free(void * dir){
    

    int offset=((char *)dir-buddy.base)/buddy.minPage;
    int owner=0;
    int baseLevel;
    int level = buddy.levels-1;
    while(!owner){
        baseLevel = pow(2, level)-1;
        owner=getBit(buddy.base + 2*(baseLevel+offset)/8,2*(baseLevel+offset)%8+1);
        if(!owner){
            offset=offset/2;
            level--;
        }
    }
    freeBranch(level,offset);

    return;
}

void freeBranch(unsigned int level, unsigned int offset){

    setRecursivelyDown(level, offset, CLEAR);

     int bit =  pow(2, level)-1+offset;
    clearBit(buddy.base+2*bit/8,2*bit%8+1);
    
    int buddyIndex = offset%2? offset+1:offset-1;   
    while(level>0 && getValue(level,buddyIndex)==0){
         
         clearBit(buddy.base+2*bit/8,2*bit%8);
         offset/=2;
         level--;
         buddyIndex = offset%2? offset+1:offset-1;
         bit =  pow(2, level)-1+offset;

    }
    

    
}

void allocateBranch(unsigned int level, unsigned int offset){
    int bit;
    unsigned int tmpOffset = offset;
    bit =  pow(2, level)-1+offset;
    setBit(buddy.base+2*bit/8,2*bit%8+1);
    for(int i = level ; i >= 0 ; i--){
        bit =  pow(2, i)-1+offset;
        setBit(buddy.base+2*bit/8,2*bit%8);
        offset=offset >> 1;
    }
    
    offset=tmpOffset;

    setRecursivelyDown(level, offset,SET);

}

void setRecursivelyDown(unsigned int level, unsigned int offset, int flag){
    if(level>buddy.levels) return;
    int bit =  pow(2, level)-1+offset;
    if(flag==SET){
        setBit(buddy.base+2*bit/8,2*bit%8);
    }else{
        clearBit(buddy.base+2*bit/8,2*bit%8);
    }
    

    setRecursivelyDown(level+1,offset*2,flag);
    setRecursivelyDown(level+1,offset*2+1,flag);
    return;

}


void * getPtr(unsigned int level, unsigned  int offset){

     unsigned int blockSize=buddy.virtualSize/pow(2,level);
     return buddy.base + blockSize*offset;
}

void memInfo(char * buffer){
    buffer+=strcpy(buffer, "Memory state \n");
    buffer+=strcpy(buffer, "Base Address: ");
    buffer+=intToString((int) buddy.base,buffer);
    *buffer++='\n';
    buffer+=strcpy(buffer, "Memory Size: ");
    buffer+=intToString(buddy.memSize/M,buffer);
    buffer+=strcpy(buffer, " MB");
    *buffer++='\n';
    buffer+=strcpy(buffer, "Overhead: bastante o ");
    buffer+=intToString(buddy.size,buffer);
    buffer+=strcpy(buffer, " KB");
    *buffer++='\n';
    buffer+=strcpy(buffer, "Page size: ");
    buffer+=intToString(buddy.minPage,buffer);
    //buffer+=strcpy(buffer, " KB");
    *buffer++='\n';
    buffer+=strcpy(buffer, "Page quantity: ");
   // buffer+=intToString(buddy.,buffer);
    *buffer++='\n';
    buffer+=strcpy(buffer, "Reserved Pages: ");
    buffer+=intToString(buddy.reserved,buffer);
    *buffer++='\n';
    buffer+=strcpy(buffer, "Free Pages: ");
    buffer+=intToString(buddy.reserved,buffer);
    *buffer++='\n';
     *buffer++=0;
}
#endif