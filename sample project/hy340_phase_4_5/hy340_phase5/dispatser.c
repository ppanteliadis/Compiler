#include "header.h"

unsigned char executionFinished = 0;
unsigned pc = 0;
unsigned currLine = 0;
unsigned codeSize = 0;
instruction *code = (instruction*) 0;

char * String_executeFuncs[]={
    "execute_add",
    "execute_sub",
    "execute_mul",
    "execute_div",
    "execute_mod",
    "0,/* execute_uminus */",
    "execute_newtable" ,
    "execute_tablegetelem" ,
    "execute_tablesetelem",
    "execute_assign",
    "execute_jump",
    "execute_jeq" ,
    "execute_jne" ,
    "execute_jgt",
    "execute_jge",
    "execute_jlt",
    "execute_jle",
    "0,/* execute_not */",
    "0,/*execute_or  */",
    "0, /*execute_and */",
    "execute_pusharg",
    "execute_call",
    "execute_funcenter ",
    "execute_funcexit",
    "execute_nop"
};

execute_func_t executeFuncs[]={
    execute_add,
    execute_sub,
    execute_mul,
    execute_div,
    execute_mod,
    0,/* execute_uminus */
    execute_newtable ,
    execute_tablegetelem ,
    execute_tablesetelem,
    execute_assign,
    execute_jump,
    execute_jeq ,
    execute_jne ,
    execute_jgt,
    execute_jge,
    execute_jlt,
    execute_jle,
    0, /*execute_not */
    0,/*execute_or  */
    0,/* execute_and */
    execute_pusharg,
    execute_call,
    execute_funcenter ,
    execute_funcexit,
    execute_nop
};




void execute_cycle(void){
    instruction *instr;
    unsigned oldPC;

    if(executionFinished){
        return;
    }else if(pc == AVM_ENDING_PC){
        executionFinished = 1;
        return;

    }else{

        assert(pc<AVM_ENDING_PC);

        instr = code + pc;

        assert(instr->opcode>=0 && instr->opcode <= AVM_MAX_INSTRUCTIONS(unsigned));

        if(instr->srcLine){
            currLine = instr->srcLine;
        }

        oldPC = pc;
        //printf("-- op %s ----\n",String_executeFuncs[instr->opcode]);
        //printf("- 1 pc %u -----",pc);
        (*executeFuncs[instr->opcode]) (instr);
        //printf(" 2 pc %u ---\n\n",pc);

        if(pc == oldPC){
            ++pc;
        }
    }
}

void avm_warning(char *format , int scrLine, char *id, char *id1 ){
    if(id == NULL){
        printf("\nLine : %d  , %s\n", scrLine , format);
    }else{
        printf("\nLine : %d  , %s [ %s ] %s\n", scrLine, id, id1, format);
    }

    return;
}


void avm_error(char *format ,int scrLine , char *id, char *id1){
    printf("pc %u---",pc);

    if(scrLine == -1){
        printf("%s\n", format);
    }else if(scrLine == -2 ){
        printf("%s %s\n", format , id);
    }else{
        if(id1 == NULL){
                if(id == NULL){
                    printf("Line : %d  , %s\n", scrLine , format);
                }else{
                    printf("Line : %d   ,%s   %s\n", scrLine , format ,id);
                }
        }else{
            printf("Line : %d  , %s == %s %s\n", scrLine , id, id1,format);
        }
    }

    executionFinished = 1;
    return;
}

void execute_nop(instruction * instr){
    printf("execute_nop\n");
    assert(0);
    return;
}
