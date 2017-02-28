/* A Bison parser, made by GNU Bison 2.5.  */

/* Bison interface for Yacc-like parsers in C
   
      Copyright (C) 1984, 1989-1990, 2000-2011 Free Software Foundation, Inc.
   
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.
   
   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */


/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     ASSIGN = 258,
     OR = 259,
     AND = 260,
     EQ = 261,
     N_EQ = 262,
     B_T = 263,
     BE_T = 264,
     L_T = 265,
     LE_T = 266,
     PLUS = 267,
     MINUS = 268,
     MUL = 269,
     DIV = 270,
     MOD = 271,
     NOT = 272,
     PP = 273,
     MM = 274,
     UMINUS = 275,
     F = 276,
     FF = 277,
     L_B = 278,
     R_B = 279,
     L_P = 280,
     R_P = 281,
     BREAK = 282,
     SEMICOLON = 283,
     CONTINUE = 284,
     A_N_D = 285,
     O_R = 286,
     LOCAL = 287,
     CATHOL = 288,
     COMMA = 289,
     UP_DOWN_DOT = 290,
     ENDBLOCK = 291,
     BLOCK = 292,
     FUNCTION = 293,
     IDENTIFIER = 294,
     STRING = 295,
     NIL = 296,
     TRUE = 297,
     FALSE = 298,
     IF = 299,
     ELSE = 300,
     FOR = 301,
     WHILE = 302,
     RETURN = 303,
     INTEGER = 304,
     REAL = 305,
     THEN = 306
   };
#endif
/* Tokens.  */
#define ASSIGN 258
#define OR 259
#define AND 260
#define EQ 261
#define N_EQ 262
#define B_T 263
#define BE_T 264
#define L_T 265
#define LE_T 266
#define PLUS 267
#define MINUS 268
#define MUL 269
#define DIV 270
#define MOD 271
#define NOT 272
#define PP 273
#define MM 274
#define UMINUS 275
#define F 276
#define FF 277
#define L_B 278
#define R_B 279
#define L_P 280
#define R_P 281
#define BREAK 282
#define SEMICOLON 283
#define CONTINUE 284
#define A_N_D 285
#define O_R 286
#define LOCAL 287
#define CATHOL 288
#define COMMA 289
#define UP_DOWN_DOT 290
#define ENDBLOCK 291
#define BLOCK 292
#define FUNCTION 293
#define IDENTIFIER 294
#define STRING 295
#define NIL 296
#define TRUE 297
#define FALSE 298
#define IF 299
#define ELSE 300
#define FOR 301
#define WHILE 302
#define RETURN 303
#define INTEGER 304
#define REAL 305
#define THEN 306




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{

/* Line 2068 of yacc.c  */
#line 26 "parser.y"

int intVal;
char *strVal;
float floatVal;
unsigned unsignedVal;
struct SymbolTableEntry * symVal;
struct expr * exprVal;
struct methodcall_t * methVal;
struct listOfExpression  * elistVal;
struct forprefixNode * forprefixNodeVal;



/* Line 2068 of yacc.c  */
#line 166 "parser.h"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE yylval;


