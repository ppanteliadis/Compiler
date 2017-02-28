%{
	#include "symtable.h"

	int yylex(void);
	int yyerror(char const * yaccProvidedMessage);

	char * SymbolTableTypeArray[] = {"GLOBAL", "LOCAL", "FORMAL",
		"USERFUNC", "LIBFUNC", "NILL"};
	int loop = 0;
	int _return = 0;

	int max_scope = 0;

	extern int yylineno;
	extern char * yytext;
	extern FILE * yyin;
	extern FILE * yyout;

	SyntaxError * syntaxerrorhead;
	int scope = 0;

%}

%expect 1

%start program


%union{
	int intVal;
	char * strVal;
	float realVal;
	SymbolTableEntry * exprNode;
}

%token <intVal> INTEGER;
%token <strVal> STRING;
%token <strVal> IDENTIFIER;
%token <realVal> REAL;
%token <strVal> UNDEFINED;

%token IF ELSE WHILE FOR FUNCTION RETURN BREAK CONTINUE AND NOT OR LOCAL TRUE FALSE NIL ASSIGNMENT PLUS MINUS MUL DIV MOD EQUAL NOT_EQUAL PLUS_PLUS MINUS_MINUS GREATER LESS GREATER_EQUAL LESS_EQUAL LEFT_BRACE RIGHT_BRACE LEFT_BRACKET RIGHT_BRACKET LEFT_PARENTHESIS RIGHT_PARENTHESIS SEMICOLON COMMA COLON DOUBLE_COLON DOT DOUBLE_DOT SPACE

%type <strVal> stmt stmtstar
%type <intVal> expr
%type <strVal> ifstmt
%type <strVal> whilestmt
%type <strVal> forstmt
%type <strVal> returnstmt
%type <strVal> block
%type <strVal> funcdef
%type <strVal> assignexpr
%type <strVal> term
%type <exprNode> lvalue
%type <strVal> primary
%type <strVal> call
%type <strVal> objectdef
%type <strVal> const
%type <strVal> member
%type <strVal> elist eliststar
%type <strVal> callsuffix
%type <strVal> normcall
%type <strVal> methodcall
%type <strVal> indexed indexedstar
%type <strVal> indexedelem
%type <strVal> idlist idliststar



%nonassoc GREATER GREATER_EQUAL LESS LESS_EQUAL EQUAL NOT_EQUAL
%right NOT PLUS_PLUS MINUS_MINUS MINUS ASSIGNMENT
%left LEFT_PARENTHESIS RIGHT_PARENTHESIS LEFT_BRACKET RIGHT_BRACKET DOT DOUBLE_DOT MUL DIV MOD PLUS AND OR
%error-verbose


%%
program: stmt stmtstar
			{
				fprintf(yyout, "program => stmt stmtstar line:%d\n", yylineno);
			}
			;

stmtstar: stmt stmtstar
			{
				fprintf(yyout, "stmtstar => stmt stmtstar line:%d\n", yylineno);
			}
		|
			{
				fprintf(yyout, "stmtstar => empty line:%d\n", yylineno);
			}

stmt: expr SEMICOLON		{fprintf(yyout,"stmt => expr; line:%d\n", yylineno);}
		| ifstmt			{fprintf(yyout,"stmt => IF statement\n");}
		| whilestmt			{fprintf(yyout,"stmt => WHILE statement\n");}
		| forstmt			{fprintf(yyout,"stmt => FOR statement\n");}
		| returnstmt
			{
				fprintf(yyout,"stmt => RETURN statement\n");
				if (scope == 0) {
					SyntaxError_insert(strdup("return"), strdup("cannot be used while not in a function."), yylineno);
				}
				else if (isActiveFunction(scope) == 0 && _return == 1) {
					SyntaxError_insert(strdup("return"), strdup("cannot be used while not in a function."), yylineno);
				}
			}
		| BREAK SEMICOLON
			{
				fprintf(yyout,"stmt => BREAK;\n");
				if(loop == 0)
				{
					SyntaxError_insert(strdup("break"), strdup("cannot be used while not inside a loop."), yylineno);
				}
			}
		| CONTINUE SEMICOLON
			{
				fprintf(yyout,"stmt => CONTINUE\n");
				if(loop == 0)
				{
					SyntaxError_insert(strdup("continue"), strdup("cannot be used while not inside a loop."), yylineno);
				}
			}
		| block				{fprintf(yyout,"stmt => BLOCK statement\n");}
		| funcdef			{fprintf(yyout,"stmt => FUNCTION DEFINITION\n");}
		| SEMICOLON			{fprintf(yyout,"stmt => \";\"\n");}
		;

expr: assignexpr			{fprintf(yyout,"expr => assignexpr\n");}
		| expr PLUS expr
			{
				fprintf(yyout,"expr => expression  + expression \t%d+%d\n", $1, $3);
				$$ = $1 + $3;
			}

		| expr MINUS expr
			{
				fprintf(yyout,"expr => expr - expr \t%d-%d\n", $1, $3);
				$$ = $1 - $3;
			}

		| expr MUL expr
			{
				fprintf(yyout,"expr => expr * expr \t%d*%d\n", $1, $3);
				$$ = $1 * $3;
			}
		| expr DIV expr
			{
				fprintf(yyout,"expr => expr / expr \t%d/%d\n", $1, $3);
				$$ = $1 / $3;
			}
		| expr MOD expr
			{
				fprintf(yyout,"expr => expr /% expr \t%d%%d\n", $1, $3);
				$$ = $1 % $3;
			}
		| expr GREATER expr
			{
				fprintf(yyout,"expr => expr > expr \t%d>%d\n", $1, $3);
				$$ = ($1 > $3)?1:0;
			}
		| expr GREATER_EQUAL expr
			{
				fprintf(yyout,"expr => expr >= expr \t%d>=%d\n", $1, $3);
				$$ = ($1>=$3)?1:0;
			}
		| expr LESS expr
			{
				fprintf(yyout,"expr => expr < expr \t%d<%d\n", $1, $3);
				$$ = ($1<$3)?1:0;
			}
		| expr LESS_EQUAL expr
			{
				fprintf(yyout,"expr => expr <= expr \t%d<=%d\n", $1, $3);
				$$ = ($1<=$3)?1:0;
			}
		| expr EQUAL expr
			{
				fprintf(yyout,"expr => expr == expr \t%d==%d\n", $1, $3);
				$$ = ($1==$3)?1:0;
			}
		| expr NOT_EQUAL expr
			{
				fprintf(yyout,"expr => expr != expr \t%d!=%d\n", $1, $3);
				$$ = ($1!=$3)?1:0;
			}
		| expr AND expr
			{
				fprintf(yyout,"expr => expr && expr \t%d&&%d\n", $1, $3);
				$$ = ($1&&$3)?1:0;
			}
		| expr OR expr
			{
				fprintf(yyout,"expr => expr || expr \t%d/%d\n", $1, $3);
				$$ = ($1||$3)?1:0;
			}
		| term
			{
				fprintf(yyout,"expr => term\n");
			}
		;

assignexpr: lvalue ASSIGNMENT expr
			{
				fprintf(yyout,"assignexpr => lvalue = expr\n");
				if ($1 != NULL) {
					printf("$1->type = %d, $1->name = %s\n", $1->type, $1->name);
					if ($1->type == LIBFUNC) {
						SyntaxError_insert(strdup($1->name), strdup("is a library function and cannot be used as an lvalue."), $1->line);
					}
					else if ($1->type == USERFUNC) {
						/*GAMW TH PANAGIA, TO XRISTO KAI TOUS 12*/
						SyntaxError_insert(strdup($1->name), strdup("is a predefined user function and cannot be used as an lvalue."), $1->line);
					}
				}
				
			}
		;

term: LEFT_PARENTHESIS expr RIGHT_PARENTHESIS {fprintf(yyout, "term => (expr)\n");}
		| MINUS expr
			{
				fprintf(yyout,"term => -expr\n");
			}
		| NOT expr
			{
				fprintf(yyout,"term => not expr\n");
			}
		| PLUS_PLUS lvalue
			{
				fprintf(yyout,"term => ++lvalue\n");
				if ($2->type == LIBFUNC) {
					SyntaxError_insert(strdup($2->name), strdup("is a library function and cannot be used as an ++lvalue."), yylineno);
				}
				else if ($2->type == USERFUNC) {
					SyntaxError_insert(strdup($2->name), strdup("is a predefined user function and cannot be used as an ++lvalue."), yylineno);
				}
			}
		| lvalue PLUS_PLUS
			{
				fprintf(yyout,"term => lvalue++\n");
				if ($1->type == LIBFUNC) {
					SyntaxError_insert(strdup($1->name), strdup("is a library function and cannot be used as an lvalue++."), yylineno);
				}
				else if ($1->type == USERFUNC) {
					SyntaxError_insert(strdup($1->name), strdup("is a predefined user function and cannot be used as an lvalue++."), yylineno);
				}
			}
		| MINUS_MINUS lvalue
			{
				fprintf(yyout,"term => --lvalue\n");
				if ($2->type == LIBFUNC) {
					SyntaxError_insert(strdup($2->name), strdup("is a library function and cannot be used as an --lvalue."), yylineno);
				}
				else if ($2->type == USERFUNC) {
					SyntaxError_insert(strdup($2->name), strdup("is a predefined user function and cannot be used as an --lvalue."), yylineno);
				}
			}
		| lvalue MINUS_MINUS
			{
				fprintf(yyout,"term => lvalue--\n");
				if ($1->type == LIBFUNC) {
					SyntaxError_insert(strdup($1->name), strdup("is a library function and cannot be used as an lvalue--."), yylineno);
				}
				else if ($1->type == USERFUNC) {
					SyntaxError_insert(strdup($1->name), strdup("is a predefined user function and cannot be used as an lvalue--."), yylineno);
				}
			}
		| primary
			{
				fprintf(yyout,"term => primary\n");
			}
		;

primary: lvalue
			{
				fprintf(yyout,"primary => lvalue\n");
			}
		| call
			{
				fprintf(yyout,"primary => call\n");
			}
		| objectdef
			{
				fprintf(yyout,"primary => objectdef\n");
			}
		| LEFT_PARENTHESIS funcdef RIGHT_PARENTHESIS
			{
				fprintf(yyout,"primary => (funcdef)\n");
			}
		| const
			{
				fprintf(yyout,"primary => const %s\n", yytext);
			}
		;

lvalue: IDENTIFIER
			{
				fprintf(yyout,"lvalue => id %s\n", yytext);
				int tmpscope = scope;
				SymbolTableEntry* sym = NULL;
				/* Check all active scopes for a symbol of the same name. */
				while (tmpscope >= 0 && sym == NULL) {
					sym = Symbol_lookup(tmpscope, yytext);
					tmpscope--;
				}

				/* NOW we know!! we have to check the cases... */
				if (sym == NULL) {
					if (scope == 0) {
						sym = Symbol_create(GLOBAL, strdup(yytext), yylineno, scope, 1);
						SymbolTable_insert(sym);
						$$=sym;
						printf("$$->type = %d, $$->name = %s\n", sym->type, sym->name);
					}
					else {
						sym = Symbol_create(LOCAL_VAR, strdup(yytext), yylineno, tmpscope, 1);
						SymbolTable_insert(sym);
						$$=sym;
					}
				}
				else {
					/* FOUND SOMETHING!! */
					/* Need to see if we can access it! */
					if (sym->type == GLOBAL) {
						/* Found a global var, referencing to it! */
					}
					else if (isActiveFunction(scope-1) && scope > 0) {
						/* See if a function interferes... */
						if (sym->type == FORMAL && sym->scope == scope) {
							/* Valid FORMAL variable in the current scope. Referrencing to it.*/
						}
						else if (sym->type == USERFUNC) {
							/* Valid userfunc, referencing to it. */
						}
						else {
							if (isLibraryFunction(yytext)) {
								/*No issue here.*/
							}
							else {
								SyntaxError_insert(strdup(yytext), strdup("cannot be accessed due to an active function being interposed."), yylineno);
							}
						}
					}
					
					/* We have a function. Use reduction so we can test if it's ok on the FUNCTION rule.*/
				}
				$$ = sym;
				
			}
			| LOCAL IDENTIFIER
			{
				fprintf(yyout, "lvalue => local id\n");
				SymbolTableEntry* sym = Symbol_lookup(scope, yytext);
				if (sym != NULL) {
					/* Found variable or function in the current scope, referencing to it. */
					$$=sym;
				}
				else {
					/* Found nothing, create a new symbol if it doesnt collide with libfunc. */
					if (isLibraryFunction(yytext) == 1) {
						SyntaxError_insert(strdup(yytext), strdup("is a local variable colliding with a library function."), yylineno);
						$$=NULL;
					}
					else {
						if (scope == 0) {
							sym = Symbol_create(GLOBAL, strdup(yytext), yylineno, 0, 1);
						}
						else {
							sym = Symbol_create(LOCAL_VAR, strdup(yytext), yylineno, scope, 1);
						}
						SymbolTable_insert(sym);
						$$=sym;
					}
					
				}
			}
		| DOUBLE_COLON IDENTIFIER
			{
				fprintf(yyout,"lvalue => ::id\n");
				/* Searching in global scope. */
				SymbolTableEntry* sym = Symbol_lookup(0, yytext);
				if (sym != NULL) {
					if (sym->type == GLOBAL) {
						/* Referencing to a GLOBAL variable. */
						printf("HELLO\n");
					}
					else if (sym->type == USERFUNC) {
						/* Referrencing to a USERFUNC. */
					}
					else if (sym->type == LIBFUNC) {
						/* Referrencing to a LIBFUNC. */
					}
					$$=sym;
				}
				else {
					$$=NULL;
					SyntaxError_insert(strdup(yytext), strdup("is referencing to an undefined global variable."), yylineno);
				}
			}
		| member
			{
				fprintf(yyout,"lvalue => member\n");
			}
		;

call: call LEFT_PARENTHESIS elist RIGHT_PARENTHESIS
		{
			fprintf(yyout,"call => call(elist)\n");
		}
	| lvalue callsuffix
		{
			fprintf(yyout,"call => lvalue callsuffix\n");
		}
	| LEFT_PARENTHESIS funcdef RIGHT_PARENTHESIS LEFT_PARENTHESIS elist RIGHT_PARENTHESIS
		{
			fprintf(yyout,"call => (funcdef)(elist)\n");
		}
	;

member: lvalue DOT IDENTIFIER
			{
				fprintf(yyout,"member => lvalue.id\n");
			}
		| lvalue LEFT_BRACKET expr RIGHT_BRACKET
			{
				fprintf(yyout,"member => lvalue[expr]\n");
			}
		| call DOT IDENTIFIER
			{
				fprintf(yyout,"member => call.id\n");
			}
		| call LEFT_BRACKET expr RIGHT_BRACKET
			{
				fprintf(yyout,"member => call[expr]\n");
			}
		;

callsuffix: normcall
			{
				fprintf(yyout,"callsuffix => normcall\n");
			}
		| methodcall
			{
				fprintf(yyout,"callsuffix =>methodcall\n");
			}
		;


normcall: LEFT_PARENTHESIS elist RIGHT_PARENTHESIS
			{
				fprintf(yyout,"normcall => (elist)\n");
			}
		;


methodcall: DOUBLE_DOT IDENTIFIER LEFT_PARENTHESIS elist RIGHT_PARENTHESIS
			{
				fprintf(yyout,"methodcall => ..id(elist)\n");
			}
		;

elist: expr eliststar
			{
				fprintf(yyout,"elist => expr elist*\n");
			}
		|	{
				fprintf(yyout,"elist => void\n");
			}
		;

eliststar: COMMA expr eliststar
			{
				fprintf(yyout,"eliststar => , expr eliststar\n");
			}
		|	{
				fprintf(yyout,"eliststar => void\n");
			}
		;

objectdef: LEFT_BRACKET elist RIGHT_BRACKET
			{
				fprintf(yyout,"objectdef => [elist]\n");
			}
		| LEFT_BRACKET indexed RIGHT_BRACKET
			{
				fprintf(yyout,"objectdef => [indexed]\n");
			}
		;

indexed: indexedelem indexedstar
			{
				fprintf(yyout,"indexed => indexedelem\n");
			}
indexedstar: COMMA indexedelem indexedstar
			{
				fprintf(yyout,"indexedstar => , indexedelem indexedstar\n");
			}
		|	{
				{fprintf(yyout, "indexedstar => void\n");}
			}
		;

indexedelem: LEFT_BRACE{} expr COLON expr RIGHT_BRACE
			{
				fprintf(yyout,"indexedelem => {expr:expr}\n");

			}
		;

block: LEFT_BRACE {scope++;raisemaxScope();} stmt stmtstar RIGHT_BRACE{SymbolTable_hide_scope(scope);scope--;}
			{
				fprintf(yyout,"block => { stmt stmtstar }\n");
			}
		| LEFT_BRACE{scope++;raisemaxScope();} RIGHT_BRACE{SymbolTable_hide_scope(scope);scope--;}
			{
				fprintf(yyout,"block => {}\n");
			}
		;


funcdef: FUNCTION IDENTIFIER
			{
				SymbolTableEntry* sym = Symbol_lookup(scope, yytext);
				if (sym == NULL) {
					/* Nothing like this exists in this scope. */
					if (isLibraryFunction(yytext)==1) {
						SyntaxError_insert(strdup(yytext), strdup("is a library function and cannot be overshadowed."), yylineno);
					}
					else {
						sym = Symbol_create(USERFUNC, strdup(yytext), yylineno, scope, 1);
						SymbolTable_insert(sym);
					}
				}
				else {
					/* WE GOT SOMETHING! LET'S CHECK IT! */
					if (sym->type == USERFUNC) {
						SyntaxError_insert(strdup(yytext), strdup("is a previously defined user function and cannot be redefined."), yylineno);
					}
					else if (sym->type == LOCAL_VAR) {
						SyntaxError_insert(strdup(yytext), strdup("is a previously defined local variable and cannot be redefined as a function."), yylineno);
					}
					else if (sym->type == GLOBAL) {
						SyntaxError_insert(strdup(yytext), strdup("is a previously defined global variable and cannot be redefined as a function."), yylineno);
					}
					else if (sym->type == FORMAL) {
						SyntaxError_insert(strdup(yytext), strdup("is a previously defined formal variable and cannot be redefined as a function."), yylineno);
					}
					else if (isLibraryFunction(sym->name)==1) {
						SyntaxError_insert(strdup(yytext), strdup("is a library function and cannot be overshadowed."), yylineno);
					}
				}
			}
		 LEFT_PARENTHESIS
			{
				scope++;
			}
		 idlist
		 RIGHT_PARENTHESIS
			{
				scope--;
			}
		 block
			{
				fprintf(yyout,"funcdef => function id (idlist) {}\n");
			}
		| FUNCTION
			{
				SymbolTableEntry* sym = Symbol_create(USERFUNC, strdup(createNamelessFunc()), yylineno, scope, 1);
				SymbolTable_insert(sym);
			} LEFT_PARENTHESIS{scope++;} idlist 
			RIGHT_PARENTHESIS
			{
				SymbolTable_hide_scope(scope);
				scope--;
			}
			block
			{
				fprintf(yyout,"funcdef => function (idlist) {}\n");
			}
		;

const: INTEGER
			{
				fprintf(yyout,"const => INTEGER %d\n", atoi(yytext));
			}
		| REAL
			{
				fprintf(yyout,"const => REAL %f\n", atof(yytext));
			}
		| STRING
			{
				fprintf(yyout,"const => STRING %s\n", yytext);
			}
		| NIL
			{
				fprintf(yyout,"const => NIL\n");
			}
		| TRUE
			{
				fprintf(yyout,"const => TRUE\n");
			}
		| FALSE
			{
				fprintf(yyout,"const => FALSE\n");
			}
		;


idlist: IDENTIFIER
			{
				fprintf(yyout, "idlist => id\n");
				SymbolTableEntry* sym = Symbol_lookup(scope, yytext);
				if (sym == NULL) {
					/* Lookup didnt find anything in the current scope. Check if it collides with libfunc. */
					if (isLibraryFunction(yytext)==1) {
						/* Libfunc COLLISION!!! */
						SyntaxError_insert(strdup(yytext), strdup("is a formal argument, colliding with a library function."), yylineno);
					}
					else {
						sym = Symbol_create(FORMAL, strdup(yytext), yylineno, scope, 1);
						SymbolTable_insert(sym);
					}
				}
				else {
					/* Found the symbol in this scope. Do some research on it. */
					if (sym->type == FORMAL) {
						if (isActiveFunction(scope)) {
							//SyntaxError_insert(strdup(yytext), strdup("is already declared as a formal argument.1"), yylineno);
						}
						else {
							/* No active function must mean that this symbol is a FORMAL arg on a new user function */
							sym = Symbol_create(FORMAL, strdup(yytext), yylineno, scope, 1);
							SymbolTable_insert(sym);
						}
					}
				}
			}
			idliststar
			{
				fprintf(yyout,"idlist => id idliststar\n");
			}
		|	{
				fprintf(yyout,"idlist => void\n");
			}
		;
		idliststar: COMMA IDENTIFIER
			{
				fprintf(yyout,"idliststar => , id\n");
				SymbolTableEntry* sym = Symbol_lookup(scope, yytext);
				if (sym == NULL) {
					/* Lookup didnt find anything in the current scope. Check if it collides with libfunc. */
					if (isLibraryFunction(yytext)==1) {
						/* Libfunc COLLISION!!! */
						SyntaxError_insert(strdup(yytext), strdup("is a formal argument, colliding with a library function."), yylineno);
					}
					else {
						sym = Symbol_create(FORMAL, strdup(yytext), yylineno, scope, 1);
						SymbolTable_insert(sym);
					}
				}
				else {
					/* Found the symbol in this scope. Do some research on it. */
					if (sym->type == FORMAL) {
						SyntaxError_insert(strdup(yytext), strdup("is already declared as a formal argument."), yylineno);
					}
				}
			} idliststar
			{
				fprintf(yyout,"idliststar => , id eliststar\n");
			}
		|	{
				fprintf(yyout,"idliststar => void\n");
			}
		;



ifstmt: IF LEFT_PARENTHESIS expr RIGHT_PARENTHESIS stmt
			{
				fprintf(yyout,"ifstmt => if(expr)stmt\n");
			}
		| IF LEFT_PARENTHESIS expr RIGHT_PARENTHESIS stmt ELSE stmt
			{
				fprintf(yyout,"ifstmt => if (expr) else  stmt\n");
			}
		;

whilestmt: WHILE LEFT_PARENTHESIS expr RIGHT_PARENTHESIS {loop = 1;} stmt {loop = 0;}
			{
				fprintf(yyout,"whilestmt => while(expr)stmt\n");
			}
		;


forstmt: FOR LEFT_PARENTHESIS elist SEMICOLON expr SEMICOLON elist RIGHT_PARENTHESIS {loop = 1;} stmt {loop = 0;}
			{
				fprintf(yyout,"forstmt => for(elist;expr;elist)stmt\n");
			}
		;

returnstmt: RETURN{_return = 1;} SEMICOLON {_return = 0;}
			{
				fprintf(yyout,"return ;\n");
			}
	| RETURN {_return = 1;}expr SEMICOLON{_return = 0;}
			{
				fprintf(yyout,"return expr;\n");
			}
		;

%%
int yyerror(char const * yaccProvidedMessage) {
	fprintf(stderr, "Parse Error at line %d, %s\n", yylineno, yaccProvidedMessage);
	return -1;
}


 int main (int argc, char** argv)
{
	initialize_system();

	if (argc == 1) //no arguments
	{
		yyin = stdin;
		yyout = stdout;
	}
	else if (argc == 2) //INPUT_FILE
	{
		if ((yyin = fopen(argv[1], "r")) == NULL)
		{
			fprintf(stderr, "Unable to open the source file\nProgram Exit...\n");
			return -1;
		}
		yyout = stdout;
	}
	else if(argc == 3) //file output
	{
		if ((yyin = fopen(argv[1], "r")) == NULL)
		{
			fprintf(stderr, "Unable to open the source file\nProgram Exit...\n");
			return -1;
		}
		if ((yyout = fopen(argv[2], "w")) == NULL)
		{
			fprintf(stderr, "Error to open the destination file\nProgram Exit...\n");
			return -1;
		}
	}
	yyparse();
	if (scope != 0){
		SyntaxError_insert(strdup("}"), strdup("is missing"), yylineno);
	}
	putchar('\n');
	SymbolTable_print();
	putchar('\n');
	putchar('\n');
	SyntaxError_print();
	printf("\nDONE BB\n");
	return 0;
}
