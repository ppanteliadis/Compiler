#include "header.h"

quad*	quads =NULL;
unsigned total = 0;
unsigned int currQuad = 0;
unsigned int tempcounter=0;
unsigned programVarOffset = 0;
unsigned functionLocalOffset = 0;
unsigned formalArgOffset = 0;
unsigned scopeSpaceCounter = 1;
unsigned int G_syntaxError = 0 ;

char * expr_tArray[] = {
    "var_e",
    "tableitem_e",
    "programfunc_e",
    "libraryfunc_e",
    "arithexpr_e",
    "boolexpr_e",
    "assignexpr_e",
    "newtable_e",
    "constnum_e",
    "constbool_e",
    "conststring_e",
    "nil_e"
};

char *  scopespace_tArray[] = {
    "programvar",
    "formalarg",
    "functionlocal"
};

char *  symbol_tArray[] = {
    "var_s",
    "programfunc_s",
    "libraryfunc_s"
};


char * opcodeArray[] = {
    "ADD",
    "SUB",
    "MUL",
    "DIV",
    "MOD",
    "UMINUS",
    "TABLECREATE",
    "TABLEGETELEM",
    "TABLESETELEM",
    "ASSIGN",
    "JUMP",
    "IF_EQ",
    "IF_NOT_EQ",
    "IF_GREATER",
    "IF_GREATER_EQ",
    "IF_LESS",
    "IF_LESS_EQ",
    "NOT",
    "OR",
    "AND",
    "PARAM",
    "CALL",
    "GETRETVAL",
    "FUNCSTART",
    "RETURN",
    "FUNCEND"
};



//////////////// Temporary Variable - Function Definition ////////////////
char *newtempname(void){
    int n=tempcounter;
    int digits=0;
    char *newname='\0';

    while(n!=0){
        n=n/10;
        ++digits;
    }

    if(digits==0) digits=1;
    newname = (char *)malloc(2+digits+1);
    sprintf(newname , "_t%d" , tempcounter);
    tempcounter++;

    return newname;
}


SymbolTableEntry *newtemp(int scope ,int line ,enum scopespace_t scopeSpace ,unsigned  scopeOffset){
    char *name=newtempname();
    SymbolTableEntry * sym;

    sym=SymbolTableLookUp_Temporary_Variables_At_This_Scope(name,scope);

    if(sym==NULL){
        inccurrscopeoffset();
        return SymbolTableInsert(name , scope , line ,LOCAL_T , var_s , scopeSpace ,  scopeOffset);

    }else{
        return sym;
    }
}

void resettemp(void){
    tempcounter = 0;
}



/////////////////////ScopeSpace - Function Definition ////////////////////

scopespace_t currscopespace( void ){
    if( scopeSpaceCounter == 1){
        return programvar;
    }
    else if( scopeSpaceCounter % 2 == 0){
        return formalarg;
    }
    else{
        return functionlocal;
    }
}

unsigned  currscopeoffset( void ){

    switch( currscopespace() ){
        case programvar	:
            return programVarOffset;
        case functionlocal:
            return functionLocalOffset;
        case formalarg:
            return formalArgOffset;
        default	: assert(0);
    }
}

void inccurrscopeoffset( void ){
    switch( currscopespace() ){
        case programvar	:
            ++programVarOffset;
            break;
        case functionlocal :
            ++functionLocalOffset;
            break;
        case formalarg	:
            ++formalArgOffset;
            break;
        default	:
            assert(0);
    }
}

void enterscopespace( void ){
    ++scopeSpaceCounter;
}

void exitscopespace( void ){
    assert(scopeSpaceCounter > 1);
    --scopeSpaceCounter;
}


////////////////////Offset - Function Definition ///////////////////////

void resetformalargoffset( void ){
    formalArgOffset = 0;
}

void resetfunctionlocaloffset( void ){
    functionLocalOffset = 0;
}

void restorecurrscopeoffset( unsigned n ){
    switch( currscopespace()){
        case programvar	:
            programVarOffset = n;
            break;
        case functionlocal :
            functionLocalOffset = n;
            break;
        case formalarg :
            formalArgOffset = n;
            break;
        default	:
            assert(0);
    }
}



///////////////////Expression - Function Definition ///////////////////

expr *newexpr(expr_t t){
   expr *e = (expr *)malloc(sizeof(expr));
   memset(e,0,sizeof(expr));
   e->type = t;
   return e;
}


expr * newexpr_constbool(bool b){
    expr * e = newexpr(constbool_e);
    e->boolConst = b;
    return e;
}


expr * newexpr_conststring(char * s){
    expr *e= newexpr(conststring_e);
    e->strConst = strdup(s);
    return e;
}


expr * newexpr_consnum(double num){
    expr * e = newexpr(constnum_e);
    e->numConst = num;
    return e;
}

/////////////////////////////////////////////////////////////////////


void expand(void){
    assert(total==currQuad);
    quad* p = (quad*) malloc(NEW_SIZE);

    if (quads){
        memcpy(p, quads, CURR_SIZE);
        free(quads);
    }

    quads = p;
    total += EXPAND_SIZE;
}

void emit(iopcode op ,expr * arg1, expr * arg2, expr * result, unsigned label, unsigned line){

    if(currQuad == total){
        expand();
    }

    quad *p = quads + currQuad++;
    p->op = op;
    p->arg1 = arg1;
    p->arg2 = arg2;
    p->result = result;
    p->label = label;
    p->line = line;
}


//////////////////////////////////////////////////////////////////////

void patchlabel(unsigned quadNo, unsigned  label){
    assert(quadNo<currQuad);
    quads[quadNo].label = label;
}


expr *lvalue_expr(SymbolTableEntry *sym){
    assert(sym);
    expr *e= (expr *)malloc(sizeof(expr));
    memset(e , 0 , sizeof(expr));

    e-> next = NULL;
    e-> sym = sym;

    switch(sym->s_type){
        case programfunc_s:
        e->type = programfunc_e;
      break;
    case libraryfunc_s:
        e->type = libraryfunc_e;
        break;
    case var_s:
        e->type = var_e;
        break;
    default:
        assert(0);
    }
    return e;
}


expr* emit_iftableitem( expr* e, int line){
    expr* result;

    if( e->type != tableitem_e ){
        return e;
    }else{
        result =newexpr( var_e );
        result->sym = newtemp(currScope(), line ,currscopespace(),currscopeoffset() );
        emit( tablegetelem , e , e->index , result, -1 ,line);
        return result;
    }
}


expr *member_item(expr *lvalue,char *name,int line){
      expr *lvalue1;
      expr *item;

      lvalue1 = emit_iftableitem(lvalue,line);
      item = newexpr(tableitem_e);
      item->sym = lvalue1->sym;
      item->index =newexpr_conststring(name);

      return item;
}


expr *make_call(expr *lvalue, expr *elist, unsigned line){
    expr *func;
    expr *result;
    expr *tmp_elist;
    expr **reservedElist = NULL;
    int i,counter=0;

    func = emit_iftableitem(lvalue,line);

    tmp_elist = elist;
    //Trexoume mia fora thn elist gia na metrhsoume to plh8os twn stoixeiwn
    while(tmp_elist){
        tmp_elist = tmp_elist->next;
        counter++;
    }

    reservedElist= (expr **)malloc(sizeof(expr*)*counter);

    //apo8hkeuoume ta arguments se ena pinaka gia an ta paroume
    //sthn sunexeia anapoda
    tmp_elist = elist;
    for(i = 0 ; i < counter ; i++){
        reservedElist[i]= tmp_elist;
        tmp_elist = tmp_elist->next;
    }

    for(i = counter-1; i>=0 ; i--){
        emit(param , NULL , NULL , reservedElist[i], -1 , line);
    }

    emit(call, NULL, NULL,func, -1 , line);
    result = newexpr(var_e);
    result -> sym =  newtemp(currScope(), line ,currscopespace(),currscopeoffset() );
    emit(getretval, NULL, NULL, result,-1, line);

    return result;
}



void checkuminus(expr *e,int lineno){

    if(e->type==constbool_e ||
    e->type==conststring_e ||
    e->type==nil_e ||
    e->type==newtable_e ||
    e->type==programfunc_e ||
    e->type==libraryfunc_e ||
    e->type==boolexpr_e ){
        printf("Suntax error in line %d: Illegal expression to uminary\n", lineno);
        G_syntaxError=1;
    }
}


unsigned  nextquadlabel(void){
    return currQuad;
}

char * newtempfuncname(){
    char * newName = '\0';
    char * numberToString= '\0';
    int n , digits = 0;

    n = numberOfAnonymousFunctions;
    numberOfAnonymousFunctions++;

    while(n!=0){
        n=n/10;
        ++digits;
    }

    if(n == 0){ digits = 1;}

    sprintf(numberToString, "%d", numberOfAnonymousFunctions);
    numberToString[digits]='\0';
    newName = (char*)malloc(2+ digits +1);
    newName[0]='_';
    newName[1]='F';
    newName[2]='\0';
    strcat(newName,numberToString);

    return newName;
}




void PrintQuadsExpression(expr * expression){

    expr_t type = expression->type;
    printf("Expression type : %s\n",expr_tArray[type]);
    if(type == var_e){
        printf("\ts_type  :  %s\n",symbol_tArray[expression->sym->s_type]);
        printf("\tname    :  %s\n",expression->sym->value.varVal->name);
        printf("\tline    :  %u\n",expression->sym->value.varVal->line);
        printf("\tscope   :  %u\n",expression->sym->value.varVal->scope);
        printf("\toffset  :  %u\n",expression->sym->value.varVal->scopeOffset);
        printf("\tSspace  :  %s\n",scopespace_tArray[expression->sym->value.varVal->scopeSpace]);
    }else if(type == tableitem_e){
        printf("\ts_type  :  %s\n",symbol_tArray[expression->sym->s_type]);
        printf("\tname    :  %s\n",expression->sym->value.varVal->name);
        printf("\tline    :  %u\n",expression->sym->value.varVal->line);
        printf("\tscope   :  %u\n",expression->sym->value.varVal->scope);
        printf("\toffset  :  %u\n",expression->sym->value.varVal->scopeOffset);
        printf("\tSspace  :  %s\n",scopespace_tArray[expression->sym->value.varVal->scopeSpace]);
    }else if((type == libraryfunc_e)||(type == programfunc_e)){
        printf("\ts_type  :  %s\n",symbol_tArray[expression->sym->s_type]);
        printf("\tname    :  %s\n",expression->sym->value.funcVal->name);
        printf("\tline    :  %u\n",expression->sym->value.funcVal->line);
        printf("\tscope   :  %u\n",expression->sym->value.funcVal->scope);
        printf("\tiaddr   :  %u\n",expression->sym->value.funcVal->iaddress);
        printf("\tlocals  :  %u\n",expression->sym->value.funcVal->totallocals);
    }else if(type == arithexpr_e){
        printf("\ts_type  :  %s\n",symbol_tArray[expression->sym->s_type]);
        printf("\tname    :  %s\n",expression->sym->value.varVal->name);
        printf("\tline    :  %u\n",expression->sym->value.varVal->line);
        printf("\tscope   :  %u\n",expression->sym->value.varVal->scope);
        printf("\toffset  :  %u\n",expression->sym->value.varVal->scopeOffset);
        printf("\tSspace  :  %s\n",scopespace_tArray[expression->sym->value.varVal->scopeSpace]);
    }else if(type == boolexpr_e){
        printf("\ts_type  :  %s\n",symbol_tArray[expression->sym->s_type]);
        printf("\tname    :  %s\n",expression->sym->value.varVal->name);
        printf("\tline    :  %u\n",expression->sym->value.varVal->line);
        printf("\tscope   :  %u\n",expression->sym->value.varVal->scope);
        printf("\toffset  :  %u\n",expression->sym->value.varVal->scopeOffset);
        printf("\tSspace  :  %s\n",scopespace_tArray[expression->sym->value.varVal->scopeSpace]);
    }else if(type == assignexpr_e){
        printf("\ts_type  :  %s\n",symbol_tArray[expression->sym->s_type]);
        printf("\tname    :  %s\n",expression->sym->value.varVal->name);
        printf("\tline    :  %u\n",expression->sym->value.varVal->line);
        printf("\tscope   :  %u\n",expression->sym->value.varVal->scope);
        printf("\toffset  :  %u\n",expression->sym->value.varVal->scopeOffset);
        printf("\tSspace  :  %s\n",scopespace_tArray[expression->sym->value.varVal->scopeSpace]);
    }else if(type == newtable_e){
        printf("\ts_type  :  %s\n",symbol_tArray[expression->sym->s_type]);
        printf("\tname    :  %s\n",expression->sym->value.varVal->name);
        printf("\tline    :  %u\n",expression->sym->value.varVal->line);
        printf("\tscope   :  %u\n",expression->sym->value.varVal->scope);
        printf("\toffset  :  %u\n",expression->sym->value.varVal->scopeOffset);
        printf("\tSspace  :  %s\n",scopespace_tArray[expression->sym->value.varVal->scopeSpace]);
    }else if(type == constnum_e){
        printf("\t%lf",expression->numConst);
    }else if(type == constbool_e){
        printf("\t%s", expression->boolConst ? "true" : "false");
    }else if(type == conststring_e){
        printf("\t\"%s\"",expression->strConst);
    }else if(type == nil_e){
        printf("\tNULL");
    }else{
        printf("ERROR type NOT match\n");
    }

    return ;
}


void PrintQuads(){
    int i ;
    char * op , * arg1 , * arg2 , *result;
    int opcode;
    function_var_list *tmp;

    printf("_________________________ Quads Array _________________________\n\n");
    for(i = 0 ; i< currQuad ; i++){
            opcode = quads[i].op;

            printf("____________________ quads[  %d  ] _________________________\n",i);
            printf(" .op    =  %s\n",opcodeArray[quads[i].op]);
            printf(" .line  =  %u\n",quads[i].line);

            if((opcode == add)||(opcode == sub )||(opcode == mul)||(opcode == divide)||(opcode == mod)||(opcode == or)||(opcode == and)||(opcode == tablesetelem)||(opcode == tablegetelem)){
                printf("\n .arg1 \n");
                PrintQuadsExpression(quads[i].arg1);
                printf("\n .arg2 \n");
                PrintQuadsExpression(quads[i].arg2);
                printf("\n .result \n");
                PrintQuadsExpression(quads[i].result);
                printf("\n");
            }else if((opcode == uminus)||(opcode == not)){
                printf("\n .arg1 \n");
                PrintQuadsExpression(quads[i].arg1);
                printf("\n .result \n");
                PrintQuadsExpression(quads[i].result);
                printf("\n");
            }else if((opcode == if_eq)||(opcode == if_not_eq )||(opcode == if_lesseq)||(opcode == if_greatereq)||(opcode == if_less)||(opcode == if_greater)){
                printf("\n .arg1 \n");
                PrintQuadsExpression(quads[i].arg1);
                printf("\n .arg2 \n");
                PrintQuadsExpression(quads[i].arg2);
                printf("\n .label \n");
                printf("  %d\n",quads[i].label);
                printf("\n");
            }else if(opcode == jump){
                printf("  %d\n",quads[i].label);
                printf("\n");
            }else if(opcode == assign){
                printf("\n .arg1 \n");
                PrintQuadsExpression(quads[i].arg1);
                printf("\n .result \n");
                PrintQuadsExpression(quads[i].result);
                printf("\n");
            }else if((opcode == funcstart)||(opcode == funcend)){
                printf("\n .result \n");
                PrintQuadsExpression(quads[i].result);

                tmp = quads[i].result->sym->value.funcVal->head;
                printf(" Function Var list\n");
                while(tmp){
                    printf("\t%s , scope : %d , offset : %d , scopeSpace %s\n",tmp->variable->name, tmp->variable->scope , tmp->variable->scopeOffset,  scopespace_tArray[tmp->variable->scopeSpace]);
                    tmp = tmp->next;
                }

	    }else if ((opcode == param)||(opcode == getretval)||(opcode == call)){
                printf("\n .result \n");
                PrintQuadsExpression(quads[i].result);
            }else if ((opcode == ret)||(opcode == tablecreate)){
                printf("\n .result \n");
                if(quads[i].result != NULL){
                        PrintQuadsExpression(quads[i].result);
                }
                printf("\n");
            }else{
                printf("ERROR opcode NOT match %s\n",opcodeArray[quads[i].op]);
            }

    }

    return ;
}


void PrintIcodeExpression(FILE * fp , expr * expression){

    expr_t type = expression->type;

    if(type == var_e){
        fprintf(fp ,"  %6s",expression->sym->value.varVal->name);
    }else if(type == tableitem_e){
        fprintf(fp ,"  %6s",expression->sym->value.varVal->name);
    }else if(type == programfunc_e){
        fprintf(fp ,"  %6s",expression->sym->value.funcVal->name);
    }else if(type == libraryfunc_e){
        fprintf(fp ,"  %6s",expression->sym->value.funcVal->name);
    }else if(type == arithexpr_e){
        fprintf(fp ,"  %6s",expression->sym->value.varVal->name);
    }else if(type == boolexpr_e){
        fprintf(fp ,"  %6s",expression->sym->value.varVal->name);
    }else if(type == assignexpr_e){
        fprintf(fp ,"  %6s",expression->sym->value.varVal->name);
    }else if(type == newtable_e){
        fprintf(fp ,"  %6s",expression->sym->value.varVal->name);
    }else if(type == constnum_e){
        fprintf(fp ,"  %6lf",expression->numConst);
    }else if(type == constbool_e){
        fprintf(fp ,"  %6s", expression->boolConst ? "true" : "false");
    }else if(type == conststring_e){
        fprintf(fp ,"  \"%s\"",expression->strConst);
    }else if(type == nil_e){
        fprintf(fp ,"NULL");
    }else{
        printf("ERROR type NOT match\n");
    }

    return ;
}



void PrintIcode(){
    int i ;
    char * op , * arg1 , * arg2 , *result;
    int opcode;

    FILE * fp;
    fp = fopen("quads.txt","w");

    fprintf(fp,"\n\n====================== I CODE ====================== \n");
    for(i = 0 ; i< currQuad ; i++){
            opcode = quads[i].op;
            if((opcode == add)||(opcode == sub )||(opcode == mul)||(opcode == divide)||(opcode == mod)||(opcode == or)||(opcode == and)||(opcode == tablesetelem)||(opcode == tablegetelem)){
                fprintf(fp," %-3d  :  %-8s    ",i,opcodeArray[quads[i].op] );
                PrintIcodeExpression(fp ,quads[i].arg1);
                PrintIcodeExpression(fp ,quads[i].arg2);
                PrintIcodeExpression(fp ,quads[i].result);
                fprintf(fp,"\t\t\t   : %u",quads[i].taddress);
                fprintf(fp,"\n");
            }else if((opcode == uminus)||(opcode == not)){
                fprintf(fp," %-3d  :  %-8s    ",i,opcodeArray[quads[i].op]);
                PrintIcodeExpression(fp ,quads[i].arg1);
                PrintIcodeExpression(fp ,quads[i].result);
                fprintf(fp,"\t\t\t   : %u",quads[i].taddress);
                fprintf(fp,"\n");

            }else if((opcode == if_eq)||(opcode == if_not_eq )||(opcode == if_lesseq)||(opcode == if_greatereq)||(opcode == if_less)||(opcode == if_greater)){
                fprintf(fp," %-3d  :  %-8s    ",i,opcodeArray[quads[i].op] );
                PrintIcodeExpression(fp ,quads[i].arg1);
                PrintIcodeExpression(fp ,quads[i].arg2);
                fprintf(fp,"  %d",quads[i].label);
                fprintf(fp,"\t\t\t   : %u",quads[i].taddress);
                fprintf(fp,"\n");
            }else if(opcode == jump){
                fprintf(fp," %-3d  :  %-8s    ",i,opcodeArray[quads[i].op] );
                fprintf(fp,"  %d",quads[i].label);
                fprintf(fp,"\t\t\t   : %u",quads[i].taddress);
                fprintf(fp,"\n");
            }else if(opcode == assign){
                fprintf(fp," %-3d  :  %-8s    ",i , opcodeArray[quads[i].op] );
                PrintIcodeExpression(fp ,quads[i].arg1);
                PrintIcodeExpression(fp ,quads[i].result);
                fprintf(fp,"\t\t\t   : %u",quads[i].taddress);
                fprintf(fp,"\n");
            }else if((opcode == funcstart)||(opcode == funcend)||(opcode == param)||(opcode == getretval)||(opcode == call)){
                fprintf(fp," %-3d  :  %-8s    ",i,opcodeArray[quads[i].op] );
                PrintIcodeExpression(fp ,quads[i].result);
                fprintf(fp,"\t\t\t   : %u",quads[i].taddress);
                fprintf(fp,"\n");
	    }else if ((opcode == ret)||(opcode == tablecreate)){
                fprintf(fp," %-3d  :  %-8s    ",i,opcodeArray[quads[i].op] );
                if(quads[i].result != NULL){
	                PrintIcodeExpression(fp ,quads[i].result);
                }
                fprintf(fp,"\t\t\t   : %u",quads[i].taddress);
                fprintf(fp,"\n");
        }else{
            fprintf(fp,"ERROR opcode NOT match %s\n",opcodeArray[quads[i].op]);
        }

    }

    fclose(fp);
    return ;
}

