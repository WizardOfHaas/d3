#include "mem.h"
#include "vm.h"
#include "vm_ops.h"
#include "term.h"
#include "kernel.h"

vm_pool main_vm_pool;

void init_vmm(){
	//Init default VM optcode table
	vm_ops_init();

	main_vm_pool.name = "Main VM Pool";
	main_vm_pool.next_vm_slot = 0;

	//Setup VM test
	vm_t test_vm;
	vm_init(&test_vm, "Test VM");

	//Add test to VM pool
	vm_add_to_pool(&main_vm_pool, &test_vm);

	//Lets add some data to the heap...
	vm_ins test_ins;
	test_ins.op_mask = 0;
	test_ins.op = 2;
	test_ins.arg0_mask = 3;
	test_ins.arg0 = 1;
	test_ins.arg1_mask = 0;
	test_ins.arg1 = 6;

	short test_data[16] = {
		6, 6, 6, 6,
		6, 6, 6, 6
	};

	vm_copy_to_heap(&test_vm, 0, &test_ins, 8);
	vm_copy_to_heap(&test_vm, 8, &test_data, 16);

	//Try to run test_vm
	vm_run_op(&test_vm);
}

void vm_init(vm_t *machine, char *name){
	machine->name = name;
	machine->heap = malloc(1024);	
	machine->registers.ip = 0;
	machine->registers.sp = 0;
	machine->registers.bp = 0;
	machine->registers.r0 = 8;
	machine->registers.r1 = 1;
	machine->registers.r2 = 2;
	machine->registers.r3 = 3;
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

	dest += address;

	mem_cpy(
		dest,
		data, size
	);
}

vm_ins *vm_get_instuction(vm_t *machine){
	uint32_t *heap = (uint32_t)machine->heap->address;
	uint32_t ip = machine->registers.ip;

	//mask,op,mask,val,mask,val
	//1,   1, 1    2,  1,   2    <--bytes

	return (vm_ins*)(heap + ip);
}

void vm_run(vm_t *machine){
	vm_run_op(machine);
}

void vm_dump_registers(term_t *term, vm_t *machine){
	term_writestring(term, "\n  ip: ");
	term_writestring(term, itoa(machine->registers.ip));
	term_writestring(term, "  sp: ");
	term_writestring(term, itoa(machine->registers.sp));
	term_writestring(term, "  bp: ");
	term_writestring(term, itoa(machine->registers.bp));

	term_writestring(term, "\n  r0: ");
	term_writestring(term, itoa(machine->registers.r0));
	term_writestring(term, "  r1: ");
	term_writestring(term, itoa(machine->registers.r1));
	term_writestring(term, "  r2: ");
	term_writestring(term, itoa(machine->registers.r2));
	term_writestring(term, "  r3: ");
	term_writestring(term, itoa(machine->registers.r3));

	term_writestring(term, "\n");
}

void vm_write(vm_t *machine, vm_ins *ins){

}

short vm_read(vm_t *machine, char mask0, short arg0){
	short val = 0;

	if(mask0 == 0){ //Raw value
		val = arg0;	
	}else if(mask0 == 1){ //Register value
		unsigned char* regs = (unsigned char*) &machine->registers;
		val = (short) regs[2 * arg0];
	}else if(mask0 == 2){ //Raw pointer
		unsigned char* heap = (unsigned char*) &machine->heap;
		mem_dump(&tty0, heap, 16);
		val = (short) heap[arg0 + machine->registers.bp];
	}else if (mask0 == 3){ //Register pointer
		unsigned char* regs = (unsigned char*) &machine->registers;
		unsigned char* heap = (unsigned char*) &machine->heap;
		short p = (short) regs[2 * arg0];
		val = (short) heap[p+ machine->registers.bp];
	}

	return val;
}

void *vm_parse_ins(vm_t *machine){
	unsigned char *ins = (unsigned char*) vm_get_instuction(machine);


}