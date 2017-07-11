#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "multiboot.h"
#include "term.h"
#include "mem.h"
#include "str.h"
#include "task.h"
#include "vm.h"
#include "vm_ops.h"
#include "gdt.h"
#include "idt.h"
#include "dasm.h"
#include "time.h"

#include "io.h"
#include "fat12.h"

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
  term_init(&tty0, 80, 25, COLOR_LIGHT_GREY, COLOR_BLACK, (uint16_t*) 0xB8000);

  //Splash...
  term_writestring(&tty0, "d3 Booting...\n\n");

  //Init GDT...
  /*term_writestring(&tty0, "Initializing GDT...");
  gdt_init();
  term_writestring(&tty0, "[OK]\n");

  //Init IDT...
  term_writestring(&tty0, "Initializing IDT/ISRs...");
  init_idt();
  term_writestring(&tty0, "[OK]\n");*/

  //Init memory manager and print some stats...
  term_writestring(&tty0, "Initializing memory manager...");
  init_mm(mbd);
  term_writestring(&tty0, "[OK]\n");

  //Init task manager...
  /*term_writestring(&tty0, "Initializing task manager...");
  init_tasker();
  term_writestring(&tty0, "[OK]\n");*/

  //Init vm manager...
  //term_writestring(&tty0, "Initializing vm manager...");
  //init_vmm();
  //term_writestring(&tty0, "[OK]\n");

  //Init fat12 driver
  //term_writestring(&tty0, "Initializing fat12 driver...");
  //init_fat12();
  //term_writestring(&tty0, "[OK]\n");

  term_writestring(&tty0, itoa(get_time(), 10));
  term_writestring(&tty0, "\n");
}
