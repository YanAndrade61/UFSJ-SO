#include "tools.h"

List* read_entry(char* fpath){

  FILE* fp = fopen(fpath,"r");
  List* acess_list = list_init();
  Acess *acess;
  
  if(fp == NULL){
    printf("read_entry: fail to load file");
    exit(EXIT_FAILURE);
  }
  while(1){  
    acess = (Acess*)calloc(1,sizeof(Acess));
    fscanf(fp,"%x %c", &(acess->addr), &(acess->rw));
    if(feof(fp))break;
    list_push(acess_list, (void*)acess);
  }
  
  fclose(fp);
  free(acess);
  return acess_list;
}
