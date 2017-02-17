#ifndef VM_H
#define VM_H

#include <stddef.h>

#include "mem.h"

#define FLAG_EQ 0x01;
#define FLAG_GTE 0x02;
#define FLAG_LTE 0x04;
#define FLAG_ZERO 0x08;
#define FLAG_ERR 0x10;

//const unsigned char flag_eq = 0x01; // hex for 0000 0001
//const unsigned char flag_gte = 0x02; // hex for 0000 0010
//const unsigned char flag_lte = 0x04; // hex for 0000 0100
//const unsigned char flag_zero = 0x08; // hex for 0000 1000
//const unsigned char flag_err = 0x10; // hex for 0001 0000
//const unsigned char option6 = 0x20; // hex for 0010 0000
//const unsigned char option7 = 0x40; // hex for 0100 0000
//const unsigned char option8 = 0x80; // hex for 1000 0000

typedef struct vm_registers_tag{
	short r0, r1, r2, r3, ip, sp, bp;
	char flags;
}vm_regs_t;

enum vm_status{
	VM_READY,
	VM_RUN,
	VM_DONE
};

typedef enum{
	NONE,
	OPCODE,
	VALUE,
	REGISTER,
	VALUE_POINTER,
	REGISTER_POINTER
}vm_op_mask;

static char* vm_status_names[] = {
  "READY",
  "RUN",
  "DONE"
};

typedef struct vm_tag{
	char *name;
	int id;
	enum vm_status status;

	vm_regs_t registers;
	mp_t *heap;
}vm_t;

typedef struct vm_pool_tag{
	char *name;
	int next_vm_slot;
	vm_t *machines[1024];	
}vm_pool;

typedef struct vm_ins_tag{
	char op_mask;
	char op;

	char arg0_mask;
	short arg0;

	char arg1_mask;
	short arg1;
} vm_ins;

typedef struct vm_prog_tag{
	vm_ins ins[255];
} vm_prog_t;

void init_vmm();
vm_ins *vm_get_instuction(vm_t *machine);
void vm_write(vm_t *machine, char mask0, short arg0, short val);
short vm_read(vm_t *machine, char mask0, short arg0);
void *vm_parse_ins(vm_t *machine);

#endif