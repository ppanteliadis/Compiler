/* A Bison parser, made by GNU Bison 2.5.  */

/* Bison implementation for Yacc-like parsers in C
   
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

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "2.5"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1

/* Using locations.  */
#define YYLSP_NEEDED 0



/* Copy the first part of user declarations.  */

/* Line 268 of yacc.c  */
#line 1 "parser.y"


#include "header.h"
#include "stack.h"

int inloop=0;                                         //we are in a loop for, while
int infunction=0;                                     //we are in a function
int scope=0;                                          //global variable that show us the scope we are
void yyerror(const char *yaccProvidedMessage);
extern int  yylineno;
extern char *yytext;
extern FILE* yyin;
int yylex(void);
struct function_var_list * listOfFunctionVars=NULL, * end_listOfFunctionVars = NULL;

stack_t * scopeoffsetstack;
stack_t * breakContstack;
stack_t * inloopStack ;

struct SymbolTableEntry * LastFunctionInsertedToSymTable = NULL;

struct stmtNode * breakContList=NULL;


/* Line 268 of yacc.c  */
#line 96 "parser.c"

/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 1
#endif

/* Enabling the token table.  */
#ifndef YYTOKEN_TABLE
# define YYTOKEN_TABLE 0
#endif


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

/* Line 293 of yacc.c  */
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



/* Line 293 of yacc.c  */
#line 248 "parser.c"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif


/* Copy the second part of user declarations.  */


/* Line 343 of yacc.c  */
#line 260 "parser.c"

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#elif (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
typedef signed char yytype_int8;
#else
typedef short int yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(e) ((void) (e))
#else
# define YYUSE(e) /* empty */
#endif

/* Identity function, used to suppress warnings about constant conditions.  */
#ifndef lint
# define YYID(n) (n)
#else
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static int
YYID (int yyi)
#else
static int
YYID (yyi)
    int yyi;
#endif
{
  return yyi;
}
#endif

#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (YYID (0))
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
	     && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
	 || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)				\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack_alloc, Stack, yysize);			\
	Stack = &yyptr->Stack_alloc;					\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (YYID (0))

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  YYSIZE_T yyi;				\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (YYID (0))
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  71
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   604

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  52
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  44
/* YYNRULES -- Number of rules.  */
#define YYNRULES  105
/* YYNRULES -- Number of states.  */
#define YYNSTATES  191

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   306

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     4,     7,    10,    12,    14,    16,    18,
      21,    24,    26,    28,    30,    32,    36,    40,    44,    48,
      52,    56,    60,    64,    68,    72,    76,    80,    84,    86,
      90,    94,    98,   101,   104,   107,   110,   113,   116,   118,
     122,   124,   126,   128,   132,   134,   136,   139,   142,   144,
     148,   153,   155,   159,   164,   169,   172,   179,   181,   183,
     187,   193,   194,   197,   198,   202,   206,   210,   212,   216,
     222,   223,   228,   229,   232,   233,   235,   238,   239,   244,
     246,   250,   252,   254,   256,   258,   260,   262,   263,   265,
     269,   274,   276,   281,   284,   285,   286,   290,   292,   296,
     300,   301,   302,   310,   318,   322
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] =
{
      53,     0,    -1,    -1,    54,    53,    -1,    55,    28,    -1,
      84,    -1,    90,    -1,    94,    -1,    95,    -1,    27,    28,
      -1,    29,    28,    -1,    71,    -1,    79,    -1,    28,    -1,
      57,    -1,    55,    12,    55,    -1,    55,    13,    55,    -1,
      55,    14,    55,    -1,    55,    15,    55,    -1,    55,    16,
      55,    -1,    55,     8,    55,    -1,    55,     9,    55,    -1,
      55,    10,    55,    -1,    55,    11,    55,    -1,    55,     6,
      55,    -1,    55,     7,    55,    -1,    55,     5,    55,    -1,
      55,     4,    55,    -1,    56,    -1,    55,    30,    55,    -1,
      55,    31,    55,    -1,    25,    55,    26,    -1,    13,    55,
      -1,    17,    55,    -1,    18,    59,    -1,    59,    18,    -1,
      19,    59,    -1,    59,    19,    -1,    58,    -1,    59,     3,
      55,    -1,    59,    -1,    62,    -1,    68,    -1,    25,    79,
      26,    -1,    80,    -1,    39,    -1,    32,    39,    -1,    33,
      39,    -1,    61,    -1,    59,    21,    39,    -1,    59,    23,
      55,    24,    -1,    60,    -1,    62,    21,    39,    -1,    62,
      23,    55,    24,    -1,    62,    25,    66,    26,    -1,    59,
      63,    -1,    25,    79,    26,    25,    66,    26,    -1,    64,
      -1,    65,    -1,    25,    66,    26,    -1,    22,    39,    25,
      66,    26,    -1,    -1,    55,    67,    -1,    -1,    34,    55,
      67,    -1,    23,    66,    24,    -1,    23,    69,    24,    -1,
      70,    -1,    69,    34,    70,    -1,    37,    55,    35,    55,
      36,    -1,    -1,    37,    72,    73,    36,    -1,    -1,    54,
      73,    -1,    -1,    39,    -1,    38,    74,    -1,    -1,    25,
      77,    81,    26,    -1,    71,    -1,    75,    76,    78,    -1,
      49,    -1,    40,    -1,    41,    -1,    42,    -1,    43,    -1,
      50,    -1,    -1,    39,    -1,    81,    34,    39,    -1,    44,
      25,    55,    26,    -1,    45,    -1,    82,    54,    83,    54,
      -1,    82,    54,    -1,    -1,    -1,    85,    54,    86,    -1,
      47,    -1,    25,    55,    26,    -1,    88,    89,    87,    -1,
      -1,    -1,    46,    25,    66,    28,    92,    55,    28,    -1,
      93,    91,    66,    26,    91,    87,    91,    -1,    48,    55,
      28,    -1,    48,    28,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,    72,    72,    73,    77,    82,    86,    90,    94,    98,
     101,   104,   108,   112,   119,   123,   129,   134,   139,   144,
     149,   157,   166,   174,   182,   191,   199,   204,   209,   213,
     218,   226,   229,   235,   240,   252,   268,   280,   295,   301,
     318,   321,   324,   327,   331,   337,   341,   345,   349,   356,
     360,   369,   370,   375,   385,   388,   400,   409,   413,   419,
     429,   439,   447,   457,   464,   476,   490,   521,   527,   537,
     544,   544,   549,   550,   554,   565,   578,   598,   598,   612,
     622,   642,   646,   651,   655,   659,   663,   669,   670,   671,
     676,   683,   688,   693,   700,   702,   704,   709,   722,   730,
     750,   755,   760,   776,   799,   802
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "ASSIGN", "OR", "AND", "EQ", "N_EQ",
  "B_T", "BE_T", "L_T", "LE_T", "PLUS", "MINUS", "MUL", "DIV", "MOD",
  "NOT", "PP", "MM", "UMINUS", "F", "FF", "L_B", "R_B", "L_P", "R_P",
  "BREAK", "SEMICOLON", "CONTINUE", "A_N_D", "O_R", "LOCAL", "CATHOL",
  "COMMA", "UP_DOWN_DOT", "ENDBLOCK", "BLOCK", "FUNCTION", "IDENTIFIER",
  "STRING", "NIL", "TRUE", "FALSE", "IF", "ELSE", "FOR", "WHILE", "RETURN",
  "INTEGER", "REAL", "THEN", "$accept", "program", "stmt", "expression",
  "term", "assignexpr", "primary", "lvalue", "tableitem", "member", "call",
  "callsuffix", "normcall", "methodcall", "elist", "e_list", "tablemake",
  "indexed", "indexedelem", "block", "$@1", "stmt1", "funcname",
  "funcprefix", "funcargs", "$@2", "funcbody", "funcdef", "const",
  "idlist", "ifprefix", "elseprefix", "ifstmt", "loopstart", "loopend",
  "loopstmt", "whilestart", "whilecond", "whilestmt", "N", "M",
  "forprefix", "forstmt", "returnstmt", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    52,    53,    53,    54,    54,    54,    54,    54,    54,
      54,    54,    54,    54,    55,    55,    55,    55,    55,    55,
      55,    55,    55,    55,    55,    55,    55,    55,    55,    55,
      55,    56,    56,    56,    56,    56,    56,    56,    56,    57,
      58,    58,    58,    58,    58,    59,    59,    59,    59,    60,
      60,    61,    61,    61,    62,    62,    62,    63,    63,    64,
      65,    66,    66,    67,    67,    68,    68,    69,    69,    70,
      72,    71,    73,    73,    74,    74,    75,    77,    76,    78,
      79,    80,    80,    80,    80,    80,    80,    81,    81,    81,
      82,    83,    84,    84,    85,    86,    87,    88,    89,    90,
      91,    92,    93,    94,    95,    95
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     0,     2,     2,     1,     1,     1,     1,     2,
       2,     1,     1,     1,     1,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     1,     3,
       3,     3,     2,     2,     2,     2,     2,     2,     1,     3,
       1,     1,     1,     3,     1,     1,     2,     2,     1,     3,
       4,     1,     3,     4,     4,     2,     6,     1,     1,     3,
       5,     0,     2,     0,     3,     3,     3,     1,     3,     5,
       0,     4,     0,     2,     0,     1,     2,     0,     4,     1,
       3,     1,     1,     1,     1,     1,     1,     0,     1,     3,
       4,     1,     4,     2,     0,     0,     3,     1,     3,     3,
       0,     0,     7,     7,     3,     2
};

/* YYDEFACT[STATE-NAME] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       2,     0,     0,     0,     0,    61,     0,     0,    13,     0,
       0,     0,    70,    74,    45,    82,    83,    84,    85,     0,
       0,    97,     0,    81,    86,     0,     2,     0,    28,    14,
      38,    40,    51,    48,    41,    42,    11,     0,    12,    44,
       0,     5,     0,     6,   100,     7,     8,    32,    33,     0,
      34,     0,    36,     0,    63,     0,     0,    67,     0,     0,
       9,    10,    46,    47,    72,    75,    76,     0,    61,   105,
       0,     1,     3,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     4,     0,     0,     0,
      35,    37,     0,     0,     0,    61,    55,    57,    58,     0,
       0,    61,    77,     0,    93,     0,    94,    61,     0,     0,
       0,    62,    65,    66,     0,    31,    43,    72,     0,     0,
       0,   104,    27,    26,    24,    25,    20,    21,    22,    23,
      15,    16,    17,    18,    19,    29,    30,    39,    49,     0,
       0,     0,    52,     0,     0,    87,    79,    80,    91,     0,
       0,     0,    99,     0,     0,     0,    63,    68,    61,    73,
      71,    90,   101,    61,    50,    59,    53,    54,    88,     0,
      92,    98,    95,   100,     0,    64,     0,     0,     0,    78,
       0,    96,    94,    69,    56,     0,    60,    89,   100,   102,
     103
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,    25,    26,    27,    28,    29,    30,    31,    32,    33,
      34,    96,    97,    98,    55,   111,    35,    56,    57,    36,
      64,   118,    66,    37,   103,   145,   147,    38,    39,   169,
      40,   149,    41,   151,   181,   152,    42,   106,    43,   107,
     177,    44,    45,    46
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -167
static const yytype_int16 yypact[] =
{
      95,   244,   244,    -2,    -2,   142,   176,   -22,  -167,     5,
       6,    12,  -167,    16,  -167,  -167,  -167,  -167,  -167,    18,
      39,  -167,   210,  -167,  -167,    59,    95,   349,  -167,  -167,
    -167,    35,  -167,  -167,    19,  -167,  -167,    43,  -167,  -167,
      95,  -167,    44,  -167,  -167,  -167,  -167,  -167,  -167,    32,
      40,    19,    40,   244,   336,    47,     2,  -167,   377,    63,
    -167,  -167,  -167,  -167,    95,  -167,  -167,   244,   244,  -167,
     405,  -167,  -167,   244,   244,   244,   244,   244,   244,   244,
     244,   244,   244,   244,   244,   244,  -167,   244,   244,   244,
    -167,  -167,    51,    52,   244,   244,  -167,  -167,  -167,    56,
     244,   244,  -167,    61,    57,   244,  -167,   244,    70,   304,
     244,  -167,  -167,  -167,    64,  -167,    78,    95,    69,   433,
      79,  -167,     4,   416,   556,   556,   386,   386,   386,   386,
      34,    34,  -167,  -167,  -167,   416,     4,   573,  -167,    85,
     461,    89,  -167,   489,    90,    72,  -167,  -167,  -167,    95,
     517,    95,  -167,    99,    78,   244,   336,  -167,   244,  -167,
    -167,  -167,  -167,   244,  -167,  -167,  -167,  -167,  -167,    13,
    -167,  -167,  -167,  -167,   291,  -167,   100,   244,   103,  -167,
      82,  -167,  -167,  -167,  -167,   545,  -167,  -167,  -167,  -167,
    -167
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -167,   104,   -32,    -1,  -167,  -167,  -167,    21,  -167,  -167,
      24,  -167,  -167,  -167,   -66,   -25,  -167,  -167,    26,    45,
    -167,    29,  -167,  -167,  -167,  -167,  -167,    -3,  -167,  -167,
    -167,  -167,  -167,  -167,  -167,   -35,  -167,  -167,  -167,  -166,
    -167,  -167,  -167,  -167
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -1
static const yytype_int16 yytable[] =
{
      47,    48,   120,    59,    54,    58,    60,   182,   104,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    70,   190,    49,    50,    52,   113,    51,    51,   141,
      10,    11,   117,    61,    87,   144,   114,    14,    89,   179,
      99,   153,   100,    67,   101,    62,   108,   180,    83,    84,
      85,    63,   109,    90,    91,    65,    92,    93,    94,    71,
      95,    92,    93,    94,    68,    95,   119,    54,   102,   105,
      13,   112,   122,   123,   124,   125,   126,   127,   128,   129,
     130,   131,   132,   133,   134,   117,   135,   136,   137,   116,
     138,   139,   176,   140,    54,   142,   154,   178,    12,   143,
      54,    53,   148,   158,   150,   160,    54,   162,     1,   156,
     163,   168,     2,     3,     4,   165,   167,   170,     5,   172,
       6,   187,     7,     8,     9,   173,   184,    10,    11,   186,
      72,   175,    12,    13,    14,    15,    16,    17,    18,    19,
     157,    20,    21,    22,    23,    24,   159,   188,   146,     0,
       0,     0,     0,     0,   174,     1,     0,    54,     0,     2,
       3,     4,    54,     0,     0,     5,     0,     6,     0,     0,
       0,     0,     0,     0,    10,    11,   185,     0,     0,    53,
       0,    14,    15,    16,    17,    18,     0,     0,     0,     1,
       0,    23,    24,     2,     3,     4,     0,     0,     0,     5,
       0,     6,     0,     0,     0,     0,     0,     0,    10,    11,
       0,     0,     0,     0,    13,    14,    15,    16,    17,    18,
       0,     0,     0,     1,     0,    23,    24,     2,     3,     4,
       0,     0,     0,     5,     0,     6,     0,     0,    69,     0,
       0,     0,    10,    11,     0,     0,     0,     0,     0,    14,
      15,    16,    17,    18,     0,     0,     0,     1,     0,    23,
      24,     2,     3,     4,     0,     0,     0,     5,     0,     6,
       0,     0,     0,     0,     0,     0,    10,    11,     0,     0,
       0,     0,     0,    14,    15,    16,    17,    18,     0,     0,
       0,     0,     0,    23,    24,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    87,    88,     0,     0,     0,     0,   183,     0,     0,
       0,     0,     0,     0,    87,    88,     0,     0,     0,   155,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    87,    88,     0,     0,
     110,     0,     0,     0,     0,     0,     0,    86,     0,    87,
      88,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    -1,    -1,    -1,    -1,    81,    82,
      83,    84,    85,   115,     0,     0,     0,    87,    88,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,   121,     0,    87,    88,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   161,
       0,     0,     0,    87,    88,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,     0,     0,
       0,     0,     0,     0,     0,   164,     0,     0,     0,     0,
       0,    87,    88,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,     0,     0,     0,     0,
       0,     0,     0,   166,     0,     0,     0,     0,     0,    87,
      88,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   171,     0,     0,     0,    87,    88,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    -1,    -1,    77,    78,    79,    80,    81,    82,
      83,    84,    85,   189,     0,    87,    88,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    87,    88
};

#define yypact_value_is_default(yystate) \
  ((yystate) == (-167))

#define yytable_value_is_error(yytable_value) \
  ((yytable_value) == (-1))

static const yytype_int16 yycheck[] =
{
       1,     2,    68,     6,     5,     6,    28,   173,    40,     5,
       6,     7,     8,     9,    10,    11,    12,    13,    14,    15,
      16,    22,   188,    25,     3,     4,    24,     3,     4,    95,
      32,    33,    64,    28,    30,   101,    34,    39,     3,    26,
      21,   107,    23,    25,    25,    39,    49,    34,    14,    15,
      16,    39,    53,    18,    19,    39,    21,    22,    23,     0,
      25,    21,    22,    23,    25,    25,    67,    68,    25,    25,
      38,    24,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,   117,    87,    88,    89,    26,
      39,    39,   158,    94,    95,    39,    26,   163,    37,   100,
     101,    37,    45,    25,   105,    36,   107,    28,    13,   110,
      25,    39,    17,    18,    19,    26,    26,   149,    23,   151,
      25,    39,    27,    28,    29,    26,    26,    32,    33,    26,
      26,   156,    37,    38,    39,    40,    41,    42,    43,    44,
     114,    46,    47,    48,    49,    50,   117,   182,   103,    -1,
      -1,    -1,    -1,    -1,   155,    13,    -1,   158,    -1,    17,
      18,    19,   163,    -1,    -1,    23,    -1,    25,    -1,    -1,
      -1,    -1,    -1,    -1,    32,    33,   177,    -1,    -1,    37,
      -1,    39,    40,    41,    42,    43,    -1,    -1,    -1,    13,
      -1,    49,    50,    17,    18,    19,    -1,    -1,    -1,    23,
      -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    32,    33,
      -1,    -1,    -1,    -1,    38,    39,    40,    41,    42,    43,
      -1,    -1,    -1,    13,    -1,    49,    50,    17,    18,    19,
      -1,    -1,    -1,    23,    -1,    25,    -1,    -1,    28,    -1,
      -1,    -1,    32,    33,    -1,    -1,    -1,    -1,    -1,    39,
      40,    41,    42,    43,    -1,    -1,    -1,    13,    -1,    49,
      50,    17,    18,    19,    -1,    -1,    -1,    23,    -1,    25,
      -1,    -1,    -1,    -1,    -1,    -1,    32,    33,    -1,    -1,
      -1,    -1,    -1,    39,    40,    41,    42,    43,    -1,    -1,
      -1,    -1,    -1,    49,    50,     4,     5,     6,     7,     8,
       9,    10,    11,    12,    13,    14,    15,    16,     4,     5,
       6,     7,     8,     9,    10,    11,    12,    13,    14,    15,
      16,    30,    31,    -1,    -1,    -1,    -1,    36,    -1,    -1,
      -1,    -1,    -1,    -1,    30,    31,    -1,    -1,    -1,    35,
       4,     5,     6,     7,     8,     9,    10,    11,    12,    13,
      14,    15,    16,     4,     5,     6,     7,     8,     9,    10,
      11,    12,    13,    14,    15,    16,    30,    31,    -1,    -1,
      34,    -1,    -1,    -1,    -1,    -1,    -1,    28,    -1,    30,
      31,     4,     5,     6,     7,     8,     9,    10,    11,    12,
      13,    14,    15,    16,     8,     9,    10,    11,    12,    13,
      14,    15,    16,    26,    -1,    -1,    -1,    30,    31,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,     6,     7,     8,     9,    10,    11,    12,    13,
      14,    15,    16,    28,    -1,    30,    31,     4,     5,     6,
       7,     8,     9,    10,    11,    12,    13,    14,    15,    16,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    26,
      -1,    -1,    -1,    30,    31,     4,     5,     6,     7,     8,
       9,    10,    11,    12,    13,    14,    15,    16,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    24,    -1,    -1,    -1,    -1,
      -1,    30,    31,     4,     5,     6,     7,     8,     9,    10,
      11,    12,    13,    14,    15,    16,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    24,    -1,    -1,    -1,    -1,    -1,    30,
      31,     4,     5,     6,     7,     8,     9,    10,    11,    12,
      13,    14,    15,    16,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    26,    -1,    -1,    -1,    30,    31,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,     6,     7,     8,     9,    10,    11,    12,    13,
      14,    15,    16,    28,    -1,    30,    31,     4,     5,     6,
       7,     8,     9,    10,    11,    12,    13,    14,    15,    16,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    30,    31
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    13,    17,    18,    19,    23,    25,    27,    28,    29,
      32,    33,    37,    38,    39,    40,    41,    42,    43,    44,
      46,    47,    48,    49,    50,    53,    54,    55,    56,    57,
      58,    59,    60,    61,    62,    68,    71,    75,    79,    80,
      82,    84,    88,    90,    93,    94,    95,    55,    55,    25,
      59,    62,    59,    37,    55,    66,    69,    70,    55,    79,
      28,    28,    39,    39,    72,    39,    74,    25,    25,    28,
      55,     0,    53,     4,     5,     6,     7,     8,     9,    10,
      11,    12,    13,    14,    15,    16,    28,    30,    31,     3,
      18,    19,    21,    22,    23,    25,    63,    64,    65,    21,
      23,    25,    25,    76,    54,    25,    89,    91,    79,    55,
      34,    67,    24,    24,    34,    26,    26,    54,    73,    55,
      66,    28,    55,    55,    55,    55,    55,    55,    55,    55,
      55,    55,    55,    55,    55,    55,    55,    55,    39,    39,
      55,    66,    39,    55,    66,    77,    71,    78,    45,    83,
      55,    85,    87,    66,    26,    35,    55,    70,    25,    73,
      36,    26,    28,    25,    24,    26,    24,    26,    39,    81,
      54,    26,    54,    26,    55,    67,    66,    92,    66,    26,
      34,    86,    91,    36,    26,    55,    26,    39,    87,    28,
      91
};

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  However,
   YYFAIL appears to be in use.  Nevertheless, it is formally deprecated
   in Bison 2.4.2's NEWS entry, where a plan to phase it out is
   discussed.  */

#define YYFAIL		goto yyerrlab
#if defined YYFAIL
  /* This is here to suppress warnings from the GCC cpp's
     -Wunused-macros.  Normally we don't worry about that warning, but
     some users do, and we want to make it easy for users to remove
     YYFAIL uses, which will produce warnings from Bison 2.5.  */
#endif

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      YYPOPSTACK (1);						\
      goto yybackup;						\
    }								\
  else								\
    {								\
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;							\
    }								\
while (YYID (0))


#define YYTERROR	1
#define YYERRCODE	256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#define YYRHSLOC(Rhs, K) ((Rhs)[K])
#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)				\
    do									\
      if (YYID (N))                                                    \
	{								\
	  (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;	\
	  (Current).first_column = YYRHSLOC (Rhs, 1).first_column;	\
	  (Current).last_line    = YYRHSLOC (Rhs, N).last_line;		\
	  (Current).last_column  = YYRHSLOC (Rhs, N).last_column;	\
	}								\
      else								\
	{								\
	  (Current).first_line   = (Current).last_line   =		\
	    YYRHSLOC (Rhs, 0).last_line;				\
	  (Current).first_column = (Current).last_column =		\
	    YYRHSLOC (Rhs, 0).last_column;				\
	}								\
    while (YYID (0))
#endif


/* This macro is provided for backward compatibility. */

#ifndef YY_LOCATION_PRINT
# define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */

#ifdef YYLEX_PARAM
# define YYLEX yylex (YYLEX_PARAM)
#else
# define YYLEX yylex ()
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (YYID (0))

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)			  \
do {									  \
  if (yydebug)								  \
    {									  \
      YYFPRINTF (stderr, "%s ", Title);					  \
      yy_symbol_print (stderr,						  \
		  Type, Value); \
      YYFPRINTF (stderr, "\n");						  \
    }									  \
} while (YYID (0))


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_value_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# else
  YYUSE (yyoutput);
# endif
  switch (yytype)
    {
      default:
	break;
    }
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
#else
static void
yy_stack_print (yybottom, yytop)
    yytype_int16 *yybottom;
    yytype_int16 *yytop;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (YYID (0))


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_reduce_print (YYSTYPE *yyvsp, int yyrule)
#else
static void
yy_reduce_print (yyvsp, yyrule)
    YYSTYPE *yyvsp;
    int yyrule;
#endif
{
  int yynrhs = yyr2[yyrule];
  int yyi;
  unsigned long int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
	     yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
		       &(yyvsp[(yyi + 1) - (yynrhs)])
		       		       );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (yyvsp, Rule); \
} while (YYID (0))

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif


#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static YYSIZE_T
yystrlen (const char *yystr)
#else
static YYSIZE_T
yystrlen (yystr)
    const char *yystr;
#endif
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static char *
yystpcpy (char *yydest, const char *yysrc)
#else
static char *
yystpcpy (yydest, yysrc)
    char *yydest;
    const char *yysrc;
#endif
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
	switch (*++yyp)
	  {
	  case '\'':
	  case ',':
	    goto do_not_strip_quotes;

	  case '\\':
	    if (*++yyp != '\\')
	      goto do_not_strip_quotes;
	    /* Fall through.  */
	  default:
	    if (yyres)
	      yyres[yyn] = *yyp;
	    yyn++;
	    break;

	  case '"':
	    if (yyres)
	      yyres[yyn] = '\0';
	    return yyn;
	  }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYSIZE_T *yymsg_alloc, char **yymsg,
                yytype_int16 *yyssp, int yytoken)
{
  YYSIZE_T yysize0 = yytnamerr (0, yytname[yytoken]);
  YYSIZE_T yysize = yysize0;
  YYSIZE_T yysize1;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = 0;
  /* Arguments of yyformat. */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Number of reported tokens (one for the "unexpected", one per
     "expected"). */
  int yycount = 0;

  /* There are many possibilities here to consider:
     - Assume YYFAIL is not used.  It's too flawed to consider.  See
       <http://lists.gnu.org/archive/html/bison-patches/2009-12/msg00024.html>
       for details.  YYERROR is fine as it does not invoke this
       function.
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[*yyssp];
      yyarg[yycount++] = yytname[yytoken];
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for
             this state because they are default actions.  */
          int yyxbegin = yyn < 0 ? -yyn : 0;
          /* Stay within bounds of both yycheck and yytname.  */
          int yychecklim = YYLAST - yyn + 1;
          int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
          int yyx;

          for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                yysize1 = yysize + yytnamerr (0, yytname[yyx]);
                if (! (yysize <= yysize1
                       && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
                  return 2;
                yysize = yysize1;
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  yysize1 = yysize + yystrlen (yyformat);
  if (! (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
    return 2;
  yysize = yysize1;

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yyarg[yyi++]);
          yyformat += 2;
        }
      else
        {
          yyp++;
          yyformat++;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
#else
static void
yydestruct (yymsg, yytype, yyvaluep)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  YYUSE (yyvaluep);

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  switch (yytype)
    {

      default:
	break;
    }
}


/* Prevent warnings from -Wmissing-prototypes.  */
#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int yyparse (void *YYPARSE_PARAM);
#else
int yyparse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int yyparse (void);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */


/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;

/* Number of syntax errors so far.  */
int yynerrs;


/*----------.
| yyparse.  |
`----------*/

#ifdef YYPARSE_PARAM
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void *YYPARSE_PARAM)
#else
int
yyparse (YYPARSE_PARAM)
    void *YYPARSE_PARAM;
#endif
#else /* ! YYPARSE_PARAM */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void)
#else
int
yyparse ()

#endif
#endif
{
    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       `yyss': related to states.
       `yyvs': related to semantic values.

       Refer to the stacks thru separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yytoken = 0;
  yyss = yyssa;
  yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */
  yyssp = yyss;
  yyvsp = yyvs;

  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack.  Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	yytype_int16 *yyss1 = yyss;

	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow (YY_("memory exhausted"),
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),
		    &yystacksize);

	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
	goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
	yystacksize = YYMAXDEPTH;

      {
	yytype_int16 *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyexhaustedlab;
	YYSTACK_RELOCATE (yyss_alloc, yyss);
	YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  *++yyvsp = yylval;

  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:

/* Line 1806 of yacc.c  */
#line 72 "parser.y"
    {printf("program\n");}
    break;

  case 3:

/* Line 1806 of yacc.c  */
#line 73 "parser.y"
    {printf("stmt program\n");}
    break;

  case 4:

/* Line 1806 of yacc.c  */
#line 77 "parser.y"
    {
          //  resettemp();
            printf(" expression;\n");

        }
    break;

  case 5:

/* Line 1806 of yacc.c  */
#line 82 "parser.y"
    {
          //  resettemp();
            printf("ifstmt\n");
        }
    break;

  case 6:

/* Line 1806 of yacc.c  */
#line 86 "parser.y"
    {
           // resettemp();
            printf("whilestmt\n");
        }
    break;

  case 7:

/* Line 1806 of yacc.c  */
#line 90 "parser.y"
    {
           // resettemp();
            printf("forstmt\n");
        }
    break;

  case 8:

/* Line 1806 of yacc.c  */
#line 94 "parser.y"
    {
            Manage_stmt_returnstmt(yylineno,infunction);
           // resettemp();
        }
    break;

  case 9:

/* Line 1806 of yacc.c  */
#line 98 "parser.y"
    {
            Manage_stmt_BREAK_SEMICOLON(yylineno,inloop);
        }
    break;

  case 10:

/* Line 1806 of yacc.c  */
#line 101 "parser.y"
    {
            Manage_stmt_CONTINUE_SEMICOLON(yylineno,inloop);
        }
    break;

  case 11:

/* Line 1806 of yacc.c  */
#line 104 "parser.y"
    {
          //  resettemp();
            printf("block\n");
        }
    break;

  case 12:

/* Line 1806 of yacc.c  */
#line 108 "parser.y"
    {
         //   resettemp();
            printf("funcdef\n");
        }
    break;

  case 13:

/* Line 1806 of yacc.c  */
#line 112 "parser.y"
    {
          //  resettemp();
            printf(";\n");
        }
    break;

  case 14:

/* Line 1806 of yacc.c  */
#line 119 "parser.y"
    {
            printf("assignexpr\n");
            (yyval.exprVal)=(yyvsp[(1) - (1)].exprVal);
        }
    break;

  case 15:

/* Line 1806 of yacc.c  */
#line 123 "parser.y"
    {
            printf("expression + expression\n");
            (yyval.exprVal)= newexpr(arithexpr_e);
            (yyval.exprVal)->sym = newtemp(scope , yylineno , currscopespace() , currscopeoffset());
            emit(add, (yyvsp[(1) - (3)].exprVal) , (yyvsp[(3) - (3)].exprVal),(yyval.exprVal), -1,yylineno);
        }
    break;

  case 16:

/* Line 1806 of yacc.c  */
#line 129 "parser.y"
    {printf("expression - expression\n");
            (yyval.exprVal)= newexpr(arithexpr_e);
            (yyval.exprVal)->sym = newtemp(scope , yylineno , currscopespace() , currscopeoffset());
            emit(sub, (yyvsp[(1) - (3)].exprVal) , (yyvsp[(3) - (3)].exprVal),(yyval.exprVal), -1,yylineno);
        }
    break;

  case 17:

/* Line 1806 of yacc.c  */
#line 134 "parser.y"
    {printf("expression * expression\n");
            (yyval.exprVal)= newexpr(arithexpr_e);
            (yyval.exprVal)->sym = newtemp(scope , yylineno , currscopespace() , currscopeoffset());
            emit(mul, (yyvsp[(1) - (3)].exprVal) , (yyvsp[(3) - (3)].exprVal),(yyval.exprVal), -1,yylineno);
        }
    break;

  case 18:

/* Line 1806 of yacc.c  */
#line 139 "parser.y"
    {printf("expression / expression\n");
            (yyval.exprVal)= newexpr(arithexpr_e);
            (yyval.exprVal)->sym = newtemp(scope , yylineno , currscopespace() , currscopeoffset());
            emit(divide, (yyvsp[(1) - (3)].exprVal) , (yyvsp[(3) - (3)].exprVal),(yyval.exprVal), -1,yylineno);
        }
    break;

  case 19:

/* Line 1806 of yacc.c  */
#line 144 "parser.y"
    {printf("expression %% expression\n");
            (yyval.exprVal)= newexpr(arithexpr_e);
            (yyval.exprVal)->sym = newtemp(scope , yylineno , currscopespace() , currscopeoffset());
            emit(mod, (yyvsp[(1) - (3)].exprVal) , (yyvsp[(3) - (3)].exprVal),(yyval.exprVal), -1,yylineno);
        }
    break;

  case 20:

/* Line 1806 of yacc.c  */
#line 149 "parser.y"
    {printf("expression > expression\n");
            (yyval.exprVal) = newexpr(boolexpr_e);
            (yyval.exprVal)->sym = newtemp(scope , yylineno , currscopespace() , currscopeoffset());
            emit(if_greater, (yyvsp[(1) - (3)].exprVal) , (yyvsp[(3) - (3)].exprVal),NULL, nextquadlabel()+3, yylineno);
            emit(assign, newexpr_constbool(false),NULL ,(yyval.exprVal),-1,yylineno);
            emit(jump,NULL,NULL,NULL, nextquadlabel()+2,yylineno);
            emit(assign, newexpr_constbool(true),NULL, (yyval.exprVal),-1,yylineno);
        }
    break;

  case 21:

/* Line 1806 of yacc.c  */
#line 157 "parser.y"
    {printf("expression >= expression\n");
            (yyval.exprVal) = newexpr(boolexpr_e);
            (yyval.exprVal)->sym = newtemp(scope , yylineno , currscopespace() , currscopeoffset());
            emit(if_greatereq, (yyvsp[(1) - (3)].exprVal) , (yyvsp[(3) - (3)].exprVal),NULL, nextquadlabel()+3, yylineno);
            emit(assign, newexpr_constbool(false),NULL, (yyval.exprVal),-1,yylineno);
            emit(jump,NULL,NULL,NULL, nextquadlabel()+2,yylineno);
            emit(assign, newexpr_constbool(true),NULL, (yyval.exprVal),-1,yylineno);

        }
    break;

  case 22:

/* Line 1806 of yacc.c  */
#line 166 "parser.y"
    {printf("expression < expression\n");
            (yyval.exprVal) = newexpr(boolexpr_e);
            (yyval.exprVal)->sym = newtemp(scope , yylineno , currscopespace() , currscopeoffset());
            emit(if_less, (yyvsp[(1) - (3)].exprVal) , (yyvsp[(3) - (3)].exprVal),NULL, nextquadlabel()+3, yylineno);
            emit(assign, newexpr_constbool(false),NULL, (yyval.exprVal),-1,yylineno);
            emit(jump,NULL,NULL,NULL, nextquadlabel()+2,yylineno);
            emit(assign, newexpr_constbool(true),NULL, (yyval.exprVal),-1,yylineno);
        }
    break;

  case 23:

/* Line 1806 of yacc.c  */
#line 174 "parser.y"
    {printf("expression <= expression\n");
            (yyval.exprVal) = newexpr(boolexpr_e);
            (yyval.exprVal)->sym = newtemp(scope , yylineno , currscopespace() , currscopeoffset());
            emit(if_lesseq, (yyvsp[(1) - (3)].exprVal) , (yyvsp[(3) - (3)].exprVal),NULL, nextquadlabel()+3, yylineno);
            emit(assign, newexpr_constbool(false),NULL, (yyval.exprVal),-1,yylineno);
            emit(jump,NULL,NULL,NULL, nextquadlabel()+2,yylineno);
            emit(assign, newexpr_constbool(true),NULL, (yyval.exprVal),-1,yylineno);
        }
    break;

  case 24:

/* Line 1806 of yacc.c  */
#line 182 "parser.y"
    {printf("expression == expression\n");
            (yyval.exprVal) = newexpr(boolexpr_e);
            (yyval.exprVal)->sym = newtemp(scope , yylineno , currscopespace() , currscopeoffset());
            emit(if_eq, (yyvsp[(1) - (3)].exprVal) , (yyvsp[(3) - (3)].exprVal),NULL, nextquadlabel()+3, yylineno);
            emit(assign, newexpr_constbool(false),NULL, (yyval.exprVal),-1,yylineno);
            emit(jump,NULL,NULL,NULL, nextquadlabel()+2,yylineno);
            emit(assign, newexpr_constbool(true),NULL, (yyval.exprVal),-1,yylineno);
        }
    break;

  case 25:

/* Line 1806 of yacc.c  */
#line 191 "parser.y"
    {printf("expression != expression\n");
            (yyval.exprVal) = newexpr(boolexpr_e);
            (yyval.exprVal)->sym = newtemp(scope , yylineno , currscopespace() , currscopeoffset());
            emit(if_not_eq, (yyvsp[(1) - (3)].exprVal) , (yyvsp[(3) - (3)].exprVal),NULL, nextquadlabel()+3, yylineno);
            emit(assign, newexpr_constbool(false),NULL, (yyval.exprVal),-1,yylineno);
            emit(jump,NULL,NULL,NULL, nextquadlabel()+2,yylineno);
            emit(assign, newexpr_constbool(true),NULL, (yyval.exprVal),-1,yylineno);
        }
    break;

  case 26:

/* Line 1806 of yacc.c  */
#line 199 "parser.y"
    {printf("expression and expression\n");
            (yyval.exprVal) = newexpr(boolexpr_e);
            (yyval.exprVal)->sym = newtemp(scope , yylineno , currscopespace() , currscopeoffset());
            emit(and, (yyvsp[(1) - (3)].exprVal) , (yyvsp[(3) - (3)].exprVal), (yyval.exprVal), -1,yylineno);
        }
    break;

  case 27:

/* Line 1806 of yacc.c  */
#line 204 "parser.y"
    {printf("expression or expression\n");
            (yyval.exprVal) = newexpr(boolexpr_e);
            (yyval.exprVal)->sym = newtemp(scope , yylineno , currscopespace() , currscopeoffset());
            emit(or, (yyvsp[(1) - (3)].exprVal) , (yyvsp[(3) - (3)].exprVal), (yyval.exprVal), -1,yylineno);
        }
    break;

  case 28:

/* Line 1806 of yacc.c  */
#line 209 "parser.y"
    {
            printf("term\n");
            (yyval.exprVal) = (yyvsp[(1) - (1)].exprVal);
        }
    break;

  case 29:

/* Line 1806 of yacc.c  */
#line 213 "parser.y"
    {printf("expression boolop expression\n");
            (yyval.exprVal) = newexpr(boolexpr_e);
            (yyval.exprVal)->sym = newtemp(scope , yylineno , currscopespace() , currscopeoffset());
            emit(and, (yyvsp[(1) - (3)].exprVal) , (yyvsp[(3) - (3)].exprVal), (yyval.exprVal), -1,yylineno);
        }
    break;

  case 30:

/* Line 1806 of yacc.c  */
#line 218 "parser.y"
    {printf("expression boolop expression\n");
            (yyval.exprVal) = newexpr(boolexpr_e);
            (yyval.exprVal)->sym = newtemp(scope , yylineno , currscopespace() , currscopeoffset());
            emit(or, (yyvsp[(1) - (3)].exprVal) , (yyvsp[(3) - (3)].exprVal), (yyval.exprVal), -1,yylineno);
        }
    break;

  case 31:

/* Line 1806 of yacc.c  */
#line 226 "parser.y"
    {printf("(expression)\n");
            (yyval.exprVal) = (yyvsp[(2) - (3)].exprVal);
        }
    break;

  case 32:

/* Line 1806 of yacc.c  */
#line 229 "parser.y"
    {printf("-expression\n");
            checkuminus((yyvsp[(2) - (2)].exprVal),yylineno);
            (yyval.exprVal)= newexpr(arithexpr_e);
            (yyval.exprVal)->sym = newtemp(scope , yylineno , currscopespace() , currscopeoffset());
            emit(uminus, (yyvsp[(2) - (2)].exprVal),NULL, (yyval.exprVal),-1, yylineno);
        }
    break;

  case 33:

/* Line 1806 of yacc.c  */
#line 235 "parser.y"
    {printf("not expression\n");
            (yyval.exprVal) = newexpr(boolexpr_e);
            (yyval.exprVal)->sym = newtemp(scope , yylineno , currscopespace() , currscopeoffset());
            emit(not, (yyvsp[(2) - (2)].exprVal) , NULL, (yyval.exprVal), -1,yylineno);
        }
    break;

  case 34:

/* Line 1806 of yacc.c  */
#line 240 "parser.y"
    { Manage_term_PP_lvalue((yyvsp[(2) - (2)].exprVal)->sym , yylineno);
            if ((yyvsp[(2) - (2)].exprVal)->type == tableitem_e) {
                (yyval.exprVal)= emit_iftableitem((yyvsp[(2) - (2)].exprVal), yylineno);
                emit(add, (yyval.exprVal), newexpr_consnum(1), (yyval.exprVal), -1, yylineno);
                emit(tablesetelem,(yyvsp[(2) - (2)].exprVal), (yyvsp[(2) - (2)].exprVal)->index, (yyval.exprVal), -1, yylineno);
            }else {
                emit(add, (yyvsp[(2) - (2)].exprVal), newexpr_consnum(1), (yyvsp[(2) - (2)].exprVal), -1, yylineno);
                (yyval.exprVal)= newexpr(arithexpr_e);
                (yyval.exprVal)->sym= newtemp(scope , yylineno , currscopespace() , currscopeoffset());
                emit(assign, (yyvsp[(2) - (2)].exprVal),NULL, (yyval.exprVal),-1, yylineno);
            }
        }
    break;

  case 35:

/* Line 1806 of yacc.c  */
#line 252 "parser.y"
    { Manage_term_lvalue_PP((yyvsp[(1) - (2)].exprVal)->sym , yylineno);
            expr *value ;
            (yyval.exprVal) = newexpr(var_e);
            (yyval.exprVal)->sym = newtemp(scope , yylineno , currscopespace() , currscopeoffset());

            if ((yyvsp[(1) - (2)].exprVal)->type==tableitem_e){
                value = emit_iftableitem((yyvsp[(1) - (2)].exprVal), yylineno);
                emit(assign, value,NULL, (yyval.exprVal),-1, yylineno);
                emit(add, value, newexpr_consnum(1),value, -1, yylineno);
                emit( tablesetelem,(yyvsp[(1) - (2)].exprVal), (yyvsp[(1) - (2)].exprVal)->index, value,-1, yylineno);
            }else {
                emit(assign, (yyvsp[(1) - (2)].exprVal),NULL, (yyval.exprVal), -1, yylineno);
                emit(add, (yyvsp[(1) - (2)].exprVal), newexpr_consnum(1), (yyvsp[(1) - (2)].exprVal),-1, yylineno);
            }

        }
    break;

  case 36:

/* Line 1806 of yacc.c  */
#line 268 "parser.y"
    { Manage_term_MM_lvalue((yyvsp[(2) - (2)].exprVal)->sym , yylineno);
            if ((yyvsp[(2) - (2)].exprVal)->type== tableitem_e) {
                (yyval.exprVal)= emit_iftableitem((yyvsp[(2) - (2)].exprVal), yylineno);
                emit(sub, (yyval.exprVal), newexpr_consnum(1), (yyval.exprVal), -1, yylineno);
                emit(tablesetelem,(yyvsp[(2) - (2)].exprVal), (yyvsp[(2) - (2)].exprVal)->index, (yyval.exprVal), -1, yylineno);
            }else {
                emit(sub, (yyvsp[(2) - (2)].exprVal), newexpr_consnum(1), (yyvsp[(2) - (2)].exprVal), -1, yylineno);
                (yyval.exprVal)= newexpr(arithexpr_e);
                (yyval.exprVal)->sym= newtemp(scope , yylineno , currscopespace() , currscopeoffset());
                emit(assign, (yyvsp[(2) - (2)].exprVal),NULL, (yyval.exprVal),-1, yylineno);
            }
        }
    break;

  case 37:

/* Line 1806 of yacc.c  */
#line 280 "parser.y"
    { Manage_term_lvalue_MM((yyvsp[(1) - (2)].exprVal)->sym , yylineno);
            expr *value ;
            (yyval.exprVal) = newexpr(var_e);
            (yyval.exprVal)->sym = newtemp(scope , yylineno , currscopespace() , currscopeoffset());

            if ((yyvsp[(1) - (2)].exprVal)->type==tableitem_e){
                value = emit_iftableitem((yyvsp[(1) - (2)].exprVal), yylineno);
                emit(assign, value,NULL, (yyval.exprVal),-1, yylineno);
                emit(sub, value, newexpr_consnum(1),value, -1, yylineno);
                emit( tablesetelem,(yyvsp[(1) - (2)].exprVal), (yyvsp[(1) - (2)].exprVal)->index, value,-1, yylineno);
            }else {
                emit(assign, (yyvsp[(1) - (2)].exprVal),NULL, (yyval.exprVal), -1, yylineno);
                emit(sub, (yyvsp[(1) - (2)].exprVal), newexpr_consnum(1), (yyvsp[(1) - (2)].exprVal),-1, yylineno);
            }
        }
    break;

  case 38:

/* Line 1806 of yacc.c  */
#line 295 "parser.y"
    {printf("primary\n");
        (yyval.exprVal) = (yyvsp[(1) - (1)].exprVal);
    }
    break;

  case 39:

/* Line 1806 of yacc.c  */
#line 301 "parser.y"
    {
        Manage_assignexpr_lvalue_ASSIGN_expr((yyvsp[(1) - (3)].exprVal)->sym , yylineno);

        if ((yyvsp[(1) - (3)].exprVal)->type == tableitem_e) {
            emit(tablesetelem,(yyvsp[(1) - (3)].exprVal),(yyvsp[(1) - (3)].exprVal)->index,(yyvsp[(3) - (3)].exprVal), -1, yylineno);
            (yyval.exprVal) = emit_iftableitem ((yyvsp[(1) - (3)].exprVal),yylineno);
            (yyval.exprVal)->type = assignexpr_e;
        }else {
            emit(assign,(yyvsp[(3) - (3)].exprVal),NULL,(yyvsp[(1) - (3)].exprVal), -1, yylineno);
            (yyval.exprVal)= newexpr(assignexpr_e);
            (yyval.exprVal)->sym = newtemp(scope , yylineno , currscopespace(), currscopeoffset());
            emit(assign, (yyvsp[(1) - (3)].exprVal), NULL,(yyval.exprVal), -1, yylineno);
        }

    }
    break;

  case 40:

/* Line 1806 of yacc.c  */
#line 318 "parser.y"
    {printf("lvalue\n");
            (yyval.exprVal)  = emit_iftableitem((yyvsp[(1) - (1)].exprVal) , yylineno);
        }
    break;

  case 41:

/* Line 1806 of yacc.c  */
#line 321 "parser.y"
    {printf("call\n");
            (yyval.exprVal) = (yyvsp[(1) - (1)].exprVal);
        }
    break;

  case 42:

/* Line 1806 of yacc.c  */
#line 324 "parser.y"
    {printf("tablemake\n");
            (yyval.exprVal) = (yyvsp[(1) - (1)].exprVal);
        }
    break;

  case 43:

/* Line 1806 of yacc.c  */
#line 327 "parser.y"
    {printf("(funcdef)\n");
            (yyval.exprVal) = newexpr(programfunc_e);
            (yyval.exprVal)->sym = (yyvsp[(2) - (3)].symVal);
        }
    break;

  case 44:

/* Line 1806 of yacc.c  */
#line 331 "parser.y"
    {printf("const\n");
            (yyval.exprVal) = (yyvsp[(1) - (1)].exprVal);
        }
    break;

  case 45:

/* Line 1806 of yacc.c  */
#line 337 "parser.y"
    {
            SymbolTableEntry * Sym= Manage_lvalue_IDENTIFIER(scope, yytext, yylineno,currscopespace(),currscopeoffset());
            (yyval.exprVal)= lvalue_expr(Sym);
        }
    break;

  case 46:

/* Line 1806 of yacc.c  */
#line 341 "parser.y"
    {
            SymbolTableEntry * Sym =Manage_lvalue_LOCAL_IDENTIFIER(scope, yytext, yylineno,currscopespace(),currscopeoffset());
            (yyval.exprVal)= lvalue_expr(Sym);
        }
    break;

  case 47:

/* Line 1806 of yacc.c  */
#line 345 "parser.y"
    {
            SymbolTableEntry * Sym = Manage_lvalue_global_IDENTIFIER( yytext, yylineno);
            (yyval.exprVal)= lvalue_expr(Sym);
        }
    break;

  case 48:

/* Line 1806 of yacc.c  */
#line 349 "parser.y"
    {
            (yyval.exprVal) = (yyvsp[(1) - (1)].exprVal);
            printf("member\n");
        }
    break;

  case 49:

/* Line 1806 of yacc.c  */
#line 356 "parser.y"
    {
            printf("lvalue.id \n");
            (yyval.exprVal) = member_item((yyvsp[(1) - (3)].exprVal), (yyvsp[(3) - (3)].strVal), yylineno);
        }
    break;

  case 50:

/* Line 1806 of yacc.c  */
#line 360 "parser.y"
    {
        printf("lvalue[expression]\n");
        (yyvsp[(1) - (4)].exprVal)  = emit_iftableitem((yyvsp[(1) - (4)].exprVal) , yylineno);
        (yyval.exprVal)  = newexpr(tableitem_e);
        (yyval.exprVal)->sym = (yyvsp[(1) - (4)].exprVal)->sym ;
        (yyval.exprVal)->index = (yyvsp[(3) - (4)].exprVal);
    }
    break;

  case 51:

/* Line 1806 of yacc.c  */
#line 369 "parser.y"
    { (yyval.exprVal) = (yyvsp[(1) - (1)].exprVal);}
    break;

  case 52:

/* Line 1806 of yacc.c  */
#line 370 "parser.y"
    {
            printf("call.id \n");
            (yyval.exprVal) = member_item((yyvsp[(1) - (3)].exprVal), (yyvsp[(3) - (3)].strVal), yylineno);

        }
    break;

  case 53:

/* Line 1806 of yacc.c  */
#line 375 "parser.y"
    {
            printf("call[expression]\n");
            (yyvsp[(1) - (4)].exprVal)  = emit_iftableitem((yyvsp[(1) - (4)].exprVal) , yylineno);
            (yyval.exprVal)  = newexpr(tableitem_e);
            (yyval.exprVal)->sym = (yyvsp[(1) - (4)].exprVal)->sym ;
            (yyval.exprVal)->index = (yyvsp[(3) - (4)].exprVal);
        }
    break;

  case 54:

/* Line 1806 of yacc.c  */
#line 385 "parser.y"
    {printf("call (elist)\n");
            (yyval.exprVal)= make_call((yyvsp[(1) - (4)].exprVal) , (yyvsp[(3) - (4)].elistVal)->start , yylineno);
        }
    break;

  case 55:

/* Line 1806 of yacc.c  */
#line 388 "parser.y"
    {
            printf("lvalue callsuffix\n");
		    expr * self;
            if((yyvsp[(2) - (2)].methVal)->method){
                self = (yyvsp[(1) - (2)].exprVal);
                (yyvsp[(1) - (2)].exprVal) = emit_iftableitem(member_item(self , (yyvsp[(2) - (2)].methVal)->name , yylineno),yylineno);
                self->next = (yyvsp[(2) - (2)].methVal)->elist;
                (yyvsp[(2) - (2)].methVal)->elist = self;
            }

            (yyval.exprVal) = make_call((yyvsp[(1) - (2)].exprVal) , (yyvsp[(2) - (2)].methVal)->elist , yylineno);
        }
    break;

  case 56:

/* Line 1806 of yacc.c  */
#line 400 "parser.y"
    {
            printf("(funcdef) (elist)\n");
            expr * func ;
            func = newexpr(programfunc_e);
            func->sym = (yyvsp[(2) - (6)].symVal) ;
            (yyval.exprVal) = make_call(func , (yyvsp[(5) - (6)].elistVal)->start , yylineno);
        }
    break;

  case 57:

/* Line 1806 of yacc.c  */
#line 409 "parser.y"
    {
            printf("normacall\n");
            (yyval.methVal) = (yyvsp[(1) - (1)].methVal);
        }
    break;

  case 58:

/* Line 1806 of yacc.c  */
#line 413 "parser.y"
    {
            printf("methodcall\n");
            (yyval.methVal) = (yyvsp[(1) - (1)].methVal);
        }
    break;

  case 59:

/* Line 1806 of yacc.c  */
#line 419 "parser.y"
    {
        printf("(elist)\n");

        (yyval.methVal) = (struct methodcall_t *)malloc(sizeof(struct methodcall_t));
        (yyval.methVal)->elist = (yyvsp[(2) - (3)].elistVal)->start;
        (yyval.methVal)->method = false;
        (yyval.methVal)->name = NULL;
    }
    break;

  case 60:

/* Line 1806 of yacc.c  */
#line 429 "parser.y"
    {
        printf("..id  (elist)\n");

        (yyval.methVal) = (struct methodcall_t *)malloc(sizeof(struct methodcall_t));
        (yyval.methVal)->elist = (yyvsp[(4) - (5)].elistVal)->start;
        (yyval.methVal)->method = true;
        (yyval.methVal)->name = (yyvsp[(2) - (5)].strVal);
    }
    break;

  case 61:

/* Line 1806 of yacc.c  */
#line 439 "parser.y"
    {
            printf("elist : NULL\n");

            (yyval.elistVal) = (struct listOfExpression *)malloc(sizeof(struct listOfExpression));
            (yyval.elistVal)->start = NULL;
            (yyval.elistVal)->end = NULL ;

        }
    break;

  case 62:

/* Line 1806 of yacc.c  */
#line 447 "parser.y"
    {
            printf("elist : expression e_list\n");

            (yyvsp[(1) - (2)].exprVal)->next = (yyvsp[(2) - (2)].elistVal)->start;
            (yyvsp[(2) - (2)].elistVal)->start = (yyvsp[(1) - (2)].exprVal);
            (yyval.elistVal) = (yyvsp[(2) - (2)].elistVal);
        }
    break;

  case 63:

/* Line 1806 of yacc.c  */
#line 457 "parser.y"
    {
        printf("e_list : NULL\n");

	    (yyval.elistVal) = (struct listOfExpression *)malloc(sizeof(struct listOfExpression));
        (yyval.elistVal)->start = NULL ;
	    (yyval.elistVal)->end = NULL ;
	}
    break;

  case 64:

/* Line 1806 of yacc.c  */
#line 464 "parser.y"
    {
        printf("e_list : COMMA expression e_list\n");

		(yyvsp[(2) - (3)].exprVal)->next = (yyvsp[(3) - (3)].elistVal)->start;
		(yyvsp[(3) - (3)].elistVal)->start = (yyvsp[(2) - (3)].exprVal);
        (yyval.elistVal) = (yyvsp[(3) - (3)].elistVal);

        }
    break;

  case 65:

/* Line 1806 of yacc.c  */
#line 476 "parser.y"
    {
            int i = 0 ;
            printf("tablemake [elist]\n");

            expr * t = newexpr(newtable_e);
            t->sym = newtemp(currScope(),yylineno , currscopespace(), currscopeoffset());
            emit(tablecreate , NULL , NULL , t , -1 , yylineno);

            while((yyvsp[(2) - (3)].elistVal)->start){
                emit(tablesetelem , t,newexpr_consnum(i++), (yyvsp[(2) - (3)].elistVal)->start ,-1 , yylineno);
                (yyvsp[(2) - (3)].elistVal)->start = (yyvsp[(2) - (3)].elistVal)->start->next ;
            }
            (yyval.exprVal) = t;
        }
    break;

  case 66:

/* Line 1806 of yacc.c  */
#line 490 "parser.y"
    {
            int counter , i ;

            printf("tablemake [indexed]\n");

            expr * t = newexpr(newtable_e) , *tmp;
            t->sym = newtemp(currScope(),yylineno , currscopespace(), currscopeoffset());
            emit(tablecreate , NULL , NULL , t , -1 , yylineno);

            tmp = (yyvsp[(2) - (3)].exprVal);
            counter=0;

            while(tmp){
                counter++;
                tmp = tmp->next;
            }

            while(counter){
                tmp = (yyvsp[(2) - (3)].exprVal);

                for(i=0 ; i<counter-1; i++){
                    tmp = tmp->next;
                }
                emit(tablesetelem , t ,tmp , tmp->index , -1 , yylineno);
                counter--;
            }

            (yyval.exprVal) = t;
        }
    break;

  case 67:

/* Line 1806 of yacc.c  */
#line 521 "parser.y"
    {
            printf("indexed  : indexdelem\n");

            (yyval.exprVal)=(yyvsp[(1) - (1)].exprVal);

        }
    break;

  case 68:

/* Line 1806 of yacc.c  */
#line 527 "parser.y"
    {
            printf("indexed, indexdelem\n");

            (yyvsp[(3) - (3)].exprVal)->next= (yyvsp[(1) - (3)].exprVal);
            (yyval.exprVal) = (yyvsp[(3) - (3)].exprVal);

        }
    break;

  case 69:

/* Line 1806 of yacc.c  */
#line 537 "parser.y"
    {
            printf("{expression:expression}\n");
            (yyvsp[(2) - (5)].exprVal)->index = (yyvsp[(4) - (5)].exprVal);
            (yyval.exprVal)=(yyvsp[(2) - (5)].exprVal);
        }
    break;

  case 70:

/* Line 1806 of yacc.c  */
#line 544 "parser.y"
    {scope++;}
    break;

  case 71:

/* Line 1806 of yacc.c  */
#line 544 "parser.y"
    {
        printf("{stmt*}\n");
        SymbolTableHide(scope--);}
    break;

  case 72:

/* Line 1806 of yacc.c  */
#line 549 "parser.y"
    {;}
    break;

  case 74:

/* Line 1806 of yacc.c  */
#line 554 "parser.y"
    {
		char* function_name;
    	function_name=(char*)malloc(14);
        function_name[0]='\0';
        strncpy(function_name , "$_ANONYMOUS_$" ,13 );
    	function_name[13]='\0';
		(yyval.strVal) = function_name;

		printf("funcname ANONYMOUS\n");

    }
    break;

  case 75:

/* Line 1806 of yacc.c  */
#line 565 "parser.y"
    {
		char* function_name;
        function_name=(char*)malloc(strlen(yytext)+1);
        function_name[0]='\0';
        strcpy(function_name , yytext);
		(yyval.strVal) = function_name;

		printf("funcname %s\n",yytext);

    }
    break;

  case 76:

/* Line 1806 of yacc.c  */
#line 578 "parser.y"
    {
    (yyval.symVal) = Manage_funcprefix_FUNCTION_funcname((yyvsp[(2) - (2)].strVal), yylineno , currScope());
    LastFunctionInsertedToSymTable = (yyval.symVal);
    (yyval.symVal)->value.funcVal->iaddress = nextquadlabel();

    emit(funcstart , NULL , NULL , lvalue_expr((yyval.symVal)), -1 , yylineno);

    push(scopeoffsetstack, currscopeoffset() ,NULL);
    push( inloopStack , inloop , NULL);
    inloop = 0 ;


    enterscopespace();
    resetformalargoffset();

    printf("funcprefix\n");

    }
    break;

  case 77:

/* Line 1806 of yacc.c  */
#line 598 "parser.y"
    { infunction++; scope++;}
    break;

  case 78:

/* Line 1806 of yacc.c  */
#line 598 "parser.y"
    {
    Manage_funcargs_LP_idlist_RP(LastFunctionInsertedToSymTable);
    LastFunctionInsertedToSymTable = NULL;

    scope--;
    enterscopespace();
    resetfunctionlocaloffset();

    printf("funcargs\n");

	}
    break;

  case 79:

/* Line 1806 of yacc.c  */
#line 612 "parser.y"
    {
    infunction--;
    (yyval.unsignedVal) = currscopeoffset();
    exitscopespace();
    printf("funcbody: block\n");

    }
    break;

  case 80:

/* Line 1806 of yacc.c  */
#line 622 "parser.y"
    {
    int oldOffset;

    exitscopespace();
    (yyvsp[(1) - (3)].symVal)->value.funcVal->totallocals = (yyvsp[(3) - (3)].unsignedVal);

    oldOffset = pop(scopeoffsetstack);
    inloop = pop(inloopStack);

    restorecurrscopeoffset(oldOffset);

    (yyval.symVal) = (yyvsp[(1) - (3)].symVal);
    emit(funcend , NULL , NULL , lvalue_expr((yyvsp[(1) - (3)].symVal)), -1 , yylineno);

    printf("funcdef\n");

	}
    break;

  case 81:

/* Line 1806 of yacc.c  */
#line 642 "parser.y"
    {
            printf("number %d\n",atoi(yytext));
            (yyval.exprVal) = newexpr_consnum((yyvsp[(1) - (1)].intVal));
        }
    break;

  case 82:

/* Line 1806 of yacc.c  */
#line 646 "parser.y"
    {
            printf("string \n");
            (yyval.exprVal) = newexpr_conststring((yyvsp[(1) - (1)].strVal));
            printf("string = %s \n",(yyvsp[(1) - (1)].strVal));
        }
    break;

  case 83:

/* Line 1806 of yacc.c  */
#line 651 "parser.y"
    {
            printf("nil\n");
            (yyval.exprVal) = newexpr(nil_e);
        }
    break;

  case 84:

/* Line 1806 of yacc.c  */
#line 655 "parser.y"
    {
            printf("true\n");
            (yyval.exprVal) = newexpr_constbool(true);
        }
    break;

  case 85:

/* Line 1806 of yacc.c  */
#line 659 "parser.y"
    {
            printf("false\n");
            (yyval.exprVal) = newexpr_constbool(false);
        }
    break;

  case 86:

/* Line 1806 of yacc.c  */
#line 663 "parser.y"
    {
            printf("number %f\n",atof(yytext));
            (yyval.exprVal) = newexpr_consnum((yyvsp[(1) - (1)].floatVal));
        }
    break;

  case 87:

/* Line 1806 of yacc.c  */
#line 669 "parser.y"
    {;}
    break;

  case 88:

/* Line 1806 of yacc.c  */
#line 670 "parser.y"
    {Manage_idlist_IDENTIFIER(yytext , yylineno , scope);}
    break;

  case 89:

/* Line 1806 of yacc.c  */
#line 671 "parser.y"
    {Manage_idlist_IDENTIFIER(yytext , yylineno , scope); printf("id , id \n");}
    break;

  case 90:

/* Line 1806 of yacc.c  */
#line 676 "parser.y"
    {
        emit(if_eq , (yyvsp[(3) - (4)].exprVal) , newexpr_constbool(true), NULL ,nextquadlabel()+2, yylineno);
        (yyval.unsignedVal) = nextquadlabel();
        emit(jump , NULL , NULL , NULL , -2 , yylineno);
    }
    break;

  case 91:

/* Line 1806 of yacc.c  */
#line 683 "parser.y"
    {
        (yyval.unsignedVal) = nextquadlabel();
        emit(jump , NULL , NULL , NULL , -2 , yylineno);
    }
    break;

  case 92:

/* Line 1806 of yacc.c  */
#line 688 "parser.y"
    {
        printf("if(expression) stmt else stmt\n");
        patchlabel((yyvsp[(1) - (4)].unsignedVal) , (yyvsp[(3) - (4)].unsignedVal)+1);
        patchlabel((yyvsp[(3) - (4)].unsignedVal) , nextquadlabel());
    }
    break;

  case 93:

/* Line 1806 of yacc.c  */
#line 693 "parser.y"
    {
        printf("if(expression) stmt\n");
        patchlabel((yyvsp[(1) - (2)].unsignedVal) , nextquadlabel());
    }
    break;

  case 94:

/* Line 1806 of yacc.c  */
#line 700 "parser.y"
    { ++inloop ;}
    break;

  case 95:

/* Line 1806 of yacc.c  */
#line 702 "parser.y"
    { --inloop ;}
    break;

  case 96:

/* Line 1806 of yacc.c  */
#line 704 "parser.y"
    {
        printf("loopstmt\n");
    }
    break;

  case 97:

/* Line 1806 of yacc.c  */
#line 709 "parser.y"
    {

        push(breakContstack , 0 , breakContList);
        breakContList = NULL ;

        breakContList = (struct stmtNode *)malloc(sizeof(struct stmtNode ));
        breakContList->breaklist = NULL ;
        breakContList->continuelist = NULL ;
        (yyval.unsignedVal) = nextquadlabel();
        printf("whilestart\n");
    }
    break;

  case 98:

/* Line 1806 of yacc.c  */
#line 722 "parser.y"
    {
        emit(if_eq , (yyvsp[(2) - (3)].exprVal) , newexpr_constbool(true), NULL , nextquadlabel()+2 , yylineno);
        (yyval.unsignedVal) = nextquadlabel();
        emit(jump , NULL , NULL , NULL , -2 , yylineno);
        printf("whilecond\n");
    }
    break;

  case 99:

/* Line 1806 of yacc.c  */
#line 730 "parser.y"
    {
        printf("while(expression) stmt \n");
        emit(jump , NULL , NULL , NULL , (yyvsp[(1) - (3)].unsignedVal) , yylineno);
        patchlabel((yyvsp[(2) - (3)].unsignedVal) ,nextquadlabel());

        while(breakContList->breaklist){
            patchlabel( breakContList->breaklist->quadPossition , nextquadlabel());
            breakContList->breaklist = breakContList->breaklist->next;
        }

        while(breakContList->continuelist){
            patchlabel(breakContList->continuelist->quadPossition , (yyvsp[(1) - (3)].unsignedVal));
            breakContList->continuelist = breakContList->continuelist->next;
        }

        breakContList = NULL;
        breakContList = (struct stmtNode *) pop_list(breakContstack);
    }
    break;

  case 100:

/* Line 1806 of yacc.c  */
#line 750 "parser.y"
    {
        (yyval.unsignedVal) = nextquadlabel();
        emit(jump , NULL , NULL , NULL , -2 , yylineno);
    }
    break;

  case 101:

/* Line 1806 of yacc.c  */
#line 755 "parser.y"
    {
        (yyval.unsignedVal) = nextquadlabel();
    }
    break;

  case 102:

/* Line 1806 of yacc.c  */
#line 760 "parser.y"
    {

        push(breakContstack , 0 , breakContList);
        breakContList = NULL ;

        breakContList = (struct stmtNode *)malloc(sizeof(struct stmtNode ));
        breakContList->breaklist = NULL ;
        breakContList->continuelist = NULL ;

        (yyval.forprefixNodeVal) = (struct forprefixNode *)malloc(sizeof(struct forprefixNode));
        (yyval.forprefixNodeVal)->test = (yyvsp[(5) - (7)].unsignedVal);
        (yyval.forprefixNodeVal)->enter = nextquadlabel();
        emit(if_eq , (yyvsp[(6) - (7)].exprVal) , newexpr_constbool(true) , NULL , -2 , yylineno);
    }
    break;

  case 103:

/* Line 1806 of yacc.c  */
#line 776 "parser.y"
    {
        printf("for(elist; expression; elist) stmt\n");
        patchlabel((yyvsp[(1) - (7)].forprefixNodeVal)->enter , (yyvsp[(5) - (7)].unsignedVal)+1);
        patchlabel((yyvsp[(2) - (7)].unsignedVal) , nextquadlabel());
        patchlabel((yyvsp[(5) - (7)].unsignedVal) , (yyvsp[(1) - (7)].forprefixNodeVal)->test);
        patchlabel((yyvsp[(7) - (7)].unsignedVal) , (yyvsp[(2) - (7)].unsignedVal)+1);


        while(breakContList->breaklist){
            patchlabel( breakContList->breaklist->quadPossition , nextquadlabel());
            breakContList->breaklist = breakContList->breaklist->next;
        }

        while(breakContList->continuelist){
            patchlabel(breakContList->continuelist->quadPossition , (yyvsp[(2) - (7)].unsignedVal)+1);
            breakContList->continuelist = breakContList->continuelist->next;
        }

        breakContList = NULL;
        breakContList = (struct stmtNode *) pop_list(breakContstack);
    }
    break;

  case 104:

/* Line 1806 of yacc.c  */
#line 799 "parser.y"
    { printf("return expression;\n");
            emit(ret , NULL , NULL , (yyvsp[(2) - (3)].exprVal) , -1 , yylineno);
        }
    break;

  case 105:

/* Line 1806 of yacc.c  */
#line 802 "parser.y"
    { printf("return;\n");
            emit(ret , NULL , NULL , NULL , -1 , yylineno);
        }
    break;



/* Line 1806 of yacc.c  */
#line 3004 "parser.c"
      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = (char *) YYSTACK_ALLOC (yymsg_alloc);
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
	 error, discard it.  */

      if (yychar <= YYEOF)
	{
	  /* Return failure if at end of input.  */
	  if (yychar == YYEOF)
	    YYABORT;
	}
      else
	{
	  yydestruct ("Error: discarding",
		      yytoken, &yylval);
	  yychar = YYEMPTY;
	}
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule which action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
	{
	  yyn += YYTERROR;
	  if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
	    {
	      yyn = yytable[yyn];
	      if (0 < yyn)
		break;
	    }
	}

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
	YYABORT;


      yydestruct ("Error: popping",
		  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  *++yyvsp = yylval;


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#if !defined(yyoverflow) || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule which action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
		  yystos[*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  /* Make sure YYID is used.  */
  return YYID (yyresult);
}



/* Line 2067 of yacc.c  */
#line 807 "parser.y"


int currScope(){
    return scope;
}


void yyerror(const char* yaccProvidedMessage){
    fprintf(stderr, "\nline %d :%s\n",yylineno, yaccProvidedMessage);
}

int main(int argc, char **argv){
	if(argc <2 ){
		fprintf(stderr,"\n No  input file to parse!!!\n");
		return 1;
	}

	if( !(yyin = fopen( argv[1], "r" )) ){
		fprintf(stderr,"\nCannot open file : %s\n",argv[1]);
		return 1;
	}

	scopeoffsetstack =  MakeEmptyStack();
	breakContstack = MakeEmptyStack() ;
    inloopStack = MakeEmptyStack();


    SymbolTable_scopeList_Initialize();

    yyparse();

    SymbolTablePrintAll();
    ScopePrintAll();

	PrintQuads();
	//PrintIcode();

    stackInitialize();
    generateTargetCode();

    PrintIcode();
    PrintBinaryCode();
    PrintTargetCode();

    fclose(yyin);

	return 0 ;

}

