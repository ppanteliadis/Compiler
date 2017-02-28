#include "header.h"

#define SYMBOL_TABLE_SIZE 1000
#define SYMBOL_TABLE_MULTIPLIER 65599


SymbolTableEntry **SymbolTable;
ScopeListNode   *ScopeList;

int numberOfAnonymousFunctions;
int lookUpAllUserFunctFound;
int numberOfGlobalVars ;


char * LibraryFunctions[]={
    "print",
    "input",
    "objectmemberkeys",
    "objecttotalmembers",
    "objectcopy",
    "totalarguments",
    "argument",
    "typeof",
    "strtonum",
    "sqrt",
    "cos",
    "sin"
};

unsigned int SymbolTableHashFunction(const char *id){
  int i ;
  unsigned int hash = 0;
  for (i = 0; id[i] != '\0'; i++)
    hash = hash * SYMBOL_TABLE_MULTIPLIER + id[i];
  return (hash%SYMBOL_TABLE_SIZE);
}

char * CheckForLibraryNameCollision(char *id){
    int i ;
    for(i = 0 ; i< 12 ;i++){
        if(strcmp(LibraryFunctions[i],id)==0){
            return LibraryFunctions[i];
        }
    }
    return NULL;
}

/////////////////////////////////////////////////////////////
//EPISTREFEI TO PRWTO ENERGO APO TO TELOS PROS THN ARXH    //
//AN PAREMBAINEI SYNARTHSH MARKAROUME THN GLOBAL MATABLHTH //
//WSTE NA PRA3OUME ANALOGA KATA THN PERIPTWSH              //
/////////////////////////////////////////////////////////////

SymbolTableEntry * SymbolTableLookUp( char * id , int scope){
    int hash_pos;
    SymbolTableEntry * tmp_SymbolTable;

    hash_pos = SymbolTableHashFunction(id);
    tmp_SymbolTable = SymbolTable[hash_pos];

    while(1){
        if(tmp_SymbolTable == NULL){
            break;
        }

        if((tmp_SymbolTable->type == GLOBAL)||(tmp_SymbolTable->type == LOCAL_T)||(tmp_SymbolTable->type == FORMAL)){
            if((strcmp(tmp_SymbolTable->value.varVal->name , id)==0)&&(tmp_SymbolTable->isActive == true)){
                return tmp_SymbolTable;
            }
        }else{
            if((strcmp(tmp_SymbolTable->value.funcVal->name , id)==0)&&(tmp_SymbolTable->isActive == true)){
                return tmp_SymbolTable;
            }
        }

        tmp_SymbolTable = tmp_SymbolTable->next_list_entry;
    }
    return NULL;
}



/* ***************************************************************************** */


SymbolTableEntry * SymbolTableLookUp_Temporary_Variables_At_This_Scope(char * id ,int scope ){
    ScopeListEntries * tmp_ScopeList = ScopeList->head;
    SymbolTableEntry * tmp_SymbolTableEntry;

    if(SymbolTableLookUp( id , scope) == NULL){
        return NULL;
    }

    while((tmp_ScopeList!=NULL)&&(tmp_ScopeList->scopeVal != scope)){
        tmp_ScopeList = tmp_ScopeList->next_list_node;
    }

    if(tmp_ScopeList == NULL){
        printf("SCOPE : %d is null at this time SymbolTableLookUp_THIS \n",scope);
        return NULL ;
    }

    tmp_SymbolTableEntry = tmp_ScopeList->end_of_scope_i;

    while(1){
        if(tmp_SymbolTableEntry == NULL){
            break;
        }

        if((tmp_SymbolTableEntry->type == GLOBAL)||(tmp_SymbolTableEntry->type == LOCAL_T)||(tmp_SymbolTableEntry->type == FORMAL)){
            if((strcmp(tmp_SymbolTableEntry->value.varVal->name , id)==0)){
                return tmp_SymbolTableEntry;
            }
        }else{
            if((strcmp(tmp_SymbolTableEntry->value.funcVal->name , id)==0)){
                return tmp_SymbolTableEntry;
            }
        }

        tmp_SymbolTableEntry = tmp_SymbolTableEntry->prev_list_scope;
    }

    return NULL;
}


/* ***************************************************************************** */




SymbolTableEntry * SymbolTableLookUp_ALL_FROM_THIS(char * id , int scope){
    lookUpAllUserFunctFound = 0 ;
    printf("SymbolTableLookUp_ALL_FROM_THIS Search for %s \n",id);
    ScopeListEntries * tmp_ScopeList = ScopeList->head;
    SymbolTableEntry * tmp_SymbolTableEntry;
    if(SymbolTableLookUp( id , scope) == NULL){
        return NULL;
    }

    while(scope!=-1){
	tmp_ScopeList = ScopeList->head;
    	while((tmp_ScopeList!=NULL)&&(tmp_ScopeList->scopeVal != scope)){
		tmp_ScopeList = tmp_ScopeList->next_list_node;
	}
	if((tmp_ScopeList!=NULL)&&(tmp_ScopeList->scopeVal == scope)){
		break;
	}
	scope--;
    }

    if(tmp_ScopeList == NULL){
	printf("SCOPE : %d is null at this time SymbolTableLookUp_ALL_FROM_THIS \n",scope);
        return NULL ;
    }

    tmp_SymbolTableEntry = tmp_ScopeList->end_of_scope_i;

    while(1){
        if(tmp_SymbolTableEntry == NULL){
            tmp_ScopeList = tmp_ScopeList->next_list_node;
            if(tmp_ScopeList==NULL){
                break;
            }else{
                tmp_SymbolTableEntry = tmp_ScopeList->end_of_scope_i;
            }
        }

        if((tmp_SymbolTableEntry->type == GLOBAL)||(tmp_SymbolTableEntry->type == LOCAL_T)||(tmp_SymbolTableEntry->type == FORMAL)){
            if((strcmp(tmp_SymbolTableEntry->value.varVal->name , id)==0)&&(tmp_SymbolTableEntry->isActive == true)){
                return tmp_SymbolTableEntry;
            }
        }else{
            if((tmp_SymbolTableEntry->type == USERFUNC)&&(tmp_SymbolTableEntry->isActive == true)){
                lookUpAllUserFunctFound = 1 ;
            }else{
                printf("%s , %d\n",tmp_SymbolTableEntry->value.funcVal->name,tmp_SymbolTableEntry->type);
            }

            if((strcmp(tmp_SymbolTableEntry->value.funcVal->name , id)==0)&&(tmp_SymbolTableEntry->isActive == true)){
                return tmp_SymbolTableEntry;
            }
        }

        tmp_SymbolTableEntry = tmp_SymbolTableEntry->prev_list_scope;
    }

    return NULL;

}

SymbolTableEntry * SymbolTableLookUp_THIS(char * id ,int scope ){
    ScopeListEntries * tmp_ScopeList = ScopeList->head;
    SymbolTableEntry * tmp_SymbolTableEntry;

    if(SymbolTableLookUp( id , scope) == NULL){
        return NULL;
    }

    while((tmp_ScopeList!=NULL)&&(tmp_ScopeList->scopeVal != scope)){
        tmp_ScopeList = tmp_ScopeList->next_list_node;
    }

    if(tmp_ScopeList == NULL){
        printf("SCOPE : %d is null at this time SymbolTableLookUp_THIS \n",scope);
        return NULL ;
    }

    tmp_SymbolTableEntry = tmp_ScopeList->end_of_scope_i;

    while(1){
        if(tmp_SymbolTableEntry == NULL){
            break;
        }

        if((tmp_SymbolTableEntry->type == GLOBAL)||(tmp_SymbolTableEntry->type == LOCAL_T)||(tmp_SymbolTableEntry->type == FORMAL)){
            if((strcmp(tmp_SymbolTableEntry->value.varVal->name , id)==0)&&(tmp_SymbolTableEntry->isActive == true)){
                return tmp_SymbolTableEntry;
            }
        }else{
            if((strcmp(tmp_SymbolTableEntry->value.funcVal->name , id)==0)&&(tmp_SymbolTableEntry->isActive == true)){
                return tmp_SymbolTableEntry;
            }
        }

        tmp_SymbolTableEntry = tmp_SymbolTableEntry->prev_list_scope;
    }

    return NULL;
}

/*typedef enum scopespace_t{
    programvar,
    formalarg,
    functionlocal
}scopespace_t;*/

char * scopespaceArray[] ={
    "programvar",
    "formalarg",
    "functionlocal"
};


/*typedef enum symbol_t{
    var_s,
    programfunc_s,
    libraryfunc_s
}symbol_t;*/

char * symbolArray[]={
    "var_s",
    "programfunc_s",
    "libraryfunc_s"
};

/*enum SymbolTableType{
    GLOBAL=0,
    LOCAL_T,
    FORMAL,
    USERFUNC,
    LIBFUNC
};*/

char *  SymbolTableTypeArray[]={
    "GLOBAL",
    "LOCAL_T",
    "FORMAL",
    "USERFUNC",
    "LIBFUNC"
};

//////////////////////////////////////////////////////////////////////////////////////
// SUMBASH_1 : GIA SUNARTHSEIS XWRIS ONOMA 8A PERNAME SAN PARAMETRO TO "$_ANONYMOUS_$"
// SE PERIPTWSH POU DW8EI SCOPE==0 GIA METABLHTH BAZOUME AYTOMATA TYPE GLOBAL
/////////////////////////////////////////////////////////////////////////////////////

SymbolTableEntry *  SymbolTableInsert(char * id , int scope , int line ,enum SymbolTableType type , enum symbol_t s_type , enum scopespace_t scopeSpace ,
                                      unsigned  scopeOffset ){
    int symTablePos ,digits=0 ,n;
    ScopeListEntries * new_scope_list_entry=NULL;
    ScopeListEntries *tmp_ScopeListEntry , *prev_ScopeListEntry ;
    SymbolTableEntry *tmp_SymbolTableEntry ;
    char numberToString[20];
    char *name;
    Variable * new_variable;
    Function * new_function;
    SymbolTableEntry *new_sym_table_entry , *sym_table_list ;

    printf("1 : insert name %s ,scope %d , line %d , %s , %s ,scopespace  %s , offset %d \n",id,scope,line,SymbolTableTypeArray[type] , symbolArray[s_type],scopespaceArray[scopeSpace] ,scopeOffset);

    if(strcmp(id,"$_ANONYMOUS_$")== 0){
        numberOfAnonymousFunctions++;
        n = numberOfAnonymousFunctions;

        while(n!=0){
            n=n/10;
            ++digits;
        }

        sprintf(numberToString, "%d", numberOfAnonymousFunctions);
        numberToString[digits]='\0';
        id = (char*)malloc(2+ digits +1);
        id[0]='_';
        id[1]='F';
        id[2]='\0';
        strcat(id,numberToString);

    }

    symTablePos = SymbolTableHashFunction(id);

    new_sym_table_entry = (SymbolTableEntry * )malloc(sizeof(SymbolTableEntry));
    new_sym_table_entry->isActive = true;
    new_sym_table_entry->s_type = s_type;

    if((type == GLOBAL)||(type == LOCAL_T)||(type == FORMAL)){
        new_sym_table_entry->value.varVal = (Variable*)malloc(sizeof(Variable));
        new_sym_table_entry->value.varVal->line = line;
        new_sym_table_entry->value.varVal->scope = scope;
        name = (char*)malloc(strlen(id)+1);
        name[0]='\0';
        strcpy(name,id);
        new_sym_table_entry->value.varVal->name = name;
        new_sym_table_entry->value.varVal->scopeSpace = scopeSpace;
        new_sym_table_entry->value.varVal->scopeOffset = scopeOffset;
    }else{
        new_sym_table_entry->value.funcVal = (Function*)malloc(sizeof(Function));
        new_sym_table_entry->value.funcVal->head = NULL ;
        new_sym_table_entry->value.funcVal->line = line;
        new_sym_table_entry->value.funcVal->scope = scope;
        new_sym_table_entry->value.funcVal->iaddress = -1;
        new_sym_table_entry->value.funcVal->totallocals = 0;

        if(id[0] == '_'){
            new_sym_table_entry->value.funcVal->name = id;
        }else{
            name = (char*)malloc(strlen(id)+1);
            name[0]='\0';
            strcpy(name,id);
            new_sym_table_entry->value.funcVal->name=name ;
        }
    }

    if((type == LOCAL_T)&&(scope == 0)){
        new_sym_table_entry->type = GLOBAL;
    }else{
        new_sym_table_entry->type = type;
    }



    new_sym_table_entry->next_list_scope = NULL;
    new_sym_table_entry->prev_list_scope = NULL;
    new_sym_table_entry->next_list_entry = SymbolTable[symTablePos];
    SymbolTable[symTablePos] = new_sym_table_entry;


    //To topo8etoume kai sthn scope list sto katalhlo scope
    //koitazoume arxika an yparxei kombos gia to sugkekrimeno scope , an oxi ton ftiaxnoume

    tmp_ScopeListEntry = ScopeList->head;
    while(tmp_ScopeListEntry){
        if(scope == tmp_ScopeListEntry->scopeVal){
            break;
        }
        tmp_ScopeListEntry = tmp_ScopeListEntry->next_list_node;
    }

    //tote 8a prepei na eisagoume ena neo kombo gia to scope
    if((tmp_ScopeListEntry==NULL)||(tmp_ScopeListEntry->scopeVal< scope)){

        new_scope_list_entry= (ScopeListEntries*)malloc(sizeof(ScopeListEntries));
        new_scope_list_entry->start_of_scope_i = NULL;
        new_scope_list_entry->end_of_scope_i = NULL;
        new_scope_list_entry->scopeVal = scope;
        new_scope_list_entry->next_list_node=NULL;


        prev_ScopeListEntry = NULL;
        tmp_ScopeListEntry = ScopeList->head;
        while((tmp_ScopeListEntry!=NULL)&& (scope < tmp_ScopeListEntry->scopeVal)){
            prev_ScopeListEntry = tmp_ScopeListEntry;
            tmp_ScopeListEntry = tmp_ScopeListEntry->next_list_node;
        }


        if(prev_ScopeListEntry == NULL){
            if(ScopeList->head == NULL){
                ScopeList->head = new_scope_list_entry;
            }else{
                new_scope_list_entry->next_list_node = ScopeList->head;
                ScopeList->head = new_scope_list_entry;
            }
        }else{
            new_scope_list_entry->next_list_node = prev_ScopeListEntry->next_list_node;
            prev_ScopeListEntry->next_list_node = new_scope_list_entry;
        }
    }


    //tote shmainei oti den exoume ballei kainourgio kombo opote prepei na ton psa3oume
    if(new_scope_list_entry == NULL){
        tmp_ScopeListEntry = ScopeList->head;
        while(tmp_ScopeListEntry){
            if(tmp_ScopeListEntry->scopeVal == scope){
                break;
            }
            tmp_ScopeListEntry = tmp_ScopeListEntry->next_list_node;
        }
    }else{
        tmp_ScopeListEntry = new_scope_list_entry;
    }
    //twra prepei na kanoume kai thn sundesh aytou toy kombou me tous allous se ayto to scope

    tmp_SymbolTableEntry = tmp_ScopeListEntry->end_of_scope_i ;

    if(tmp_ScopeListEntry->start_of_scope_i == NULL){
        tmp_ScopeListEntry->start_of_scope_i = new_sym_table_entry;
    }else{
        tmp_ScopeListEntry->end_of_scope_i->next_list_scope = new_sym_table_entry;
        new_sym_table_entry->prev_list_scope = tmp_ScopeListEntry->end_of_scope_i;
    }

    tmp_ScopeListEntry->end_of_scope_i = new_sym_table_entry;


    /////////////////////////////////////////////

    printf("2 : insert name %s ,scope %d , line %d , %s , %s ,scopespace  %s , offset %d \n",id,scope,line,SymbolTableTypeArray[new_sym_table_entry->type] , symbolArray[s_type],scopespaceArray[scopeSpace] ,scopeOffset);

    if((scopeSpace == programvar)&&(s_type == var_s)){
        numberOfGlobalVars++;
    }


    /////////////////////////////////////////////

    return new_sym_table_entry ;
}


void SymbolTable_scopeList_Initialize(){
    int i = 0 ;
    SymbolTable = (SymbolTableEntry**)malloc(sizeof(SymbolTableEntry*)*SYMBOL_TABLE_SIZE);

    for(i=0 ; i<SYMBOL_TABLE_SIZE ; i++){
        SymbolTable[i] = NULL;
    }

    ScopeList = (ScopeListNode*)malloc(sizeof(ScopeListNode));
    ScopeList->head = NULL ;

    //Topo8etoume tis LIBRARY synarthseis se scope 0
    SymbolTableInsert("print",0,0,LIBFUNC,libraryfunc_s,0 ,-1);
    SymbolTableInsert("input",0,0,LIBFUNC,libraryfunc_s,0 ,-1);
    SymbolTableInsert("objectmemberkeys",0,0,LIBFUNC,libraryfunc_s,0 ,-1);
    SymbolTableInsert("objecttotalmembers",0,0,LIBFUNC,libraryfunc_s,0 ,-1);
    SymbolTableInsert("objectcopy",0,0,LIBFUNC,libraryfunc_s,0 ,-1);
    SymbolTableInsert("totalarguments",0,0,LIBFUNC,libraryfunc_s,0 ,-1);
    SymbolTableInsert("argument",0,0,LIBFUNC,libraryfunc_s,0 ,-1);
    SymbolTableInsert("typeof",0,0,LIBFUNC,libraryfunc_s,0 ,-1);
    SymbolTableInsert("strtonum",0,0,LIBFUNC,libraryfunc_s,0 ,-1);
    SymbolTableInsert("sqrt",0,0,LIBFUNC,libraryfunc_s,0 ,-1);
    SymbolTableInsert("cos",0,0,LIBFUNC,libraryfunc_s,0 ,-1);
    SymbolTableInsert("sin",0,0,LIBFUNC,libraryfunc_s,0 ,-1);

    numberOfAnonymousFunctions = 0 ;
    lookUpAllUserFunctFound = 0 ;
    numberOfGlobalVars  = 0 ;
    return ;
}



void SymbolTablePrintAll(){
    int i = 0;
    SymbolTableEntry *tmp_SymbolTable  ;

    printf("SYMBOL TABLE\n");
    for(i=0 ; i< SYMBOL_TABLE_SIZE;i++){
        tmp_SymbolTable = SymbolTable[i];
        if(tmp_SymbolTable!=NULL)
            printf("\nSYMBOLTABLE LINE %d\n\n",i);

        while(tmp_SymbolTable){
            if((tmp_SymbolTable->type == GLOBAL)||(tmp_SymbolTable->type == LOCAL_T)||(tmp_SymbolTable->type == FORMAL))
                printf("VARIABLE Name : %s , Scope : %d , Programm line : %d\n",tmp_SymbolTable->value.varVal->name,tmp_SymbolTable->value.varVal->scope,tmp_SymbolTable->value.varVal->line);
            else
                printf("FUNCTION Name : %s , Scope : %d , Programm line : %d\n",tmp_SymbolTable->value.funcVal->name,tmp_SymbolTable->value.funcVal->scope,tmp_SymbolTable->value.funcVal->line);

            tmp_SymbolTable = tmp_SymbolTable->next_list_entry;
        }
    }

    return;
}

void ScopePrintAll(){
    int i = 0;
    ScopeListEntries *tmp_ScopeList = ScopeList->head ;
    SymbolTableEntry *tmp_SymbolTableEntry;

    printf("SCOPE LIST\n");
    while(tmp_ScopeList){

        printf("\n SCOPE %d\n\n",tmp_ScopeList->scopeVal);

        tmp_SymbolTableEntry = tmp_ScopeList->start_of_scope_i ;
        while(tmp_SymbolTableEntry){

            if((tmp_SymbolTableEntry->type == GLOBAL)||(tmp_SymbolTableEntry->type == LOCAL_T)||(tmp_SymbolTableEntry->type == FORMAL))
                printf("VARIABLE Name : %s , Scope : %d , Programm line : %d\n",tmp_SymbolTableEntry->value.varVal->name,tmp_SymbolTableEntry->value.varVal->scope,tmp_SymbolTableEntry->value.varVal->line);
            else
                printf("FUNCTION Name : %s , Scope : %d , Programm line : %d\n",tmp_SymbolTableEntry->value.funcVal->name,tmp_SymbolTableEntry->value.funcVal->scope,tmp_SymbolTableEntry->value.funcVal->line);


            tmp_SymbolTableEntry = tmp_SymbolTableEntry->next_list_scope;
        }
        tmp_ScopeList = tmp_ScopeList->next_list_node;
    }

    return;

}


int SymbolTableHide(int scope){
    printf("=====HIDE %d=====\n",scope);
    ScopeListEntries * tmp_ScopeList = ScopeList->head;
    SymbolTableEntry * tmp_SymbolTableEntry;

    while((tmp_ScopeList)&&(tmp_ScopeList->scopeVal != scope)){
        tmp_ScopeList = tmp_ScopeList->next_list_node;
    }

    if(tmp_ScopeList == NULL){
        printf("SCOPE : %d is null at this time  SymbolTableHide \n",scope);
        return 0 ;
    }

    tmp_SymbolTableEntry = tmp_ScopeList->start_of_scope_i;

    while(tmp_SymbolTableEntry){
        tmp_SymbolTableEntry->isActive = false ;
        tmp_SymbolTableEntry = tmp_SymbolTableEntry->next_list_scope;
    }
    return 0 ;
}
