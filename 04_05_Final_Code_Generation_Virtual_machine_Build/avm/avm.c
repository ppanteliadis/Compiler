
#include "libfuncs.h"


unsigned current_instruction = 0;
unsigned total_instructions = 0;
instruction* instruction_table;

int total_globals = 0;


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
	instruction* instruction_entry = (instruction*)malloc(sizeof(instruction));
	instruction_entry = instruction_table + current_instruction++;
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



void open_binary_numbers(FILE* fp) {
	unsigned int i;
	int size;
	double ds;		/* Not a boob joke :P */
	fread(&size, sizeof(int), 1, fp);
	for (i = 0; i < size; i++) {
		fread(&ds, sizeof(double), 1, fp);
		consts_newnumber(ds);
	}
}

void open_binary_strings(FILE* fp) {
	unsigned int i;
	unsigned arrsize, size;
	char* name;
	fread(&arrsize, sizeof(unsigned), 1, fp);
	for (i = 0; i < arrsize; i++) {
		fread(&size, sizeof(unsigned), 1, fp);
		name = (char*)malloc(size * sizeof(char));
		fread(name, size * sizeof(char), 1, fp);
		consts_newstring(name);
	}
}

void open_binary_libfuncs(FILE* fp) {
	unsigned int i;
	unsigned arrsize, size;
	char* name;
	fread(&arrsize, sizeof(unsigned), 1, fp);
	for (i = 0; i < arrsize; i++) {
		fread(&size, sizeof(unsigned), 1, fp);
		name = (char*)malloc(size * sizeof(char));
		fread(name, size * sizeof(char), 1, fp);
		libfuncs_newused(name);
	}
}

void open_binary_userfuncs(FILE* fp) {
	unsigned int i;
	unsigned size;

	char* id;
	unsigned address, localSize, idlength;

	fread(&size, sizeof(unsigned), 1, fp);
	for (i = 0; i < size; i++) {
		fread(&idlength, sizeof(unsigned), 1, fp);
		id = (char*)malloc(idlength);
		fread(id, idlength, 1, fp);
		fread(&address, sizeof(unsigned), 1, fp);
		fread(&localSize, sizeof(unsigned), 1, fp);

		SymbolTableEntry* newsymbol = (SymbolTableEntry*)malloc(sizeof(SymbolTableEntry));
		newsymbol->taddress = address;
		newsymbol->totallocals = localSize;
		newsymbol->name = strdup(id);

		userfuncs_newfunc(newsymbol);
	}
}

vmarg* open_binary_arg(int arg, instruction* instr, FILE* fp) {
	unsigned type, val;
	fread(&type, sizeof(unsigned), 1, fp);
	fread(&val, sizeof(unsigned), 1, fp);
	switch (arg) {
		case 0: {
			/* result */
			instr->result = (vmarg*)malloc(sizeof(vmarg));
			instr->result->type = type;
			instr->result->val = val;
			return instr->result;
		}
		case 1: {
			/* arg1 */
			instr->arg1 = (vmarg*)malloc(sizeof(vmarg));
			instr->arg1->type = type;
			instr->arg1->val = val;
			return instr->arg1;
		}
		case 2: {
			/* arg2 */
			instr->arg2 = (vmarg*)malloc(sizeof(vmarg));
			instr->arg2->type = type;
			instr->arg2->val = val;
			return instr->arg2;
		}
		default: fprintf(stderr, "Unacceptable vm argument!\n"); assert(0);
	}
}


/**
 * @brief
 */
void open_binary_targetcode(FILE* fp) {
	unsigned int i;
	unsigned curr_instr, srcLine;
	int op;

	fread(&curr_instr, sizeof(unsigned), 1, fp);
	instruction* instr;
	for (i = 0; i < curr_instr; i++) {
		instr = (instruction*)malloc(sizeof(instruction));
		fread(&op, sizeof(int), 1, fp);
		instr->opcode = op;
		switch (op) {
			case assign_v:
				instr->arg1 = (vmarg*)malloc(sizeof(vmarg));
				fread(&instr->arg1->type, sizeof(unsigned), 1, fp);
				if (instr->arg1->type != retval_a) {
					fread(&instr->arg1->val, sizeof(unsigned), 1, fp);
				}
				instr->result = open_binary_arg(0, instr, fp);
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
				instr->arg1 = open_binary_arg(1, instr, fp);
				instr->arg2 = open_binary_arg(2, instr, fp);
				instr->result = open_binary_arg(0, instr, fp);
				break;
			case uminus_v:
			case not_v:
				instr->arg1 = open_binary_arg(1, instr, fp);
				instr->result = open_binary_arg(0, instr, fp);
				break;
			case jump_v:
				instr->result = open_binary_arg(0, instr, fp);
				break;
			case funcenter_v:
			case funcexit_v:
			case pusharg_v:
			case call_v:
			case tablecreate_v:
				instr->result = open_binary_arg(0, instr, fp);
				break;
		}
		fread(&srcLine, sizeof(unsigned), 1, fp);
		instr->srcLine = srcLine;
		emit_instruction(instr);
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
			//if (userFuncs[i].id != NULL){
				printf("%-2d : %-12s%-12u%-12u\n", i, userFuncs[i].id, userFuncs[i].address, userFuncs[i].localSize);
			//}
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
 * @brief Opens the binary file and stores everything in the arrays.
 */
void open_binary_file(FILE* fp) {
	unsigned magicnumber;
	fread(&magicnumber, sizeof(unsigned), 1, fp);
	fread(&total_globals, sizeof(int), 1, fp);
	if (magicnumber == 340200501) {
		/* TARGET CODE */
		open_binary_targetcode(fp);

		/* CONSTANT ARRAYS */
		open_binary_numbers(fp);
		open_binary_strings(fp);
		open_binary_libfuncs(fp);
		open_binary_userfuncs(fp);

		/*
		print_constnums();
		print_conststrings();
		print_libfuncs();
		print_userfuncs();
		*/

		/* Close the file. */
		printf("Binary file loaded!\n");
	}
	else {
		fprintf(stderr, "Not an alpha compilable code.");
	}
	
}


static void avm_initstack(void) {
	unsigned i;
	for (i = 0; i < AVM_STACKSIZE; ++i) {
		AVM_WIPEOUT(stack[i]);
		stack[i].type = undef_m;
	}
}


void avm_initialize() {
	avm_initstack();
	/* Rest of initiations include library functions... */
	avm_registerlibfunc("print", libfunc_print);
	avm_registerlibfunc("input", libfunc_input);
	avm_registerlibfunc("objectmemberkeys", libfunc_objectmemberkeys);
	avm_registerlibfunc("objecttotalmembers", libfunc_objecttotalmembers);
	avm_registerlibfunc("objectcopy", libfunc_objectcopy);
	avm_registerlibfunc("totalarguments", libfunc_totalarguments);
	avm_registerlibfunc("argument", libfunc_argument);
	avm_registerlibfunc("typeof", libfunc_typeof);
	avm_registerlibfunc("strtonum", libfunc_strtonum);
	avm_registerlibfunc("sqrt", libfunc_sqrt);
	avm_registerlibfunc("cos", libfunc_cos);
	avm_registerlibfunc("sin", libfunc_sin);
}

/**
 * @brief DO NOT RUN THIS YET!
 */
int main (int argc, char** argv) {
	printf("Alpha Virtual Machine initiated!\n");

	FILE* fp;
	fp = fopen("../compiler/alphatargetcode.bin", "r");
	if (fp) {
		avm_initialize();
		open_binary_file(fp);
		print_targetcode();		/*DEBUG*/
		printf("total_globals = %d\n", total_globals);
		top = topsp = AVM_STACKSIZE - total_globals - 1;
		while (!executionFinished) {
			execute_cycle();
		}
		fclose(fp);
	}
	else {
		fprintf(stderr, "Cannot open file: alphatargetcode.bin\n");
		assert(0);
	}

	return 0;
}

