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

List *newList(CMP cmp);
void add(List *list, void * val);
void push(List * list, void * val);
void remove(List * list, void * val);
void * pop(List * list);
void * get(List * list, void * val);
void *peek(List *list);
void freeList(List * list);

#endif