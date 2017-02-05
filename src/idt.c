#include "kernel.h"
#include "term.h"
#include "mem.h"
#include "gdt.h"
#include "idt.h"

void idt_init(){
	i86_install_ir(5, I86_IDT_DESC_PRESENT | I86_IDT_DESC_BIT32,
		0x8, (I86_IRQ_HANDLER)int_handler);

	//asm volatile("int $5");
}

struct interrupt_frame;
 
__attribute__((interrupt)) void int_handler(struct interrupt_frame* frame)
{
    term_writestring(&tty0, "int 0x005\n");
}

//! installs idtr into processors idtr register
void idt_install(){
	//_asm lidt [_idtr]
	asm volatile("lidt (%0)" :: "r"(_idtr));
}

//! default handler to catch unhandled system interrupts.
void i86_default_handler(){
	for(;;);
}

//! generate interrupt call
void geninterrupt(int n){
	/*
	_asm {
		mov al, byte ptr [n]
		mov byte ptr [genint+1], al
		jmp genint
	genint:
		int 0	// above code modifies the 0 to int number to generate
	}
	*/

	/*asm volatile(
		"mov byte ptr(%0), %%al" :: "r"(n)
	);*/
}

//! installs a new interrupt handler
int i86_install_ir(uint32_t i, uint16_t flags, uint16_t sel, void* irq){
	if (i>I86_MAX_INTERRUPTS)
		return 0;
 
	if (!irq)
		return 0;
 
	//! get base address of interrupt handler
	uint64_t		uiBase = (uint64_t)&(*irq);
 
	//! store base address into idt
	_idt[i].baseLo		=	uiBase & 0xffff;
	_idt[i].baseHi		=	(uiBase >> 16) & 0xffff;
	_idt[i].reserved	=	0;
	_idt[i].flags		=	flags;
	_idt[i].sel		=	sel;
 
	return	0;
}

//! initialize idt
int i86_idt_initialize(uint16_t codeSel){
	//! set up idtr for processor
	_idtr.limit = sizeof (struct idt_descriptor) * I86_MAX_INTERRUPTS -1;
	_idtr.base	= (uint32_t)&_idt[0];
 
	//! null out the idt
	mem_set((void*)&_idt[0], 0, 8 * I86_MAX_INTERRUPTS-1);
 
	//! register default handlers
	for (int i=0; i<I86_MAX_INTERRUPTS; i++)
		i86_install_ir (i, I86_IDT_DESC_PRESENT | I86_IDT_DESC_BIT32,
			codeSel, (I86_IRQ_HANDLER)i86_default_handler);
 
	//! install our idt
	idt_install ();
 
	return 0;
}