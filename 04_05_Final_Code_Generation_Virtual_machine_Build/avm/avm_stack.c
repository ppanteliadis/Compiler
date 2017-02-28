/*******************************************************************
 * @file   avm_stack.h                                             *
 * @author Steve Cangonj <cangonj@csd.uoc.gr>, 2871                *
 * @author Ioannis-Pavlos Panteliadis <panteliad@csd.uoc.gr>, 2794 *
 * @author Kiriakos Paraskevas <paraskevas@csd.uoc.gr>, 2832       *
 *                                                                 *
 * @brief  One of the many implementation files for oneheader.h    *
 *******************************************************************/


#include "oneheader.h"

extern double*		numConsts;
extern char**		stringConsts;
extern char**		namedLibfuncs;
extern userfunc*	userFuncs;
extern unsigned	currentNumConst, currentStringConst, currentNamedLibfunc, currentUserFunc;
extern unsigned	totalNumConsts, totalStringConsts, totalNamedLibfuncs, totalUserFuncs;


avm_memcell stack[AVM_STACKSIZE];
avm_memcell ax, bx, cx, retval;

unsigned top, topsp;


double consts_getnumber (int i) {
	if (i < 0 || i > currentNumConst) {
		fprintf(stderr, "consts_getnumber: index out of bounds!\n");
		assert(0);
	}
	return numConsts[i];
}


char* consts_getstring (int i) {
	if (i < 0 || i > currentStringConst) {
		fprintf(stderr, "consts_getstring: index out of bounds!\n");
		assert(0);
	}
	return stringConsts[i];
}


char* libfuncs_getused (int i) {
	if (i < 0 || i > currentNamedLibfunc) {
		fprintf(stderr, "libfuncs_getused: index out of bounds!\n");
		assert(0);
	}
	return namedLibfuncs[i];
}


unsigned userfuncs_getused(int i) {
	if (i < 0 || i > currentUserFunc) {
		fprintf(stderr, "userfuncs_getused: index out of bounds!\n");
		assert(0);
	}
	return userFuncs[i].address;
}

avm_memcell* avm_translate_operand(vmarg* arg, avm_memcell* reg) {
	switch (arg->type) {
		/* Variables */
		case global_a:		return &stack[AVM_STACKSIZE - 1 - arg->val];
		case local_a:		return &stack[topsp - arg->val];
		case formal_a:		return &stack[topsp + AVM_STACKENV_SIZE + 1 + arg->val];
		case retval_a:		return &retval;

		/* Constants */
		case number_a: {
			reg->type = number_m;
			reg->data.numVal = consts_getnumber(arg->val);
			return reg;
		}
		case string_a: {
			reg->type = string_m;
			reg->data.strVal = strdup(consts_getstring(arg->val));
			return reg;
		}
		case bool_a: {
			reg->type = bool_m;
			reg->data.boolVal = arg->val;
			return reg;
		}
		case nil_a: reg->type = nil_m; return reg;
		
		/* Functions */
		case userfunc_a: {
			reg->type = userfunc_m;
			reg->data.funcVal = arg->val;	/* Address already stored! */
			return reg;
		}
		case libfunc_a: {
			reg->type = libfunc_m;
			reg->data.libfuncVal = libfuncs_getused(arg->val);
			return reg;
		}
		default: fprintf(stderr, "avm_translate_operand: Undefined vmarg type!\n"); assert(0);
	}
}