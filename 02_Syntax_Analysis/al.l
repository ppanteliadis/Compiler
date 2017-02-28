%{
#if defined(WIN32)
#define YY_NO_UNISTD_H
static isatty(int i){ return 0; }
#elif defined(_WIN32_WCE)
#define YY_NO_UNISTD_H
static int isatty(void * i){ return 0; }
#endif

#include "stack.h"
#include <stdlib.h>
#include "token.h"
#include "symtable.h"

#include "parser.h"



#define YY_DELC int alpha_yylex (void* yylval)

extern YYSTYPE yylval;

%}

%option case-insensitive
%option noyywrap
%option yylineno


id              [a-zA-Z][a-zA-Z_0-9]*

integer         (0[xX][0-9A-Fa-f]+)|([0-9]+)

real            [0-9]*\.[0-9](e|E)?[0-9]*

undefined 		[^"\r\n \t]

%x string
%%

"if"			{return IF;}
"else"			{return ELSE;}
"while"			{return WHILE;}
"for"			{return FOR;}
"function"		{return FUNCTION;}
"return"		{return RETURN;}
"break"			{return BREAK;}
"continue"		{return CONTINUE;}
"and"			{return AND;}
"not"			{return NOT;}
"or"			{return OR;}
"local"			{return LOCAL;}
"true"			{return TRUE;}
"false"			{return FALSE;}
"nil"			{return NIL;}

"=" 			{return ASSIGNMENT;}
"+" 			{return PLUS;}
"-" 			{return MINUS;}
"*" 			{return MUL;}
"/" 			{return DIV;}
"%" 			{return MOD;}
"==" 			{return EQUAL;}
"!=" 			{return NOT_EQUAL;}
"++" 			{return PLUS_PLUS;}
"--" 			{return MINUS_MINUS;}
">" 			{return GREATER;}
"<"  			{return LESS;}
">=" 			{return GREATER_EQUAL;}
"<=" 			{return LESS_EQUAL;}

"{" 			{return LEFT_BRACE;}
"}" 			{return RIGHT_BRACE;}
"[" 			{return LEFT_BRACKET;}
"]" 			{return RIGHT_BRACKET;}
"(" 			{return LEFT_PARENTHESIS;}
")" 			{return RIGHT_PARENTHESIS;}
";" 			{return SEMICOLON;}
"," 			{return COMMA;}
":" 			{return COLON;}
"::" 			{return DOUBLE_COLON;}
"." 			{return DOT;}
".." 			{return DOUBLE_DOT;}
"\n"
"\t"
"\r"
"\v"

{id}			{yylval.strVal = strdup(yytext);return IDENTIFIER;}
{integer}		{yylval.intVal = atoi(yytext);return INTEGER;}
{real}			{yylval.realVal = atof(yytext);return REAL;}
{undefined}		{yylval.realVal = atof(yytext);return UNDEFINED;}






\"	{ 
	
	int c;
	int flag = 0;
	int flag1 = 0;
	string_buffer_ptr = string_buffer;
	
	while((c = input()) != EOF)
	{
	    if (c == '\"') {
		  flag1 = 0;
		  *string_buffer_ptr++ = '\0' ;
		   break;
	    }
	    else
	    {
		  if( c != '\\')
		  {
			*string_buffer_ptr++ = c;
			flag = 0;
			continue;
			
		  }  
		  else if(c == '\\')
		  {
			flag = 1;
			c  = input();
			if (c == '\'')
			{
			    *string_buffer_ptr++ = c;
			    flag = 0;
			    continue;
			}
			else if( c == '\"')
			{
			    *string_buffer_ptr++ = c;
			    flag1 = 1;
			    flag = 0;
			    continue;
			}
			else if( c == 'n')
			{
			    *string_buffer_ptr++ = '\n';
			    flag = 0;
			    continue;
	    
			}
			else if( c == 't')
			{
			    *string_buffer_ptr++ = '\t';
			    flag = 0;
			    continue;
	    
			}
			else if( c == '\\')
			{
			    *string_buffer_ptr++ = '\\';
			    flag = 0;
			    continue;
			}
			else {
                //	*string_buffer_ptr++ = '\\'; //debug only
                // *string_buffer_ptr++ = c;
                //insert(yylineno, UNDEFINED, NO_KEYWORD, NO_OPERATOR, NO_DELIMETER, "BAD STRING");
                printf("\\%c is invalid", c);
                flag = 0;
                continue;
			}
		 }
	
	    }
	}

	if (flag1 == 1) {
	      *string_buffer_ptr++ = '\0';
	      //insert(yylineno, UNDEFINED, NO_KEYWORD, NO_OPERATOR, NO_DELIMETER, "BAD STRING");
	}
	else if (c == EOF) {
	      *string_buffer_ptr++ = '\0';
	      //insert(yylineno, UNDEFINED, NO_KEYWORD, NO_OPERATOR, NO_DELIMETER, "BAD STRING");
	}
			yylval.strVal = strdup(yytext);return STRING;
	      //insert(yylineno, STRING, NO_KEYWORD, NO_OPERATOR, NO_DELIMETER, string_buffer);
	int i = 0;
	for (i=0;i<10000;i++)
	      string_buffer[i]='\0';

}



"//"    {
            int c;
            while((c = input())!= '\n'){
                ;
            }
        }


"/*"    {
    int flag = 0;
            push('/');
            /*printf("Comment opened in line %d\n",Last());*/
            int c;
            while((shead != NULL) && ((c = input()) != EOF)) {
                if (c=='/' && (flag != -1)) {
                    flag = 1;
                    continue;
                }
                if ((c == '*') && (flag == 1)) {
                    /*insert(yylineno, STRING, NO_KEYWORD, NO_OPERATOR, NO_DELIMETER, string_buffer);*/
                    push('/');
                    flag = 0;
                    //printf("Comment opened in line %d\n",Last());
                    continue;
                }
                else if (c == '*') {
                    flag = -1;
                    continue;
                }
                if ((c=='/') && (flag == -1)) {
                    //printf("Comment closed in line %d\n",Last());
                    pop();
                    if(shead == NULL){
                        //printf("All comments closed succesfully\n");
                        break;
                    }
                    flag = 0;
                    continue;
                }
                else {
                    flag = 0;
                    continue;
                }
            }
            if (shead != NULL) {
                //insert(Last(),UNDEFINED, NO_KEYWORD, NO_OPERATOR, NO_DELIMETER, "bad comment");
            }
        }

%%


int alpha_yylex (void* yylval) {
    yylex();
    return 0;
}

void pop() {
    symbol_stack_t *tmp = shead;
    shead = shead->next;
    free(tmp);
}

symbol_stack_t *top() {
    symbol_stack_t *current = shead;
    while (current->next != NULL) {
        current = current->next;
    }
    return current;
}

void push(char newElement) {
    symbol_stack_t *tmp = (symbol_stack_t*)malloc(sizeof(symbol_stack_t));
    if (tmp == NULL) {
        exit(0);
    }
    tmp->symbol = newElement;
    tmp->lineNo = yylineno;
    tmp->next = shead;
    shead = tmp;
}

int Last() {
    symbol_stack_t *current =shead;
    while(current->next != NULL)
        current=current->next;
    return current->lineNo;
}