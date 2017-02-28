#include "libraryFunctions.h"





/*=========================================================*/
/*====================TO=STRING=FUNCTIONS==================*/
/*=========================================================*/


tostring_func_t tostringFuncs[]={
    number_tostring,
    string_tostring,
    bool_tostring,
    table_tostring,
    userfunc_tostring,
    libfunc_tostring,
    nil_tostring,
    undef_tostring
};


char *number_tostring(avm_memcell *m){
    char *s = (char*)malloc(100*sizeof(char));

    sprintf(s ,"%.3lf" , m->data.numVal);

    return s;
}

char *string_tostring(avm_memcell *m){
    char *s = strdup(m->data.strVal);
    return s;
}

char *bool_tostring(avm_memcell *m){
    char *s = (char*)malloc(6*sizeof(char));

    if(m->data.boolVal==0) {
        strcpy(s,"false");
        s[5] = '\0';
    }else{
         strcpy(s ,"true");
         s[4] = '\0';
    }
    return s;

}


char * tableValue_ToString(avm_memcell *m){
    avm_memcell_t type = m->type;

    if(type == number_m ){
        return number_tostring(m);

    }else if(type == string_m){
        return string_tostring(m);

    }else if (type == bool_m){
        return bool_tostring(m);

    }else if (type == table_m ){
        return table_tostring(m);

    }else if (type == userfunc_m ){
        return userfunc_tostring(m);

    }else if (type == libfunc_m){
        return libfunc_tostring(m);

    }else if (type == nil_m ){
        return nil_tostring(m);

    }else if (type == undef_m){
        return undef_tostring(m);

    }else{
        assert(0);
    }
}

char * tableKey_ToString(avm_memcell *m){
    avm_memcell_t type = m->type;

    if(type == number_m ){

        return number_tostring(m);

    }else if(type == string_m){

        return string_tostring(m);

    }else if (type == bool_m){

        return bool_tostring(m);

    }else if (type == userfunc_m ){

        return userfunc_tostring(m);

    }else if (type == libfunc_m){

        return libfunc_tostring(m);

    }else if(type == table_m){
        char *s = (char*)malloc(30*sizeof(char));
        strcpy(s , "Table with ID : ");

        sprintf(s , "%u" , m->data.tableVal->tableIDnumber);
        return s;

    }else if (type == nil_m ){
        avm_error("Try to get key->type == nil ",currLine, NULL ,NULL);

    }else if(type == undef_m ){
        avm_error("Try to get key->type == undef ",currLine, NULL ,NULL);

    }else {
        printf("Not supported table key type 'avm_tablegetelem' \n");
        assert(0);

    }
}



char * table_tostring_HELP(avm_table_bucket **p , int size , char * printTable ){
    int i;
    char * L_Curly_B  = " { ";
    char * R_Curly_B  = " } ";
    char * upDownDot  = " : ";
    char * valueToString , *keyToString ;
    avm_table_bucket * tmpList;


    for( i = 0 ; i<size ; i++){
        tmpList = p[i];
        while(tmpList){

            /*Grafoume to { */
            printTable = (char *)realloc(printTable , strlen(printTable)+ strlen(L_Curly_B) + 1);
            strcat(printTable , L_Curly_B );

            /*Kanoume to key string */
            keyToString = tableKey_ToString(tmpList->key);

            /*Kanoume to value string */
            valueToString = tableValue_ToString(tmpList->value);
            tmpList = tmpList->next;


            /*Grafoume to key  */
            printTable = (char *)realloc(printTable , strlen(printTable)+ strlen(keyToString) + 1);
            strcat(printTable , keyToString );

            /*Grafoume to : */
            printTable = (char *)realloc(printTable , strlen(printTable)+ strlen(upDownDot) + 1);
            strcat(printTable , upDownDot );

            /*Grafoume to value  */
            printTable = (char *)realloc(printTable , strlen(printTable)+ strlen(valueToString) + 1);
            strcat(printTable , valueToString );

            /*Grafoume to } */
            printTable = (char *)realloc(printTable , strlen(printTable)+ strlen(R_Curly_B) + 1);
            strcat(printTable , R_Curly_B );

        }
    }

    return printTable;
}




char *table_tostring(avm_memcell *m){
    char * printTable ;
    char * L_Square_B = " [ ";
    char * R_Square_B = " ] ";

    avm_table * table = m->data.tableVal;


    printTable = (char *)malloc(sizeof(char)*(strlen(L_Square_B) + 1));
    strcpy(printTable , L_Square_B);

    printTable = table_tostring_HELP(table->numIndexed ,AVM_TABLE_HASHSIZE  , printTable);

    printTable = table_tostring_HELP(table->strIndexed ,AVM_TABLE_HASHSIZE  , printTable);

    printTable = table_tostring_HELP(table->libraryfuncIndexed ,AVM_TABLE_HASHSIZE  , printTable);

    printTable = table_tostring_HELP(table->userfuncIndexed ,AVM_TABLE_HASHSIZE  , printTable);
    printTable = table_tostring_HELP(table->booleanIndexed ,AVM_TABLE_BOOLEAN_HASHSIZE  , printTable);

    printTable = (char *)realloc(printTable , strlen(printTable)+ strlen(R_Square_B) + 1);
    printTable = strcat(printTable , R_Square_B );

    return printTable ;
}

char *userfunc_tostring(avm_memcell *m){
    char numberToString[20];
    char * tmp = "User function : ";
    char * printText;

    sprintf(numberToString, "%u", m->data.funcVal);

    printText = (char *)malloc(sizeof(char)*(strlen(tmp)+strlen(numberToString) + 1 ));

    strcpy(printText , tmp);
    strcpy(printText , numberToString);

    return printText;
}

char *libfunc_tostring(avm_memcell *m){
    char * tmp = "Library function : ";
    char * printText;

    printText = (char *)malloc(sizeof(char)*(strlen(tmp)+strlen(m->data.libfuncVal) + 1 ));

    strcpy(printText , tmp);
    strcpy(printText , m->data.libfuncVal);

    return printText;

}

char *nil_tostring(avm_memcell *m){
    char *s = (char *)malloc(sizeof(char)*4);
    strcpy(s,"nil");
    return s;
}

char *undef_tostring(avm_memcell *m){
    char *s = (char *)malloc(sizeof(char)*6);
    strcpy(s,"UNDEF");
    return s;
}




char *avm_tostring(avm_memcell *m){
    assert(m->type>=0 && m->type<=undef_m);

    return (*tostringFuncs[m->type])(m);
}




/*=========================================================*/


void libfunc_print(void){
    unsigned n = avm_totalactuals();
    unsigned i;

    for( i=0; i<n; ++i){
        char *s = avm_tostring(avm_getactual(i));
        printf("%s",s);
        free(s);
    }
    return;
}



void libfunc_typeof(void){
    char string[100];
    unsigned n = avm_totalactuals();

    if(n!=1){
        sprintf(string ,"one argument (not %u) expected in 'typeof'!",n);
        avm_error(string,currLine , NULL , NULL);
    }else{
        avm_memcellclear(&retval);
        retval.type=string_m;
        retval.data.strVal = strdup(typeStrings[avm_getactual(0)->type]);
    }

    return;
}



void libfunc_totalarguments(void){

    unsigned p_topsp ;
    avm_memcellclear(&retval);

    if(G_inFunction == 0 ){
        avm_warning("totalarguments() called outside a function!" , currLine , NULL , NULL);
        retval.type = nil_m;
    }
    else{
        p_topsp = avm_get_envvalue(topsp+AVM_SAVEDTOPSP_OFFSET);
        retval.type = number_m;
        retval.data.numVal = avm_get_envvalue(p_topsp+ AVM_NUMACTUALS_OFFSET);
    }

    return;
}


void libfunc_argument(void){
    unsigned i, n, t;
    unsigned p_topsp;
    char string[100];

    avm_memcellclear(&retval);

    if(G_inFunction == 0 ){
        avm_warning("argument() called outside a function!",currLine , NULL , NULL);
        retval.type=nil_m;
    }else{
        n = avm_totalactuals();
        if(n!=1){
            sprintf(string ,"one argument (not %u) expected in 'argument()'!",n);
            avm_error(string,currLine , NULL , NULL);

        }else{
            p_topsp = avm_get_envvalue(topsp+AVM_SAVEDTOPSP_OFFSET);
            i = avm_get_envvalue(topsp + AVM_STACKENV_SIZE + 1);        //number of argument to take it
            t = avm_get_envvalue(p_topsp+ AVM_NUMACTUALS_OFFSET);       //number of args in caller

            if(i>=t || i<0){
                sprintf(string ,"caller of argument doesn't have  %u argument!",i);
                avm_error(string,currLine , NULL , NULL);

            }else{
                avm_assign(&retval,&stack[p_topsp+ AVM_NUMACTUALS_OFFSET+1+i], currLine);
            }
        }
    }
    return;
}



void libfunc_cos(void){
    char string[100];
    unsigned n = avm_totalactuals();
    unsigned t;

    if(n!=1){
        sprintf(string ,"one argument (not %u) expected in 'cos()'!",n);
        avm_error(string,currLine , NULL , NULL);

    }else{
        t = avm_get_envvalue(topsp+ AVM_STACKENV_SIZE+1);
        avm_memcellclear(&retval);

        retval.type = number_m;
        retval.data.numVal = cos(t);
    }

    return;
}

void libfunc_sin(void){
    char string[100];
    unsigned n = avm_totalactuals();
    unsigned t;

    if(n!=1){
        sprintf(string ,"one argument (not %u) expected in 'sin()'!",n);
        avm_error(string,currLine , NULL , NULL);

    }else{
        t = avm_get_envvalue(topsp+ AVM_STACKENV_SIZE+1);
        avm_memcellclear(&retval);

        retval.type = number_m;
        retval.data.numVal = sin(t);
    }

    return;
}




void libfunc_sqrt(void){
    char string[100];
    unsigned n = avm_totalactuals();
    unsigned t;

    if(n!=1){
        sprintf(string ,"one argument (not %u) expected in 'sqrt()'!",n);
        avm_error(string,currLine , NULL , NULL);

    }else{

        if(checkIFisNegativeNum(topsp+ AVM_STACKENV_SIZE+1)){
            sprintf(string ,"positive argument (not %u) expected in 'sqrt'!",t);
            avm_warning(string,currLine , NULL , NULL);

            retval.type = nil_m;

        }else{
            t = avm_get_envvalue(topsp+ AVM_STACKENV_SIZE+1);
            avm_memcellclear(&retval);
            retval.type = number_m;

            retval.data.numVal = sqrt(t);

        }
    }
    return;
}




void libfunc_input(void){
    double b;
    int isNumber = 1 ;
    int seeDot = 0;
    char buffer [10000];
    int i;

    printf("\nGive some input\n");
    gets (buffer);
    avm_memcellclear(&retval);

    if(strcmp(buffer,"false") == 0){
        retval.type=bool_m;
        retval.data.boolVal = false;

    }else if (strcmp(buffer,"true") == 0){
        retval.type=bool_m;
        retval.data.boolVal = true;

    }else if (strcmp(buffer,"nil") == 0){
        retval.type=nil_m;

    }else{
        if(!isdigit(buffer[0])){
            isNumber = 0 ;
        }else{
            for( i = 1; i<strlen(buffer); i++){
                if(!isdigit(buffer[i])){
                    if((buffer[i] == '.')){
                        if(seeDot){
                            isNumber = 0 ;
                            break;
                        }else{
                            seeDot = 1;
                        }
                    }else{
                        isNumber = 0 ;
                        break;
                    }
                }
            }
        }

        if(isNumber){
            retval.type=number_m;
            retval.data.numVal=atof(buffer);
        }else {
            retval.type=string_m;
            retval.data.strVal=strdup(buffer);
        }
    }

    return;
}


void libfunc_strtonum(void){
    char string[100];
    unsigned n = avm_totalactuals();
    int i;
    double d ;
    char *ptr = NULL , * buffer;

    avm_memcellclear(&retval);

    if(n!=1){
        sprintf(string ,"one argument (not %u) expected in 'strtonum()'!",n);
        avm_error(string,currLine , NULL , NULL);

    }else{
        if(avm_getactual(0)->type!=string_m){
            avm_error("expected string as argument in 'strtonum'!" ,currLine , NULL , NULL);

        }else{
            i = strlen(avm_getactual(0)->data.strVal);
            buffer = malloc(i*sizeof(char)+1);
            buffer = strdup(avm_getactual(0)->data.strVal);

            d = strtod(buffer, &ptr);

            if(*ptr == '\0'){
                retval.type = number_m;
                retval.data.numVal = d;
            }else{
                retval.type = nil_m;
            }

        }
    }
    return;
}




void table_BucketCopy(avm_table * table , avm_table_bucket **p , int size  ){
    int i;
    char * valueToString , *keyToString ;
    avm_table_bucket * tmpList;


    for( i = 0 ; i<size ; i++){
        tmpList = p[i];
        while(tmpList){
            avm_tablesetelem(table , tmpList->key, tmpList->value);
            tmpList = tmpList->next;
        }
    }

    return;
}


void libfunc_objectcopy(void){
    char string[100];
    unsigned n = avm_totalactuals();
    avm_table * tableVal;
    avm_table * newTable;
    if(n!=1){
        sprintf(string ,"one argument (not %u) expected in 'libfunc_objectcopy'!",n);
        avm_error(string,currLine , NULL , NULL);
    }else{
        if(avm_getactual(0)->type!=table_m){
            avm_error("expected string as argument in 'table_m'!" ,currLine , NULL , NULL);
        }else{
            tableVal =  avm_getactual(0)->data.tableVal ;
            newTable = avm_tablenew();

            table_BucketCopy(newTable , tableVal->numIndexed ,AVM_TABLE_HASHSIZE );
            table_BucketCopy(newTable , tableVal->strIndexed ,AVM_TABLE_HASHSIZE  );
            table_BucketCopy(newTable , tableVal->libraryfuncIndexed ,AVM_TABLE_HASHSIZE );
            table_BucketCopy(newTable , tableVal->userfuncIndexed ,AVM_TABLE_HASHSIZE );
            table_BucketCopy(newTable , tableVal->booleanIndexed ,AVM_TABLE_BOOLEAN_HASHSIZE );
            table_BucketCopy(newTable , tableVal->tableIndex , AVM_TABLE_HASHSIZE );

            avm_memcellclear(&retval);
            retval.type = table_m;
            retval.data.tableVal = newTable;

        }
    }

    return;
}

char * typeArray2[] = {
 "number_m"       ,
 "string_m"       ,
 "bool_m"         ,
 "table_m"        ,
 "userfunc_m"     ,
 "libfunc_m"     ,
 "nil_m"        ,
 "undef_m"
};

void libfunc_objecttotalmembers(void){
    char string[100];
    unsigned n = avm_totalactuals();
    avm_table * tableVal;

    if(n!=1){
        sprintf(string ,"one argument (not %u) expected in 'tablelength'!",n);
        avm_error(string,currLine , NULL , NULL);
    }else{
        if(avm_getactual(0)->type!=table_m){
            avm_error("expected table as argument in 'table_m'!" ,currLine , NULL , NULL);
        }else{
            tableVal =  avm_getactual(0)->data.tableVal ;

            avm_memcellclear(&retval);
            retval.type = number_m;
            retval.data.numVal = tableVal->total;
        }
    }

    return;

}


int table_BucketCopy_memberkeys(avm_table * table , avm_table_bucket **p , int size  , int pos){
    int i;
    char * valueToString , *keyToString ;
    avm_table_bucket * tmpList;
    avm_memcell * posMemcell = (avm_memcell *)malloc(sizeof(avm_memcell));
    posMemcell->type = number_m;

    for( i = 0 ; i<size ; i++){
        tmpList = p[i];
        while(tmpList){
            posMemcell->data.numVal = pos;
            avm_tablesetelem(table , posMemcell, tmpList->key);
            pos++;
            tmpList = tmpList->next;
            //if(i==1)
            //exit(0);
        }
    }

    return pos;
}



void libfunc_objectmemberkeys(void){
    char string[100];
    unsigned n = avm_totalactuals();
    avm_table * tableVal;
    avm_table * newTable;
    int nextPos = 0;
    if(n!=1){
        sprintf(string ,"one argument (not %u) expected in 'objectmemberkeys'!",n);
        avm_error(string,currLine , NULL , NULL);
    }else{
        if(avm_getactual(0)->type!=table_m){
            avm_error("expected string as argument in 'table_m'!" ,currLine , NULL , NULL);
        }else{
            tableVal =  avm_getactual(0)->data.tableVal ;
            newTable = avm_tablenew();

            table_tostring(avm_getactual(0));

            nextPos = table_BucketCopy_memberkeys(newTable , tableVal->numIndexed ,AVM_TABLE_HASHSIZE , nextPos);

            nextPos = table_BucketCopy_memberkeys(newTable , tableVal->strIndexed ,AVM_TABLE_HASHSIZE , nextPos );

            nextPos = table_BucketCopy_memberkeys(newTable , tableVal->libraryfuncIndexed ,AVM_TABLE_HASHSIZE , nextPos);
            nextPos = table_BucketCopy_memberkeys(newTable , tableVal->userfuncIndexed ,AVM_TABLE_HASHSIZE , nextPos);
            nextPos = table_BucketCopy_memberkeys(newTable , tableVal->booleanIndexed ,AVM_TABLE_BOOLEAN_HASHSIZE , nextPos);
            nextPos = table_BucketCopy_memberkeys(newTable , tableVal->tableIndex , AVM_TABLE_HASHSIZE , nextPos);
            avm_memcellclear(&retval);
            retval.type = table_m;
            retval.data.tableVal = newTable;

        }
    }

    return;
}
