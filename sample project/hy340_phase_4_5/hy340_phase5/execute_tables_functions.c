#include "libraryFunctions.h"


char * typeStrings[] ={
    "number",
    "string",
    "bool",
    "table",
    "userfunc",
    "libfunc",
    "nil",
    "undef"
};

void execute_newtable(instruction *instr){

    avm_memcell *lv = avm_translate_operand((instr->result), NULL);
    assert(lv && (&stack[AVM_STACKSIZE-1] >= lv && lv>&stack[top] || lv == &retval));

    avm_memcellclear(lv);
    lv->type = table_m;
    lv->data.tableVal = avm_tablenew();
    avm_tableincrefcounter(lv->data.tableVal);
    return ;
}


void execute_tablesetelem(instruction *instr){

    avm_memcell *t=avm_translate_operand((instr->arg1),NULL);
    avm_memcell *i=avm_translate_operand((instr->arg2),&ax);
    avm_memcell *c=avm_translate_operand((instr->result),&bx);

    assert(t && &stack[AVM_STACKSIZE-1] >= t && t > &stack[top]);
    assert(i && c);

    if(t->type != table_m){
        avm_error("illegal use of type as table", instr->srcLine, NULL, NULL);
    }else{
        avm_tablesetelem(t->data.tableVal, i, c);
    }
}

void execute_tablegetelem(instruction *instr){
    avm_memcell *content;
    char *ts;
    char *is;

    avm_memcell *lv = avm_translate_operand((instr->result),NULL);
    avm_memcell *t = avm_translate_operand((instr->arg1),NULL);
    avm_memcell *i = avm_translate_operand((instr->arg2),&ax);


    assert(lv);

    assert(lv && (&stack[AVM_STACKSIZE-1]>=lv && lv>&stack[top] || lv==&retval));
    assert(t && &stack[AVM_STACKSIZE-1]>=t && t>&stack[top]);
    assert(i);

    avm_memcellclear(lv);
    lv->type = nil_m;

    if(t->type != table_m){
        avm_error("illegal use of type as table", instr->srcLine ,typeStrings[t->type],NULL);
    }else{

        content = avm_tablegetelem(t->data.tableVal,i);

        if(content != NULL ){
            //printf("\n1 content value = %s\n", tableValue_ToString(content));
            //printf("1 lv value = %s\n", tableValue_ToString(lv));
            avm_assign(lv,content,instr->srcLine);
            //printf("2 content value = %s\n", tableValue_ToString(content));
            //printf("2 lv value = %s\n", tableValue_ToString(lv));
        }else{
            ts = avm_tostring(t);
            is = avm_tostring(i);
            avm_warning("not found",instr->srcLine, ts ,is);
            free(ts);
            free(is);

            avm_memcell * null =(avm_memcell *)malloc(sizeof(avm_memcell));
            null->type = nil_m ;

            avm_assign(lv,null,instr->srcLine);

        }

    }
}

