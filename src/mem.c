#ifndef TERM_H
#define TERM_H
#include "term.h"
#endif

#include "multiboot.h"
#include "str.h"
#include "mem.h"
#include "kernel.h"

stack_t free;
stack_t used;

mp_t* mm_free;
mp_t* mm_used;

uint64_t mm_heap_bottom;
uint64_t mm_heap_top;
uint64_t mm_heap_size = sizeof(mp_t)*256;

int mem_size;

void* movemem(void* dstptr, const void* srcptr, size_t size){
  /*Move data at &srcpty to &dstptr.
    No garuntee of what lies at &srcptr when done.    
    Safe if &srcptr and &dstprt are close together.*/
  unsigned char* dst = (unsigned char*) dstptr;
  const unsigned char* src = (const unsigned char*) srcptr;

  if(dst < src){
    for(size_t i = 0; i < size; i++){
      dst[i] = src[i];
    }
  }else{
    for(size_t i = size; i != 0; i--){
      dst[i-1] = src[i-1];
    }
  }

  return dstptr;
}

void init_mm(multiboot_info_t* mbd){
  init_stack(&free);  
  init_stack(&used);

  mem_size = mbd->mem_lower + mbd->mem_upper + 1024;

  //Grab and prep to traverse mmap from grub
  multiboot_memory_map_t* mmap = mbd->mmap_addr;

  while(mmap < mbd->mmap_addr + mbd->mmap_length){
    //Make into stacks to translate later...
    if(mmap->type == 1){
      stack_push(&free, &mmap);
    }else{
      stack_push(&used, &mmap);
    }

    mmap = (multiboot_memory_map_t*) ((unsigned int)mmap + mmap->size + sizeof(unsigned int));
  }

  //Find somewhere to start a linked list for free/used management...
  multiboot_memory_map_t* free_top = free.data[free.top - 1];
  if(free_top->len > mm_heap_size){
    //Its big enough!
    mm_heap_bottom = free_top->addr;
    mm_heap_top = mm_heap_bottom + mm_heap_size;
    
    //Setup first entry in free list
    mm_free->next = NULL;
    mm_free->prev = NULL;
    mm_free->address = (void*)mm_heap_top;
    mm_free->size = free_top->len - mm_heap_size;
    
    //Setup first entry in used list
    mm_used->next = NULL;
    mm_used->prev = NULL;
    mm_used->address = (void*)mm_heap_bottom;
    mm_used->size = mm_heap_size;
  }else{
    //Rats! An error should be here!
    kernel_panic("init_mm: Rats! Not enough free memory!");
  }

  mp_t *test;
  test->size = 1024;  
  add_buddy(mm_free, test);
}

void* malloc(size_t size){
  mp_t *temp = mm_free;
  while(temp->next != NULL){
    temp = temp->next;
  }
}

void add_buddy(mp_t* entry, mp_t* new){
  new->prev = entry;
  new->next = entry->next;
  entry->next = new;
}

void init_stack(stack_t* stack){
  stack->top = 0;
}

void stack_push(stack_t* stack, void* el){
  stack->data[stack->top] = *((int*)el);
  stack->top++;
}

void* stack_pop(stack_t* stack){
  stack->top--;
  return stack->data[stack->top + 1];
}

void mem_dump(term_t* term, void* addr, size_t size){
  unsigned char* data = (unsigned char*) addr;
  size_t i = 0;
    
  term_writestring(term, itoa(&addr, 16));
  term_writestring(term, "|");

  while(i < size){
    term_writestring(term, itoa(data[i], 16));
    term_writestring(term, ":");
    i++;
  }
  term_writestring(term, "\n");
}
