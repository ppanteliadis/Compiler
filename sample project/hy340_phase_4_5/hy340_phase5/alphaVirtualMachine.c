#include "libraryFunctions.h"

int numberOfGlobals = 0 ;
unsigned tableID;
int G_inFunction;

/*========================================================*/
/*=================READ=BINARY=FILE=======================*/
/*========================================================*/


unsigned v_total = 0;

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


void v_expand(void){
    assert(v_total==codeSize);
    instruction * v= (instruction *) malloc(V_NEW_SIZE);

    if (code){
        memcpy(v, code, V_CURR_SIZE);
        free(code);
    }

    code = v;
    v_total += V_EXPAND_SIZE;
    return;
}

void v_emit(instruction *t){

    if(codeSize == v_total){
        v_expand();
    }

    instruction *v = code + codeSize++;
    v->opcode = t->opcode;
    v->arg1 = t->arg1;
    v->arg2 = t->arg2;
    v->result = t->result;
    v->srcLine =t->srcLine;
    return;
}


void ReadBinaryFile(FILE * fp){
    unsigned magicnumber=340200501;
    unsigned size, i;
    int opcode;
    char * name;
    double d;
    unsigned taddress ;
    unsigned totallocals;
    unsigned currSize;
    instruction * instr = (instruction *)malloc(sizeof(instruction));

    fread(&magicnumber, sizeof(unsigned), 1, fp);

    fread(&numberOfGlobals  , sizeof(int), 1, fp);


    /////////////////////
    //  string array   //
    /////////////////////

    fread(&currSize, sizeof(unsigned), 1, fp);
    for(i=0; i<currSize; i++){
        fread(&size,sizeof(unsigned), 1,fp);
        name = (char *)malloc(sizeof(char)*size);
        fread(name,size*sizeof(char), 1,fp);
        consts_newstring(name);
    }

    /////////////////////
    //  number array   //
    /////////////////////
    fread(&currSize, sizeof(unsigned), 1, fp);
    for(i=0; i<currSize; i++){
        fread(&d,sizeof(double), 1,fp);
        consts_newnumber(d);
    }

    ////////////////////////////
    //  userfunctions array   //
    ////////////////////////////
    fread(&currSize, sizeof(unsigned), 1, fp);
    for(i=0; i<currSize; i++){
        fread(&(taddress),sizeof(unsigned),1,fp);
        fread(&(totallocals),sizeof(unsigned),1,fp);
        fread(&size,sizeof(unsigned), 1,fp);
        name = (char *)malloc(sizeof(char)*size);
        fread(name,size,1,fp);
        userfuncs_newfunc(name ,   taddress , totallocals);
    }


    ///////////////////////////////
    //  libraryfunctions array   //
    ///////////////////////////////
    fread(&currSize, sizeof(unsigned), 1, fp);
    for( i=0; i<currSize; i++){
        fread(&size,sizeof(unsigned), 1,fp);
        name = (char *)malloc(sizeof(char)*size);
        fread(name,size, 1,fp);
        libfuncs_newused(name);
    }


    /////////////
    //  CODE   //
    /////////////
    fread(&currSize, sizeof(unsigned int), 1, fp);
    for(i = 0 ; i< currSize ; i++){
        fread(&opcode,sizeof(int),1,fp);
        instr->opcode = opcode;

        if((opcode == add_v)||(opcode == sub_v )||(opcode == mul_v)||(opcode == div_v)||(opcode == mod_v)||(opcode == or_v)||(opcode == and_v)||(opcode == tablesetelem_v)||(opcode == tablegetelem_v)){
            instr->arg1     = (vmarg*)malloc(sizeof(vmarg));
            instr->arg2     = (vmarg*)malloc(sizeof(vmarg));
            instr->result   = (vmarg*)malloc(sizeof(vmarg));

            fread(&(instr->arg1->type),sizeof(int),1,fp);
            fread(&(instr->arg1->val),sizeof(unsigned),1,fp);
            fread(&(instr->arg2->type),sizeof(int),1,fp);
            fread(&(instr->arg2->val),sizeof(unsigned),1,fp);
            fread(&(instr->result->type),sizeof(int),1,fp);
            fread(&(instr->result->val),sizeof(unsigned),1,fp);
            fread(&(instr->srcLine),sizeof(unsigned),1,fp);
        }
        else if((opcode == uminus_v)||(opcode == not_v)){
            instr->arg1     = (vmarg*)malloc(sizeof(vmarg));
            instr->arg2     = (vmarg*)0;
            instr->result   = (vmarg*)malloc(sizeof(vmarg));

            fread(&(instr->arg1->type),sizeof(int),1,fp);
            fread(&(instr->arg1->val),sizeof(unsigned),1,fp);
            fread(&(instr->result->type),sizeof(int),1,fp);
            fread(&(instr->result->val),sizeof(unsigned),1,fp);
            fread(&(instr->srcLine),sizeof(unsigned),1,fp);

        }
        else if((opcode == jeq_v)||(opcode == jne_v )||(opcode == jlt_v)||(opcode == jgt_v)||(opcode == jle_v)||(opcode == jge_v)){
            instr->arg1     = (vmarg*)malloc(sizeof(vmarg));
            instr->arg2     = (vmarg*)malloc(sizeof(vmarg));
            instr->result   = (vmarg*)malloc(sizeof(vmarg));

            fread(&(instr->arg1->type),sizeof(int),1,fp);
            fread(&(instr->arg1->val),sizeof(unsigned),1,fp);
            fread(&(instr->arg2->type),sizeof(int),1,fp);
            fread(&(instr->arg2->val),sizeof(unsigned),1,fp);
            fread(&(instr->result->type),sizeof(int),1,fp);
            fread(&(instr->result->val),sizeof(unsigned),1,fp);
            fread(&(instr->srcLine),sizeof(unsigned),1,fp);
        }
        else if(opcode == jump_v){
            instr->arg1     = (vmarg*)0;
            instr->arg2     = (vmarg*)0;
            instr->result   = (vmarg*)malloc(sizeof(vmarg));

            fread(&(instr->result->type),sizeof(int),1,fp);
            fread(&(instr->result->val),sizeof(unsigned),1,fp);
            fread(&(instr->srcLine),sizeof(unsigned),1,fp);
        }
        else if(opcode == assign_v){
            instr->arg1     = (vmarg*)malloc(sizeof(vmarg));
            instr->arg2     = (vmarg*)0;
            instr->result   = (vmarg*)malloc(sizeof(vmarg));
                fread(&(instr->arg1->type),sizeof(int),1,fp);

            if(instr->arg1->type!=retval_a){
                fread(&(instr->arg1->val),sizeof(unsigned),1,fp);
            }

            fread(&(instr->result->type),sizeof(int),1,fp);
            fread(&(instr->result->val),sizeof(unsigned),1,fp);
            fread(&(instr->srcLine),sizeof(unsigned),1,fp);
        }
        else if((opcode == funcenter_v)||(opcode == funcexit_v)||(opcode == pusharg_v)||(opcode == call_v)){
            instr->arg1     = (vmarg*)0;
            instr->arg2     = (vmarg*)0;
            instr->result   = (vmarg*)malloc(sizeof(vmarg));

            fread(&(instr->result->type),sizeof(int),1,fp);
            fread(&(instr->result->val),sizeof(unsigned),1,fp);
            fread(&(instr->srcLine),sizeof(unsigned),1,fp);

        }else if (opcode == newtable_v){
            instr->arg1     = (vmarg*)0;
            instr->arg2     = (vmarg*)0;
            instr->result   = (vmarg*)malloc(sizeof(vmarg));


            fread(&(instr->result->type),sizeof(int),1,fp);
            fread(&(instr->result->val),sizeof(unsigned),1,fp);
            fread(&(instr->srcLine),sizeof(unsigned),1,fp);

        }else{
            printf("No match opode \n");
            assert(0);
        }

        v_emit(instr);
    }

    fclose(fp);

    return;
}


/*========================================================*/



void avm_initialize(void){

    avm_initStack();

    avm_registerlibfunc("print", libfunc_print);
    avm_registerlibfunc("input", libfunc_input);
    avm_registerlibfunc("objectmemberkeys", libfunc_objectmemberkeys);
    avm_registerlibfunc("objecttotalmembers",libfunc_objecttotalmembers);
    avm_registerlibfunc("objectcopy",libfunc_objectcopy);
    avm_registerlibfunc("totalarguments",libfunc_totalarguments);
    avm_registerlibfunc("argument",libfunc_argument);
    avm_registerlibfunc("typeof",libfunc_typeof);
    avm_registerlibfunc("strtonum",libfunc_strtonum);
    avm_registerlibfunc("sqrt",libfunc_sqrt);
    avm_registerlibfunc("cos",libfunc_cos);
    avm_registerlibfunc("sin",libfunc_sin);

    return ;
}






////////////////////////////////////////////////////////////////////


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



///////////////////////////////////////////////////////////////////


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
    instruction  * instructions  = code;
    int i ;
    char * op , * arg1 , * arg2 , *result;
    unsigned magicnumber=340200501;
    int opcode;
    FILE * fp;
    fp = fopen("targetCode.txt","w");

    fprintf(fp , "magicnumber = %u\n",magicnumber);

    fprintf( fp , "NUmber of Global vars %d \n" ,numberOfGlobals);


    PrintTables(fp);

    fprintf(fp,"\n\n====================== T CODE ====================== \n");

    fprintf(fp , " size : %u \n",codeSize);
    for(i = 0 ; i< codeSize ; i++){
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

            PrintTcodeVAM(fp ,instructions[i].arg1);

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





int main(int argc, char **argv){
    FILE *binaryFile = NULL;
    int i ;
    executionFinished = 0;
	if(argc <2 ){
		fprintf(stderr,"\n No  input file to parse!!!\n");
		return 1;
	}

	if( !(binaryFile = fopen( argv[1], "rb" )) ){
		fprintf(stderr,"\nCannot open file : %s\n",argv[1]);
		return 1;
	}
    tableID= 0;
    G_inFunction = 0;

    avm_initialize();

    ReadBinaryFile(binaryFile);

    top   = AVM_STACKSIZE - numberOfGlobals - 1;
    topsp = AVM_STACKSIZE - numberOfGlobals - 1;


    while(!executionFinished){
        execute_cycle();
    }

   //PrintTargetCode();

    return 0 ;
}
