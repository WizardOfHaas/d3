#ifndef MEM_H
#define MEM_H

#include <stddef.h>

#include "multiboot.h"
#include "term.h"

typedef struct memory_page_tag{
  struct mp_t *prev;
  struct mp_t *next;

  void *address;
  size_t size;
}mp_t;

typedef struct stack_tag{
  void *data[32];
  size_t top;
}stack_t;

stack_t mb_free;
stack_t used;
int mem_size;

mp_t mm_free;
mp_t mm_used;

void* mem_cpy(void* dstptr, const void* srcptr, size_t size);
void init_mm(multiboot_info_t* mbd);
mp_t* malloc(size_t size);
void* malloc_raw(size_t size);
void free(mp_t *entry);
void add_buddy(mp_t* entry, mp_t* new);
void mem_dump(term_t* term, void* addr, size_t size);
void print_mem_map();
void mem_set(void *dst, char val, size_t size);

void init_stack(stack_t* stack);
void stack_push(stack_t* stack, void* el);
void* stack_pop(stack_t* stack);

#endif
