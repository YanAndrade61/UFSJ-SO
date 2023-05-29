#ifndef __TOOLS_H__
#define __TOOLS_H__

#include<stdio.h>
#include<stdlib.h>
#include "list.h"

typedef struct config{

  unsigned page_sz;
  unsigned mem_sz;
  char *fpath;
  char *pol_subst;

} Config;

typedef struct acess{
  
  unsigned addr;
  char rw;

}Acess;


List* read_entry(char* fpath);


#endif