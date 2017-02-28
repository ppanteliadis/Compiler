 
#ifndef stack_h
#define stack_h

#include <stdlib.h>

typedef struct bhta {
    char symbol;
    int lineNo;
    struct bhta *next;
}symbol_stack_t;

symbol_stack_t* shead = NULL;

void pop();

int Last();

symbol_stack_t *top();

void push(char);


#endif