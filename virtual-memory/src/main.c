// #include "include.h"
#include "tools.h"
#include "list.h"

int main(int argc, char** argv){

  Stats stats = init_config(argc, argv);

  List* acess_list = read_entry(stats.config.fpath);

  run_memory(acess_list,&stats);
  list_free(acess_list);
  
  print_stats(stats);
  return 0;
}

