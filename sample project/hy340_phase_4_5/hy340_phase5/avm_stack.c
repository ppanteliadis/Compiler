#include "header.h"

avm_memcell ax ;
avm_memcell bx ;
avm_memcell cx ;
avm_memcell retval;

unsigned top;
unsigned topsp;

avm_memcell stack[AVM_STACKSIZE];


double consts_getnumber(unsigned index){
    assert(!((index < 0)||(index >= currNUM)));
    return t_num[index];
}


char* consts_getstring(unsigned index){
    assert(!((index < 0)||(index >= currSTRING)));
    return t_string[index];
}


char* libfuncs_getused(unsigned index){
    assert(!((index < 0)||(index >= currLIB)));
    return t_lib[index];
}


unsigned userfuncs_getused(unsigned index){
    assert(!((index < 0)||(index >= currUSERFUNC)));
    return t_userfunc[index].taddress;
}





avm_memcell* avm_translate_operand(vmarg *arg, avm_memcell *reg){

    switch(arg->type){
        case global_a:{
            return &stack[AVM_STACKSIZE-1-arg->val];
        }case local_a:{
            return &stack[topsp-arg->val];
        }case formal_a:{
            return &stack[topsp+AVM_STACKENV_SIZE+1+arg->val];
        }case retval_a:{
            return &retval;
        }case number_a:{
            reg->type = number_m;
            reg->data.numVal = consts_getnumber(arg->val);
            return reg;
        }case string_a:{
            reg->type = string_m;
            reg->data.strVal = strdup(consts_getstring(arg->val));
            return reg;
        }case bool_a:{
            reg->type = bool_m;
            reg->data.boolVal = arg->val;
            return reg;
        }case nil_a:{
            reg->type = nil_m;
            return reg;
        }case userfunc_a:{
            reg->type=userfunc_m;
            reg->data.funcVal = userfuncs_getused(arg->val);
            return reg;
        }case libfunc_a:{
            reg->type=libfunc_m;
            reg->data.libfuncVal = strdup(libfuncs_getused(arg->val));
            return reg;
        }default :{
            assert(0);
        }
    }
}

 void avm_initStack(void){
    unsigned i;
    for( i=0; i<AVM_STACKSIZE; i++){
        AVM_WIPEOUT(stack[i]);
        stack[i].type=undef_m;
    }

    return;
}

