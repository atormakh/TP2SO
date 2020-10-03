#include<list.h>

List * newList(CMP cmp){
    List * list = m_alloc(PAGE_SIZE);
    list->size=0;
    list->start=0;
    list->cmp = cmp;
}

void push(List * list, void * val) {
    if(list->cmp==NULL){
        Node * head = list->start;
        Node * new_node;
        new_node = (Node *) m_alloc(sizeof(Node));

        new_node->elem = val;
        new_node->next = head;
        head = new_node;
    }else{
        Node * next = list->start;
        while( list->cmp(val,next->elem) ){
            next = next->next;

        } 
    }
}

void * pop(List * list){
    Node * header = list->start;
    list->start = header->next;
    return header;
}

void * peek(List * list){
    return list->start;
}