#include <buddySystem.h>

BuddySystem buddy;

void initializeBuddySystem(void * memBase, unsigned long long memSize, unsigned long long minPageSize){
        
    
    unsigned long long size = computeSize(memSize, minPageSize);
    unsigned long long virtualSize = pow(2,log2n(size));
    buddy.base=memBase;
    buddy.memSize=size;
    buddy.minPage=minPageSize;

    for(unsigned long long i=0; i< virtualSize;i++) buddy.base[i]=0;

    //reservo zona donde esta el buddy 
    int level = getLevel(size);
    *(buddy.base + pow(2, level)-1) = 1;

    //reservo zona de memoria que en realidad no tenemos 
    unsigned long long diffSize = virtualSize-size;
    level = getLevel(diffSize);
    *(buddy.base +pow(2, level+1)-2)=1;
    
}

unsigned int Log2n(unsigned int n) 
{ 
    return (n > 1) ? 1 + Log2n(n >> 1) : 0; 
}

unsigned long long pow(unsigned int base, unsigned int exp){
    unsigned long long ret=1;
    for(int i=0;i<exp;i++) ret*=base;
    return ret;
}
unsigned long long computeSize(unsigned long long memSize, unsigned long long minPageSize){
    int levels = Log2n(memSize/minPageSize)+1;
    return pow(2,levels)/8;
}

unsigned int getLevel(unsigned long long size){
    if(size<buddy.minPage) size=buddy.minPage;
    return Log2n( buddy.memSize/size);
}

void * m_alloc(unsigned long size){
    return 6000000;
}
