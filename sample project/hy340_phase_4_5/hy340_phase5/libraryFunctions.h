#include <math.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "header.h"

/*=============================================================*/
/*=============================================================*/
/*=============================================================*/

typedef char * (*tostring_func_t)(avm_memcell*);


char *avm_tostring(avm_memcell *m);
extern char * typeStrings[];

extern char * number_tostring(avm_memcell*);
extern char * string_tostring(avm_memcell*);
extern char * bool_tostring(avm_memcell*);
extern char * table_tostring(avm_memcell*);
extern char * userfunc_tostring(avm_memcell*);
extern char * libfunc_tostring(avm_memcell*);
extern char * nil_tostring(avm_memcell*);
extern char * undef_tostring(avm_memcell*);

/*=============================================================*/

void libfunc_sin(void);

void libfunc_cos(void);

void libfunc_sqrt(void);

void libfunc_argument(void);

void libfunc_totalarguments(void);

void libfunc_typeof(void);

////////////////////////////////////////////////
char * tableValue_ToString(avm_memcell *m);
char * tableKey_ToString(avm_memcell *m);
void libfunc_print(void);
////////////////////////////////////////////////

void libfunc_input(void);

void libfunc_strtonum(void);

void libfunc_objectmemberkeys(void);

void libfunc_objecttotalmembers(void);

void libfunc_objectcopy(void);
