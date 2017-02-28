#include "oneheader.h"
#include "libfuncs.h"

char* typeStrings[] = {
	"number",
	"string",
	"bool",
	"userfunc",
	"libfunc",
	"nil",
	"undef"
};



int jgt_impl(double x, double y) {
	return x > y;
}

int jge_impl(double x, double y) {
	return x >= y;
}

int jlt_impl(double x, double y) {
	return x < y;
}

int jle_impl(double x, double y) {
	return x <= y;
}

cond_jump_t conditionalJumps[] = {
	jgt_impl,
	jge_impl,
	jlt_impl,
	jle_impl
};



void execute_cmp(instruction* instr) {
	printf("execute_cmp();\n");
	assert(instr->result->type == label_a);

	avm_memcell* rv1 = avm_translate_operand(instr->arg1, &ax);
	avm_memcell* rv2 = avm_translate_operand(instr->arg2, &bx);

	assert(rv1 && rv2);

	if (rv1->type != number_m || rv2->type != number_m) {
		avm_error("cannot compare 2 non-numbers!");
	}
	else {
		unsigned result = 0;
		switch (instr->opcode) {
			case jgt_v: result = conditionalJumps[0](rv1->data.numVal, rv2->data.numVal); break;
			case jge_v: result = conditionalJumps[1](rv1->data.numVal, rv2->data.numVal); break;
			case jlt_v: result = conditionalJumps[2](rv1->data.numVal, rv2->data.numVal); break;
			case jle_v: result = conditionalJumps[3](rv1->data.numVal, rv2->data.numVal); break;
			default: fprintf(stderr, "execute_cmp: opcode out of bounds\nil"); assert(0);
		}
		if (result && !executionFinished) {
			pc = instr->result->val;
		}
	}
}


void execute_jump(instruction* instr) {
	printf("execute_jump();\n");
	assert(instr);
	assert(instr->result->type == label_a);

	pc = instr->result->val;
}


void execute_jeq(instruction* instr) {
	printf("execute_jeq();\n");
	assert(instr->result->type == label_a);

	avm_memcell* rv1 = avm_translate_operand(instr->arg1, &ax);
	avm_memcell* rv2 = avm_translate_operand(instr->arg2, &bx);

	unsigned char result = 0;

	if (rv1->type == undef_m || rv2->type == undef_m) {
		avm_error("\'undef\' involved in equality!");
	}
	else if (rv1->type == nil_m || rv2->type == nil_m) {
		result = rv1->type == nil_m && rv2->type == nil_m;
	}
	else if (rv1->type == bool_m || rv2->type == bool_m) {
		result = (avm_tobool(rv1) == avm_tobool(rv2));
	}
	else if (rv1->type != rv2->type) {
		avm_error("%s == %s is illegal!", typeStrings[rv1->type], typeStrings[rv2->type]);
	}
	else {
		/* Equality check with dispatching */
		switch (rv1->type) {
			case number_m: result = (rv1->data.numVal == rv2->data.numVal); break;
			case string_m: result = !(strcmp(rv1->data.strVal,rv2->data.strVal)); break;
			case table_m: result = (rv1->data.tableVal == rv2->data.tableVal); break;
			case userfunc_m: result = (rv1->data.funcVal == rv2->data.funcVal); break;
			case libfunc_m: result = !(strcmp(rv1->data.libfuncVal,rv2->data.libfuncVal)); break;
			default: fprintf(stderr, "execute_jeq: type mis-match\n"); assert(0);
		}
	}
	if (!executionFinished && result) {
		pc = instr->result->val;
	}
}


void execute_jne(instruction* instr) {
	printf("execute_jne();\n");
	assert(instr->result->type == label_a);

	avm_memcell* rv1 = avm_translate_operand(instr->arg1, &ax);
	avm_memcell* rv2 = avm_translate_operand(instr->arg2, &bx);

	unsigned char result = 0;

	if (rv1->type == undef_m || rv2->type == undef_m) {
		avm_error("\'undef\' involved in equality!");
	}
	else if (rv1->type == nil_m || rv2->type == nil_m) {
		result = rv1->type == nil_m && rv2->type == nil_m;
	}
	else if (rv1->type == bool_m || rv2->type == bool_m) {
		result = (avm_tobool(rv1) == avm_tobool(rv2));
	}
	else if (rv1->type != rv2->type) {
		avm_error("%s == %s is illegal!", typeStrings[rv1->type], typeStrings[rv2->type]);
	}
	else {
		/* Equality check with dispatching */
		switch (rv1->type) {
			case number_m: result = (rv1->data.numVal == rv2->data.numVal); break;
			case string_m: result = !(strcmp(rv1->data.strVal,rv2->data.strVal)); break;
			case table_m: result = (rv1->data.tableVal == rv2->data.tableVal); break;
			case userfunc_m: result = (rv1->data.funcVal == rv2->data.funcVal); break;
			case libfunc_m: result = !(strcmp(rv1->data.libfuncVal,rv2->data.libfuncVal)); break;
			default: fprintf(stderr, "execute_jne: type mis-match\n"); assert(0);
		}
	}
	if (!executionFinished && !result) {
		pc = instr->result->val;
	}
}