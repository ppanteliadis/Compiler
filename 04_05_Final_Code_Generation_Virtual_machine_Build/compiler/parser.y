%{
	#include "symtable.h"
	#include "quad.h"
	#include "offsetstack.h"
	#include "loopcounterstack.h"
	#include "breakcontlist.h"
	#include "targetcode.h"

	int yylex(void);
	int yyerror(char const * yaccProvidedMessage);

	char * SymbolTableTypeArray[] = {"GLOBAL", "LOCAL", "FORMAL",
		"USERFUNC", "LIBFUNC", "NILL"};
	char* ScopeSpaceArray[] = {"NILL", "PROGRAM_VAR", "FUNC_LOCAL", "FORMAL_ARG"};



	int loopcounter = 0;
	int returnflag = 0;

	int max_scope = 0;


	extern int current_quad;
	extern quad* quad_table;



	extern int yylineno;
	extern char * yytext;
	extern FILE * yyin;
	extern FILE * yyout;

	SyntaxError * syntaxerrorhead;

	offset_stack* scopeoffsetstack;
	loopcounter_stack* loopcounterstack;
	breakcontlist* breakconthead;
	semanticerror* semanticerrorhead;

	int scope = 0;

%}

%expect 1

%start program


%union {
	int intVal;
	char* strVal;
	float realVal;
	expr* exprNode;
	SymbolTableEntry* symNode;
	call_t* callNode;
	forprefix* forNode;
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
%type <intVal> ifprefix elseprefix
%type <intVal> whilestmt whilestart whilecond
%type <forNode> forstmt forprefix
%type <strVal> returnstmt
%type <strVal> block

%type <strVal> loopstart loopstmt loopend
%type <strVal> funcblockstart funcblockend

%type <strVal> assignexpr
%type <strVal> term
%type <exprNode> lvalue
%type <strVal> primary
%type <exprNode> call objectdef
%type <strVal> const
%type <strVal> member
%type <exprNode> elist eliststar
%type <exprNode> indexed indexedstar indexedelem
%type <strVal> idlist idliststar

%type <strVal> funcname
%type <intVal> funcbody
%type <symNode> funcprefix funcdef
%type <callNode> normcall methodcall callsuffix
%type <intVal> N M



%right ASSIGNMENT

%left OR
%left AND

%nonassoc EQUAL NOT_EQUAL GREATER GREATER_EQUAL LESS LESS_EQUAL

%left PLUS MINUS
%left MUL DIV MOD

%right NOT PLUS_PLUS MINUS_MINUS UMINUS

%left DOT DOUBLE_DOT
%left LEFT_BRACKET RIGHT_BRACKET
%left LEFT_PARENTHESIS RIGHT_PARENTHESIS



%error-verbose


%%
program: stmt stmtstar
			{
				fprintf(yyout, "program => stmt stmtstar\n");
			}
		;

stmtstar: stmt stmtstar
			{
				fprintf(yyout, "stmtstar => stmt stmtstar\n");
			}
		|
			{
				fprintf(yyout, "stmtstar => empty\n");
			}
		;

stmt: expr SEMICOLON
			{
				fprintf(yyout,"stmt => expr;\n");
			}
		| ifstmt
			{
				fprintf(yyout,"stmt => IF statement\n");
			}
		| whilestmt
			{
				fprintf(yyout,"stmt => WHILE statement\n");
			}
		| forstmt
			{
				fprintf(yyout,"stmt => FOR statement\n");
			}
		| returnstmt
			{
				fprintf(yyout, "stmt => RETURN statement\n");
				if (scope == 0) {
					SyntaxError_insert(strdup("return"), strdup("cannot be used while not in a function."), yylineno);
				}
				else {
					if (returnflag) {
						if (!isActiveFunction(scope - 1)) {
							SyntaxError_insert(strdup("return"), strdup("cannot be used while not in a function."), yylineno);
						}
						returnflag = 0;
					}
				}
			}
		| BREAK SEMICOLON
			{
				fprintf(yyout,"stmt => BREAK;\n");
				if (loopcounter == 0) {
					SyntaxError_insert(strdup("break"), strdup("cannot be used while not inside a loop."), yylineno);
				}
				else {
					break_insert(nextquadlabel(), yylineno);
				}
			}
		| CONTINUE SEMICOLON
			{
				fprintf(yyout,"stmt => CONTINUE\n");
				if (loopcounter == 0) {
					SyntaxError_insert(strdup("continue"), strdup("cannot be used while not inside a loop."), yylineno);
				}
				else {
					cont_insert(nextquadlabel(), yylineno);
				}
			}
		| block
			{
				fprintf(yyout,"stmt => BLOCK statement\n");
			}
		| funcdef
			{
				fprintf(yyout,"stmt => FUNCTION DEFINITION\n");
				$<exprNode>$ = $<exprNode>1;
			}
		| SEMICOLON
			{
				fprintf(yyout,"stmt => \";\"\n");
			}
		;



expr: assignexpr
			{
				fprintf(yyout,"expr => assignexpr\n");
				//resettempname();
				$<exprNode>$ = $<exprNode>1;
			}
		| expr PLUS expr
			{
				fprintf(yyout,"expr => expression  + expression\n");
				expr* result;
				if ((result = compute_ifnum($<exprNode>1, $<exprNode>3, add))) {
					$<exprNode>$ = result;
				}
				else if ((result = compute_ifstr($<exprNode>1, $<exprNode>3, add, yylineno))) {
					$<exprNode>$ = result;
				}
				else {
					$<exprNode>$ = newexpr(arithexpr_e);
					$<exprNode>$->sym = newtemp();
					
					SymbolTable_insert($<exprNode>$->sym);
					$<exprNode>$->sym->offset = currscopeoffset();
					$<exprNode>$->sym->scopespace = currscopespace();
					incurrscopeoffset();

					if (currscopespace() == program_variable) {
						inc_globals();
					}


					emit(add, $<exprNode>1, $<exprNode>3, $<exprNode>$, -1, yylineno);
				}
			}
		| expr MINUS expr
			{
				fprintf(yyout,"expr => expr - expr\n");
				expr* result;
				if ((result = compute_ifnum($<exprNode>1, $<exprNode>3, sub))) {
					$<exprNode>$ = result;
				}
				else if ((result = compute_ifstr($<exprNode>1, $<exprNode>3, sub, yylineno))) {
					$<exprNode>$ = result;
				}
				else {
					$<exprNode>$ = newexpr(arithexpr_e);
					$<exprNode>$->sym = newtemp();

					SymbolTable_insert($<exprNode>$->sym);
					$<exprNode>$->sym->offset = currscopeoffset();
					incurrscopeoffset();
					$<exprNode>$->sym->scopespace = currscopespace();

					if (currscopespace() == program_variable) {
						inc_globals();
					}
					emit(sub, $<exprNode>1, $<exprNode>3, $<exprNode>$, -1, yylineno);
				}
			}
		| expr MUL expr
			{
				fprintf(yyout,"expr => expr * expr\n");
				expr* result;
				if ((result = compute_ifnum($<exprNode>1, $<exprNode>3, mul))) {
					$<exprNode>$ = result;
				}
				else if ((result = compute_ifstr($<exprNode>1, $<exprNode>3, mul, yylineno))) {
					$<exprNode>$ = result;
				}
				else {
					$<exprNode>$ = newexpr(arithexpr_e);
					$<exprNode>$->sym = newtemp();
					
					SymbolTable_insert($<exprNode>$->sym);
					$<exprNode>$->sym->offset = currscopeoffset();
					incurrscopeoffset();
					$<exprNode>$->sym->scopespace = currscopespace();

					if (currscopespace() == program_variable) {
						inc_globals();
					}

					emit(mul, $<exprNode>1, $<exprNode>3, $<exprNode>$, -1, yylineno);
				}

			}
		| expr DIV expr
			{
				fprintf(yyout,"expr => expr / expr\n");
				expr* result;
				if ((result = compute_ifnum($<exprNode>1, $<exprNode>3, divi))) {
					$<exprNode>$ = result;
				}
				else if ((result = compute_ifstr($<exprNode>1, $<exprNode>3, divi, yylineno))) {
					$<exprNode>$ = result;
					//emit(add, $<exprNode>1, $<exprNode>3, $<exprNode>$, -1, yylineno);
				}
				else{
					$<exprNode>$ = newexpr(arithexpr_e);
					$<exprNode>$->sym = newtemp();
					
					SymbolTable_insert($<exprNode>$->sym);

					$<exprNode>$->sym->offset = currscopeoffset();
					incurrscopeoffset();
					$<exprNode>$->sym->scopespace = currscopespace();

					if (currscopespace() == program_variable && !istempname($<exprNode>$->sym->name)) {
						inc_globals();
					}

					emit(divi, $<exprNode>1, $<exprNode>3, $<exprNode>$, -1, yylineno);
				}

			}
		| expr MOD expr
			{
				fprintf(yyout,"expr => expr % expr\n");
				expr* result;
				if ((result = compute_ifnum($<exprNode>1, $<exprNode>3, mod))) {
					$<exprNode>$ = result;
				}
				else if ((result = compute_ifstr($<exprNode>1, $<exprNode>3, mod, yylineno))) {
					$<exprNode>$ = result;
					//emit(add, $<exprNode>1, $<exprNode>3, $<exprNode>$, -1, yylineno);
				}
				else {
					$<exprNode>$ = newexpr(arithexpr_e);
					$<exprNode>$->sym = newtemp();
					
					SymbolTable_insert($<exprNode>$->sym);
					$<exprNode>$->sym->offset = currscopeoffset();
					incurrscopeoffset();
					$<exprNode>$->sym->scopespace = currscopespace();

					if (currscopespace() == program_variable) {
						inc_globals();
					}

					emit(mod, $<exprNode>1, $<exprNode>3, $<exprNode>$, -1, yylineno);
				}
			}
		| expr GREATER expr
			{
				fprintf(yyout,"expr => expr > expr\n");
				$<exprNode>$ = newexpr(boolexpr_e);
				$<exprNode>$->sym = newtemp();
				
				SymbolTable_insert($<exprNode>$->sym);
				$<exprNode>$->sym->offset = currscopeoffset();
				incurrscopeoffset();
				$<exprNode>$->sym->scopespace = currscopespace();

				if (currscopespace() == program_variable) {
					inc_globals();
				}

				emit(if_greater, $<exprNode>1, $<exprNode>3, (expr*)NULL, nextquadlabel() + 3, yylineno);
				emit(assign, expr_boolConst(0), (expr*)NULL, $<exprNode>$, -1, yylineno);
				emit(jump, (expr*)NULL, (expr*)NULL, (expr*)NULL, nextquadlabel() + 2, yylineno);
				emit(assign, expr_boolConst(1), (expr*)NULL, $<exprNode>$, -1, yylineno);
			}
		| expr GREATER_EQUAL expr
			{
				fprintf(yyout,"expr => expr >= expr\n");
				$<exprNode>$ = newexpr(boolexpr_e);
				$<exprNode>$->sym = newtemp();
				
				SymbolTable_insert($<exprNode>$->sym);
				$<exprNode>$->sym->offset = currscopeoffset();
				incurrscopeoffset();
				$<exprNode>$->sym->scopespace = currscopespace();

				if (currscopespace() == program_variable) {
					inc_globals();
				}

				emit(if_greatereq, $<exprNode>1, $<exprNode>3, (expr*)NULL, nextquadlabel() + 3, yylineno);
				emit(assign, expr_boolConst(0), (expr*)NULL, $<exprNode>$, -1, yylineno);
				emit(jump, (expr*)NULL, (expr*)NULL, (expr*)NULL, nextquadlabel() + 2, yylineno);
				emit(assign, expr_boolConst(1), (expr*)NULL, $<exprNode>$, -1, yylineno);
			}
		| expr LESS expr
			{
				fprintf(yyout,"expr => expr < expr\n");
				$<exprNode>$ = newexpr(boolexpr_e);
				$<exprNode>$->sym = newtemp();
				
				SymbolTable_insert($<exprNode>$->sym);
				$<exprNode>$->sym->offset = currscopeoffset();
				incurrscopeoffset();
				$<exprNode>$->sym->scopespace = currscopespace();

				if (currscopespace() == program_variable) {
					inc_globals();
				}

				emit(if_less, $<exprNode>1, $<exprNode>3, (expr*)NULL, nextquadlabel() + 3, yylineno);
				emit(assign, expr_boolConst(0), (expr*)NULL, $<exprNode>$, -1, yylineno);
				emit(jump, (expr*)NULL, (expr*)NULL, (expr*)NULL, nextquadlabel() + 2, yylineno);
				emit(assign, expr_boolConst(1), (expr*)NULL, $<exprNode>$, -1, yylineno);
			}
		| expr LESS_EQUAL expr
			{
				fprintf(yyout,"expr => expr <= expr\n");
				$<exprNode>$ = newexpr(boolexpr_e);
				$<exprNode>$->sym = newtemp();
				
				SymbolTable_insert($<exprNode>$->sym);
				$<exprNode>$->sym->offset = currscopeoffset();
				incurrscopeoffset();
				$<exprNode>$->sym->scopespace = currscopespace();

				if (currscopespace() == program_variable) {
					inc_globals();
				}

				emit(if_lesseq, $<exprNode>1, $<exprNode>3, (expr*)NULL, nextquadlabel() + 3, yylineno);
				emit(assign, expr_boolConst(0), (expr*)NULL, $<exprNode>$, -1, yylineno);
				emit(jump, (expr*)NULL, (expr*)NULL, (expr*)NULL, nextquadlabel() + 2, yylineno);
				emit(assign, expr_boolConst(1), (expr*)NULL, $<exprNode>$, -1, yylineno);
			}
		| expr EQUAL expr
			{
				fprintf(yyout,"expr => expr == expr\n");
				$<exprNode>$ = newexpr(boolexpr_e);
				$<exprNode>$->sym = newtemp();
				
				SymbolTable_insert($<exprNode>$->sym);
				$<exprNode>$->sym->offset = currscopeoffset();
				incurrscopeoffset();
				$<exprNode>$->sym->scopespace = currscopespace();

				if (currscopespace() == program_variable) {
					inc_globals();
				}

				emit(if_eq, $<exprNode>1, $<exprNode>3, (expr*)NULL, nextquadlabel() + 3, yylineno);
				emit(assign, expr_boolConst(0), (expr*)NULL, $<exprNode>$, -1, yylineno);
				emit(jump, (expr*)NULL, (expr*)NULL, (expr*)NULL, nextquadlabel() + 2, yylineno);
				emit(assign, expr_boolConst(1), (expr*)NULL, $<exprNode>$, -1, yylineno);
			}
		| expr NOT_EQUAL expr
			{
				fprintf(yyout,"expr => expr != expr\n");
				$<exprNode>$ = newexpr(boolexpr_e);
				$<exprNode>$->sym = newtemp();
				
				SymbolTable_insert($<exprNode>$->sym);
				$<exprNode>$->sym->offset = currscopeoffset();
				incurrscopeoffset();
				$<exprNode>$->sym->scopespace = currscopespace();

				if (currscopespace() == program_variable) {
					inc_globals();
				}

				emit(if_noteq, $<exprNode>1, $<exprNode>3, (expr*)NULL, nextquadlabel() + 3, yylineno);
				emit(assign, expr_boolConst(0), (expr*)NULL, $<exprNode>$, -1, yylineno);
				emit(jump, (expr*)NULL, (expr*)NULL, (expr*)NULL, nextquadlabel() + 2, yylineno);
				emit(assign, expr_boolConst(1), (expr*)NULL, $<exprNode>$, -1, yylineno);
			}
		| expr AND expr
			{
				fprintf(yyout,"expr => expr && expr\n");
				$<exprNode>$ = newexpr(boolexpr_e);
				$<exprNode>$->sym = newtemp();
				
				SymbolTable_insert($<exprNode>$->sym);
				$<exprNode>$->sym->offset = currscopeoffset();
				incurrscopeoffset();
				$<exprNode>$->sym->scopespace = currscopespace();

				if (currscopespace() == program_variable) {
					inc_globals();
				}

				emit(and, $<exprNode>1, $<exprNode>3, $<exprNode>$, -1, yylineno);
			}
		| expr OR expr
			{
				fprintf(yyout,"expr => expr || expr\n");
				$<exprNode>$ = newexpr(boolexpr_e);
				$<exprNode>$->sym = newtemp();
				
				SymbolTable_insert($<exprNode>$->sym);
				$<exprNode>$->sym->offset = currscopeoffset();
				incurrscopeoffset();
				$<exprNode>$->sym->scopespace = currscopespace();

				if (currscopespace() == program_variable) {
					inc_globals();
				}

				emit(or, $<exprNode>1, $<exprNode>3, $<exprNode>$, -1, yylineno);
			}
		| term
			{
				fprintf(yyout,"expr => term\n");
				$<exprNode>$ = $<exprNode>1;
			}
		;

assignexpr: lvalue ASSIGNMENT expr
			{
				fprintf(yyout,"assignexpr => lvalue = expr\n");
				if ($1->sym->type == LIBFUNC) {
					SyntaxError_insert(strdup($1->sym->name), strdup("is a library function and cannot be used as an lvalue."), $1->sym->line);
				}
				else if ($1->sym->type == USERFUNC) {
					SyntaxError_insert(strdup($1->sym->name), strdup("is a library function and cannot be used as an lvalue."), $1->sym->line);
				}
				else {
					if ($<exprNode>1->type == tableitem_e) {
						emit(tablesetelem, $<exprNode>1, $<exprNode>1->index, $<exprNode>3, -1, yylineno);
						$<exprNode>$ = emit_iftableitem($<exprNode>1);
						$<exprNode>$->type = assignexpr_e;
					}
					else {
						emit(assign, $<exprNode>3, (expr*)NULL, $<exprNode>1, -1, yylineno);
						$<exprNode>$ = newexpr(assignexpr_e);
						$<exprNode>$->sym = newtemp();
						
						SymbolTable_insert($<exprNode>$->sym);
						$<exprNode>$->sym->offset = currscopeoffset();
						incurrscopeoffset();
						$<exprNode>$->sym->scopespace = currscopespace();

						if (currscopespace() == program_variable) {
							inc_globals();
						}

						emit(assign, $<exprNode>1, (expr*)NULL, $<exprNode>$, -1, yylineno);
					}
				}
			}
		;

term: LEFT_PARENTHESIS expr RIGHT_PARENTHESIS
			{
				fprintf(yyout, "term => (expr)\n");
				$<exprNode>$ = $<exprNode>2;
			}
		| MINUS expr %prec UMINUS
			{
				fprintf(yyout,"term => -expr\n");
				checkuminus($<exprNode>2);
				$<exprNode>$ = newexpr(arithexpr_e);
				$<exprNode>$->sym = newtemp();
				$<exprNode>$->sym->offset = currscopeoffset();
				incurrscopeoffset();
				$<exprNode>$->sym->scopespace = currscopespace();
				SymbolTable_insert($<exprNode>$->sym);

				emit(uminus, $<exprNode>2, (expr*)NULL, $<exprNode>$, -1, yylineno);
			}
		| NOT expr
			{
				fprintf(yyout,"term => not expr\n");
				$<exprNode>$ = newexpr(boolexpr_e);
				$<exprNode>$->sym = istempexpr($<exprNode>2) ? $<exprNode>$->sym : newtemp();
				$<exprNode>$->sym->offset = currscopeoffset();
				incurrscopeoffset();
				$<exprNode>$->sym->scopespace = currscopespace();
				SymbolTable_insert($<exprNode>$->sym);

				emit(not, $<exprNode>2, (expr*)NULL, $<exprNode>$, -1, yylineno);
			}
		| PLUS_PLUS lvalue
			{
				fprintf(yyout,"term => ++lvalue\n");
				if ($2->sym->type == LIBFUNC) {
					SyntaxError_insert(strdup($2->sym->name), strdup("is a library function and cannot be used as an ++lvalue."), yylineno);
				}
				else if ($2->sym->type == USERFUNC) {
					SyntaxError_insert(strdup($2->sym->name), strdup("is a library function and cannot be used as an ++lvalue."), yylineno);
				}
				else {
					if ($<exprNode>2->type == tableitem_e) {
						$<exprNode>$ = emit_iftableitem($<exprNode>2);
						emit(add, $<exprNode>$, expr_numConst(1), $<exprNode>$, -1, yylineno);
						emit(tablesetelem, $<exprNode>2, $<exprNode>2->index, $<exprNode>$, -1, yylineno);
					}
					else {
						emit(add, $<exprNode>2, expr_numConst(1), $<exprNode>2, -1, yylineno);
						$<exprNode>$ = newexpr(arithexpr_e);
						$<exprNode>$->sym = newtemp();
						$<exprNode>$->sym->offset = currscopeoffset();
						incurrscopeoffset();
						$<exprNode>$->sym->scopespace = currscopespace();
						SymbolTable_insert($<exprNode>$->sym);

						emit(assign, $<exprNode>2, NULL, $<exprNode>$, -1, yylineno);
					}
				}

			}
		| lvalue PLUS_PLUS
			{
				fprintf(yyout,"term => lvalue++\n");
				if ($1->sym->type == LIBFUNC) {
					SyntaxError_insert(strdup($1->sym->name), strdup("is a library function and cannot be used as an lvalue++."), yylineno);
				}
				else if ($1->sym->type == USERFUNC) {
					SyntaxError_insert(strdup($1->sym->name), strdup("is a library function and cannot be used as an lvalue++."), yylineno);
				}
				else {
					/* All is good, proceed... */
					$<exprNode>$ = newexpr(var_e);
					$<exprNode>$->sym = newtemp();
					$<exprNode>$->sym->offset = currscopeoffset();
					incurrscopeoffset();
					$<exprNode>$->sym->scopespace = currscopespace();
					SymbolTable_insert($<exprNode>$->sym);

					if ($<exprNode>$->type == tableitem_e) {
						expr* value = emit_iftableitem($<exprNode>1);
						emit(assign, value, $<exprNode>$, NULL, -1, yylineno);
						emit(add, value, expr_numConst(1), value, -1, yylineno);
						emit(tablesetelem, $<exprNode>1, $<exprNode>1->index, value, -1, yylineno);
					}
					else {
						emit(assign, $<exprNode>1, NULL, $<exprNode>$, -1, yylineno);
						emit(add, $<exprNode>1, expr_numConst(1), $<exprNode>1, -1, yylineno);
					}
				}
			}
		| MINUS_MINUS lvalue
			{
				fprintf(yyout,"term => --lvalue\n");
				if ($2->sym->type == LIBFUNC) {
					SyntaxError_insert(strdup($2->sym->name), strdup("is a library function and cannot be used as an --lvalue."), yylineno);
				}
				else if ($2->sym->type == USERFUNC) {
					SyntaxError_insert(strdup($2->sym->name), strdup("is a library function and cannot be used as an --lvalue."), yylineno);
				}
				else {
					if ($<exprNode>2->type == tableitem_e) {
						$<exprNode>$ = emit_iftableitem($<exprNode>2);
						emit(sub, $<exprNode>$, expr_numConst(1), $<exprNode>$, -1, yylineno);
						emit(tablesetelem, $<exprNode>2, $<exprNode>2->index, $<exprNode>$, -1, yylineno);
					}
					else {
						emit(sub, $<exprNode>2, expr_numConst(1), $<exprNode>2, -1, yylineno);
						$<exprNode>$ = newexpr(arithexpr_e);
						$<exprNode>$->sym = newtemp();
						$<exprNode>$->sym->offset = currscopeoffset();
						incurrscopeoffset();
						$<exprNode>$->sym->scopespace = currscopespace();
						SymbolTable_insert($<exprNode>$->sym);

						emit(assign, $<exprNode>2, NULL, $<exprNode>$, -1, yylineno);
					}
				}
			}
		| lvalue MINUS_MINUS
			{
				fprintf(yyout,"term => lvalue--\n");
				if ($1->sym->type == LIBFUNC) {
					SyntaxError_insert(strdup($1->sym->name), strdup("is a library function and cannot be used as an lvalue--."), yylineno);
				}
				else if ($1->sym->type == USERFUNC) {
					SyntaxError_insert(strdup($1->sym->name), strdup("is a library function and cannot be used as an lvalue--."), yylineno);
				}
				else {
					/* All is good, proceed... */
					$<exprNode>$ = newexpr(var_e);
					$<exprNode>$->sym = newtemp();
					$<exprNode>$->sym->offset = currscopeoffset();
					incurrscopeoffset();
					$<exprNode>$->sym->scopespace = currscopespace();
					SymbolTable_insert($<exprNode>$->sym);

					if ($<exprNode>$->type == tableitem_e) {
						expr* value = emit_iftableitem($<exprNode>1);
						emit(assign, value, $<exprNode>$, NULL, -1, yylineno);
						emit(sub, value, expr_numConst(1), value, -1, yylineno);
						emit(tablesetelem, $<exprNode>1, $<exprNode>1->index, value, -1, yylineno);
					}
					else {
						emit(assign, $<exprNode>1, NULL, $<exprNode>$, -1, yylineno);
						emit(sub, $<exprNode>1, expr_numConst(1), $<exprNode>1, -1, yylineno);
					}
				}
			}
		| primary
			{
				fprintf(yyout,"term => primary\n");
				$<exprNode>$ = $<exprNode>1;
			}
		;

primary: lvalue
			{
				fprintf(yyout,"primary => lvalue\n");
				$<exprNode>$ = emit_iftableitem($<exprNode>1);
			}
		| call
			{
				fprintf(yyout,"primary => call\n");
				$<exprNode>$ = $<exprNode>1;
			}
		| objectdef
			{
				fprintf(yyout,"primary => objectdef\n");
				$<exprNode>$ = newexpr(newtable_e);
				$<exprNode>$ = $<exprNode>1;
			}
		| LEFT_PARENTHESIS funcdef RIGHT_PARENTHESIS
			{
				fprintf(yyout,"primary => (funcdef)\n");
				$<exprNode>$ = newexpr(programfunc_e);
				$<exprNode>$->sym = $2;
			}
		| const
			{
				fprintf(yyout,"primary => const\n");
				$<exprNode>$ = $<exprNode>1;
			}
		;

lvalue: IDENTIFIER
			{
				fprintf(yyout,"lvalue => id\n");
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
						sym->scopespace = currscopespace();
						sym->offset = currscopeoffset();
						SymbolTable_insert(sym);

						if (currscopespace() == program_variable) {
							inc_globals();
						}
					}
					else {
						sym = Symbol_create(LOCAL_VAR, strdup(yytext), yylineno, tmpscope, 1);
						sym->scopespace = currscopespace();
						sym->offset = currscopeoffset();
						SymbolTable_insert(sym);
					}
					incurrscopeoffset();
					/* Create new lvalue expression. */
					$<exprNode>$ = expr_lvalue(sym);
				}
				else {
					/* FOUND SOMETHING!! */
					/* Need to see if we can access it! */
					if (sym->type == GLOBAL || sym->type == LOCAL_VAR || sym->type == LIBFUNC) {
						/* Found a global || local var || LIBFUNC, referencing to it! */
						$<exprNode>$ = expr_lvalue(sym);
					}
					else if (isActiveFunction(scope-1) && scope >= 0) {
						/* See if a function interferes... */
						if (sym->type == FORMAL && sym->scope == scope) {
							/* Valid FORMAL variable in the current scope. Referrencing to it.*/
							$<exprNode>$ = expr_lvalue(sym);
						}
						else if (sym->type == USERFUNC) {
							/* Valid userfunc, referencing to it. */
							$$->sym = sym;
							/***********/
						}
						else {
							if (isLibraryFunction(yytext)) {
								/*No issue here.*/
								$$->sym = sym;
								/***********/
							}
							else {
								SyntaxError_insert(strdup(yytext), strdup("cannot be accessed due to an active function being interposed."), yylineno);
							}
						}
					}
					else {
						/* We have a function. Use reduction so we can test if it's ok on the FUNCTION rule.*/
						$$->sym = sym;
					}
				}
			}
		| LOCAL IDENTIFIER
			{
				fprintf(yyout, "lvalue => local id\n");
				SymbolTableEntry* sym = Symbol_lookup(scope, yytext);
				if (sym != NULL) {
					/* Found variable or function in the current scope, referencing to it. */
				}
				else {
					/* Found nothing, create a new symbol if it doesnt collide with libfunc. */
					if (isLibraryFunction(yytext) == 1) {
						SyntaxError_insert(strdup(yytext), strdup("is a local variable colliding with a library function."), yylineno);
					}
					else {
						if (scope == 0) {
							sym = Symbol_create(GLOBAL, strdup(yytext), yylineno, 0, 1);
						}
						else {
							sym = Symbol_create(LOCAL_VAR, strdup(yytext), yylineno, scope, 1);
						}
						SymbolTable_insert(sym);
						sym->scopespace = currscopespace();
						sym->offset = currscopeoffset();

						if (currscopespace() == program_variable) {
							inc_globals();
						}
						incurrscopeoffset();
					}
				}
				$<exprNode>$ = expr_lvalue(sym);
			}
		| DOUBLE_COLON IDENTIFIER
			{
				fprintf(yyout,"lvalue => ::id\n");
				/* Searching in global scope. */
				SymbolTableEntry* sym = Symbol_lookup(0, yytext);
				if (sym != NULL) {
					if (sym->type == GLOBAL) {
						/* Referencing to a GLOBAL variable. */
						$<exprNode>$ = expr_lvalue(sym);
					}
					else if (sym->type == USERFUNC) {
						/* Referrencing to a USERFUNC. */
					}
					else if (sym->type == LIBFUNC) {
						/* Referrencing to a LIBFUNC. */
					}
				}
				else {
					SyntaxError_insert(strdup(yytext), strdup("is referencing to an undefined global variable."), yylineno);
				}
			}
		| member
			{
				fprintf(yyout,"lvalue => member\n");
				$<exprNode>$ = $<exprNode>1;
			}
		;

call: call LEFT_PARENTHESIS elist RIGHT_PARENTHESIS
		{
			fprintf(yyout,"call => call(elist)\n");
			$$ = make_call($1, $3);
		}
	| lvalue callsuffix
		{
			fprintf(yyout,"call => lvalue callsuffix\n");
			if ($2->method) {
				expr* self = $<exprNode>1;
				$1 = emit_iftableitem(member_item(self, $2->name));
				$2->elist = add_front($2->elist, self);
			}
			$<exprNode>$ = make_call($<exprNode>1, $2->elist);

		}
	| LEFT_PARENTHESIS funcdef RIGHT_PARENTHESIS LEFT_PARENTHESIS elist RIGHT_PARENTHESIS
		{
			fprintf(yyout,"call => (funcdef)(elist)\n");
			expr* func = newexpr(programfunc_e);
			func->sym = $2;
			$<exprNode>$ = make_call(func, $5);

		}
	;

member: lvalue DOT IDENTIFIER
			{
				fprintf(yyout,"member => lvalue.id\n");
				$<exprNode>$ = member_item($<exprNode>1, $3);
			}
		| lvalue LEFT_BRACKET expr RIGHT_BRACKET
			{
				fprintf(yyout,"member => lvalue[expr]\n");
				$<exprNode>1 = emit_iftableitem($<exprNode>1);
				$<exprNode>$ = newexpr(tableitem_e);
				$<exprNode>$->sym = $<exprNode>1->sym;
				$<exprNode>$->index = $<exprNode>3;
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
				fprintf(yyout, "callsuffix => normcall\n");
				$$ = $1;
			}
		| methodcall
			{
				fprintf(yyout, "callsuffix =>methodcall\n");
				$$ = $1;
			}
		;


normcall: LEFT_PARENTHESIS elist RIGHT_PARENTHESIS
			{
				fprintf(yyout, "normcall => (elist)\n");
				$$ = make_normcall($2);
			}
		;


methodcall: DOUBLE_DOT IDENTIFIER LEFT_PARENTHESIS elist RIGHT_PARENTHESIS
			{
				fprintf(yyout, "methodcall => ..id(elist)\n");
				$$ = make_methodcall($4, $2);
			}
		;

elist: expr eliststar
			{
				fprintf(yyout,"elist => expr elist*\n");
				$<exprNode>$ = $<exprNode>1;
				$$->next = $2;
			}
		|	{
				fprintf(yyout,"elist => void\n");
				$$=NULL;
			}
		;

eliststar: COMMA expr eliststar
			{
				fprintf(yyout,"eliststar => , expr eliststar\n");
				$<exprNode>$ = $<exprNode>2;
				$$->next = $3;
			}
		|	{
				fprintf(yyout,"eliststar => void\n");
				$$=NULL;
			}
		;

objectdef: LEFT_BRACKET elist RIGHT_BRACKET
			{
				fprintf(yyout,"objectdef => [elist]\n");
				expr* t = newexpr(newtable_e);
				t->sym = newtemp();

				SymbolTable_insert(t->sym);
				t->sym->offset = currscopeoffset();
				incurrscopeoffset();
				t->sym->scopespace = currscopespace();

				if (currscopespace() == program_variable) {
					inc_globals();
				}

				emit(tablecreate, (expr*)NULL, (expr*)NULL, t, -1, yylineno);
				int i = 0;
				expr* x;
				for (x = $2; x != NULL; x = x->next) {
					emit(tablesetelem, t, expr_numConst(i++), x, -1, yylineno);
				}
				$$ = t;

			}
		| LEFT_BRACKET indexed RIGHT_BRACKET
			{
				fprintf(yyout,"objectdef => [indexed]\n");
				expr* t = newexpr(newtable_e);
				t->sym = newtemp();
		
				SymbolTable_insert(t->sym);
				t->sym->offset = currscopeoffset();
				incurrscopeoffset();
				t->sym->scopespace = currscopespace();

				if (currscopespace() == program_variable) {
					inc_globals();
				}

				emit(tablecreate, (expr*)NULL, (expr*)NULL, t, -1, yylineno);
				expr* x;
				for (x = $2; x != NULL; x = x->next) {
					emit(tablesetelem, t, x, x->index, -1, yylineno);
				}
				$$ = t;
			}
		;

indexed: indexedelem indexedstar
			{
				fprintf(yyout,"indexed => indexedelem\n");
				$$ = $1;
				$$->next = $2;
			}
indexedstar: COMMA indexedelem indexedstar
			{
				fprintf(yyout,"indexedstar => , indexedelem indexedstar\n");
				$$ = $2;
				$$->next = $3;
			}
		|	{
				fprintf(yyout, "indexedstar => void\n");
				$$ = NULL;
			}
		;

indexedelem: LEFT_BRACE expr COLON expr RIGHT_BRACE
			{
				fprintf(yyout,"indexedelem => {expr:expr}\n");
				$<exprNode>$ = $<exprNode>2;
				$<exprNode>$->index = $<exprNode>4;
			}
		;

block: LEFT_BRACE {scope++;raisemaxScope();} stmt stmtstar RIGHT_BRACE{SymbolTable_hide_scope(scope);scope--;}
			{
				fprintf(yyout,"block => {stmt stmtstar}\n");
			}
		| LEFT_BRACE{scope++;raisemaxScope();} RIGHT_BRACE{SymbolTable_hide_scope(scope);scope--;}
			{
				fprintf(yyout,"block => {}\n");
			}
		;



funcdef: funcprefix funcargs funcblockstart funcbody funcblockend
			{
				fprintf(yyout, "funcdef => funcprefix funcargs funcbody\n");
				exitscopespace();
				$<exprNode>1->sym->totallocals = $4;
				scopespace_t old_offset = pop_offset();
				restorecurrscopeoffset(old_offset);
				$$ = $<exprNode>1->sym;
				emit(funcend, (expr*)NULL, (expr*)NULL, expr_lvalue($<exprNode>1->sym), -1, yylineno);
			}
		;


funcprefix: FUNCTION funcname
			{
				fprintf(yyout,"funcprefix => function funcname\n");
				SymbolTableEntry* sym = Symbol_lookup(scope, yytext);

				if (sym == NULL) {
					/* Nothing like this exists in this scope. */
					if (isLibraryFunction($2)==1) {
						SyntaxError_insert(strdup($2), strdup("is a library function and cannot be overshadowed."), yylineno);
					}
					else {
						sym = Symbol_create(USERFUNC, strdup($2), yylineno, scope, 1);
						sym->scopespace = currscopespace();
						sym->offset = currscopeoffset();
						SymbolTable_insert(sym);

						$<exprNode>$ = newexpr(programfunc_e);
						$<exprNode>$->sym = sym;
						$<exprNode>$->sym->iaddress = nextquadlabel();

						emit(funcstart, (expr*)NULL, (expr*)NULL, expr_lvalue($<exprNode>$->sym), -1, yylineno);
						push_offset(currscopeoffset());

						enterscopespace();

						resetformalargoffset();
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
		;


funcname: IDENTIFIER
			{
				fprintf(yyout,"funcname => id\n");
				$$ = $1;
			}
		|
			{
				fprintf(yyout,"funcname => (void)\n");
				$$ = createNamelessFunc();
			}
		;

funcargs: LEFT_PARENTHESIS
			{
				fprintf(yyout,"funcargs => (idlist)\n");
				scope++;
			} idlist
			RIGHT_PARENTHESIS
			{
				scope--;
				enterscopespace();
				resetfunctionlocalsoffset();
			}
		;

funcbody: block
			{
				fprintf(yyout,"funbody => {}\n");
				$$ = currscopeoffset();
				exitscopespace();
			}
		;

const: INTEGER
			{
				fprintf(yyout,"const => INTEGER %d\n", atoi(yytext));
				$<exprNode>$ = newexpr(constnum_e);
				$<exprNode>$->numConst = yylval.intVal;
			}
		| REAL
			{
				fprintf(yyout,"const => REAL %f\n", atof(yytext));
				$<exprNode>$ = newexpr(constnum_e);
				$<exprNode>$->numConst = yylval.realVal;
			}
		| STRING
			{
				fprintf(yyout,"const => STRING %s\n", yytext);
				$<exprNode>$ = newexpr(conststring_e);
				$<exprNode>$->strConst = yylval.strVal;
			}
		| NIL
			{
				fprintf(yyout,"const => NIL\n");
				$<exprNode>$ = newexpr(nil_e);
			}
		| TRUE
			{
				fprintf(yyout,"const => TRUE\n");
				$<exprNode>$ = expr_boolConst(1);
			}
		| FALSE
			{
				fprintf(yyout,"const => FALSE\n");
				$<exprNode>$ = expr_boolConst(0);
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
						sym->scopespace = currscopespace();
						sym->offset = currscopeoffset();
						SymbolTable_insert(sym);

						$<exprNode>$ = expr_lvalue(sym);
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
							sym->scopespace = currscopespace();
							sym->offset = currscopeoffset();
							SymbolTable_insert(sym);
							//$<exprNode>$ = expr_lvalue(sym);
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
				fprintf(yyout,"idliststar => ,id\n");
				incurrscopeoffset();
				SymbolTableEntry* sym = Symbol_lookup(scope, yytext);
				if (sym == NULL) {
					/* Lookup didnt find anything in the current scope. Check if it collides with libfunc. */
					if (isLibraryFunction(yytext)==1) {
						/* Libfunc COLLISION!!! */
						SyntaxError_insert(strdup(yytext), strdup("is a formal argument, colliding with a library function."), yylineno);
					}
					else {
						sym = Symbol_create(FORMAL, strdup(yytext), yylineno, scope, 1);
						sym->scopespace = currscopespace();
						sym->offset = currscopeoffset();
						SymbolTable_insert(sym);
						//$<exprNode>$ = expr_lvalue(sym);
					}
				}
				else {
					/* Found the symbol in this scope. Do some research on it. */
					if (sym->type == FORMAL) {
						if (sym->isActive == 0) {
							SymbolTableEntry* new_sym = Symbol_create(FORMAL, strdup(yytext), yylineno, scope, 1);
							new_sym->scopespace = currscopespace();
							new_sym->offset = currscopeoffset();
							SymbolTable_insert(new_sym);
						}
						else {
							SyntaxError_insert(strdup(yytext), strdup("is already declared as a formal argument."), yylineno);
						}
					}
				}
			}
		idliststar
			{
				fprintf(yyout,"idliststar => , id eliststar\n");
			}
		|	{
				fprintf(yyout,"idliststar => void\n");
			}
		;


ifprefix: IF LEFT_PARENTHESIS expr RIGHT_PARENTHESIS
			{
				fprintf(yyout,"ifprefix => if(expr)\n");
				emit(if_eq, $<exprNode>3, expr_boolConst(1), (expr*)NULL, nextquadlabel() + 2, yylineno);
				$$ = nextquadlabel();
				emit(jump, (expr*)NULL, (expr*)NULL, (expr*)NULL, -10, yylineno);
			}
		;

elseprefix: ELSE
			{
				$$ = nextquadlabel();
				emit(jump, (expr*)NULL, (expr*)NULL, (expr*)NULL, -10, yylineno);
			}
		;


ifstmt: ifprefix stmt
			{
				patchlabel($1, nextquadlabel());
			}
		| ifprefix stmt elseprefix stmt
			{
				patchlabel($1, $3 + 1);
				patchlabel($3, nextquadlabel());
			}
		;


whilestart: WHILE
			{
				fprintf(yyout,"whilestart => while\n");
				$$ = nextquadlabel();
			}
		;

whilecond: LEFT_PARENTHESIS expr RIGHT_PARENTHESIS
			{
				fprintf(yyout,"whilecond => (expr)\n");
				emit(if_eq, $<exprNode>2, expr_boolConst(1), (expr*)NULL, nextquadlabel() + 2, yylineno);
				$$ = nextquadlabel();
				emit(jump, (expr*)NULL, (expr*)NULL, (expr*)NULL, -10, yylineno);
			}
		;

whilestmt: whilestart whilecond loopstmt
			{
				fprintf(yyout,"whilestmt => whilestart whilecond loopstmt\n");
				emit(jump, (expr*)NULL, (expr*)NULL, (expr*)NULL, $1, yylineno);
				patchlabel($2, nextquadlabel());

				while_backpatch();
			}
		;

loopstart:
			{
				++loopcounter;
			}
		;

loopend:
			{
				--loopcounter;
			}
		;

loopstmt: loopstart stmt loopend
			{
				$$ = $2;
			}
		;

funcblockstart:
			{
				push_funcblock(loopcounter);
				loopcounter = 0;
			}
		;

funcblockend:
			{
				loopcounter = pop_funcblock();
			}
		;


N:
			{
				$$ = nextquadlabel();
				emit(jump, (expr*)NULL, (expr*)NULL, (expr*)NULL, -10, yylineno);
			}
		;

M:
			{
				$$ = nextquadlabel();
			}
		;

forprefix: FOR LEFT_PARENTHESIS elist SEMICOLON M expr SEMICOLON
			{
				fprintf(yyout, "forprefix => for ( elist; M expr;\n");
				$$ = (forprefix*)malloc(sizeof(forprefix));
				$$->test = $5;
				$$->enter = nextquadlabel();
				emit(if_eq, $<exprNode>6, expr_boolConst(1), (expr*)NULL, -10, yylineno);
			}
		;

forstmt: forprefix N elist RIGHT_PARENTHESIS N loopstmt N
			{
				fprintf(yyout, "forstmt => forstmt forprefix N elist )N stmt N loopstmt\n");
				patchlabel($1->enter, $2 + 1); 				//true jump
				patchlabel($2, nextquadlabel());			//false jump
				patchlabel($5, $1->test);					//loop jump
				patchlabel($7, $2 + 1);						//closure jump

				for_backpatch($2);							//false and closure jumps.
			}
		;

returnstmt: RETURN SEMICOLON
			{
				fprintf(yyout,"return ;\n");
				returnflag = 1;
				emit(retur, (expr*)NULL, (expr*)NULL, (expr*)NULL, -1, yylineno);
			}
		| RETURN expr SEMICOLON
			{
				fprintf(yyout,"return expr;\n");
				returnflag = 1;
				emit(retur, (expr*)NULL, (expr*)NULL, $<exprNode>2, -1, yylineno);
			}
		;

%%
int yyerror(char const * yaccProvidedMessage){
	fprintf(stderr, "Parse Error: %s: at line %d, before token: %s\n", yaccProvidedMessage, yylineno, yytext);
	return -1;
}


int main (int argc, char** argv) {
	if (argc == 1) {
		//no arguments
		yyin = stdin;
		yyout = stdout;
	}
	else if (argc == 2) {
		//INPUT_FILE
		if ((yyin = fopen(argv[1], "r")) == NULL) {
			fprintf(stderr, "Unable to open the source file\nProgram Exit...\n");
			return -1;
		}
		yyout = stdout;
	}
	else if (argc == 3) {
		//file output
		if ((yyin = fopen(argv[1], "r")) == NULL) {
			fprintf(stderr, "Unable to open the source file\nProgram Exit...\n");
			return -1;
		}
		if ((yyout = fopen(argv[2], "w")) == NULL) {
			fprintf(stderr, "Error to open the destination file\nProgram Exit...\n");
			return -1;
		}
	}
	initialize_system();
	init_breakcontlist();

	yyparse();
	if (scope != 0){
		SyntaxError_insert(strdup("}"), strdup("is missing"), yylineno);
	}

	print_symtable_and_errors();

	print_intermiediate_code();


	generate_targetcode(get_total_globals());
	print_targetcode();
	print_const_tables();
	avm_binaryfile();
	avm_normalfile();


	SemanticError_print();
	printf("\nDONE BB\n");
	return 0;
}
