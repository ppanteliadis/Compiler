%{

#include <math.h>
#include <stdio.h>
#include <string.h>
#include "parser.h"
typedef struct possition_comment{
    int possition;
    struct possition_comment * next;
}possition_comment ;


unsigned int number_of_readed_tokens = 0 ;


char *str;
%}

%option yylineno
%option noyywrap
%option outfile = "scanner.c"


INTCONST [0-9]+
REALCONST [0-9]+\.[0-9]+
ALPHA_CHAR  	[A-Za-z]
IDENTIFIER {ALPHA_CHAR}+({ALPHA_CHAR}|{INTCONST}|"_")*
KEYWORD 	"and"|"not"|"or"|"local"|"true"|"false"|"nil"|"if"|"else"|"while"|"for"|"function"|"return"|"break"|"continue"
OPERATOR ">"|"<"|">="|"<="|"="|"+"|"-"|"*"|"/"|"%"|"=="|"!="|"++"|"--"|"&&"|"||"
PUNCTUATION "\{"|"\}"|"\["|"\]"|"\("|"\)"|"\;"|"\,"|"\:"|"\::"|"\."|"\.."
WHITESPACE  [ \t\n]+


%%


\n {;}

{INTCONST}  {
    yylval.intVal = atoi(yytext);
		return (INTEGER);
	}

{REALCONST}  {
        yylval.floatVal = atof(yytext);
			return(REAL);
                     }
"break" {
			return (BREAK);
                     }

"continue" {
			return (CONTINUE);
                     }

"+" {
			return (PLUS);
                     }

"-" {
			return (MINUS);
                     }

"*" {
			return (MUL);
                     }

 "/" {
			return (DIV);
                     }
"%" {
			return (MOD);
                     }

">" {
			return (B_T);
                     }

">=" {
			return (BE_T);
                     }

"<" {
			return (L_T);
                     }
"<=" {
			return (LE_T);
                     }

 "==" {
			return (EQ);
                     }

 "!=" {
			return (N_EQ);
                     }

"and" {
			return (AND);
                     }

 "or" {
			return (OR);
                     }
"(" {
			return (L_P);
                     }

")" {
			return (R_P);
                     }

"&&" {
			return (A_N_D);
			}


"||" {
			return (O_R);
			}

"not" {
			return (NOT);
                     }

"++" {
			return (PP);
                     }

"--" {
			return (MM);
                     }


"=" {
			return (ASSIGN);
                     }

"local" {
			return (LOCAL);
                     }

"::" {
			return (CATHOL);
                     }

"[" {
			return (L_B);
                     }

"]" {
			return (R_B);
                     }

".." {
			return (FF);
                     }
"." {
			return (F);
                     }

 "," {
			return (COMMA);
                     }

 ":" {
			return (UP_DOWN_DOT);
                     }

 "{" {
			return (BLOCK);
                     }

 "}" {
			return (ENDBLOCK);
                     }

"function" {
			return (FUNCTION);
                     }

"false" {
			return (FALSE);
                     }
"true" {
			return (TRUE);
                     }

"nil" {
			return (NIL);
                     }

"if" {
			return (IF);
                     }
"else" {
			return (ELSE);
                     }
"while" {
			return (WHILE);
                     }

"for" {
			return (FOR);
                     }
"return" {
			return (RETURN);
                     }
";" {
         return(SEMICOLON);
         }


{IDENTIFIER} {
    yylval.strVal = malloc(strlen(yytext));
    strncpy(yylval.strVal, yytext, strlen(yytext));

			return (IDENTIFIER);
                     }

\" {
    char c;
    int j=0;
    int line=yylineno;
    str=malloc(2);
    while((c=input())!=EOF){
       if( c == '\\'){
				c =input();
				if(c==EOF) break;
				if( c == 'n' ){
					str[j]='\n' ;
					j++;
                str = realloc(str, j+2);
				}else if( c == 't'){
					str[j]='\t' ;
					j++;
                str = realloc( str, j+2);
				}else if( c == '\\'){

					str[j]='\\' ;
					j++;
                str = realloc(str, j+2);
				}
				else if( c == '\"'){

					str[j]='\"' ;
					   j++;
                    str = realloc(str,j+2);
				}else{
					str[j]='\\' ;
					j++;
                    str =  realloc(str, j+2);
					str[j]=c;
					j++;
				    str =  realloc( str,j+2);
				}

    }
    else if(c=='\"'){break;}
    else {
        str[j]=c;
        j++;
       str = realloc(str, j+2);

    }
    }
    if(c==EOF) printf("\t Unterminated String  in line %d\n",line);
    else{
        str[j]='\0';
        yylval.strVal = malloc(strlen(str)+1);
	strcpy( yylval.strVal , str);
        return (STRING);

    }
}


"//"   {                          /*eat up // commnets*/
      char c ;
      while((c=input())!='\n'){;}
      unput(c);
      number_of_readed_tokens++ ;
}



"/*"  {
      char c ;
      int number_of_open_comments=1;
      possition_comment * list , * tmp_node;

      number_of_readed_tokens++;

      list = (possition_comment*)malloc(sizeof(possition_comment));
      list->possition = yylineno ;
      list->next = NULL ;

      while((c=input())!=EOF){
             if(c=='*'){
                 if((c=input())=='/'){
                     number_of_open_comments--;
                     tmp_node = list ;
                     list = list->next ;

                     if(number_of_open_comments == 0){
                        printf( "\t%d: \t#%d \t\t\t\t\t COMMENT TYPE /* */ \n",tmp_node->possition ,number_of_readed_tokens);
                        free(tmp_node);
                        break;
                    }else{
                        printf( "\t%d: \t#%d \t\t\t\t\t COMMENT TYPE NESTETED /* */ \n",tmp_node->possition ,number_of_readed_tokens);
                        number_of_readed_tokens++;
                        free(tmp_node);
                    }

                 }
                 else  unput(c);
             }
             else if(c=='/'){
                 if((c=input())=='*'){
                    tmp_node = (possition_comment*)malloc(sizeof(possition_comment));
                    tmp_node -> possition = yylineno ;
                    tmp_node -> next = list ;
                    list = tmp_node ;

                 	number_of_open_comments++;
		          }
                else  unput(c);
             }
      }
}


{WHITESPACE} {}


.     {printf( "\t%d: \t Unrecognized characters: ++#%s#++\n", yylineno ,yytext );
        exit(0);
}
