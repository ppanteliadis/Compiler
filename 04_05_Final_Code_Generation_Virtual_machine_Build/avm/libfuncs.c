/*******************************************************************
 * @file   libfuncs.c                                              *
 * @author Steve Cangonj <cangonj@csd.uoc.gr>, 2871                *
 * @author Ioannis-Pavlos Panteliadis <panteliad@csd.uoc.gr>, 2794 *
 * @author Kiriakos Paraskevas <paraskevas@csd.uoc.gr>, 2832       *
 *                                                                 *
 * @brief  Implementation file of the 'libfuncs.h' header file     *
 *******************************************************************/


#include "libfuncs.h"


tostring_func_t tostringFuncs[]={
	number_tostring,
	string_tostring,
	bool_tostring,
	table_tostring,
	userfunc_tostring,
	libfunc_tostring,
	nil_tostring,
	undef_tostring
};


library_func_t libFuncs[]={
	libfunc_print,
	libfunc_objectmemberkeys,
	libfunc_objecttotalmembers,
	libfunc_objectcopy,
	libfunc_argument,
	libfunc_typeof,
	libfunc_strtonum,
	libfunc_sqrt,
	libfunc_cos,
	libfunc_sin
};

char* number_tostring(avm_memcell* m) {
	char* str = (char*)malloc(STR_SIZE);
	sprintf(str, "%.2f", m->data.numVal);
	return str;
}

char* string_tostring(avm_memcell* m) {
	return m->data.strVal;
}

char* bool_tostring(avm_memcell* m) {
	switch(m->data.boolVal) {
		case 0: {
			char* str = (char*)malloc(STR_SIZE);
			sprintf(str, "%s", "FALSE");
			return str;
		}
		case 1: {
			char* str = (char*)malloc(STR_SIZE);
			sprintf(str, "%s", "TRUE");
			return str;
		}
		default: fprintf(stderr, "bool_tostring: boolVal has an illegal value!\n"); assert(0);
	}
}


char* table_tostring(avm_memcell* m) {
	/* FIXME */
}

char* userfunc_tostring(avm_memcell* m) {
	/* FIXME */
}

char* libfunc_tostring(avm_memcell* m) {
	/* FIXME */
}


char* nil_tostring(avm_memcell* m) {
	char* str = (char*)malloc(4 * sizeof(char)); /* "NIL" + '\0' = 4 chars!*/
	sprintf(str, "%s", "NIL");
	return str;
}


char* undef_tostring(avm_memcell* m) {
	char* str = (char*)malloc(6 * sizeof(char)); /* "UNDEF" + '\0' = 6 chars!*/
	sprintf(str, "%s", "UNDEF");
	return str;
}



char* avm_tostring(avm_memcell* m){
	assert(m->type >= 0 && m->type <= undef_m);
	return (*tostringFuncs[m->type])(m);
}

char* tableKey_toString(avm_memcell* m){
	switch (m->type) {
		case number_m:		return number_tostring(m);
		case string_m:		return string_tostring(m);
		case bool_m:		return bool_tostring(m);
		case userfunc_m:	return userfunc_tostring(m);
		case libfunc_m:		return libfunc_tostring(m);
		case table_m: {
			/* FIXME */
		}
		case nil_m:			avm_error("trying to convert \'nil\'' to table key!\n");
		case undef_m:		avm_error("trying to convert \'undef\' to table key!\n");
		default: fprintf(stderr, "Not supported table key type!\n"); assert(0);
	}
}

char* tableValue_toString(avm_memcell* m) {
	switch (m->type) {
		case number_m:		return number_tostring(m);
		case string_m:		return string_tostring(m);
		case bool_m:		return bool_tostring(m);
		case userfunc_m:	return userfunc_tostring(m);
		case libfunc_m:		return libfunc_tostring(m);
		case table_m:		return table_tostring(m);
		case nil_m:			return nil_tostring(m);
		case undef_m:		return undef_tostring(m);
		default: fprintf(stderr, "Not supported table key type!\n"); assert(0);
	}
}



void libfunc_print(void) {
	unsigned n = avm_totalactuals();
	unsigned i;
	for (i = 0; i < n; i++) {
		char* s = avm_tostring(avm_getactual(i));
		puts(s);
		//free(s);			/* FIXME CARE MAYBE IT WILL CAUSE ERRORS LATER ON... */
	}
}


void libfunc_input(void) {

}


void libfunc_objectmemberkeys(void) {}
void libfunc_objecttotalmembers(void) {}
void libfunc_objectcopy(void) {}


void libfunc_totalarguments(void) {
	/* Get topsp of previous activation record. */
	unsigned p_topsp = avm_get_envvalue(topsp + AVM_SAVEDTOPSP_OFFSET);
	avm_memcellclear(&retval);

	if (!p_topsp) { /*if 0, no previous activation record.*/
		avm_error("totalarguments() library function cannot be called outside a function!");
		retval.type = nil_m;
	}
	else {
		/* Extract the number of actual arguments for the previous 
		 * activation record.
		 */
		retval.type = number_m;
		retval.data.numVal = avm_get_envvalue(p_topsp + AVM_NUMACTUALS_OFFSET);
	}

}


void libfunc_argument(void) {

}


void libfunc_typeof() {
	unsigned n = avm_totalactuals();
	if (n != 1) {
		avm_error("one (1) argument (not %d) expected in \'typeof()\' library function!\n", n);
	}
	else {
		/* That's how a library func returns a result.
		 * It has to only set the 'retval' register!
		 */
		avm_memcellclear(&retval);	/* Don't forget to clean it up! */
		retval.type = string_m;
		retval.data.strVal = strdup(typeStrings[avm_getactual(0)->type]);
	}
}


void libfunc_strtonum(void) {
	unsigned n = avm_totalactuals();
	if (n != 1) {
		avm_error("at least one argument is expected in strtonum() library function!\n");
	}
	else {
		if (avm_getactual(0)->type != string_m) {
			avm_error("strtonum() library function, expects a \'string\'' as an argument!\n");
		}
		else {
			double f = atof(avm_getactual(0)->data.strVal);
			if (f != 0.0) {
				retval.type = number_m;
				retval.data.numVal = f;
			}
			else {
				avm_warning("successfull conversion of string %s to num failed!\n", avm_getactual(0)->data.strVal);
				retval.type = nil_m;
			}
			
		}
	}
}


void libfunc_sqrt(void) {
	unsigned n = avm_totalactuals();
	if (n != 1) {
		avm_error("at least one argument is expected in sqrt() library function!\n");
	}
	else {
		if (stack[topsp + AVM_STACKENV_SIZE + 1].data.numVal < 0) {
			avm_warning("sqrt() library function expects only non-negative numbers!\n");
		}
		else {
			unsigned env = avm_get_envvalue(topsp + AVM_STACKENV_SIZE + 1);
			avm_memcellclear(&retval);

			retval.type = number_m;
			retval.data.numVal = cos(env);
		}
		
	}
}


void libfunc_cos(void) {
	unsigned n = avm_totalactuals();
	if (n != 1) {
		avm_error("at least one argument is expected in cos() library function!\n");
	}
	else {
		unsigned env = avm_get_envvalue(topsp + AVM_STACKENV_SIZE + 1);
		avm_memcellclear(&retval);

		retval.type = number_m;
		retval.data.numVal = cos(env);
	}
}


void libfunc_sin(void) {
	unsigned n = avm_totalactuals();
	if (n != 1) {
		avm_error("at least one argument is expected in sin()! library function!\n");
	}
	else {
		unsigned env = avm_get_envvalue(topsp + AVM_STACKENV_SIZE + 1);
		avm_memcellclear(&retval);

		retval.type = number_m;
		retval.data.numVal = sin(env);
	}
}


/**
 * @param	name 		The name we are checking.
 * @brief				Return's the correspondings library functions index.
 */
int get_libfunc_code(char* name) {
	if (strcmp(name, "print") == 0)
		return 0;
	else if (strcmp(name, "input") == 0)
		return 1;
	else if (strcmp(name, "objectmemberkeys") == 0)
		return 2;
	else if (strcmp(name, "objecttotalmembers") == 0)
		return 3;
	else if (strcmp(name, "objectcopy") == 0)
		return 4;
	else if (strcmp(name, "totalarguments") == 0)
		return 5;
	else if (strcmp(name, "argument") == 0)
		return 6;
	else if (strcmp(name, "typeof") == 0)
		return 7;
	else if (strcmp(name, "strtonum") == 0)
		return 8;
	else if (strcmp(name, "sqrt") == 0)
		return 9;
	else if (strcmp(name, "cos") == 0)
		return 10;
	else if (strcmp(name, "sin") == 0)
		return 11;
	else
		return -1;
}


void avm_registerlibfunc(char* name, library_func_t addr) {
	if (strcmp(name, "")==0 || name == NULL) {
		fprintf(stderr, "avm_assignlibfunc: \'%s\' is not a library function.\n");
		assert(0);
	}
	else {
		int index = get_libfunc_code(name);
		if (index != -1) {
			libFuncs[index] = addr;
		}
		else {
			fprintf(stderr, "avm_assignlibfunc: \'%s\' is not a library function.\n");
			assert(0);
		}
	}
}





