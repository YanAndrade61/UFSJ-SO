#ifndef __LIST_H__
#define __LIST_H__

#include <stdio.h>
#include <stdlib.h>
 
typedef struct node {
    void* data;
    struct node* next;
} Node;
 
typedef struct list {
  size_t sz;
  Node* head;
  Node* last;
} List;

List* list_init();
void list_push(List* list, void* data);
void* list_pop(List* list);
void list_free(List* list);
void list_print(List* list);


#endif