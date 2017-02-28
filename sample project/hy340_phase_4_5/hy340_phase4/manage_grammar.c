#include "header.h"


SymbolTableEntry *  Manage_lvalue_LOCAL_IDENTIFIER(int scope, char *name, int line , enum scopespace_t scopeSpace ,unsigned  scopeOffset){

    SymbolTableEntry * tmp_local_SymbolTableEntry , *tmp;
    tmp_local_SymbolTableEntry=SymbolTableLookUp_THIS(name, scope);
    if(tmp_local_SymbolTableEntry==NULL)
    {

        if(CheckForLibraryNameCollision(name)==NULL){
            printf("local id \n");
            tmp = SymbolTableInsert(name ,scope , line ,LOCAL_T ,var_s , scopeSpace , scopeOffset);
            inccurrscopeoffset();
            return tmp;
        }
        else printf("Syntax Error in line %d :Reference to local %s trying to shadow  library function %s\n", line, name, name);

    }
    printf("local id \n");
    return tmp_local_SymbolTableEntry;
}


SymbolTableEntry *  Manage_lvalue_global_IDENTIFIER( char *name, int line){
    SymbolTableEntry * tmp_local_SymbolTableEntry;
    tmp_local_SymbolTableEntry=SymbolTableLookUp_THIS(name, 0);
    if(tmp_local_SymbolTableEntry==NULL)
        printf("Syntax Error in line %d : No global  %s\n", line,name);

    printf("::id \n");
    return tmp_local_SymbolTableEntry;
}

SymbolTableEntry *  Manage_lvalue_IDENTIFIER(int scope, char *name, int line, enum scopespace_t scopeSpace ,unsigned  scopeOffset){
    int scope_var ;
    SymbolTableEntry * tmp_local_SymbolTableEntry , *tmp;
    tmp_local_SymbolTableEntry=SymbolTableLookUp_ALL_FROM_THIS(name ,scope);
    if(tmp_local_SymbolTableEntry==NULL){
        printf("id \n");
        tmp = SymbolTableInsert(name ,scope , line ,LOCAL_T ,var_s , scopeSpace , scopeOffset);
        inccurrscopeoffset();
        return tmp;
    }else{
        if(lookUpAllUserFunctFound == 1){
            if((tmp_local_SymbolTableEntry->type == GLOBAL)||(tmp_local_SymbolTableEntry->type == LOCAL_T)||(tmp_local_SymbolTableEntry->type == FORMAL)){
                scope_var = tmp_local_SymbolTableEntry->value.varVal->scope;
                //8a mpoume edw mono sthn periptwsh pou 8a exoume metablhth
                if((scope_var != 0)&&(scope_var!=scope)){
                    printf("Syntax Error in line %d : Cannot access %s in scope %d\n",line,name,scope_var);
                }
	    }
        }

     }

    printf("id \n");
    return tmp_local_SymbolTableEntry;
}


void Manage_stmt_returnstmt(int line , int infunction){
    if(infunction==0)
        printf("Syntax error in line %d.  Use of 'return' while not in a function\n",line);

    printf("returnstmt\n");

    return ;
}

void Manage_stmt_BREAK_SEMICOLON(int line , int inloop){
    struct node * nodeVal;

    if(inloop==0)
        printf("Syntax error in line %d.  Use of 'break' while not in a loop\n",line);

    nodeVal = (struct node *)malloc(sizeof(struct node));
    nodeVal->quadPossition = nextquadlabel();
    nodeVal->next = NULL ;

    nodeVal->next = breakContList->breaklist;
    breakContList->breaklist = nodeVal;

    emit(jump , NULL ,NULL , NULL , -2 , line);

    //resettemp();

    printf("break;\n");

    return ;
}


void Manage_stmt_CONTINUE_SEMICOLON(int line , int inloop){

    if(inloop==0)
        printf("Syntax error in line %d.  Use of 'continue' while not in a loop\n",line);


    struct node * nodeVal;

    nodeVal = (struct node *)malloc(sizeof(struct node));
    nodeVal->quadPossition = nextquadlabel();
    nodeVal->next = NULL ;

    nodeVal->next = breakContList->continuelist;
    breakContList->continuelist = nodeVal;

    emit(jump , NULL ,NULL , NULL , -2 , line);

    //resettemp();

    printf("continue;\n");

    return ;
}



void Manage_term_PP_lvalue(struct SymbolTableEntry * exprNode , int line ){

    if(exprNode!=NULL){
        if((exprNode->type == USERFUNC)||(exprNode->type == LIBFUNC)){
            printf("Syntax Error in line %d : Trying ++ to FUNCTION %s \n",line,((exprNode->type==3 /*USERFUNC*/)?"USERFUNC":"LIBFUNC"));
        }
    }

    printf("lvalue++\n");

    return ;
}

void Manage_term_lvalue_PP(struct SymbolTableEntry * exprNode , int line){

    if(exprNode!=NULL){
        if((exprNode->type == USERFUNC)||(exprNode->type == LIBFUNC)){
            printf("Syntax Error in line %d : Trying ++ to FUNCTION %s \n",line,((exprNode->type==3 /*USERFUNC*/)?"USERFUNC":"LIBFUNC"));
        }
    }

    printf("++lvalue\n");

    return ;
}


void Manage_term_lvalue_MM(struct SymbolTableEntry * exprNode , int line){

    if(exprNode!=NULL){
        if((exprNode->type == USERFUNC)||(exprNode->type == LIBFUNC)){
            printf("Syntax Error in line %d : Trying -- to FUNCTION %s \n",line,((exprNode->type==3 /*USERFUNC*/)?"USERFUNC":"LIBFUNC"));
        }
    }

    printf("lvalue--\n");

    return ;
}

void Manage_term_MM_lvalue(struct SymbolTableEntry * exprNode , int line){

    if(exprNode!=NULL){
        if((exprNode->type == USERFUNC)||(exprNode->type == LIBFUNC)){
            printf("Syntax Error in line %d : Trying -- to FUNCTION %s \n",line,((exprNode->type==3 /*USERFUNC*/)?"USERFUNC":"LIBFUNC"));
        }
    }

    printf("--lvalue\n");

    return ;
}

void Manage_assignexpr_lvalue_ASSIGN_expr(struct SymbolTableEntry * exprNode , int line){

    if(exprNode!=NULL){
        if((exprNode->type == USERFUNC)||(exprNode->type == LIBFUNC)){
            printf("Syntax Error in line %d : Trying ASSIGN to FUNCTION %s \n",line,((exprNode->type==3 /*USERFUNC*/)?"USERFUNC":"LIBFUNC"));
        }
    }

    printf("lvalue=expr\n");

    return ;
}


void  Manage_idlist_IDENTIFIER(char * id, int line , int scope){
    struct function_var_list * newNode;
    struct Variable * newVar;

    newNode = (struct function_var_list *)malloc(sizeof(struct function_var_list));
    newVar = ( struct Variable *)malloc(sizeof( struct Variable));

    newVar->name = (char *)malloc(strlen(id)+1);
    strcpy(newVar->name , id);
    newVar->scope = scope;
    newVar->line = line;

    newNode->variable = newVar;

    if(listOfFunctionVars == NULL){
        listOfFunctionVars = newNode;
        end_listOfFunctionVars = newNode;
    }else{
        end_listOfFunctionVars->next = newNode;
        end_listOfFunctionVars = newNode ;
    }
	printf("FORMAL : %s\n",newVar->name);
    printf("id \n");
    return;
}



SymbolTableEntry * Manage_funcprefix_FUNCTION_funcname(char * function_name , int function_line , int function_scope){
    SymbolTableEntry * tmp_SymbolTableEntry = NULL;
    enum SymbolTableType type;

    int flag_Library_function = 0;

    tmp_SymbolTableEntry = SymbolTableLookUp_THIS(function_name  , function_scope );

    if(CheckForLibraryNameCollision(function_name)!=NULL){
        type = LIBFUNC ;
        flag_Library_function = 1 ;
    }


    if((tmp_SymbolTableEntry != NULL)||(flag_Library_function == 1)){

	if(flag_Library_function == 0){
           type = tmp_SymbolTableEntry->type;
	}

        if(type == LIBFUNC){
            printf("Syntax Error in line %d : Shadows library function %s\n",function_line,function_name);
        }else if(type == USERFUNC){
            printf("Syntax Error in line %d : Redeclaration of function %s\n",function_line,function_name);
        }else{
            printf("Syntax Error in line %d : %s is a variable\n",function_line,function_name);
        }
    }

    tmp_SymbolTableEntry = SymbolTableInsert(function_name , function_scope , function_line , USERFUNC , programfunc_s ,0,-1);

    return tmp_SymbolTableEntry;
}

SymbolTableEntry * Manage_funcargs_LP_idlist_RP(SymbolTableEntry * function){
    SymbolTableEntry * tmp_SymbolTableEntry_1 = NULL;
    struct function_var_list * tmp_listOfFunctionVars=NULL , *prev_listOfFunctionVars=NULL , *tmp;
    int flag_error = 0;

    prev_listOfFunctionVars = NULL;
    tmp_listOfFunctionVars = listOfFunctionVars;

    listOfFunctionVars = NULL;
    end_listOfFunctionVars = NULL;

    while(tmp_listOfFunctionVars!=NULL){
        flag_error = 0 ;

        tmp_SymbolTableEntry_1 = SymbolTableLookUp_THIS(tmp_listOfFunctionVars->variable->name  , tmp_listOfFunctionVars->variable->scope );

        if(tmp_SymbolTableEntry_1 != NULL){
            printf("Syntax Error in line %d : Formal redeclaration %s \n",tmp_listOfFunctionVars->variable->line ,tmp_listOfFunctionVars->variable->name);
            flag_error = 1;
        }

        if(CheckForLibraryNameCollision(tmp_listOfFunctionVars->variable->name)!=NULL){
            printf("Syntax Error in line %d : Formal shadows library %s \n",tmp_listOfFunctionVars->variable->line ,tmp_listOfFunctionVars->variable->name);
            flag_error = 1;
        }

        if(flag_error == 0){
            if(listOfFunctionVars == NULL){
                listOfFunctionVars = tmp_listOfFunctionVars;
                end_listOfFunctionVars = tmp_listOfFunctionVars;
            }else{
                end_listOfFunctionVars->next = tmp_listOfFunctionVars;
                end_listOfFunctionVars = tmp_listOfFunctionVars ;
            }
	    tmp_listOfFunctionVars->variable->scopeOffset = currscopeoffset();
	    tmp_listOfFunctionVars->variable->scopeSpace = currscopespace();
            SymbolTableInsert(tmp_listOfFunctionVars->variable->name , tmp_listOfFunctionVars->variable->scope , tmp_listOfFunctionVars->variable->line , FORMAL ,var_s, currscopespace() ,currscopeoffset());
	    inccurrscopeoffset( );
        }

        prev_listOfFunctionVars = tmp_listOfFunctionVars;
        tmp_listOfFunctionVars = tmp_listOfFunctionVars->next;
        prev_listOfFunctionVars->next = NULL ;
    }


    function->value.funcVal->head = listOfFunctionVars;
    tmp = function->value.funcVal->head;
                    printf("Function Var list\n");
                while(tmp){
                    printf("\t%s\n",tmp->variable->name);
                    tmp = tmp->next;
                }


    listOfFunctionVars=NULL;
    end_listOfFunctionVars=NULL;

    return function;
}
