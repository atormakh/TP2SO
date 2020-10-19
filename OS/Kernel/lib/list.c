#include <list.h>
#include<ipc.h>

List *newList(){
    List *list = c_alloc(PAGE_SIZE);
    list->size = 0;
    list->start = 0;
    return list;
}

void add(List *list, void * val, unsigned long long hash){
    Node * next = list->start;
    Node * prev = next;
    Node * newNode = (Node *)m_alloc(sizeof(Node));

    if(next == NULL){
        list->start = newNode;
        newNode->next = NULL;
        newNode->elem = val;
        newNode->hash=hash;
        return;
    }

    while ( next!=NULL && hash-next->hash > 0){
        prev = next;
        next = next->next;
    }
    if(prev == next) list->start = newNode;
    else prev->next = newNode;
    newNode->next=next;
    newNode->elem = val;
    newNode->hash=hash;
    list->size++;

}

void push(List *list, void *val, unsigned long long hash){

    Node *head = list->start;
    Node *new_node;
    new_node = (Node *)c_alloc(sizeof(Node));

    new_node->elem = val;
    new_node->hash = hash;
    new_node->next = head;
    list->start = new_node;
    list->size++;
}

void remove(List * list,  unsigned long long hash){
    Node * node = list->start;
    Node * prev = node;
    while(node!=NULL && node->hash - hash!=0){
        prev=node;
        node=node->next;
    }
    if(prev==node) list->start = node->next;
    else prev->next=node->next;
    m_free(node);
}

void * get(List * list,  unsigned long long hash){
    Node * node = list->start;
    while(node!=NULL && node->hash -hash !=0)
        node=node->next;

    if(node==NULL){
        return NULL;
    }
    return node->elem;
}

void *pop(List *list){
    if(list->size ==0)return NULL;
    Node * header = list->start;
    list->start = header->next;
    list->size--;
    void * elem = header->elem;
    m_free(header);
    return elem;
}

void *peek(List *list){
    return list->start->elem;
}

void freeList(List * list){
    if( list != NULL){
        Node * prev = list->start;
        Node * actual = list->start;

        while(actual !=NULL){
            prev = actual;
            actual = actual->next;
            m_free(prev);
        }
        m_free(list);
    }
}

Node * first(List * list){
    if(list != NULL){
        return list->start->elem;
    }
    return NULL;
}

void iterator(List * list){
    list->iterator = list->start;
}

int hasNext(List * list){
    return list->iterator!=NULL;
}

void * next(List * list){
    void * aux = list->iterator->elem;
    list->iterator = list->iterator->next;
    return aux;
}