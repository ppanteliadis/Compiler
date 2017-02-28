/*******************************************************************
 * @file   quad.h                                                  *
 * @author Steve Cangonj <cangonj@csd.uoc.gr>, 2871                *
 * @author Ioannis-Pavlos Panteliadis <panteliad@csd.uoc.gr>, 2794 *
 * @author Kiriakos Paraskevas <paraskevas@csd.uoc.gr>, 2832       *
 *                                                                 *
 * @brief  Contains all the type declarations and function         *
 *         prototypes needed in order to handle the quad A.D.T.,   *
 *         as this is defined in our Grammar.                      *
 *******************************************************************/

#ifndef QUAD_H
#define QUAD_H

#include <stdarg.h>
#include <string.h>
#include <math.h>

#include "symtable.h"

#define EXPAND_SIZE		1024
#define CURR_SIZE		(total_quads*sizeof(quad))
#define NEW_SIZE		(EXPAND_SIZE*sizeof(quad) + CURR_SIZE)

#define STR_SIZE		128

typedef enum iopcode {
	assign,			//0
	add,			//1
	sub,			//2
	mul,			//3
	divi,			//4
	mod,			//5
	uminus,			//6
	and,			//7
	or,				//8
	not,			//9
	if_eq,			//10
	if_noteq,		//11
	if_lesseq,		//12
	if_greatereq,	//13
	if_less,		//14
	if_greater,		//15
	jump,			//16
	call,			//17
	param,			//18
	tablecreate,	//19
	tablegetelem,	//20
	tablesetelem,	//21
	funcstart,		//22
	funcend,		//23
	retur,			//24
	getretval,		//25
}iopcode;



typedef enum expr_t{
	var_e,
	tableitem_e,

	programfunc_e,
	libraryfunc_e,

	arithexpr_e,
	boolexpr_e,
	assignexpr_e,
	newtable_e,

	constnum_e,
	constbool_e,
	conststring_e,

	nil_e
}expr_t;

typedef struct expr {
	expr_t type;
	SymbolTableEntry* sym;
	struct expr* index;
	double numConst;
	char* strConst;
	unsigned int boolConst;
	struct expr* next;
}expr;

typedef struct call_t {
	expr* elist;
	int method;			//Boolean: 0,1
	char* name;
}call_t;

typedef struct quad {
	iopcode op;
	expr* result;
	expr* arg1;
	expr* arg2;
	unsigned label;
	unsigned line;
	unsigned taddress;
}quad;



typedef struct semanticerror {
	unsigned int lineNum;
	iopcode op;
	char* var;
	struct semanticerror *next;
}semanticerror;


extern semanticerror* semanticerrorhead;


/**
 * @brief Expand the quad array.
 */
void expand_quad_table();

/**
 * @param
 * @param
 * @param
 * @brief Create a new iopcode quad and insert it in the table
 */
void emit(iopcode, expr *, expr *, expr *, unsigned int, unsigned int);

/**
 * @brief
 */
expr* emit_iftableitem(expr*);


/**
 * @brief
 */
void emit_param(expr*);


/**
 *
 * @brief Creates a new expresion
 */
expr * newexpr(expr_t);

/**
 * @brief
 */
expr * expr_lvalue(SymbolTableEntry *);

/**
 * @brief
 */
expr * member_item(expr *, char*);

/**
 * @brief
 */
expr * expr_boolConst(unsigned int);

/**
 * @brief
 */
expr * expr_strConst(char *);

/**
 * @brief
 */
expr * expr_numConst(int);


/**
 * @brief
 */
expr* make_call(expr*, expr*);


/**
 * @brief
 */
expr* add_front(expr*, expr*);


/**
 * @brief
 */
expr* compute_ifnum(expr*, expr*, iopcode);


/**
 * @brief
 */
expr* compute_ifbool(expr*, expr*, iopcode);


/**
 * @brief
 */
expr* compute_ifstr(expr*, expr*, iopcode, int);

/**
 * @brief
 */
void checkuminus(expr* e);



/**
 * @param		elist		
 * @param		name 		A Name for the new methodcall.
 * @brief					Creates a new methodcall and returns it.
 */
call_t* make_methodcall(expr*, char*);


/**
 * @param		elist		
 * @brief					Creates a new methodcall and returns it.
 */
call_t* make_normcall(expr*);


/**
 *
 * @brief Creates the hidden variable and returns it
 */
char* newtempname(void);

/**
 *
 * @brief Resets the temp counter to 0
 */
void resettempname(void);

/**
 * @brief
 */
SymbolTableEntry * newtemp(void);

/**
 *
 * @brief Returns the current scope space
 */
scopespace_t currscopespace(void);

/**
 *
 * @brief Returns the current scope offset
 */
unsigned currscopeoffset(void);

/** 
 *
 * @brief Increments the current scope offset depending on the offset type
 */
void incurrscopeoffset(void);

/**
 *
 * @brief Increments the scopeSpaceCounter
 */
void enterscopespace(void);

/**
 * 
 * @brief Decrements the scopeSpaceCounter
 */
void exitscopespace(void);

/**
 *
 * @brief Resets the formal arg offset to 0.
 */
void resetformalargoffset(void);

/**
 * @brief Resets the function locals offset to 0.
 */
void resetfunctionlocalsoffset(void);

/**
 *
 * @brief Restores the current scope offset to n.
 */
void restorecurrscopeoffset(unsigned);

/**
 * @brief Returns the label of the next quad
 */
unsigned int nextquadlabel(void);

/**
 * @brief		label			The label of the quad.
 * @param		guadNo			The quad that needs patching number.
 */
void patchlabel(unsigned, unsigned);

/**
 * @brief
 */
void print_quad_table(void);

/**
 * @brief
 */
unsigned int istempname(char*);

/**
 * @brief 
 */
unsigned int istempexpr(expr*);


void SemanticError_insert(char*, iopcode, int);

void SemanticError_print();

void print_intermiediate_code();


#endif
