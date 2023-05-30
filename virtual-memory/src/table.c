#include "table.h"

int get_desl(Stats stats){
  unsigned tmp = stats.page_sz;
  unsigned s = 0;
  while (tmp>1) {
    tmp = tmp>>1;
    s++;
  }
  return s;
}

Table* table_init(Stats stats){
  
  Table* table = (Table*)calloc(stats.mem_sz/stats.page_sz,sizeof(List*));
  
  for(unsigned i = 0; i < sz; i++){
    table->tb[i] = list_init();
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