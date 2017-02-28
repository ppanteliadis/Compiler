#include "constTables.h"
#include "stack.h"


typedef void (*generator_func_t)(quad*);
unsigned int currInstruction = 0;
unsigned v_total = 0;
instruction  *instructions =NULL;

unsigned  currProcessedQuadNum ;
incomplete_jump *ij_head = NULL;

stack_t * funcstack = NULL;


char * Vexpr_tArray[] = {
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

char * vmarg_tArray[]={
    "label_a"     ,
    "global_a"    ,
    "formal_a"    ,
    "local_a"     ,
    "number_a"    ,
    "string_a"    ,
    "bool_a"      ,
    "nil_a"       ,
    "userfunc_a"  ,
    "libfunc_a"   ,
    "retval_a"
};

char * v_opcodeArray[] ={
    "add",
    "sub",
    "mul",
    "div",
    "mod",
    "uminus",
    "newtable",
    "tablegetelem",
    "tablesetelem",
    "assign",
    "jump",
    "jeq",
    "jne",
    "jgt",
    "jge",
    "jlt",
    "jle",
    "not",
    "or",
    "and",
    "pusharg",
    "call",
    "funcenter",
    "funcexit",
    "nop"
};

//prepei na dw8ei swsth seira kai sta opcode twn quads
generator_func_t generators[]={
    generate_ADD,
    generate_SUB,
    generate_MUL,
    generate_DIV,
    generate_MOD,
    generate_UMINUS,
    generate_NEWTABLE,
    generate_TABLEGETELEM,
    generate_TABLESETELEM,
    generate_ASSIGN,
    generate_JUMP,
    generate_IF_EQ,
    generate_IF_NOTEQ,
    generate_IF_GREATER,
    generate_IF_GREATEREQ,
    generate_IF_LESS,
    generate_IF_LESSEQ,
    generate_NOT,
    generate_OR,
    generate_AND,
    generate_PARAM,
    generate_CALL,
    generate_GETRETVAL,
    generate_FUNCSTART,
    generate_RETURN,
    generate_FUNCEND,
    generate_NOP
};

unsigned currProcessedQuad(){
    return currProcessedQuadNum;
}



void make_operand(expr *e, vmarg *arg){

    switch(e->type){
        case var_e:
        case tableitem_e:
        case assignexpr_e:
        case arithexpr_e:
        case boolexpr_e:
        case newtable_e:{
            assert(e->sym);

            arg->val=e->sym->value.varVal->scopeOffset;

            switch(e->sym->value.varVal->scopeSpace){
                case  programvar:
                    arg->type=global_a;
                    break;
                case formalarg:
                    arg->type=formal_a;
                    break;
                case functionlocal:
                    arg->type=local_a;
                    break;
                default: assert(0);
            }

            break;
        }
        case constbool_e:{
            if(e->boolConst == true){
                arg->val=1;
            }else{
                arg->val=0;
            }
            arg->type=bool_a;
            break;
        }
        case conststring_e:{
            arg->val=consts_newstring(e->strConst);
            arg->type=string_a;
            break;
        }
        case constnum_e:{
            arg->val=consts_newnumber(e->numConst);
            arg->type=number_a;
            break;
        }
        case nil_e:{
            arg->type=nil_a;
            break;
        }
        case programfunc_e:{
            arg->type=userfunc_a;
            arg->val = userfuncs_newfunc(e->sym);
            break;
        }
        case libraryfunc_e:{
            arg->type=libfunc_a;
            arg->val=libfuncs_newused(e->sym->value.funcVal->name);
            break;
        }
        default: assert(0);
    }
    return;
}

void make_numberoperand(vmarg *arg, double val){
    arg->type=number_a;
    arg->val=consts_newnumber(val);
    return;
}

void make_booloperand(vmarg *arg, bool val){
    arg->type=bool_a;

    if(val == true){
        arg->val=1;
    }else{
        arg->val=0;
    }

    return;
}


void make_retvaloperand(vmarg *arg){
    arg->type=retval_a;
    return;
}


void add_incomplete_jump(unsigned instrNo, unsigned iaddress){

    incomplete_jump *ij = (incomplete_jump*)malloc(sizeof(incomplete_jump));
    ij->iaddress=iaddress;
    ij->instrNo=instrNo;
    ij->next=ij_head;
    ij_head=ij;
    return;
}


void patch_incomplete_jumps(void) {
    incomplete_jump *x=ij_head;

    while(x!=NULL){
        if (x->iaddress == nextquadlabel()){
            instructions[x->instrNo].result->val=currInstruction;
        }else{
            instructions[x->instrNo].result->val = quads[x->iaddress].taddress;
        }
        x=x->next;
    }

    ij_head=NULL;
    return;
}



void v_expand(void){
    assert(v_total==currInstruction);
    instruction * v= (instruction *) malloc(V_NEW_SIZE);

    if (instructions){
        memcpy(v, instructions, V_CURR_SIZE);
        free(instructions);
    }

    instructions = v;
    v_total += V_EXPAND_SIZE;
    return;
}

void v_emit(instruction *t){

    if(currInstruction == v_total){
        v_expand();
    }

    instruction *v = instructions + currInstruction++;
    v->opcode = t->opcode;
    v->arg1 = t->arg1;
    v->arg2 = t->arg2;
    v->result = t->result;
    v->srcLine =t->srcLine;
    return;
}

unsigned nextinstructionlabel(){
    return currInstruction;
}

void generate (vmopcode op, quad *p) {
    instruction *t=(instruction*)malloc(sizeof(instruction));

    t->opcode = op;

    if(p->arg1!=NULL){
        t->arg1=(vmarg*)malloc(sizeof(vmarg));
        make_operand(p->arg1, t->arg1);
    }else{
        t->arg1=NULL;
    }

    if(p->arg2!=NULL){
        t->arg2=(vmarg*)malloc(sizeof(vmarg));
        make_operand(p->arg2, t->arg2);
    }else{
        t->arg2=NULL;
    }

    if(p->result!=NULL){
        t->result=(vmarg*)malloc(sizeof(vmarg));
        make_operand(p->result, t->result);
    }else {
        t->result=NULL;
    }

    t->srcLine = p->line;
    p->taddress = nextinstructionlabel();
    v_emit(t);

    return;
}

void generate_ADD (quad *p) {
    generate(add_v, p);
    return;
}

void generate_SUB (quad *p) {
    generate(sub_v, p);
    return;
}

void generate_MUL (quad *p) {
    generate(mul_v, p);
    return;
}

void generate_DIV (quad *p) {
    generate(div_v, p);
    return;
}

void generate_MOD (quad *p) {
    generate(mod_v, p);
    return;
}


void generate_NEWTABLE (quad *p) {
    generate(newtable_v, p);
    return;
}

void generate_TABLEGETELEM (quad *p) {
    generate(tablegetelem_v, p);
    return;
}

void generate_TABLESETELEM (quad *p) {
    generate(tablesetelem_v, p);
    return;
}

void generate_ASSIGN (quad *p) {
    generate(assign_v, p);
    return;
}

void generate_NOP (quad *p) {
     instruction *t = (instruction*)malloc(sizeof(instruction));

     t->opcode = nop_v;
     t->arg1=NULL;
     t->arg2=NULL;
     t->result=NULL;
     t->srcLine = p->line;

     v_emit(t);

     return;
}

void generate_relational (vmopcode op, quad *p) {
    instruction *t = (instruction*)malloc(sizeof(instruction));

    t->opcode = op;

    if(p->arg1 != NULL){
        t->arg1 = (vmarg*)malloc(sizeof(vmarg));
        make_operand(p->arg1, t->arg1);
    }else{
        t->arg1 = NULL;
    }

    if(p->arg2 != NULL){
        t->arg2 = (vmarg*)malloc(sizeof(vmarg));
        make_operand(p->arg2, t->arg2);
    }else{
        t->arg2 = NULL;
    }

    t->result = (vmarg*)malloc(sizeof(vmarg));
    t->result->type = label_a;

    if (p->label< currProcessedQuad()){
        t->result->val = quads[p->label].taddress;
    }else{
        add_incomplete_jump(nextinstructionlabel(), p->label);
    }

    p->taddress = nextinstructionlabel();
    t->srcLine=p->line;

    v_emit(t);
    return;
}



void generate_JUMP (quad *p){
    generate_relational(jump_v,  p);
    return;
}

void generate_IF_EQ (quad *p){
    generate_relational(jeq_v,  p);
    return;
}

void generate_IF_NOTEQ(quad *p){
    generate_relational(jne_v,  p);
    return;
}

void generate_IF_GREATER (quad *p){
    generate_relational(jgt_v,  p);
    return;
}

void generate_IF_GREATEREQ(quad *p){
    generate_relational(jge_v,  p);
    return;
}

void generate_IF_LESS (quad *p){
    generate_relational(jlt_v,  p);
    return;
}

void generate_IF_LESSEQ (quad *p){
    generate_relational(jle_v,  p);
    return;
}

void reset_operand(vmarg * v){
    v=NULL;
    return;
}


void generate_NOT (quad *p){

    instruction *t = (instruction*)malloc(sizeof(instruction));
    p->taddress = nextinstructionlabel();

    t->opcode = jeq_v;

    t->arg1 = (vmarg*)malloc(sizeof(vmarg));
    make_operand(p->arg1, t->arg1);

    t->arg2 = (vmarg*)malloc(sizeof(vmarg));
    make_booloperand(t->arg2, false);

    t->result = (vmarg*)malloc(sizeof(vmarg));
    t->result->type = label_a;
    t->result->val = nextinstructionlabel()+3;

    t->srcLine=p->line;

    v_emit(t);

    //////////////////////////////////////////////////////////////
    t = (instruction*)malloc(sizeof(instruction));

    t->opcode = assign_v;

    t->arg1 = (vmarg*)malloc(sizeof(vmarg));
    make_booloperand(t->arg1, false);

    reset_operand(t->arg2);

    t->result = (vmarg*)malloc(sizeof(vmarg));
    make_operand(p->result, t->result);
    t->srcLine = p->line;

    v_emit(t);
    //////////////////////////////////////////////////////////////
    t = (instruction*)malloc(sizeof(instruction));
    t->opcode = jump_v;

    reset_operand(t->arg1);
    reset_operand(t->arg2);

    t->result = (vmarg*)malloc(sizeof(vmarg));
    t->result->type = label_a;
    t->result->val = nextinstructionlabel()+2;
    t->srcLine = p->line;

    v_emit(t);
    //////////////////////////////////////////////////////////////
    t = (instruction*)malloc(sizeof(instruction));
    t->opcode = assign_v;

    t->arg1 = (vmarg*)malloc(sizeof(vmarg));
    make_booloperand(t->arg1, true);

    reset_operand(t->arg2);

    t->result = (vmarg*)malloc(sizeof(vmarg));
    make_operand(p->result, t->result);
    t->srcLine = p->line;

    v_emit(t);

    return;
}



void generate_UMINUS (quad *p) {
    instruction *t = (instruction*)malloc(sizeof(instruction));

    t->opcode = mul_v;

    t->arg1 = (vmarg*)malloc(sizeof(vmarg));
    make_operand(p->arg1, t->arg1);

    t->arg2 = (vmarg*)malloc(sizeof(vmarg));
    make_numberoperand(t->arg2, -1);

    t->result = (vmarg*)malloc(sizeof(vmarg));
    make_operand(p->result, t->result);

    t->srcLine=p->line;
    p->taddress = nextinstructionlabel();

    v_emit(t);

    return;
}

void generate_OR (quad *p){
    instruction *t = (instruction*)malloc(sizeof(instruction));
    p->taddress = nextinstructionlabel();

    t->opcode = jeq_v;

    t->arg1 = (vmarg*)malloc(sizeof(vmarg));
    make_operand(p->arg1, t->arg1);

    t->arg2 = (vmarg*)malloc(sizeof(vmarg));
    make_booloperand(t->arg2, true);

    t->result = (vmarg*)malloc(sizeof(vmarg));
    t->result->type = label_a;
    t->result->val = nextinstructionlabel()+4;

    t->srcLine=p->line;
    v_emit(t);
    ////////////////////////////////////////////////////////////////
    t = (instruction*)malloc(sizeof(instruction));
    t->opcode = jeq_v;

    t->arg1 = (vmarg*)malloc(sizeof(vmarg));
    make_operand(p->arg2, t->arg1);

    t->arg2 = (vmarg*)malloc(sizeof(vmarg));
    make_booloperand(t->arg2, true);

    t->result = (vmarg*)malloc(sizeof(vmarg));
    t->result->val = nextinstructionlabel()+3;
    t->srcLine = p->line;

    v_emit(t);
    ///////////////////////////////////////////////////////////////
    t = (instruction*)malloc(sizeof(instruction));
    t->opcode = assign_v;

    t->arg1 = (vmarg*)malloc(sizeof(vmarg));
    make_booloperand(t->arg1, false);

    reset_operand(t->arg2);

    t->result = (vmarg*)malloc(sizeof(vmarg));
    make_operand(p->result, t->result);
    t->srcLine = p->line;

    v_emit(t);
    ////////////////////////////////////////////////////////////////
    t = (instruction*)malloc(sizeof(instruction));
    t->opcode = jump_v;

    reset_operand(t->arg1);
    reset_operand(t->arg2);

    t->result = (vmarg*)malloc(sizeof(vmarg));
    t->result->type = label_a;
    t->result->val = nextinstructionlabel()+2;
    t->srcLine = p->line;

    v_emit(t);
    ////////////////////////////////////////////////////////////////
    t = (instruction*)malloc(sizeof(instruction));
    t->opcode = assign_v;

    t->arg1 = (vmarg*)malloc(sizeof(vmarg));
    make_booloperand(t->arg1, true);

    reset_operand(t->arg2);

    t->result = (vmarg*)malloc(sizeof(vmarg));
    make_operand(p->result, t->result);
    t->srcLine = p->line;

    v_emit(t);

    return;
}

void generate_AND (quad *p){
    instruction *t = (instruction*)malloc(sizeof(instruction));
    p->taddress = nextinstructionlabel();

    t->opcode = jeq_v;

    t->arg1 = (vmarg*)malloc(sizeof(vmarg));
    make_operand(p->arg1, t->arg1);

    t->arg2 = (vmarg*)malloc(sizeof(vmarg));
    make_booloperand(t->arg2, false);

    t->result = (vmarg*)malloc(sizeof(vmarg));
    t->result->type = label_a;
    t->result->val = nextinstructionlabel()+4;

    t->srcLine=p->line;
    v_emit(t);
    ////////////////////////////////////////////////////////////////
    t = (instruction*)malloc(sizeof(instruction));
    t->opcode = jeq_v;

    t->arg1 = (vmarg*)malloc(sizeof(vmarg));
    make_operand(p->arg2, t->arg1);

    t->arg2 = (vmarg*)malloc(sizeof(vmarg));
    make_booloperand(t->arg2, false);

    t->result = (vmarg*)malloc(sizeof(vmarg));
    t->result->val = nextinstructionlabel()+3;
    t->srcLine = p->line;

    v_emit(t);
    ///////////////////////////////////////////////////////////////
    t = (instruction*)malloc(sizeof(instruction));
    t->opcode = assign_v;

    t->arg1 = (vmarg*)malloc(sizeof(vmarg));
    make_booloperand(t->arg1, true);

    reset_operand(t->arg2);

    t->result = (vmarg*)malloc(sizeof(vmarg));
    make_operand(p->result, t->result);
    t->srcLine = p->line;

    v_emit(t);
    ////////////////////////////////////////////////////////////////
    t = (instruction*)malloc(sizeof(instruction));
    t->opcode = jump_v;

    reset_operand(t->arg1);
    reset_operand(t->arg2);

    t->result = (vmarg*)malloc(sizeof(vmarg));
    t->result->type = label_a;
    t->result->val = nextinstructionlabel()+2;
    t->srcLine = p->line;

    v_emit(t);
    ////////////////////////////////////////////////////////////////
    t = (instruction*)malloc(sizeof(instruction));
    t->opcode = assign_v;

    t->arg1 = (vmarg*)malloc(sizeof(vmarg));
    make_booloperand(t->arg1, false);

    reset_operand(t->arg2);

    t->result = (vmarg*)malloc(sizeof(vmarg));
    make_operand(p->result, t->result);
    t->srcLine = p->line;

    v_emit(t);

    return;
}


void generate_PARAM(quad *p) {
    instruction *t = malloc(sizeof(instruction));

    p->taddress = nextinstructionlabel();
    t->opcode = pusharg_v;
    t->result = (vmarg*)malloc(sizeof(vmarg));
    make_operand(p->result, t->result);
    t->arg1=NULL;
    t->arg2=NULL;
    t->srcLine=p->line;

    v_emit(t);

    return;
}

void generate_CALL(quad *p) {
    instruction * t;

    p->taddress = nextinstructionlabel();

    t = (instruction *)malloc(sizeof(instruction));

    t->opcode = call_v;

    t->result = (vmarg*)malloc(sizeof(vmarg));
    make_operand(p->result, t->result);
    t->srcLine = p->line;
    t->arg1 = NULL;
    t->arg2 = NULL;

    v_emit(t);

    return;
}


void generate_GETRETVAL(quad *p) {
    instruction *t=malloc(sizeof(instruction));

    p->taddress = nextinstructionlabel();

    t->opcode = assign_v;

    t->result = (vmarg*)malloc(sizeof(vmarg));
    make_operand(p->result, t->result);

    t->arg1 = (vmarg*)malloc(sizeof(vmarg));
    make_retvaloperand(t->arg1);

    t->arg2=NULL;
    t->srcLine=p->line;

    v_emit(t);

    return;
}


void appendReturnList(SymbolTableEntry *function ,unsigned possition){

    incompleteReturn *newNode = (incompleteReturn*)malloc(sizeof(incompleteReturn));

    newNode->possition = possition;
    newNode->next = function->value.funcVal->returnList;
    function->value.funcVal->returnList = newNode;

    return;
}

void backPatchFunction(SymbolTableEntry *function ,unsigned label){
    incompleteReturn *tmp;

    while(function->value.funcVal->returnList){
        tmp = function->value.funcVal->returnList;
        instructions[function->value.funcVal->returnList->possition].result->val = label;
        function->value.funcVal->returnList = function->value.funcVal->returnList->next;
        tmp->next = NULL;
        free(tmp);
    }
    return;
}

void generate_FUNCSTART (quad *p){
    unsigned incompleteJump;
    instruction *t = (instruction*)malloc(sizeof(instruction));
    SymbolTableEntry *f;
    p->taddress = nextinstructionlabel();

    t->opcode=jump_v;
    reset_operand(t->arg1);
    reset_operand(t->arg2);
    t->result = (vmarg*)malloc(sizeof(vmarg));
    t->result->type=label_a;
    incompleteJump = nextinstructionlabel();
    t->srcLine = p->line;

    v_emit(t);
    ////////////////////////////////////////////////////////////////

    f = p->result->sym;
    f->value.funcVal->taddress = nextinstructionlabel();
    f->value.funcVal->returnList = NULL;
    f->value.funcVal->incompleteJump = incompleteJump ;

////
    t = (instruction*)malloc(sizeof(instruction));
    t->result = (vmarg*)malloc(sizeof(vmarg));
    t->result->val = userfuncs_newfunc(f);
    push(funcstack, 0, f);
////
    t->opcode=funcenter_v;
    make_operand(p->result,t->result);

    reset_operand(t->arg1);
    reset_operand(t->arg2);
    t->srcLine=p->line;

    v_emit(t);

    return;
}

void generate_RETURN(quad *p){
    SymbolTableEntry * f;
    instruction *t ;
    p->taddress=nextinstructionlabel();

    if(t->result != NULL){
        t = (instruction*)malloc(sizeof(instruction));

        t->opcode=assign_v;

        t->result = (vmarg*)malloc(sizeof(vmarg));
        make_retvaloperand(t->result);

        t->arg1 = (vmarg*)malloc(sizeof(vmarg));
        make_operand(p->result , t->arg1);

        reset_operand(t->arg2);
        t->srcLine=p->line;
        v_emit(t);
    }

    ///////////////////////////////////////////////////////////
//////
    f = (SymbolTableEntry *)top(funcstack);
    appendReturnList(f,nextinstructionlabel());
/////
    t = (instruction*)malloc(sizeof(instruction));
    t->opcode=jump_v;

    reset_operand(t->arg1);

    reset_operand(t->arg2);

    t->result = (vmarg*)malloc(sizeof(vmarg));
    t->result->type=label_a;
    t->srcLine=p->line;

    v_emit(t);

    return;
}

void generate_FUNCEND(quad *p){
    SymbolTableEntry * f;
    instruction *t = (instruction*)malloc(sizeof(instruction));
/////
    f = (SymbolTableEntry *)pop_fuc(funcstack);
    backPatchFunction(f,nextinstructionlabel());
/////

    p->taddress=nextinstructionlabel();

    t->opcode=funcexit_v;
    t->result = (vmarg*)malloc(sizeof(vmarg));
    make_operand(p->result,t->result);
    t->result->val = userfuncs_newfunc(f);
    t->srcLine=p->line;
    reset_operand(t->arg1);
    reset_operand(t->arg2);

    v_emit(t);

    instructions[f->value.funcVal->incompleteJump].result->val = nextinstructionlabel();

    return;
}

void stackInitialize(){
    funcstack = MakeEmptyStack();
}

void generateTargetCode(void){
    unsigned i;
    for(i=0; i<currQuad; i++){
        currProcessedQuadNum = i;
        (*generators[quads[i].op])(quads+i);
    }

    patch_incomplete_jumps();

    if(currQuad==0){
        generate_NOP(NULL);
    }
}



void PrintBinaryCode(void){
    FILE * fp;
    unsigned magicnumber=340200501;
    unsigned size, i;
    int opcode;
    fp = fopen("avmbinaryfile.abc","wb");
    if (!fp)
		{
			printf("Unable to open binary file!");
			return ;
		}

    fwrite(&magicnumber, sizeof(unsigned), 1, fp);

    fwrite(&numberOfGlobalVars , sizeof(int) , 1 , fp);

    /////////////////////
    //  string array   //
    /////////////////////
    fwrite(&currSTRING, sizeof(unsigned), 1, fp);
    for(i=0; i<currSTRING; i++){
        size = (strlen(t_string[i])+1)*sizeof(char);
        fwrite(&size,sizeof(unsigned), 1,fp);
        fwrite(t_string[i],size*sizeof(char), 1,fp);
    }

    /////////////////////
    //  number array   //
    /////////////////////
    fwrite(&currNUM, sizeof(unsigned), 1, fp);
    for(i=0; i<currNUM; i++){
        fwrite(&t_num[i],sizeof(double), 1,fp);

    }

    ////////////////////////////
    //  userfunctions array   //
    ////////////////////////////
    fwrite(&currUSERFUNC, sizeof(unsigned), 1, fp);
    for(i=0; i<currUSERFUNC; i++){
        fwrite(&(t_userfunc[i].taddress),sizeof(unsigned),1,fp);
        fwrite(&(t_userfunc[i].totallocals),sizeof(unsigned),1,fp);
        size = (strlen(t_userfunc[i].name)+1)*sizeof(char);
        fwrite(&size,sizeof(unsigned), 1,fp);
        fwrite(t_userfunc[i].name,size,1,fp);
    }


    ///////////////////////////////
    //  libraryfunctions array   //
    ///////////////////////////////
    fwrite(&currLIB, sizeof(unsigned), 1, fp);
    for( i=0; i<currLIB; i++){
        size = (strlen(t_lib[i])+1)*sizeof(char);
        fwrite(&size,sizeof(unsigned), 1,fp);
        fwrite(t_lib[i],size, 1,fp);
    }


    /////////////
    //  CODE   //
    /////////////
    fwrite(&currInstruction, sizeof(unsigned int), 1, fp);
    for(i = 0 ; i< currInstruction ; i++){
        opcode = instructions[i].opcode;

        if((opcode == add_v)||(opcode == sub_v )||(opcode == mul_v)||(opcode == div_v)||(opcode == mod_v)||(opcode == or_v)||(opcode == and_v)||(opcode == tablesetelem_v)||(opcode == tablegetelem_v)){
            fwrite(&opcode,sizeof(int),1,fp);
            fwrite(&(instructions[i].arg1->type),sizeof(int),1,fp);
            fwrite(&(instructions[i].arg1->val),sizeof(unsigned),1,fp);
            fwrite(&(instructions[i].arg2->type),sizeof(int),1,fp);
            fwrite(&(instructions[i].arg2->val),sizeof(unsigned),1,fp);
            fwrite(&(instructions[i].result->type),sizeof(int),1,fp);
            fwrite(&(instructions[i].result->val),sizeof(unsigned),1,fp);
            fwrite(&(instructions[i].srcLine),sizeof(unsigned),1,fp);
        }
        else if((opcode == uminus_v)||(opcode == not_v)){
            fwrite(&opcode,sizeof(int),1,fp);
            fwrite(&(instructions[i].arg1->type),sizeof(int),1,fp);
            fwrite(&(instructions[i].arg1->val),sizeof(unsigned),1,fp);
            fwrite(&(instructions[i].result->type),sizeof(int),1,fp);
            fwrite(&(instructions[i].result->val),sizeof(unsigned),1,fp);
            fwrite(&(instructions[i].srcLine),sizeof(unsigned),1,fp);
        }
        else if((opcode == jeq_v)||(opcode == jne_v )||(opcode == jlt_v)||(opcode == jgt_v)||(opcode == jle_v)||(opcode == jge_v)){
            fwrite(&opcode,sizeof(int),1,fp);
            fwrite(&(instructions[i].arg1->type),sizeof(int),1,fp);
            fwrite(&(instructions[i].arg1->val),sizeof(unsigned),1,fp);
            fwrite(&(instructions[i].arg2->type),sizeof(int),1,fp);
            fwrite(&(instructions[i].arg2->val),sizeof(unsigned),1,fp);
            fwrite(&(instructions[i].result->type),sizeof(int),1,fp);
            fwrite(&(instructions[i].result->val),sizeof(unsigned),1,fp);
            fwrite(&(instructions[i].srcLine),sizeof(unsigned),1,fp);
        }
        else if(opcode == jump_v){
            fwrite(&opcode,sizeof(int),1,fp);
            fwrite(&(instructions[i].result->type),sizeof(int),1,fp);
            fwrite(&(instructions[i].result->val),sizeof(unsigned),1,fp);
            fwrite(&(instructions[i].srcLine),sizeof(unsigned),1,fp);
        }
        else if(opcode == assign_v){
            fwrite(&opcode,sizeof(int),1,fp);

            if(instructions[i].arg1->type==retval_a){
                fwrite(&(instructions[i].arg1->type),sizeof(int),1,fp);
            }else {
                fwrite(&(instructions[i].arg1->type),sizeof(int),1,fp);
                fwrite(&(instructions[i].arg1->val),sizeof(unsigned),1,fp);
            }

            fwrite(&(instructions[i].result->type),sizeof(int),1,fp);
            fwrite(&(instructions[i].result->val),sizeof(unsigned),1,fp);
            fwrite(&(instructions[i].srcLine),sizeof(unsigned),1,fp);
        }
        else if((opcode == funcenter_v)||(opcode == funcexit_v)||(opcode == pusharg_v)||(opcode == call_v)){
            fwrite(&opcode,sizeof(int),1,fp);
            fwrite(&(instructions[i].result->type),sizeof(int),1,fp);
            fwrite(&(instructions[i].result->val),sizeof(unsigned),1,fp);
            fwrite(&(instructions[i].srcLine),sizeof(unsigned),1,fp);

        }else if (opcode == newtable_v){
            fwrite(&opcode,sizeof(int),1,fp);
            fwrite(&(instructions[i].result->type),sizeof(int),1,fp);
            fwrite(&(instructions[i].result->val),sizeof(unsigned),1,fp);
            fwrite(&(instructions[i].srcLine),sizeof(unsigned),1,fp);

        }
    }

    fclose(fp);
    return;
}


void PrintTables(FILE * fp){
    int i ;

    fprintf(fp , "\n\nString Table\n");
    fprintf(fp , "size : %u\n",currSTRING);
    for(i = 0 ; i< currSTRING ; i++){
        fprintf(fp,"    %d  :  %s\n",i,t_string[i]);
    }

    fprintf(fp,"\n\nNum Table\n");
    fprintf(fp , "size : %u\n",currNUM);
    for(i = 0 ; i< currNUM ; i++){
        fprintf(fp,"    %d  :  %lf\n",i,t_num[i]);
    }


    fprintf(fp,"\n\nUserfunc Table\n");
    fprintf(fp , "size : %u\n",currUSERFUNC);
    for(i = 0 ; i< currUSERFUNC ; i++){
        fprintf(fp,"    %d  :   %s   ,address  %u  ,  local  %u  \n",i,t_userfunc[i].name ,t_userfunc[i].taddress  , t_userfunc[i].totallocals );
    }

    fprintf(fp,"\n\nLib Table\n");
    fprintf(fp , "size : %u\n",currLIB);
    for(i = 0 ; i< currLIB ; i++){
        fprintf(fp,"    %d  :  %s\n",i,t_lib[i]);
    }

    return;
}


void PrintTcodeVAM(FILE * fp , vmarg * arg){

    vmarg_t type = arg->type;
    if(type==label_a || type==global_a || type==formal_a || type==local_a || type==number_a || type== string_a || type==userfunc_a || type==libfunc_a){
        fprintf(fp ,"  %6s",vmarg_tArray[arg->type]);
        fprintf(fp ,"  %6u",arg->val);

    }
    else if(type==bool_a){
        fprintf(fp ,"  %6s",vmarg_tArray[arg->type]);
        if(arg->val==1) fprintf(fp ,"  true");
        else  fprintf(fp ," false");
    }
    else if(type== nil_a)  fprintf(fp ,"  %6s",vmarg_tArray[arg->type]);
    else if(type==retval_a){
        fprintf(fp ,"  %6s",vmarg_tArray[arg->type]);
    }
    else{
        printf("ERROR type NOT match\n");
        assert(0);
    }
    return ;
}


void PrintTargetCode(){
    int i ;
    char * op , * arg1 , * arg2 , *result;
    unsigned magicnumber = 340200501;
    int opcode;
    FILE * fp;
    fp = fopen("targetCode.txt","w");

    fprintf(fp , "magicnumber = %u\n",magicnumber);

    fprintf( fp , "NUmber of Global vars %d \n" ,numberOfGlobalVars );


    PrintTables(fp);

    fprintf(fp,"\n\n====================== T CODE ====================== \n");

    fprintf(fp , " size : %u \n",currInstruction);
    for(i = 0 ; i< currInstruction ; i++){
        opcode = instructions[i].opcode;

        if((opcode == add_v)||(opcode == sub_v )||(opcode == mul_v)||(opcode == div_v)||(opcode == mod_v)||(opcode == or_v)||(opcode == and_v)||(opcode == tablesetelem_v)||(opcode == tablegetelem_v)){
            fprintf(fp," %-3d  :  %-8s    ",i,v_opcodeArray[instructions[i].opcode] );
            PrintTcodeVAM(fp ,instructions[i].arg1);
            PrintTcodeVAM(fp ,instructions[i].arg2);
            PrintTcodeVAM(fp ,instructions[i].result);
            fprintf(fp , "\t : %u ",instructions[i].srcLine);

            fprintf(fp,"\n");
        }
        else if((opcode == uminus_v)||(opcode == not_v)){
            fprintf(fp," %-3d  :  %-8s    ",i,v_opcodeArray[instructions[i].opcode]);
            PrintTcodeVAM(fp ,instructions[i].arg1);
            PrintTcodeVAM(fp ,instructions[i].result);
            fprintf(fp , "\t : %u ",instructions[i].srcLine);

            fprintf(fp,"\n");
        }
        else if((opcode == jeq_v)||(opcode == jne_v )||(opcode == jlt_v)||(opcode == jgt_v)||(opcode == jle_v)||(opcode == jge_v)){
            fprintf(fp," %-3d  :  %-8s    ",i,v_opcodeArray[instructions[i].opcode] );
            PrintTcodeVAM(fp ,instructions[i].arg1);
            PrintTcodeVAM(fp ,instructions[i].arg2);
            PrintTcodeVAM(fp ,instructions[i].result);
            fprintf(fp , "\t : %u ",instructions[i].srcLine);

            fprintf(fp,"\n");
        }
        else if(opcode == jump_v){
            fprintf(fp," %-3d  :  %-8s    ",i,v_opcodeArray[instructions[i].opcode] );
            PrintTcodeVAM(fp ,instructions[i].result);
            fprintf(fp , "\t : %u ",instructions[i].srcLine);

            fprintf(fp,"\n");
        }
        else if(opcode == assign_v){

            fprintf(fp," %-3d  :  %-8s    ",i , v_opcodeArray[instructions[i].opcode] );

            lab(fp ,instructions[i].arg1);

            if((instructions[i].result)!=NULL)
                PrintTcodeVAM(fp ,instructions[i].result);


            fprintf(fp , "\t : %u ",instructions[i].srcLine);
            fprintf(fp,"\n");
        }
        else if((opcode == funcenter_v)||(opcode == funcexit_v)||(opcode == pusharg_v)||(opcode == call_v)){
            fprintf(fp," %-3d  :  %-8s    ",i,v_opcodeArray[instructions[i].opcode] );
            PrintTcodeVAM(fp ,instructions[i].result);
            fprintf(fp , "\t : %u ",instructions[i].srcLine);

            fprintf(fp,"\n");
        }
        else if (opcode == newtable_v){
            fprintf(fp," %-3d  :  %-8s    ",i,v_opcodeArray[instructions[i].opcode] );
            PrintTcodeVAM(fp ,instructions[i].result);
            fprintf(fp , "\t : %u ",instructions[i].srcLine);

            fprintf(fp,"\n");
        }
        else{
            fprintf(fp,"ERROR opcode NOT match %s\n",v_opcodeArray[instructions[i].opcode]);
            assert(0);
        }

    }

    fclose(fp);

    return ;
}


