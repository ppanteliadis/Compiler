 
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


#define YY_DELC int yylex (void* yylval)

%}

%option noyywrap
%option yylineno


id              [a-zA-Z][a-zA-Z_0-9]*

integer         (0[xX][0-9A-Fa-f]+)|([0-9]+)

real            [0-9]*\.[0-9](e|E)?[0-9]*

space           [\r\n\t\v]




%x string
%%

"if"        	{insert(yylineno, KEYWORD, IF, NO_OPERATOR, NO_DELIMETER, "if");}
"else"      	{insert(yylineno, KEYWORD, ELSE, NO_OPERATOR, NO_DELIMETER, "else");}
"while"     	{insert(yylineno, KEYWORD, WHILE, NO_OPERATOR, NO_DELIMETER,"while");}
"for"	    	{insert(yylineno, KEYWORD, FOR, NO_OPERATOR, NO_DELIMETER,"for");}
"function"      {insert(yylineno, KEYWORD, FUNCTION, NO_OPERATOR, NO_DELIMETER, "function");}
"return"        {insert(yylineno, KEYWORD, RETURN, NO_OPERATOR, NO_DELIMETER, "return");}
"break"	    	{insert(yylineno, KEYWORD, BREAK, NO_OPERATOR, NO_DELIMETER, "break");}
"continue"      {insert(yylineno, KEYWORD, CONTINUE, NO_OPERATOR, NO_DELIMETER, "continue");}
"and"           {insert(yylineno, KEYWORD, AND, NO_OPERATOR, NO_DELIMETER, "and");}
"not"           {insert(yylineno, KEYWORD, NOT, NO_OPERATOR, NO_DELIMETER, "not");}
"or"            {insert(yylineno, KEYWORD, OR, NO_OPERATOR, NO_DELIMETER, "or");}
"local"	    	{insert(yylineno, KEYWORD, LOCAL, NO_OPERATOR, NO_DELIMETER,"local");}
"true"	    	{insert(yylineno, KEYWORD, TRUE, NO_OPERATOR, NO_DELIMETER,"true");}
"false"	    	{insert(yylineno, KEYWORD, FALSE, NO_OPERATOR, NO_DELIMETER,"false");}
"nil"       	{insert(yylineno, KEYWORD, NIL, NO_OPERATOR, NO_DELIMETER,"nil");}

"=" 	    	{insert(yylineno, OPERATOR, NO_KEYWORD, ASSIGNMENT, NO_DELIMETER, "=");}
"+" 	    	{insert(yylineno, OPERATOR, NO_KEYWORD, PLUS, NO_DELIMETER, "+");}
"-" 	    	{insert(yylineno, OPERATOR, NO_KEYWORD, MINUS, NO_DELIMETER, "-");}
"*" 	    	{insert(yylineno, OPERATOR, NO_KEYWORD, MUL, NO_DELIMETER, "*");}
"/" 	    	{insert(yylineno, OPERATOR, NO_KEYWORD, DIV, NO_DELIMETER, "/");}
"%" 	    	{insert(yylineno, OPERATOR, NO_KEYWORD, MOD, NO_DELIMETER, "%");}
"==" 	    	{insert(yylineno, OPERATOR, NO_KEYWORD, EQUAL, NO_DELIMETER, "==");}
"!=" 	    	{insert(yylineno, OPERATOR, NO_KEYWORD, NOT_EQUAL, NO_DELIMETER, "!=");}
"++" 	    	{insert(yylineno, OPERATOR, NO_KEYWORD, PLUS_PLUS, NO_DELIMETER, "++");}
"--" 	    	{insert(yylineno, OPERATOR, NO_KEYWORD, MINUS_MINUS, NO_DELIMETER, "--");}
">" 	    	{insert(yylineno, OPERATOR, NO_KEYWORD, GREATER, NO_DELIMETER, ">");}
"<"  	    	{insert(yylineno, OPERATOR, NO_KEYWORD, LESS, NO_DELIMETER, "<");}
">=" 	    	{insert(yylineno, OPERATOR, NO_KEYWORD, GREATER_EQUAL, NO_DELIMETER, ">=");}
"<=" 	    	{insert(yylineno, OPERATOR, NO_KEYWORD, LESS_EQUAL, NO_DELIMETER, "<=");}

"{" 	    	{insert(yylineno, DELIMETER, NO_KEYWORD, NO_OPERATOR, LEFT_BRACE,"{");}
"}" 	    	{insert(yylineno, DELIMETER, NO_KEYWORD, NO_OPERATOR, RIGHT_BRACE,"}");}
"[" 	    	{insert(yylineno, DELIMETER, NO_KEYWORD, NO_OPERATOR, LEFT_BRACKET,"[");}
"]" 	    	{insert(yylineno, DELIMETER, NO_KEYWORD, NO_OPERATOR, RIGHT_BRACKET,"]");}
"(" 	    	{insert(yylineno, DELIMETER, NO_KEYWORD, NO_OPERATOR, LEFT_PARENTHESIS,"(");}
")" 	    	{insert(yylineno, DELIMETER, NO_KEYWORD, NO_OPERATOR, RIGHT_PARENTHESIS,")");}
";" 	    	{insert(yylineno, DELIMETER, NO_KEYWORD, NO_OPERATOR, SEMICOLON,";");}
"," 	    	{insert(yylineno, DELIMETER, NO_KEYWORD, NO_OPERATOR, COMMA,",");}
":" 	    	{insert(yylineno, DELIMETER, NO_KEYWORD, NO_OPERATOR, COLON,":");}
"::" 	    	{insert(yylineno, DELIMETER, NO_KEYWORD, NO_OPERATOR, DOUBLE_COLON,"::");}
"." 	    	{insert(yylineno, DELIMETER, NO_KEYWORD, NO_OPERATOR, DOT,".");}
".." 	    	{insert(yylineno, DELIMETER, NO_KEYWORD, NO_OPERATOR, DOUBLE_DOT,".");}

{id}        	{insert(yylineno, IDENTIFIER, NO_KEYWORD, NO_OPERATOR, NO_DELIMETER, yytext);}
{integer}   	{insert(yylineno, INTEGER, NO_KEYWORD, NO_OPERATOR, NO_DELIMETER, yytext);}
{real}      	{insert(yylineno, REAL, NO_KEYWORD, NO_OPERATOR, NO_DELIMETER, yytext);}
{space}     	{insert(yylineno, SPACE, NO_KEYWORD, NO_OPERATOR, NO_DELIMETER, " ");}

	 	 



 
\"	{ 
      
	int c;
	int flag = 0;
	int flag1 = 0;
	string_buffer_ptr = string_buffer;
	
	while((c = input()) != EOF)
	{
	    if( (c == '\"') ){
		  flag1 = 0;
		  *string_buffer_ptr++ = '\0' ;
		   break;
	    }
	    else
	    {
		  if( c != '\\')
		  {
			*string_buffer_ptr++ = c;
			flag=0;
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
                insert(yylineno, UNDEFINED, NO_KEYWORD, NO_OPERATOR, NO_DELIMETER, "BAD STRING");
                printf("\\%c is invalid", c);
                flag = 0;
                continue;
			}
		 }
	
	    }
	}

	if (flag1 == 1) {
	      *string_buffer_ptr++ = '\0';
	      insert(yylineno, UNDEFINED, NO_KEYWORD, NO_OPERATOR, NO_DELIMETER, "BAD STRING");
	}
	else if (c == EOF) {
	      *string_buffer_ptr++ = '\0';
	      insert(yylineno, UNDEFINED, NO_KEYWORD, NO_OPERATOR, NO_DELIMETER, "BAD STRING");
	}
	else
	      insert(yylineno, STRING, NO_KEYWORD, NO_OPERATOR, NO_DELIMETER, string_buffer);
	int i;
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
            printf("Comment opened in line %d\n",Last());
            int c;
            while((shead != NULL) && ((c = input()) != EOF)) {
                if (c=='/' && (flag != -1)) {
                    flag = 1;
                    continue;
                }
                if ((c == '*') && (flag == 1)) {
                    insert(yylineno, STRING, NO_KEYWORD, NO_OPERATOR, NO_DELIMETER, string_buffer);
                    push('/');
                    flag = 0;
                    printf("Comment opened in line %d\n",Last());
                    continue;
                }
                else if (c == '*') {
                    flag = -1;
                    continue;
                }
                if ((c=='/') && (flag == -1)) {
                    printf("Comment closed in line %d\n",Last());
                    pop();
                    if(shead == NULL){
                        printf("All comments closed succesfully\n");
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
                insert(Last(),UNDEFINED, NO_KEYWORD, NO_OPERATOR, NO_DELIMETER, "bad comment");
            }
        }

%%



void insert(int line_number, Category category, Keyword keyword,Operator operator_,Delimeter delimeter, char * value ) {
    alpha_token_t * new_node = malloc(sizeof (alpha_token_t));
    new_node->lineNum = line_number;
    new_node->category = category;
    new_node->keyword = keyword;
    new_node->operator_ = operator_;
    new_node->delimeter = delimeter;
    new_node->value = strdup(value);
    new_node->next = NULL;

    if(head == NULL) {
        head = new_node;
        return;
    }
    else {
        alpha_token_t * current = head;
        while(current->next != NULL) {
            current = current->next;
        }
        current->next = new_node;
    }
    return;;
}



void print(void) { //to provlima einai oti epeidi epistrefei se char* o integere mpainei k san char * ?
    alpha_token_t* current = head;
    int token_count = 0;
    while(current != NULL) {
        switch (current->category) {
            case KEYWORD:
                fprintf(yyout, "%d: [%d] \"%s\" \"%s\", %s\n", current->lineNum, ++token_count, current->value, category_table[current->category], keyword_table[current->keyword]);
                break;
            case OPERATOR:
                fprintf(yyout, "%d: [%d] \"%s\" \"%s\", %s\n", current->lineNum, ++token_count, current->value, category_table[current->category], operator_table[current->operator_]);
                break;
            case INTEGER:
                fprintf(yyout, "%d: [%d] \"%s\" \"%s\", %s\n", current->lineNum, ++token_count, current->value, category_table[current->category], current->value);
                break;
            case REAL:
                fprintf(yyout, "%d: [%d] \"%s\" \"%s\", %s\n", current->lineNum, ++token_count, current->value, category_table[current->category], current->value);
                break;
            case STRING:
                fprintf(yyout, "%d: [%d] \"%s\" \"%s\" \n", current->lineNum, ++token_count, current->value, category_table[current->category]);
                break;
            case DELIMETER:
                fprintf(yyout, "%d: [%d] \"%s\" \"%s\", %s\n", current->lineNum, ++token_count, current->value, category_table[current->category], delimeter_table[current->delimeter]);
                break;
            case IDENTIFIER:
                fprintf(yyout, "%d: [%d] \"%s\" \"%s\", %s\n", current->lineNum, ++token_count, current->value, category_table[current->category]);
                break;
            case UNDEFINED:
                fprintf(yyout, "Error in line %d. Reason: %s.\n", (current->lineNum)-1, current->value);
                break;
        }
        current = current->next;
    }
}

int alpha_yylex (void* yylval) {
    yylex();
    return 0;
}

void pop() {
    symbol_stack_t *tmp = shead;
    shead = shead->next;
    free(tmp);
}

/*by Pavlos....*/
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

int main(int argc , char ** argv) {
    if (argc > 1) {
        if (!(yyin = fopen(argv[1],"r"))) {
            fprintf(stderr,"Cannot read file: %s\n",argv[1]);
            return 1;
        }
    }
    else {
        yyin = stdin;
        yyout = stdout;
    }
    alpha_yylex(&head);
    print();
    return 0;
}
