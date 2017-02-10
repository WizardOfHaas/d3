#ifndef DASM_H
#define DASM_H

#include <stddef.h>

typedef struct ins_tag{
	char* raw;
	char* tokens[3];

	char op_mask;
	char op;
	
	char m0;
	short a0;
	
	char m1;
	short a1;
}ins_t;

typedef struct interim_ins_tag{
	ins_t ins[255];
} interim_ins_t;

void parse(ins_t* ins);
void parse_arg(ins_t *ins, int t);
int get_reg(char* reg);
void* dasm(char* code);

#endif