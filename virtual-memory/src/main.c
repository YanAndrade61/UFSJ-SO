#include<stdio.h>
#include<stdlib.h>
#include"list.h"
#include "table.h"
#include "tools.h"

int main(int argc, char** argv){

  Stats stats = init_config(argc, argv);

  List* acess_list = read_entry(stats.config.fpath);

  run_memory(acess_list,stats);
  list_free(acess_list);
  
  print_stats(stats);
  return 0;
}

