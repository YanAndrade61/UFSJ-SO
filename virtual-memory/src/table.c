#include "table.h"

int get_desl(Stats stats){
  unsigned tmp = stats.config.page_sz*1024;
  unsigned s = 0;
  while (tmp>1) {
    tmp = tmp>>1;
    s++;
  }
  return s;
}

Table* table_init(Stats stats){
  
  int sz = stats.config.mem_sz/stats.config.page_sz;
  Table* table = (Table*)calloc(1,sizeof(Table));
  table->tb = (List**)calloc(sz,sizeof(List*));
  printf("ok\n");
  
  for(int i = 0; i < sz; i++){
    printf("ok\n");
    table->tb[i] = list_init();
    printf("ok\n");
  }

  
  table->sz = sz;
  table->desl = get_desl(stats);
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

