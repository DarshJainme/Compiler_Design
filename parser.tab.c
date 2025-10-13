/* A Bison parser, made by GNU Bison 2.3.  */

/* Skeleton implementation for Bison's Yacc-like parsers in C

   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.  */

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
#define YYBISON_VERSION "2.3"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Using locations.  */
#define YYLSP_NEEDED 0



/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     IDENTIFIER = 258,
     CONSTANT = 259,
     STRING_LITERAL = 260,
     CHAR_LITERAL = 261,
     BOOL_TRUE = 262,
     BOOL_FALSE = 263,
     TYPE_NAME = 264,
     SIZEOF = 265,
     PTR_OP = 266,
     INC_OP = 267,
     DEC_OP = 268,
     LSHIFT_OP = 269,
     RSHIFT_OP = 270,
     LE_OP = 271,
     GE_OP = 272,
     EQ_OP = 273,
     NE_OP = 274,
     LAND_OP = 275,
     LOR_OP = 276,
     MUL_ASSIGN = 277,
     DIV_ASSIGN = 278,
     MOD_ASSIGN = 279,
     ADD_ASSIGN = 280,
     SUB_ASSIGN = 281,
     LSHIFT_ASSIGN = 282,
     RSHIFT_ASSIGN = 283,
     AND_ASSIGN = 284,
     XOR_ASSIGN = 285,
     OR_ASSIGN = 286,
     TYPEDEF = 287,
     EXTERN = 288,
     STATIC = 289,
     AUTO = 290,
     REGISTER = 291,
     STRING = 292,
     CHAR = 293,
     SHORT = 294,
     INT = 295,
     LONG = 296,
     SIGNED = 297,
     UNSIGNED = 298,
     FLOAT = 299,
     DOUBLE = 300,
     CONST = 301,
     VOLATILE = 302,
     VOID = 303,
     BOOL = 304,
     STRUCT = 305,
     UNION = 306,
     ENUM = 307,
     ELLIPSIS = 308,
     CASE = 309,
     DEFAULT = 310,
     IF = 311,
     ELSE = 312,
     SWITCH = 313,
     WHILE = 314,
     DO = 315,
     FOR = 316,
     GOTO = 317,
     CONTINUE = 318,
     BREAK = 319,
     RETURN = 320,
     CLASS = 321,
     DELETE = 322,
     FRIEND = 323,
     NAMESPACE = 324,
     NEW = 325,
     OVERRIDE = 326,
     PRIVATE = 327,
     PROTECTED = 328,
     PUBLIC = 329,
     THROW = 330,
     TRY = 331,
     USING = 332,
     VIRTUAL = 333,
     SCOPE_OP = 334,
     CATCH = 335,
     FINAL = 336,
     UNTIL = 337,
     FILE_TYPE = 338
   };
#endif
/* Tokens.  */
#define IDENTIFIER 258
#define CONSTANT 259
#define STRING_LITERAL 260
#define CHAR_LITERAL 261
#define BOOL_TRUE 262
#define BOOL_FALSE 263
#define TYPE_NAME 264
#define SIZEOF 265
#define PTR_OP 266
#define INC_OP 267
#define DEC_OP 268
#define LSHIFT_OP 269
#define RSHIFT_OP 270
#define LE_OP 271
#define GE_OP 272
#define EQ_OP 273
#define NE_OP 274
#define LAND_OP 275
#define LOR_OP 276
#define MUL_ASSIGN 277
#define DIV_ASSIGN 278
#define MOD_ASSIGN 279
#define ADD_ASSIGN 280
#define SUB_ASSIGN 281
#define LSHIFT_ASSIGN 282
#define RSHIFT_ASSIGN 283
#define AND_ASSIGN 284
#define XOR_ASSIGN 285
#define OR_ASSIGN 286
#define TYPEDEF 287
#define EXTERN 288
#define STATIC 289
#define AUTO 290
#define REGISTER 291
#define STRING 292
#define CHAR 293
#define SHORT 294
#define INT 295
#define LONG 296
#define SIGNED 297
#define UNSIGNED 298
#define FLOAT 299
#define DOUBLE 300
#define CONST 301
#define VOLATILE 302
#define VOID 303
#define BOOL 304
#define STRUCT 305
#define UNION 306
#define ENUM 307
#define ELLIPSIS 308
#define CASE 309
#define DEFAULT 310
#define IF 311
#define ELSE 312
#define SWITCH 313
#define WHILE 314
#define DO 315
#define FOR 316
#define GOTO 317
#define CONTINUE 318
#define BREAK 319
#define RETURN 320
#define CLASS 321
#define DELETE 322
#define FRIEND 323
#define NAMESPACE 324
#define NEW 325
#define OVERRIDE 326
#define PRIVATE 327
#define PROTECTED 328
#define PUBLIC 329
#define THROW 330
#define TRY 331
#define USING 332
#define VIRTUAL 333
#define SCOPE_OP 334
#define CATCH 335
#define FINAL 336
#define UNTIL 337
#define FILE_TYPE 338




/* Copy the first part of user declarations.  */
#line 1 "parser.y"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "ast.h"

ASTNode *root = NULL;
int error_flag = 1;

extern int yylineno;
extern int yylex();
extern char *yytext;
extern FILE *yyin;
void yyerror(const char *s);

#define MAX_TYPENAMES 100
static char* type_names[MAX_TYPENAMES];
static int type_name_count = 0;

void add_typename(const char* name) {
    if (!name) return;
    for (int i = 0; i < type_name_count; ++i) {
        if (strcmp(type_names[i], name) == 0) return;
    }
    if (type_name_count < MAX_TYPENAMES) {
        type_names[type_name_count++] = strdup(name);
    }
}

int is_typename(const char* name) {
    if (!name) return 0;
    for (int i = 0; i < type_name_count; ++i) {
        if (strcmp(type_names[i], name) == 0) return 1;
    }
    return 0;
}



/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* Enabling the token table.  */
#ifndef YYTOKEN_TABLE
# define YYTOKEN_TABLE 0
#endif

#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
#line 40 "parser.y"
{
    char *str;
    struct ASTNode *node;
    struct ASTNodeList *list;
    int token;
}
/* Line 193 of yacc.c.  */
#line 308 "parser.tab.c"
	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif



/* Copy the second part of user declarations.  */


/* Line 216 of yacc.c.  */
#line 321 "parser.tab.c"

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
YYID (int i)
#else
static int
YYID (i)
    int i;
#endif
{
  return i;
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
#    if ! defined _ALLOCA_H && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#     ifndef _STDLIB_H
#      define _STDLIB_H 1
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
#  if (defined __cplusplus && ! defined _STDLIB_H \
       && ! ((defined YYMALLOC || defined malloc) \
	     && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef _STDLIB_H
#    define _STDLIB_H 1
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
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
  yytype_int16 yyss;
  YYSTYPE yyvs;
  };

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

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

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack)					\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack, Stack, yysize);				\
	Stack = &yyptr->Stack;						\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (YYID (0))

#endif

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  109
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   2352

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  108
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  73
/* YYNRULES -- Number of rules.  */
#define YYNRULES  235
/* YYNRULES -- Number of states.  */
#define YYNSTATES  400

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   338

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    97,     2,     2,     2,    96,    89,     2,
     102,   103,    94,    92,   105,    93,    99,    95,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    86,   104,
      90,    84,    91,    85,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   100,     2,   101,    88,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   106,    87,   107,    98,     2,     2,     2,
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
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     5,     7,    10,    12,    14,    16,    18,
      22,    24,    26,    28,    30,    32,    34,    38,    40,    42,
      47,    51,    56,    60,    64,    67,    70,    72,    76,    78,
      81,    84,    87,    90,    93,    96,    99,   102,   105,   110,
     112,   114,   116,   121,   123,   127,   131,   135,   137,   141,
     145,   147,   151,   155,   157,   161,   165,   169,   173,   175,
     179,   183,   185,   189,   191,   195,   197,   201,   203,   207,
     209,   213,   215,   221,   223,   227,   229,   231,   233,   235,
     237,   239,   241,   243,   245,   247,   249,   251,   255,   257,
     262,   265,   269,   272,   279,   285,   287,   289,   291,   293,
     295,   297,   301,   306,   310,   313,   317,   319,   322,   324,
     326,   328,   331,   337,   345,   351,   357,   365,   372,   380,
     387,   395,   399,   402,   405,   408,   412,   415,   419,   421,
     424,   426,   428,   430,   432,   436,   438,   442,   444,   446,
     448,   450,   452,   454,   456,   458,   460,   462,   464,   466,
     468,   470,   472,   474,   476,   478,   480,   482,   484,   486,
     488,   494,   500,   505,   510,   513,   516,   518,   521,   525,
     527,   531,   533,   537,   542,   548,   551,   553,   557,   559,
     563,   570,   572,   574,   575,   578,   582,   585,   587,   589,
     591,   593,   594,   597,   599,   601,   604,   607,   609,   611,
     615,   620,   624,   629,   633,   636,   640,   641,   643,   645,
     648,   650,   654,   656,   660,   663,   666,   668,   671,   673,
     675,   678,   680,   684,   687,   691,   695,   700,   703,   707,
     711,   716,   718,   722,   727,   729
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int16 yyrhs[] =
{
     109,     0,    -1,   110,    -1,   111,    -1,   110,   111,    -1,
     112,    -1,   145,    -1,   141,    -1,   104,    -1,   146,   168,
     138,    -1,     3,    -1,     4,    -1,     5,    -1,     6,    -1,
       7,    -1,     8,    -1,   102,   131,   103,    -1,   135,    -1,
     113,    -1,   114,   100,   131,   101,    -1,   114,   102,   103,
      -1,   114,   102,   115,   103,    -1,   114,    99,     3,    -1,
     114,    11,     3,    -1,   114,    12,    -1,   114,    13,    -1,
     129,    -1,   115,   105,   129,    -1,   114,    -1,    12,   116,
      -1,    13,   116,    -1,    89,   117,    -1,    94,   117,    -1,
      92,   117,    -1,    93,   117,    -1,    98,   117,    -1,    97,
     117,    -1,    10,   116,    -1,    10,   102,   176,   103,    -1,
     133,    -1,   134,    -1,   116,    -1,   102,   176,   103,   117,
      -1,   117,    -1,   118,    94,   117,    -1,   118,    95,   117,
      -1,   118,    96,   117,    -1,   118,    -1,   119,    92,   118,
      -1,   119,    93,   118,    -1,   119,    -1,   120,    14,   119,
      -1,   120,    15,   119,    -1,   120,    -1,   121,    90,   120,
      -1,   121,    91,   120,    -1,   121,    16,   120,    -1,   121,
      17,   120,    -1,   121,    -1,   122,    18,   121,    -1,   122,
      19,   121,    -1,   122,    -1,   123,    89,   122,    -1,   123,
      -1,   124,    88,   123,    -1,   124,    -1,   125,    87,   124,
      -1,   125,    -1,   126,    20,   125,    -1,   126,    -1,   127,
      21,   126,    -1,   127,    -1,   127,    85,   131,    86,   128,
      -1,   128,    -1,   116,   130,   129,    -1,    84,    -1,    22,
      -1,    23,    -1,    24,    -1,    25,    -1,    26,    -1,    27,
      -1,    28,    -1,    29,    -1,    30,    -1,    31,    -1,   129,
      -1,   131,   105,   129,    -1,   128,    -1,    70,   176,   102,
     103,    -1,    70,   176,    -1,    70,   176,   179,    -1,    67,
     117,    -1,   100,   101,   102,   173,   103,   138,    -1,   100,
     101,   102,   103,   138,    -1,   137,    -1,   138,    -1,   141,
      -1,   142,    -1,   143,    -1,   144,    -1,     3,    86,   136,
      -1,    54,   132,    86,   136,    -1,    55,    86,   136,    -1,
     106,   107,    -1,   106,   139,   107,    -1,   140,    -1,   139,
     140,    -1,   145,    -1,   136,    -1,   104,    -1,   131,   104,
      -1,    56,   102,   131,   103,   136,    -1,    56,   102,   131,
     103,   136,    57,   136,    -1,    58,   102,   131,   103,   136,
      -1,    59,   102,   131,   103,   136,    -1,    60,   136,    59,
     102,   131,   103,   104,    -1,    61,   102,   141,   141,   103,
     136,    -1,    61,   102,   141,   141,   131,   103,   136,    -1,
      61,   102,   145,   141,   103,   136,    -1,    61,   102,   145,
     141,   131,   103,   136,    -1,    62,     3,   104,    -1,    63,
     104,    -1,    64,   104,    -1,    65,   104,    -1,    65,   131,
     104,    -1,   146,   104,    -1,   146,   148,   104,    -1,   147,
      -1,   146,   147,    -1,   150,    -1,   151,    -1,   152,    -1,
     149,    -1,   148,   105,   149,    -1,   168,    -1,   168,    84,
     179,    -1,    32,    -1,    33,    -1,    34,    -1,    35,    -1,
      36,    -1,    48,    -1,    38,    -1,    39,    -1,    40,    -1,
      41,    -1,    44,    -1,    45,    -1,    42,    -1,    43,    -1,
      49,    -1,    37,    -1,    83,    -1,   153,    -1,   158,    -1,
     161,    -1,     9,    -1,    46,    -1,    47,    -1,    50,     3,
     106,   154,   107,    -1,    51,     3,   106,   154,   107,    -1,
      50,   106,   154,   107,    -1,    51,   106,   154,   107,    -1,
      50,     9,    -1,    51,     9,    -1,   155,    -1,   154,   155,
      -1,   146,   156,   104,    -1,   157,    -1,   156,   105,   157,
      -1,   168,    -1,   168,    86,   132,    -1,    52,   106,   159,
     107,    -1,    52,     3,   106,   159,   107,    -1,    52,     9,
      -1,   160,    -1,   159,   105,   160,    -1,     3,    -1,     3,
      84,   132,    -1,    66,   162,   163,   106,   166,   107,    -1,
       3,    -1,     9,    -1,    -1,    86,   164,    -1,   163,   105,
     164,    -1,   165,   162,    -1,   162,    -1,    74,    -1,    72,
      -1,    73,    -1,    -1,   166,   167,    -1,   145,    -1,   112,
      -1,   165,    86,    -1,   170,   169,    -1,   169,    -1,     3,
      -1,   102,   168,   103,    -1,   169,   100,   132,   101,    -1,
     169,   100,   101,    -1,   169,   102,   173,   103,    -1,   169,
     102,   103,    -1,    94,   171,    -1,   170,    94,   171,    -1,
      -1,   172,    -1,   152,    -1,   172,   152,    -1,   174,    -1,
     174,   105,    53,    -1,   175,    -1,   174,   105,   175,    -1,
     146,   168,    -1,   146,   177,    -1,   146,    -1,   146,   177,
      -1,   146,    -1,   170,    -1,   170,   178,    -1,   178,    -1,
     102,   177,   103,    -1,   100,   101,    -1,   100,   132,   101,
      -1,   178,   100,   101,    -1,   178,   100,   132,   101,    -1,
     102,   103,    -1,   102,   173,   103,    -1,   178,   102,   103,
      -1,   178,   102,   173,   103,    -1,   129,    -1,   106,   180,
     107,    -1,   106,   180,   105,   107,    -1,   179,    -1,   180,
     105,   179,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   123,   123,   127,   128,   132,   133,   134,   135,   139,
     144,   145,   146,   147,   148,   149,   150,   151,   155,   156,
     157,   158,   159,   160,   161,   162,   166,   168,   173,   174,
     175,   176,   177,   178,   179,   180,   181,   182,   183,   184,
     185,   198,   199,   203,   204,   205,   206,   210,   211,   212,
     216,   217,   218,   222,   223,   224,   225,   226,   230,   231,
     232,   236,   237,   241,   242,   246,   247,   251,   252,   256,
     257,   261,   262,   266,   267,   271,   272,   273,   274,   275,
     276,   277,   278,   279,   280,   281,   285,   286,   290,   295,
     296,   297,   301,   305,   306,   311,   312,   313,   314,   315,
     316,   320,   321,   322,   326,   327,   331,   333,   338,   339,
     343,   344,   348,   349,   350,   354,   355,   356,   357,   358,
     359,   363,   364,   365,   366,   367,   372,   373,   377,   378,
     382,   383,   384,   388,   390,   395,   396,   400,   401,   402,
     403,   404,   408,   409,   410,   411,   412,   413,   414,   415,
     416,   417,   418,   419,   420,   421,   422,   423,   427,   428,
     432,   433,   434,   435,   436,   437,   441,   443,   448,   452,
     454,   459,   460,   464,   465,   466,   470,   472,   477,   478,
     482,   486,   487,   491,   492,   493,   497,   498,   502,   503,
     504,   508,   509,   513,   514,   515,   520,   522,   526,   527,
     528,   529,   530,   531,   535,   537,   542,   543,   547,   549,
     554,   555,   559,   561,   566,   568,   570,   575,   576,   580,
     581,   582,   586,   587,   588,   589,   590,   591,   592,   593,
     594,   598,   599,   600,   604,   606
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "IDENTIFIER", "CONSTANT",
  "STRING_LITERAL", "CHAR_LITERAL", "BOOL_TRUE", "BOOL_FALSE", "TYPE_NAME",
  "SIZEOF", "PTR_OP", "INC_OP", "DEC_OP", "LSHIFT_OP", "RSHIFT_OP",
  "LE_OP", "GE_OP", "EQ_OP", "NE_OP", "LAND_OP", "LOR_OP", "MUL_ASSIGN",
  "DIV_ASSIGN", "MOD_ASSIGN", "ADD_ASSIGN", "SUB_ASSIGN", "LSHIFT_ASSIGN",
  "RSHIFT_ASSIGN", "AND_ASSIGN", "XOR_ASSIGN", "OR_ASSIGN", "TYPEDEF",
  "EXTERN", "STATIC", "AUTO", "REGISTER", "STRING", "CHAR", "SHORT", "INT",
  "LONG", "SIGNED", "UNSIGNED", "FLOAT", "DOUBLE", "CONST", "VOLATILE",
  "VOID", "BOOL", "STRUCT", "UNION", "ENUM", "ELLIPSIS", "CASE", "DEFAULT",
  "IF", "ELSE", "SWITCH", "WHILE", "DO", "FOR", "GOTO", "CONTINUE",
  "BREAK", "RETURN", "CLASS", "DELETE", "FRIEND", "NAMESPACE", "NEW",
  "OVERRIDE", "PRIVATE", "PROTECTED", "PUBLIC", "THROW", "TRY", "USING",
  "VIRTUAL", "SCOPE_OP", "CATCH", "FINAL", "UNTIL", "FILE_TYPE", "'='",
  "'?'", "':'", "'|'", "'^'", "'&'", "'<'", "'>'", "'+'", "'-'", "'*'",
  "'/'", "'%'", "'!'", "'~'", "'.'", "'['", "']'", "'('", "')'", "';'",
  "','", "'{'", "'}'", "$accept", "program", "translation_unit",
  "external_declaration", "function_definition", "primary_expression",
  "postfix_expression", "argument_expression_list", "unary_expression",
  "cast_expression", "multiplicative_expression", "additive_expression",
  "shift_expression", "relational_expression", "equality_expression",
  "and_expression", "exclusive_or_expression", "inclusive_or_expression",
  "logical_and_expression", "logical_or_expression",
  "conditional_expression", "assignment_expression", "assignment_operator",
  "expression", "constant_expression", "new_expression",
  "delete_expression", "lambda_expression", "statement",
  "labeled_statement", "compound_statement", "block_item_list",
  "block_item", "expression_statement", "selection_statement",
  "iteration_statement", "jump_statement", "declaration",
  "declaration_specifiers", "declaration_specifier",
  "init_declarator_list", "init_declarator", "storage_class_specifier",
  "type_specifier", "type_qualifier", "struct_or_union_specifier",
  "struct_declaration_list", "struct_declaration",
  "struct_declarator_list", "struct_declarator", "enum_specifier",
  "enumerator_list", "enumerator", "class_specifier", "class_name",
  "base_clause", "base_specifier", "access_specifier", "class_member_list",
  "class_member", "declarator", "direct_declarator", "pointer",
  "type_qualifier_list_opt", "type_qualifier_list", "parameter_type_list",
  "parameter_list", "parameter_declaration", "type_name",
  "abstract_declarator", "direct_abstract_declarator", "initializer",
  "initializer_list", 0
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
     305,   306,   307,   308,   309,   310,   311,   312,   313,   314,
     315,   316,   317,   318,   319,   320,   321,   322,   323,   324,
     325,   326,   327,   328,   329,   330,   331,   332,   333,   334,
     335,   336,   337,   338,    61,    63,    58,   124,    94,    38,
      60,    62,    43,    45,    42,    47,    37,    33,   126,    46,
      91,    93,    40,    41,    59,    44,   123,   125
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,   108,   109,   110,   110,   111,   111,   111,   111,   112,
     113,   113,   113,   113,   113,   113,   113,   113,   114,   114,
     114,   114,   114,   114,   114,   114,   115,   115,   116,   116,
     116,   116,   116,   116,   116,   116,   116,   116,   116,   116,
     116,   117,   117,   118,   118,   118,   118,   119,   119,   119,
     120,   120,   120,   121,   121,   121,   121,   121,   122,   122,
     122,   123,   123,   124,   124,   125,   125,   126,   126,   127,
     127,   128,   128,   129,   129,   130,   130,   130,   130,   130,
     130,   130,   130,   130,   130,   130,   131,   131,   132,   133,
     133,   133,   134,   135,   135,   136,   136,   136,   136,   136,
     136,   137,   137,   137,   138,   138,   139,   139,   140,   140,
     141,   141,   142,   142,   142,   143,   143,   143,   143,   143,
     143,   144,   144,   144,   144,   144,   145,   145,   146,   146,
     147,   147,   147,   148,   148,   149,   149,   150,   150,   150,
     150,   150,   151,   151,   151,   151,   151,   151,   151,   151,
     151,   151,   151,   151,   151,   151,   151,   151,   152,   152,
     153,   153,   153,   153,   153,   153,   154,   154,   155,   156,
     156,   157,   157,   158,   158,   158,   159,   159,   160,   160,
     161,   162,   162,   163,   163,   163,   164,   164,   165,   165,
     165,   166,   166,   167,   167,   167,   168,   168,   169,   169,
     169,   169,   169,   169,   170,   170,   171,   171,   172,   172,
     173,   173,   174,   174,   175,   175,   175,   176,   176,   177,
     177,   177,   178,   178,   178,   178,   178,   178,   178,   178,
     178,   179,   179,   179,   180,   180
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     1,     2,     1,     1,     1,     1,     3,
       1,     1,     1,     1,     1,     1,     3,     1,     1,     4,
       3,     4,     3,     3,     2,     2,     1,     3,     1,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     4,     1,
       1,     1,     4,     1,     3,     3,     3,     1,     3,     3,
       1,     3,     3,     1,     3,     3,     3,     3,     1,     3,
       3,     1,     3,     1,     3,     1,     3,     1,     3,     1,
       3,     1,     5,     1,     3,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     3,     1,     4,
       2,     3,     2,     6,     5,     1,     1,     1,     1,     1,
       1,     3,     4,     3,     2,     3,     1,     2,     1,     1,
       1,     2,     5,     7,     5,     5,     7,     6,     7,     6,
       7,     3,     2,     2,     2,     3,     2,     3,     1,     2,
       1,     1,     1,     1,     3,     1,     3,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       5,     5,     4,     4,     2,     2,     1,     2,     3,     1,
       3,     1,     3,     4,     5,     2,     1,     3,     1,     3,
       6,     1,     1,     0,     2,     3,     2,     1,     1,     1,
       1,     0,     2,     1,     1,     2,     2,     1,     1,     3,
       4,     3,     4,     3,     2,     3,     0,     1,     1,     2,
       1,     3,     1,     3,     2,     2,     1,     2,     1,     1,
       2,     1,     3,     2,     3,     3,     4,     2,     3,     3,
       4,     1,     3,     4,     1,     3
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,    10,    11,    12,    13,    14,    15,   157,     0,     0,
       0,   137,   138,   139,   140,   141,   152,   143,   144,   145,
     146,   149,   150,   147,   148,   158,   159,   142,   151,     0,
       0,     0,     0,     0,     0,   153,     0,     0,     0,     0,
       0,     0,     0,     0,     8,     0,     2,     3,     5,    18,
      28,    41,    43,    47,    50,    53,    58,    61,    63,    65,
      67,    69,    71,    73,    86,     0,    39,    40,    17,     7,
       6,     0,   128,   130,   131,   132,   154,   155,   156,     0,
      37,     0,    29,    30,     0,   164,     0,     0,   165,     0,
       0,   175,     0,   181,   182,   183,    41,    92,   218,    90,
      31,    33,    34,    32,    36,    35,     0,     0,     0,     1,
       4,     0,    24,    25,     0,     0,     0,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    75,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   111,     0,
     198,   206,     0,   126,   129,     0,   133,   135,   197,     0,
       0,     0,     0,     0,   166,     0,     0,     0,   178,     0,
     176,     0,     0,     0,     0,   219,   217,   221,     0,     0,
     231,    91,     0,    16,     0,    23,    22,     0,    20,     0,
      26,    74,    44,    45,    46,    48,    49,    51,    52,    56,
      57,    54,    55,    59,    60,    62,    64,    66,    68,    70,
       0,    87,   208,   204,   207,     0,   127,     0,     0,     0,
       9,     0,     0,   206,   196,    38,     0,     0,   169,   171,
     162,   167,     0,   163,     0,     0,     0,   173,   189,   190,
     188,   187,   184,     0,     0,   191,   223,    88,     0,   227,
     216,     0,   210,   212,     0,   220,     0,     0,    89,   234,
       0,     0,     0,    42,    19,    21,     0,     0,   209,   199,
     134,   135,   136,    10,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   110,   104,   109,    95,    96,
       0,   106,    97,    98,    99,   100,   108,     0,   201,     0,
     203,     0,   205,   160,   168,     0,     0,   161,   174,   179,
     177,   186,   185,     0,   224,     0,   214,   219,   215,   228,
       0,   222,   225,     0,   229,     0,     0,   232,    94,     0,
      27,    72,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   122,   123,   124,     0,   105,   107,   200,   202,   170,
     172,   180,   194,   193,     0,   192,   211,   213,   226,   230,
     233,   235,    93,   101,     0,   103,     0,     0,     0,     0,
       0,     0,   121,   125,   195,   102,     0,     0,     0,     0,
       0,     0,   112,   114,   115,     0,     0,     0,     0,     0,
       0,     0,   117,     0,   119,     0,   113,   116,   118,   120
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,    45,    46,    47,    48,    49,    50,   189,    51,    52,
      53,    54,    55,    56,    57,    58,    59,    60,    61,    62,
      63,    64,   128,    65,   248,    66,    67,    68,   287,   288,
     289,   290,   291,   292,   293,   294,   295,    70,   162,    72,
     155,   156,    73,    74,    75,    76,   163,   164,   227,   228,
      77,   169,   170,    78,   241,   172,   242,   243,   313,   355,
     215,   158,   159,   213,   214,   251,   252,   253,   108,   254,
     177,   181,   260
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -208
static const yytype_int16 yypact[] =
{
     627,  -208,  -208,  -208,  -208,  -208,  -208,  -208,  1468,  1512,
    1512,  -208,  -208,  -208,  -208,  -208,  -208,  -208,  -208,  -208,
    -208,  -208,  -208,  -208,  -208,  -208,  -208,  -208,  -208,    22,
      23,    24,    60,  1524,  2269,  -208,  1524,  1524,  1524,  1524,
    1524,  1524,   -71,  1206,  -208,    37,   627,  -208,  -208,  -208,
      90,   390,  -208,    42,   -27,   180,    44,   187,   -34,    -2,
      -9,    91,    -6,  -208,  -208,   131,  -208,  -208,  -208,  -208,
    -208,   802,  -208,  -208,  -208,  -208,  -208,  -208,  -208,  1206,
    -208,  1524,  -208,  -208,    15,  -208,  2269,    18,  -208,  2269,
      81,  -208,   128,  -208,  -208,    87,  -208,  -208,  2146,   259,
    -208,  -208,  -208,  -208,  -208,  -208,    89,   -65,    98,  -208,
    -208,   219,  -208,  -208,   226,  1524,   895,  -208,  -208,  -208,
    -208,  -208,  -208,  -208,  -208,  -208,  -208,  -208,  1524,  1524,
    1524,  1524,  1524,  1524,  1524,  1524,  1524,  1524,  1524,  1524,
    1524,  1524,  1524,  1524,  1524,  1524,  1524,  1524,  -208,  1524,
    -208,   198,    25,  -208,  -208,   156,  -208,   -42,    67,    31,
     136,  2269,  1348,  1650,  -208,  2269,  1702,   128,   173,    77,
    -208,   169,   168,  1360,  1858,   -18,  -208,    86,   968,   552,
    -208,  -208,  1930,  -208,  1524,  -208,  -208,   -10,  -208,    95,
    -208,  -208,  -208,  -208,  -208,    42,    42,   -27,   -27,   180,
     180,   180,   180,    44,    44,   187,   -34,    -2,    -9,    91,
     -47,  -208,  -208,  -208,   198,   167,  -208,    25,   552,   341,
    -208,  1398,  2002,   198,    67,  -208,  1754,   171,  -208,   192,
    -208,  -208,  1806,  -208,   104,  1524,   128,  -208,  -208,  -208,
    -208,  -208,  -208,    60,   169,  -208,  -208,  -208,   182,  -208,
    1277,   189,   188,  -208,   197,    86,  1412,  2074,  -208,  -208,
     108,   195,   199,  -208,  -208,  -208,  1524,  1524,  -208,  -208,
    -208,   221,  -208,   217,  1524,   224,   205,   210,   213,   514,
     215,   316,   216,   218,   852,  -208,  -208,  -208,  -208,  -208,
     446,  -208,  -208,  -208,  -208,  -208,  -208,   802,  -208,   222,
    -208,   225,  -208,  -208,  -208,    25,  1524,  -208,  -208,  -208,
    -208,  -208,  -208,  1598,  -208,  1040,  -208,    10,  -208,  -208,
    2217,  -208,  -208,   223,  -208,   227,   110,  -208,  -208,   195,
    -208,  -208,   514,   239,   514,  1524,  1524,  1524,   268,   729,
     228,  -208,  -208,  -208,   176,  -208,  -208,  -208,  -208,  -208,
    -208,  -208,  -208,  -208,   245,  -208,  -208,  -208,  -208,  -208,
    -208,  -208,  -208,  -208,   514,  -208,   116,   122,   127,   231,
     866,   866,  -208,  -208,  -208,  -208,   514,   514,   514,  1524,
    1090,  1105,   277,  -208,  -208,   146,   514,   150,   514,   151,
     514,   232,  -208,   514,  -208,   514,  -208,  -208,  -208,  -208
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -208,  -208,  -208,   289,    39,  -208,  -208,  -208,    12,    34,
     152,   153,   -40,   149,   196,   194,   211,   249,   212,  -208,
    -167,   -92,  -208,   -38,     3,  -208,  -208,  -208,   -82,  -208,
    -153,  -208,    70,     0,  -208,  -208,  -208,  -207,     1,   -69,
    -208,   145,  -208,  -208,  -133,  -208,   -73,     5,  -208,    58,
    -208,   200,   130,  -208,   -29,  -208,   120,    55,  -208,  -208,
     -57,  -151,   -89,   175,  -208,  -163,  -208,    49,   -23,   -88,
    -158,  -156,  -208
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -1
static const yytype_uint16 yytable[] =
{
      69,    71,   154,    95,   220,   107,   247,   180,   224,   175,
     176,    99,   296,   150,   157,   146,   166,   255,   212,   262,
      80,    82,    83,   259,   190,    84,    87,    90,   150,   154,
     106,    85,    88,    91,   150,    98,   191,   109,   183,   267,
     149,   107,   218,   107,    98,    96,    69,    71,    96,    96,
      96,    96,    96,    96,   247,   142,   160,   211,   149,   301,
     136,   137,   272,    93,   219,   132,   133,    97,   247,    94,
     100,   101,   102,   103,   104,   105,   223,   187,   144,   147,
      98,   268,   173,   296,   174,   175,   143,   180,   226,   247,
     212,   264,   232,   154,   325,   149,   199,   200,   201,   202,
     331,   111,   112,   113,   223,   229,   353,   247,   328,   210,
     173,   145,   315,     1,     2,     3,     4,     5,     6,   151,
       8,   161,     9,    10,   165,   223,   180,   152,    86,    89,
      92,   168,   371,   152,   138,   139,   129,   130,   131,   247,
     107,    96,    96,    96,    96,    96,    96,    96,    96,    96,
      96,    96,    96,    96,    96,    96,    96,    96,    96,   255,
     271,   317,   318,   192,   193,   194,   224,   221,   231,   222,
     361,   231,    93,   171,   330,   250,   362,    33,    94,    98,
      34,   154,   236,   250,   237,    96,   256,   167,   257,   114,
     115,   182,   116,   316,   134,   135,    96,   338,   265,    36,
     266,   184,    37,    38,    39,   140,   141,    40,    41,   236,
      42,   308,    43,   326,   311,   327,   179,   360,   263,   376,
     297,   149,   185,   250,   299,   377,   317,   149,   154,   186,
     378,   231,   149,    96,   180,   148,   149,   231,   309,   225,
     271,   238,   239,   240,    25,    26,   344,    96,   229,   391,
     363,   149,   365,   393,   395,   149,   149,   235,   250,   323,
     216,   217,     1,     2,     3,     4,     5,     6,    96,     8,
     269,     9,    10,   244,   245,   304,   305,   333,   306,    96,
     373,   149,   375,   314,   195,   196,    96,   197,   198,   203,
     204,   297,   319,   320,   382,   383,   384,   366,   367,   368,
     321,   219,   329,   332,   392,   218,   394,   335,   396,   350,
     334,   398,   336,   399,    71,   337,   250,   339,    96,   340,
     341,   250,   342,   347,   358,   364,    33,   369,   348,    34,
     359,   374,   372,   379,   390,   110,   397,   206,   205,   370,
     297,   385,   387,   389,   273,     2,     3,     4,     5,     6,
       7,     8,   352,     9,    10,   207,    40,    41,   209,    42,
     346,   178,   270,   349,   312,   179,   310,   234,   354,   357,
     380,   381,     0,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    31,   208,   274,   275,   276,   302,   277,
     278,   279,   280,   281,   282,   283,   284,    32,    33,     0,
       0,    34,   117,   118,   119,   120,   121,   122,   123,   124,
     125,   126,     0,     0,    35,     0,     0,     0,     0,     0,
      36,     0,     0,    37,    38,    39,     0,     0,    40,    41,
       0,    42,     0,    43,     0,   285,     0,   219,   286,   273,
       2,     3,     4,     5,     6,     7,     8,     0,     9,    10,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   127,     0,     0,     0,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    31,     0,
     274,   275,   276,     0,   277,   278,   279,   280,   281,   282,
     283,   284,    32,    33,     0,     0,    34,   273,     2,     3,
       4,     5,     6,     0,     8,     0,     9,    10,     0,    35,
       0,     0,     0,     0,     0,    36,     0,     0,    37,    38,
      39,     0,     0,    40,    41,     0,    42,     0,    43,     0,
     285,     0,   219,   345,     0,     1,     2,     3,     4,     5,
       6,     0,     8,     0,     9,    10,     0,     0,   274,   275,
     276,     0,   277,   278,   279,   280,   281,   282,   283,   284,
       0,    33,     0,     0,    34,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    36,     0,     0,    37,    38,    39,     0,
       0,    40,    41,     0,    42,     0,    43,     0,   285,    33,
     219,     0,    34,     0,     0,     0,     0,     0,     0,     0,
       1,     2,     3,     4,     5,     6,     7,     8,     0,     9,
      10,    36,     0,     0,    37,    38,    39,     0,     0,    40,
      41,     0,    42,     0,    43,     0,     0,     0,   179,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    32,    33,     0,     0,    34,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      35,     0,     0,     0,     0,     0,    36,     0,     0,    37,
      38,    39,     0,     0,    40,    41,     0,    42,     0,    43,
       0,    44,     1,     2,     3,     4,     5,     6,     7,     8,
       0,     9,    10,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    31,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    32,    33,     0,     0,    34,
       0,     0,     0,     0,     0,   150,     0,     0,     0,     0,
       0,     7,    35,     0,     0,     0,     0,     0,    36,     0,
       0,    37,    38,    39,     0,     0,    40,    41,     0,    42,
       0,    43,     0,   285,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,     1,     2,     3,     4,     5,
       6,     0,     8,     0,     9,    10,     0,     0,    32,     1,
       2,     3,     4,     5,     6,     0,     8,     0,     9,    10,
       0,     0,     0,     0,     0,    35,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   151,     0,     1,     2,
       3,     4,     5,     6,   152,     8,   153,     9,    10,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    33,
       0,     0,    34,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    33,     0,     0,    34,     0,     0,     0,
       0,    36,     0,     0,    37,    38,    39,     0,     0,    40,
      41,     0,    42,     0,    43,    36,   343,     0,    37,    38,
      39,     0,    33,    40,    41,    34,    42,     0,    43,     0,
     285,     1,     2,     3,     4,     5,     6,     7,     8,     0,
       9,    10,     0,     0,    36,     0,     0,    37,    38,    39,
       0,     0,    40,    41,     0,    42,     0,    43,   188,     0,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    32,    33,     0,     0,    34,     0,
       0,     0,     0,   150,     0,     0,     0,     0,     0,     7,
       0,    35,     0,     0,     0,     0,     0,    36,     0,     0,
      37,    38,    39,     0,     0,    40,    41,     0,    42,     0,
      43,   258,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,     1,     2,     3,     4,     5,     6,     0,
       8,     0,     9,    10,     0,     0,    32,     0,     1,     2,
       3,     4,     5,     6,     0,     8,     0,     9,    10,     0,
       0,     0,     0,    35,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   151,     0,     0,     0,     0,     0,
     173,     0,   315,   249,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    33,     0,     0,
      34,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    33,     0,     0,    34,     0,     0,     0,    36,
       0,     0,    37,    38,    39,     0,     0,    40,    41,     0,
      42,     0,    43,   386,    36,     0,     0,    37,    38,    39,
       0,     0,    40,    41,     0,    42,     0,    43,   388,     1,
       2,     3,     4,     5,     6,     7,     8,     0,     9,    10,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    31,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    32,    33,     0,     0,    34,     0,     0,     0,
     150,     0,     0,     0,     0,     0,     7,     0,     0,    35,
       0,     0,     0,     0,     0,    36,     0,     0,    37,    38,
      39,     0,     0,    40,    41,     0,    42,     0,    43,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    32,     0,     0,     0,     0,     0,     0,
       0,   150,     0,     0,     0,     0,     0,     7,     0,     0,
      35,     0,     0,     1,     2,     3,     4,     5,     6,     0,
       8,   151,     9,    10,     0,     0,     0,   173,     0,   315,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31,     1,     2,     3,     4,     5,     6,     0,     8,     0,
       9,    10,     0,     0,    32,     1,     2,     3,     4,     5,
       6,     0,     8,     0,     9,    10,     0,    33,     0,     0,
      34,    35,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   151,     0,     0,     0,     0,     0,     0,    36,
     152,     0,    37,    38,    39,     0,     0,    40,    41,     0,
      42,   246,    43,     0,     0,    33,     0,     0,    34,     0,
       0,     1,     2,     3,     4,     5,     6,     0,     8,    33,
       9,    10,    34,     0,     0,     0,     0,    36,     0,     0,
      37,    38,    39,     0,     0,    40,    41,     0,    42,   298,
      43,    36,     0,     0,    37,    38,    39,     0,     0,    40,
      41,     0,    42,   322,    43,     1,     2,     3,     4,     5,
       6,     0,     8,     0,     9,    10,     0,     1,     2,     3,
       4,     5,     6,     0,     8,    33,     9,    10,    34,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    36,     0,     0,
      37,    38,    39,     0,     0,    40,    41,     0,    42,     0,
      79,     0,     0,     0,     0,     0,     0,     0,     0,    33,
       0,     0,    34,     0,     0,     0,     0,     0,     0,     0,
       0,    33,     0,     0,    34,     0,     0,     0,     0,     0,
       0,    36,     0,     0,    37,    38,    39,     7,     0,    40,
      41,     0,    42,    36,    81,     0,    37,    38,    39,     0,
       0,    40,    41,     0,    42,     0,    43,     0,     0,     0,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31,     0,     0,     0,     0,     0,     0,     0,     0,     7,
       0,     0,     0,     0,    32,     0,     0,     0,     0,     0,
     238,   239,   240,     0,     0,     0,     0,     0,     0,     0,
       0,    35,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,     0,     0,   351,     0,     0,     0,     0,
       0,     7,     0,     0,     0,     0,    32,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    35,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,     0,     0,   230,     0,     0,
       0,     0,     0,     7,     0,     0,     0,     0,    32,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    35,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,     0,     0,   233,
       0,     0,     0,     0,     0,     7,     0,     0,     0,     0,
      32,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    35,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    31,     0,
       0,   303,     0,     0,     0,     0,     0,     7,     0,     0,
       0,     0,    32,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    35,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31,     0,     0,   307,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    32,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     7,
       0,    35,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   151,     0,     0,     0,     0,     0,   173,     0,
     174,   249,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    32,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     7,     0,    35,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   261,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    32,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     7,     0,    35,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   300,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      32,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     7,     0,    35,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   324,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    31,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    32,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     7,     0,     0,    35,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     151,     0,     0,     0,     0,     0,   173,     0,   174,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
     356,     0,     0,     0,     0,     0,     0,     0,     7,     0,
       0,     0,     0,    32,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      35,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    31,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    32,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    35
};

static const yytype_int16 yycheck[] =
{
       0,     0,    71,    32,   157,    43,   173,    99,   159,    98,
      98,    34,   219,     3,    71,    21,    89,   175,   151,   182,
       8,     9,    10,   179,   116,     3,     3,     3,     3,    98,
     101,     9,     9,     9,     3,    34,   128,     0,   103,    86,
     105,    79,    84,    81,    43,    33,    46,    46,    36,    37,
      38,    39,    40,    41,   221,    89,    79,   149,   105,   222,
      16,    17,   218,     3,   106,    92,    93,    33,   235,     9,
      36,    37,    38,    39,    40,    41,    94,   115,    87,    85,
      79,   214,   100,   290,   102,   174,    88,   179,   161,   256,
     223,   101,   165,   162,   257,   105,   136,   137,   138,   139,
     267,    11,    12,    13,    94,   162,   313,   274,   261,   147,
     100,    20,   102,     3,     4,     5,     6,     7,     8,    94,
      10,   106,    12,    13,   106,    94,   218,   102,   106,   106,
     106,     3,   339,   102,    90,    91,    94,    95,    96,   306,
     178,   129,   130,   131,   132,   133,   134,   135,   136,   137,
     138,   139,   140,   141,   142,   143,   144,   145,   146,   317,
     217,   250,   250,   129,   130,   131,   317,   100,   163,   102,
     326,   166,     3,    86,   266,   174,   329,    67,     9,   178,
      70,   250,   105,   182,   107,   173,   100,   106,   102,    99,
     100,   102,   102,   250,    14,    15,   184,   279,   103,    89,
     105,   103,    92,    93,    94,    18,    19,    97,    98,   105,
     100,   107,   102,   105,   243,   107,   106,   107,   184,   103,
     219,   105,     3,   222,   221,   103,   315,   105,   297,     3,
     103,   226,   105,   221,   326,   104,   105,   232,   235,   103,
     297,    72,    73,    74,    46,    47,   284,   235,   305,   103,
     332,   105,   334,   103,   103,   105,   105,    84,   257,   256,
     104,   105,     3,     4,     5,     6,     7,     8,   256,    10,
     103,    12,    13,   105,   106,   104,   105,   274,    86,   267,
     104,   105,   364,   101,   132,   133,   274,   134,   135,   140,
     141,   290,   103,   105,   376,   377,   378,   335,   336,   337,
     103,   106,   103,    86,   386,    84,   388,   102,   390,   306,
      86,   393,   102,   395,   313,   102,   315,   102,   306,     3,
     104,   320,   104,   101,   101,    86,    67,    59,   103,    70,
     103,    86,   104,   102,    57,    46,   104,   143,   142,   339,
     339,   379,   380,   381,     3,     4,     5,     6,     7,     8,
       9,    10,   313,    12,    13,   144,    97,    98,   146,   100,
     290,   102,   217,   305,   244,   106,   236,   167,   313,   320,
     370,   371,    -1,    32,    33,    34,    35,    36,    37,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    47,    48,
      49,    50,    51,    52,   145,    54,    55,    56,   223,    58,
      59,    60,    61,    62,    63,    64,    65,    66,    67,    -1,
      -1,    70,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    -1,    -1,    83,    -1,    -1,    -1,    -1,    -1,
      89,    -1,    -1,    92,    93,    94,    -1,    -1,    97,    98,
      -1,   100,    -1,   102,    -1,   104,    -1,   106,   107,     3,
       4,     5,     6,     7,     8,     9,    10,    -1,    12,    13,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    84,    -1,    -1,    -1,    32,    33,
      34,    35,    36,    37,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    47,    48,    49,    50,    51,    52,    -1,
      54,    55,    56,    -1,    58,    59,    60,    61,    62,    63,
      64,    65,    66,    67,    -1,    -1,    70,     3,     4,     5,
       6,     7,     8,    -1,    10,    -1,    12,    13,    -1,    83,
      -1,    -1,    -1,    -1,    -1,    89,    -1,    -1,    92,    93,
      94,    -1,    -1,    97,    98,    -1,   100,    -1,   102,    -1,
     104,    -1,   106,   107,    -1,     3,     4,     5,     6,     7,
       8,    -1,    10,    -1,    12,    13,    -1,    -1,    54,    55,
      56,    -1,    58,    59,    60,    61,    62,    63,    64,    65,
      -1,    67,    -1,    -1,    70,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    89,    -1,    -1,    92,    93,    94,    -1,
      -1,    97,    98,    -1,   100,    -1,   102,    -1,   104,    67,
     106,    -1,    70,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
       3,     4,     5,     6,     7,     8,     9,    10,    -1,    12,
      13,    89,    -1,    -1,    92,    93,    94,    -1,    -1,    97,
      98,    -1,   100,    -1,   102,    -1,    -1,    -1,   106,    32,
      33,    34,    35,    36,    37,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    47,    48,    49,    50,    51,    52,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    66,    67,    -1,    -1,    70,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      83,    -1,    -1,    -1,    -1,    -1,    89,    -1,    -1,    92,
      93,    94,    -1,    -1,    97,    98,    -1,   100,    -1,   102,
      -1,   104,     3,     4,     5,     6,     7,     8,     9,    10,
      -1,    12,    13,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    32,    33,    34,    35,    36,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    47,    48,    49,    50,
      51,    52,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    66,    67,    -1,    -1,    70,
      -1,    -1,    -1,    -1,    -1,     3,    -1,    -1,    -1,    -1,
      -1,     9,    83,    -1,    -1,    -1,    -1,    -1,    89,    -1,
      -1,    92,    93,    94,    -1,    -1,    97,    98,    -1,   100,
      -1,   102,    -1,   104,    32,    33,    34,    35,    36,    37,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    47,
      48,    49,    50,    51,    52,     3,     4,     5,     6,     7,
       8,    -1,    10,    -1,    12,    13,    -1,    -1,    66,     3,
       4,     5,     6,     7,     8,    -1,    10,    -1,    12,    13,
      -1,    -1,    -1,    -1,    -1,    83,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    94,    -1,     3,     4,
       5,     6,     7,     8,   102,    10,   104,    12,    13,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    67,
      -1,    -1,    70,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    67,    -1,    -1,    70,    -1,    -1,    -1,
      -1,    89,    -1,    -1,    92,    93,    94,    -1,    -1,    97,
      98,    -1,   100,    -1,   102,    89,   104,    -1,    92,    93,
      94,    -1,    67,    97,    98,    70,   100,    -1,   102,    -1,
     104,     3,     4,     5,     6,     7,     8,     9,    10,    -1,
      12,    13,    -1,    -1,    89,    -1,    -1,    92,    93,    94,
      -1,    -1,    97,    98,    -1,   100,    -1,   102,   103,    -1,
      32,    33,    34,    35,    36,    37,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    47,    48,    49,    50,    51,
      52,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    66,    67,    -1,    -1,    70,    -1,
      -1,    -1,    -1,     3,    -1,    -1,    -1,    -1,    -1,     9,
      -1,    83,    -1,    -1,    -1,    -1,    -1,    89,    -1,    -1,
      92,    93,    94,    -1,    -1,    97,    98,    -1,   100,    -1,
     102,   103,    32,    33,    34,    35,    36,    37,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    47,    48,    49,
      50,    51,    52,     3,     4,     5,     6,     7,     8,    -1,
      10,    -1,    12,    13,    -1,    -1,    66,    -1,     3,     4,
       5,     6,     7,     8,    -1,    10,    -1,    12,    13,    -1,
      -1,    -1,    -1,    83,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    94,    -1,    -1,    -1,    -1,    -1,
     100,    -1,   102,   103,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    67,    -1,    -1,
      70,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    67,    -1,    -1,    70,    -1,    -1,    -1,    89,
      -1,    -1,    92,    93,    94,    -1,    -1,    97,    98,    -1,
     100,    -1,   102,   103,    89,    -1,    -1,    92,    93,    94,
      -1,    -1,    97,    98,    -1,   100,    -1,   102,   103,     3,
       4,     5,     6,     7,     8,     9,    10,    -1,    12,    13,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,    33,
      34,    35,    36,    37,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    47,    48,    49,    50,    51,    52,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    66,    67,    -1,    -1,    70,    -1,    -1,    -1,
       3,    -1,    -1,    -1,    -1,    -1,     9,    -1,    -1,    83,
      -1,    -1,    -1,    -1,    -1,    89,    -1,    -1,    92,    93,
      94,    -1,    -1,    97,    98,    -1,   100,    -1,   102,    32,
      33,    34,    35,    36,    37,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    47,    48,    49,    50,    51,    52,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    66,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,     3,    -1,    -1,    -1,    -1,    -1,     9,    -1,    -1,
      83,    -1,    -1,     3,     4,     5,     6,     7,     8,    -1,
      10,    94,    12,    13,    -1,    -1,    -1,   100,    -1,   102,
      32,    33,    34,    35,    36,    37,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    47,    48,    49,    50,    51,
      52,     3,     4,     5,     6,     7,     8,    -1,    10,    -1,
      12,    13,    -1,    -1,    66,     3,     4,     5,     6,     7,
       8,    -1,    10,    -1,    12,    13,    -1,    67,    -1,    -1,
      70,    83,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    94,    -1,    -1,    -1,    -1,    -1,    -1,    89,
     102,    -1,    92,    93,    94,    -1,    -1,    97,    98,    -1,
     100,   101,   102,    -1,    -1,    67,    -1,    -1,    70,    -1,
      -1,     3,     4,     5,     6,     7,     8,    -1,    10,    67,
      12,    13,    70,    -1,    -1,    -1,    -1,    89,    -1,    -1,
      92,    93,    94,    -1,    -1,    97,    98,    -1,   100,   101,
     102,    89,    -1,    -1,    92,    93,    94,    -1,    -1,    97,
      98,    -1,   100,   101,   102,     3,     4,     5,     6,     7,
       8,    -1,    10,    -1,    12,    13,    -1,     3,     4,     5,
       6,     7,     8,    -1,    10,    67,    12,    13,    70,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    89,    -1,    -1,
      92,    93,    94,    -1,    -1,    97,    98,    -1,   100,    -1,
     102,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    67,
      -1,    -1,    70,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    67,    -1,    -1,    70,    -1,    -1,    -1,    -1,    -1,
      -1,    89,    -1,    -1,    92,    93,    94,     9,    -1,    97,
      98,    -1,   100,    89,   102,    -1,    92,    93,    94,    -1,
      -1,    97,    98,    -1,   100,    -1,   102,    -1,    -1,    -1,
      32,    33,    34,    35,    36,    37,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    47,    48,    49,    50,    51,
      52,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     9,
      -1,    -1,    -1,    -1,    66,    -1,    -1,    -1,    -1,    -1,
      72,    73,    74,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    83,    32,    33,    34,    35,    36,    37,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    47,    48,    49,
      50,    51,    52,    -1,    -1,   107,    -1,    -1,    -1,    -1,
      -1,     9,    -1,    -1,    -1,    -1,    66,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    83,    32,    33,    34,    35,    36,    37,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    47,
      48,    49,    50,    51,    52,    -1,    -1,   107,    -1,    -1,
      -1,    -1,    -1,     9,    -1,    -1,    -1,    -1,    66,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    83,    32,    33,    34,    35,
      36,    37,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    47,    48,    49,    50,    51,    52,    -1,    -1,   107,
      -1,    -1,    -1,    -1,    -1,     9,    -1,    -1,    -1,    -1,
      66,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    83,    32,    33,
      34,    35,    36,    37,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    47,    48,    49,    50,    51,    52,    -1,
      -1,   107,    -1,    -1,    -1,    -1,    -1,     9,    -1,    -1,
      -1,    -1,    66,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    83,
      32,    33,    34,    35,    36,    37,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    47,    48,    49,    50,    51,
      52,    -1,    -1,   107,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    66,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     9,
      -1,    83,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    94,    -1,    -1,    -1,    -1,    -1,   100,    -1,
     102,   103,    32,    33,    34,    35,    36,    37,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    47,    48,    49,
      50,    51,    52,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    66,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,     9,    -1,    83,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   103,    32,    33,    34,    35,    36,    37,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    47,
      48,    49,    50,    51,    52,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    66,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,     9,    -1,    83,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   103,    32,    33,    34,    35,
      36,    37,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    47,    48,    49,    50,    51,    52,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      66,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,     9,    -1,    83,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   103,    32,    33,
      34,    35,    36,    37,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    47,    48,    49,    50,    51,    52,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    66,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,     9,    -1,    -1,    83,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      94,    -1,    -1,    -1,    -1,    -1,   100,    -1,   102,    32,
      33,    34,    35,    36,    37,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    47,    48,    49,    50,    51,    52,
      53,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     9,    -1,
      -1,    -1,    -1,    66,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      83,    32,    33,    34,    35,    36,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    47,    48,    49,    50,
      51,    52,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    66,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    83
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     3,     4,     5,     6,     7,     8,     9,    10,    12,
      13,    32,    33,    34,    35,    36,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    47,    48,    49,    50,
      51,    52,    66,    67,    70,    83,    89,    92,    93,    94,
      97,    98,   100,   102,   104,   109,   110,   111,   112,   113,
     114,   116,   117,   118,   119,   120,   121,   122,   123,   124,
     125,   126,   127,   128,   129,   131,   133,   134,   135,   141,
     145,   146,   147,   150,   151,   152,   153,   158,   161,   102,
     116,   102,   116,   116,     3,     9,   106,     3,     9,   106,
       3,     9,   106,     3,     9,   162,   116,   117,   146,   176,
     117,   117,   117,   117,   117,   117,   101,   131,   176,     0,
     111,    11,    12,    13,    99,   100,   102,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    84,   130,    94,
      95,    96,    92,    93,    14,    15,    16,    17,    90,    91,
      18,    19,    89,    88,    87,    20,    21,    85,   104,   105,
       3,    94,   102,   104,   147,   148,   149,   168,   169,   170,
     176,   106,   146,   154,   155,   106,   154,   106,     3,   159,
     160,    86,   163,   100,   102,   170,   177,   178,   102,   106,
     129,   179,   102,   103,   103,     3,     3,   131,   103,   115,
     129,   129,   117,   117,   117,   118,   118,   119,   119,   120,
     120,   120,   120,   121,   121,   122,   123,   124,   125,   126,
     131,   129,   152,   171,   172,   168,   104,   105,    84,   106,
     138,   100,   102,    94,   169,   103,   154,   156,   157,   168,
     107,   155,   154,   107,   159,    84,   105,   107,    72,    73,
      74,   162,   164,   165,   105,   106,   101,   128,   132,   103,
     146,   173,   174,   175,   177,   178,   100,   102,   103,   179,
     180,   103,   173,   117,   101,   103,   105,    86,   152,   103,
     149,   168,   179,     3,    54,    55,    56,    58,    59,    60,
      61,    62,    63,    64,    65,   104,   107,   136,   137,   138,
     139,   140,   141,   142,   143,   144,   145,   146,   101,   132,
     103,   173,   171,   107,   104,   105,    86,   107,   107,   132,
     160,   162,   164,   166,   101,   102,   168,   170,   177,   103,
     105,   103,   101,   132,   103,   173,   105,   107,   138,   103,
     129,   128,    86,   132,    86,   102,   102,   102,   136,   102,
       3,   104,   104,   104,   131,   107,   140,   101,   103,   157,
     132,   107,   112,   145,   165,   167,    53,   175,   101,   103,
     107,   179,   138,   136,    86,   136,   131,   131,   131,    59,
     141,   145,   104,   104,    86,   136,   103,   103,   103,   102,
     141,   141,   136,   136,   136,   131,   103,   131,   103,   131,
      57,   103,   136,   103,   136,   103,   136,   104,   136,   136
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
   Once GCC version 2 has supplanted version 1, this can go.  */

#define YYFAIL		goto yyerrlab

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      yytoken = YYTRANSLATE (yychar);				\
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


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef YY_LOCATION_PRINT
# if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL
#  define YY_LOCATION_PRINT(File, Loc)			\
     fprintf (File, "%d.%d-%d.%d",			\
	      (Loc).first_line, (Loc).first_column,	\
	      (Loc).last_line,  (Loc).last_column)
# else
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif
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
yy_stack_print (yytype_int16 *bottom, yytype_int16 *top)
#else
static void
yy_stack_print (bottom, top)
    yytype_int16 *bottom;
    yytype_int16 *top;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (; bottom <= top; ++bottom)
    YYFPRINTF (stderr, " %d", *bottom);
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
      fprintf (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
		       &(yyvsp[(yyi + 1) - (yynrhs)])
		       		       );
      fprintf (stderr, "\n");
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

/* Copy into YYRESULT an error message about the unexpected token
   YYCHAR while in state YYSTATE.  Return the number of bytes copied,
   including the terminating null byte.  If YYRESULT is null, do not
   copy anything; just return the number of bytes that would be
   copied.  As a special case, return 0 if an ordinary "syntax error"
   message will do.  Return YYSIZE_MAXIMUM if overflow occurs during
   size calculation.  */
static YYSIZE_T
yysyntax_error (char *yyresult, int yystate, int yychar)
{
  int yyn = yypact[yystate];

  if (! (YYPACT_NINF < yyn && yyn <= YYLAST))
    return 0;
  else
    {
      int yytype = YYTRANSLATE (yychar);
      YYSIZE_T yysize0 = yytnamerr (0, yytname[yytype]);
      YYSIZE_T yysize = yysize0;
      YYSIZE_T yysize1;
      int yysize_overflow = 0;
      enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
      char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
      int yyx;

# if 0
      /* This is so xgettext sees the translatable formats that are
	 constructed on the fly.  */
      YY_("syntax error, unexpected %s");
      YY_("syntax error, unexpected %s, expecting %s");
      YY_("syntax error, unexpected %s, expecting %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s");
# endif
      char *yyfmt;
      char const *yyf;
      static char const yyunexpected[] = "syntax error, unexpected %s";
      static char const yyexpecting[] = ", expecting %s";
      static char const yyor[] = " or %s";
      char yyformat[sizeof yyunexpected
		    + sizeof yyexpecting - 1
		    + ((YYERROR_VERBOSE_ARGS_MAXIMUM - 2)
		       * (sizeof yyor - 1))];
      char const *yyprefix = yyexpecting;

      /* Start YYX at -YYN if negative to avoid negative indexes in
	 YYCHECK.  */
      int yyxbegin = yyn < 0 ? -yyn : 0;

      /* Stay within bounds of both yycheck and yytname.  */
      int yychecklim = YYLAST - yyn + 1;
      int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
      int yycount = 1;

      yyarg[0] = yytname[yytype];
      yyfmt = yystpcpy (yyformat, yyunexpected);

      for (yyx = yyxbegin; yyx < yyxend; ++yyx)
	if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
	  {
	    if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
	      {
		yycount = 1;
		yysize = yysize0;
		yyformat[sizeof yyunexpected - 1] = '\0';
		break;
	      }
	    yyarg[yycount++] = yytname[yyx];
	    yysize1 = yysize + yytnamerr (0, yytname[yyx]);
	    yysize_overflow |= (yysize1 < yysize);
	    yysize = yysize1;
	    yyfmt = yystpcpy (yyfmt, yyprefix);
	    yyprefix = yyor;
	  }

      yyf = YY_(yyformat);
      yysize1 = yysize + yystrlen (yyf);
      yysize_overflow |= (yysize1 < yysize);
      yysize = yysize1;

      if (yysize_overflow)
	return YYSIZE_MAXIMUM;

      if (yyresult)
	{
	  /* Avoid sprintf, as that infringes on the user's name space.
	     Don't have undefined behavior even if the translation
	     produced a string with the wrong number of "%s"s.  */
	  char *yyp = yyresult;
	  int yyi = 0;
	  while ((*yyp = *yyf) != '\0')
	    {
	      if (*yyp == '%' && yyf[1] == 's' && yyi < yycount)
		{
		  yyp += yytnamerr (yyp, yyarg[yyi++]);
		  yyf += 2;
		}
	      else
		{
		  yyp++;
		  yyf++;
		}
	    }
	}
      return yysize;
    }
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



/* The look-ahead symbol.  */
int yychar;

/* The semantic value of the look-ahead symbol.  */
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
  int yyn;
  int yyresult;
  /* Number of tokens to shift before error messages enabled.  */
  int yyerrstatus;
  /* Look-ahead token as an internal (translated) token number.  */
  int yytoken = 0;
#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

  /* Three stacks and their tools:
     `yyss': related to states,
     `yyvs': related to semantic values,
     `yyls': related to locations.

     Refer to the stacks thru separate pointers, to allow yyoverflow
     to reallocate them elsewhere.  */

  /* The state stack.  */
  yytype_int16 yyssa[YYINITDEPTH];
  yytype_int16 *yyss = yyssa;
  yytype_int16 *yyssp;

  /* The semantic value stack.  */
  YYSTYPE yyvsa[YYINITDEPTH];
  YYSTYPE *yyvs = yyvsa;
  YYSTYPE *yyvsp;



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  YYSIZE_T yystacksize = YYINITDEPTH;

  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;


  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY;		/* Cause a token to be read.  */

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
	YYSTACK_RELOCATE (yyss);
	YYSTACK_RELOCATE (yyvs);

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

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     look-ahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to look-ahead token.  */
  yyn = yypact[yystate];
  if (yyn == YYPACT_NINF)
    goto yydefault;

  /* Not known => get a look-ahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid look-ahead symbol.  */
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
      if (yyn == 0 || yyn == YYTABLE_NINF)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the look-ahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token unless it is eof.  */
  if (yychar != YYEOF)
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
#line 123 "parser.y"
    { root = (yyvsp[(1) - (1)].node); ;}
    break;

  case 3:
#line 127 "parser.y"
    { (yyval.node) = create_node(NODE_TRANSLATION_UNIT); (yyval.node)->data.items_list = create_list_node((yyvsp[(1) - (1)].node)); ;}
    break;

  case 4:
#line 128 "parser.y"
    { if ((yyvsp[(2) - (2)].node)) { (yyvsp[(1) - (2)].node)->data.items_list = append_to_list((yyvsp[(1) - (2)].node)->data.items_list, (yyvsp[(2) - (2)].node)); } (yyval.node) = (yyvsp[(1) - (2)].node); ;}
    break;

  case 5:
#line 132 "parser.y"
    { (yyval.node) = (yyvsp[(1) - (1)].node); ;}
    break;

  case 6:
#line 133 "parser.y"
    { (yyval.node) = (yyvsp[(1) - (1)].node); ;}
    break;

  case 7:
#line 134 "parser.y"
    { (yyval.node) = (yyvsp[(1) - (1)].node); ;}
    break;

  case 8:
#line 135 "parser.y"
    { (yyval.node) = NULL; ;}
    break;

  case 9:
#line 139 "parser.y"
    { (yyval.node) = create_function_definition_node((yyvsp[(1) - (3)].list), (yyvsp[(2) - (3)].node), (yyvsp[(3) - (3)].node)); ;}
    break;

  case 10:
#line 144 "parser.y"
    { (yyval.node) = create_identifier_node((yyvsp[(1) - (1)].str)); ;}
    break;

  case 11:
#line 145 "parser.y"
    { (yyval.node) = create_constant_node((yyvsp[(1) - (1)].str)); ;}
    break;

  case 12:
#line 146 "parser.y"
    { (yyval.node) = create_string_literal_node((yyvsp[(1) - (1)].str)); ;}
    break;

  case 13:
#line 147 "parser.y"
    { (yyval.node) = create_constant_node((yyvsp[(1) - (1)].str)); ;}
    break;

  case 14:
#line 148 "parser.y"
    { (yyval.node) = create_constant_node((yyvsp[(1) - (1)].str)); ;}
    break;

  case 15:
#line 149 "parser.y"
    { (yyval.node) = create_constant_node((yyvsp[(1) - (1)].str)); ;}
    break;

  case 16:
#line 150 "parser.y"
    { (yyval.node) = (yyvsp[(2) - (3)].node); ;}
    break;

  case 17:
#line 151 "parser.y"
    { (yyval.node) = (yyvsp[(1) - (1)].node); ;}
    break;

  case 19:
#line 156 "parser.y"
    { (yyval.node) = create_array_subscript_node((yyvsp[(1) - (4)].node), (yyvsp[(3) - (4)].node)); ;}
    break;

  case 20:
#line 157 "parser.y"
    { (yyval.node) = create_func_call_node((yyvsp[(1) - (3)].node), NULL); ;}
    break;

  case 21:
#line 158 "parser.y"
    { (yyval.node) = create_func_call_node((yyvsp[(1) - (4)].node), (yyvsp[(3) - (4)].list)); ;}
    break;

  case 22:
#line 159 "parser.y"
    { (yyval.node) = create_member_access_node((yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].str), 0); ;}
    break;

  case 23:
#line 160 "parser.y"
    { (yyval.node) = create_member_access_node((yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].str), 1); ;}
    break;

  case 24:
#line 161 "parser.y"
    { (yyval.node) = create_unary_expr_node(INC_OP, (yyvsp[(1) - (2)].node)); ;}
    break;

  case 25:
#line 162 "parser.y"
    { (yyval.node) = create_unary_expr_node(DEC_OP, (yyvsp[(1) - (2)].node)); ;}
    break;

  case 26:
#line 167 "parser.y"
    { (yyval.list) = create_list_node((yyvsp[(1) - (1)].node)); ;}
    break;

  case 27:
#line 169 "parser.y"
    { (yyval.list) = append_to_list((yyvsp[(1) - (3)].list), (yyvsp[(3) - (3)].node)); ;}
    break;

  case 29:
#line 174 "parser.y"
    { (yyval.node) = create_unary_expr_node(INC_OP, (yyvsp[(2) - (2)].node)); ;}
    break;

  case 30:
#line 175 "parser.y"
    { (yyval.node) = create_unary_expr_node(DEC_OP, (yyvsp[(2) - (2)].node)); ;}
    break;

  case 31:
#line 176 "parser.y"
    { (yyval.node) = create_unary_expr_node('&', (yyvsp[(2) - (2)].node)); ;}
    break;

  case 32:
#line 177 "parser.y"
    { (yyval.node) = create_unary_expr_node('*', (yyvsp[(2) - (2)].node)); ;}
    break;

  case 33:
#line 178 "parser.y"
    { (yyval.node) = create_unary_expr_node('+', (yyvsp[(2) - (2)].node)); ;}
    break;

  case 34:
#line 179 "parser.y"
    { (yyval.node) = create_unary_expr_node('-', (yyvsp[(2) - (2)].node)); ;}
    break;

  case 35:
#line 180 "parser.y"
    { (yyval.node) = create_unary_expr_node('~', (yyvsp[(2) - (2)].node)); ;}
    break;

  case 36:
#line 181 "parser.y"
    { (yyval.node) = create_unary_expr_node('!', (yyvsp[(2) - (2)].node)); ;}
    break;

  case 37:
#line 182 "parser.y"
    { (yyval.node) = create_unary_expr_node(SIZEOF, (yyvsp[(2) - (2)].node)); ;}
    break;

  case 38:
#line 183 "parser.y"
    { (yyval.node) = create_unary_expr_node(SIZEOF, (yyvsp[(3) - (4)].node)); ;}
    break;

  case 39:
#line 184 "parser.y"
    { (yyval.node) = (yyvsp[(1) - (1)].node); ;}
    break;

  case 40:
#line 185 "parser.y"
    { (yyval.node) = (yyvsp[(1) - (1)].node); ;}
    break;

  case 42:
#line 199 "parser.y"
    { (yyval.node) = create_cast_expr_node((yyvsp[(2) - (4)].node), (yyvsp[(4) - (4)].node)); ;}
    break;

  case 44:
#line 204 "parser.y"
    { (yyval.node) = create_binary_expr_node((yyvsp[(1) - (3)].node), '*', (yyvsp[(3) - (3)].node)); ;}
    break;

  case 45:
#line 205 "parser.y"
    { (yyval.node) = create_binary_expr_node((yyvsp[(1) - (3)].node), '/', (yyvsp[(3) - (3)].node)); ;}
    break;

  case 46:
#line 206 "parser.y"
    { (yyval.node) = create_binary_expr_node((yyvsp[(1) - (3)].node), '%', (yyvsp[(3) - (3)].node)); ;}
    break;

  case 48:
#line 211 "parser.y"
    { (yyval.node) = create_binary_expr_node((yyvsp[(1) - (3)].node), '+', (yyvsp[(3) - (3)].node)); ;}
    break;

  case 49:
#line 212 "parser.y"
    { (yyval.node) = create_binary_expr_node((yyvsp[(1) - (3)].node), '-', (yyvsp[(3) - (3)].node)); ;}
    break;

  case 51:
#line 217 "parser.y"
    { (yyval.node) = create_binary_expr_node((yyvsp[(1) - (3)].node), LSHIFT_OP, (yyvsp[(3) - (3)].node)); ;}
    break;

  case 52:
#line 218 "parser.y"
    { (yyval.node) = create_binary_expr_node((yyvsp[(1) - (3)].node), RSHIFT_OP, (yyvsp[(3) - (3)].node)); ;}
    break;

  case 54:
#line 223 "parser.y"
    { (yyval.node) = create_binary_expr_node((yyvsp[(1) - (3)].node), '<', (yyvsp[(3) - (3)].node)); ;}
    break;

  case 55:
#line 224 "parser.y"
    { (yyval.node) = create_binary_expr_node((yyvsp[(1) - (3)].node), '>', (yyvsp[(3) - (3)].node)); ;}
    break;

  case 56:
#line 225 "parser.y"
    { (yyval.node) = create_binary_expr_node((yyvsp[(1) - (3)].node), LE_OP, (yyvsp[(3) - (3)].node)); ;}
    break;

  case 57:
#line 226 "parser.y"
    { (yyval.node) = create_binary_expr_node((yyvsp[(1) - (3)].node), GE_OP, (yyvsp[(3) - (3)].node)); ;}
    break;

  case 59:
#line 231 "parser.y"
    { (yyval.node) = create_binary_expr_node((yyvsp[(1) - (3)].node), EQ_OP, (yyvsp[(3) - (3)].node)); ;}
    break;

  case 60:
#line 232 "parser.y"
    { (yyval.node) = create_binary_expr_node((yyvsp[(1) - (3)].node), NE_OP, (yyvsp[(3) - (3)].node)); ;}
    break;

  case 62:
#line 237 "parser.y"
    { (yyval.node) = create_binary_expr_node((yyvsp[(1) - (3)].node), '&', (yyvsp[(3) - (3)].node)); ;}
    break;

  case 64:
#line 242 "parser.y"
    { (yyval.node) = create_binary_expr_node((yyvsp[(1) - (3)].node), '^', (yyvsp[(3) - (3)].node)); ;}
    break;

  case 66:
#line 247 "parser.y"
    { (yyval.node) = create_binary_expr_node((yyvsp[(1) - (3)].node), '|', (yyvsp[(3) - (3)].node)); ;}
    break;

  case 68:
#line 252 "parser.y"
    { (yyval.node) = create_binary_expr_node((yyvsp[(1) - (3)].node), LAND_OP, (yyvsp[(3) - (3)].node)); ;}
    break;

  case 70:
#line 257 "parser.y"
    { (yyval.node) = create_binary_expr_node((yyvsp[(1) - (3)].node), LOR_OP, (yyvsp[(3) - (3)].node)); ;}
    break;

  case 72:
#line 262 "parser.y"
    { (yyval.node) = create_conditional_expr_node((yyvsp[(1) - (5)].node), (yyvsp[(3) - (5)].node), (yyvsp[(5) - (5)].node)); ;}
    break;

  case 74:
#line 267 "parser.y"
    { (yyval.node) = create_assignment_node((yyvsp[(1) - (3)].node), (yyvsp[(2) - (3)].token), (yyvsp[(3) - (3)].node)); ;}
    break;

  case 75:
#line 271 "parser.y"
    { (yyval.token) = '='; ;}
    break;

  case 76:
#line 272 "parser.y"
    { (yyval.token) = MUL_ASSIGN; ;}
    break;

  case 77:
#line 273 "parser.y"
    { (yyval.token) = DIV_ASSIGN; ;}
    break;

  case 78:
#line 274 "parser.y"
    { (yyval.token) = MOD_ASSIGN; ;}
    break;

  case 79:
#line 275 "parser.y"
    { (yyval.token) = ADD_ASSIGN; ;}
    break;

  case 80:
#line 276 "parser.y"
    { (yyval.token) = SUB_ASSIGN; ;}
    break;

  case 81:
#line 277 "parser.y"
    { (yyval.token) = LSHIFT_ASSIGN; ;}
    break;

  case 82:
#line 278 "parser.y"
    { (yyval.token) = RSHIFT_ASSIGN; ;}
    break;

  case 83:
#line 279 "parser.y"
    { (yyval.token) = AND_ASSIGN; ;}
    break;

  case 84:
#line 280 "parser.y"
    { (yyval.token) = XOR_ASSIGN; ;}
    break;

  case 85:
#line 281 "parser.y"
    { (yyval.token) = OR_ASSIGN; ;}
    break;

  case 87:
#line 286 "parser.y"
    { (yyval.node) = create_binary_expr_node((yyvsp[(1) - (3)].node), ',', (yyvsp[(3) - (3)].node)); ;}
    break;

  case 89:
#line 295 "parser.y"
    { (yyval.node) = create_new_expr_node((yyvsp[(2) - (4)].node), NULL); ;}
    break;

  case 90:
#line 296 "parser.y"
    { (yyval.node) = create_new_expr_node((yyvsp[(2) - (2)].node), NULL); ;}
    break;

  case 91:
#line 297 "parser.y"
    { (yyval.node) = create_new_expr_node((yyvsp[(2) - (3)].node), (yyvsp[(3) - (3)].node)); ;}
    break;

  case 92:
#line 301 "parser.y"
    { (yyval.node) = create_delete_expr_node((yyvsp[(2) - (2)].node)); ;}
    break;

  case 93:
#line 305 "parser.y"
    { (yyval.node) = create_lambda_node(NULL, (yyvsp[(4) - (6)].list), (yyvsp[(6) - (6)].node)); ;}
    break;

  case 94:
#line 306 "parser.y"
    { (yyval.node) = create_lambda_node(NULL, NULL, (yyvsp[(5) - (5)].node)); ;}
    break;

  case 95:
#line 311 "parser.y"
    { (yyval.node) = (yyvsp[(1) - (1)].node); ;}
    break;

  case 96:
#line 312 "parser.y"
    { (yyval.node) = (yyvsp[(1) - (1)].node); ;}
    break;

  case 97:
#line 313 "parser.y"
    { (yyval.node) = (yyvsp[(1) - (1)].node); ;}
    break;

  case 98:
#line 314 "parser.y"
    { (yyval.node) = (yyvsp[(1) - (1)].node); ;}
    break;

  case 99:
#line 315 "parser.y"
    { (yyval.node) = (yyvsp[(1) - (1)].node); ;}
    break;

  case 100:
#line 316 "parser.y"
    { (yyval.node) = (yyvsp[(1) - (1)].node); ;}
    break;

  case 101:
#line 320 "parser.y"
    { (yyval.node) = create_labeled_statement_node((yyvsp[(1) - (3)].str), (yyvsp[(3) - (3)].node)); ;}
    break;

  case 102:
#line 321 "parser.y"
    { (yyval.node) = create_case_statement_node((yyvsp[(2) - (4)].node), (yyvsp[(4) - (4)].node)); ;}
    break;

  case 103:
#line 322 "parser.y"
    { (yyval.node) = create_default_statement_node((yyvsp[(3) - (3)].node)); ;}
    break;

  case 104:
#line 326 "parser.y"
    { (yyval.node) = create_compound_statement_node(NULL); ;}
    break;

  case 105:
#line 327 "parser.y"
    { (yyval.node) = create_compound_statement_node((yyvsp[(2) - (3)].list)); ;}
    break;

  case 106:
#line 332 "parser.y"
    { (yyval.list) = create_list_node((yyvsp[(1) - (1)].node)); ;}
    break;

  case 107:
#line 334 "parser.y"
    { (yyval.list) = append_to_list((yyvsp[(1) - (2)].list), (yyvsp[(2) - (2)].node)); ;}
    break;

  case 108:
#line 338 "parser.y"
    { (yyval.node) = (yyvsp[(1) - (1)].node); ;}
    break;

  case 109:
#line 339 "parser.y"
    { (yyval.node) = (yyvsp[(1) - (1)].node); ;}
    break;

  case 110:
#line 343 "parser.y"
    { (yyval.node) = create_expression_statement_node(NULL); ;}
    break;

  case 111:
#line 344 "parser.y"
    { (yyval.node) = create_expression_statement_node((yyvsp[(1) - (2)].node)); ;}
    break;

  case 112:
#line 348 "parser.y"
    { (yyval.node) = create_if_statement_node((yyvsp[(3) - (5)].node), (yyvsp[(5) - (5)].node), NULL); ;}
    break;

  case 113:
#line 349 "parser.y"
    { (yyval.node) = create_if_statement_node((yyvsp[(3) - (7)].node), (yyvsp[(5) - (7)].node), (yyvsp[(7) - (7)].node)); ;}
    break;

  case 114:
#line 350 "parser.y"
    { (yyval.node) = create_switch_statement_node((yyvsp[(3) - (5)].node), (yyvsp[(5) - (5)].node)); ;}
    break;

  case 115:
#line 354 "parser.y"
    { (yyval.node) = create_while_statement_node((yyvsp[(3) - (5)].node), (yyvsp[(5) - (5)].node)); ;}
    break;

  case 116:
#line 355 "parser.y"
    { (yyval.node) = create_do_while_statement_node((yyvsp[(2) - (7)].node), (yyvsp[(5) - (7)].node)); ;}
    break;

  case 117:
#line 356 "parser.y"
    { (yyval.node) = create_for_statement_node((yyvsp[(3) - (6)].node), (yyvsp[(4) - (6)].node), NULL, (yyvsp[(6) - (6)].node)); ;}
    break;

  case 118:
#line 357 "parser.y"
    { (yyval.node) = create_for_statement_node((yyvsp[(3) - (7)].node), (yyvsp[(4) - (7)].node), (yyvsp[(5) - (7)].node), (yyvsp[(7) - (7)].node)); ;}
    break;

  case 119:
#line 358 "parser.y"
    { (yyval.node) = create_for_statement_node((yyvsp[(3) - (6)].node), (yyvsp[(4) - (6)].node), NULL, (yyvsp[(6) - (6)].node)); ;}
    break;

  case 120:
#line 359 "parser.y"
    { (yyval.node) = create_for_statement_node((yyvsp[(3) - (7)].node), (yyvsp[(4) - (7)].node), (yyvsp[(5) - (7)].node), (yyvsp[(7) - (7)].node)); ;}
    break;

  case 121:
#line 363 "parser.y"
    { (yyval.node) = create_goto_statement_node((yyvsp[(2) - (3)].str)); ;}
    break;

  case 122:
#line 364 "parser.y"
    { (yyval.node) = create_node(NODE_CONTINUE_STATEMENT); ;}
    break;

  case 123:
#line 365 "parser.y"
    { (yyval.node) = create_node(NODE_BREAK_STATEMENT); ;}
    break;

  case 124:
#line 366 "parser.y"
    { (yyval.node) = create_return_statement_node(NULL); ;}
    break;

  case 125:
#line 367 "parser.y"
    { (yyval.node) = create_return_statement_node((yyvsp[(2) - (3)].node)); ;}
    break;

  case 126:
#line 372 "parser.y"
    { (yyval.node) = create_declaration_node((yyvsp[(1) - (2)].list), NULL); ;}
    break;

  case 127:
#line 373 "parser.y"
    { (yyval.node) = create_declaration_node((yyvsp[(1) - (3)].list), (yyvsp[(2) - (3)].list)); ;}
    break;

  case 128:
#line 377 "parser.y"
    { (yyval.list) = create_list_node((yyvsp[(1) - (1)].node)); ;}
    break;

  case 129:
#line 378 "parser.y"
    { (yyval.list) = append_to_list((yyvsp[(1) - (2)].list), (yyvsp[(2) - (2)].node)); ;}
    break;

  case 130:
#line 382 "parser.y"
    { (yyval.node) = (yyvsp[(1) - (1)].node); ;}
    break;

  case 131:
#line 383 "parser.y"
    { (yyval.node) = (yyvsp[(1) - (1)].node); ;}
    break;

  case 132:
#line 384 "parser.y"
    { (yyval.node) = (yyvsp[(1) - (1)].node); ;}
    break;

  case 133:
#line 389 "parser.y"
    { (yyval.list) = create_list_node((yyvsp[(1) - (1)].node)); ;}
    break;

  case 134:
#line 391 "parser.y"
    { (yyval.list) = append_to_list((yyvsp[(1) - (3)].list), (yyvsp[(3) - (3)].node)); ;}
    break;

  case 135:
#line 395 "parser.y"
    { (yyval.node) = create_init_declarator_node((yyvsp[(1) - (1)].node), NULL); ;}
    break;

  case 136:
#line 396 "parser.y"
    { (yyval.node) = create_init_declarator_node((yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node)); ;}
    break;

  case 137:
#line 400 "parser.y"
    { (yyval.node) = create_specifier_node(TYPEDEF); ;}
    break;

  case 138:
#line 401 "parser.y"
    { (yyval.node) = create_specifier_node(EXTERN); ;}
    break;

  case 139:
#line 402 "parser.y"
    { (yyval.node) = create_specifier_node(STATIC); ;}
    break;

  case 140:
#line 403 "parser.y"
    { (yyval.node) = create_specifier_node(AUTO); ;}
    break;

  case 141:
#line 404 "parser.y"
    { (yyval.node) = create_specifier_node(REGISTER); ;}
    break;

  case 142:
#line 408 "parser.y"
    { (yyval.node) = create_specifier_node(VOID); ;}
    break;

  case 143:
#line 409 "parser.y"
    { (yyval.node) = create_specifier_node(CHAR); ;}
    break;

  case 144:
#line 410 "parser.y"
    { (yyval.node) = create_specifier_node(SHORT); ;}
    break;

  case 145:
#line 411 "parser.y"
    { (yyval.node) = create_specifier_node(INT); ;}
    break;

  case 146:
#line 412 "parser.y"
    { (yyval.node) = create_specifier_node(LONG); ;}
    break;

  case 147:
#line 413 "parser.y"
    { (yyval.node) = create_specifier_node(FLOAT); ;}
    break;

  case 148:
#line 414 "parser.y"
    { (yyval.node) = create_specifier_node(DOUBLE); ;}
    break;

  case 149:
#line 415 "parser.y"
    { (yyval.node) = create_specifier_node(SIGNED); ;}
    break;

  case 150:
#line 416 "parser.y"
    { (yyval.node) = create_specifier_node(UNSIGNED); ;}
    break;

  case 151:
#line 417 "parser.y"
    { (yyval.node) = create_specifier_node(BOOL); ;}
    break;

  case 152:
#line 418 "parser.y"
    { (yyval.node) = create_specifier_node(STRING); ;}
    break;

  case 153:
#line 419 "parser.y"
    { (yyval.node) = create_specifier_node(FILE_TYPE); ;}
    break;

  case 154:
#line 420 "parser.y"
    { (yyval.node) = (yyvsp[(1) - (1)].node); ;}
    break;

  case 155:
#line 421 "parser.y"
    { (yyval.node) = (yyvsp[(1) - (1)].node); ;}
    break;

  case 156:
#line 422 "parser.y"
    { (yyval.node) = (yyvsp[(1) - (1)].node); ;}
    break;

  case 157:
#line 423 "parser.y"
    { (yyval.node) = create_typename_node((yyvsp[(1) - (1)].str)); ;}
    break;

  case 158:
#line 427 "parser.y"
    { (yyval.node) = create_specifier_node(CONST); ;}
    break;

  case 159:
#line 428 "parser.y"
    { (yyval.node) = create_specifier_node(VOLATILE); ;}
    break;

  case 160:
#line 432 "parser.y"
    { add_typename((yyvsp[(2) - (5)].str)); (yyval.node) = create_struct_union_node(STRUCT, (yyvsp[(2) - (5)].str), (yyvsp[(4) - (5)].list)); ;}
    break;

  case 161:
#line 433 "parser.y"
    { add_typename((yyvsp[(2) - (5)].str)); (yyval.node) = create_struct_union_node(UNION, (yyvsp[(2) - (5)].str), (yyvsp[(4) - (5)].list)); ;}
    break;

  case 162:
#line 434 "parser.y"
    { (yyval.node) = create_struct_union_node(STRUCT, NULL, (yyvsp[(3) - (4)].list)); ;}
    break;

  case 163:
#line 435 "parser.y"
    { (yyval.node) = create_struct_union_node(UNION, NULL, (yyvsp[(3) - (4)].list)); ;}
    break;

  case 164:
#line 436 "parser.y"
    { (yyval.node) = create_struct_union_node(STRUCT, (yyvsp[(2) - (2)].str), NULL); ;}
    break;

  case 165:
#line 437 "parser.y"
    { (yyval.node) = create_struct_union_node(UNION, (yyvsp[(2) - (2)].str), NULL); ;}
    break;

  case 166:
#line 442 "parser.y"
    { (yyval.list) = create_list_node((yyvsp[(1) - (1)].node)); ;}
    break;

  case 167:
#line 444 "parser.y"
    { (yyval.list) = append_to_list((yyvsp[(1) - (2)].list), (yyvsp[(2) - (2)].node)); ;}
    break;

  case 168:
#line 448 "parser.y"
    { (yyval.node) = create_declaration_node((yyvsp[(1) - (3)].list), (yyvsp[(2) - (3)].list)); ;}
    break;

  case 169:
#line 453 "parser.y"
    { (yyval.list) = create_list_node((yyvsp[(1) - (1)].node)); ;}
    break;

  case 170:
#line 455 "parser.y"
    { (yyval.list) = append_to_list((yyvsp[(1) - (3)].list), (yyvsp[(3) - (3)].node)); ;}
    break;

  case 172:
#line 460 "parser.y"
    { (yyval.node) = (yyvsp[(1) - (3)].node); ;}
    break;

  case 173:
#line 464 "parser.y"
    { (yyval.node) = create_enum_specifier_node(NULL, (yyvsp[(3) - (4)].list)); ;}
    break;

  case 174:
#line 465 "parser.y"
    { add_typename((yyvsp[(2) - (5)].str)); (yyval.node) = create_enum_specifier_node((yyvsp[(2) - (5)].str), (yyvsp[(4) - (5)].list)); ;}
    break;

  case 175:
#line 466 "parser.y"
    { (yyval.node) = create_enum_specifier_node((yyvsp[(2) - (2)].str), NULL); ;}
    break;

  case 176:
#line 471 "parser.y"
    { (yyval.list) = create_list_node((yyvsp[(1) - (1)].node)); ;}
    break;

  case 177:
#line 473 "parser.y"
    { (yyval.list) = append_to_list((yyvsp[(1) - (3)].list), (yyvsp[(3) - (3)].node)); ;}
    break;

  case 178:
#line 477 "parser.y"
    { (yyval.node) = create_identifier_node((yyvsp[(1) - (1)].str)); ;}
    break;

  case 179:
#line 478 "parser.y"
    { (yyval.node) = create_assignment_node(create_identifier_node((yyvsp[(1) - (3)].str)), '=', (yyvsp[(3) - (3)].node)); ;}
    break;

  case 180:
#line 482 "parser.y"
    { add_typename((yyvsp[(2) - (6)].node)->data.stringValue); (yyval.node) = create_class_node((yyvsp[(2) - (6)].node)->data.stringValue, (yyvsp[(3) - (6)].list), (yyvsp[(5) - (6)].list)); ;}
    break;

  case 181:
#line 486 "parser.y"
    { (yyval.node) = create_identifier_node((yyvsp[(1) - (1)].str)); ;}
    break;

  case 182:
#line 487 "parser.y"
    { (yyval.node) = create_typename_node((yyvsp[(1) - (1)].str)); ;}
    break;

  case 183:
#line 491 "parser.y"
    { (yyval.list) = NULL; ;}
    break;

  case 184:
#line 492 "parser.y"
    { (yyval.list) = create_list_node((yyvsp[(2) - (2)].node)); ;}
    break;

  case 185:
#line 493 "parser.y"
    { (yyval.list) = append_to_list((yyvsp[(1) - (3)].list), (yyvsp[(3) - (3)].node)); ;}
    break;

  case 186:
#line 497 "parser.y"
    { (yyval.node) = create_base_class_node((yyvsp[(1) - (2)].node)->data.specifier, (yyvsp[(2) - (2)].node)); ;}
    break;

  case 187:
#line 498 "parser.y"
    { (yyval.node) = create_base_class_node(PRIVATE, (yyvsp[(1) - (1)].node)); ;}
    break;

  case 188:
#line 502 "parser.y"
    { (yyval.node) = create_access_specifier_node(PUBLIC); ;}
    break;

  case 189:
#line 503 "parser.y"
    { (yyval.node) = create_access_specifier_node(PRIVATE); ;}
    break;

  case 190:
#line 504 "parser.y"
    { (yyval.node) = create_access_specifier_node(PROTECTED); ;}
    break;

  case 191:
#line 508 "parser.y"
    { (yyval.list) = NULL; ;}
    break;

  case 192:
#line 509 "parser.y"
    { (yyval.list) = (yyvsp[(1) - (2)].list) ? append_to_list((yyvsp[(1) - (2)].list), (yyvsp[(2) - (2)].node)) : create_list_node((yyvsp[(2) - (2)].node)); ;}
    break;

  case 195:
#line 515 "parser.y"
    { (yyval.node) = (yyvsp[(1) - (2)].node); ;}
    break;

  case 196:
#line 520 "parser.y"
    { (yyval.node) = create_pointer_declarator_node((yyvsp[(1) - (2)].node), (yyvsp[(2) - (2)].node)); ;}
    break;

  case 198:
#line 526 "parser.y"
    { (yyval.node) = create_identifier_node((yyvsp[(1) - (1)].str)); ;}
    break;

  case 199:
#line 527 "parser.y"
    { (yyval.node) = (yyvsp[(2) - (3)].node); ;}
    break;

  case 200:
#line 528 "parser.y"
    { (yyval.node) = create_array_declarator_node((yyvsp[(1) - (4)].node), (yyvsp[(3) - (4)].node)); ;}
    break;

  case 201:
#line 529 "parser.y"
    { (yyval.node) = create_array_declarator_node((yyvsp[(1) - (3)].node), NULL); ;}
    break;

  case 202:
#line 530 "parser.y"
    { (yyval.node) = create_function_declarator_node((yyvsp[(1) - (4)].node), (yyvsp[(3) - (4)].list)); ;}
    break;

  case 203:
#line 531 "parser.y"
    { (yyval.node) = create_function_declarator_node((yyvsp[(1) - (3)].node), NULL); ;}
    break;

  case 204:
#line 536 "parser.y"
    { (yyval.node) = create_pointer_node((yyvsp[(2) - (2)].list), NULL); ;}
    break;

  case 205:
#line 538 "parser.y"
    { (yyval.node) = create_pointer_node((yyvsp[(3) - (3)].list), (yyvsp[(1) - (3)].node)); ;}
    break;

  case 206:
#line 542 "parser.y"
    { (yyval.list) = NULL; ;}
    break;

  case 207:
#line 543 "parser.y"
    { (yyval.list) = (yyvsp[(1) - (1)].list); ;}
    break;

  case 208:
#line 548 "parser.y"
    { (yyval.list) = create_list_node((yyvsp[(1) - (1)].node)); ;}
    break;

  case 209:
#line 550 "parser.y"
    { (yyval.list) = append_to_list((yyvsp[(1) - (2)].list), (yyvsp[(2) - (2)].node)); ;}
    break;

  case 211:
#line 555 "parser.y"
    { (yyval.list) = (yyvsp[(1) - (3)].list); ;}
    break;

  case 212:
#line 560 "parser.y"
    { (yyval.list) = create_list_node((yyvsp[(1) - (1)].node)); ;}
    break;

  case 213:
#line 562 "parser.y"
    { (yyval.list) = append_to_list((yyvsp[(1) - (3)].list), (yyvsp[(3) - (3)].node)); ;}
    break;

  case 214:
#line 567 "parser.y"
    { (yyval.node) = create_parameter_declaration_node((yyvsp[(1) - (2)].list), (yyvsp[(2) - (2)].node)); ;}
    break;

  case 215:
#line 569 "parser.y"
    { (yyval.node) = create_parameter_declaration_node((yyvsp[(1) - (2)].list), (yyvsp[(2) - (2)].node)); ;}
    break;

  case 216:
#line 571 "parser.y"
    { (yyval.node) = create_parameter_declaration_node((yyvsp[(1) - (1)].list), NULL); ;}
    break;

  case 217:
#line 575 "parser.y"
    { (yyval.node) = create_declaration_node((yyvsp[(1) - (2)].list), create_list_node((yyvsp[(2) - (2)].node))); ;}
    break;

  case 218:
#line 576 "parser.y"
    { (yyval.node) = create_declaration_node((yyvsp[(1) - (1)].list), NULL); ;}
    break;

  case 220:
#line 581 "parser.y"
    { (yyval.node) = create_pointer_declarator_node((yyvsp[(1) - (2)].node), (yyvsp[(2) - (2)].node)); ;}
    break;

  case 222:
#line 586 "parser.y"
    { (yyval.node) = (yyvsp[(2) - (3)].node); ;}
    break;

  case 223:
#line 587 "parser.y"
    { (yyval.node) = create_array_declarator_node(NULL, NULL); ;}
    break;

  case 224:
#line 588 "parser.y"
    { (yyval.node) = create_array_declarator_node(NULL, (yyvsp[(2) - (3)].node)); ;}
    break;

  case 225:
#line 589 "parser.y"
    { (yyval.node) = create_array_declarator_node((yyvsp[(1) - (3)].node), NULL); ;}
    break;

  case 226:
#line 590 "parser.y"
    { (yyval.node) = create_array_declarator_node((yyvsp[(1) - (4)].node), (yyvsp[(3) - (4)].node)); ;}
    break;

  case 227:
#line 591 "parser.y"
    { (yyval.node) = create_function_declarator_node(NULL, NULL); ;}
    break;

  case 228:
#line 592 "parser.y"
    { (yyval.node) = create_function_declarator_node(NULL, (yyvsp[(2) - (3)].list)); ;}
    break;

  case 229:
#line 593 "parser.y"
    { (yyval.node) = create_function_declarator_node((yyvsp[(1) - (3)].node), NULL); ;}
    break;

  case 230:
#line 594 "parser.y"
    { (yyval.node) = create_function_declarator_node((yyvsp[(1) - (4)].node), (yyvsp[(3) - (4)].list)); ;}
    break;

  case 232:
#line 599 "parser.y"
    { (yyval.node) = create_node(NODE_INITIALIZER_LIST); (yyval.node)->data.items_list = (yyvsp[(2) - (3)].list); ;}
    break;

  case 233:
#line 600 "parser.y"
    { (yyval.node) = create_node(NODE_INITIALIZER_LIST); (yyval.node)->data.items_list = (yyvsp[(2) - (4)].list); ;}
    break;

  case 234:
#line 605 "parser.y"
    { (yyval.list) = create_list_node((yyvsp[(1) - (1)].node)); ;}
    break;

  case 235:
#line 607 "parser.y"
    { (yyval.list) = append_to_list((yyvsp[(1) - (3)].list), (yyvsp[(3) - (3)].node)); ;}
    break;


/* Line 1267 of yacc.c.  */
#line 3367 "parser.tab.c"
      default: break;
    }
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
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
      {
	YYSIZE_T yysize = yysyntax_error (0, yystate, yychar);
	if (yymsg_alloc < yysize && yymsg_alloc < YYSTACK_ALLOC_MAXIMUM)
	  {
	    YYSIZE_T yyalloc = 2 * yysize;
	    if (! (yysize <= yyalloc && yyalloc <= YYSTACK_ALLOC_MAXIMUM))
	      yyalloc = YYSTACK_ALLOC_MAXIMUM;
	    if (yymsg != yymsgbuf)
	      YYSTACK_FREE (yymsg);
	    yymsg = (char *) YYSTACK_ALLOC (yyalloc);
	    if (yymsg)
	      yymsg_alloc = yyalloc;
	    else
	      {
		yymsg = yymsgbuf;
		yymsg_alloc = sizeof yymsgbuf;
	      }
	  }

	if (0 < yysize && yysize <= yymsg_alloc)
	  {
	    (void) yysyntax_error (yymsg, yystate, yychar);
	    yyerror (yymsg);
	  }
	else
	  {
	    yyerror (YY_("syntax error"));
	    if (yysize != 0)
	      goto yyexhaustedlab;
	  }
      }
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse look-ahead token after an
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

  /* Else will try to reuse look-ahead token after shifting the error
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
      if (yyn != YYPACT_NINF)
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

  if (yyn == YYFINAL)
    YYACCEPT;

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

#ifndef yyoverflow
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEOF && yychar != YYEMPTY)
     yydestruct ("Cleanup: discarding lookahead",
		 yytoken, &yylval);
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


#line 609 "parser.y"


void yyerror(const char *s) {
    fprintf(stderr, "Syntax Error on line %d near '%s': %s\n", yylineno, yytext, s);
    error_flag = 0;
}

int main(int argc, char **argv) {
    if (argc > 1) {
        yyin = fopen(argv[1], "r");
        if (!yyin) {
            perror(argv[1]);
            return 1;
        }
    } else {
        yyin = stdin;
    }

    yyparse();

    if (error_flag) {
        printf("\n--- Parsing Successful ---\n");
        printf("\n--- Abstract Syntax Tree ---\n\n");
        if (root) {
            print_ast(root, 0);
        } else {
            printf("AST root is null.\n");
        }
    } else {
        printf("\n--- Parsing Failed ---\n");
    }
    
    if (yyin != stdin) {
        fclose(yyin);
    }
    
    return error_flag ? 0 : 1;
}

