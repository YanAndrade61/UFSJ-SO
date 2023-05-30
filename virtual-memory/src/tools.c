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

void run_memory(List* acess_list, Stats* stats){

  // printf("ok\n");
  Node* node = acess_list->head->next;
  Acess* acess = NULL;
  Table* table = table_init(stats->config.mem_sz, stats->config.page_sz);
  Frame* aux = NULL;
  int time = 0;

  for(;node != NULL; node = node->next){
    acess = (Acess*)(node->data);
    if(acess->rw == 'W') stats->written_pages++;
    else stats->pages_read++;

    aux = table_find(table,acess->addr);
    if(aux != NULL){
      if(acess->rw == 'W')aux->isModified = 1; 
      aux->lastAcess = time;
      aux->isReference = 1;
      aux->isPresent = 1;
      continue;
    }
    stats->page_faults++;
    if(stats->inMemory < table->sz){
      table_push(table,acess->addr,acess->rw,time);
      stats->inMemory++;
      continue;
    }

    if(strcmp(stats->config.pol_subst, "lru") == 0){
        lru(table, acess->addr, acess->rw, time, stats);
    }
    if(strcmp(stats->config.pol_subst, "nru") == 0){
        nru(table, acess->addr, acess->rw, time, stats);
    }
    if(strcmp(stats->config.pol_subst, "second-chance") == 0){
      // second(table, acess->addr, acess->rw, time, stats);        
    }
    
    time++;  
  }
}

void lru(Table* table, unsigned addr, unsigned mode, unsigned time, Stats* stats){

  Frame* subst = NULL;
  unsigned min = INT_MAX;
  Node* node = NULL;

  //Find the page that was last used
  for(int i = 0; i < table->sz; i++){
    node = table->tb[i]->head->next;
    for(;node != NULL; node = node->next){
      Frame* atual = (Frame*)(node->data);
      if(atual->isPresent)continue;
      
      if(atual->lastAcess < min){
        subst = atual;
        min = subst->lastAcess;
      }
    }
  }

  //Subst the page last used
  subst->isPresent = 0;
  table_push(table,addr,mode,time);
  if(subst->isModified == 'W')stats->dirty_pages++;
  
}

void nru(Table* table, unsigned addr, unsigned mode, unsigned time, Stats* stats){

  Frame* subst = NULL;
  Node* node = NULL;
  int _class = 4;
  
  //Find the page that was not recently used based on 4 classes
  for(int i = 0; i < table->sz; i++){
    node = table->tb[i]->head->next;
    for(;node != NULL; node = node->next){
      Frame* atual = (Frame*)(node->data);
      if(atual->isPresent)continue;
      
      if(!atual->isReference && !atual->isModified){
        subst = (Frame*)(node->data);
        _class = 0;
      }
      if(_class > 1 &&
         !atual->isReference && atual->isModified){
        subst = (Frame*)(node->data);
        _class = 1;
      }
      if(_class > 2 &&
         atual->isReference && !atual->isModified){
        subst = (Frame*)(node->data);
        _class = 2;
      }
      if(_class > 3 &&
         atual->isReference && atual->isModified){
        subst = (Frame*)(node->data);
        _class = 3;
      }
      atual->isReference = 0;
    }
  }

  //Subst the page last used
  subst->isPresent = 0;
  if(subst->isModified == 'W')stats->dirty_pages++;
  table_push(table,addr,mode,time);
  
}

void second_chance(Table* table, unsigned addr, unsigned mode, unsigned time, Stats* stats){

  Frame* subst = NULL;
  unsigned min = INT_MAX;
  Node* node = NULL;

  //Find the page that was last used and is not reference
  while(1){
    for(int i = 0; i < table->sz; i++){
      node = table->tb[i]->head->next;
      for(;node != NULL; node = node->next){
        Frame* atual = (Frame*)(node->data);
        if(atual->isPresent)continue;
        
        if(atual->lastAcess < min){
          subst = atual;
          min = subst->lastAcess;
        }
      }
    }
    //Give a second chance 
    if(subst->isReference == 1){
      subst->isReference = 0;
      subst->lastAcess = time;
    }else break;
  }

  //Subst the page last used
  subst->isPresent = 0;
  table_push(table,addr,mode,time);
  if(subst->isModified == 'W')stats->dirty_pages++;
  
}
