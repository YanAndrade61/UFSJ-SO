#ifndef __TABLE_H__
#define __TABLE_H__

#include<stdio.h>
#include"list.h"

typedef struct frame{

  unsigned isModified;
  unsigned lastAcess;
  unsigned isPresent;
  unsigned addr;

}Frame;

typedef struct table{
  List** tb;
  unsigned desl;
  unsigned sz;
}Table;

#endif