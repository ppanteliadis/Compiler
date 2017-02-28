#include "header.h"
#include <stdio.h>
#include <stdlib.h>
#define V_EXPAND_SIZE 1024
#define V_CURR_SIZE (v_total*sizeof(instruction))
#define V_NEW_SIZE (V_EXPAND_SIZE * sizeof(instruction)+V_CURR_SIZE)

typedef enum vmopcode{
    add_v,
    sub_v,
    mul_v,
    div_v,
    mod_v,
    uminus_v,
    newtable_v,
    tablegetelem_v,
    tablesetelem_v,
    assign_v,
    jump_v,
    jeq_v,
    jne_v,
    jgt_v,
    jge_v,
    jlt_v,
    jle_v,
    not_v,
    or_v,
    and_v,
    pusharg_v,
    call_v,
    funcenter_v,
    funcexit_v,
    nop_v
}vmopcode;


typedef enum vmarg_t{
    label_a     =0,
    global_a    =1,
    formal_a    =2,
    local_a     =3,
    number_a    =4,
    string_a    =5,
    bool_a      =6,
    nil_a       =7,
    userfunc_a  =8,
    libfunc_a   =9,
    retval_a    =10
}vmarg_t;

typedef struct vmarg{
    vmarg_t type;
    unsigned  val;
}vmarg;


typedef struct instruction {
    vmopcode opcode;
    vmarg *result;
    vmarg *arg1;
    vmarg *arg2;
    unsigned srcLine;
}instruction;


typedef struct userfunc {
    unsigned taddress;
    unsigned totallocals;
    char * name;
}userfunc;


typedef struct incomplete_jump{
    unsigned instrNo;
    unsigned iaddress;
    struct incomplete_jump *next;
}incomplete_jump;


void generate (vmopcode op, quad *p);

extern void generate_ADD (quad *p);
extern void generate_SUB (quad *p);
extern void generate_MUL (quad *p);
extern void generate_DIV (quad *p);
extern void generate_MOD (quad *p);
extern void generate_NEWTABLE (quad *p);
extern void generate_TABLEGETELEM (quad *p);
extern void generate_TABLESETELEM (quad *p);
extern void generate_ASSIGN (quad *p);
extern void generate_NOP (quad *p);
extern void generate_JUMP (quad *p);
extern void generate_IF_EQ (quad *p);
extern void generate_IF_NOTEQ(quad *p);
extern void generate_IF_GREATER (quad *p);
extern void generate_IF_GREATEREQ(quad *p);
extern void generate_IF_LESS (quad *p);
extern void generate_IF_LESSEQ (quad *p);
extern void generate_NOT (quad *p);
extern void generate_UMINUS (quad *p);
extern void generate_OR (quad *p);
extern void generate_AND (quad *p);
extern void generate_PARAM(quad *p);
extern void generate_CALL(quad *p);
extern void generate_GETRETVAL(quad *p);
extern void generate_FUNCSTART (quad *p);
extern void generate_RETURN(quad *p);
extern void generate_FUNCEND(quad *p);

