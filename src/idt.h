#ifndef IDT_H
#define IDT_H

#include <stddef.h>

#include "gdt.h"

//! i86 defines 256 possible interrupt handlers (0-255)
#define I86_MAX_INTERRUPTS		256

//! must be in the format 0D110, where D is descriptor type
#define I86_IDT_DESC_BIT16		0x06	//00000110
#define I86_IDT_DESC_BIT32		0x0E	//00001110
#define I86_IDT_DESC_RING1		0x40	//01000000
#define I86_IDT_DESC_RING2		0x20	//00100000
#define I86_IDT_DESC_RING3		0x60	//01100000
#define I86_IDT_DESC_PRESENT		0x80	//10000000

typedef void (*I86_IRQ_HANDLER)(void);

//! interrupt descriptor
struct idt_descriptor{
	//! bits 0-16 of interrupt routine (ir) address
	uint16_t		baseLo;
 
	//! code selector in gdt
	uint16_t		sel;
 
	//! reserved, shold be 0
	uint8_t			reserved;
 
	//! bit flags. Set with flags above
	uint8_t			flags;
 
	//! bits 16-32 of ir address
	uint16_t		baseHi;
};

struct idtr{
	//! size of the interrupt descriptor table (idt)
	uint16_t		limit;
 
	//! base address of idt
	uint32_t		base;
};

static struct idt_descriptor	_idt [I86_MAX_INTERRUPTS];
static struct idtr				_idtr;

void int_handler();
void idt_init();
int i86_install_ir(uint32_t i, uint16_t flags, uint16_t sel, void* irq);
void geninterrupt(int n);

#endif