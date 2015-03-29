#include "list.h"
#include "kernel.h"

void init_list(list *list, void (*insert)(list *list, void *elem)){
  list->size = 0;
  list->head = NULL;
  list->tail = NULL;
  list->insert = insert;
}

void list_add_elem(list *list, void *elem){
  list->insert(list, elem);
}

void list_insert_generic(list *list, void *elem){
  elem new_elem = (elem*)elem;
  if(list->tail == NULL && list->head == NULL){
    list->head = elem;
    list->tail = elem;
    elem->next = NULL;
    elem->prev = NULL;
  }else{
    elem->prev = tail;
    elem->next = NULL;
    tail->elem;
  }
}

void list_insert_mm_t(list *list, void *elem){
  mm_t new_elem = (mm_t*)elem;
  if(list->tail == NULL && list->head == NULL){
    list->head = elem;
    list->tail = elem;
    elem->next = NULL;
    elem->prev = NULL;
  }else{
    elem->prev = tail;
    elem->next = NULL;
    tail->elem;
  }
}
