#include "header.h"

typedef double (* arithmetic_func_t)(double x, double y);



double add_impl(double x, double y){
    return x+y;
}

double sub_impl(double x, double y){
    return x-y;
}

double mul_impl(double x, double y){
    return x*y;
}

double div_impl(double x, double y){
    if(y==0){
        avm_error("Division with zero!!!",currLine ,NULL,NULL);
    }
    return x/y;
}
double mod_impl(double x, double y){
    if(y==0){
        avm_error("MOD with zero!!!", currLine, NULL,NULL);
    }
    return (unsigned)x% (unsigned)y;

}

arithmetic_func_t arithmeticFuncs[]={
    add_impl,
    sub_impl,
    mul_impl,
    div_impl,
    mod_impl
};

char * typeArray1[] = {
 "number_m"       ,
 "string_m"       ,
 "bool_m"         ,
 "table_m"        ,
 "userfunc_m"     ,
 "libfunc_m"     ,
 "nil_m"        ,
 "undef_m"
};

void execute_arithmetic(instruction * instr){
    avm_memcell *lv = avm_translate_operand(instr->result, NULL);
    avm_memcell *rv1 = avm_translate_operand(instr->arg1, &ax);
    avm_memcell *rv2 = avm_translate_operand(instr->arg2, &bx);

    assert(lv && (&stack[AVM_STACKSIZE-1]>=lv && lv>&stack[top] || lv==&retval));
    assert(rv1 && rv2);

    if(rv1->type!=number_m && rv2->type!=number_m){
        if(rv1->type==string_m && rv2->type==string_m){
            char *s = strdup(rv1->data.strVal);
            s = (char *)realloc(s , strlen(rv2->data.strVal) + 1);
            s =strcat(s , rv2->data.strVal );
            lv->data.strVal = s;

        }else{
            avm_error("Not Strings  both of operands  at '+' ",instr->srcLine,NULL,NULL);
        }

        /*
        else if (rv1->type==string_m){
            char *s = strdup(rv1->data.strVal);
            lv->data.strVal = s;
        }else if (rv2->type==string_m){
             char *s = strdup(rv2->data.strVal);
            lv->data.strVal = s;
        }

        */

    }else if (rv1->type!=number_m){
        printf("rv1  %d \n",rv1->type);
        avm_error("Not A number in arithmetic!!! operand 1",instr->srcLine,NULL,NULL);
    }else if (rv2->type!=number_m){
        printf("rv2  %d \n",rv2->type);
        avm_error("Not A number in arithmetic!!! operand 2",instr->srcLine,NULL,NULL);
    }else{
        arithmetic_func_t op = arithmeticFuncs[instr->opcode-add_v];
        avm_memcellclear(lv);
        lv->type = number_m;
        lv->data.numVal = (*op)(rv1->data.numVal, rv2->data.numVal);
    }
    return ;
}

