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
	unsigned char *ins = (unsigned char*) vm_get_instuction(machine);
	vm_ops[ins[1]].op(machine, ins);
}

int op_halt(vm_t *machine, char *instruction){
	machine->status = VM_DONE;

	return 0;
}

int op_mov(vm_t *machine, char *instruction){
	term_writestring(&tty0, "MOV\n");

	return 0;
}