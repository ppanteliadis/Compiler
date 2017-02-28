#include "header.h"


void avm_assign(avm_memcell *lv, avm_memcell *rv , unsigned scrLine){

    if(lv == rv){
        return;
    }

    if(lv->type == table_m && rv->type==table_m && lv->data.tableVal == rv->data.tableVal){
        return;
    }

    if(rv->type==undef_m) {
        avm_warning("assigning from 'undef' content!",scrLine,NULL , NULL);
    }

    avm_memcellclear(lv);

    memcpy(lv, rv, sizeof(avm_memcell));

    if(lv->type==string_m) {
        lv->data.strVal=strdup(rv->data.strVal);
    }else if(lv->type==table_m){
        avm_tableincrefcounter(lv->data.tableVal);
    }

    return;
}


void execute_assign(instruction * instr){
    assert(instr);
    avm_memcell *lv = avm_translate_operand(instr->result, NULL);
    avm_memcell *rv = avm_translate_operand(instr->arg1, &ax);

    assert(lv && (&stack[AVM_STACKSIZE-1]>=lv && lv>&stack[top] || lv==&retval));
    assert(rv);

    avm_assign(lv, rv , instr->srcLine);

    return;
}
