#include "kernel.h"
#include "term.h"
#include "mem.h"
#include "gdt.h"
#include "idt.h"

void init_idt(){
	idt_set_gate(32, (unsigned)isr_wrapper, 0x08, 0x8E);
	//__asm__("int $32");
}

void idt_load(){
	__asm__("lidt (%0)" :: "m"(idtp));
}

void idt_set_gate(unsigned char num, unsigned long base, unsigned short sel, unsigned char flags){
    /* The interrupt routine's base address */
    idt[num].base_lo = (base & 0xFFFF);
    idt[num].base_hi = (base >> 16) & 0xFFFF;

    /* The segment or 'selector' that this IDT entry will use
    *  is set here, along with any access flags */
    idt[num].sel = sel;
    idt[num].always0 = 0;
    idt[num].flags = flags;
}

void idt_install(){
    /* Sets the special IDT pointer up, just like in 'gdt.c' */
    idtp.limit = (sizeof(struct idt_entry) * 256) - 1;
    idtp.base = &idt;

    /* Clear out the entire IDT, initializing it to zeros */
    mem_set(&idt, 0, sizeof(struct idt_entry) * 256);

    /* Add any new ISRs to the IDT here using idt_set_gate */

    /* Points the processor's internal register to the new IDT */
    idt_load();
}

void isr_stub(){
    __asm__("pushal");
    term_writestring(&tty0, "ISR!\n");
    __asm__("popal; leave; iret"); /* BLACK MAGIC! */
}

void isr_handler(){
	term_writestring(&tty0, "ISR!\n");
}