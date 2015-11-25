#include "term.h"
#include "vm.h"
#include "vm_ops.h"
#include "mem.h"
#include "kernel.h"

vm_op vm_ops[256];

void vm_ops_init(){
	vm_op_register(&vm_ops, &op_halt, 0);
	vm_op_register(&vm_ops, &op_mov, 1);
}

void vm_op_register(vm_op *vm_ops[], vm_op *op, int opcode){
	vm_ops[opcode] = op;
}

void vm_run_op(vm_t *machine){
	int *instruction = vm_get_instuction(machine);
	mem_dump(&tty0, instruction, 8);
}

int op_halt(vm_t *machine){
	machine->status = VM_DONE;

	return 0;
}

int op_mov(vm_t *machine){
	int *instruction = vm_get_instuction(machine);

	return 0;
}