#include "oneheader.h"

extern double*		numConsts;
extern char**		stringConsts;
extern char**		namedLibfuncs;
extern userfunc*	userFuncs;
extern unsigned		currentNumConst, currentStringConst, currentNamedLibfunc, currentUserFunc;
extern unsigned		totalNumConsts, totalStringConsts, totalNamedLibfuncs, totalUserFuncs;

extern library_func_t libFuncs[];
extern unsigned totalActuals;




/**
 * @brief
 */
unsigned avm_get_envvalue(unsigned i) {
	printf("avm_get_envvalue(%d)\n", i);
	printf("stack[%u].type = %u\n", i, stack[i].type);
	assert(stack[i].type == number_m);
	unsigned val = (unsigned)stack[i].data.numVal;
	assert(stack[i].data.numVal == (double)val);
	return val;
}


/**
 * @brief
 */
void avm_push_envvalue(unsigned val) {
	printf("avm_push_envvalue(%u);\n", val);
	printf("top = %u\n", top);
	stack[top].type = number_m;
	stack[top].data.numVal = val;
	avm_dec_top();
}


/**
 * @brief
 */
userfunc* avm_getfuncinfo(unsigned address) {
	if (address < 0 || address > currentUserFunc) {
		fprintf(stderr, "avm_getfuncinfo: address out of bounds!!!");
		assert(0);
	}
	return &userFuncs[address];
}



/**
 * @brief
 */
library_func_t avm_getlibraryfunc(char* id) {
	if (!strcmp(id, "print")) {
		return libFuncs[0];
	}
	else if (!strcmp(id, "objectmemberkeys")) {
		return libFuncs[1];
	}
	else if (!strcmp(id, "objecttotalmembers")) {
		return libFuncs[2];
	}
	else if (!strcmp(id, "objectcopy")) {
		return libFuncs[3];
	}
	else if (!strcmp(id, "argument")) {
		return libFuncs[4];
	}
	else if (!strcmp(id, "typeof")) {
		return libFuncs[5];
	}
	else if (!strcmp(id, "strtonum")) {
		return libFuncs[6];
	}
	else if (!strcmp(id, "sqrt")) {
		return libFuncs[7];
	}
	else if (!strcmp(id, "cos")) {
		return libFuncs[8];
	}
	else if (!strcmp(id, "sin")) {
		return libFuncs[9];
	}
}




/**
 * @brief
 */
void avm_dec_top() {
	printf("avm_dec_top();\n");
	if (!top) {
		avm_error("Stack overflow!\n");
		executionFinished = 1;
	}
	else {
		--top;
	}
}



/**
 * @brief
 */
void avm_callsaveenviroment() {
	printf("avm_callsaveenviroment();\n");
	avm_push_envvalue(totalActuals);
	avm_push_envvalue(pc + 1);
	avm_push_envvalue(top + totalActuals + 2);
	avm_push_envvalue(topsp);
}



/**
 * @brief
 */
void avm_calllibfunc(char* id) {
	printf("avm_calllibfunc();\n");
	library_func_t f = avm_getlibraryfunc(id);
	if (!f) {
		avm_error("unsupported libfunc \'%s\' called!\n", id);
		executionFinished = 1;
	}
	else {
		/* Notice that enter function and exit function
		 * are called manually!
		 */
		topsp = top;			/* Enter function sequence. No stack locals. */
		totalActuals = 0;
		(*f)();					/* Call library function. */
		if (!executionFinished) {		/* An error may naturally occur inside. */
			execute_funcexit((instruction*)0);		/* Return sequence. */
		}
	}
}



/**
 * @brief
 */
unsigned avm_totalactuals() {
	printf("avm_totalactuals();\n");
	return avm_get_envvalue(topsp + AVM_NUMACTUALS_OFFSET);
}

avm_memcell* avm_getactual(unsigned i) {
	printf("avm_getactual();\n");
	assert(i < avm_totalactuals());
	return &stack[topsp + AVM_STACKENV_SIZE + 1 + i];
}

void execute_call(instruction* instr) {
	printf("execute_call();\n");
	avm_memcell* func = avm_translate_operand(instr->result, &ax);
	assert(func);
	avm_callsaveenviroment();

	switch (func->type) {
		case userfunc_m:
			pc = func->data.funcVal;
			assert(pc < AVM_ENDING_PC);
			assert(instruction_table[pc].opcode == funcenter_v);
			break;
		case string_m: avm_calllibfunc(func->data.strVal); break;
		case libfunc_m: avm_calllibfunc(func->data.libfuncVal); break;
		default: {
			avm_error("cannot bind \'%s\' to function!", avm_tostring(func));
			executionFinished = 1;
		}
	}
}


/**
 * @brief
 */
void execute_pusharg(instruction* instr) {
	printf("execute_pusharg();\n");
	avm_memcell* arg = avm_translate_operand(instr->result, &ax); /*ISWS PREPEI NA GINEI ARG1*/
	assert(arg);

	/* This is actually stack[top] = arg, but we have to 
	 * use avm_assign.
	 */
	avm_assign(&stack[top], arg);
	++totalActuals;
	printf("top = %u\n", top);
	avm_dec_top();
}


/**
 * @brief
 */
void execute_funcenter(instruction* instr) {
	printf("execute_funcenter();\n");
	avm_memcell* func = avm_translate_operand(instr->result, &ax);
	assert(func);

	printf("func->data.funcVal = %d\n", func->data.funcVal);
	assert(pc == func->data.funcVal);	/* Func address should match PC. */

	/* Callee action here... */
	totalActuals = 0;
	userfunc* funcInfo = avm_getfuncinfo(pc);
	topsp = top;
	top = top - funcInfo->localSize;
}


void execute_funcexit(instruction* instr) {
	printf("execute_funcexit();\n");
	unsigned oldTop = top;
	top		= avm_get_envvalue(topsp + AVM_SAVEDTOP_OFFSET);
	pc		= avm_get_envvalue(topsp + AVM_SAVEDPC_OFFSET);
	topsp	= avm_get_envvalue(topsp + AVM_SAVEDTOPSP_OFFSET);
	while (oldTop++ <= top) {	/* Intentionally ignoring first. */
		avm_memcellclear(&stack[oldTop]);
	}
}

void execute_getretval(instruction* instr) {
	printf("execute_getretval();\n");
	/*FIXME*/
}


void execute_ret(instruction* instr) {
	printf("execute_ret();\n");
	/*FIXME*/
}
