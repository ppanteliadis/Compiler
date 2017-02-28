/*******************************************************************
 * @file   alpha.c                                                 *
 * @author Steve Cangonj <cangonj@csd.uoc.gr>, 2871                *
 * @author Ioannis-Pavlos Panteliadis <panteliad@csd.uoc.gr>, 2794 *
 * @author Kiriakos Paraskevas <paraskevas@csd.uoc.gr>, 2832       *
 *                                                                 *
 * @brief  Contains all the type declarations and function         *
 *         prototypes needed in order to produce the target        *
 *         code for the alpha programming language.                *
 *******************************************************************/

#include "oneheader.h"


double*		numConsts;
char**		stringConsts;
char**		namedLibfuncs;
userfunc*	userFuncs;
unsigned	currentNumConst, currentStringConst, currentNamedLibfunc, currentUserFunc;
unsigned	totalNumConsts, totalStringConsts, totalNamedLibfuncs, totalUserFuncs;


/**
 * @brief
 */
void expand_numConsts() {
	assert(currentNumConst == totalNumConsts);
	double* n = (double*)malloc(NUMBERS_NEW_SIZE);
	if (numConsts != NULL) {
		memcpy(n, numConsts, NUMBERS_CURR_SIZE);
		free(numConsts);
	}
	numConsts = n;
	totalNumConsts += ARRAY_EXPAND_SIZE;
}

/**
 * @brief
 */
void expand_stringConsts() {
	assert(currentStringConst == totalStringConsts);
	char** s = (char**)malloc(STRINGS_NEW_SIZE);
	if (stringConsts != NULL) {
		memcpy(s, stringConsts, STRINGS_CURR_SIZE);
		free(stringConsts);
	}
	stringConsts = s;
	totalStringConsts += ARRAY_EXPAND_SIZE;
}


/**
 * @brief
 */
void expand_namedLibfuncs() {
	assert(currentNamedLibfunc == totalNamedLibfuncs);
	char** lf = (char**)malloc(LIBFUNCS_NEW_SIZE);

	if (namedLibfuncs != NULL) {
		memcpy(lf, namedLibfuncs, LIBFUNCS_CURR_SIZE);
		free(namedLibfuncs);
	}
	namedLibfuncs = lf;
	totalNamedLibfuncs += ARRAY_EXPAND_SIZE;
}



/**
 * @brief
 */
void expand_userFuncs(){
	assert(currentUserFunc == totalUserFuncs);
	userfunc* uf = (userfunc*)malloc(USERFUNCS_NEW_SIZE);
	if (userFuncs != NULL) {
		memcpy(uf, userFuncs, USERFUNCS_CURR_SIZE);
		free(userFuncs);
	}
	userFuncs = uf;
	totalUserFuncs += ARRAY_EXPAND_SIZE;
}



/**
 * @brief
 */
unsigned consts_newnumber(double n) {
	if (currentNumConst == totalNumConsts) {
		expand_numConsts();
	}
	double* table_pos = numConsts + currentNumConst++;
	*table_pos = n;
	return currentNumConst - 1;
}



/**
 * @brief
 */
unsigned consts_newstring(char* s) {
	if (currentStringConst == totalStringConsts) {
		expand_stringConsts();
	}
	stringConsts[currentStringConst] = (char*)malloc(strlen(s) * sizeof(char));
	stringConsts[currentStringConst++] = strdup(s);

	return currentStringConst - 1;
}



/**
 * @brief
 */
unsigned libfuncs_newused(char* s) {
	if (currentNamedLibfunc == totalNamedLibfuncs) {
		expand_namedLibfuncs();
	}
	namedLibfuncs[currentNamedLibfunc] = (char*)malloc(strlen(s) * sizeof(char));
	namedLibfuncs[currentNamedLibfunc++] = strdup(s);

	return currentNamedLibfunc - 1;
}


/**
 * @brief
 */
unsigned userfuncs_newfunc(SymbolTableEntry* sym) {
	if (currentUserFunc == totalUserFuncs) {
		expand_userFuncs();
	}
	userfunc* uf = userFuncs + currentUserFunc++;
	uf->address = sym->taddress;
	uf->id = strdup(sym->name);

	return currentUserFunc - 1;
}




/**
 * @param	name 		The name we are checking.
 * @brief				Return's 1 if 'name' is a library function.
 */
int isLibraryFunction(char* name) {
	if (strcmp(name,"print") == 0)
		return 1;
	if (strcmp(name,"input") == 0)
		return 1;
	if (strcmp(name,"objectmemberkeys") == 0)
		return 1;
	if (strcmp(name,"objecttotalmembers") == 0)
		return 1;
	if (strcmp(name,"objectcopy") == 0)
		return 1;
	if (strcmp(name,"totalarguments") == 0)
		return 1;
	if (strcmp(name,"typeof") == 0)
		return 1;
	if (strcmp(name,"strtonum") == 0)
		return 1;
	if (strcmp(name,"sqrt") == 0)
		return 1;
	if (strcmp(name,"cos") == 0)
		return 1;
	if (strcmp(name,"sin") == 0)
		return 1;
	if (strcmp(name,"argument") == 0)
		return 1;
	return 0;
}









