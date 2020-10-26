#ifndef LIST_H
#define LIST_H

#include<memorymanager.h>

typedef struct Node{
    struct Node * next;
    void * elem;
    unsigned long long hash;
}Node;

typedef struct List{
    Node * start;
    unsigned long long size;
    Node * iterator;
}List;

List *newList();
void add(List *list, void * val, unsigned long long hash);
void push(List * list, void * val, unsigned long long hash);
void remove(List * list,  unsigned long long hash);
void * pop(List * list);
void * get(List * list,  unsigned long long hash);
void *peek(List *list);
void freeList(List * list);
void iterator(List * list);
int hasNext(List * list);
void * getNext(List * list);

#endif