/*******************************************************************
 * @file   oneheader.h                                             *
 * @author Steve Cangonj <cangonj@csd.uoc.gr>, 2871                *
 * @author Ioannis-Pavlos Panteliadis <panteliad@csd.uoc.gr>, 2794 *
 * @author Kiriakos Paraskevas <paraskevas@csd.uoc.gr>, 2832       *
 *                                                                 *
 * @brief  One Header to rule them all, One Header to find them,   *
 *         One Header to compile them all and in the darkness      *
 *         link them.                                              *
 *******************************************************************/


#ifndef ONEHEADER_H
#define ONEHEADER_H

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <assert.h>
#include <math.h>


/******************************************************************/
/*   Needed structures and functions from final code generation   */
/******************************************************************/
#define ARRAY_EXPAND_SIZE		1024
#define CURR_INSTR_SIZE			(total_instructions*sizeof(instruction))
#define NEW_INSTR_SIZE			(ARRAY_EXPAND_SIZE*sizeof(instruction) + CURR_INSTR_SIZE)


#define NUMBERS_CURR_SIZE		(totalNumConsts*sizeof(double))
#define NUMBERS_NEW_SIZE		(ARRAY_EXPAND_SIZE*sizeof(double) + NUMBERS_CURR_SIZE)

#define STRINGS_CURR_SIZE		(totalStringConsts*sizeof(char*))
#define STRINGS_NEW_SIZE		(ARRAY_EXPAND_SIZE*sizeof(char*) + STRINGS_CURR_SIZE)


#define USERFUNCS_CURR_SIZE		(totalUserFuncs*sizeof(userfunc))
#define USERFUNCS_NEW_SIZE		(ARRAY_EXPAND_SIZE*sizeof(userfunc) + USERFUNCS_CURR_SIZE)


#define LIBFUNCS_CURR_SIZE		(totalNamedLibfuncs*sizeof(char*))
#define LIBFUNCS_NEW_SIZE		(ARRAY_EXPAND_SIZE*sizeof(char*) + LIBFUNCS_CURR_SIZE)




typedef enum vmopcode {
	assign_v,		//0
	add_v,			//1
	sub_v,			//2
	mul_v,			//3
	divi_v,			//4
	mod_v,			//5
	uminus_v,		//6
	and_v,			//7
	or_v,			//8
	not_v,			//9
	jeq_v,			//10
	jne_v,			//11
	jle_v,			//12
	jge_v,			//13
	jlt_v,			//14
	jgt_v,			//15
	jump_v,			//16
	call_v,			//17
	pusharg_v,		//18
	tablecreate_v,	//19
	tablegetelem_v,	//20
	tablesetelem_v,	//21
	funcenter_v,	//22
	funcexit_v,		//23
	nop_v			//24
}vmopcode;

typedef enum vmarg_t {
	label_a		= 0,
	global_a	= 1,
	formal_a	= 2,
	local_a		= 3,
	number_a	= 4,
	string_a	= 5,
	bool_a		= 6,
	nil_a		= 7,
	userfunc_a	= 8,
	libfunc_a	= 9,
	retval_a	= 10
}vmarg_t;


typedef struct vmarg {
	vmarg_t		type;
	unsigned	val;
}vmarg;


typedef struct instruction {
	vmopcode	opcode;
	vmarg*		result;
	vmarg*		arg1;
	vmarg*		arg2;
	unsigned	srcLine;
}instruction;


typedef struct userfunc{
	unsigned	address;
	unsigned	localSize;
	char*		id;
}userfunc;

typedef struct SymbolTableEntry{
	char*		name;
	unsigned	totallocals;
	unsigned	taddress;
}SymbolTableEntry;


typedef void (*execute_func_t)(instruction *);

extern double*		numConsts;
extern unsigned		currentNumConst;
extern unsigned		totalNumConsts;

extern char**		stringConsts;
extern unsigned		currentStringConst;
extern unsigned		totalStringConsts;

extern char**		namedLibfuncs;
extern unsigned		currentNamedLibfunc;
extern unsigned		totalNamedLibfuncs;

extern userfunc*	userFuncs;
extern unsigned		currentUserFunc;
extern unsigned		totalUserFuncs;


void expand_numConsts();
void expand_stringConsts();
void expand_namedLibfuncs();
void expand_userFuncs();

void expand_instruction_table();
void emit_instruction(instruction*);


void read_binary_numbers(FILE*);
void read_binary_strings(FILE*);
void read_binary_libfuncs(FILE*);
void read_binary_userfuncs(FILE*);
void read_binary_targetcode(FILE*);
void read_binary_arg(int, FILE*);
void read_binary_file(FILE*);


unsigned consts_newnumber(double);
unsigned consts_newstring(char*);
unsigned libfuncs_newused(char*);
unsigned userfuncs_newfunc(SymbolTableEntry*);

//int isLibraryFunction(char*);

/******************************************************************/
/******************************************************************/

/*         Needed structures and functions for AVM stack          */

/******************************************************************/
/******************************************************************/
#define AVM_STACKENV_SIZE	4
#define AVM_STACKSIZE		4096
#define AVM_ENDING_PC		current_instruction
#define AVM_WIPEOUT(m)		memset(&(m),0,sizeof(m))

#define AVM_TABLE_HASHSIZE	211
#define AVM_TABLE_BOOL_HASHSIZE 2

struct avm_table;

typedef enum avm_memcell_t{
	number_m	= 0,
	string_m	= 1,
	bool_m		= 2,
	table_m		= 3,
	userfunc_m	= 4,
	libfunc_m	= 5,
	nil_m		= 6,
	undef_m		= 7
}avm_memcell_t;

typedef struct avm_memcell {
	avm_memcell_t type ;
	union {
		double numVal;
		char* strVal;
		unsigned char boolVal;
		struct avm_table* tableVal;
		unsigned int funcVal;
		char* libfuncVal;
	}data;
}avm_memcell;



extern avm_memcell stack[AVM_STACKSIZE];
extern avm_memcell ax, bx, cx, retval;

extern unsigned top, topsp;

double consts_getnumber(int);
char* consts_getstring(int);
char* libfuncs_getused(int);
unsigned userfuncs_getused(int);

avm_memcell* avm_translate_operand(vmarg*, avm_memcell*);

typedef void (*memclear_func_t)(avm_memcell*);

typedef struct avm_table_bucket{
	avm_memcell* key;
	avm_memcell* value;
	struct avm_table_bucket* next;
}avm_table_bucket;

typedef struct avm_table{
	unsigned			refCounter;
	unsigned			total;
	avm_table_bucket*	numIndexed[AVM_TABLE_HASHSIZE];
	avm_table_bucket*	strIndexed[AVM_TABLE_HASHSIZE];
	/* NEW STUFF */
	avm_table_bucket*	userfuncIndexed[AVM_TABLE_HASHSIZE];
	avm_table_bucket*	libfuncIndexed[AVM_TABLE_HASHSIZE];
	avm_table_bucket*	boolIndexed[AVM_TABLE_BOOL_HASHSIZE];
	avm_table_bucket*	tableIndexed[AVM_TABLE_HASHSIZE];
	/* NEW STUFF */
}avm_table;

avm_table* avm_tablenew(void);
avm_memcell* avm_tablegetelem(avm_table*, avm_memcell*);

static void avm_initstack(void);
void avm_tabledestroy(avm_table*);
void avm_tablesetelem(avm_table*, avm_memcell*, avm_memcell*);
void avm_tableincrefcounter(avm_table*);
void avm_tabledecrefcounter(avm_table*);
void avm_tablebucketsinit(avm_table_bucket**);
void avm_tablebucketsdestroy(avm_table_bucket**);

extern void avm_memcellclear(avm_memcell*);

void memclear_table(avm_memcell*);
void memclear_string(avm_memcell*);


/******************************************************************/
/******************************************************************/

/* Function dispatcher */

/******************************************************************/
/******************************************************************/
#define AVM_NUMACTUALS_OFFSET	+4
#define AVM_SAVEDPC_OFFSET		+3
#define AVM_SAVEDTOP_OFFSET		+2
#define AVM_SAVEDTOPSP_OFFSET	+1
#define AVM_MAX_INSTRUCTIONS	(unsigned) nop_v

typedef void (*execute_func_t)(instruction*);


extern unsigned totalActuals;
extern unsigned char executionFinished;
extern unsigned pc;
extern unsigned currLine;
extern unsigned total_instructions;
extern unsigned current_instruction;
extern instruction* instruction_table;


extern void avm_warning(char*, ...);
extern void avm_error(char*, ...);

/******************************************************************/
/******************************************************************/

/* Declare the execute_... functions */

/******************************************************************/
/******************************************************************/
#define execute_add execute_arithmetic
#define execute_sub execute_arithmetic
#define execute_mul execute_arithmetic
#define execute_div execute_arithmetic
#define execute_mod execute_arithmetic

#define execute_jgt execute_cmp
#define execute_jge execute_cmp
#define execute_jlt execute_cmp
#define execute_jle execute_cmp


typedef int (*cond_jump_t)(double, double);
typedef void (*library_func_t)(void);
typedef double (*arithmetic_func_t)(double, double);
typedef unsigned char (*tobool_func_t)(avm_memcell*);

void execute_arithmetic(instruction * instr);

extern void execute_assign(instruction*);
extern void execute_add(instruction*);
extern void execute_sub(instruction*);
extern void execute_mul(instruction*);
extern void execute_div(instruction*);
extern void execute_mod(instruction*);
extern void execute_uminus(instruction*);
extern void execute_and(instruction*);
extern void execute_or(instruction*);
extern void execute_not(instruction*);
extern void execute_jeq(instruction*);
extern void execute_jne(instruction*);
extern void execute_jle(instruction*);
extern void execute_jge(instruction*);
extern void execute_jlt(instruction*);
extern void execute_jgt(instruction*);
extern void execute_jump(instruction*);
extern void execute_call(instruction*);
extern void execute_pusharg(instruction*);
extern void execute_ret(instruction*);
extern void execute_getretval(instruction*);
extern void execute_funcenter(instruction*);
extern void execute_funcexit(instruction*);
extern void execute_tablecreate(instruction*);
extern void execute_tablegetelem(instruction*);
extern void execute_tablesetelem(instruction*);
extern void execute_nop(instruction*);



extern void avm_dec_top(void);
extern void avm_push_envvalue(unsigned);
extern void avm_calllibfunc(char*);
extern void avm_registerlibfunc(char*, library_func_t);
extern void avm_registerliibfunc(char*, library_func_t);
extern void avm_callsaveenviroment(void);
extern void avm_registerlibfunc(char*, library_func_t);

extern unsigned avm_totalactuals(void);
extern unsigned avm_get_envvalue(unsigned);

extern library_func_t avm_getlibraryfunc(char*);

extern userfunc* avm_getfuncinfo(unsigned);
extern avm_memcell* avm_getactual(unsigned);

void avm_initialize();

/******************************************************************/
/******************************************************************/

/* Boolean operations */

/******************************************************************/
/******************************************************************/

unsigned char number_tobool(avm_memcell*);
unsigned char string_tobool(avm_memcell*);
unsigned char bool_tobool(avm_memcell*);
unsigned char table_tobool(avm_memcell*);
unsigned char userfunc_tobool(avm_memcell*);
unsigned char libfunc_tobool(avm_memcell*);
unsigned char nil_tobool(avm_memcell*);
unsigned char undef_tobool(avm_memcell*);

unsigned char avm_tobool(avm_memcell*);


/******************************************************************/
/******************************************************************/

/* Declare hash functions and lookups for their responsive tables.*/

/******************************************************************/
/******************************************************************/
extern unsigned int numHash(double);
extern unsigned int strHash(const char*);
//extern unsigned int boolHash(unsigned char);
extern unsigned int userfuncHash(unsigned int);

extern avm_table_bucket* numLookup(avm_table_bucket**, avm_memcell*);
extern avm_table_bucket* strLookup(avm_table_bucket**, avm_memcell*);
extern avm_table_bucket* boolLookup(avm_table_bucket**, avm_memcell*);
extern avm_table_bucket* userfuncLookup(avm_table_bucket**, avm_memcell*);
extern avm_table_bucket* libfuncLookup(avm_table_bucket**, avm_memcell*);
extern avm_table_bucket* tableLookup(avm_table_bucket**, avm_memcell*);
/******************************************************************/
/******************************************************************/

#endif