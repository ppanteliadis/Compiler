#include <stdio.h>
#include <stdlib.h>



typedef struct stackElement{
    unsigned scopeOffset;
    void * breakContList;
    struct stackElement *next;
}stackElement;

typedef struct stack{
    struct stackElement *topElement;
}stack_t;

stack_t * MakeEmptyStack();

unsigned pop(stack_t *);

void* pop_list(stack_t *);

void* pop_fuc(stack_t *);

int push(stack_t *, unsigned , void*  );

void * top(stack_t *);
