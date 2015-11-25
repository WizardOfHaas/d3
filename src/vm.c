#include "mem.h"
#include "vm.h"
#include "vm_ops.h"

vm_pool main_vm_pool;

void init_vmm(){
	main_vm_pool.name = "Main VM Pool";
	main_vm_pool.next_vm_slot = 0;

	vm_t test_vm;
	vm_init(test_vm, "Test VM");
	vm_add_to_pool(&main_vm_pool, &test_vm);
}

void vm_init(vm_t *machine, char *name){
	machine->name = name;
	machine->heap = malloc(1024);
	machine->status = VM_READY;
}

void vm_add_to_pool(vm_pool *pool, vm_t *machine){
	pool->machines[pool->next_vm_slot] = machine;
	pool->next_vm_slot++;
}