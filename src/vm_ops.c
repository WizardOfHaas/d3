#include "vm_ops.h"
#include "mem.h"

vm_op vm_ops[256];

void vm_ops_init(){
	vm_op_register(&vm_ops, &op_mov, 0);
}

void vm_op_register(vm_op *vm_ops[], vm_op *op, int opcode){
	vm_ops[opcode] = op;
}

int op_mov(vm_t *machine){

}