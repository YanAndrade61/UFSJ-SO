#ifndef __TOOLS_H__
#define __TOOLS_H__

#include<stdio.h>
#include<stdlib.h>
#include "list.h"
#include "table.h"

typedef struct config{

  unsigned page_sz;
  unsigned mem_sz;
  char *fpath;
  char *pol_subst;
  int debug;

} Config;

typedef struct acess{
  
  unsigned addr;
  char rw;

}Acess;

typedef struct stats{

  Config config;
  unsigned pages_read;
  unsigned written_pages;
  unsigned page_faults;
  unsigned dirty_pages;
  unsigned inMemory;

} Stats;      

List* read_entry(char* fpath);
Stats init_config(int argc, char** argv);
void print_stats(Stats stats);


#endif