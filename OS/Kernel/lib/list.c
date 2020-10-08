#include <list.h>

List *newList(CMP cmp){
    List *list = m_alloc(PAGE_SIZE);
    list->size = 0;
    list->start = 0;
    list->cmp = cmp;
}

void add(List *list, void * val){
    if (list->cmp == NULL){
        push(list, val);
        return;
    }
    Node * next = list->start;
    Node * prev = next;
    Node * newNode = (Node *)m_alloc(sizeof(Node));

    if(next == NULL){
        list->start = newNode;
        newNode->next = NULL;
        newNode->elem = val;
        return;
    }

    while ( next!=NULL && list->cmp(val, next->elem) > 0){
        prev = next;
        next = next->next;
    }
    prev->next = newNode;
    newNode->next=next;
    newNode->elem = val;
    list->size++;

}

void push(List *list, void *val){

    Node *head = list->start;
    Node *new_node;
    new_node = (Node *)m_alloc(sizeof(Node));

    new_node->elem = val;
    new_node->next = head;
    head = new_node;
    list->size++;
}

void remove(List * list, void * val){
    Node * node = list->start;
    Node * prev = node;
    while(node!=NULL || list->cmp(node->elem,val)==0){
        prev=node;
        node=node->next;
    }
    prev->next=node->next;
    m_free(node);
}

void * get(List * list, void * val){
    Node * node = list->start;
    while(node!=NULL || list->cmp(node->elem,val)==0)
        node=node->next;
    return node;
}

void *pop(List *list){
    Node *header = list->start;
    list->start = header->next;
    list->size--;
    return header;
}

void *peek(List *list){
    return list->start;
}

void freeList(List *list){
    if( list != NULL){
        Node * prev = list->start;
        Node * actual = list->start;

        while(actual !=NULL){
            prev = actual;
            actual = actual->next;
            m_free(prev);
        }
    }
}