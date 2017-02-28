#include "libraryFunctions.h"




typedef unsigned char(*tobool_func_t)(avm_memcell*);
typedef bool (*cmp_func)(double, double);


bool jge_impl(double x, double y){
    return x>=y;
}
bool jgt_impl(double x, double y){
    return x>y;
}
bool jle_impl(double x, double y){
    return x<=y;
}
bool jlt_impl(double x, double y){
    return x<y;
}

cmp_func comparisonFuncs[]={
    jgt_impl,
    jge_impl,
    jlt_impl,
    jle_impl
};




tobool_func_t toboolFuncs[]={
    number_tobool,
    string_tobool,
    bool_tobool,
    table_tobool,
    userfunc_tobool,
    libfunc_tobool,
    nil_tobool,
    undef_tobool,
};

unsigned char number_tobool(avm_memcell * m){
    return m->data.numVal!=0;
}

unsigned char string_tobool(avm_memcell * m){
    return m->data.strVal[0]!=0;
}

unsigned char bool_tobool(avm_memcell * m){
    return m->data.boolVal;
}

unsigned char table_tobool(avm_memcell * m){
    return 1;
}

unsigned char userfunc_tobool(avm_memcell * m){
    return 1;
}

unsigned char libfunc_tobool(avm_memcell * m){
    return 1;
}

unsigned char nil_tobool(avm_memcell * m){
    return 0;
}

unsigned char undef_tobool(avm_memcell * m){
    assert(0);
    return 0;
}

unsigned char avm_tobool(avm_memcell *m){
    assert(m->type >= 0 && m->type < undef_m);
    return (*toboolFuncs[m->type])(m);
}

void execute_jump(instruction* instr){

    assert(instr);
    assert(instr->result->type == label_a);

    pc = instr->result->val ;

    return;
}


void execute_cmp(instruction *instr){
    bool result = false;

    assert(instr->result->type == label_a);

    avm_memcell *rv1 = avm_translate_operand(instr->arg1, &ax);
    avm_memcell *rv2 = avm_translate_operand(instr->arg2, &bx);

    assert(rv1 && rv2);

    if(rv1->type!=number_m || rv2->type!=number_m){
        avm_error("not a number in compare!", instr->srcLine, NULL,NULL);
    }else{
        cmp_func cmp = comparisonFuncs[instr->opcode-jgt_v];
        result = (*cmp)(rv1->data.numVal, rv2->data.numVal);
    }

    if(!executionFinished && result){
        pc = instr->result->val ;
    }

    return;
}


void execute_jeq(instruction * instr){

    unsigned char result = 0;
    assert(instr->result->type==label_a);

    avm_memcell* rv1 = avm_translate_operand(instr->arg1, &ax);
    avm_memcell* rv2 = avm_translate_operand(instr->arg2, &bx);

    assert(rv1 && rv2);

    if(rv1->type == undef_m || rv2->type == undef_m){
        avm_error("undef involved in equality",instr->srcLine, NULL, NULL);

    }else if(rv1->type == nil_m || rv2->type == nil_m){
        result = (rv1->type == nil_m && rv2->type == nil_m);

    }else if(rv1->type == bool_m || rv2->type == bool_m){
        result = (avm_tobool(rv1)==avm_tobool(rv2));

    }else if(rv1->type != rv2->type){
        avm_error("is illegal",instr->srcLine,typeStrings[rv1->type],typeStrings[rv2->type]);
    }else{
        if(rv1->type == number_m ){
            result = (rv1->data.numVal == rv2->data.numVal);

        }else if(rv1->type == string_m){
            if(strcmp(rv1->data.strVal,rv2->data.strVal) == 0){
                result = 1;
            }else{
                result = 0;
            }
        }else if(rv1->type == table_m){
            result =(rv1->data.tableVal == rv2->data.tableVal);

        }else if(rv1->type == userfunc_m){
            result = (rv1->data.funcVal == rv2->data.funcVal);

        }else if(rv1->type == libfunc_m){
             if(strcmp(rv1->data.libfuncVal ,rv2->data.libfuncVal) == 0){
                result = 1;
            }else{
                result = 0;
            }
        }else{
            assert(0);
        }

    }

    if(!executionFinished && result){
        pc = instr->result->val;
    }

    return;
}



void execute_jne(instruction * instr){

    unsigned char result = 0;
    assert(instr->result->type == label_a);

    avm_memcell* rv1 = avm_translate_operand(instr->arg1, &ax);
    avm_memcell* rv2 = avm_translate_operand(instr->arg2, &bx);

    assert(rv1 && rv2);

    if(rv1->type == undef_m || rv2->type == undef_m){
        avm_error("undef involved in equality",instr->srcLine, NULL, NULL);

    }else if(rv1->type == nil_m || rv2->type == nil_m){
        result = !(rv1->type == nil_m && rv2->type == nil_m);

    }else if(rv1->type == bool_m || rv2->type == bool_m){
        result = !(avm_tobool(rv1)==avm_tobool(rv2));

    }else if(rv1->type != rv2->type){
        avm_error("is illegal",instr->srcLine,typeStrings[rv1->type],typeStrings[rv2->type]);
    }else{
        if(rv1->type == number_m ){
            result = !(rv1->data.numVal == rv2->data.numVal);

        }else if(rv1->type == string_m){
            if(strcmp(rv1->data.strVal,rv2->data.strVal) == 0){
                result = 0;
            }else{
                result = 1;
            }
        }else if(rv1->type == table_m){
            result = !(rv1->data.tableVal == rv2->data.tableVal);

        }else if(rv1->type == userfunc_m){
            result = !(rv1->data.funcVal == rv2->data.funcVal);

        }else if(rv1->type == libfunc_m){
            if(strcmp(rv1->data.libfuncVal ,rv2->data.libfuncVal) == 0){
                result = 0;
            }else{
                result = 1;
            }
        }else{
            assert(0);
        }

    }

    if(!executionFinished && result){
        pc = instr->result->val;
    }

    return;
}
