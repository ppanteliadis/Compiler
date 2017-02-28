#include "libraryFunctions.h"

unsigned totalActuals = 0;

library_func_t LibraryFuncs[12];

char * typeArray[] = {
 "number_m"       ,
 "string_m"       ,
 "bool_m"         ,
 "table_m"        ,
 "userfunc_m"     ,
 "libfunc_m"     ,
 "nil_m"        ,
 "undef_m"
};

void execute_call(instruction * instr){
    char *s;
    char tmp[100] = "call: cannot bind to function!";
    avm_memcell* func = avm_translate_operand(instr->result, &ax);
    assert(func);
    if(func->type == table_m){
        execute_pusharg(instr);

    }


    avm_callsaveenvironment();

    switch(func->type){
        case userfunc_m:{
            pc = func->data.funcVal;
            assert(pc<AVM_ENDING_PC);
            assert(code[pc].opcode==funcenter_v);
            break;
        }case string_m:{
            avm_calllibfunc(func->data.strVal);
            break;
        }case libfunc_m:{
            avm_calllibfunc(func->data.libfuncVal);
            break;
        }case  table_m :{
            avm_memcell *key = (avm_memcell *)malloc(sizeof(avm_memcell));
            key->type = string_m;
            key->data.strVal = (char * )malloc(sizeof(char)+10);
            strcpy(key->data.strVal , "()");
            avm_memcell * function = avm_tablegetelem(func->data.tableVal ,key );

            if(function->type == userfunc_m){
                pc = function->data.funcVal;
                assert(pc<AVM_ENDING_PC);
                assert(code[pc].opcode==funcenter_v);

            }else if (function->type == libfunc_m){
                avm_calllibfunc(function->data.libfuncVal);

            }else{
                s = avm_tostring(function);
                s = realloc(s , strlen(s)+strlen(tmp)+1);
                strcat(s , tmp);
                avm_error(s, instr->srcLine , NULL ,NULL);
                free(s);
            }

            break;
        }default:{
            s = avm_tostring(func);
            s = realloc(s , strlen(s)+strlen(tmp)+1);
            strcat(s , tmp);
            avm_error(s, instr->srcLine , NULL ,NULL);

            free(s);
        }
    }

    return;
}



void execute_funcenter(instruction *instr){
    G_inFunction++;
    //////////////
    avm_memcell* func = avm_translate_operand(instr->result,&ax);
    assert(func);
    assert(pc == func->data.funcVal);

    totalActuals = 0;

    userfunc *funcInfo = avm_getfuncinfo(instr->result->val);

    topsp = top;
    top = top - (funcInfo->totallocals);

    return;
}




void avm_dec_top(void){
    if(!top){
        avm_error("stack overflow" ,-1 , NULL,NULL);
    }
    else{
        --top;
    }
}



void avm_calllibfunc(char *id){

    library_func_t f = avm_getlibraryfunc(id);


    if(!f){
        avm_error("unsupported lid func ", -2 ,id,NULL);
        executionFinished = 1;
    }else{
        topsp = top;
        totalActuals = 0;

        (*f)();

        if(!executionFinished)
            execute_funcexit(NULL);
    }

    return;
}

void avm_push_envvalue(unsigned val){

    stack[top].type = number_m;
    stack[top].data.numVal = val;
    avm_dec_top();

    return;
}

void avm_callsaveenvironment(void){

    avm_push_envvalue(totalActuals);
    avm_push_envvalue(pc+1);
    avm_push_envvalue(top+2+totalActuals);
    avm_push_envvalue(topsp);

    return;
}

bool checkIFisNegativeNum(unsigned i){
    unsigned val;

    assert(stack[i].type == number_m);
    val=(unsigned) stack[i].data.numVal;

    if(stack[i].data.numVal==(double) val){
        return false;
    }else{
        return true;
    }
}

unsigned avm_get_envvalue(unsigned i){
    unsigned val;

    assert(stack[i].type == number_m);
    val=(unsigned) stack[i].data.numVal;

    assert(stack[i].data.numVal==(double) val);

    return val;
}


void execute_funcexit(instruction * unused){
    unsigned oldTop = top;

    top     = avm_get_envvalue(topsp+AVM_SAVEDTOP_OFFSET);
    pc      = avm_get_envvalue(topsp+AVM_SAVEDPC_OFFSET);
    topsp   = avm_get_envvalue(topsp+AVM_SAVEDTOPSP_OFFSET);

    int i = 0;
    while(oldTop++ <= top){
        avm_memcellclear(&stack[oldTop]);

    }
    //////////////
    if(unused != NULL){
        G_inFunction--;
        assert(G_inFunction>=0);
    }
    return;
}





userfunc* avm_getfuncinfo(unsigned position){
    assert(!((position < 0)||(position >= currUSERFUNC)));

    return &t_userfunc[position];
}



unsigned avm_totalactuals(void){
    return avm_get_envvalue(topsp + AVM_NUMACTUALS_OFFSET);
}

avm_memcell *avm_getactual(unsigned i){
    assert(i<avm_totalactuals());
    return &stack[topsp+ AVM_STACKENV_SIZE+1+i];
}


void execute_pusharg(instruction * instr){

    assert(instr->result);
    avm_memcell *arg = avm_translate_operand(instr->result,&ax);
    assert(arg);


    avm_assign(&stack[top], arg,instr->srcLine);

    ++totalActuals;
    avm_dec_top();

    return;
}

int LibraryNameToInt(char * name){
    if(strcmp(name,"print")==0){
        return 0;
    }else if(strcmp(name,"input")==0){
        return 1;
    }else if(strcmp(name,"objectmemberkeys")==0){
        return 2;
    }else if(strcmp(name,"objecttotalmembers")==0){
        return 3;
    }else if(strcmp(name,"objectcopy")==0){
        return 4;
    }else if(strcmp(name,"totalarguments")==0){
        return 5;
    }else if(strcmp(name,"argument")==0){
        return 6;
    }else if(strcmp(name,"typeof")==0){
        return 7;
    }else if(strcmp(name,"strtonum")==0){
        return 8;
    }else if(strcmp(name,"sqrt")==0){
        return 9;
    }else if(strcmp(name,"cos")==0){
        return 10;
    }else if(strcmp(name,"sin")==0){
        return 11;
    }else{
        assert(0);
        return -1;
    }

}

library_func_t avm_getlibraryfunc(char * id){
    assert(id);
    return LibraryFuncs[LibraryNameToInt(id)];
}


void avm_registerlibfunc(char *id, library_func_t addr){

    assert(id);
    assert(addr);

    LibraryFuncs[LibraryNameToInt(id)] = addr ;

    return;
}


