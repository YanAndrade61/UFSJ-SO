#ifndef __TOOLS_H__
#define __TOOLS_H__

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include "table.h"
// #include "list.h"


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
  int inMemory;

}Stats;

void print_stats(Stats stats);
List* read_entry(char* fpath);
Stats init_config(int argc, char** argv);
void run_memory(List* acess_list, Stats* stats);
void lru(Table* table, unsigned addr, unsigned mode, unsigned time, Stats* stats);
void nru(Table* table, unsigned addr, unsigned mode, unsigned time, Stats* stats);
void second_chance(Table* table, unsigned addr, unsigned mode, unsigned time, Stats* stats);

#endif
