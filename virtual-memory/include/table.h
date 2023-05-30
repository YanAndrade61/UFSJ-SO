#ifndef __TABLE_H__
#define __TABLE_H__

#include "list.h"

typedef struct frame{

  unsigned isModified;
  unsigned isPresent;
  unsigned isReference;
  unsigned lastAcess;
  unsigned addr;

}Frame;

typedef struct table{
  List** tb;
  unsigned desl;
  int sz;
}Table;

#endif


int get_desl(int page_sz);
void table_push(Table* tb, unsigned addr, char rw, int time);
Table* table_init(int mem_sz, int page_sz);
Frame* table_find(Table* table, unsigned addr);
unsigned table_hash(Table* table,unsigned addr);
void table_push(Table* table,unsigned addr, char rw, int time);
Frame frame_init(unsigned addr, char rw, int time);
