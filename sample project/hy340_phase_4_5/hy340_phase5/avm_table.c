#include "libraryFunctions.h"

unsigned int numHashFunction(double numVal){
    return ((int)numVal)%AVM_TABLE_HASHSIZE;
}

unsigned int UserFuncHashFunction(unsigned funcVal){
    return funcVal%AVM_TABLE_HASHSIZE;
}

unsigned int BoolHashFunction(bool boolVal){
    if(boolVal){
        return 1;
    }else{
        return 0;
    }
}

unsigned int StringHashFunction(const char *id){
  int i ;
  unsigned int hash = 0;
  for (i = 0; id[i] != '\0'; i++)
    hash = hash * 65599 + id[i];
  return (hash%AVM_TABLE_HASHSIZE);
}

avm_table_bucket * NumLookUp(avm_table_bucket ** t , avm_memcell *key){
    unsigned int pos;
    avm_table_bucket * bucketList ;

    pos = numHashFunction(key->data.numVal);

    bucketList = t[pos];

    while(bucketList){
        if(bucketList->key->data.numVal == key->data.numVal){
            return bucketList;
        }
        bucketList = bucketList->next;
    }

    return NULL;
}


avm_table_bucket * UserFuncLookUp(avm_table_bucket ** t , avm_memcell *key){
    unsigned int pos;
    avm_table_bucket * bucketList ;

    pos = UserFuncHashFunction(key->data.funcVal);

    bucketList = t[pos];

    while(bucketList){
        if(bucketList->key->data.funcVal == key->data.funcVal){
            return bucketList;
        }
        bucketList = bucketList->next;
    }

    return NULL;
}

avm_table_bucket * BoolLookUp(avm_table_bucket ** t , avm_memcell *key){
    unsigned int pos;
    avm_table_bucket * bucketList ;

    pos = BoolHashFunction(key->data.boolVal);

    bucketList = t[pos];

    while(bucketList){
        if(bucketList->key->data.boolVal == key->data.boolVal){
            return bucketList;
        }
        bucketList = bucketList->next;
    }

    return NULL;
}


avm_table_bucket * StringLookUp(avm_table_bucket ** t , avm_memcell *key){
    unsigned int pos;
    avm_table_bucket * bucketList ;

    pos = StringHashFunction(key->data.strVal);

    bucketList = t[pos];

    while(bucketList){
        if(strcmp(bucketList->key->data.strVal , key->data.strVal)==0){
            return bucketList;
        }
        bucketList = bucketList->next;
    }

    return NULL;
}


avm_table_bucket * LibLookUp(avm_table_bucket ** t , avm_memcell *key){
    unsigned int pos;
    avm_table_bucket * bucketList ;

    pos = LibHashFunction(key->data.libfuncVal);

    bucketList = t[pos];

    while(bucketList){
        if(strcmp(bucketList->key->data.libfuncVal , key->data.libfuncVal)==0){
            return bucketList;
        }
        bucketList = bucketList->next;
    }

    return NULL;
}


avm_table_bucket * TableLookUp(avm_table_bucket ** t , avm_memcell *key){
    unsigned int pos;
    avm_table_bucket * bucketList ;

    pos = numHashFunction(key->data.tableVal->tableIDnumber);

    bucketList = t[pos];

    while(bucketList){
        if(bucketList->key->data.tableVal->tableIDnumber == key->data.tableVal->tableIDnumber){
            return bucketList;
        }
        bucketList = bucketList->next;
    }

    return NULL;
}

avm_memcell * tableValue_Copy(avm_memcell *m){
    avm_memcell *newValue = (avm_memcell*)malloc(sizeof(avm_memcell));
    avm_memcell_t type = m->type;

    newValue->type = type ;

    if(type == number_m ){
        newValue->data.numVal = m->data.numVal;

    }else if(type == string_m){
        newValue->data.strVal = (char *)malloc(sizeof(char)*(strlen(m->data.strVal)+1));
        strcpy(newValue->data.strVal , m->data.strVal);

    }else if (type == bool_m){
        newValue->data.boolVal = m->data.boolVal;

    }else if (type == table_m ){
        newValue->data.tableVal = m->data.tableVal;
        avm_tableincrefcounter(m->data.tableVal);

    }else if (type == userfunc_m ){
        newValue->data.funcVal = m->data.funcVal;

    }else if (type == libfunc_m){
        newValue->data.libfuncVal = (char *)malloc(sizeof(char)*(strlen(m->data.libfuncVal)+1));
        strcpy(newValue->data.libfuncVal , m->data.libfuncVal);

    }else if (type == nil_m ){


    }else if (type == undef_m){


    }else{
        assert(0);
    }
    return newValue;
}

avm_memcell * tableKey_Copy(avm_memcell *m){
    avm_memcell *newKey   = (avm_memcell*)malloc(sizeof(avm_memcell));
    avm_memcell_t type = m->type;

    newKey->type = type;

    if(type == number_m ){
        newKey->data.numVal = m->data.numVal;

    }else if(type == string_m){
        newKey->data.strVal = (char *)malloc(sizeof(char)*(strlen(m->data.strVal)+1));
        strcpy(newKey->data.strVal , m->data.strVal);

    }else if (type == bool_m){
        newKey->data.boolVal = m->data.boolVal;

    }else if (type == userfunc_m ){
        newKey->data.funcVal = m->data.funcVal;

    }else if (type == libfunc_m){
        newKey->data.libfuncVal = (char *)malloc(sizeof(char)*(strlen(m->data.libfuncVal)+1));
        strcpy(newKey->data.libfuncVal , m->data.libfuncVal);

    }else if(type == table_m){
        newKey->data.tableVal = m->data.tableVal;
        avm_tableincrefcounter(m->data.tableVal);

    }else if (type == nil_m ){
        avm_warning("Try to copy key->type == nil ",currLine, NULL ,NULL);

    }else if(type == undef_m ){
        avm_error("Try to copy key->type == undef ",currLine, NULL ,NULL);

    }else {
        printf("Not supported table key type 'tableKey_Copy' \n");
        assert(0);

    }

    return newKey;
}

avm_table_bucket * TableInsert(avm_table_bucket ** t,unsigned int pos ,avm_memcell * key, avm_memcell * value){
    avm_table_bucket * newBucket;

    avm_memcell * newValue = tableValue_Copy(value);
    avm_memcell *   newKey = tableKey_Copy(key);


    newBucket = (avm_table_bucket *)malloc(sizeof(avm_table_bucket));
    newBucket->key = newKey;
    newBucket->value = newValue;
    newBucket->next = NULL;

    newBucket->next  = t[pos];
    t[pos] = newBucket;

    return newBucket;
}




avm_memcell * avm_tablegetelem(avm_table *t , avm_memcell *key){
    assert(key);
    assert(t);

    avm_table_bucket * element;


    if( key->type == number_m){
        element  = NumLookUp(t->numIndexed , key);

    }else if (key->type == string_m){
        element =  StringLookUp(t->strIndexed , key);

    }else if (key->type == bool_m){
        element =  BoolLookUp(t->booleanIndexed , key);

    }else if (key->type == userfunc_m){
        element = UserFuncLookUp(t->userfuncIndexed , key);

    }else if (key->type == libfunc_m){
        element = LibLookUp(t->libraryfuncIndexed , key);

    }else if(key->type == table_m){
           element = TableLookUp(t->tableIndex , key);

    }else if (key->type == nil_m ){
        avm_error("Try to get key->type == nil ",currLine, NULL ,NULL);

    }else if(key->type == undef_m ){
        avm_error("Try to get key->type == undef ",currLine, NULL ,NULL);

    }else {
        printf("Not supported table key type 'avm_tablegetelem' \n");
        assert(0);

    }

    if(element != NULL){
        return element->value;

    }
    return NULL;
}



void avm_tablesetelem(avm_table *t , avm_memcell *key, avm_memcell *value){
    avm_table_bucket * tableItem = NULL;

    assert(t);
    assert(key);

    if( key->type == number_m){

        tableItem = NumLookUp(t->numIndexed , key);
        if(tableItem == NULL){

            TableInsert(t->numIndexed , numHashFunction(key->data.numVal),key, value);

            t->total++;
        }else{
            tableItem->value = tableValue_Copy(value);
        }

    }else if (key->type == string_m){
        tableItem = StringLookUp(t->strIndexed , key);
        if(tableItem == NULL){
            TableInsert(t->strIndexed , StringHashFunction(key->data.strVal) , key, value);

            t->total++;
        }else{
            tableItem->value = tableValue_Copy(value);
        }

    }else if (key->type == bool_m){
        tableItem = BoolLookUp(t->booleanIndexed , key);
        if(tableItem == NULL){
            TableInsert(t->booleanIndexed , BoolHashFunction(key->data.boolVal) ,key, value);
            t->total++;
        }else{
            tableItem->value = tableValue_Copy(value);
        }

    }else if (key->type == userfunc_m){
        tableItem = UserFuncLookUp(t->userfuncIndexed , key);
        if(tableItem == NULL){
            TableInsert(t->userfuncIndexed , UserFuncHashFunction(key->data.funcVal) ,key, value);
            t->total++;
        }else{
            tableItem->value = tableValue_Copy(value);
        }


    }else if (key->type == libfunc_m){
        tableItem = LibLookUp(t->libraryfuncIndexed , key);
        if(tableItem == NULL){
            TableInsert(t->libraryfuncIndexed , LibHashFunction(key->data.libfuncVal) , key, value);
            t->total++;
        }else{
            tableItem->value = tableValue_Copy(value);
        }

    }else if(key->type == table_m){
        tableItem = TableLookUp(t->tableIndex ,key);
        if(tableItem == NULL){
            TableInsert(t->tableIndex , numHashFunction(key->data.tableVal->tableIDnumber) , key, value);
            t->total++;
        }else{
            tableItem->value = tableValue_Copy(value);
        }

    }else if (key->type == nil_m ){
        avm_warning("Try to set key->type == nil ",currLine, NULL ,NULL);

    }else if(key->type == undef_m ){
        avm_error("Try to set key->type == undef ",currLine, NULL ,NULL);

    }else {
        printf("Not supported table key type 'avm_tablesetelem' \n");
        assert(0);

    }



    return;
}

void avm_tableincrefcounter(avm_table *t){
    ++t->refCounter;
    return;
}


void avm_tabledecrefcounter(avm_table *t){
    //assert(t->refCounter>0);
    printf("de refcounter 1 %d\n",t->refCounter);
    if(!--t->refCounter){
        avm_tabledestroy(t);
    }else

    return;
}

void avm_tablebucketsinit(avm_table_bucket **p , int size){
    int i;

    for(i=0; i<size; i++){
        p[i] = (avm_table_bucket *)0 ;
    }

    return;
}

avm_table * avm_tablenew(void){
    avm_table *t = (avm_table *)malloc(sizeof(avm_table));
    AVM_WIPEOUT(*t);

    t->tableIDnumber = tableID;
    tableID++;
    t->refCounter = 0;
    t->total = 0;

    avm_tablebucketsinit(t->numIndexed ,AVM_TABLE_HASHSIZE);
    avm_tablebucketsinit(t->strIndexed ,AVM_TABLE_HASHSIZE);
    avm_tablebucketsinit(t->libraryfuncIndexed ,AVM_TABLE_HASHSIZE);
    avm_tablebucketsinit(t->userfuncIndexed ,AVM_TABLE_HASHSIZE);
    avm_tablebucketsinit(t->booleanIndexed ,AVM_TABLE_BOOLEAN_HASHSIZE);
    avm_tablebucketsinit(t->tableIndex ,AVM_TABLE_HASHSIZE);

    return t;
}



void avm_tablebucketsdestroy(avm_table_bucket **p  ,int size){
    int i ;
    avm_table_bucket* del;
    avm_table_bucket *b;

    for(i=0; i<size; ++i , ++p){
        for(b=*p; b; ){

            del=b;
            b = b->next;
            avm_memcellclear(del->key);
            avm_memcellclear(del->value);
            free(del);

        }
        p[i]=NULL;
    }

    return;
}


void avm_tabledestroy(avm_table *t){

    avm_tablebucketsdestroy(t->strIndexed ,AVM_TABLE_HASHSIZE);
    avm_tablebucketsdestroy(t->numIndexed ,AVM_TABLE_HASHSIZE);
    avm_tablebucketsdestroy(t->userfuncIndexed ,AVM_TABLE_HASHSIZE);
    avm_tablebucketsdestroy(t->libraryfuncIndexed ,AVM_TABLE_HASHSIZE);
    avm_tablebucketsdestroy(t->booleanIndexed ,AVM_TABLE_BOOLEAN_HASHSIZE);
    avm_tablebucketsdestroy(t->tableIndex ,AVM_TABLE_HASHSIZE);

    free(t);

    return;
}

memclear_func_t memclearFuncs[] = {
    0,                  /*NUMBER*/
    memclear_string,
    0,                  /*BOOL*/
    memclear_table,
    0,                  /*USERFUNCTION*/
    0,                  /*LIBRFUNCTION*/
    0,                  /*NIL*/
    0                   /*UNDEF*/
};


void avm_memcellclear(avm_memcell *m){

    if(m->type!=undef_m){
        memclear_func_t f = memclearFuncs[m->type];

        if(f){
            (*f)(m);
        }

        m->type=undef_m;
    }

    return;
}



void memclear_string(avm_memcell *m){
    assert(m->data.strVal);
    free(m->data.strVal);

    return;
}

void memclear_table(avm_memcell *m){
    assert(m->data.tableVal);
    avm_tabledecrefcounter(m->data.tableVal);

    return;
}
