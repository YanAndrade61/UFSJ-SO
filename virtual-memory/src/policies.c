
void lru(Table* table, unsigned addr, unsigned mode){

  Frame* subst = NULL;
  unsigned min = INT_MAX;
  Node* node = NULL;

  //Find the page that was last used
  for(int i = 0; i < table->sz; i++){
    node = table->tb[i]->head->next;
    for(;node != NULL; node = node->next){
      if((Frame*)(node->data)->isPresent)continue;
      
      if((Frame*)(node->data)->lastAcess < min){
        subst = (Frame*)(node->data);
        min = subst->lastAcess;
      }
    }
  }

  //Subst the page last used
  subst->isPresent = 0;
  table_push(table,addr,mode);
  
}

void nru(Table* table, unsigned addr, unsigned mode){

  Frame* subst = NULL;
  Node* node = NULL;
  int _class;
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
      if(!atual->isReference && atual->isModified){
        subst = (Frame*)(node->data);
        _class = 1;
      }
      if(atual->isReference && !atual->isModified){
        subst = (Frame*)(node->data);
        _class = 2;
      }
      if(atual->isReference && atual->isModified){
        subst = (Frame*)(node->data);
        _class = 3;
      }
    }
    //We find a page in min class
    if(_class == 0)break;
  }

  //Subst the page last used
  subst->isPresent = 0;
  table_push(table,addr,mode);
  
}