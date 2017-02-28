#include<stdio.h>
#include<stdbool.h>
#include<stdlib.h>
#include<string.h>
#include<assert.h>

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



/*===================================================================================*/
/*====================================CONST=TABLES===================================*/
/*===================================================================================*/

#define TSTRING_EXPAND_SIZE 1024
#define TSTRING_CURR_SIZE (tstring_total*sizeof(char *))
#define TSTRING_NEW_SIZE (TSTRING_EXPAND_SIZE * sizeof(char *)+TSTRING_CURR_SIZE)

#define TNUM_EXPAND_SIZE 1024
#define TNUM_CURR_SIZE (tnum_total*sizeof(double))
#define TNUM_NEW_SIZE (TNUM_EXPAND_SIZE * sizeof(double)+TNUM_CURR_SIZE)

#define TLIB_EXPAND_SIZE 1024
#define TLIB_CURR_SIZE (tlib_total*sizeof(char *))
#define TLIB_NEW_SIZE (TLIB_EXPAND_SIZE * sizeof(char *)+TLIB_CURR_SIZE)

#define TUSERFUNC_EXPAND_SIZE 1024
#define TUSERFUNC_CURR_SIZE (t_userfunc_total*sizeof( userfunc))
#define TUSERFUNC_NEW_SIZE (TUSERFUNC_EXPAND_SIZE * sizeof( userfunc)+TUSERFUNC_CURR_SIZE)


extern char  **t_string ;
extern unsigned int currSTRING ;
extern unsigned tstring_total ;

extern double *t_num ;
extern unsigned int currNUM ;
extern unsigned tnum_total ;

extern char  **t_lib ;
extern unsigned int currLIB ;
extern unsigned tlib_total ;

extern userfunc  *t_userfunc ;
extern unsigned int currUSERFUNC ;
extern unsigned t_userfunc_total ;


//------------------------------------------------------------

void TSTRING_expand(void);
unsigned consts_newstring(char *);

//------------------------------------------------------------

void TNUM_expand(void);
unsigned consts_newnumber(double);

//------------------------------------------------------------

void TLIB_expand(void);
unsigned libfuncs_newused(const char *);

//------------------------------------------------------------

void T_USERFUNC_expand(void);
unsigned userfuncs_newfunc(char * name ,   unsigned taddress , unsigned totallocals);

//------------------------------------------------------------

/*===================================================================================*/
/*==========================STACK=HEADER=============================================*/
/*===================================================================================*/


#define AVM_STACKSIZE 4096
#define AVM_STACKENV_SIZE 4

struct avm_table;

extern unsigned top;
extern unsigned topsp;


typedef  enum avm_memcell_t{
 number_m       =0,
 string_m       =1,
 bool_m         =2,
 table_m        =3,
 userfunc_m     =4,
 libfunc_m      =5,
 nil_m          =6,
 undef_m        =7
}avm_memcell_t;



typedef struct avm_memcell{
    avm_memcell_t type;
    union{
        double numVal;
        char *strVal;
        bool boolVal;
        struct avm_table * tableVal;
        unsigned funcVal;
        char *libfuncVal;
    } data;

} avm_memcell;

extern avm_memcell stack[AVM_STACKSIZE];
extern avm_memcell ax ;
extern avm_memcell bx ;
extern avm_memcell cx ;
extern avm_memcell retval;


void avm_initStack(void);

double consts_getnumber(unsigned index);

char* consts_getstring(unsigned index);

char* libfuncs_getused(unsigned index);

unsigned userfuncs_getused(unsigned index);

avm_memcell* avm_translate_operand(vmarg *arg, avm_memcell *reg);


#define AVM_TABLE_HASHSIZE 211
#define AVM_TABLE_BOOLEAN_HASHSIZE 2
#define AVM_WIPEOUT(m) memset(&(m),0,sizeof(m))

#define  LibHashFunction StringHashFunction


typedef void (*memclear_func_t)(avm_memcell*);


typedef struct avm_table_bucket{
    avm_memcell *key;
    avm_memcell *value;
    struct avm_table_bucket *next;
} avm_table_bucket;


extern unsigned tableID;

typedef struct avm_table{
    unsigned refCounter;
    unsigned tableIDnumber;
    avm_table_bucket *strIndexed[AVM_TABLE_HASHSIZE];
    avm_table_bucket *numIndexed[AVM_TABLE_HASHSIZE];
    avm_table_bucket *userfuncIndexed[AVM_TABLE_HASHSIZE];
    avm_table_bucket *libraryfuncIndexed[AVM_TABLE_HASHSIZE];
    avm_table_bucket *booleanIndexed[AVM_TABLE_BOOLEAN_HASHSIZE];
    avm_table_bucket *tableIndex[AVM_TABLE_HASHSIZE];
    unsigned total;
} avm_table;


avm_table *avm_tablenew(void);
void avm_tabledestroy(avm_table *t);
avm_memcell * avm_tablegetelem(avm_table *t , avm_memcell *key);
void avm_tablesetelem(avm_table *t , avm_memcell *key, avm_memcell *value);

void avm_tableincrefcounter(avm_table *t);
void avm_tabledecrefcounter(avm_table *t);
void avm_tablebucketsinit(avm_table_bucket **p , int size);
void avm_tablebucketsdestroy(avm_table_bucket **p  ,int size);

void avm_memcellclear(avm_memcell * m);

void memclear_table(avm_memcell *m);

void memclear_string(avm_memcell *m);


/*===================================================================================*/
/*======================================DISPATSER====================================*/
/*===================================================================================*/



#define AVM_MAX_INSTRUCTIONS(unsigned) nop_v
#define AVM_ENDING_PC codeSize


#define AVM_NUMACTUALS_OFFSET   +4
#define AVM_SAVEDPC_OFFSET      +3
#define AVM_SAVEDTOP_OFFSET     +2
#define AVM_SAVEDTOPSP_OFFSET   +1


typedef void (*execute_func_t) (instruction*);

extern unsigned totalActuals;
extern unsigned char executionFinished;
extern unsigned pc;
extern unsigned currLine;
extern unsigned codeSize;
extern instruction *code;



extern void avm_warning(char *format , int scrLine, char * id, char *id1);
extern void avm_error(char *format ,int scrLine , char * id,char *id1);
extern void avm_assign(avm_memcell *lv, avm_memcell *rv , unsigned scrLine);

/*==========================================================*/
/*=================EXECUTE=ASSIGN===========================*/
/*==========================================================*/

extern void execute_assign(instruction*);


/*==========================================================*/
/*===========EXECUTE=ARITHMETIC=OPERATIONS==================*/
/*==========================================================*/

#define execute_add execute_arithmetic
#define execute_sub execute_arithmetic
#define execute_mul execute_arithmetic
#define execute_div execute_arithmetic
#define execute_mod execute_arithmetic


void execute_arithmetic(instruction * instr);

extern void execute_add (instruction*);
extern void execute_sub (instruction*);
extern void execute_mul(instruction*);
extern void execute_div(instruction*);
extern void execute_mod (instruction*);


/*==========================================================*/
/*=============EXECUTE=EQUALITY=OPERATIONS==================*/
/*==========================================================*/

#define execute_jgt execute_cmp
#define execute_jge execute_cmp
#define execute_jlt execute_cmp
#define execute_jle execute_cmp

unsigned char number_tobool(avm_memcell * m);

unsigned char string_tobool(avm_memcell * m);

unsigned char bool_tobool(avm_memcell * m);

unsigned char table_tobool(avm_memcell * m);

unsigned char userfunc_tobool(avm_memcell * m);

unsigned char libfunc_tobool(avm_memcell * m);

unsigned char nil_tobool(avm_memcell * m);

unsigned char undef_tobool(avm_memcell * m);

unsigned char avm_tobool(avm_memcell *m);

void execute_cmp(instruction *instr);

extern void execute_jump(instruction*);
extern void execute_jeq (instruction*);
extern void execute_jne (instruction*);
extern void execute_jle(instruction*);
extern void execute_jge(instruction*);
extern void execute_jlt (instruction*);
extern void execute_jgt(instruction*);


/*==========================================================*/
/*==============EXECUTE=CALLING=FUNCTIONS===================*/
/*==========================================================*/
typedef void (*library_func_t)(void);


extern void execute_call(instruction*);
extern void execute_pusharg(instruction*);
extern void execute_funcenter (instruction*);
extern void execute_funcexit (instruction*);

/////////////////////////////
extern void avm_dec_top(void);
extern void avm_push_envvalue(unsigned val);
extern void avm_calllibfunc(char * funcName );
extern int LibraryNameToInt(char *name);
extern library_func_t avm_getlibraryfunc(char * id);
extern void avm_registerlibfunc(char *id, library_func_t addr);
extern unsigned avm_get_envvalue(unsigned i);
extern void avm_callsaveenvironment(void);
extern userfunc* avm_getfuncinfo(unsigned );
extern void avm_registerliibfunc(char *id, library_func_t addr);
extern avm_memcell *avm_getactual(unsigned i);
extern unsigned avm_totalactuals(void);
/////////////////////////////
extern int G_inFunction;
bool checkIFisNegativeNum(unsigned i);


/*==========================================================*/
/*==============EXECUTE==TABLE==FUNCTIONS===================*/
/*==========================================================*/

extern void execute_newtable (instruction*);
extern void execute_tablegetelem (instruction*);
extern void execute_tablesetelem(instruction*);

/*==========================================================*/
/*==============EXECUTE=NOP=================================*/
/*==========================================================*/

extern void execute_nop(instruction*);

/*==========================================================*/

