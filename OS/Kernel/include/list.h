#ifndef LIST_H
#define LIST_H

#include<memorymanager.h>
typedef int (*CMP)(void * elem1, void * elem2);
typedef struct Node{
    struct Node * next;
    void * elem;
}Node;

typedef struct List{
    Node * start;
    unsigned long long size;
    CMP cmp;
}List;

void push(List * list, void * val);
void * pop(List * list);
#endif