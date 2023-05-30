#include "tools.h"

Stats init_config(int argc, char** argv){
  if(argc < 5 || argc > 6){ 
    printf("Numero de argumentos incompativel com o exigido pelo programa");
    exit(1);
  }

  Stats stats;
  stats.config.pol_subst = argv[1];
  stats.config.fpath = argv[2];
  stats.config.page_sz = atoi(argv[3]);
  stats.config.mem_sz = atoi(argv[4]);
  if(argc == 6) stats.config.debug = atoi(argv[5]);
  else stats.config.debug = 0;
  stats.pages_read = 0;
  stats.written_pages = 0;
  stats.dirty_pages = 0;
  stats.page_faults = 0;
  stats.inMemory = 0;

  return stats;
}

List* read_entry(char* fpath){

  FILE* fp = fopen(fpath,"r");
  List* acess_list = list_init();
  Acess *acess;
  
  if(fp == NULL){
    printf("read_entry: fail to load file");
    exit(EXIT_FAILURE);
  }
  while(1){  
    acess = (Acess*)calloc(1,sizeof(Acess));
    fscanf(fp,"%x %c", &(acess->addr), &(acess->rw));
    if(feof(fp))break;
    list_push(acess_list, (void*)acess);
  }
  
  fclose(fp);
  free(acess);
  return acess_list;
}

void print_stats(Stats stats){
  printf("\tArquivo de entrada: %s\n", stats.config.fpath);
  printf("\tTamanho da memoria: %d KB\n", stats.config.mem_sz);
  printf("\tTamanho das paginas: %d KB\n", stats.config.page_sz);
  printf("\tTecnica de reposicao: %s\n", stats.config.pol_subst);
  printf("\tPaginas lidas: %d\n", stats.pages_read);
  printf("\tPaginas escritas: %d\n", stats.written_pages);
  printf("\tPaginas sujas: %d\n", stats.dirty_pages);
  printf("\tPage faults: %d\n", stats.page_faults);
}

void run_memory(List* acess_list, Stats stats){

  Node* node = acess_list->head->next;
  Acess* acess = NULL;
  Table* table = table_init();
  int time = 0;
  
  for(;node != NULL; node = node->next){
    acess = (Acess*)(node->data);
    table_push(table,acess->addr,acess->rw,time);
    time++;  
  }
}