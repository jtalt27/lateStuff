#include "LRUstack.h"

/* initializes the LRU stack */
void initialize(lrustack* lrus, unsigned int maxsize) {
    // lrus=(lrustack*)malloc(sizeof(lrustack));
    lrus->head = NULL;
    lrus->maxsize = maxsize;
    lrus->size = 0;
    lrus->tail = NULL;
}

/* use pagenum when creating a new node, which will be pushed onto
   the LRU stack; make sure to keep track of the LRU stack's size
   and free and reset the tail as necessary to limit it to max size */
void push(lrustack* lrus, unsigned long pagenum) {
    node* node1 = (node*)malloc(sizeof(node));
    node1->pagenum = pagenum;
    node1->next = NULL;
    node1->prev = NULL;
    if( lrus->head == NULL ) {
        lrus->head = node1;
        lrus->tail = node1;
        lrus->size++;
    }
    else if( lrus->size == 1 ) {
        node1->next = lrus->head;
        lrus->head->prev = node1;
        lrus->head = node1;
        lrus->tail = lrus->head->next;
        lrus->size++;
    }
    else if( lrus->size == lrus->maxsize ) {
        // node* temp = (node*)malloc(sizeof(node));
        node* temp = lrus->tail;
        lrus->tail = temp->prev;
        lrus->tail->next = NULL;
        free(temp);

        node* temp1 = lrus->head;
        temp1->prev = node1;
        node1->next = temp1;
        lrus->head = node1;
    }
    else {
        node1->next = lrus->head;
        lrus->head->prev = node1;
        lrus->head = node1;
        lrus->size++;
    }
}

/* seek pagenum in lrus and remove it if found; return the depth
   at which pagenum was found or -1 if not */
int seek_and_remove(lrustack* lrus, unsigned long pagenum) {
    node* node1 = lrus->head;
    int depth = -1;
    int count = 1;
    bool found = false;
    if(node1 == NULL) {
        return -1;
    }
    while(node1!=NULL && !found){
        if(node1->pagenum == pagenum){
            found = true;
            depth = count;
        }
        else{
            node1 = node1->next;
            count++;
        }
        
    }
    // while( node1->pagenum != pagenum && node1 != NULL ) {
    //     if( node1->next == NULL ) {
    //         return -1;
    //     }
    //     node1 = node1->next;
    //     count++;
    // }
    if(found){
        //Only thing in the stack 
        if(node1 == lrus->head && node1 == lrus->tail ){
            lrus->head = NULL;
            lrus->tail = NULL;
            lrus->size--;
        }
        //Head of Stack
        else if( lrus->head == node1 ) {
            lrus->head = node1->next;
            lrus->head->prev = NULL;
            lrus->size--;
        }
        //Tail of Stack
        else if( lrus->tail == node1 ) {
            lrus->tail = node1->prev;
            lrus->tail->next = NULL;
            lrus->size--;
        }
        //Middle of the stack 
        else {
            node1->prev->next = node1->next;
            node1->next->prev = node1->prev;
            lrus->size--;
        }
        free(node1);
    }
    return depth;
}