#ifndef OFFSETSTACK_H
#define OFFSETSTACK_H

#include "symtable.h"
#include "quad.h"

typedef struct offset_stack {
	scopespace_t scopespace;
	struct offset_stack *next;
}offset_stack;

extern offset_stack* scopeoffsetstack;

/**
 * @brief
 */
void push_offset(scopespace_t);

/**
 * @brief
 */
scopespace_t pop_offset(void);


#endif