#ifndef VM_H
#define VM_H

#include <stddef.h>

#include "mem.h"

typedef struct vm_registers_tag{
	short r0, r1, r2, r3, ip, sp, bp;
}vm_regs_t;

enum vm_status{
	VM_READY,
	VM_RUN,
	VM_DONE
};

typedef enum{
	NONE,
	OPCODE,
	VALUE,
	REGISTER,
	VALUE_POINTER,
	REGISTER_POINTER
}vm_op_mask;

static char* vm_status_names[] = {
  "READY",
  "RUN",
  "DONE"
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

typedef struct vm_ins_tag{
	char op_mask;
	char op;

	char arg0_mask;
	short arg0;

	char arg1_mask;
	short arg1;
} vm_ins;

void init_vmm();
vm_ins *vm_get_instuction(vm_t *machine);
void vm_write(vm_t *machine, vm_ins *ins);
void *vm_read(vm_t *machine, char mask0, short arg0);
void *vm_parse_ins(vm_t *machine);

#endif