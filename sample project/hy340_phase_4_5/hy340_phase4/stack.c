#include "stack.h"

stack_t * MakeEmptyStack(){
    stack_t * newStack;

    newStack = (stack_t*)malloc(sizeof(stack_t));
    newStack->topElement = NULL;
    return newStack;
}

unsigned pop(stack_t * stack){
    unsigned scopeOffset;
    stackElement * tmpStackElement;

    if(stack == NULL){
        printf("Stack POP function null stack pointer\n");
        exit(1);
    }

    if(stack->topElement == NULL){
        printf("Stack POP function empty stack\n");
        exit(1);
    }

    tmpStackElement = stack->topElement;
    stack->topElement = tmpStackElement->next;

    tmpStackElement->next = NULL;
    scopeOffset = tmpStackElement->scopeOffset;

    free(tmpStackElement);
    return scopeOffset;
}


void * pop_list(stack_t * stack){
    void* breakContList = NULL ;
    stackElement * tmpStackElement;

    if(stack == NULL){
        printf("Stack POP function null stack pointer\n");
        exit(1);
    }

    if(stack->topElement == NULL){
        printf("Stack POP function empty stack\n");
        exit(1);
    }

    tmpStackElement = stack->topElement;
    stack->topElement = tmpStackElement->next;

    tmpStackElement->next = NULL;
    breakContList = tmpStackElement->breakContList;

    free(tmpStackElement);

    return breakContList;
}

void* pop_fuc(stack_t * stack){
    return pop_list(stack);
}


int push(stack_t * stack , unsigned scopeOffset ,void* breakContList ){

    stackElement *newStackNode;

    if(stack == NULL){
        printf("Stack PUSH function null stack pointer\n");
        exit(1);
    }

    newStackNode = (stackElement *)malloc(sizeof(stackElement));

    newStackNode->scopeOffset= scopeOffset;
    newStackNode->breakContList = breakContList;
    newStackNode->next = stack->topElement;

    stack->topElement = newStackNode;

    return 0 ;
}


void * top(stack_t * stack){

    if(stack == NULL){
        printf("Stack TOP function null stack pointer\n");
        exit(1);
    }

    if(stack->topElement == NULL){
        printf("Stack TOP function empty stack\n");
        exit(1);
    }

    return stack->topElement->breakContList;

}


