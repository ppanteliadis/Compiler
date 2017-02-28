#ifndef	SYMTABLE_H
#define SYMTABLE_H


#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define HASH_MULTIPLIER 65599
#define TABLE_SIZE 509

int scope_length;

typedef enum SymbolTableType
{
	GLOBAL, LOCAL_VAR, FORMAL,
	USERFUNC, LIBFUNC, NILL
}SymbolType;

extern char * SymbolTableTypeArray[] ;

typedef struct SymbolTableEntry
{
	int isActive;

	char * name;
	unsigned int scope;
	unsigned int line;

	SymbolType type;

	struct SymbolTableEntry * next;
	struct SymbolTableEntry * scope_next;
}SymbolTableEntry;

typedef struct cage
{
	SymbolTableEntry * cageshead;
}Cage;

typedef struct symtable
{
	unsigned int bindings;
	Cage hashtable[TABLE_SIZE];
}SymbolTable;

typedef struct syntaxerror
{
	char * Var;
	unsigned int lineNum;
	char * errorMessage;
	struct syntaxerror *next;
}SyntaxError;


SymbolTable * MySymbolTable;
SymbolTableEntry * * MyScopeList;
extern SyntaxError * syntaxerrorhead;

extern int max_scope;

extern int scope;

extern char * SymbolTableTypeArray[];



static unsigned int SymbolTable_hash(const char * pcKey);

int SymbolTable_insert(SymbolTableEntry *sym);

void SymbolTable_hide_scope(int scope);

void SymbolTable_print();



SymbolTableEntry* Symbol_create(SymbolType, const char *, unsigned int, unsigned int, int);

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


void initialize_system();



#endif
