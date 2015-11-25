#include "mem.h"
#include "vm.h"

int vm_last_id = 0;

void init_vmm(){
	vm_t test_vm;

	create_vm(test_vm, "Test VM");
}

void create_vm(vm_t *machine, char *name){
	machine->name = name;

	machine->heap = malloc(1024);

	machine->state = VM_READY;	
}