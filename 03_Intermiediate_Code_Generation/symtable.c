/*******************************************************************
 * @file   symtable.c                                              *
 * @author Steve Cangonj <cangonj@csd.uoc.gr>, 2871                *
 * @author Ioannis-Pavlos Panteliadis <panteliad@csd.uoc.gr>, 2794 *
 * @author Kiriakos Paraskevas <paraskevas@csd.uoc.gr>, 2832       *
 *                                                                 *
 * @brief  Implementation file of the 'symtable.h' header file     *
 *******************************************************************/

#include "symtable.h"

int namelesscount = 0;

/**
 * @brief 			Hash function for the symbol table.
 */
static unsigned int SymbolTable_hash(const char * pcKey)
{
	size_t ui;
	unsigned int uiHash = 0U;
	for (ui = 0U; pcKey[ui] != '\0'; ui++)
		uiHash = uiHash * HASH_MULTIPLIER + pcKey[ui];
	uiHash = uiHash % 509 - 1;
	return uiHash;
}

/**
 * @brief			Incerements the max scope we are currently in.
 */
void raisemaxScope()
{
	if (scope > max_scope)
	{
		max_scope ++;
	}
}

/**
 * @brief			Inserts a new symbol in the Hash table.
 */
int SymbolTable_insert(SymbolTableEntry* sym)
{
	int i;
	unsigned int hashnum = SymbolTable_hash(sym->name);
	SymbolTableEntry * tmpcagehead;

	/* New Entry in SymbolTable */
	if (MySymbolTable->hashtable[hashnum].cageshead == NULL) {
		MySymbolTable->hashtable[hashnum].cageshead = sym;
		sym->next = NULL;
		MySymbolTable->bindings++;
	}
	else {
		tmpcagehead = MySymbolTable->hashtable[hashnum].cageshead;
		sym->next =  tmpcagehead;
		MySymbolTable->hashtable[hashnum].cageshead = sym;
		MySymbolTable->bindings++;
	}
	
	/* New Entry in ScopeList */
	while (max_scope > scope_length) {
		MyScopeList[scope_length+1]=*(SymbolTableEntry**)malloc(10 * sizeof(SymbolTableEntry*));

		for (i=scope_length+1 ;i<(scope_length+11);i++) {
			MyScopeList[i] = NULL;
		}
		scope_length = scope_length + 10;

	}
	if (MyScopeList[scope] == NULL && max_scope == 0) {
		MyScopeList[scope] = sym;
		return 1;
	}
	else {
		sym->scope_next = MyScopeList[scope];
		MyScopeList[scope] = sym;
		return 1;
	}
	return 0;
}

/**
 * @brief 			
 */
void SymbolTable_hide_scope(int scope)
{
	SymbolTableEntry * head;
	if(scope_length < max_scope)
		return;
	head = MyScopeList[scope];
	if (head == NULL) {
		return;
	}
	while (head != NULL)
	{
		head->isActive = 0;
		head = head->scope_next;
	}
	return;
}


SymbolTableEntry* Symbol_create(SymbolType type, const char * name , unsigned int line, unsigned int scope, 
								int isactive) {
	/* Initialize some stuff */
	assert(MySymbolTable != NULL);
	assert(name != NULL);
	SymbolTableEntry * newentry;

	/* If the error-checking clears, proceed... */
	/* Make the new symbol */
	newentry = (SymbolTableEntry *)malloc(sizeof(SymbolTableEntry));
	newentry->type = type;
	newentry->name = strdup(name);
	newentry->line = line;
	newentry->scope = scope;
	newentry->isActive = isactive;
	newentry->next = NULL;
	newentry->scope_next = NULL;

	return newentry;
}

int Symbol_activate(SymbolTableEntry *sym) {
	sym->isActive = 1;
	return 1;
}

int Symbol_deactivate(SymbolTableEntry *sym) {
	sym->isActive = 0;
	return 1;
}


void SymbolTable_print() {
	SymbolTableEntry *  tmphead;
	int i = 0;
	printf("Symbol table elements: ");
	for (i = 0; i <= max_scope; i++) {
		tmphead = MyScopeList[i];
		if (tmphead == NULL) {
			continue;
		}
		printf("\nIn scope[%d]:\n\t|%-25s|%-12s|%-12s|%-12s|%-12s|%-12s", i, "ID", "Type", "Line", "isActive", "Offset", "SSpace");
		while (tmphead != NULL) {
			printf("\n\t|%-25s|%-12s|%-12d|%-12d|%-12d|%-12s\t", tmphead->name, SymbolTableTypeArray[tmphead->type], tmphead->line, tmphead->isActive, tmphead->offset, ScopeSpaceArray[tmphead->scopespace]);
			tmphead = tmphead->scope_next;
		}
	}
	return;
	
}

int Scope_isActive(unsigned int scope, const char * name)
{
	SymbolTableEntry * head  = MyScopeList[scope];
	if (scope_length < max_scope)
		return 0;
	while (head != NULL)
	{
		if (strcmp(head->name,name) == 0 && head->isActive == 1)
		{
			return 1;
		}
		head = head->scope_next;
	}
	return 0;
}



SymbolTableEntry* Symbol_lookup(unsigned int scope, const char * name)
{
	SymbolTableEntry * iter = MyScopeList[scope];
	if (scope_length < max_scope)
		return NULL;
	while (iter != NULL)
	{
		if (strcmp(iter->name, name) == 0)
		{
			return iter;
		}
		iter = iter->scope_next;
	}
	return NULL;
}


/**
 * @brief			Functions that store and print, every Syntax error found while parsing
 */
void SyntaxError_insert(char * variable, char * errorMessage, int lineNum)
{
	SyntaxError* newerror = (SyntaxError*)malloc(sizeof(SyntaxError));
	newerror->errorMessage = strdup(errorMessage);
	newerror->Var = strdup(variable);
	newerror->lineNum = lineNum;
	newerror->next = NULL;
	SyntaxError * current = syntaxerrorhead;
	if (syntaxerrorhead == NULL)
	{
		syntaxerrorhead = newerror;
		return;
	}
	while (current->next != NULL)
	{
		current = current->next;
	}
	current->next = newerror;
	return;
}


/**
 * @brief			Prints all the Syntax errors, found during the analysis.
 */
void SyntaxError_print() {
	if (syntaxerrorhead == NULL) {
		return;
	}
	else
	{
		SyntaxError * current = syntaxerrorhead;
		while (current != NULL )
		{
			printf("Syntax Error in line %d, '%s' %s\n", current->lineNum, current->Var, current->errorMessage);
			current = current->next;
		}
	}
}

/*END Syntax Errors.*/



/**
 * @return				Returns a valid name for a nameless function.
 * @brief				Function that creates a nameless function e.x._f2. It uses a global variable that
 *						increments with every new creation.
 */
char* createNamelessFunc() {
	char * namelessfunc = malloc(STR_SIZE);
	sprintf(namelessfunc, "_f%d" , namelesscount++);
	return namelessfunc;
}


/**
 * @param	scope 		The scope we are currently in.
 * @brief 				Returns 1 is there is an active function in the same OR lower scope than we currently are.
 */
int isActiveFunction(int scope) {
	SymbolTableEntry* curr;
	curr = MyScopeList[scope];
	while (scope >= 0){
		while (curr != NULL) {
			if (curr->isActive == 1 && (curr->type == USERFUNC || curr->type == LIBFUNC)) {
				return 1;
			}
			curr = curr->next;
		}
		scope--;
	}
	return 0;
}


/**
 * @param	scope 		The scope we are currently in.
 * @brief 				Returns 1 is there is an active function in the SAME scope than we currently in.
 */
int isActiveFunctionScope(int scope) {
	SymbolTableEntry* curr;
	curr = MyScopeList[scope];
	while (curr != NULL) {
		if (curr->isActive == 1 && (curr->type == USERFUNC || curr->type == LIBFUNC)) {
			return 1;
		}
		curr = curr->next;
	}
	return 0;
}


/**
 * @param	scope 		The scope we are currently in.
 * @brief 				Returns 1 is there is a predefined function in the SAME scope than we currently in.
 */
int isPredefinedFunction(int scope, const char* name){
	SymbolTableEntry* curr;
	curr = MyScopeList[scope];
	while (curr != NULL) {
		if (curr->type == USERFUNC && strcmp(curr->name, name)==0) {
			return 1;
		}
		curr=curr->next;
	}
	return 0;
}


/**
 * @param	name 		The name we are checking.
 * @brief				Return's 1 if 'name' is a library function.
 */
int isLibraryFunction(char* name)
{
	if(strcmp(name,"print") == 0)
		return 1;
	if(strcmp(name,"input") == 0)
		return 1;
	if(strcmp(name,"objectmemberkeys") == 0)
		return 1;
	if(strcmp(name,"objecttotalmembers") == 0)
		return 1;
	if(strcmp(name,"objectcopy") == 0)
		return 1;
	if(strcmp(name,"totalarguments") == 0)
		return 1;
	if(strcmp(name,"typeof") == 0)
		return 1;
	if(strcmp(name,"strtonum") == 0)
		return 1;
	if(strcmp(name,"sqrt") == 0)
		return 1;
	if(strcmp(name,"cos") == 0)
		return 1;
	if(strcmp(name,"sin") == 0)
		return 1;
	if(strcmp(name,"argument") == 0)
		return 1;
	return 0;
}



/**
 * @brief				Initializes the system.
 */
void initialize_system() {
	int i;
	MySymbolTable = (SymbolTable *)malloc(sizeof(SymbolTable));
	MySymbolTable->bindings = 0;
	MyScopeList = (SymbolTableEntry **)malloc(100 * sizeof(SymbolTableEntry *));
	scope_length = 100;
	
	for (i=0; i<100; i++) {
		MyScopeList[i] = NULL;
	}

	/* Create new symbols. */
	SymbolTableEntry* _print = Symbol_create(LIBFUNC, "print", 0, 0, 1);
	SymbolTableEntry* _input = Symbol_create(LIBFUNC, "input", 0, 0, 1);
	SymbolTableEntry* _objectmemberkeys = Symbol_create(LIBFUNC, "objectmemberkeys", 0, 0, 1);
	SymbolTableEntry* _objecttotalmembers = Symbol_create(LIBFUNC, "objecttotalmembers", 0, 0, 1);
	SymbolTableEntry* _objectcopy = Symbol_create(LIBFUNC, "objectcopy", 0, 0, 1);
	SymbolTableEntry* _totalarguments = Symbol_create(LIBFUNC, "totalarguments", 0, 0, 1);
	SymbolTableEntry* _argument = Symbol_create(LIBFUNC, "argument", 0, 0, 1);
	SymbolTableEntry* _typeof = Symbol_create(LIBFUNC, "typeof", 0, 0, 1);
	SymbolTableEntry* _strtonum = Symbol_create(LIBFUNC, "strtonum", 0, 0, 1);
	SymbolTableEntry* _sqrt = Symbol_create(LIBFUNC, "sqrt", 0, 0, 1);
	SymbolTableEntry* _cos = Symbol_create(LIBFUNC, "cos", 0, 0, 1);
	SymbolTableEntry* _sin = Symbol_create(LIBFUNC, "sin", 0, 0, 1);


	/* Insert them in the Hash. */
	SymbolTable_insert(_print);
	SymbolTable_insert(_input);
	SymbolTable_insert(_objectmemberkeys);
	SymbolTable_insert(_objecttotalmembers);
	SymbolTable_insert(_objectcopy);
	SymbolTable_insert(_totalarguments);
	SymbolTable_insert(_argument);
	SymbolTable_insert(_typeof);
	SymbolTable_insert(_strtonum);
	SymbolTable_insert(_sqrt);
	SymbolTable_insert(_cos);
	SymbolTable_insert(_sin);
}
