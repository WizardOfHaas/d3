#ifndef VM_OPS_H
#define VM_OPS_H

#include "vm.h"

typedef struct vm_op_tag{
	int (*op)(struct vm_t *machine);
}vm_op;

void vm_op_init();
void vm_op_register(vm_op *vm_ops[], vm_op *op, int opcode);
int op_mov(vm_t *machine);

#endif