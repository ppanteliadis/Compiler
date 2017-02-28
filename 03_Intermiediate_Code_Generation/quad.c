/*******************************************************************
 * @file   quad.c                                                  *
 * @author Steve Cangonj <cangonj@csd.uoc.gr>, 2871                *
 * @author Ioannis-Pavlos Panteliadis <panteliad@csd.uoc.gr>, 2794 *
 * @author Kiriakos Paraskevas <paraskevas@csd.uoc.gr>, 2832       *
 *                                                                 *
 * @brief  Implementation file of the 'quad.h' header file         *
 *******************************************************************/

#include "quad.h"

int tempcounter		= 0;

quad * quad_table	= NULL;
int total_quads		= 0;
int current_quad	= 0;

unsigned programVarOffset	 = 0;
unsigned functionLocalOffset = 0;
unsigned formalArgOffset	 = 0;
unsigned scopeSpaceCounter	 = 1;

extern int scope;
expr* func_params_head		 = NULL;
extern char* OpcodeArray[];

/**
 * @brief Expand the quad array.
 */
void expand_quad_table() {
	assert(current_quad == total_quads);
	quad* q = (quad*)malloc(NEW_SIZE);
	if (quad_table != NULL) {
		memcpy(q,quad_table, CURR_SIZE);
		free(quad_table);
	}
	quad_table = q;
	total_quads += EXPAND_SIZE;
}

/**
 * @brief Create a new iopcode quad and insert it in the table
 */
void emit(iopcode op, expr * arg1, expr * arg2, expr * result, unsigned int label, unsigned int line) {
	if (current_quad == total_quads) {
		expand_quad_table();
	}
	//printf("arg1 = %s\narg2 = %s\nresult = %s\n\n", arg1->sym->name, arg2->sym->name, result->sym->name);
	quad* quad_entry = quad_table + current_quad++;
	quad_entry->op = op;
	quad_entry->result = result;
	quad_entry->arg1 = arg1;
	quad_entry->arg2 = arg2;
	quad_entry->label = label;
	quad_entry->line = line;
}


/**
 * @brief   Parameters are required to be emited in a reverse order. But we only
 *          have a next pointer. So in order to achieve this, we need a recursive
 *          function.
 */
void emit_param(expr* e) {
	if (e == NULL) {
		return;
	}
	emit_param(e->next);
	emit(param, e, (expr*)NULL, (expr*)NULL, -1, -1);
}

/**
 * @brief
 */
expr * emit_iftableitem(expr * e) {
	if (e->type != tableitem_e) {
		return e;
	}
	else {
		expr * result = newexpr(var_e);
		result->sym = newtemp();
		emit(tablegetelem, e, e->index, result, -1, -10);
		return result;
	}
}

/**
 *
 * @brief Creates a new expresion
 */
expr * newexpr(expr_t type) {
	expr * newentry;
	newentry = (expr *)malloc(sizeof(expr));
	newentry->type = type;
	return newentry;
}

/**
 * @brief
 */
expr * expr_lvalue(SymbolTableEntry * symbol) {
	assert(symbol);
	expr * newentry;
	newentry = (expr *)malloc(sizeof(expr));
	newentry->sym = (SymbolTableEntry*)malloc(sizeof(SymbolTableEntry));

	newentry->sym = symbol;
	newentry->next = (expr*)NULL;

	assert(newentry->sym);
	if (symbol->type == GLOBAL || symbol->type == LOCAL_VAR || symbol->type == FORMAL)
	{
		newentry->type = var_e;
	}
	else if (symbol->type == USERFUNC)
	{
		newentry->type = programfunc_e;
	}
	else if (symbol->type == LIBFUNC)
	{
		newentry->type = libraryfunc_e;
	}
	return newentry;
}

/**
 * @brief In case
 */
expr * member_item(expr * lvalue, char* name) {
	lvalue = emit_iftableitem(lvalue);
	expr * item = newexpr(tableitem_e);
	item->sym = lvalue->sym;
	item->index = expr_strConst(name);
	return item;
}

/**
 *
 * @brief
 */
expr * expr_boolConst(unsigned int boolConst) {
	expr * newentry = (expr *) malloc (sizeof(expr));
	newentry->type = constbool_e;
	newentry->boolConst = boolConst;
	return newentry;
}

/**
 *
 * @brief
 */
expr * expr_strConst(char * strConst) {
	expr * newentry = newexpr(conststring_e);
	newentry->strConst = strdup(strConst);
	return newentry;
}


/**
 * @param 	numConst 	
 * @brief				
 */
expr * expr_numConst(int numConst) {
	expr* newentry = newexpr(constnum_e);
	newentry->numConst = numConst;
	return newentry;
}



/**
 * @param		lvalue		An expression pointer containing all info on the processed symbol.
 * @param		elist		A list containing all parameters being parsed to the function call.
 * @brief					Fills the quad array with the appropriate parameters for a successfull function call.
 */
expr* make_call(expr* lvalue, expr* elist) {
	expr* func = emit_iftableitem(lvalue);
	emit_param(elist);

	if (lvalue->sym->type == LIBFUNC) {
		func = newexpr(libraryfunc_e);
		func->sym = (SymbolTableEntry*)malloc(sizeof(SymbolTableEntry));
		func->sym->name = lvalue->sym->name;
	}
	else if (lvalue->sym->type == USERFUNC) {
		func = newexpr(programfunc_e);
		func->sym = (SymbolTableEntry*)malloc(sizeof(SymbolTableEntry));
		func->sym->name = lvalue->sym->name;
	}
	emit(call, func, (expr*)NULL, (expr*)NULL, -1, -1);
	expr* result = newexpr(var_e);
	result->sym = newtemp();
	emit(getretval, result, (expr*)NULL, (expr*)NULL, -1, -1);
	return result;
}



/**
 * @param		expr1		The first expression
 * @param		expr2		The second expression
 * @param		op			The opcode of the operation
 * @brief					Returns the result of the arithmetic operation beetween expr1 and expr2.
 */
expr* compute_ifnum(expr* expr1, expr* expr2, iopcode op) {
	if (expr1->type == constnum_e && expr2->type == constnum_e) {
		/* valid expressions! */
		expr* result = newexpr(constnum_e);
		switch (op) {
			case add:
				result->numConst = expr1->numConst + expr2->numConst;
				break;
			case sub:
				result->numConst = expr1->numConst - expr2->numConst;
				break;
			case mul:
				result->numConst = expr1->numConst * expr2->numConst;
				break;
			case divi:
				result->numConst = expr1->numConst / expr2->numConst;
				break;
			case mod:
				result->numConst = fmod(expr1->numConst, expr2->numConst);
				break;
		}
		return result;
	}
	return NULL;
}



/**
 * @param		expr1		The first expression
 * @param		expr2		The second expression
 * @param		op			The opcode of the operation
 * @brief					Returns the result of the boolean operation beetween expr1 and expr2.
 */
expr* compute_ifbool(expr* expr1, expr* expr2, iopcode op) {
	if (expr1->type == constbool_e && expr2->type == constbool_e) {
		/* valid expressions! */
		expr* result = newexpr(constbool_e);
		switch (op) {
			case and:
				result->boolConst = expr1->boolConst && expr2->boolConst;
				break;
			case or:
				result->boolConst = expr1->boolConst || expr2->boolConst;
				break;
		}
		return result;
	}
	return NULL;
}


/**
 * @param		expr1		The first expression
 * @param		expr2		The second expression
 * @param		op			The opcode of the operation
 * @brief					Returns the result of the string operation beetween expr1 and expr2.
 */
expr* compute_ifstr(expr* expr1, expr* expr2, iopcode op, int line) {
	if (expr1->type == conststring_e && expr2->type == conststring_e) {
		/* valid expressions! */
		expr* result = newexpr(conststring_e);
		switch (op) {
			case add:
				result->strConst = strdup(strcat(expr1->strConst, expr2->strConst));
				break;
			case sub:
				while (expr1->strConst = strstr(expr1->strConst, expr2->strConst)) {
					memmove(expr1->strConst, expr1->strConst + strlen(expr2->strConst), 1 + strlen(expr1->strConst + strlen(expr2->strConst)));
				}
				break;
			default:
				SemanticError_insert("strings", op, line);
		}
		return result;
	}
	return NULL;
}



/**
 * @param		base		
 * @param		front		
 * @brief					Adds front to ahead of base and returns a pointer to new base.
 */
expr* add_front(expr* base, expr* front) {
	front->next = base;
	base = front;
	return base;
}



/**
 * @param		e			
 * @brief					Adds front to ahead of base and returns a pointer to new base.
 */
void checkuminus(expr* e) {
	if (e->type == constbool_e 		||
		e->type == conststring_e	||
		e->type == nil_e			||
		e->type == newtable_e		||
		e->type == programfunc_e	||
		e->type == libraryfunc_e	||
		e->type == boolexpr_e		) {
		printf("Compiler Error: Illegal expression to unary -");
		exit(-1);
	}
}

/**
 * @param		elist		
 * @param		name 		A Name for the new methodcall.
 * @brief					Creates a new methodcall and returns it.
 */
call_t* make_methodcall(expr* elist, char* name) {
	call_t* new_methodcall = (call_t*)malloc(sizeof(call_t));
	if (new_methodcall == NULL) {
		printf("make_methodcall: Memory allocation error!\n");
		exit(1);
	}
	new_methodcall->elist = elist;
	new_methodcall->method = 1;		//TRUE
	new_methodcall->name = strdup(name);
	return new_methodcall;
}


/**
 * @param		elist		
 * @brief					Creates a new methodcall and returns it.
 */
call_t* make_normcall(expr* elist) {
	call_t* new_normcall = (call_t*)malloc(sizeof(call_t));
	if (new_normcall == NULL) {
		printf("make_normcall: Memory allocation error!\n");
		exit(1);
	}
	new_normcall->elist = elist;
	new_normcall->method = 0;		//FALSE
	new_normcall->name = NULL;
	return new_normcall;
}



/**
 *
 * @brief Creates the hidden variable and returns it
 */
char * newtempname(void) {
	char* tempname = malloc(STR_SIZE);
	sprintf(tempname, "_t%d" , tempcounter++);
	return tempname;
}

/**
 *
 * @brief Resets the temp counter to 0
 */
void resettempname(void) {
	tempcounter = 0;
}

/**
 * @brief
 */
SymbolTableEntry* newtemp(void) {
	const char* name = newtempname();
	SymbolTableEntry * sym = Symbol_lookup(scope, name);
	if (sym == NULL) {
		return Symbol_create(LOCAL_VAR, name, 0, scope, 1);
	}
	else {
		return sym;
	}
}



/**
 *
 * @brief Returns the current scope space
 */
scopespace_t currscopespace() {
	if (scopeSpaceCounter == 1) {
		return program_variable;
	}
	else if (scopeSpaceCounter % 2 == 0) {
		return formal_arg;
	}
	else {
		return function_local;
	}
}



/**
 * 
 * @brief Returns the current scope offset
 */
unsigned currscopeoffset() {
	switch (currscopespace()) {
		case program_variable:
			return programVarOffset;
		case function_local:
			return functionLocalOffset;
		case formal_arg:
			return formalArgOffset;
		default:
			assert(0);
	}
}



/**
 *
 * @brief Increments the current scope offset depending on the offset type
 */
void incurrscopeoffset() {
	switch (currscopespace()) {
		case program_variable:
			++programVarOffset;
			break;
		case function_local:
			++functionLocalOffset;
			break;
		case formal_arg:
			++formalArgOffset;
			break;
		default:
			assert(0);
	}
}


/**
 *
 * @brief Increments the scopeSpaceCounter
 */
void enterscopespace() {
	++scopeSpaceCounter;
}


/**
 *
 * @brief Decrements the scopeSpaceCounter
 */
void exitscopespace() {
	assert(scopeSpaceCounter > 1);
	--scopeSpaceCounter;
}


/**
 *
 * @brief Resets the formal arg offset to 0.
 */
void resetformalargoffset() {
	formalArgOffset = 0;
}



/**
 *
 * @brief Resets the function locals offset to 0.
 */
void resetfunctionlocalsoffset() {
	functionLocalOffset = 0;
}



/**
 *
 * @brief Restores the current scope offset to n.
 */
void restorecurrscopeoffset(unsigned n) {
	switch (currscopespace()) {
		case program_variable:
			programVarOffset = n;
			break;
		case function_local:
			functionLocalOffset = n;
			break;
		case formal_arg:
			formalArgOffset = n;
			break;
		default:
			assert(0);
	}
}



/**
 *
 * @brief Returns the label of the next quad
 */
unsigned int nextquadlabel() {
	return current_quad;
}



/**
 * @param		quadNo		The quad number
 * @param		label		The label which should be assigned to the quad
 * @brief					Patches the quad with quadNo with the label.
 */
void patchlabel(unsigned quadNo, unsigned label) {
	assert(quadNo < current_quad);
	quad_table[quadNo].label = label;
}



/**
 * @brief 					Prints the entire quad_table.
 */
void print_quad_table() {
	int i = 0;
	unsigned int quad_count = 0;
	quad * quad_entry = quad_table;
	printf("---- QUAD TABLE ----\n");
	printf("%-3s %-25s%-12s%-12s%-12s%-12s\n", "#", "opcode", "arg1", "arg2", "result", "label");
	while (i < current_quad) {
		printf("%-2d%-1c ", quad_count,':');
		switch (quad_entry->op) {
			case assign:
				printf("%-25s", "ASSIGN");
				break;
			case add:
				printf("%-25s", "ADD");
				break;
			case sub:
				printf("%-25s", "SUB");
				break;
			case mul:
				printf("%-25s", "MUL");
				break;
			case divi:
				printf("%-25s", "DIV");
				break;
			case mod:
				printf("%-25s", "MOD");
				break;
			case uminus:
				printf("%-25s", "UMINUS");
				break;
			case and:
				printf("%-25s", "AND");
				break;
			case or:
				printf("%-25s", "OR");
				break;
			case not:
				printf("%-25s", "NOT");
				break;
			case if_eq:
				printf("%-25s", "IF_EQ");
				break;
			case if_noteq:
				printf("%-25s", "IF_NOTEQ");
				break;
			case if_lesseq:
				printf("%-25s", "IF_LESSEQ");
				break;
			case if_greatereq:
				printf("%-25s", "IF_GREATEREQ");
				break;
			case if_less:
				printf("%-25s", "IF_LESS");
				break;
			case if_greater:
				printf("%-25s", "IF_GREATER");
				break;
			case jump:
				printf("%-25s", "JUMP");
				break;
			case call:
				printf("%-25s", "CALL");
				break;
			case param:
				printf("%-25s", "PARAM");
				break;
			case retur:
				printf("%-25s", "RETURN");
				break;
			case getretval:
				printf("%-25s", "GETRETVAL");
				break;
			case funcstart:
				printf("%-25s", "FUNCSTART");
				break;
			case funcend:
				printf("%-25s", "FUNCEND");
				break;
			case tablecreate:
				printf("%-25s", "TABLECREATE");
				break;
			case tablegetelem:
				printf("%-25s", "TABLEGETELEM");
				break;
			case tablesetelem:
				printf("%-25s", "TABLESETELEM");
				break;
		}

		if (quad_entry->arg1 != NULL) {
			switch (quad_entry->arg1->type) {
				case var_e:
				case tableitem_e:
				case programfunc_e:
				case libraryfunc_e:
				case arithexpr_e:
				case assignexpr_e:
				case newtable_e:
					printf("%-12s", quad_entry->arg1->sym->name);
					break;
				case boolexpr_e:
					if (quad_entry->arg1->sym->name != NULL) {
						printf("%-12s", quad_entry->arg1->sym->name);
					}
					else {
						if (quad_entry->arg1->boolConst == 1) {
							printf("%-12s", "TRUE");
						}
						else {
							printf("%-12s", "FALSE");
						}
					}
					break;
				
				case constnum_e:
					printf("%-12.2f", quad_entry->arg1->numConst);
					break;
				case constbool_e:
					if (quad_entry->arg1->sym != NULL) {
						printf("%-12s", quad_entry->arg1->sym->name);
					}
					else {
						if (quad_entry->arg1->boolConst == 1) {
							printf("%-12s", "TRUE");
						}
						else {
							printf("%-12s", "FALSE");
						}
					}
					break;
				case conststring_e:
					printf("%-12s", quad_entry->arg1->strConst);
					break;
				case nil_e:
					printf("%-12s", "NIL");
					break;
			}
		}
		else {
			printf("%-12s"," ");
		}

		if (quad_entry->arg2 != NULL) {
			switch (quad_entry->arg2->type) {
				case var_e:
				case tableitem_e:
				case programfunc_e:
				case libraryfunc_e:
				case arithexpr_e:
				case assignexpr_e:
				case newtable_e:
					printf("%-12s", quad_entry->arg2->sym->name);
					break;
				case boolexpr_e:
					if (quad_entry->arg2->boolConst == 1) {
						printf("%-12s", "TRUE");
					}
					else {
						printf("%-12s", "FALSE");
					}
					break;
				case constnum_e:
					printf("%-12.2f", quad_entry->arg2->numConst);
					break;
				case constbool_e:
					if (quad_entry->arg2->boolConst == 1) {
						printf("%-12s", "TRUE");
					}
					else {
						printf("%-12s", "FALSE");
					}
					break;
				case conststring_e:
					printf("%-12s", quad_entry->arg2->strConst);
					break;
				case nil_e:
					printf("%-12s", "NIL");
					break;
			}
		}
		else {
			printf("%-12s"," ");
		}

		if (quad_entry->result != NULL){
			switch (quad_entry->result->type){
				case var_e:
				case programfunc_e:
				case libraryfunc_e:
				case arithexpr_e:
				case boolexpr_e:
				case assignexpr_e:
				case newtable_e:
				case tableitem_e:
					printf("%-12s", quad_entry->result->sym->name);
					break;
				case constnum_e:
					printf("%-12.2f", quad_entry->result->numConst);
					break;
				case constbool_e:
					if (quad_entry->arg2->boolConst == 1) {
						printf("%-12s", "TRUE");
					}
					else {
						printf("%-12s", "FALSE");
					}
					break;
				case conststring_e:
					printf("%-12s", quad_entry->result->strConst);
					break;
				case nil_e:
					printf("%-12s", "NIL");
					break;
			}
		}
		else {
			printf("%-12s"," ");
		}

		printf("%-12d", quad_entry->label);
		quad_entry = quad_entry + 1;
		i++;
		quad_count++;
		printf("\n");
	}
}

/** 
 * @param		s		The string to be checked.
 * @brief				Returns TRUE if the input string is a temporary variable.
 */
unsigned int istempname(char* s) {
	{return *s == '_';}
}


/**
 * @param		e		The expression to be checked.
 * @brief				Returns TRUE if the input expr is a temporary expr.
 */
unsigned int istempexpr(expr* e) {
	return e->sym && 
			(e->sym->type == GLOBAL ||
			e->sym->type == LOCAL_VAR ||
			e->sym->type == FORMAL) &&
			istempname(e->sym->name);
}




/**
 * @brief			Functions that store and print, every semantic error found while parsing
 */
void SemanticError_insert(char* var, iopcode op, int lineNum) {
	semanticerror* newerror = (semanticerror*)malloc(sizeof(semanticerror));
	newerror->op = op;
	newerror->var = var;
	newerror->lineNum = lineNum;
	newerror->next = NULL;
	semanticerror* current = semanticerrorhead;
	if (semanticerrorhead == NULL) {
		semanticerrorhead = newerror;
		return;
	}
	while (current->next != NULL) {
		current = current->next;
	}
	current->next = newerror;
	return;
}


/**
 * @brief			Prints all the Semantic errors, found during the analysis.
 */
void SemanticError_print() {
	if (semanticerrorhead == NULL) {
		return;
	}
	else
	{
		semanticerror* current = semanticerrorhead;
		while (current != NULL ) {
			printf("Semantic error in line %d, applying %s operation on %s is invalid.\n", current->lineNum, OpcodeArray[current->op], current->var);
			current = current->next;
		}
	}
}





