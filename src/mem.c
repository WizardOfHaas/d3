#ifndef TERM_H
#define TERM_H
#include "term.h"
#endif

#include "multiboot.h"
#include "str.h"
#include "list.h"
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
    
    //Setup first free struct...
    mm_free = (mp_t*)(unsigned int)free_top->addr;
    mm_free->address = (void*)mm_heap_top;
    mm_free->size = free_top->len - mm_heap_size;
    mm_free->next = NULL;

    //And reserve a small heap as first used struct
    mm_used = (mp_t*)((unsigned int)free_top->addr + sizeof(mp_t));
    mm_used->address = (void*)mm_heap_bottom;
    mm_used->size = mm_heap_size;
    mm_used->next = NULL;
  }else{
    //Rats! An error should be here!
    kernel_panic("init_mm: Rats! Not enough free memory!");
  }

  /*
    Break free memory into chunks for buddy fit
    Sizes: n^2 [0, 64]Kb    
   */
  //Get amount of free after mm_heap reserved
  size_t free_size = free_top->len - mm_heap_size;
  size_t max_buddy_size = (64*1024) + (7*sizeof(mp_t));
  //How many buddies can I start with?
  size_t num_buddies = free_size/max_buddy_size;

  term_writestring(&tty0, "Free Memory:");
  term_writestring(&tty0, itoa(free_size, 10));

  term_writestring(&tty0, "\nMax Buddy Size:");
  term_writestring(&tty0, itoa(max_buddy_size, 10));

  term_writestring(&tty0, "\nNumber of Big Buddies:");
  term_writestring(&tty0, itoa(num_buddies, 10));

  if(num_buddies < 1){
    kernel_panic("init_mm: Darn! Not enough memory to have any buddies!");
  }

  init_list(mm_list, list_insert_mm_t);
  
  //Lets start filling in the buddies!
  for(int i = 0; i < num_buddies; i++){
    //Calc start of this buddy chunk
    uint64_t buddy_addr = free_top->addr + i*max_buddy_size;
    
    //Break this into smaller buddies...using structs!
    size_t new_buddy_size = 64;
    while(new_buddy_size >= 1){
      //Build up the buddy struct...
      mp_t* buddy = (mp_t*)buddy_addr;
      uint64_t address = buddy_addr + sizeof(mp_t) + new_buddy_size*1024;
      buddy->address = (void*)address;
      buddy->size = new_buddy_size*1024;

      //Put it in the list and move onto the next one!
      list_add_elem(mm_list, buddy);
      new_buddy_size = new_buddy_size/2;
    }
  }
 }

void* add_buddy(mp_t* entry, mp_t* new){
  /*mp_t* current = entry;
  while(current->next != NULL){    
    current = current->next;
  }

  current->next = new;
  new->next = NULL;*/

  new->next = entry;
  return new;
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
  term_writestring(term, ":");

  while(i <= size){
    term_writestring(term, itoa(data[i], 16));
    i++;
  }
}
