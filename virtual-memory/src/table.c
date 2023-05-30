#include "table.h"

int get_desl(int page_sz){
  unsigned tmp = page_sz*1024;
  unsigned s = 0;
  while (tmp>1) {
    tmp = tmp>>1;
    s++;
  }
  return s;
}

Table* table_init(int mem_sz, int page_sz){
  
  int sz = mem_sz/page_sz;
  Table* table = (Table*)calloc(1,sizeof(Table));
  table->tb = (List**)calloc(sz,sizeof(List*));
  
  for(int i = 0; i < sz; i++){
    table->tb[i] = list_init();
  }

  
  table->sz = sz;
  table->desl = get_desl(page_sz);
  return table;  
} 

unsigned table_hash(Table* table,unsigned addr){
  return (addr >> table->desl) % table->sz;
}

void table_push(Table* table,unsigned addr, char rw, int time){
  Frame f = frame_init(addr, rw, time);
  unsigned pos = table_hash(table, addr);

  list_push(table->tb[pos],(void*)&f);
    
}

Frame* table_find(Table* table, unsigned addr){
  
  unsigned pos = table_hash(table, addr);
  Node *node = table->tb[pos]->head->next;
  Frame* aux = NULL;
  
  for(;node != NULL; node = node->next){
    Frame* aux = (Frame*)node->data;
    if(aux->addr == addr){
      break;
    }
  }

  return aux;
}

Frame frame_init(unsigned addr, char rw, int time){
  Frame f;
  
  f.addr = addr;
  f.isReference = 1;
  if(rw == 'w')f.isModified = 1;
  else f.isModified = 0;
  f.isPresent = 1;
  f.lastAcess = time;

  return f;
}

