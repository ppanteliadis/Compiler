 
//
//  token.h
//  project340_phase1
//
//  Created by Pavlos Panteliadis on 2/28/15.
//  Copyright (c) 2015 Computer Science Department. All rights reserved.
//

#ifndef token_h
#define token_h

#include <stdlib.h>

#define MAX_STR_SIZE 10000

char* category_table[] = {"KEYWORD","OPERATOR","INTEGER","REAL","STRING","DELIMETERS","IDENTIFIER","COMMENT","UNDEFINED","EOT"};
char* keyword_table[] = {"IF","ELSE","WHILE","FOR","FUNCTION","RETURN","BREAK","CONTINUE","AND","NOT","OR","LOCAL","TRUE","FALSE","NIL",""};
char* operator_table[] = {"ASSIGNMENT","PLUS","MINUS","MUL","DIV","MOD","EQUAL","NOT_EQUAL","PLUS_PLUS","MINUS_MINUS","GREATER","LESS","GREATER_EQUAL","LESS_EQUAL",""};
char* delimeter_table[]= { "LEFT_BRACE","RIGHT_BRACE", "LEFT_BRACKET", "RIGHT_BRACKET", "LEFT_PARENTHESIS", "RIGHT_PARENTHESIS", "SEMICOLON", "COMMA", "COLON", "DOUBLE_COLON", "DOT", "DOUBLE_DOT",""};

typedef enum
{
    KEYWORD,
    OPERATOR,
    INTEGER,
    REAL,
    STRING,
    DELIMETER,
    IDENTIFIER,
    SPACE,
    UNDEFINED,
}Category;

typedef enum
{
    IF,
    ELSE,
    WHILE,
    FOR,
    FUNCTION,
    RETURN,
    BREAK,
    CONTINUE,
    AND,
    NOT,
    OR,
    LOCAL,
    TRUE,
    FALSE,
    NIL,
    NO_KEYWORD
}Keyword;

typedef enum
{
    ASSIGNMENT,
    PLUS,
    MINUS,
    MUL,
    DIV,
    MOD,
    EQUAL,
    NOT_EQUAL,
    PLUS_PLUS,
    MINUS_MINUS,
    GREATER,
    LESS,
    GREATER_EQUAL,
    LESS_EQUAL,
    NO_OPERATOR
}Operator;

typedef enum
{
    LEFT_BRACE,
    RIGHT_BRACE,
    LEFT_BRACKET,
    RIGHT_BRACKET,
    LEFT_PARENTHESIS,
    RIGHT_PARENTHESIS,
    SEMICOLON,
    COMMA,
    COLON,
    DOUBLE_COLON,
    DOT,
    DOUBLE_DOT,
    NO_DELIMETER
}Delimeter;

typedef struct alpha
{
    int lineNum;
    Category category;
    Keyword keyword;
    Operator operator_;
    Delimeter delimeter;
    char* value;
    struct alpha *next;
    
}alpha_token_t;


alpha_token_t *head = NULL;



char *string_buffer_ptr;
char string_buffer[MAX_STR_SIZE];



void insert(int line_number, Category category, Keyword keyword,Operator operator_,Delimeter delimeter, char* value);
void error_insert(int line_number, char* reason);

int alpha_yylex (void* ylval);

#endif
