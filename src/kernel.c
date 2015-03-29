#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "multiboot.h"
#include "term.h"
#include "mem.h"
#include "str.h"

term_t tty0;

void kernel_panic(const char* c){
  term_setcolor(&tty0, make_color(COLOR_LIGHT_GREY, COLOR_RED));

  term_writestring(&tty0, "\n**KERNEL PANIC**:\n     ");
  term_writestring(&tty0, c);
  
  while(1){}
}

void cmain(multiboot_info_t* mbd)
{
  //Initialize first terminal
  term_init(&tty0, 80, 25, COLOR_GREEN, COLOR_BLACK, (uint16_t*) 0xB8000);
 
  //Splash...
  term_writestring(&tty0, "d3 Booting...\n");

  //Init memory manager and print some stats...
  init_mm(mbd);
  
  mp_t* temp = mm_free->next;
  for(int i = 0; i < 10 && temp->next != NULL; i++){    
    term_writestring(&tty0, "\n---Sample Buddy---\nAddress:");
    term_writestring(&tty0, itoa(temp->address, 10));

    term_writestring(&tty0, "\nSize:");
    term_writestring(&tty0, itoa(temp->size, 10));

    temp = temp->next;
  }
}
