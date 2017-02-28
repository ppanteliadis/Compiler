/*******************************************************************
 * @file   targetcode.c                                            *
 * @author Steve Cangonj <cangonj@csd.uoc.gr>, 2871                *
 * @author Ioannis-Pavlos Panteliadis <panteliad@csd.uoc.gr>, 2794 *
 * @author Kiriakos Paraskevas <paraskevas@csd.uoc.gr>, 2832       *
 *                                                                 *
 * @brief  Implementation file of the 'targetcode.h' header file   *
 *******************************************************************/

#include "targetcode.h"




instruction* instruction_table;
unsigned int total_instructions		= 0;
unsigned int current_instruction	= 0;
unsigned int processed_quad			= 0;

incomplete_jump* ij_head = (incomplete_jump*)NULL;
unsigned		ij_total = 0;

int totalglobals = 0;


double*		numConsts;
char**		stringConsts;
char**		namedLibfuncs;
userfunc*	userFuncs;
unsigned	currentNumConst, currentStringConst, currentNamedLibfunc, currentUserFunc;
unsigned	totalNumConsts, totalStringConsts, totalNamedLibfuncs, totalUserFuncs;


extern int current_quad;
extern quad* quad_table;

func* funcstack = (func*)NULL;

/**
 * @brief Ensure that the order of presence in the array is equal to the enumerated
 *        constant value of the respective i-code instruction.
 */
generator_func_t generators[] = {
	generate_ASSIGN,		//0
	generate_ADD,			//1
	generate_SUB,			//2
	generate_MUL,			//3
	generate_DIV,			//4
	generate_MOD,			//5
	generate_UMINUS,		//6
	generate_AND,			//7
	generate_OR,			//8
	generate_NOT,			//9
	generate_IF_EQ,			//10
	generate_IF_NOTEQ,		//11
	generate_IF_LESSEQ,		//12
	generate_IF_GREATEREQ,	//13
	generate_IF_LESS,		//14
	generate_IF_GREATER,	//15
	generate_JUMP,			//16
	generate_CALL,			//17
	generate_PARAM,			//18 /* <--- pusharg_v */
	generate_TABLECREATE,	//19
	generate_TABLEGETELEM,	//20
	generate_TABLESETELEM,	//21
	generate_FUNCSTART,		//22
	generate_FUNCEND,		//23
	generate_RETURN,		//24
	generate_GETRETVAL,		//25
	generate_NOP			//26
};


char* opcodes[]={
	"ASSIGN",
	"ADD",
	"SUB",
	"MUL",
	"DIV",
	"MOD",
	"UMINUS",
	"AND",
	"OR",
	"NOT",
	"JEQ",
	"JNE",
	"JLE",
	"JGE",
	"JLT",
	"JGT",
	"JUMP",
	"CALL",
	"PUSHARG",
	"TABLECREATE",
	"TABLEGETELEM",
	"TABLESETELEM",
	"FUNCENTER",
	"FUNCEXIT",
	"NOP"
};

char* vmargs_t[]={
	"label_a",
	"global_a",
	"formal_a",
	"local_a",
	"number_a",
	"string_a",
	"bool_a",
	"nil_a",
	"userfunc_a",
	"libfunc_a",
	"retval_a"
};


/**
 * @brief Expand the instructions array.
 */
void expand_instruction_table() {
	assert(current_instruction == total_instructions);

	instruction* i = (instruction*)malloc(NEW_INSTR_SIZE);
	if (instruction_table != NULL) {
		memcpy(i, instruction_table, CURR_INSTR_SIZE);
		free(instruction_table);
	}
	instruction_table = i;
	total_instructions += ARRAY_EXPAND_SIZE;
}


/**
 * @brief Create a new instruction and insert it in the instruction table.
 */
void emit_instruction(instruction* instr){
	if (current_instruction == total_instructions) {
		expand_instruction_table();
	}
	instruction* instruction_entry = instruction_table + current_instruction++;
	instruction_entry->opcode = instr->opcode;

	if (instr->result) {
		instruction_entry->result = instr->result;
		instruction_entry->result->val = instr->result->val;
	}

	if (instr->arg1) {
		instruction_entry->arg1 = instr->arg1;
		instruction_entry->arg1->val = instr->arg1->val;
	}

	if (instr->arg2) {
		instruction_entry->arg2 = instr->arg2;
		instruction_entry->arg2->val = instr->arg2->val;
	}

	instruction_entry->srcLine = instr->srcLine;
}


/**
 * @param	instrNo			The jump instruction number.
 * @param	iaddress		The i-code jump-target address.
 * @brief					Adds a new incomplete jump to the responsive list for future backpatching.
 */
void add_incomplete_jump(unsigned instrNo, unsigned iaddress) {
	incomplete_jump* new_entry = (incomplete_jump*)malloc(sizeof(incomplete_jump));
	new_entry->instrNo = instrNo;
	new_entry->iaddress = iaddress;

	if (!ij_head) {
		ij_head = new_entry;
	}
	else {
		new_entry->next = ij_head;
		ij_head = new_entry;
	}
	ij_total++;
}



/**
 * @brief Backpatches incomplete jumps.
 */
void patch_incomplete_jumps() {
	incomplete_jump* x;
	for (x = ij_head; x != NULL; x = x->next) {
		if (x->iaddress == nextquadlabel()) {
			instruction_table[x->instrNo].result->val = nextinstructionlabel();
		}
		else {
			instruction_table[x->instrNo].result->val = quad_table[x->iaddress].taddress;
		}
	}
}



/**
 * @brief
 */
void make_operand(expr* e, vmarg* arg) {
	switch (e->type) {
		/*
		 * All those below use a variable for storage
		 */
		case var_e:
		case tableitem_e:
		case arithexpr_e:
		case boolexpr_e:
		case assignexpr_e:
		case newtable_e: {
			assert(e->sym);
			arg->val = e->sym->offset;
			switch (e->sym->scopespace){
				case program_variable:	arg->type = global_a;	break;
				case function_local:	arg->type = local_a;	break;
				case formal_arg:		arg->type = formal_a;	break;
				default: fprintf(stderr, "Symbol not in known scopespace! scopespace = %d, name = %s, \n", e->sym->scopespace, e->sym->name); assert(0);
			}
			break; /* from case newtable_e */
		}

		/*
		 *	Constants
		 */
		case constbool_e:{
			make_booloperand(arg, e->boolConst);
			break;
		}
		case conststring_e:{
			make_stringoperand(arg, e->strConst);
			break;
		}
		case constnum_e:{
			make_numberoperand(arg, e->numConst);
			break;
		}
		case nil_e: arg->type = nil_a; break;

		/* Functions */
		case programfunc_e:{
			arg->type = userfunc_a;
			//arg->val = e->sym->taddress;
			arg->val = userfuncs_newfunc(e->sym);
			break;
		}
		case libraryfunc_e:{
			arg->val = libfuncs_newused(e->sym->name);
			arg->type = libfunc_a;
			break;
		}
		default: fprintf(stderr, "Unmatched expression type!\n"); assert(0);
	}
}



/* Helper functions to produce common arguments for generated instructions
 * like 1, 0, true, false and function return values.
 */
/**
 * @brief
 */
void make_numberoperand(vmarg* arg, double val){
	arg->val = consts_newnumber(val);
	arg->type = number_a;
}


void make_stringoperand(vmarg* arg, char* val) {
	arg->val = consts_newstring(val);
	arg->type = string_a;
}


/**
 * @brief
 */
void make_booloperand(vmarg* arg, unsigned val){
	arg->val = val;
	arg->type = bool_a;
}


/**
 * @brief
 */
void make_retvaloperand(vmarg* arg){
	arg->type = retval_a;
}


/**
 * @brief
 */
void reset_operand(vmarg* arg) {
	arg = (vmarg*)NULL;
}


/**
 * @brief Returns the label of the instruction being proccessed.
 */
unsigned int nextinstructionlabel(){
	return current_instruction;
}


/**
 * @brief Returns the label of the quad being proccessed.
 */
unsigned int currprocessedquad() {
	return processed_quad;
}


/**
 * @brief Initializes all the constant arrays.
 */
void init_const_arrays(){
	numConsts = (double*)malloc(sizeof(double));
	stringConsts = (char**)malloc(ARRAY_EXPAND_SIZE * sizeof(char*));
	namedLibfuncs = (char**)malloc(ARRAY_EXPAND_SIZE * sizeof(char*));
	userFuncs = (userfunc*)malloc(sizeof(userfunc));
}


/**
 * @brief Free's any memory allocated for the arrays.
 */
void free_const_arrays(){
	int i;
	/* Free the numbers dynamic array. */
	free(numConsts);
	
	/* Free the strings dynamic array. */
	for (i = 0; i < totalStringConsts; i++) {
		free(stringConsts[i]);
	}
	free(stringConsts);

	/* Free the libfuncs dynamic array. */
	for (i = 0; i < totalNamedLibfuncs; i++) {
		free(namedLibfuncs[i]);
	}
	free(namedLibfuncs);

	/* Free the userfuncs dynamic array. */
	free(userFuncs);
}


/**
 * @brief
 */
void expand_numConsts() {
	assert(currentNumConst == totalNumConsts);
	double* n = (double*)malloc(NUMBERS_NEW_SIZE);
	if (numConsts != NULL) {
		memcpy(n, numConsts, NUMBERS_CURR_SIZE);
		free(numConsts);
	}
	numConsts = n;
	totalNumConsts += ARRAY_EXPAND_SIZE;
}

/**
 * @brief
 */
void expand_stringConsts() {
	assert(currentStringConst == totalStringConsts);
	char** s = (char**)malloc(STRINGS_NEW_SIZE);
	if (stringConsts != NULL) {
		memcpy(s, stringConsts, STRINGS_CURR_SIZE);
		free(stringConsts);
	}
	stringConsts = s;
	totalStringConsts += ARRAY_EXPAND_SIZE;
}


/**
 * @brief
 */
void expand_namedLibfuncs() {
	assert(currentNamedLibfunc == totalNamedLibfuncs);
	char** lf = (char**)malloc(LIBFUNCS_NEW_SIZE);

	if (namedLibfuncs != NULL) {
		memcpy(lf, namedLibfuncs, LIBFUNCS_CURR_SIZE);
		free(namedLibfuncs);
	}
	namedLibfuncs = lf;
	totalNamedLibfuncs += ARRAY_EXPAND_SIZE;
}



/**
 * @brief
 */
void expand_userFuncs(){
	assert(currentUserFunc == totalUserFuncs);
	userfunc* uf = (userfunc*)malloc(USERFUNCS_NEW_SIZE);
	if (userFuncs != NULL) {
		memcpy(uf, userFuncs, USERFUNCS_CURR_SIZE);
		free(userFuncs);
	}
	userFuncs = uf;
	totalUserFuncs += ARRAY_EXPAND_SIZE;
}



/**
 * @brief
 */
unsigned consts_newnumber(double n) {
	if (currentNumConst == totalNumConsts) {
		expand_numConsts();
	}
	double* table_pos = numConsts + currentNumConst++;
	*table_pos = n;
	return currentNumConst - 1;
}



/**
 * @brief
 */
unsigned consts_newstring(char* s) {
	if (currentStringConst == totalStringConsts) {
		expand_stringConsts();
	}
	stringConsts[currentStringConst] = (char*)malloc(strlen(s) * sizeof(char));
	stringConsts[currentStringConst++] = strdup(s);

	return currentStringConst - 1;
}



/**
 * @brief
 */
unsigned libfuncs_newused(char* s) {
	if (currentNamedLibfunc == totalNamedLibfuncs) {
		expand_namedLibfuncs();
	}
	namedLibfuncs[currentNamedLibfunc] = (char*)malloc(strlen(s) * sizeof(char));
	namedLibfuncs[currentNamedLibfunc++] = strdup(s);

	return currentNamedLibfunc - 1;
}


/**
 * @brief
 */
unsigned userfuncs_newfunc(SymbolTableEntry* sym) {
	if (currentUserFunc == totalUserFuncs) {
		expand_userFuncs();
	}
	int i;
	for (i = 0; i < currentUserFunc; i++) {
		if (strcmp(userFuncs[i].id, sym->name)==0) {
			return userFuncs[i].address;
		}
	}

	userfunc* uf = userFuncs + currentUserFunc++;
	uf->address = sym->taddress;
	uf->localSize = sym->totallocals;
	uf->id = strdup(sym->name);

	return currentUserFunc - 1;
}



/**
 * @brief
 */
void numbers_to_binary(FILE* fp){
	unsigned int i;
	fwrite(&currentNumConst, sizeof(unsigned), 1, fp);
	for (i = 0; i < currentNumConst; i++) {
		fwrite(&numConsts[i], sizeof(double), 1, fp);
	}
}



/**
 * @brief
 */
void strings_to_binary(FILE* fp){
	unsigned int i, size;
	char* tmp;
	fwrite(&currentStringConst, sizeof(unsigned), 1, fp);
	for (i = 0; i < currentStringConst; i++) {
		tmp = stringConsts[i];
		size = strlen(tmp)+1;
		fwrite(&size, sizeof(unsigned), 1, fp);
		fwrite(tmp, size * sizeof(char), 1, fp);
	}
}


/**
 * @brief
 */
void libfuncs_to_binary(FILE* fp){
	unsigned int i, size;
	char* tmp;
	fwrite(&currentNamedLibfunc, sizeof(unsigned), 1, fp);
	for (i = 0; i < currentNamedLibfunc; i++) {
		tmp = strdup(namedLibfuncs[i]);
		size = strlen(tmp) * sizeof(char);
		fwrite(&size, sizeof(unsigned), 1, fp);
		fwrite(tmp, (strlen(tmp)) * sizeof(char), 1, fp);
	}
}


/**
 * @brief
 */
void userfuncs_to_binary(FILE* fp){
	unsigned int i;
	unsigned idlength;
	fwrite(&currentUserFunc, sizeof(unsigned), 1, fp);
	for (i = 0; i < currentUserFunc; i++) {
		idlength = strlen(userFuncs[i].id) * sizeof(char);
		fwrite(&idlength, sizeof(unsigned), 1, fp);
		fwrite(userFuncs[i].id, strlen(userFuncs[i].id) * sizeof(char), 1, fp);
		fwrite(&userFuncs[i].address, sizeof(unsigned), 1, fp);
		fwrite(&userFuncs[i].localSize, sizeof(unsigned), 1, fp);
	}
}


/**
 * @brief
 */
void arg_to_binary(vmarg* arg, FILE* fp){
	fwrite(&arg->type, sizeof(unsigned), 1, fp);
	fwrite(&arg->val, sizeof(unsigned), 1, fp);
}



/**
 * @brief
 */
void targetcode_to_binary(FILE* fp){
	unsigned int i;
	unsigned op;
	fwrite(&current_instruction, sizeof(unsigned), 1, fp);
	for (i = 0; i < current_instruction; i++) {
		op = instruction_table[i].opcode;
		fwrite(&op, sizeof(int), 1, fp);
		switch (op) {
			case assign_v:
				fwrite(&instruction_table[i].arg1->type, sizeof(unsigned), 1, fp);
				if (instruction_table[i].arg1->type != retval_a) {
					fwrite(&instruction_table[i].arg1->val, sizeof(unsigned), 1, fp);
				}
				arg_to_binary(instruction_table[i].result, fp);
				break;
			case add_v:
			case sub_v:
			case mul_v:
			case divi_v:
			case mod_v:
			case or_v:
			case and_v:
			case tablesetelem_v:
			case tablegetelem_v:
			case jeq_v:
			case jne_v:
			case jlt_v:
			case jle_v:
			case jge_v:
				arg_to_binary(instruction_table[i].arg1, fp);
				arg_to_binary(instruction_table[i].arg2, fp);
				arg_to_binary(instruction_table[i].result, fp);
				break;
			case uminus_v:
			case not_v:
				arg_to_binary(instruction_table[i].arg1, fp);
				arg_to_binary(instruction_table[i].result, fp);
				break;
			case jump_v:
				arg_to_binary(instruction_table[i].result, fp);
				break;
			case funcenter_v:
			case funcexit_v:
			case pusharg_v: 
			case call_v:
			case tablecreate_v:
				arg_to_binary(instruction_table[i].result, fp);
				break;
		}
		fwrite(&instruction_table[i].srcLine, sizeof(unsigned), 1, fp);
	}
}




/**
 * @brief
 */
void print_constnums() {
	printf("---- CONSTANT NUMBERS ----\n");
	unsigned int i;
	for (i = 0; i < currentNumConst; i++) {
		printf("%d : %.2f\n", i, numConsts[i]);
	}
	printf("\n");
}


/**
 * @brief
 */
void print_conststrings() {
	printf("---- CONSTANT STRINGS ----\n");
	unsigned int i;
	for (i = 0; i < currentStringConst; i++) {
		printf("%d : %s\n", i, stringConsts[i]);
	}
	printf("\n");
}


/**
 * @brief
 */
void print_userfuncs() {
	printf("---- USER FUNCTIONS ----\n");
	unsigned int i;
	printf("%-3s %-12s%-12s%-12s\n", "#", "id", "address", "localSize");
	if (userFuncs) {
		for (i = 0; i < currentUserFunc; i++) {
			printf("%-2d : %-12s%-12u%-12u\n", i, userFuncs[i].id, userFuncs[i].address, userFuncs[i].localSize);
		}
	}
	printf("\n");
}


/**
 * @brief
 */
void print_libfuncs() {
	printf("---- LIBRARY FUNCTIONS ----\n");
	unsigned int i;
	for (i = 0; i < currentNamedLibfunc; i++) {
		printf("%d : %s\n", i, namedLibfuncs[i]);
	}
	printf("\n");
}



/**
 * @brief
 */
void print_const_tables(){
	print_constnums();
	printf("\n");
	print_conststrings();
	printf("\n");
	print_libfuncs();
	printf("\n");
	print_userfuncs();
	printf("\n");
}



void print_vmarg(vmarg* arg, int flag) {
	if (flag) {
		unsigned type = arg->type;
		printf("%-12s", vmargs_t[type]);
		switch (type) {
			case label_a:
			case global_a:
			case formal_a:
			case local_a:
			case number_a:
			case string_a:
			case userfunc_a:
			case libfunc_a:
				printf("%-2u", arg->val);
				break;
			case bool_a:
				switch (arg->val) {
					case 0: printf("%s", "FALSE"); break;
					case 1: printf("%s", "TRUE"); break;
				}
			case retval_a:
			case nil_a:
				printf("%-2s", " ");
				break;
			default:
				fprintf(stderr, "Unmatched vmarg type!\n");
				assert(0);
		}
	}
	else {
		printf("%-14s", " ");
	}
}



/**
 * @brief
 */
void print_targetcode(){
	int i = 0;
	instruction* instruction_entry = instruction_table;
	unsigned op;
	printf("---- INSTRUCTION TABLE ----\n");
	printf("%-3s %-14s%-14s%-14s%-14s\n", "#", "vmopcode", "arg1", "arg2", "result");
	while (i < current_instruction) {
		printf("%-2d%-1c ", i,':');
		op = instruction_entry[i].opcode;
		printf("%-14s", opcodes[op]);
		switch (op) {
			case assign_v:
				print_vmarg(instruction_entry[i].arg1, 1);
				print_vmarg(instruction_entry[i].arg2, 0);
				print_vmarg(instruction_entry[i].result, 1);
				break;
			case add_v:
			case sub_v:
			case mul_v:
			case divi_v:
			case mod_v:
			case and_v:
			case or_v:
			case jeq_v:
			case jne_v:
			case jle_v:
			case jge_v:
			case jlt_v:
			case jgt_v:
			case tablegetelem_v:
			case tablesetelem_v:
				print_vmarg(instruction_entry[i].arg1, 1);
				print_vmarg(instruction_entry[i].arg2, 1);
				print_vmarg(instruction_entry[i].result, 1);
				break;
			case uminus_v:
			case not_v:
				print_vmarg(instruction_entry[i].arg1, 1);
				print_vmarg(instruction_entry[i].arg2, 0);
				print_vmarg(instruction_entry[i].result, 1);
				break;
			case funcenter_v:
			case funcexit_v:
			case pusharg_v:
			case call_v:
			case jump_v:
			case tablecreate_v:
				print_vmarg(instruction_entry[i].arg1, 0);
				print_vmarg(instruction_entry[i].arg2, 0);
				print_vmarg(instruction_entry[i].result, 1);
				break;
			case nop_v:
				printf("%-14s", " ");
		}
		i++;
		printf("\n");
	}
	printf("\n");
}



/**
 * @brief
 */
void numbers_to_file(FILE* fp) {
	fprintf(fp, "---- CONSTANT NUMBERS ----\n");
	unsigned int i;
	for (i = 0; i < currentNumConst; i++) {
		fprintf(fp, "%d : %.2f\n", i, numConsts[i]);
	}
	fprintf(fp, "\n");
}



/**
 * @brief
 */
void strings_to_file(FILE* fp) {
	fprintf(fp, "---- CONSTANT STRINGS ----\n");
	unsigned int i;
	for (i = 0; i < currentStringConst; i++) {
		fprintf(fp, "%d : %s\n", i, stringConsts[i]);
	}
	fprintf(fp, "\n");
}



/**
 * @brief
 */
void libfuncs_to_file(FILE* fp){
	fprintf(fp, "---- LIBRARY FUNCTIONS ----\n");
	unsigned int i;
	for (i = 0; i < currentNamedLibfunc; i++) {
		fprintf(fp, "%d : %s\n", i, namedLibfuncs[i]);
	}
	fprintf(fp, "\n");
}



/**
 * @brief
 */
void userfuncs_to_file(FILE* fp){
	fprintf(fp, "---- USER FUNCTIONS ----\n");
	unsigned int i;
	fprintf(fp, "%-3s %-12s%-12s%-12s\n", "#", "id", "address", "localSize");
	if (userFuncs) {
		for (i = 0; i < currentUserFunc; i++) {
			if (userFuncs[i].id != NULL){
				fprintf(fp, "%-2d : %-12s%-12u%-12u\n", i, userFuncs[i].id, userFuncs[i].address, userFuncs[i].localSize);
			}
		}
	}
	fprintf(fp, "\n");
}


void vmarg_to_file(vmarg* arg, int flag, FILE* fp) {
	if (flag) {
		unsigned type = arg->type;
		fprintf(fp, "%-12s", vmargs_t[type]);
		switch (type) {
			case label_a:
			case global_a:
			case formal_a:
			case local_a:
			case number_a:
			case string_a:
			case userfunc_a:
			case libfunc_a:
				fprintf(fp, "%-2u", arg->val);
				break;
			case bool_a:
				switch (arg->val) {
					case 0: fprintf(fp, "%s", "FALSE"); break;
					case 1: fprintf(fp, "%s", "TRUE"); break;
				}
			case retval_a:
			case nil_a:
				fprintf(fp, "%-2s", " ");
				break;
			default:
				fprintf(stderr, "Unmatched vmarg type!\n");
				assert(0);
		}
	}
	else {
		fprintf(fp, "%-14s", " ");
	}
}


/**
 * @brief
 */
void targetcode_to_file(FILE* fp) {
	int i = 0;
	unsigned op;
	unsigned int magicnumber = 340200501;
	instruction* instruction_entry = instruction_table;
	fprintf(fp, "magicnumber = %u\ntotalGlobals = %d\n", magicnumber, totalglobals);
	fprintf(fp, "\n");
	fprintf(fp, "---- INSTRUCTION TABLE ----\n");
	fprintf(fp, "%-3s %-14s%-14s%-14s%-14s\n", "#", "vmopcode", "arg1", "arg2", "result");
	while (i < current_instruction) {
		fprintf(fp, "%-2d%-1c ", i,':');
		op = instruction_entry[i].opcode;
		fprintf(fp, "%-14s", opcodes[op]);
		switch (op) {
			case assign_v:
				if (instruction_entry[i].arg1) {
					vmarg_to_file(instruction_entry[i].arg1, 1, fp);
				}
				else {
					vmarg_to_file(instruction_entry[i].arg1, 0, fp);
				}
				
				vmarg_to_file(instruction_entry[i].arg2, 0, fp);
				vmarg_to_file(instruction_entry[i].result, 1, fp);
				break;
			case add_v:
			case sub_v:
			case mul_v:
			case divi_v:
			case mod_v:
			case and_v:
			case or_v:
			case jeq_v:
			case jne_v:
			case jle_v:
			case jge_v:
			case jlt_v:
			case jgt_v:
			case tablegetelem_v:
			case tablesetelem_v:
				vmarg_to_file(instruction_entry[i].arg1, 1, fp);
				vmarg_to_file(instruction_entry[i].arg2, 1, fp);
				vmarg_to_file(instruction_entry[i].result, 1, fp);
				break;
			case uminus_v:
			case not_v:
				vmarg_to_file(instruction_entry[i].arg1, 1, fp);
				vmarg_to_file(instruction_entry[i].arg2, 0, fp);
				vmarg_to_file(instruction_entry[i].result, 1, fp);
				break;
			case funcenter_v:
			case funcexit_v:
			case pusharg_v:
			case call_v:
			case jump_v:
			case tablecreate_v:
				vmarg_to_file(instruction_entry[i].arg1, 0, fp);
				vmarg_to_file(instruction_entry[i].arg2, 0, fp);
				vmarg_to_file(instruction_entry[i].result, 1, fp);
				break;
			case nop_v:
				fprintf(fp, "%-14s", " ");
		}
		i++;
		fprintf(fp, "\n");
	}
	fprintf(fp, "\n");
}


/**
 * @brief
 */
void avm_binaryfile() {
	FILE* fp;
	unsigned magicnumber = 340200501;
	fp = fopen("alphatargetcode.bin", "wb");
	if (!fp) {
		printf("Can't open binary file!\n");
		exit(-1);
	}
	else {
		fwrite(&magicnumber, sizeof(unsigned), 1, fp);
		fwrite(&totalglobals, sizeof(int), 1, fp);
		/* TARGET CODE */
		targetcode_to_binary(fp);

		/* CONSTANT ARRAYS */
		numbers_to_binary(fp);
		strings_to_binary(fp);
		libfuncs_to_binary(fp);
		userfuncs_to_binary(fp);
	}
	/* Close the file. */
	fclose(fp);
	printf("Alpha binary file printed!\n");
}



/**
 * @brief
 */
void avm_normalfile() {
	FILE * fp;
	fp = fopen("alphatargetcode.norm", "wb");
	if (!fp) {
		printf("Can't open normal file!\n");
		exit(-1);
	}
	else {
		/* TARGET CODE */
		targetcode_to_file(fp);

		/* CONSTANT ARRAYS */
		numbers_to_file(fp);
		strings_to_file(fp);
		libfuncs_to_file(fp);
		userfuncs_to_file(fp);
	}
	/* Close the file. */
	fclose(fp);
	printf("Alpha normal file printed!\n");
}



/**
 * @brief
 */
void generate(vmopcode op, quad* q) {
	instruction* t = (instruction*)malloc(sizeof(instruction));
	t->opcode = op;
	t->arg1 = (vmarg*)NULL;
	t->arg2 = (vmarg*)NULL;
	t->result = (vmarg*)NULL;

	if (q->arg1 != NULL) {
		t->arg1 = (vmarg*)malloc(sizeof(vmarg));
		make_operand(q->arg1, t->arg1);
	}

	if (q->arg2 != NULL) {
		t->arg2 = (vmarg*)malloc(sizeof(vmarg));
		make_operand(q->arg2, t->arg2);
	}

	if (q->result != NULL) {
		t->result = (vmarg*)malloc(sizeof(vmarg));
		make_operand(q->result, t->result);
	}

	q->taddress = nextinstructionlabel();
	t->srcLine = q->line;

	emit_instruction(t);
}



/**
 * @brief
 */
void generate_relational(vmopcode op, quad* q){
	instruction* t = (instruction*)malloc(sizeof(instruction));
	t->opcode = op;
	t->srcLine = q->line;

	t->arg1 = (vmarg*)NULL;
	t->arg2 = (vmarg*)NULL;
	t->result = (vmarg*)malloc(sizeof(vmarg));

	if (q->arg1 != NULL) {
		t->arg1 = (vmarg*)malloc(sizeof(vmarg));
		make_operand(q->arg1, t->arg1);
	}

	if (q->arg2 != NULL) {
		t->arg2 = (vmarg*)malloc(sizeof(vmarg));
		make_operand(q->arg2, t->arg2);
	}

	t->result->type = label_a;

	if (q->label < currprocessedquad()) {
		t->result->val = quad_table[q->label].taddress;
	}
	else {
		add_incomplete_jump(nextinstructionlabel(), q->label);
	}
	q->taddress = nextinstructionlabel();

	emit_instruction(t);
}



/**
 * @brief
 */
void generate_targetcode(int globals){
	printf("Generating target code...\n");
	totalglobals = globals;
	unsigned i;
	init_const_arrays();
	for (i = 0; i < current_quad; i++) {
		//printf("DEBUG: quad_table[%u] = %u\n", i, quad_table[i].op);
		(*generators[quad_table[i].op])(quad_table + i);
	}
	patch_incomplete_jumps();


	if (current_quad == 0) {
		/* Empty program. */
		generate_NOP((quad*)NULL);
	}
	printf("Target code generated!\n");
	putchar('\n');
}

/**
 * @brief
 */
void generate_ADD(quad *q){
	generate(add_v, q);
}


/**
 * @brief
 */
void generate_SUB(quad *q){
	generate(sub_v, q);
}


/**
 * @brief
 */
void generate_MUL(quad *q){
	generate(mul_v, q);
}


/**
 * @brief
 */
void generate_DIV(quad *q){
	generate(divi_v, q);
}


/**
 * @brief
 */
void generate_MOD(quad *q){
	generate(mod_v, q);
}


/**
 * @brief
 */
void generate_TABLECREATE(quad *q){
	generate(tablecreate_v, q);
}


/**
 * @brief
 */
void generate_TABLEGETELEM(quad *q){
	generate(tablegetelem_v, q);
}


/**
 * @brief
 */
void generate_TABLESETELEM(quad *q){
	generate(tablesetelem_v, q);
}


/**
 * @brief
 */
void generate_ASSIGN(quad *q){
	generate(assign_v, q);
}


/**
 * @brief
 */
void generate_NOP(quad* q){
	instruction *t = (instruction*)malloc(sizeof(instruction));
	t->opcode = nop_v;
	t->srcLine = q->line;
	t->arg1 = (vmarg*)NULL;
	t->arg2 = (vmarg*)NULL;
	t->result = (vmarg*)NULL;
	emit_instruction(t);
}


/**
 * @brief
 */
void generate_JUMP(quad *q){
	generate_relational(jump_v, q);
}


/**
 * @brief
 */
void generate_IF_EQ(quad *q){
	generate_relational(jeq_v, q);
}


/**
 * @brief
 */
void generate_IF_NOTEQ(quad *q){
	generate_relational(jne_v, q);
}


/**
 * @brief
 */
void generate_IF_GREATER(quad *q){
	generate_relational(jgt_v, q);
}


/**
 * @brief
 */
void generate_IF_GREATEREQ(quad *q){
	generate_relational(jge_v, q);
}


/**
 * @brief
 */
void generate_IF_LESS(quad *q){
	generate_relational(jlt_v, q);
}


/**
 * @brief
 */
void generate_IF_LESSEQ(quad *q){
	generate_relational(jle_v, q);
}


/**
 * @brief 
 */
void generate_UMINUS(quad *q){
	q->taddress = nextinstructionlabel();
	instruction* t = (instruction*)malloc(sizeof(instruction));

	t->arg1 = (vmarg*)malloc(sizeof(vmarg));
	t->arg2 = (vmarg*)malloc(sizeof(vmarg));
	t->result = (vmarg*)malloc(sizeof(vmarg));

	t->opcode = mul_v;
	t->srcLine = q->line;

	make_operand(q->arg1, t->arg1);
	make_numberoperand(t->arg2, -1);
	make_operand(q->result, t->result);

	emit_instruction(t);
}



/**
 * @brief
 */
void generate_NOT(quad *q) {
	q->taddress = nextinstructionlabel();

	/*****************************************************/
	instruction* t1 = (instruction*)malloc(sizeof(instruction));

	t1->arg1 = (vmarg*)malloc(sizeof(vmarg));
	t1->arg2 = (vmarg*)malloc(sizeof(vmarg));
	t1->result = (vmarg*)malloc(sizeof(vmarg));

	t1->opcode = jeq_v;
	make_operand(q->arg1, t1->arg1);
	make_booloperand(t1->arg2, 0);	/* 0 = FALSE */
	t1->result->type = label_a;
	t1->result->val = nextinstructionlabel() + 3;
	t1->srcLine = q->line;
	emit_instruction(t1);
	/*****************************************************/

	/*****************************************************/
	instruction* t2 = (instruction*)malloc(sizeof(instruction));

	t2->arg1 = (vmarg*)malloc(sizeof(vmarg));
	t2->arg2 = (vmarg*)malloc(sizeof(vmarg));
	t2->result = (vmarg*)malloc(sizeof(vmarg));

	t2->opcode = assign_v;
	make_booloperand(t2->arg1, 0);	/* 0 = FALSE */
	reset_operand(t2->arg2);
	make_operand(q->result, t2->result);
	t2->srcLine = q->line;
	emit_instruction(t2);
	/*****************************************************/


	/*****************************************************/
	instruction* t3 = (instruction*)malloc(sizeof(instruction));

	t3->arg1 = (vmarg*)malloc(sizeof(vmarg));
	t3->arg2 = (vmarg*)malloc(sizeof(vmarg));
	t3->result = (vmarg*)malloc(sizeof(vmarg));

	t3->opcode = jump_v;
	reset_operand(t3->arg1);
	reset_operand(t3->arg2);
	t3->result->type = label_a;
	t3->result->val = nextinstructionlabel() + 2;
	t3->srcLine = q->line;
	emit_instruction(t3);
	/*****************************************************/


	/*****************************************************/
	instruction* t4 = (instruction*)malloc(sizeof(instruction));

	t4->arg1 = (vmarg*)malloc(sizeof(vmarg));
	t4->arg2 = (vmarg*)malloc(sizeof(vmarg));
	t4->result = (vmarg*)malloc(sizeof(vmarg));

	t4->opcode = assign_v;
	make_booloperand(t4->arg1, 1);	/* 1 = TRUE */
	reset_operand(t4->arg2);
	make_operand(q->result, t4->result);
	t4->srcLine = q->line;
	emit_instruction(t4);
	/*****************************************************/
}


/**
 * @brief 
 */
void generate_AND(quad *q) {
	q->taddress = nextinstructionlabel();

	/*****************************************************/
	instruction* t1 = (instruction*)malloc(sizeof(instruction));

	t1->arg1 = (vmarg*)malloc(sizeof(vmarg));
	t1->arg2 = (vmarg*)malloc(sizeof(vmarg));
	t1->result = (vmarg*)malloc(sizeof(vmarg));

	t1->opcode = jeq_v;
	make_operand(q->arg1, t1->arg1);
	make_booloperand(t1->arg2, 0);	/* 0 = FALSE */
	t1->result->type = label_a;
	t1->result->val = nextinstructionlabel() + 4;
	t1->srcLine = q->line;
	emit_instruction(t1);
	/*****************************************************/


	/*****************************************************/
	instruction* t2 = (instruction*)malloc(sizeof(instruction));

	t2->arg1 = (vmarg*)malloc(sizeof(vmarg));
	t2->arg2 = (vmarg*)malloc(sizeof(vmarg));
	t2->result = (vmarg*)malloc(sizeof(vmarg));
	t2->opcode = jeq_v;
	make_operand(q->arg2, t2->arg1);
	make_booloperand(t2->arg2, 0);	/* 0 = FALSE */
	t2->result->val = nextinstructionlabel() + 3;
	t2->srcLine = q->line;
	emit_instruction(t2);
	/*****************************************************/


	/*****************************************************/
	instruction* t3 = (instruction*)malloc(sizeof(instruction));

	t3->arg1 = (vmarg*)malloc(sizeof(vmarg));
	t3->arg2 = (vmarg*)malloc(sizeof(vmarg));
	t3->result = (vmarg*)malloc(sizeof(vmarg));
	t3->opcode = assign_v;
	make_booloperand(t3->arg1, 1);	/* 1 = TRUE */
	reset_operand(t3->arg2);
	make_operand(q->result, t3->result);
	t3->srcLine = q->line;
	emit_instruction(t3);
	/*****************************************************/


	/*****************************************************/
	instruction* t4 = (instruction*)malloc(sizeof(instruction));

	t4->arg1 = (vmarg*)malloc(sizeof(vmarg));
	t4->arg2 = (vmarg*)malloc(sizeof(vmarg));
	t4->result = (vmarg*)malloc(sizeof(vmarg));
	t4->opcode = jump_v;
	reset_operand(t4->arg1);
	reset_operand(t4->arg2);
	t4->result->type = label_a;
	t4->result->val = nextinstructionlabel() + 2;
	t4->srcLine = q->line;
	emit_instruction(t4);
	/*****************************************************/


	/*****************************************************/
	instruction* t5 = (instruction*)malloc(sizeof(instruction));

	t5->arg1 = (vmarg*)malloc(sizeof(vmarg));
	t5->arg2 = (vmarg*)malloc(sizeof(vmarg));
	t5->result = (vmarg*)malloc(sizeof(vmarg));
	t5->opcode = assign_v;
	make_booloperand(t5->arg1, 0);	/* 0 = FALSE */
	reset_operand(t5->arg2);
	make_operand(q->result, t5->result);
	t5->srcLine = q->line;
	emit_instruction(t5);
	/*****************************************************/
}



/**
 * @brief 
 */
void generate_OR(quad *q){
	q->taddress = nextinstructionlabel();

	/*****************************************************/
	instruction* t1 = (instruction*)malloc(sizeof(instruction));

	t1->arg1 = (vmarg*)malloc(sizeof(vmarg));
	t1->arg2 = (vmarg*)malloc(sizeof(vmarg));
	t1->result = (vmarg*)malloc(sizeof(vmarg));

	t1->opcode = jeq_v;
	make_operand(q->arg1, t1->arg1);
	make_booloperand(t1->arg2, 1);	/* 1 = TRUE */
	t1->result->type = label_a;
	t1->result->val = nextinstructionlabel() + 4;
	t1->srcLine = q->line;
	emit_instruction(t1);
	/*****************************************************/


	/*****************************************************/
	instruction* t2 = (instruction*)malloc(sizeof(instruction));

	t2->arg1 = (vmarg*)malloc(sizeof(vmarg));
	t2->arg2 = (vmarg*)malloc(sizeof(vmarg));
	t2->result = (vmarg*)malloc(sizeof(vmarg));
	t2->opcode = jeq_v;
	make_operand(q->arg2, t2->arg1);
	make_booloperand(t2->arg2, 1);	/* 1 = TRUE */
	t2->result->val = nextinstructionlabel() + 3;
	t2->srcLine = q->line;
	emit_instruction(t2);
	/*****************************************************/


	/*****************************************************/
	instruction* t3 = (instruction*)malloc(sizeof(instruction));

	t3->arg1 = (vmarg*)malloc(sizeof(vmarg));
	t3->arg2 = (vmarg*)malloc(sizeof(vmarg));
	t3->result = (vmarg*)malloc(sizeof(vmarg));
	t3->opcode = assign_v;
	make_booloperand(t3->arg1, 0);	/* 0 = FALSE */
	reset_operand(t3->arg2);
	make_operand(q->result, t3->result);
	t3->srcLine = q->line;
	emit_instruction(t3);
	/*****************************************************/


	/*****************************************************/
	instruction* t4 = (instruction*)malloc(sizeof(instruction));

	t4->arg1 = (vmarg*)malloc(sizeof(vmarg));
	t4->arg2 = (vmarg*)malloc(sizeof(vmarg));
	t4->result = (vmarg*)malloc(sizeof(vmarg));
	t4->opcode = jump_v;
	reset_operand(t4->arg1);
	reset_operand(t4->arg2);
	t4->result->type = label_a;
	t4->result->val = nextinstructionlabel() + 2;
	t4->srcLine = q->line;
	emit_instruction(t4);
	/*****************************************************/


	/*****************************************************/
	instruction* t5 = (instruction*)malloc(sizeof(instruction));

	t5->arg1 = (vmarg*)malloc(sizeof(vmarg));
	t5->arg2 = (vmarg*)malloc(sizeof(vmarg));
	t5->result = (vmarg*)malloc(sizeof(vmarg));
	t5->opcode = assign_v;
	make_booloperand(t5->arg1, 1);	/* 1 = TRUE */
	reset_operand(t5->arg2);
	make_operand(q->result, t5->result);
	t5->srcLine = q->line;
	emit_instruction(t5);
	/*****************************************************/
}



/**
 * @brief 
 */
void generate_PARAM(quad *q){
	q->taddress = nextinstructionlabel();
	instruction* t = (instruction*)malloc(sizeof(instruction));

	t->arg1 = (vmarg*)NULL;
	t->arg2 = (vmarg*)NULL;
	t->result = (vmarg*)malloc(sizeof(vmarg));

	t->opcode = pusharg_v;
	t->srcLine = q->line;
	make_operand(q->result, t->result);

	emit_instruction(t);
}


/**
 * @brief 
 */
void generate_CALL(quad *q){
	q->taddress = nextinstructionlabel();
	instruction* t = (instruction*)malloc(sizeof(instruction));

	t->arg1 = (vmarg*)NULL;
	t->arg2 = (vmarg*)NULL;
	t->result = (vmarg*)malloc(sizeof(vmarg));

	t->opcode = call_v;
	t->srcLine = q->line;

	make_operand(q->result, t->result);
	emit_instruction(t);
}



/**
 * @brief 
 */
void generate_GETRETVAL(quad *q){
	q->taddress = nextinstructionlabel();
	instruction* t = (instruction*)malloc(sizeof(instruction));

	t->arg1 = (vmarg*)malloc(sizeof(vmarg));
	t->arg2 = (vmarg*)NULL;
	t->result = (vmarg*)malloc(sizeof(vmarg));

	t->opcode = assign_v;
	t->srcLine = q->line;

	make_operand(q->result, t->result);

	make_retvaloperand(t->arg1);

	emit_instruction(t);
}




/**
 * @brief 
 */
void generate_FUNCSTART(quad *q){
	instruction* t1 = (instruction*)malloc(sizeof(instruction));
	t1->arg1 = (vmarg*)NULL;
	t1->arg2 = (vmarg*)NULL;
	t1->result = (vmarg*)malloc(sizeof(instruction));

	t1->opcode = jump_v;
	t1->result->type = label_a;
	reset_operand(t1->arg1);
	reset_operand(t1->arg2);
	t1->srcLine = q->line;
	emit_instruction(t1);
	/*****************************************************/

	SymbolTableEntry* f = q->result->sym;
	f->taddress = nextinstructionlabel();
	q->taddress = nextinstructionlabel();
	f->incJump = nextinstructionlabel() - 1;

	userfuncs_newfunc(f);
	funcstack = push_func(funcstack, &f);

	instruction* t2 = (instruction*)malloc(sizeof(instruction));
	t2->result = (vmarg*)malloc(sizeof(vmarg));
	t2->opcode = funcenter_v;
	make_operand(q->result, t2->result);
	reset_operand(t2->arg1);
	reset_operand(t2->arg2);
	t2->srcLine = q->line;
	emit_instruction(t2);
}




/**
 * @brief 
 */
void generate_RETURN(quad *q){
	q->taddress = nextinstructionlabel();

	instruction* t1 = (instruction*)malloc(sizeof(instruction));
	t1->result = (vmarg*)malloc(sizeof(vmarg));
	t1->arg1 = (vmarg*)malloc(sizeof(vmarg));
	t1->arg2 = (vmarg*)NULL;

	t1->opcode = assign_v;
	make_retvaloperand(t1->result);
	make_operand(q->result, t1->arg1);
	reset_operand(t1->arg2);
	emit_instruction(t1);

	SymbolTableEntry* f;
	funcstack = top_func(funcstack, &f);
	f->retList = returnInsert(f, nextinstructionlabel());

	instruction* t2 = (instruction*)malloc(sizeof(instruction));
	t2->opcode = jump_v;
	t2->result = (vmarg*)malloc(sizeof(vmarg));
	reset_operand(t2->arg1);
	reset_operand(t2->arg2);
	t2->result->type = label_a;

	emit_instruction(t2);
}


/*
 * @brief 
 */
void generate_FUNCEND(quad *q){
	SymbolTableEntry* f;

	funcstack = pop_func(funcstack, &f);
	backpatch(f, nextinstructionlabel());
	instruction_table[f->incJump].result->val = nextinstructionlabel() + 1; /* Go to the next instruction after funcexit. */

	q->taddress = nextinstructionlabel();
	instruction* t = (instruction*)malloc(sizeof(instruction));
	t->arg1 = (vmarg*)NULL;
	t->arg2 = (vmarg*)NULL;
	t->result = (vmarg*)malloc(sizeof(vmarg));
	t->opcode = funcexit_v;
	make_operand(q->result, t->result);

	reset_operand(t->arg1);
	reset_operand(t->arg2);

	t->srcLine = q->line;

	emit_instruction(t);
}

/**
 * @brief A list containing all incomplete returns.
 */
void append(SymbolTableEntry* sym, unsigned int ilabel) {
	returnList* newentry = (returnList*)malloc(sizeof(returnList));
	newentry->label = ilabel;
	newentry->next = sym->retList;
	sym->retList = newentry;
}


/**
 * @brief A list containing all incomplete returns.
 */
void backpatch(SymbolTableEntry* sym, unsigned int ilabel) {
	while (sym->retList) {
		instruction_table[sym->retList->label].result->val = ilabel;
		sym->retList = sym->retList->next;
		//free(tmp);	/* Source of all evil. */
	}
}


vmarg* new_vmarg() {
	vmarg* newentry = (vmarg*)malloc(sizeof(vmarg));
	if (newentry == NULL) {
		fprintf(stderr, "newvmarg : Memory allocation error!\n");
		exit(-1);
	}
	return newentry;
}

returnList* returnInsert(SymbolTableEntry* sym, unsigned int label) {
	returnList* tmp = malloc(sizeof(returnList));
	tmp->label = label;
	tmp->next = sym->retList;
	sym->retList = tmp;
	return sym->retList;
}