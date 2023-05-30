#ifndef __TABLE_H__
#define __TABLE_H__

#include<stdio.h>
#include"list.h"
#include"tools.h"

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
  unsigned sz;
}Table;

int get_desl(Stats stats);
void table_push(Table* tb);
Table* table_init(Stats stats);
unsigned table_hash(Table* table,unsigned addr);
void table_push(Table* table,unsigned addr, char rw, int time);
Frame frame_init(unsigned addr, char rw, int time);

#endif