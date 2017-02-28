#include "header.h"

char  **t_string =NULL;
unsigned int currSTRING = 0;
unsigned tstring_total = 0;

double *t_num =NULL;
unsigned int currNUM = 0;
unsigned tnum_total = 0;

char  **t_lib =NULL;
unsigned int currLIB = 0;
unsigned tlib_total = 0;

userfunc  *t_userfunc =NULL;
unsigned int currUSERFUNC = 0;
unsigned t_userfunc_total = 0;


void TSTRING_expand(void){
    assert(tstring_total==currSTRING);
    char** s = (char **)malloc(TSTRING_NEW_SIZE);

    if (t_string){
        memcpy(s, t_string, TSTRING_CURR_SIZE);
        free(t_string);
    }

    t_string = s;
    tstring_total += TSTRING_EXPAND_SIZE;

    return;
}

unsigned consts_newstring(char *s){
    int i;
    for(i=0; i<currSTRING; i++){
        if( strcmp(t_string[i],s) == 0){
            return i;
        }
    }


    if(currSTRING == tstring_total){
        TSTRING_expand();
    }


    char **str = t_string ;

    str[currSTRING] = (char *)malloc(sizeof(char)*(strlen(s)+1));
    str[currSTRING][0]='\0';

    str[currSTRING] = strcpy(str[currSTRING],s);

    currSTRING++;

    free(s);
    return (currSTRING-1);
}




void TNUM_expand(void){
    assert(tnum_total==currNUM);
    double *d= (double *)malloc(TNUM_NEW_SIZE);

    if (t_num){
        memcpy(d, t_num, TNUM_CURR_SIZE);
        free(t_num);
    }

    t_num = d;
    tnum_total += TNUM_EXPAND_SIZE;
}

unsigned consts_newnumber(double n){
    int i;
    for(i=0; i<currNUM; i++){
        if( t_num[i] == n){
            return i;
        }
    }
    if(currNUM == tnum_total){
        TNUM_expand();
    }

    double *d = t_num + currNUM++;
    *d=n;
    return (currNUM-1);
}


void TLIB_expand(void){
    assert(tlib_total==currLIB);
    char** s = (char **)malloc(TLIB_NEW_SIZE);

    if (t_lib){
        memcpy(s, t_lib, TLIB_CURR_SIZE);
        free(t_lib);
    }

    t_lib = s;
    tlib_total += TLIB_EXPAND_SIZE;
}



unsigned libfuncs_newused(const char *s){

    int i;
    for(i=0; i<currLIB; i++){
        if( strcmp(t_lib[i],s) == 0){
            return i;
        }
    }

    if(currLIB == tlib_total){
        TLIB_expand();
    }

    char **str = t_lib;

    str[currLIB] = (char *)malloc(sizeof(char)*(strlen(s)+1));
    str[currLIB][0]='\0';

    str[currLIB] = strcpy(str[currLIB],s);

    currLIB++;

    return (currLIB-1);
}



void T_USERFUNC_expand(void){
    assert(t_userfunc_total==currUSERFUNC);
    userfunc * s = (userfunc *)malloc(TUSERFUNC_NEW_SIZE);

    if (t_userfunc){
        memcpy(s, t_userfunc, TUSERFUNC_CURR_SIZE);
        free(t_userfunc);
    }

    t_userfunc = s;
    t_userfunc_total += TUSERFUNC_EXPAND_SIZE;

    return;
}


unsigned userfuncs_newfunc(char * name ,   unsigned taddress , unsigned totallocals){
    int i;
    for(i=0; i<currUSERFUNC; i++){
        if(( strcmp(t_userfunc[i].name,name) == 0)&&(t_userfunc[i].taddress == taddress )){
            return i;
        }
    }
    if(currUSERFUNC == t_userfunc_total){
        T_USERFUNC_expand();
    }

    userfunc *function = t_userfunc + currUSERFUNC++;

    function->name = (char * )malloc(sizeof(char)*(strlen(name)+1));
    function->name[0]='\0';
    strcpy(function->name , name);
    function->name[strlen(name)]='\0';

    function->taddress = taddress;
    function->totallocals = totallocals;

    free(name);
    return (currUSERFUNC-1);
}
