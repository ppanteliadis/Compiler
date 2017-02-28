#include<stdio.h>
#include<stdbool.h>
#include<stdlib.h>
#include<string.h>
#include<assert.h>

#define EXPAND_SIZE 1024
#define CURR_SIZE (total*sizeof(quad))
#define NEW_SIZE (EXPAND_SIZE * sizeof(quad)+CURR_SIZE)


extern int numberOfGlobalVars;
extern int numberOfAnonymousFunctions;
extern struct function_var_list * listOfFunctionVars;
extern struct function_var_list * end_listOfFunctionVars;
extern unsigned int currQuad;
extern int lookUpAllUserFunctFound;

//---------------------------------------------------------
void PrintTables();
void PrintTargetCode();
void stackInitialize();
void generateTargetCode(void);
//---------------------------------------------------------

typedef enum scopespace_t{
    programvar,
    formalarg,
    functionlocal
}scopespace_t;


typedef enum symbol_t{
    var_s,
    programfunc_s,
    libraryfunc_s
}symbol_t;




typedef struct Variable{
    char *name;
    unsigned int scope;
    unsigned int line;
    scopespace_t scopeSpace;
    unsigned  scopeOffset;
}Variable;


typedef struct function_var_list{
    Variable *variable;
    struct function_var_list *next;
}function_var_list;


typedef struct incompleteReturn{
    unsigned possition;
    struct incompleteReturn *next;
}incompleteReturn;

typedef struct Function{
    const char *name;
    function_var_list *head;
    unsigned int scope;
    unsigned int line;

    //new fields for i code

    unsigned int iaddress;
    unsigned int totallocals;

    //new fields for t code

    unsigned taddress;
    incompleteReturn *returnList;
    unsigned incompleteJump;
}Function;

enum SymbolTableType{
    GLOBAL=0,
    LOCAL_T,
    FORMAL,
    USERFUNC,
    LIBFUNC
};



typedef struct SymbolTableEntry{
    bool isActive;

    union{
        Variable *varVal;
        Function *funcVal;
    }value;

    enum SymbolTableType type;
    symbol_t s_type;

    struct SymbolTableEntry * next_list_entry ;
    struct SymbolTableEntry * next_list_scope ;
    struct SymbolTableEntry * prev_list_scope ;
}SymbolTableEntry;

typedef struct ScopeListEntries{
    int scopeVal ;
    SymbolTableEntry *start_of_scope_i;
    SymbolTableEntry *end_of_scope_i;
    struct ScopeListEntries *next_list_node;
}ScopeListEntries;

typedef struct ScopeListNode{
    ScopeListEntries * head;
}ScopeListNode;


struct listOfExpression {
    struct expr *start ;
    struct expr * end ;
};


struct node {
    int quadPossition;
    struct node * next;

};

struct stmtNode{
    struct node * breaklist;
    struct node * continuelist;
};

struct forprefixNode {
    unsigned test;
    unsigned enter;

};

extern struct stmtNode * breakContList;

///////////////////////////////////////////////////////

typedef enum iopcode{
    add             = 0,
    sub             = 1,
    mul             = 2,
    divide          = 3,
    mod             = 4,
    uminus          = 5,
    tablecreate     = 6,
    tablegetelem    = 7,
    tablesetelem    = 8,
    assign          = 9,
    jump            =10,
    if_eq           =11,
    if_not_eq       =12,
    if_greater      =13,
    if_greatereq    =14,
    if_less         =15,
    if_lesseq       =16,
    not             =17,
    or              =18,
    and             =19,
    param           =20,
    call            =21,
    getretval       =22,
    funcstart       =23,
    ret             =24,
    funcend         =25,
}iopcode;




typedef enum expr_t{
    var_e,
    tableitem_e,

    programfunc_e,
    libraryfunc_e,

    arithexpr_e,
    boolexpr_e,
    assignexpr_e,
    newtable_e,

    constnum_e,
    constbool_e,
    conststring_e,

    nil_e
}expr_t;


typedef struct expr{
    expr_t type;
    SymbolTableEntry *sym;
    struct expr *index;
    double	numConst;
    char *strConst;
    unsigned char boolConst;
    struct expr *next;
}expr ;



typedef struct quad{
    iopcode	op;
    struct expr * result;
    struct expr * arg1;
    struct expr * arg2;
    unsigned  label;
    unsigned  line;
    unsigned  taddress;
}quad;

extern quad*	quads;

typedef struct methodcall_t{
    struct expr * elist;
    bool method;
    char * name;
}methodcall_t;


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//============================================ FUNCTION DECLERATION =================================================//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////





//================================================== SUMBOL TABLE .H =================================================//


unsigned int SymbolTableHashFunction(const char *id );

char * CheckForLibraryNameCollision(char * id);

SymbolTableEntry * SymbolTableLookUp( char * id , int scope);

SymbolTableEntry * SymbolTableLookUp_Temporary_Variables_At_This_Scope(char * id ,int scope );

///////////////////////////////////////////////////////////////////////
//PRINT APO KA8E LOOKUP 8A KANOUME TO lookUpAllUserFunctFound = 0
//GIA MEGALUTERH SIGOURIA , EPEIDH MPOREI NA EXEI PROHGOUMENOS ALLA3EI
SymbolTableEntry * SymbolTableLookUp_ALL_FROM_THIS(char * id , int scope);

SymbolTableEntry * SymbolTableLookUp_THIS(char * id ,int scope );

SymbolTableEntry *  SymbolTableInsert(char * id , int scope , int line ,enum SymbolTableType type , enum symbol_t s_type , enum scopespace_t scopeSpace ,
                                      unsigned  scopeOffset);

void SymbolTable_scopeList_Initialize();

void SymbolTablePrintAll();

void ScopePrintAll();

int SymbolTableHide(int scope);




//=================================================== I CODE . H =====================================================//


////////////////////////// Functions Declaration /////////////////////////

//////////////// Temporary Variable - Function Declaration ////////////////
char *newtempname(void);

SymbolTableEntry *newtemp(int scope ,int line ,enum scopespace_t scopeSpace ,unsigned  scopeOffset);

void resettemp(void);


/////////////////////ScopeSpace - Function Declaration ////////////////////
scopespace_t currscopespace( void );

unsigned  currscopeoffset( void );

void inccurrscopeoffset( void );

void enterscopespace( void );

void exitscopespace( void );


////////////////////Offset - Function Declaration ///////////////////////
void resetformalargoffset( void );

void resetfunctionlocaloffset( void );

void restorecurrscopeoffset( unsigned n );


///////////////////Expression - Function Declaration ///////////////////
expr *newexpr(expr_t t);

expr * newexpr_constbool(bool b);

expr * newexpr_conststring(char * s);

expr * newexpr_consnum(double num);


////////////////////////////////////////////////////////////////////////

void expand(void);

void emit(iopcode op ,expr * arg1, expr * arg2, expr * result, unsigned label, unsigned line);

expr* emit_iftableitem( expr* e, int line);

expr *lvalue_expr(SymbolTableEntry *sym);

expr *member_item(expr *lvalue,char *name,int line);

expr *make_call(expr *lvalue, expr *elist, unsigned line);

void checkuminus(expr *e, int lineno);

unsigned  nextquadlabel( void );

void patchlabel(unsigned quadNo, unsigned  label);

int currScope( void );

void PrintQuads( void );

void PrintIcode( void );






//=================================================== MANAGE GRAMMAR . H =====================================================//



SymbolTableEntry *  Manage_lvalue_LOCAL_IDENTIFIER(int scope, char *name, int line , enum scopespace_t scopeSpace ,unsigned  scopeOffset);

SymbolTableEntry *  Manage_lvalue_global_IDENTIFIER(char *name, int line);

SymbolTableEntry *  Manage_lvalue_IDENTIFIER(int scope, char *name, int line, enum scopespace_t scopeSpace ,unsigned  scopeOffset);

void Manage_stmt_returnstmt(int line , int infunction);

void Manage_stmt_BREAK_SEMICOLON(int line , int inloop);

void Manage_stmt_CONTINUE_SEMICOLON(int line , int inloop);


void Manage_term_PP_lvalue(struct SymbolTableEntry * exprNode , int line );

void Manage_term_lvalue_PP(struct SymbolTableEntry * exprNode , int line);

void Manage_term_lvalue_MM(struct SymbolTableEntry * exprNode , int line);

void Manage_term_MM_lvalue(struct SymbolTableEntry * exprNode , int line);


void Manage_assignexpr_lvalue_ASSIGN_expr(struct SymbolTableEntry * exprNode , int line);


void Manage_idlist_IDENTIFIER(char * id, int line , int scope);

SymbolTableEntry * Manage_funcargs_LP_idlist_RP(SymbolTableEntry * function);

SymbolTableEntry * Manage_funcprefix_FUNCTION_funcname(char * function_name , int function_line , int function_scope);


