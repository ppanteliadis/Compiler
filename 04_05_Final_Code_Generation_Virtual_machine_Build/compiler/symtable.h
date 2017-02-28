/*******************************************************************
 * @file   symtable.h                                              *
 * @author Steve Cangonj <cangonj@csd.uoc.gr>, 2871                *
 * @author Ioannis-Pavlos Panteliadis <panteliad@csd.uoc.gr>, 2794 *
 * @author Kiriakos Paraskevas <paraskevas@csd.uoc.gr>, 2832       *
 *                                                                 *
 * @brief  Contains all the type declarations and function         *
 *         prototypes needed in order to handle the symtable       *
 *         A.D.T., as this is defined in our Grammar.              *
 *******************************************************************/

#ifndef	SYMTABLE_H
#define SYMTABLE_H


#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>


#define HASH_MULTIPLIER 65599
#define TABLE_SIZE 509
#define STR_SIZE 128

int scope_length;

typedef enum scopespace_t {
	program_variable = 1,
	function_local = 2,
	formal_arg = 3
}scopespace_t;


typedef enum SymbolTableType {
	GLOBAL, LOCAL_VAR, FORMAL,
	USERFUNC, LIBFUNC, NILL
}SymbolType;


typedef enum Symbol_t {
	var_s,
	programfunc_s,
	libraryfunc_s
}Symbol_t;


extern char* SymbolTableTypeArray[];
extern char* ScopeSpaceArray[];

typedef struct returnList {
	unsigned int label;
	struct returnList* next;
}returnList;

typedef struct SymbolTableEntry {
	int isActive;

	char* name;
	unsigned int scope;
	unsigned int line;

	SymbolType type;
	scopespace_t scopespace;
	unsigned int offset;

	unsigned int totallocals;
	unsigned int iaddress;

	unsigned taddress;
	returnList* retList;
	unsigned incJump;

	struct SymbolTableEntry * next;
	struct SymbolTableEntry * scope_next;
}SymbolTableEntry;


typedef struct cage {
	SymbolTableEntry* cageshead;
}Cage;


typedef struct symtable {
	unsigned int bindings;
	Cage hashtable[TABLE_SIZE];
}SymbolTable;


typedef struct forprefix {
	unsigned int test;
	unsigned int enter;
}forprefix;

typedef struct syntaxerror
{
	char * Var;
	unsigned int lineNum;
	char * errorMessage;
	struct syntaxerror *next;
}SyntaxError;


SymbolTable* MySymbolTable;
SymbolTableEntry** MyScopeList;

extern SyntaxError * syntaxerrorhead;
extern int max_scope;
extern int scope;
extern char * SymbolTableTypeArray[];



static unsigned int SymbolTable_hash(const char * pcKey);

int SymbolTable_insert(SymbolTableEntry *sym);

void SymbolTable_hide_scope(int scope);

void SymbolTable_print();



SymbolTableEntry* Symbol_create(SymbolType, const char *, unsigned int, 
				unsigned int, int);

SymbolTableEntry* Symbol_lookup(unsigned int, const char*);

int Symbol_activate(SymbolTableEntry *);

int Symbol_deactivate(SymbolTableEntry *);



void SyntaxError_insert(char *, char *, int);

void SyntaxError_print();


void raisemaxScope();

char* createNamelessFunc();

int isActiveFunction(int);
int isActiveFunctionScope(int);
int isPredefinedFunction(int, const char* name);
int isLibraryFunction(char*);
int get_total_globals();

void inc_globals();

void append(SymbolTableEntry*, unsigned int);
void backpatch(SymbolTableEntry*, unsigned int);



void initialize_system();

void print_symtable_and_errors();


#endif
