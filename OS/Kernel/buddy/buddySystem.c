#include <buddySystem.h>
#ifdef BUDDY
#include <lib.h>
BuddySystem buddy;

void initializeBuddySystem(void * memBase, unsigned long long memSize, unsigned long long minPageSize){
        
    
    unsigned long long virtualSize = computeVirtualSize(memSize);
    unsigned long long size = computeBuddySize(virtualSize, minPageSize);
    int levels = Log2n(memSize / minPageSize)+1;
    
    buddy.base=memBase;
    buddy.memSize=size;
    buddy.minPage=minPageSize;
    buddy.virtualSize=virtualSize;
    buddy.size=size;
    buddy.levels=levels;

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

void * m_alloc(unsigned long size){
    int level = getLevel(size);
    for(int i = 0 ; i < pow(2, level) ; i++){
        if(getValue(level, i)==0){
            allocateBranch(level, i);
        }
    }
}

void m_free(void * dir){
    
}

void allocateBranch(unsigned int level, unsigned int offset){
    int bit;
    unsigned int tmpOffset = offset;
    for(int i = level ; i >= 0 ; i--){
        bit =  pow(2, i)-1+offset;
        setBit(buddy.base+bit/8,bit%8);
        offset=offset >> 1;
    }
    
    offset=tmpOffset;

    allocateRecursivelyDown(level, offset);

}

void allocateRecursivelyDown(unsigned int level, unsigned int offset){
    if(level>buddy.levels) return;
    int bit =  pow(2, level)-1+offset;
    setBit(buddy.base+bit/8,bit%8);

    allocateRecursivelyDown(level+1,offset*2);
    allocateRecursivelyDown(level+1,offset*2+1);
    return;

}
#endif