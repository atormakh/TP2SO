#ifndef BUDDY_SYSTEM_H
#define BUDDY_SYSTEM_H

typedef struct BuddySystem{
    unsigned long long memSize;
    unsigned long long minPage;
    char * base;
}BuddySystem;

#endif

