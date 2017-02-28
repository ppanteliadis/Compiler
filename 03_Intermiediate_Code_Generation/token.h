

#ifndef token_h
#define token_h

#include <stdlib.h>

#define MAX_STR_SIZE 10000

char* category_table[] = {"KEYWORD","OPERATOR","INTEGER","REAL","STRING","DELIMETERS","IDENTIFIER","COMMENT","UNDEFINED","EOT", "SPACE","IF","ELSE","WHILE","FOR","FUNCTION","RETURN","BREAK","CONTINUE","AND","NOT","OR","LOCAL","TRUE","FALSE","NIL","ASSIGNMENT","PLUS","MINUS","MUL","DIV","MOD","EQUAL","NOT_EQUAL","PLUS_PLUS","MINUS_MINUS","GREATER","LESS","GREATER_EQUAL","LESS_EQUAL", "LEFT_BRACE","RIGHT_BRACE", "LEFT_BRACKET", "RIGHT_BRACKET", "LEFT_PARENTHESIS", "RIGHT_PARENTHESIS", "SEMICOLON", "COMMA", "COLON", "DOUBLE_COLON", "DOT", "DOUBLE_DOT"};

typedef struct alpha {
    int lineNum;
    char* value;
    struct alpha *next;
}alpha_token_t;


alpha_token_t *head = NULL;




char *string_buf_ptr;
char string_buf[MAX_STR_SIZE];



//void insert(int line_number, Category category, Keyword keyword,Operator operator_,Delimeter delimeter, char* value);
void error_insert(int line_number, char* reason);

int alpha_yylex (void* ylval);

#endif
