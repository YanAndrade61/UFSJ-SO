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

#endif