/*******************************************************************
 * @file   targetcode.h                                            *
 * @author Steve Cangonj <cangonj@csd.uoc.gr>, 2871                *
 * @author Ioannis-Pavlos Panteliadis <panteliad@csd.uoc.gr>, 2794 *
 * @author Kiriakos Paraskevas <paraskevas@csd.uoc.gr>, 2832       *
 *                                                                 *
 * @brief  Contains all the type declarations and function         *
 *         prototypes needed in order to produce the target        *
 *         code for the alpha programming language.                *
 *******************************************************************/

#ifndef TARGETCODE_H
#define TARGETCODE_H

#include <stdio.h>
#include <stdlib.h>

#include "quad.h"
#include "funcstack.h"


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



extern quad* quad_table;
extern int total_quads;


typedef void (*generator_func_t)(quad*);

extern generator_func_t generators[];

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


typedef struct incomplete_jump {
	unsigned	instrNo;
	unsigned	iaddress;
	struct incomplete_jump* next;
}incomplete_jump;


void expand_instruction_table();
void emit_instruction(instruction*);
void add_incomplete_jump(unsigned, unsigned);
void patch_incomplete_jumps();


void make_operand(expr*, vmarg*);
void make_numberoperand(vmarg*, double);
void make_stringoperand(vmarg*, char*);
void make_booloperand(vmarg*, unsigned);
void make_retvaloperand(vmarg*);

void reset_operand(vmarg*);

unsigned int nextinstructionlabel();
unsigned int currprocessedquad();


void init_const_arrays();
void free_const_arrays();

void expand_numConsts();
void expand_stringConsts();
void expand_namedLibfuncs();
void expand_userFuncs();


unsigned consts_newnumber(double);
unsigned consts_newstring(char*);
unsigned libfuncs_newused(char*);
unsigned userfuncs_newfunc(SymbolTableEntry*);


void numbers_to_binary(FILE*);
void strings_to_binary(FILE*);
void libfuncs_to_binary(FILE*);
void userfuncs_to_binary(FILE*);
void targetcode_to_binary(FILE*);


void arg_to_binary(vmarg*, FILE*);


void print_constnums();
void print_conststrings();
void print_userfuncs();
void print_libfuncs();
void print_const_tables();
void print_targetcode();


void numbers_to_file(FILE*);
void strings_to_file(FILE*);
void libfuncs_to_file(FILE*);
void userfuncs_to_file(FILE*);
void targetcode_to_file(FILE*);


void avm_binaryfile();
void avm_normalfile();



void generate(vmopcode, quad*);
void generate_relational(vmopcode, quad*);
void generate_targetcode(int);

extern void generate_ADD(quad*);
extern void generate_SUB(quad*);
extern void generate_MUL(quad*);
extern void generate_DIV(quad*);
extern void generate_MOD(quad*);
extern void generate_TABLECREATE(quad*);
extern void generate_TABLEGETELEM(quad*);
extern void generate_TABLESETELEM(quad*);
extern void generate_ASSIGN(quad*);
extern void generate_NOP(quad*);
extern void generate_JUMP(quad*);
extern void generate_IF_EQ(quad*);
extern void generate_IF_NOTEQ(quad*);
extern void generate_IF_GREATER(quad*);
extern void generate_IF_GREATEREQ(quad*);
extern void generate_IF_LESS(quad*);
extern void generate_IF_LESSEQ(quad*);
extern void generate_UMINUS(quad*);
extern void generate_NOT(quad*);
extern void generate_AND(quad*);
extern void generate_OR(quad*);
extern void generate_PARAM(quad*);
extern void generate_CALL(quad*);
extern void generate_GETRETVAL(quad*);
extern void generate_FUNCSTART(quad*);
extern void generate_RETURN(quad*);
extern void generate_FUNCEND(quad*);


vmarg* new_vmarg();


returnList* returnInsert(SymbolTableEntry*, unsigned int);

#endif