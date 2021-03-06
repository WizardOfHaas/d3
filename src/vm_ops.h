#ifndef VM_OPS_H
#define VM_OPS_H

#include "vm.h"

typedef struct vm_op_tag{
	int (*op)(struct vm_t *machine, vm_ins *instruction);
}vm_op;

void vm_op_init();
void vm_op_register(vm_op *vm_ops[], vm_op *op, int opcode);
void vm_run_op(vm_t *machine);

//VM operations
int op_halt(vm_t *machine, vm_ins *instruction);
int op_mov(vm_t *machine, vm_ins *instruction);
int op_push(vm_t *machine, vm_ins *instruction);
int op_pop(vm_t *machine, vm_ins *instruction);
int op_cmp(vm_t *machine, vm_ins *instruction);

#endif