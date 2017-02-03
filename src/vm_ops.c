#include "term.h"
#include "vm.h"
#include "vm_ops.h"
#include "mem.h"
#include "kernel.h"

vm_op vm_ops[256];

void vm_ops_init(){
	vm_op_register(&vm_ops, &op_halt, 0);
	vm_op_register(&vm_ops, &op_mov, 1);
	vm_op_register(&vm_ops, &op_push, 2);
	vm_op_register(&vm_ops, &op_pop, 3);
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

	return 0;
}

int op_push(vm_t *machine, char *instruction){
	unsigned char* heap = (unsigned char*) machine->heap->address;
	heap[machine->registers.sp] = instruction[5];
	mem_dump(&tty0, machine->heap->address, 16);

	return 0;
}

int op_pop(vm_t *machine, char *instruction){

	return 0;
}
