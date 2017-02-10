#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

typedef struct vm_ins_tag{
	char op_mask;
	char op;
	char m0;
	short a0;
	char m1;
	short a1;
}vm_ins_t;

void parse(ins_t* ins);
void parse_arg(ins_t *ins, int t);
int get_reg(char* reg);

int num_ops = 4;
char* ops[] = {"hlt", "mov", "push", "pop"};

char* regs[] = {"r0", "r1", "r2", "r3", "ip", "sp", "bp"};

char* code = "mov r0 10\nmov sp [12]\npush r0\npop r1\nmov [r2] r1\nhlt";

ins_t ins[255];

int main(int argc, char *argv[]){
	int len = strlen(code);
	int ptr = 0;
	int tok_len = 0;
	int cur_ins = 0;

	while(ptr < len + 1){
		if((code[ptr] == '\n' || code[ptr] == '\0') && code[ptr - tok_len] != '#'){
			ins[cur_ins].raw = (char*) malloc(tok_len + 1);
			memcpy(ins[cur_ins].raw, &code[ptr - tok_len], tok_len);
			ins[cur_ins].raw[tok_len + 1] = '\0';

			parse(&ins[cur_ins]);

			tok_len = 0;
			cur_ins++;
			ptr++;
		}

		ptr++;
		tok_len++;
	}

	FILE *ptr_file = fopen("out.d3", "wb");

	vm_ins_t out[cur_ins];

	for(int i = 0; i < cur_ins; i++){
		vm_ins_t vm_ins;
		vm_ins.op_mask = ins[i].op_mask;
		vm_ins.op = ins[i].op;
		vm_ins.m0 = ins[i].m0;
		vm_ins.a0 = ins[i].a0;
		vm_ins.m1 = ins[i].m1;
		vm_ins.a1 = ins[i].a1;
		out[i] = vm_ins;
	}

	fwrite(&out, sizeof(vm_ins_t), cur_ins, ptr_file);
}

void parse(ins_t* ins){
	int tok_len = 0;	
	int t = 0;
	int len = strlen(ins->raw);

	for(int i = 0; i < len + 1; i++){
		if(ins->raw[i] == ' ' || ins->raw[i] == '\0'){
			ins->tokens[t] = malloc(tok_len + 1);
			memcpy(ins->tokens[t], &ins->raw[i - tok_len], tok_len); 
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
		char* reg = malloc(3);
		memcpy(reg, &ins->tokens[t][1], 2);
		reg[3] = '\0';

		a = (short) get_reg(reg);
		m = 3;
	}else if(strchr((char*)"0123456789", ins->tokens[t][0])){
		m = 0;
		a = (short) atoi(ins->tokens[t]);
	}else if(strchr((char*)"risbp", ins->tokens[t][0])){
		char* reg = malloc(3);
    memcpy(reg, &ins->tokens[t][0], 2);
		reg[3] = '\0';

		a = (short) get_reg(reg);
		m = 1;
	}else if(ins->tokens[t][0] == '[' && strchr((char*)"0123456789", ins->tokens[t][1])){
		char* val = malloc(strlen(ins->tokens[t]) - 1);
		memcpy(val, &ins->tokens[t][1], strlen(ins->tokens[t]) - 2);
	
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
