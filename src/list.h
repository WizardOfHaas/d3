#ifndef LIST_H
#define LIST_H

#include <stddef.h>

typedef struct List_Elem_tag{
  struct List_Elem_tag *prev;
  struct List_Elem_tag *next;

  void *data;
} elem;

typedef struct List_tag{
  int size;
  elem *head;
  elem *tail;

  void (*insert)(list *list, void *elem);
} list;

void init_list(list *list);
void list_add_elem(list *list, void *elem);

#endif
