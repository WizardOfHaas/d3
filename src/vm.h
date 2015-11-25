#ifndef VM_H
#define VM_H

typedef struct vm_registers_tag{
	uint32_t ip, sp, bp, as, bx, cx, dx, si, di, flags;
}vm_regs_t;

enum vm_status{
	VM_RUN,
	VM_DONE
};

typedef struct vm_tag{
	char *name;
	int id;
	enum vm_status status;

	vm_regs_t registers;
	mp_t *heap;
}vm_t;

vm_t *machines[1024];

void init_vmm();

#endif