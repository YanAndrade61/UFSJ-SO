#include "policies.h"

void lru(Table* table, unsigned addr, unsigned mode, unsigned time, Stats stats){

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
  if(subst->isModified == 'w')stats.dirty_pages++;
  
}

void nru(Table* table, unsigned addr, unsigned mode, unsigned time, Stats stats){

  Frame* subst = NULL;
  Node* node = NULL;
  int _class = 3;
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
    }
    //We find a page in min class
    if(_class == 0)break;
  }

  //Subst the page last used
  subst->isPresent = 0;
  if(subst->isModified == 'w')stats->dirty_pages++;
  table_push(table,addr,mode,time);
  
}
