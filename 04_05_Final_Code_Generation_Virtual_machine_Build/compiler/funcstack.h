#ifndef FUNCSTACK_H
#define FUNCSTACK_H

#include "symtable.h"



typedef struct func {
	SymbolTableEntry *sym;
	struct func *next;
}func;

extern func* funcstack;

func* push_func(func*, SymbolTableEntry**);
func* pop_func(func*, SymbolTableEntry**);
func* top_func(func*, SymbolTableEntry**);



#endif