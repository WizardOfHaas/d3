#ifndef VM_H
#define VM_H

#include <stddef.h>

#include "mem.h"

typedef struct vm_registers_tag{
	uint32_t ip, sp, bp, as, bx, cx, dx, si, di, flags;
}vm_regs_t;

enum vm_status{
	VM_READY,
	VM_RUN,
	VM_DONE
};

typedef struct vm_tag{
	char *name;
	int id;
	enum vm_status status;

	vm_regs_t registers;
	mp_t *heap;
}vm_t;

typedef struct vm_pool_tag{
	char *name;
	int next_vm_slot;
	vm_t *machines[1024];	
}vm_pool;

void init_vmm();

#endif