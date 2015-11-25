#include "mem.h"
#include "vm.h"
#include "vm_ops.h"
#include "term.h"
#include "kernel.h"

vm_pool main_vm_pool;

void init_vmm(){
	main_vm_pool.name = "Main VM Pool";
	main_vm_pool.next_vm_slot = 0;

	//Setup VM test
	vm_t test_vm;
	vm_init(test_vm, "Test VM");

	//Add test to VM pool
	vm_add_to_pool(&main_vm_pool, &test_vm);

	//Lets add some data to the heap...
	char test_data[16] = {
			0, 1, 2, 3, 4, 5, 6, 7,
			0, 0, 0, 0, 0, 0, 0, 0
		};
	vm_copy_to_heap(&test_vm, 0, &test_data, 16);

	mem_dump(&tty0, vm_get_instuction(&test_vm), 16);

	//Try to run test_vm
	//vm_run_op(&test_vm);
}

void vm_init(vm_t *machine, char *name){
	machine->name = name;
	machine->heap = malloc(1024);	
	machine->registers.ip = 0;
	machine->status = VM_READY;
}

void vm_add_to_pool(vm_pool *pool, vm_t *machine){
	pool->machines[pool->next_vm_slot] = machine;
	pool->next_vm_slot++;
}

void vm_load_heap(vm_t *machine, mp_t *heap){
	machine->heap = heap;
}

void vm_copy_to_heap(vm_t *machine, int address, void *data, size_t size){
	unsigned char *dest = (unsigned char*)machine->heap->address;

	//Not copying data to correct place?
	mem_dump(&tty0, dest, 16);
	
	mem_cpy(
		dest[address],
		data, size);
}

char *vm_get_instuction(vm_t *machine){
	return (int(*)[8])(machine->heap + machine->registers.ip);
}

void vm_run(vm_t *machine){

}