%{

#include "header.h"
#include "stack.h"

int inloop=0;                                         //we are in a loop for, while
int infunction=0;                                     //we are in a function
int scope=0;                                          //global variable that show us the scope we are
void yyerror(const char *yaccProvidedMessage);
extern int  yylineno;
extern char *yytext;
extern FILE* yyin;
int yylex(void);
struct function_var_list * listOfFunctionVars=NULL, * end_listOfFunctionVars = NULL;

stack_t * scopeoffsetstack;
stack_t * breakContstack;
stack_t * inloopStack ;

struct SymbolTableEntry * LastFunctionInsertedToSymTable = NULL;

struct stmtNode * breakContList=NULL;
%}

// Symbols.
%union {
int intVal;
char *strVal;
float floatVal;
unsigned unsignedVal;
struct SymbolTableEntry * symVal;
struct expr * exprVal;
struct methodcall_t * methVal;
struct listOfExpression  * elistVal;
struct forprefixNode * forprefixNodeVal;
}

%token <strVal> ASSIGN OR AND  EQ N_EQ B_T BE_T L_T LE_T PLUS MINUS MUL DIV MOD NOT PP MM UMINUS F FF L_B R_B L_P R_P BREAK SEMICOLON CONTINUE A_N_D O_R
%token <strVal> LOCAL CATHOL COMMA UP_DOWN_DOT ENDBLOCK BLOCK FUNCTION IDENTIFIER  STRING NIL TRUE FALSE  IF ELSE FOR WHILE RETURN
%token <intVal> INTEGER
%token <floatVal> REAL

%type <symVal> funcdef funcprefix
%type <unsignedVal> funcbody ifprefix elseprefix whilestart whilecond
%type <strVal> funcname
%type <methVal> methodcall normcall callsuffix
%type <exprVal> lvalue   indexedelem indexed tablemake const call  tableitem expression term assignexpr primary member
%type <elistVal> elist e_list
%type <unsignedVal>  N M
%type <forprefixNodeVal> forprefix


%left ASSIGN
%left OR O_R
%left AND A_N_D
%nonassoc EQ N_EQ
%nonassoc B_T BE_T L_T LE_T
%left PLUS MINUS
%left MUL DIV MOD
%right NOT PP MM UMINUS
%left F FF
%left L_B R_B
%left L_P R_P
%nonassoc THEN
%nonassoc ELSE
%error-verbose
%start program

%%


program : {printf("program\n");}
   |  stmt program {printf("stmt program\n");}
   ;


stmt: expression SEMICOLON {
          //  resettemp();
            printf(" expression;\n");

        }
    |ifstmt {
          //  resettemp();
            printf("ifstmt\n");
        }
    |whilestmt {
           // resettemp();
            printf("whilestmt\n");
        }
    |forstmt {
           // resettemp();
            printf("forstmt\n");
        }
    |returnstmt {
            Manage_stmt_returnstmt(yylineno,infunction);
           // resettemp();
        }
    |BREAK SEMICOLON {
            Manage_stmt_BREAK_SEMICOLON(yylineno,inloop);
        }
    |CONTINUE SEMICOLON {
            Manage_stmt_CONTINUE_SEMICOLON(yylineno,inloop);
        }
    |block {
          //  resettemp();
            printf("block\n");
        }
    |funcdef {
         //   resettemp();
            printf("funcdef\n");
        }
    |SEMICOLON {
          //  resettemp();
            printf(";\n");
        }
    ;


expression: assignexpr {
            printf("assignexpr\n");
            $$=$1;
        }
    |expression PLUS expression {
            printf("expression + expression\n");
            $$= newexpr(arithexpr_e);
            $$->sym = newtemp(scope , yylineno , currscopespace() , currscopeoffset());
            emit(add, $1 , $3,$$, -1,yylineno);
        }
    |expression MINUS expression {printf("expression - expression\n");
            $$= newexpr(arithexpr_e);
            $$->sym = newtemp(scope , yylineno , currscopespace() , currscopeoffset());
            emit(sub, $1 , $3,$$, -1,yylineno);
        }
    |expression MUL expression {printf("expression * expression\n");
            $$= newexpr(arithexpr_e);
            $$->sym = newtemp(scope , yylineno , currscopespace() , currscopeoffset());
            emit(mul, $1 , $3,$$, -1,yylineno);
        }
    |expression DIV expression {printf("expression / expression\n");
            $$= newexpr(arithexpr_e);
            $$->sym = newtemp(scope , yylineno , currscopespace() , currscopeoffset());
            emit(divide, $1 , $3,$$, -1,yylineno);
        }
    |expression MOD expression {printf("expression %% expression\n");
            $$= newexpr(arithexpr_e);
            $$->sym = newtemp(scope , yylineno , currscopespace() , currscopeoffset());
            emit(mod, $1 , $3,$$, -1,yylineno);
        }
    |expression B_T expression {printf("expression > expression\n");
            $$ = newexpr(boolexpr_e);
            $$->sym = newtemp(scope , yylineno , currscopespace() , currscopeoffset());
            emit(if_greater, $1 , $3,NULL, nextquadlabel()+3, yylineno);
            emit(assign, newexpr_constbool(false),NULL ,$$,-1,yylineno);
            emit(jump,NULL,NULL,NULL, nextquadlabel()+2,yylineno);
            emit(assign, newexpr_constbool(true),NULL, $$,-1,yylineno);
        }
    |expression BE_T expression {printf("expression >= expression\n");
            $$ = newexpr(boolexpr_e);
            $$->sym = newtemp(scope , yylineno , currscopespace() , currscopeoffset());
            emit(if_greatereq, $1 , $3,NULL, nextquadlabel()+3, yylineno);
            emit(assign, newexpr_constbool(false),NULL, $$,-1,yylineno);
            emit(jump,NULL,NULL,NULL, nextquadlabel()+2,yylineno);
            emit(assign, newexpr_constbool(true),NULL, $$,-1,yylineno);

        }
    |expression L_T expression {printf("expression < expression\n");
            $$ = newexpr(boolexpr_e);
            $$->sym = newtemp(scope , yylineno , currscopespace() , currscopeoffset());
            emit(if_less, $1 , $3,NULL, nextquadlabel()+3, yylineno);
            emit(assign, newexpr_constbool(false),NULL, $$,-1,yylineno);
            emit(jump,NULL,NULL,NULL, nextquadlabel()+2,yylineno);
            emit(assign, newexpr_constbool(true),NULL, $$,-1,yylineno);
        }
    |expression LE_T expression {printf("expression <= expression\n");
            $$ = newexpr(boolexpr_e);
            $$->sym = newtemp(scope , yylineno , currscopespace() , currscopeoffset());
            emit(if_lesseq, $1 , $3,NULL, nextquadlabel()+3, yylineno);
            emit(assign, newexpr_constbool(false),NULL, $$,-1,yylineno);
            emit(jump,NULL,NULL,NULL, nextquadlabel()+2,yylineno);
            emit(assign, newexpr_constbool(true),NULL, $$,-1,yylineno);
        }
    |expression EQ expression {printf("expression == expression\n");
            $$ = newexpr(boolexpr_e);
            $$->sym = newtemp(scope , yylineno , currscopespace() , currscopeoffset());
            emit(if_eq, $1 , $3,NULL, nextquadlabel()+3, yylineno);
            emit(assign, newexpr_constbool(false),NULL, $$,-1,yylineno);
            emit(jump,NULL,NULL,NULL, nextquadlabel()+2,yylineno);
            emit(assign, newexpr_constbool(true),NULL, $$,-1,yylineno);
        }

    |expression N_EQ expression {printf("expression != expression\n");
            $$ = newexpr(boolexpr_e);
            $$->sym = newtemp(scope , yylineno , currscopespace() , currscopeoffset());
            emit(if_not_eq, $1 , $3,NULL, nextquadlabel()+3, yylineno);
            emit(assign, newexpr_constbool(false),NULL, $$,-1,yylineno);
            emit(jump,NULL,NULL,NULL, nextquadlabel()+2,yylineno);
            emit(assign, newexpr_constbool(true),NULL, $$,-1,yylineno);
        }
    |expression AND expression {printf("expression and expression\n");
            $$ = newexpr(boolexpr_e);
            $$->sym = newtemp(scope , yylineno , currscopespace() , currscopeoffset());
            emit(and, $1 , $3, $$, -1,yylineno);
        }
    |expression OR expression {printf("expression or expression\n");
            $$ = newexpr(boolexpr_e);
            $$->sym = newtemp(scope , yylineno , currscopespace() , currscopeoffset());
            emit(or, $1 , $3, $$, -1,yylineno);
        }
    |term {
            printf("term\n");
            $$ = $1;
        }
    | expression A_N_D expression {printf("expression boolop expression\n");
            $$ = newexpr(boolexpr_e);
            $$->sym = newtemp(scope , yylineno , currscopespace() , currscopeoffset());
            emit(and, $1 , $3, $$, -1,yylineno);
        }
	 | expression O_R expression {printf("expression boolop expression\n");
            $$ = newexpr(boolexpr_e);
            $$->sym = newtemp(scope , yylineno , currscopespace() , currscopeoffset());
            emit(or, $1 , $3, $$, -1,yylineno);
        }
    ;


term: L_P expression R_P {printf("(expression)\n");
            $$ = $2;
        }
    |MINUS expression %prec UMINUS {printf("-expression\n");
            checkuminus($2,yylineno);
            $$= newexpr(arithexpr_e);
            $$->sym = newtemp(scope , yylineno , currscopespace() , currscopeoffset());
            emit(uminus, $2,NULL, $$,-1, yylineno);
        }
    |NOT expression {printf("not expression\n");
            $$ = newexpr(boolexpr_e);
            $$->sym = newtemp(scope , yylineno , currscopespace() , currscopeoffset());
            emit(not, $2 , NULL, $$, -1,yylineno);
        }
    |PP lvalue { Manage_term_PP_lvalue($2->sym , yylineno);
            if ($2->type == tableitem_e) {
                $$= emit_iftableitem($2, yylineno);
                emit(add, $$, newexpr_consnum(1), $$, -1, yylineno);
                emit(tablesetelem,$2, $2->index, $$, -1, yylineno);
            }else {
                emit(add, $2, newexpr_consnum(1), $2, -1, yylineno);
                $$= newexpr(arithexpr_e);
                $$->sym= newtemp(scope , yylineno , currscopespace() , currscopeoffset());
                emit(assign, $2,NULL, $$,-1, yylineno);
            }
        }
    |lvalue PP { Manage_term_lvalue_PP($1->sym , yylineno);
            expr *value ;
            $$ = newexpr(var_e);
            $$->sym = newtemp(scope , yylineno , currscopespace() , currscopeoffset());

            if ($1->type==tableitem_e){
                value = emit_iftableitem($1, yylineno);
                emit(assign, value,NULL, $$,-1, yylineno);
                emit(add, value, newexpr_consnum(1),value, -1, yylineno);
                emit( tablesetelem,$1, $1->index, value,-1, yylineno);
            }else {
                emit(assign, $1,NULL, $$, -1, yylineno);
                emit(add, $1, newexpr_consnum(1), $1,-1, yylineno);
            }

        }
    |MM lvalue { Manage_term_MM_lvalue($2->sym , yylineno);
            if ($2->type== tableitem_e) {
                $$= emit_iftableitem($2, yylineno);
                emit(sub, $$, newexpr_consnum(1), $$, -1, yylineno);
                emit(tablesetelem,$2, $2->index, $$, -1, yylineno);
            }else {
                emit(sub, $2, newexpr_consnum(1), $2, -1, yylineno);
                $$= newexpr(arithexpr_e);
                $$->sym= newtemp(scope , yylineno , currscopespace() , currscopeoffset());
                emit(assign, $2,NULL, $$,-1, yylineno);
            }
        }
    |lvalue MM { Manage_term_lvalue_MM($1->sym , yylineno);
            expr *value ;
            $$ = newexpr(var_e);
            $$->sym = newtemp(scope , yylineno , currscopespace() , currscopeoffset());

            if ($1->type==tableitem_e){
                value = emit_iftableitem($1, yylineno);
                emit(assign, value,NULL, $$,-1, yylineno);
                emit(sub, value, newexpr_consnum(1),value, -1, yylineno);
                emit( tablesetelem,$1, $1->index, value,-1, yylineno);
            }else {
                emit(assign, $1,NULL, $$, -1, yylineno);
                emit(sub, $1, newexpr_consnum(1), $1,-1, yylineno);
            }
        }
    |primary {printf("primary\n");
        $$ = $1;
    }
    ;


assignexpr: lvalue ASSIGN expression {
        Manage_assignexpr_lvalue_ASSIGN_expr($1->sym , yylineno);

        if ($1->type == tableitem_e) {
            emit(tablesetelem,$1,$1->index,$3, -1, yylineno);
            $$ = emit_iftableitem ($1,yylineno);
            $$->type = assignexpr_e;
        }else {
            emit(assign,$3,NULL,$1, -1, yylineno);
            $$= newexpr(assignexpr_e);
            $$->sym = newtemp(scope , yylineno , currscopespace(), currscopeoffset());
            emit(assign, $1, NULL,$$, -1, yylineno);
        }

    }
    ;

primary: lvalue {printf("lvalue\n");
            $$  = emit_iftableitem($1 , yylineno);
        }
    |call {printf("call\n");
            $$ = $1;
        }
    |tablemake {printf("tablemake\n");
            $$ = $1;
        }
    |L_P funcdef R_P  {printf("(funcdef)\n");
            $$ = newexpr(programfunc_e);
            $$->sym = $2;
        }
    |const {printf("const\n");
            $$ = $1;
        }
    ;


lvalue: IDENTIFIER {
            SymbolTableEntry * Sym= Manage_lvalue_IDENTIFIER(scope, yytext, yylineno,currscopespace(),currscopeoffset());
            $$= lvalue_expr(Sym);
        }
    |LOCAL IDENTIFIER {
            SymbolTableEntry * Sym =Manage_lvalue_LOCAL_IDENTIFIER(scope, yytext, yylineno,currscopespace(),currscopeoffset());
            $$= lvalue_expr(Sym);
        }
    |CATHOL IDENTIFIER {
            SymbolTableEntry * Sym = Manage_lvalue_global_IDENTIFIER( yytext, yylineno);
            $$= lvalue_expr(Sym);
        }
    |member {
            $$ = $1;
            printf("member\n");
        }
    ;


tableitem : lvalue F IDENTIFIER {
            printf("lvalue.id \n");
            $$ = member_item($1, $3, yylineno);
        }
    |lvalue L_B expression R_B {
        printf("lvalue[expression]\n");
        $1  = emit_iftableitem($1 , yylineno);
        $$  = newexpr(tableitem_e);
        $$->sym = $1->sym ;
        $$->index = $3;
    };


member: tableitem{ $$ = $1;}
    |call F IDENTIFIER {
            printf("call.id \n");
            $$ = member_item($1, $3, yylineno);

        }
    |call L_B expression R_B {
            printf("call[expression]\n");
            $1  = emit_iftableitem($1 , yylineno);
            $$  = newexpr(tableitem_e);
            $$->sym = $1->sym ;
            $$->index = $3;
        }
    ;


call: call L_P elist R_P {printf("call (elist)\n");
            $$= make_call($1 , $3->start , yylineno);
        }
    |lvalue callsuffix {
            printf("lvalue callsuffix\n");
		    expr * self;
            if($2->method){
                self = $1;
                $1 = emit_iftableitem(member_item(self , $2->name , yylineno),yylineno);
                self->next = $2->elist;
                $2->elist = self;
            }

            $$ = make_call($1 , $2->elist , yylineno);
        }
    |L_P funcdef R_P L_P elist R_P {
            printf("(funcdef) (elist)\n");
            expr * func ;
            func = newexpr(programfunc_e);
            func->sym = $2 ;
            $$ = make_call(func , $5->start , yylineno);
        }
    ;

callsuffix: normcall {
            printf("normacall\n");
            $$ = $1;
        }
    | methodcall {
            printf("methodcall\n");
            $$ = $1;
        }
    ;

normcall: L_P elist R_P {
        printf("(elist)\n");

        $$ = (struct methodcall_t *)malloc(sizeof(struct methodcall_t));
        $$->elist = $2->start;
        $$->method = false;
        $$->name = NULL;
    }
    ;

methodcall: FF IDENTIFIER L_P elist R_P {
        printf("..id  (elist)\n");

        $$ = (struct methodcall_t *)malloc(sizeof(struct methodcall_t));
        $$->elist = $4->start;
        $$->method = true;
        $$->name = $2;
    }
    ;

elist:  {
            printf("elist : NULL\n");

            $$ = (struct listOfExpression *)malloc(sizeof(struct listOfExpression));
            $$->start = NULL;
            $$->end = NULL ;

        }
    |expression e_list{
            printf("elist : expression e_list\n");

            $1->next = $2->start;
            $2->start = $1;
            $$ = $2;
        }
    ;


e_list:{
        printf("e_list : NULL\n");

	    $$ = (struct listOfExpression *)malloc(sizeof(struct listOfExpression));
        $$->start = NULL ;
	    $$->end = NULL ;
	}
    | COMMA expression e_list {
        printf("e_list : COMMA expression e_list\n");

		$2->next = $3->start;
		$3->start = $2;
        $$ = $3;

        }
    ;



tablemake :L_B elist  R_B {
            int i = 0 ;
            printf("tablemake [elist]\n");

            expr * t = newexpr(newtable_e);
            t->sym = newtemp(currScope(),yylineno , currscopespace(), currscopeoffset());
            emit(tablecreate , NULL , NULL , t , -1 , yylineno);

            while($2->start){
                emit(tablesetelem , t,newexpr_consnum(i++), $2->start ,-1 , yylineno);
                $2->start = $2->start->next ;
            }
            $$ = t;
        }
    | L_B indexed R_B {
            int counter , i ;

            printf("tablemake [indexed]\n");

            expr * t = newexpr(newtable_e) , *tmp;
            t->sym = newtemp(currScope(),yylineno , currscopespace(), currscopeoffset());
            emit(tablecreate , NULL , NULL , t , -1 , yylineno);

            tmp = $2;
            counter=0;

            while(tmp){
                counter++;
                tmp = tmp->next;
            }

            while(counter){
                tmp = $2;

                for(i=0 ; i<counter-1; i++){
                    tmp = tmp->next;
                }
                emit(tablesetelem , t ,tmp , tmp->index , -1 , yylineno);
                counter--;
            }

            $$ = t;
        }
    ;

indexed:indexedelem {
            printf("indexed  : indexdelem\n");

            $$=$1;

        }
   |indexed COMMA indexedelem {
            printf("indexed, indexdelem\n");

            $3->next= $1;
            $$ = $3;

        }
    ;


indexedelem: BLOCK expression UP_DOWN_DOT expression ENDBLOCK {
            printf("{expression:expression}\n");
            $2->index = $4;
            $$=$2;
        }
    ;

block: BLOCK {scope++;} stmt1 ENDBLOCK {
        printf("{stmt*}\n");
        SymbolTableHide(scope--);}
    ;

stmt1: {;}
    |stmt stmt1
    ;


funcname: {
		char* function_name;
    	function_name=(char*)malloc(14);
        function_name[0]='\0';
        strncpy(function_name , "$_ANONYMOUS_$" ,13 );
    	function_name[13]='\0';
		$$ = function_name;

		printf("funcname ANONYMOUS\n");

    }
    |IDENTIFIER {
		char* function_name;
        function_name=(char*)malloc(strlen(yytext)+1);
        function_name[0]='\0';
        strcpy(function_name , yytext);
		$$ = function_name;

		printf("funcname %s\n",yytext);

    }
    ;


funcprefix: FUNCTION funcname {
    $$ = Manage_funcprefix_FUNCTION_funcname($2, yylineno , currScope());
    LastFunctionInsertedToSymTable = $$;
    $$->value.funcVal->iaddress = nextquadlabel();

    emit(funcstart , NULL , NULL , lvalue_expr($$), -1 , yylineno);

    push(scopeoffsetstack, currscopeoffset() ,NULL);
    push( inloopStack , inloop , NULL);
    inloop = 0 ;


    enterscopespace();
    resetformalargoffset();

    printf("funcprefix\n");

    }
    ;

funcargs: L_P { infunction++; scope++;}  idlist R_P{
    Manage_funcargs_LP_idlist_RP(LastFunctionInsertedToSymTable);
    LastFunctionInsertedToSymTable = NULL;

    scope--;
    enterscopespace();
    resetfunctionlocaloffset();

    printf("funcargs\n");

	}
    ;


funcbody: block{
    infunction--;
    $$ = currscopeoffset();
    exitscopespace();
    printf("funcbody: block\n");

    }
    ;


funcdef: funcprefix  funcargs funcbody {
    int oldOffset;

    exitscopespace();
    $1->value.funcVal->totallocals = $3;

    oldOffset = pop(scopeoffsetstack);
    inloop = pop(inloopStack);

    restorecurrscopeoffset(oldOffset);

    $$ = $1;
    emit(funcend , NULL , NULL , lvalue_expr($1), -1 , yylineno);

    printf("funcdef\n");

	}
    ;


const:INTEGER {
            printf("number %d\n",atoi(yytext));
            $$ = newexpr_consnum($1);
        }
    |STRING {
            printf("string \n");
            $$ = newexpr_conststring($1);
            printf("string = %s \n",$1);
        }
    |NIL {
            printf("nil\n");
            $$ = newexpr(nil_e);
        }
    |TRUE {
            printf("true\n");
            $$ = newexpr_constbool(true);
        }
    |FALSE {
            printf("false\n");
            $$ = newexpr_constbool(false);
        }
    |REAL {
            printf("number %f\n",atof(yytext));
            $$ = newexpr_consnum($1);
        }
    ;

idlist:{;}
    |IDENTIFIER {Manage_idlist_IDENTIFIER(yytext , yylineno , scope);}
    |idlist COMMA IDENTIFIER {Manage_idlist_IDENTIFIER(yytext , yylineno , scope); printf("id , id \n");}
    ;



ifprefix : IF L_P expression R_P {
        emit(if_eq , $3 , newexpr_constbool(true), NULL ,nextquadlabel()+2, yylineno);
        $$ = nextquadlabel();
        emit(jump , NULL , NULL , NULL , -2 , yylineno);
    }
    ;

elseprefix : ELSE {
        $$ = nextquadlabel();
        emit(jump , NULL , NULL , NULL , -2 , yylineno);
    }
    ;
ifstmt: ifprefix stmt elseprefix stmt {
        printf("if(expression) stmt else stmt\n");
        patchlabel($1 , $3+1);
        patchlabel($3 , nextquadlabel());
    }
    |ifprefix stmt   %prec THEN  {
        printf("if(expression) stmt\n");
        patchlabel($1 , nextquadlabel());
    }
    ;


loopstart : { ++inloop ;};

loopend : { --inloop ;};

loopstmt : loopstart stmt loopend {
        printf("loopstmt\n");
    }
    ;

whilestart : WHILE {

        push(breakContstack , 0 , breakContList);
        breakContList = NULL ;

        breakContList = (struct stmtNode *)malloc(sizeof(struct stmtNode ));
        breakContList->breaklist = NULL ;
        breakContList->continuelist = NULL ;
        $$ = nextquadlabel();
        printf("whilestart\n");
    }
    ;

whilecond : L_P expression R_P {
        emit(if_eq , $2 , newexpr_constbool(true), NULL , nextquadlabel()+2 , yylineno);
        $$ = nextquadlabel();
        emit(jump , NULL , NULL , NULL , -2 , yylineno);
        printf("whilecond\n");
    }
    ;

whilestmt: whilestart  whilecond loopstmt {
        printf("while(expression) stmt \n");
        emit(jump , NULL , NULL , NULL , $1 , yylineno);
        patchlabel($2 ,nextquadlabel());

        while(breakContList->breaklist){
            patchlabel( breakContList->breaklist->quadPossition , nextquadlabel());
            breakContList->breaklist = breakContList->breaklist->next;
        }

        while(breakContList->continuelist){
            patchlabel(breakContList->continuelist->quadPossition , $1);
            breakContList->continuelist = breakContList->continuelist->next;
        }

        breakContList = NULL;
        breakContList = (struct stmtNode *) pop_list(breakContstack);
    }
    ;

N : {
        $$ = nextquadlabel();
        emit(jump , NULL , NULL , NULL , -2 , yylineno);
    }
    ;
M : {
        $$ = nextquadlabel();
    }
    ;

forprefix :FOR L_P elist SEMICOLON  M expression SEMICOLON {

        push(breakContstack , 0 , breakContList);
        breakContList = NULL ;

        breakContList = (struct stmtNode *)malloc(sizeof(struct stmtNode ));
        breakContList->breaklist = NULL ;
        breakContList->continuelist = NULL ;

        $$ = (struct forprefixNode *)malloc(sizeof(struct forprefixNode));
        $$->test = $5;
        $$->enter = nextquadlabel();
        emit(if_eq , $6 , newexpr_constbool(true) , NULL , -2 , yylineno);
    };


forstmt: forprefix N elist R_P N loopstmt N{
        printf("for(elist; expression; elist) stmt\n");
        patchlabel($1->enter , $5+1);
        patchlabel($2 , nextquadlabel());
        patchlabel($5 , $1->test);
        patchlabel($7 , $2+1);


        while(breakContList->breaklist){
            patchlabel( breakContList->breaklist->quadPossition , nextquadlabel());
            breakContList->breaklist = breakContList->breaklist->next;
        }

        while(breakContList->continuelist){
            patchlabel(breakContList->continuelist->quadPossition , $2+1);
            breakContList->continuelist = breakContList->continuelist->next;
        }

        breakContList = NULL;
        breakContList = (struct stmtNode *) pop_list(breakContstack);
    }
    ;

returnstmt: RETURN expression SEMICOLON { printf("return expression;\n");
            emit(ret , NULL , NULL , $2 , -1 , yylineno);
        }
    |RETURN SEMICOLON { printf("return;\n");
            emit(ret , NULL , NULL , NULL , -1 , yylineno);
        }
    ;

%%

int currScope(){
    return scope;
}


void yyerror(const char* yaccProvidedMessage){
    fprintf(stderr, "\nline %d :%s\n",yylineno, yaccProvidedMessage);
}

int main(int argc, char **argv){
	if(argc <2 ){
		fprintf(stderr,"\n No  input file to parse!!!\n");
		return 1;
	}

	if( !(yyin = fopen( argv[1], "r" )) ){
		fprintf(stderr,"\nCannot open file : %s\n",argv[1]);
		return 1;
	}

	scopeoffsetstack =  MakeEmptyStack();
	breakContstack = MakeEmptyStack() ;
    inloopStack = MakeEmptyStack();


    SymbolTable_scopeList_Initialize();

    yyparse();

    SymbolTablePrintAll();
    ScopePrintAll();

	PrintQuads();
	//PrintIcode();

    stackInitialize();
    generateTargetCode();

    PrintIcode();
    PrintBinaryCode();
    PrintTargetCode();

    fclose(yyin);

	return 0 ;

}
