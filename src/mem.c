#include "multiboot.h"
#include "kernel.h"
#include "mem.h"

mp_t* free;
mp_t* used;
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
  mem_size = mbd->mem_lower + mbd->mem_upper + 1;

  //GET MMAP
  multiboot_memory_map_t* mmap = mbd->mmap_addr;

  while(mmap < mbd->mmap_addr + mbd->mmap_length){
    //REFORMAT!
    //term_writestring(&tty0, itoa(mmap->addr, 10));
    //term_writestring(&tty0, ":");
    //term_writestring(&tty0, itoa(mmap->base_addr_high, 10));
    //term_writestring(&tty0, "\n");
    mmap = (multiboot_memory_map_t*) ((unsigned int)mmap + mmap->size + sizeof(unsigned int));
  }

  //LOAD INTO FREE/USED LINKED LIST STRUCTS
  //BREAK FREE INTO CHUNKS BASED ON FIBO
}
