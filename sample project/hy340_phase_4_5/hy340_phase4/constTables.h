#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "tcode.h"


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
unsigned userfuncs_newfunc(SymbolTableEntry *sym);

//------------------------------------------------------------
