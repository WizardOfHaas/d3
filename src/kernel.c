#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "multiboot.h"
#include "term.h"
#include "mem.h"
#include "str.h"

term_t tty0;

void cmain(multiboot_info_t* mbd)
{
  //Initialize first terminal
  term_init(&tty0, 80, 25, COLOR_GREEN, COLOR_BLACK, (uint16_t*) 0xB8000);
 
  //Splash...
  term_writestring(&tty0, "d3 Booting...\n");

  //Init memory manager and print some stats...
  init_mm(mbd);
  term_writestring(&tty0, itoa(mem_size/1024, 10));
  term_writestring(&tty0, "Mb memory detected...\nUsed Segments:\n");

  for(int i = 0; i < used.top; i++){
    multiboot_memory_map_t* mmap = used.data[i];
    term_writestring(&tty0, itoa(mmap->addr, 16));
    term_writestring(&tty0, "\n");
  }
}
