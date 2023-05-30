#include "list.h"

List* list_init() {
  List* list = (List*)calloc(1,sizeof(List));
  list->head = (Node*)calloc(1,sizeof(Node));
  list->last = list->head;
  list->sz = 0;
  return list;
}
 
void list_push(List* list, void* data) {
    Node* node = (Node*)calloc(1,sizeof(Node));

    node->data = data;
    node->next = NULL;
    list->last->next = node;
    list->last = node;
    list->sz++;
}
 
void* list_pop(List* list) {
    if (list->sz == 0) {
        return NULL;
    }
    Node* node_rm = list->head;
    void* data = node_rm->data;
    list->head = node_rm->next;
    free(node_rm);
    list->sz--;
    return data;
}
 
void list_free(List* list) {
    Node* node_curr = list->head;
    while (node_curr != NULL) {
        Node* next = node_curr->next;
        free(node_curr->data);
        free(node_curr);
        node_curr = next;
    }
    free(list);
}