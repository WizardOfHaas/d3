#include <stdint.h>

#include "vm.h"
#include "vm_ops.h"
#include "mem.h"
#include "term.h"
#include "dasm.h"
#include "str.h"
#include "kernel.h"

int num_ops = 4;
char* ops[] = {"hlt", "mov", "push", "pop"};

char* regs[] = {"r0", "r1", "r2", "r3", "ip", "sp", "bp"};

void* dasm(char* code){
	mp_t* ins_page = malloc(255 * sizeof(ins_t));
	interim_ins_t* prog = (interim_ins_t*) ins_page->address;
	ins_t* ins = prog->ins;

	int len = strlen(code);
	int ptr = 0;
	int tok_len = 0;
	int cur_ins = 0;

	while(ptr < len + 1){
		if((code[ptr] == '\n' || code[ptr] == '\0') && code[ptr - tok_len] != '#'){
			ins[cur_ins].raw = (char*) malloc_raw(tok_len + 1);
			mem_cpy(ins[cur_ins].raw, &code[ptr - tok_len], tok_len);
			ins[cur_ins].raw[tok_len + 1] = '\0';

			parse(&ins[cur_ins]);

			tok_len = 0;
			cur_ins++;
			ptr++;
		}

		ptr++;
		tok_len++;
	}

	vm_prog_t* out = malloc_raw(255 * sizeof(vm_ins));

	for(int i = 0; i < cur_ins; i++){
		vm_ins vm_in;
		vm_in.op_mask = ins[i].op_mask;
		vm_in.op = ins[i].op;
		vm_in.arg0_mask = ins[i].m0;
		vm_in.arg0 = ins[i].a0;
		vm_in.arg1_mask = ins[i].m1;
		vm_in.arg1 = ins[i].a1;
		out->ins[i] = vm_in;
	}

	free(ins_page);

	return out->ins;
}

void parse(ins_t* ins){
	int tok_len = 0;	
	int t = 0;
	int len = strlen(ins->raw);

	for(int i = 0; i < len + 1; i++){
		if(ins->raw[i] == ' ' || ins->raw[i] == '\0'){
			ins->tokens[t] = malloc_raw(tok_len + 1);
			mem_cpy(ins->tokens[t], &ins->raw[i - tok_len], tok_len); 
			ins->tokens[t][tok_len + 1] = '\0';

			if(t == 0){
				for(int o = 0; o < num_ops; o++){
					if(strcmp(ins->tokens[0], ops[o]) == 0){
						ins->op = o;
						ins->op_mask = 0;
						break;
					}
				}
			}else{
				parse_arg(ins, t);
			}

			tok_len = 0;
			i++;
			t++;
		}

		tok_len++;
	}
}

void parse_arg(ins_t *ins, int t){
	short a = 0;
	char m = 0;

	if(ins->tokens[t][0] == '[' && strchr((char*)"risbp", ins->tokens[t][1])){
		char* reg = malloc_raw(3);
		mem_cpy(reg, &ins->tokens[t][1], 2);
		reg[3] = '\0';

		a = (short) get_reg(reg);
		m = 3;
	}else if(strchr((char*)"0123456789", ins->tokens[t][0])){
		m = 0;
		a = (short) atoi(ins->tokens[t]);
	}else if(strchr((char*)"risbp", ins->tokens[t][0])){
		char* reg = malloc_raw(3);
    mem_cpy(reg, &ins->tokens[t][0], 2);
		reg[3] = '\0';

		a = (short) get_reg(reg);
		m = 1;
	}else if(ins->tokens[t][0] == '[' && strchr((char*)"0123456789", ins->tokens[t][1])){
		char* val = malloc_raw(strlen(ins->tokens[t]) - 1);
		mem_cpy(val, &ins->tokens[t][1], strlen(ins->tokens[t]) - 2);
	
		a = (short) atoi(val);
		m = 2;
	}

	if(t == 1){
		ins->m0 = m;
		ins->a0 = a;
	}else if(t == 2){
		ins->m1 = m;
		ins->a1 = a;
	}
}

int get_reg(char* reg){
	int r = -1;
	for(int i = 0; i < 7; i++){
		if(strcmp(reg, regs[i]) == 0){
			r = i;
			break;
		}
	}

	return r;
}