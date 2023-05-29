#include "table.h"

Table* table_init(size_t sz){
  
  Table table = (Table)calloc(sz,sizeof(List*));
  
  for(unsigned i = 0; i < sz; i++){
    table[i] = list_init();
  }
  return table;
  
} 

void table_push(Table* tb){}