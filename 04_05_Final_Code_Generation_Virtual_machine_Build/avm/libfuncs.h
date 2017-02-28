/*******************************************************************
 * @file   libfuncs.h                                              *
 * @author Steve Cangonj <cangonj@csd.uoc.gr>, 2871                *
 * @author Ioannis-Pavlos Panteliadis <panteliad@csd.uoc.gr>, 2794 *
 * @author Kiriakos Paraskevas <paraskevas@csd.uoc.gr>, 2832       *
 *                                                                 *
 * @brief  Contains all initializations and type declarations      *
 *         needed for our virtual machine to incorporate           *
 *         library functions.                                      *
 *******************************************************************/
#ifndef LIBFUNCS_H
#define LIBFUNCS_H

#include <math.h>

#include "oneheader.h"

#define STR_SIZE	512

typedef char* (*tostring_func_t)(avm_memcell*);


char* avm_tostring(avm_memcell*);
extern char* typeStrings[];

extern char* number_tostring(avm_memcell*);
extern char* string_tostring(avm_memcell*);
extern char* bool_tostring(avm_memcell*);
extern char* table_tostring(avm_memcell*);
extern char* userfunc_tostring(avm_memcell*);
extern char* libfunc_tostring(avm_memcell*);
extern char* nil_tostring(avm_memcell*);
extern char* undef_tostring(avm_memcell*);

int get_libfunc_code(char*);
void avm_assignlibfunc(library_func_t, char*);

char* tableValue_toString(avm_memcell*);
char* tableKey_toString(avm_memcell*);

void libfunc_print(void);
void libfunc_input(void);
void libfunc_objectmemberkeys(void);
void libfunc_objecttotalmembers(void);
void libfunc_objectcopy(void);
void libfunc_totalarguments(void);
void libfunc_argument(void);
void libfunc_typeof(void);
void libfunc_strtonum(void);
void libfunc_sqrt(void);
void libfunc_cos(void);
void libfunc_sin(void);


#endif