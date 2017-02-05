#ifndef GDT_H
#define GDT_H

#include <stddef.h>

struct gdt_descriptor {
	//! bits 0-15 of segment limit
	uint16_t		limit;

	//! bits 0-23 of base address
	uint16_t		baseLo;
	uint8_t			baseMid;

	//! descriptor access flags
	uint8_t			flags;

	uint8_t			grand;

	//! bits 24-32 of base address
	uint8_t			baseHi;
};

struct gdtr{
	//! size of gdt
	uint16_t		m_limit;
 
	//! base address of gdt
	uint32_t		m_base;
};

#define MAX_DESCRIPTORS					3
static struct gdt_descriptor _gdt[MAX_DESCRIPTORS];
static struct gdtr _gdtr;

#endif