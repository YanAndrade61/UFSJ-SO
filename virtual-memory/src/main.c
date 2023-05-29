#include<stdio.h>
#include<stdlib.h>
#include"list.h"
#include "table.h"
#include "tools.h"

int main(int argc, char** argv){

  List* acess_list = read_entry("entrada.in");
  Table
  
  
  list_free(acess_list);
  return 0;
}

/*
  PRINT LIST
  Node* node_curr = acess_list->head->next;
  while(node_curr != NULL){
    printf("%x %c\n", ((Acess*)node_curr->data)->addr,((Acess*)node_curr->data)->rw);
    node_curr = node_curr->next;
  }
  
*/