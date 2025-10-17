
/* A Bison parser, made by GNU Bison 2.4.1.  */

/* Skeleton implementation for Bison's Yacc-like parsers in C
   
      Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.
   
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
#define YYBISON_VERSION "2.4.1"

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

/* Line 189 of yacc.c  */
#line 1 "parser.y"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "ast.h"
#include "semantic.h"
#include "symbol_table.h"
#include "tac.h"

ASTNode *root = NULL;
int parse_errors = 0;

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



/* Line 189 of yacc.c  */
#line 116 "parser.tab.c"

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



#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{

/* Line 214 of yacc.c  */
#line 43 "parser.y"

    char *str;
    struct ASTNode *node;
    struct ASTNodeList *list;
    int token;



/* Line 214 of yacc.c  */
#line 244 "parser.tab.c"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif


/* Copy the second part of user declarations.  */


/* Line 264 of yacc.c  */
#line 256 "parser.tab.c"

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
# if YYENABLE_NLS
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

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  120
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   2586

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  108
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  75
/* YYNRULES -- Number of rules.  */
#define YYNRULES  249
/* YYNRULES -- Number of states.  */
#define YYNSTATES  427

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
     102,   103,    94,    92,   104,    93,    99,    95,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    86,   107,
      90,    84,    91,    85,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   100,     2,   101,    88,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   105,    87,   106,    98,     2,     2,     2,
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
       0,     0,     3,     5,     7,    10,    12,    14,    16,    20,
      22,    24,    26,    28,    30,    32,    34,    38,    40,    42,
      47,    51,    56,    60,    64,    67,    70,    72,    76,    78,
      81,    84,    87,    90,    93,    96,    99,   102,   105,   110,
     112,   114,   116,   121,   123,   127,   131,   135,   137,   141,
     145,   147,   151,   155,   157,   161,   165,   169,   173,   175,
     179,   183,   185,   189,   191,   195,   197,   201,   203,   207,
     209,   213,   215,   221,   223,   227,   229,   231,   233,   235,
     237,   239,   241,   243,   245,   247,   249,   251,   255,   257,
     262,   265,   269,   275,   279,   284,   287,   291,   298,   304,
     306,   308,   310,   312,   314,   316,   320,   325,   329,   332,
     336,   338,   341,   343,   345,   347,   350,   356,   364,   370,
     376,   382,   390,   397,   405,   412,   420,   424,   427,   430,
     433,   437,   440,   444,   448,   450,   453,   455,   457,   459,
     461,   463,   467,   469,   473,   475,   477,   479,   481,   483,
     485,   487,   489,   491,   493,   495,   497,   499,   501,   503,
     505,   507,   509,   511,   513,   515,   517,   519,   525,   531,
     536,   541,   544,   547,   549,   552,   556,   558,   562,   564,
     568,   573,   579,   582,   584,   588,   590,   594,   601,   603,
     605,   606,   609,   613,   616,   618,   620,   622,   624,   625,
     628,   630,   632,   635,   638,   641,   645,   649,   652,   655,
     658,   660,   662,   664,   668,   673,   677,   682,   686,   689,
     692,   693,   695,   697,   700,   702,   706,   708,   712,   715,
     718,   720,   723,   725,   727,   730,   732,   736,   739,   743,
     747,   752,   755,   759,   763,   768,   770,   774,   779,   781
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int16 yyrhs[] =
{
     109,     0,    -1,   110,    -1,   111,    -1,   110,   111,    -1,
     112,    -1,   145,    -1,   141,    -1,   146,   170,   138,    -1,
       3,    -1,   169,    -1,     4,    -1,     5,    -1,     6,    -1,
       7,    -1,     8,    -1,   102,   131,   103,    -1,   135,    -1,
     113,    -1,   114,   100,   131,   101,    -1,   114,   102,   103,
      -1,   114,   102,   115,   103,    -1,   114,    99,     3,    -1,
     114,    11,     3,    -1,   114,    12,    -1,   114,    13,    -1,
     129,    -1,   115,   104,   129,    -1,   114,    -1,    12,   116,
      -1,    13,   116,    -1,    89,   117,    -1,    94,   117,    -1,
      92,   117,    -1,    93,   117,    -1,    98,   117,    -1,    97,
     117,    -1,    10,   116,    -1,    10,   102,   178,   103,    -1,
     133,    -1,   134,    -1,   116,    -1,   102,   178,   103,   117,
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
      -1,   131,   104,   129,    -1,   128,    -1,    70,   178,   102,
     103,    -1,    70,   178,    -1,    70,   178,   181,    -1,    79,
      70,   178,   102,   103,    -1,    79,    70,   178,    -1,    79,
      70,   178,   181,    -1,    67,   117,    -1,    79,    67,   117,
      -1,   100,   101,   102,   175,   103,   138,    -1,   100,   101,
     102,   103,   138,    -1,   137,    -1,   138,    -1,   141,    -1,
     142,    -1,   143,    -1,   144,    -1,     3,    86,   136,    -1,
      54,   132,    86,   136,    -1,    55,    86,   136,    -1,   105,
     106,    -1,   105,   139,   106,    -1,   140,    -1,   139,   140,
      -1,   145,    -1,   136,    -1,   107,    -1,   131,   107,    -1,
      56,   102,   131,   103,   136,    -1,    56,   102,   131,   103,
     136,    57,   136,    -1,    58,   102,   131,   103,   136,    -1,
      59,   102,   131,   103,   136,    -1,    82,   102,   131,   103,
     136,    -1,    60,   136,    59,   102,   131,   103,   107,    -1,
      61,   102,   141,   141,   103,   136,    -1,    61,   102,   141,
     141,   131,   103,   136,    -1,    61,   102,   145,   141,   103,
     136,    -1,    61,   102,   145,   141,   131,   103,   136,    -1,
      62,     3,   107,    -1,    63,   107,    -1,    64,   107,    -1,
      65,   107,    -1,    65,   131,   107,    -1,   146,   107,    -1,
     146,   148,   107,    -1,    66,     3,   107,    -1,   147,    -1,
     146,   147,    -1,   150,    -1,   151,    -1,   152,    -1,    68,
      -1,   149,    -1,   148,   104,   149,    -1,   170,    -1,   170,
      84,   181,    -1,    32,    -1,    33,    -1,    34,    -1,    35,
      -1,    36,    -1,    48,    -1,    38,    -1,    39,    -1,    40,
      -1,    41,    -1,    44,    -1,    45,    -1,    42,    -1,    43,
      -1,    49,    -1,    37,    -1,    83,    -1,   153,    -1,   158,
      -1,   161,    -1,     9,    -1,    46,    -1,    47,    -1,    50,
       3,   105,   154,   106,    -1,    51,     3,   105,   154,   106,
      -1,    50,   105,   154,   106,    -1,    51,   105,   154,   106,
      -1,    50,     9,    -1,    51,     9,    -1,   155,    -1,   154,
     155,    -1,   146,   156,   107,    -1,   157,    -1,   156,   104,
     157,    -1,   170,    -1,   170,    86,   132,    -1,    52,   105,
     159,   106,    -1,    52,     3,   105,   159,   106,    -1,    52,
       9,    -1,   160,    -1,   159,   104,   160,    -1,     3,    -1,
       3,    84,   132,    -1,    66,   162,   163,   105,   166,   106,
      -1,     3,    -1,     9,    -1,    -1,    86,   164,    -1,   163,
     104,   164,    -1,   165,   162,    -1,   162,    -1,    74,    -1,
      72,    -1,    73,    -1,    -1,   166,   167,    -1,   145,    -1,
     112,    -1,   165,    86,    -1,     3,    79,    -1,     9,    79,
      -1,   168,     3,    79,    -1,   168,     9,    79,    -1,   168,
       3,    -1,   168,     9,    -1,   172,   170,    -1,   171,    -1,
       3,    -1,   169,    -1,   102,   170,   103,    -1,   171,   100,
     132,   101,    -1,   171,   100,   101,    -1,   171,   102,   175,
     103,    -1,   171,   102,   103,    -1,    94,   173,    -1,    89,
     173,    -1,    -1,   174,    -1,   152,    -1,   174,   152,    -1,
     176,    -1,   176,   104,    53,    -1,   177,    -1,   176,   104,
     177,    -1,   146,   170,    -1,   146,   179,    -1,   146,    -1,
     146,   179,    -1,   146,    -1,   172,    -1,   172,   180,    -1,
     180,    -1,   102,   179,   103,    -1,   100,   101,    -1,   100,
     132,   101,    -1,   180,   100,   101,    -1,   180,   100,   132,
     101,    -1,   102,   103,    -1,   102,   175,   103,    -1,   180,
     102,   103,    -1,   180,   102,   175,   103,    -1,   129,    -1,
     105,   182,   106,    -1,   105,   182,   104,   106,    -1,   181,
      -1,   182,   104,   181,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   129,   129,   133,   134,   138,   139,   140,   144,   149,
     150,   151,   152,   153,   154,   155,   156,   157,   161,   162,
     163,   164,   165,   166,   168,   169,   173,   175,   180,   181,
     182,   183,   184,   185,   186,   187,   188,   189,   190,   191,
     192,   205,   206,   210,   211,   212,   213,   217,   218,   219,
     223,   224,   225,   229,   230,   231,   232,   233,   237,   238,
     239,   243,   244,   248,   249,   253,   254,   258,   259,   263,
     264,   268,   269,   273,   274,   278,   279,   280,   281,   282,
     283,   284,   285,   286,   287,   288,   292,   293,   297,   302,
     303,   304,   305,   306,   307,   311,   312,   316,   317,   322,
     323,   324,   325,   326,   327,   331,   332,   333,   337,   338,
     342,   344,   349,   350,   354,   355,   359,   360,   361,   365,
     367,   368,   369,   370,   371,   372,   376,   377,   378,   379,
     380,   385,   386,   387,   392,   393,   397,   398,   399,   400,
     404,   406,   411,   412,   416,   417,   418,   419,   420,   424,
     425,   426,   427,   428,   429,   430,   431,   432,   433,   434,
     435,   436,   437,   438,   439,   443,   444,   448,   449,   450,
     451,   452,   453,   457,   459,   464,   468,   470,   475,   477,
     481,   482,   483,   487,   489,   494,   495,   499,   503,   504,
     508,   509,   510,   514,   515,   519,   520,   521,   525,   526,
     530,   531,   532,   537,   538,   539,   540,   544,   545,   549,
     559,   563,   564,   565,   566,   567,   568,   569,   574,   575,
     579,   580,   584,   586,   591,   592,   596,   598,   603,   605,
     607,   612,   613,   617,   618,   619,   623,   624,   625,   626,
     627,   628,   629,   630,   631,   635,   636,   637,   641,   643
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
  "'/'", "'%'", "'!'", "'~'", "'.'", "'['", "']'", "'('", "')'", "','",
  "'{'", "'}'", "';'", "$accept", "program", "translation_unit",
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
  "class_member", "nested_name_specifier", "qualified_id", "declarator",
  "direct_declarator", "ptr_operator", "type_qualifier_list_opt",
  "type_qualifier_list", "parameter_type_list", "parameter_list",
  "parameter_declaration", "type_name", "abstract_declarator",
  "direct_abstract_declarator", "initializer", "initializer_list", 0
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
      91,    93,    40,    41,    44,   123,   125,    59
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,   108,   109,   110,   110,   111,   111,   111,   112,   113,
     113,   113,   113,   113,   113,   113,   113,   113,   114,   114,
     114,   114,   114,   114,   114,   114,   115,   115,   116,   116,
     116,   116,   116,   116,   116,   116,   116,   116,   116,   116,
     116,   117,   117,   118,   118,   118,   118,   119,   119,   119,
     120,   120,   120,   121,   121,   121,   121,   121,   122,   122,
     122,   123,   123,   124,   124,   125,   125,   126,   126,   127,
     127,   128,   128,   129,   129,   130,   130,   130,   130,   130,
     130,   130,   130,   130,   130,   130,   131,   131,   132,   133,
     133,   133,   133,   133,   133,   134,   134,   135,   135,   136,
     136,   136,   136,   136,   136,   137,   137,   137,   138,   138,
     139,   139,   140,   140,   141,   141,   142,   142,   142,   143,
     143,   143,   143,   143,   143,   143,   144,   144,   144,   144,
     144,   145,   145,   145,   146,   146,   147,   147,   147,   147,
     148,   148,   149,   149,   150,   150,   150,   150,   150,   151,
     151,   151,   151,   151,   151,   151,   151,   151,   151,   151,
     151,   151,   151,   151,   151,   152,   152,   153,   153,   153,
     153,   153,   153,   154,   154,   155,   156,   156,   157,   157,
     158,   158,   158,   159,   159,   160,   160,   161,   162,   162,
     163,   163,   163,   164,   164,   165,   165,   165,   166,   166,
     167,   167,   167,   168,   168,   168,   168,   169,   169,   170,
     170,   171,   171,   171,   171,   171,   171,   171,   172,   172,
     173,   173,   174,   174,   175,   175,   176,   176,   177,   177,
     177,   178,   178,   179,   179,   179,   180,   180,   180,   180,
     180,   180,   180,   180,   180,   181,   181,   181,   182,   182
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     1,     2,     1,     1,     1,     3,     1,
       1,     1,     1,     1,     1,     1,     3,     1,     1,     4,
       3,     4,     3,     3,     2,     2,     1,     3,     1,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     4,     1,
       1,     1,     4,     1,     3,     3,     3,     1,     3,     3,
       1,     3,     3,     1,     3,     3,     3,     3,     1,     3,
       3,     1,     3,     1,     3,     1,     3,     1,     3,     1,
       3,     1,     5,     1,     3,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     3,     1,     4,
       2,     3,     5,     3,     4,     2,     3,     6,     5,     1,
       1,     1,     1,     1,     1,     3,     4,     3,     2,     3,
       1,     2,     1,     1,     1,     2,     5,     7,     5,     5,
       5,     7,     6,     7,     6,     7,     3,     2,     2,     2,
       3,     2,     3,     3,     1,     2,     1,     1,     1,     1,
       1,     3,     1,     3,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     5,     5,     4,
       4,     2,     2,     1,     2,     3,     1,     3,     1,     3,
       4,     5,     2,     1,     3,     1,     3,     6,     1,     1,
       0,     2,     3,     2,     1,     1,     1,     1,     0,     2,
       1,     1,     2,     2,     2,     3,     3,     2,     2,     2,
       1,     1,     1,     3,     4,     3,     4,     3,     2,     2,
       0,     1,     1,     2,     1,     3,     1,     3,     2,     2,
       1,     2,     1,     1,     2,     1,     3,     2,     3,     3,
       4,     2,     3,     3,     4,     1,     3,     4,     1,     3
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,     9,    11,    12,    13,    14,    15,   164,     0,     0,
       0,   144,   145,   146,   147,   148,   159,   150,   151,   152,
     153,   156,   157,   154,   155,   165,   166,   149,   158,     0,
       0,     0,     0,     0,   139,     0,     0,   160,     0,     0,
       0,     0,     0,     0,     0,     0,   114,     0,     2,     3,
       5,    18,    28,    41,    43,    47,    50,    53,    58,    61,
      63,    65,    67,    69,    71,    73,    86,     0,    39,    40,
      17,     7,     6,     0,   134,   136,   137,   138,   161,   162,
     163,     0,    10,   203,   204,     0,     0,    37,     0,    29,
      30,     0,   171,     0,     0,   172,     0,     0,   182,     0,
     188,   189,   190,    41,    95,   164,     0,   232,    90,     0,
       0,    31,    33,    34,    32,    36,    35,     0,     0,     0,
       1,     4,     0,    24,    25,     0,     0,     0,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    75,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     115,   211,   220,   220,     0,   131,   135,     0,   140,   212,
     142,   210,     0,   207,   208,     0,     0,     0,     0,   173,
       0,     0,     0,   185,     0,   183,   133,     0,     0,   188,
       0,     0,   233,   231,   235,     0,     0,   245,    91,    96,
      93,     0,    16,     0,    23,    22,     0,    20,     0,    26,
      74,    44,    45,    46,    48,    49,    51,    52,    56,    57,
      54,    55,    59,    60,    62,    64,    66,    68,    70,     0,
      87,   222,   219,   221,   218,     0,     0,   132,     0,     0,
       8,     0,     0,   209,   205,   206,    38,     0,     0,   176,
     178,   169,   174,     0,   170,     0,     0,     0,   180,   196,
     197,   195,   194,   191,     0,     0,   198,   237,    88,     0,
     241,   230,     0,   224,   226,     0,   234,     0,     0,    89,
     248,     0,     0,    94,     0,     0,    42,    19,    21,     0,
       0,   223,   213,   141,   142,   143,     9,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   108,
     113,    99,   100,     0,   110,   101,   102,   103,   104,   112,
       0,   215,     0,   217,     0,   167,     0,   175,     0,   168,
     181,   186,   184,   193,   192,     0,   238,     0,   228,   233,
     229,   242,     0,   236,   239,     0,   243,     0,     0,   246,
      92,    98,     0,    27,    72,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   127,   128,   129,     0,     0,   109,
     111,   214,   216,   177,   179,   187,   201,   200,     0,   199,
     225,   227,   240,   244,   247,   249,    97,   105,     0,   107,
       0,     0,     0,     0,     0,     0,   126,   130,     0,   202,
     106,     0,     0,     0,     0,     0,     0,     0,   116,   118,
     119,     0,     0,     0,     0,     0,   120,     0,     0,   122,
       0,   124,     0,   117,   121,   123,   125
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,    47,    48,    49,    50,    51,    52,   208,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,   139,    67,   269,    68,    69,    70,   310,   311,
     312,   313,   314,   315,   316,   317,   318,    72,   177,    74,
     167,   168,    75,    76,    77,    78,   178,   179,   248,   249,
      79,   184,   185,    80,   102,   188,   263,   264,   335,   379,
      81,    82,   235,   171,   172,   232,   233,   272,   273,   274,
     119,   275,   194,   198,   281
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -232
static const yytype_int16 yypact[] =
{
     578,   -69,  -232,  -232,  -232,  -232,  -232,   -52,  1661,  1713,
    1713,  -232,  -232,  -232,  -232,  -232,  -232,  -232,  -232,  -232,
    -232,  -232,  -232,  -232,  -232,  -232,  -232,  -232,  -232,    16,
      28,    31,    62,  1761,  -232,  2503,   -26,  -232,  1761,  1761,
    1761,  1761,  1761,  1761,   -79,  1401,  -232,    43,   578,  -232,
    -232,  -232,   129,   360,  -232,   110,   -13,   197,    57,   199,
     -35,     7,    25,   119,    -9,  -232,  -232,   -19,  -232,  -232,
    -232,  -232,  -232,   705,  -232,  -232,  -232,  -232,  -232,  -232,
    -232,    69,  -232,  -232,  -232,   -52,  1401,  -232,  1761,  -232,
    -232,    40,  -232,  2503,    49,  -232,  2503,    52,  -232,   188,
      88,  -232,   115,  -232,  -232,  -232,    74,  2380,   927,  1761,
    2503,  -232,  -232,  -232,  -232,  -232,  -232,   106,   116,   112,
    -232,  -232,   218,  -232,  -232,   221,  1761,  1270,  -232,  -232,
    -232,  -232,  -232,  -232,  -232,  -232,  -232,  -232,  -232,  1761,
    1761,  1761,  1761,  1761,  1761,  1761,  1761,  1761,  1761,  1761,
    1761,  1761,  1761,  1761,  1761,  1761,  1761,  1761,  1761,  1761,
    -232,   -69,   186,   186,    11,  -232,  -232,   -18,  -232,  -232,
     -63,   -32,    11,   151,   159,   139,  2503,  1543,  1884,  -232,
    2503,  1936,   188,   171,     2,  -232,  -232,   184,   130,  -232,
    1593,  2092,    30,  -232,    35,  1047,   876,  -232,  -232,  -232,
     944,  2164,  -232,  1761,  -232,  -232,   -10,  -232,   133,  -232,
    -232,  -232,  -232,  -232,   110,   110,   -13,   -13,   197,   197,
     197,   197,    57,    57,   199,   -35,     7,    25,   119,   -48,
    -232,  -232,  -232,   186,  -232,   142,    11,  -232,   876,   368,
    -232,  1609,  2236,  -232,  -232,  -232,  -232,  1988,    45,  -232,
     189,  -232,  -232,  2040,  -232,    84,  1761,   188,  -232,  -232,
    -232,  -232,  -232,  -232,    74,   184,  -232,  -232,  -232,   169,
    -232,  1472,   181,   190,  -232,   192,    35,  1645,  2308,  -232,
    -232,   103,  1148,  -232,   194,   200,  -232,  -232,  -232,  1761,
    1761,  -232,  -232,  -232,   216,  -232,   -22,  1761,   219,   206,
     207,   209,   628,   210,   313,   211,   212,   755,   223,  -232,
    -232,  -232,  -232,   473,  -232,  -232,  -232,  -232,  -232,  -232,
     705,  -232,   220,  -232,   217,  -232,    11,  -232,  1761,  -232,
    -232,  -232,  -232,  -232,  -232,  1832,  -232,  1220,  -232,   213,
    -232,  -232,  2451,  -232,  -232,   225,  -232,   224,   823,  -232,
    -232,  -232,   194,  -232,  -232,   628,   242,   628,  1761,  1761,
    1761,   270,   578,   226,  -232,  -232,  -232,    76,  1761,  -232,
    -232,  -232,  -232,  -232,  -232,  -232,  -232,  -232,   244,  -232,
    -232,  -232,  -232,  -232,  -232,  -232,  -232,  -232,   628,  -232,
     145,   147,   150,   229,   771,   771,  -232,  -232,   156,  -232,
    -232,   628,   628,   628,  1761,  1286,  1323,   628,   277,  -232,
    -232,   161,   628,   163,   628,   170,  -232,   628,   228,  -232,
     628,  -232,   628,  -232,  -232,  -232,  -232
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -232,  -232,  -232,   289,     4,  -232,  -232,  -232,    20,    85,
     134,   144,    50,   141,   187,   191,   195,   185,   196,  -232,
    -139,  -104,  -232,   -36,  -230,  -232,  -232,  -232,  -116,  -232,
    -168,  -232,    29,     0,  -232,  -232,  -232,  -231,     1,   -68,
    -232,   108,  -232,  -232,  -130,  -232,   -83,   -97,  -232,    21,
    -232,   164,    92,  -232,  -121,  -232,    86,    17,  -232,  -232,
    -232,   -57,   -58,  -232,   -90,   193,  -232,  -132,  -232,    12,
     -11,   -89,  -186,  -193,  -232
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -1
static const yytype_uint16 yytable[] =
{
      71,    73,   240,   280,   197,   166,   276,   283,   319,   118,
      83,   322,   157,   181,   161,   170,   169,   192,   193,    91,
      85,   238,   117,   209,   108,    92,   331,    84,    87,    89,
      90,    94,   231,   231,    97,   210,   107,    95,   290,   166,
      98,   109,   239,   120,   110,   295,   107,   345,    71,    73,
     118,   268,   118,   103,   153,   230,   159,    83,   103,   103,
     103,   103,   103,   103,   355,   100,   262,   356,   241,   285,
     242,   101,   173,   147,   148,   175,   158,   189,   174,   143,
     144,   252,   319,   101,   252,   159,   236,   107,   160,   237,
     206,   287,   197,   247,   159,   154,   197,   253,   374,   200,
     162,   192,   268,   291,   377,   163,   257,   169,   258,   166,
     324,   107,   155,   164,   243,   169,   351,   268,   104,   250,
     169,    93,   229,   111,   112,   113,   114,   115,   116,   103,
     190,   395,   191,    96,   197,   277,    99,   278,   268,   156,
     122,   123,   124,   333,   262,   176,   347,   149,   150,   326,
     252,   354,   327,   276,   180,   385,   252,   182,   268,   118,
     103,   103,   103,   103,   103,   103,   103,   103,   103,   103,
     103,   103,   103,   103,   103,   103,   103,   103,   294,   169,
     159,   339,   340,   397,   386,   353,   361,   189,   257,   268,
     330,   183,   271,   101,   199,   186,   107,   218,   219,   220,
     221,   187,   271,   166,   140,   141,   142,   348,   201,   349,
     103,   145,   146,   338,   169,   203,   161,   151,   152,   202,
     159,   204,    85,   103,   205,   211,   212,   213,   125,   126,
     244,   127,    25,    26,   265,   266,   288,   289,   245,   387,
     320,   389,   246,   271,   197,   292,   118,   339,   401,   159,
     402,   159,   166,   403,   159,   256,   259,   260,   261,   407,
     159,   103,   294,   169,   418,   159,   420,   159,   250,   169,
     336,   367,   400,   422,   159,   328,   103,   214,   215,   271,
     169,   243,   169,   107,   341,   408,   409,   410,   286,   216,
     217,   416,   222,   223,   342,   343,   419,   103,   421,   239,
     238,   423,   162,   352,   425,   357,   426,   163,   358,   359,
     103,   360,   362,   190,   320,   337,   363,   103,   364,   365,
     372,   371,   390,   391,   392,   368,   382,   383,   388,   393,
     399,   404,   398,   396,   417,   424,    73,   121,   271,   376,
     224,   227,   370,   271,   293,   225,   255,   373,   103,   332,
     226,   334,   378,   228,   381,     0,   234,     0,     0,     0,
       0,     0,   394,   320,     0,     0,     0,     0,   411,   413,
     415,   296,     2,     3,     4,     5,     6,     7,     8,     0,
       9,    10,   128,   129,   130,   131,   132,   133,   134,   135,
     136,   137,     0,     0,   405,   406,     0,     0,     0,     0,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31,     0,   297,   298,   299,     0,   300,   301,   302,   303,
     304,   305,   306,   307,    32,    33,    34,     0,    35,     0,
       0,     0,     0,     0,   138,     0,     0,    36,     0,     0,
     308,    37,     0,     0,     0,     0,     0,    38,     0,     0,
      39,    40,    41,     0,     0,    42,    43,     0,    44,     0,
      45,     0,     0,   239,   309,    46,   296,     2,     3,     4,
       5,     6,     7,     8,     0,     9,    10,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    31,     0,   297,   298,   299,
       0,   300,   301,   302,   303,   304,   305,   306,   307,    32,
      33,    34,     0,    35,     0,     0,     0,     0,     0,     0,
       0,     0,    36,     0,     0,   308,    37,     0,     0,     0,
       0,     0,    38,     0,     0,    39,    40,    41,     0,     0,
      42,    43,     0,    44,     0,    45,     0,     0,   239,   369,
      46,     1,     2,     3,     4,     5,     6,     7,     8,     0,
       9,    10,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31,   296,     2,     3,     4,     5,     6,    85,     8,     0,
       9,    10,     0,     0,    32,    33,    34,     0,    35,     0,
       0,     0,     0,     0,     0,     0,     0,    36,     0,     0,
       0,    37,     0,     0,     0,     0,     0,    38,     0,     0,
      39,    40,    41,     0,     0,    42,    43,     0,    44,     0,
      45,     0,   297,   298,   299,    46,   300,   301,   302,   303,
     304,   305,   306,   307,     0,    33,     0,     0,    35,     0,
       0,     0,     0,     0,     0,     0,     0,    36,   161,     0,
     308,     0,     0,     0,     7,     0,     0,    38,     0,     0,
      39,    40,    41,     0,     0,    42,    43,     0,    44,     0,
      45,     0,     0,   239,     0,    46,     0,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,     1,     2,
       3,     4,     5,     6,    85,     8,     0,     9,    10,     0,
       0,   106,     0,    34,     1,     2,     3,     4,     5,     6,
      85,     8,     0,     9,    10,     0,     0,     0,    37,     0,
       0,     0,     0,     0,   162,     0,     0,     0,     0,   163,
       0,     0,     0,     0,     0,     0,     0,   164,     0,     0,
       0,     0,   165,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    33,     0,     0,    35,     1,     2,     3,     4,
       5,     6,    85,     8,    36,     9,    10,     0,    33,     0,
       0,    35,     0,     0,    38,     0,     0,    39,    40,    41,
      36,     0,    42,    43,     0,    44,     0,    45,     0,     0,
      38,     0,   366,    39,    40,    41,     0,     0,    42,    43,
       0,    44,     0,    45,     0,     0,     0,     0,    46,     1,
       2,     3,     4,     5,     6,    85,     8,     0,     9,    10,
      33,     0,     0,    35,     0,     0,     0,     0,     0,     0,
       0,     0,    36,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    38,     0,     0,    39,    40,    41,     0,     0,
      42,    43,     0,    44,     0,    45,     0,     0,   196,   384,
       1,     2,     3,     4,     5,     6,    85,     8,     0,     9,
      10,     0,     0,    33,     0,     0,    35,     1,     2,     3,
       4,     5,     6,    85,     8,    36,     9,    10,     0,     0,
       0,     0,     0,     0,     0,    38,     0,     0,    39,    40,
      41,     0,     0,    42,    43,     0,    44,     0,    45,     0,
       0,   196,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    33,     0,     0,    35,     0,     0,
       0,     0,     0,     0,     0,     0,    36,     0,     0,     0,
       0,    33,     0,     0,    35,     0,     0,     0,     0,     0,
       0,     0,     0,    36,    42,    43,     0,    44,     0,   195,
       0,     0,   196,     0,     0,     0,     0,     0,     0,     0,
       0,    42,    43,     0,    44,     0,   282,     0,     0,   196,
       1,     2,     3,     4,     5,     6,     7,     8,     0,     9,
      10,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   106,    33,    34,     0,    35,     0,     0,
       0,     0,     0,     0,     0,     0,    36,     0,     0,     0,
      37,     0,     0,     0,     0,     0,    38,     0,     0,    39,
      40,    41,     0,     0,    42,    43,     0,    44,     0,    45,
     279,     1,     2,     3,     4,     5,     6,     7,     8,     0,
       9,    10,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   106,    33,    34,     0,    35,     0,
       0,     0,     0,   161,     0,     0,     0,    36,     0,     7,
       0,    37,     0,     0,     0,     0,     0,    38,     0,     0,
      39,    40,    41,     0,     0,    42,    43,     0,    44,     0,
      45,   350,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,     1,     2,     3,     4,     5,     6,    85,
       8,     0,     9,    10,     0,     0,   106,     0,    34,     1,
       2,     3,     4,     5,     6,    85,     8,     0,     9,    10,
       0,     0,     0,    37,     0,     0,     0,     0,     0,   162,
       0,     0,     0,     0,   163,     0,     0,     0,     0,     0,
     190,     0,   337,   270,     0,     0,     1,     2,     3,     4,
       5,     6,    85,     8,     0,     9,    10,    33,     0,     0,
      35,     0,     0,     0,     0,     0,     0,     0,     0,    36,
       0,     0,     0,    33,     0,     0,    35,     0,     0,    38,
       0,     0,    39,    40,    41,    36,     0,    42,    43,     0,
      44,     0,    45,   207,     0,    38,     0,     0,    39,    40,
      41,     0,     0,    42,    43,     0,    44,     0,    45,   412,
      33,     0,     0,    35,     0,     0,     0,     0,     0,     0,
       0,     0,    36,     0,     1,     2,     3,     4,     5,     6,
       7,     8,    38,     9,    10,    39,    40,    41,     0,     0,
      42,    43,     0,    44,     0,    45,   414,     0,     0,     0,
       0,     0,     0,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    31,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   106,    33,    34,
       0,    35,     0,     0,     0,   161,     0,     0,     0,     0,
      36,     7,     0,     0,    37,     0,     0,     0,     0,     0,
      38,     0,     0,    39,    40,    41,     0,     0,    42,    43,
       0,    44,     0,    45,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   106,     0,
      34,     0,     0,     0,     0,     0,   161,     0,     0,     0,
       0,     0,     7,     0,     0,    37,     0,     0,     0,     0,
       0,   162,     0,     0,     0,     0,   163,     0,     0,     0,
       0,     0,   190,     0,   337,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    31,     1,     2,     3,     4,
       5,     6,    85,     8,     0,     9,    10,     0,     0,   106,
       0,    34,     1,     2,     3,     4,     5,     6,    85,     8,
       0,     9,    10,     0,     0,     0,    37,     0,     0,     0,
       0,     0,   162,     0,     0,     0,     0,   163,     0,     0,
       0,     0,     0,     0,     0,   164,     0,     0,     1,     2,
       3,     4,     5,     6,    85,     8,     0,     9,    10,     0,
      33,     0,     0,    35,     1,     2,     3,     4,     5,     6,
      85,     8,    36,     9,    10,     0,    33,     0,     0,    35,
       0,     0,    38,     0,     0,    39,    40,    41,    36,     0,
      42,    43,     0,    44,   267,    45,     0,     0,    38,     0,
       0,    39,    40,    41,     0,     0,    42,    43,     0,    44,
     321,    45,    33,     0,     0,    35,     1,     2,     3,     4,
       5,     6,    85,     8,    36,     9,    10,     0,    33,     0,
       0,    35,     0,     0,    38,     0,     0,    39,    40,    41,
      36,     0,    42,    43,     0,    44,   344,    45,     0,     0,
      38,     0,     0,    39,    40,    41,     0,     0,    42,    43,
       0,    44,     0,    86,     1,     2,     3,     4,     5,     6,
      85,     8,     0,     9,    10,     0,     0,     0,     0,     0,
      33,     0,     0,    35,     0,     0,     0,     0,     0,     0,
       0,     0,    36,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    38,     0,     0,    39,    40,    41,     0,     0,
      42,    43,     0,    44,     0,    88,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    33,     0,
       0,    35,     0,     0,     0,     0,     0,     0,     0,     0,
      36,   105,     0,     0,     0,     0,     0,     0,     0,     0,
      38,     0,     0,    39,    40,    41,     0,     0,    42,    43,
       0,    44,     0,    45,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,     0,     0,     0,     0,     0,
       0,     0,     0,   105,     0,     0,     0,     0,    32,     0,
      34,     0,     0,     0,   259,   260,   261,     0,     0,     0,
       0,     0,     0,     0,     0,    37,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,     0,   375,     0,
       0,     0,     0,     0,     0,   105,     0,     0,     0,     0,
     106,     0,    34,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    37,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    31,     0,
     251,     0,     0,     0,     0,     0,     0,   105,     0,     0,
       0,     0,   106,     0,    34,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    37,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31,     0,   254,     0,     0,     0,     0,     0,     0,   105,
       0,     0,     0,     0,   106,     0,    34,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    37,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,     0,   325,     0,     0,     0,     0,     0,
       0,   105,     0,     0,     0,     0,   106,     0,    34,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    37,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,     0,   329,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   106,     0,
      34,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   105,     0,    37,     0,     0,     0,     0,
       0,   162,     0,     0,     0,     0,   163,     0,     0,     0,
       0,     0,   190,     0,   191,   270,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     106,     0,    34,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   105,     0,    37,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   284,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    31,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   106,     0,    34,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   105,     0,    37,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   323,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   106,     0,    34,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   105,
       0,    37,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   346,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   106,     0,    34,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     105,     0,     0,    37,     0,     0,     0,     0,     0,   162,
       0,     0,     0,     0,   163,     0,     0,     0,     0,     0,
     190,     0,   191,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    31,   380,     0,     0,     0,     0,     0,
       0,     0,   105,     0,     0,     0,     0,   106,     0,    34,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    37,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    31,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   106,
       0,    34,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    37
};

static const yytype_int16 yycheck[] =
{
       0,     0,   170,   196,   108,    73,   192,   200,   239,    45,
      79,   241,    21,    96,     3,    73,    73,   107,   107,     3,
       9,    84,   101,   127,    35,     9,   256,    79,     8,     9,
      10,     3,   162,   163,     3,   139,    35,     9,    86,   107,
       9,    67,   105,     0,    70,   238,    45,   277,    48,    48,
      86,   190,    88,    33,    89,   159,   104,    79,    38,    39,
      40,    41,    42,    43,    86,     3,   187,   297,   100,   201,
     102,     9,     3,    16,    17,    86,    85,     3,     9,    92,
      93,   178,   313,     9,   181,   104,   104,    86,   107,   107,
     126,   101,   196,   176,   104,    88,   200,   180,   328,   110,
      89,   191,   241,   233,   335,    94,   104,   164,   106,   177,
     242,   110,    87,   102,   172,   172,   284,   256,    33,   177,
     177,   105,   158,    38,    39,    40,    41,    42,    43,   109,
     100,   362,   102,   105,   238,   100,   105,   102,   277,    20,
      11,    12,    13,   264,   265,   105,   278,    90,    91,   104,
     247,   290,   107,   339,   105,   348,   253,   105,   297,   195,
     140,   141,   142,   143,   144,   145,   146,   147,   148,   149,
     150,   151,   152,   153,   154,   155,   156,   157,   236,   236,
     104,   271,   271,   107,   352,   289,   302,     3,   104,   328,
     106,     3,   191,     9,   109,   107,   195,   147,   148,   149,
     150,    86,   201,   271,    94,    95,    96,   104,   102,   106,
     190,    14,    15,   271,   271,   103,     3,    18,    19,   103,
     104,     3,     9,   203,     3,   140,   141,   142,    99,   100,
      79,   102,    46,    47,   104,   105,   103,   104,    79,   355,
     239,   357,   103,   242,   348,   103,   282,   337,   103,   104,
     103,   104,   320,   103,   104,    84,    72,    73,    74,   103,
     104,   241,   320,   320,   103,   104,   103,   104,   326,   326,
     101,   307,   388,   103,   104,    86,   256,   143,   144,   278,
     337,   339,   339,   282,   103,   401,   402,   403,   203,   145,
     146,   407,   151,   152,   104,   103,   412,   277,   414,   105,
      84,   417,    89,   103,   420,    86,   422,    94,   102,   102,
     290,   102,   102,   100,   313,   102,     3,   297,   107,   107,
     103,   101,   358,   359,   360,   102,   101,   103,    86,    59,
      86,   102,   368,   107,    57,   107,   335,    48,   337,   335,
     153,   156,   313,   342,   236,   154,   182,   326,   328,   257,
     155,   265,   335,   157,   342,    -1,   163,    -1,    -1,    -1,
      -1,    -1,   362,   362,    -1,    -1,    -1,    -1,   404,   405,
     406,     3,     4,     5,     6,     7,     8,     9,    10,    -1,
      12,    13,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    -1,    -1,   394,   395,    -1,    -1,    -1,    -1,
      32,    33,    34,    35,    36,    37,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    47,    48,    49,    50,    51,
      52,    -1,    54,    55,    56,    -1,    58,    59,    60,    61,
      62,    63,    64,    65,    66,    67,    68,    -1,    70,    -1,
      -1,    -1,    -1,    -1,    84,    -1,    -1,    79,    -1,    -1,
      82,    83,    -1,    -1,    -1,    -1,    -1,    89,    -1,    -1,
      92,    93,    94,    -1,    -1,    97,    98,    -1,   100,    -1,
     102,    -1,    -1,   105,   106,   107,     3,     4,     5,     6,
       7,     8,     9,    10,    -1,    12,    13,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    32,    33,    34,    35,    36,
      37,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      47,    48,    49,    50,    51,    52,    -1,    54,    55,    56,
      -1,    58,    59,    60,    61,    62,    63,    64,    65,    66,
      67,    68,    -1,    70,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    79,    -1,    -1,    82,    83,    -1,    -1,    -1,
      -1,    -1,    89,    -1,    -1,    92,    93,    94,    -1,    -1,
      97,    98,    -1,   100,    -1,   102,    -1,    -1,   105,   106,
     107,     3,     4,     5,     6,     7,     8,     9,    10,    -1,
      12,    13,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      32,    33,    34,    35,    36,    37,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    47,    48,    49,    50,    51,
      52,     3,     4,     5,     6,     7,     8,     9,    10,    -1,
      12,    13,    -1,    -1,    66,    67,    68,    -1,    70,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    79,    -1,    -1,
      -1,    83,    -1,    -1,    -1,    -1,    -1,    89,    -1,    -1,
      92,    93,    94,    -1,    -1,    97,    98,    -1,   100,    -1,
     102,    -1,    54,    55,    56,   107,    58,    59,    60,    61,
      62,    63,    64,    65,    -1,    67,    -1,    -1,    70,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    79,     3,    -1,
      82,    -1,    -1,    -1,     9,    -1,    -1,    89,    -1,    -1,
      92,    93,    94,    -1,    -1,    97,    98,    -1,   100,    -1,
     102,    -1,    -1,   105,    -1,   107,    -1,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,     3,     4,
       5,     6,     7,     8,     9,    10,    -1,    12,    13,    -1,
      -1,    66,    -1,    68,     3,     4,     5,     6,     7,     8,
       9,    10,    -1,    12,    13,    -1,    -1,    -1,    83,    -1,
      -1,    -1,    -1,    -1,    89,    -1,    -1,    -1,    -1,    94,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   102,    -1,    -1,
      -1,    -1,   107,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    67,    -1,    -1,    70,     3,     4,     5,     6,
       7,     8,     9,    10,    79,    12,    13,    -1,    67,    -1,
      -1,    70,    -1,    -1,    89,    -1,    -1,    92,    93,    94,
      79,    -1,    97,    98,    -1,   100,    -1,   102,    -1,    -1,
      89,    -1,   107,    92,    93,    94,    -1,    -1,    97,    98,
      -1,   100,    -1,   102,    -1,    -1,    -1,    -1,   107,     3,
       4,     5,     6,     7,     8,     9,    10,    -1,    12,    13,
      67,    -1,    -1,    70,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    79,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    89,    -1,    -1,    92,    93,    94,    -1,    -1,
      97,    98,    -1,   100,    -1,   102,    -1,    -1,   105,   106,
       3,     4,     5,     6,     7,     8,     9,    10,    -1,    12,
      13,    -1,    -1,    67,    -1,    -1,    70,     3,     4,     5,
       6,     7,     8,     9,    10,    79,    12,    13,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    89,    -1,    -1,    92,    93,
      94,    -1,    -1,    97,    98,    -1,   100,    -1,   102,    -1,
      -1,   105,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    67,    -1,    -1,    70,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    79,    -1,    -1,    -1,
      -1,    67,    -1,    -1,    70,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    79,    97,    98,    -1,   100,    -1,   102,
      -1,    -1,   105,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    97,    98,    -1,   100,    -1,   102,    -1,    -1,   105,
       3,     4,     5,     6,     7,     8,     9,    10,    -1,    12,
      13,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,
      33,    34,    35,    36,    37,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    47,    48,    49,    50,    51,    52,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    66,    67,    68,    -1,    70,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    79,    -1,    -1,    -1,
      83,    -1,    -1,    -1,    -1,    -1,    89,    -1,    -1,    92,
      93,    94,    -1,    -1,    97,    98,    -1,   100,    -1,   102,
     103,     3,     4,     5,     6,     7,     8,     9,    10,    -1,
      12,    13,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      32,    33,    34,    35,    36,    37,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    47,    48,    49,    50,    51,
      52,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    66,    67,    68,    -1,    70,    -1,
      -1,    -1,    -1,     3,    -1,    -1,    -1,    79,    -1,     9,
      -1,    83,    -1,    -1,    -1,    -1,    -1,    89,    -1,    -1,
      92,    93,    94,    -1,    -1,    97,    98,    -1,   100,    -1,
     102,   103,    32,    33,    34,    35,    36,    37,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    47,    48,    49,
      50,    51,    52,     3,     4,     5,     6,     7,     8,     9,
      10,    -1,    12,    13,    -1,    -1,    66,    -1,    68,     3,
       4,     5,     6,     7,     8,     9,    10,    -1,    12,    13,
      -1,    -1,    -1,    83,    -1,    -1,    -1,    -1,    -1,    89,
      -1,    -1,    -1,    -1,    94,    -1,    -1,    -1,    -1,    -1,
     100,    -1,   102,   103,    -1,    -1,     3,     4,     5,     6,
       7,     8,     9,    10,    -1,    12,    13,    67,    -1,    -1,
      70,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    79,
      -1,    -1,    -1,    67,    -1,    -1,    70,    -1,    -1,    89,
      -1,    -1,    92,    93,    94,    79,    -1,    97,    98,    -1,
     100,    -1,   102,   103,    -1,    89,    -1,    -1,    92,    93,
      94,    -1,    -1,    97,    98,    -1,   100,    -1,   102,   103,
      67,    -1,    -1,    70,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    79,    -1,     3,     4,     5,     6,     7,     8,
       9,    10,    89,    12,    13,    92,    93,    94,    -1,    -1,
      97,    98,    -1,   100,    -1,   102,   103,    -1,    -1,    -1,
      -1,    -1,    -1,    32,    33,    34,    35,    36,    37,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    47,    48,
      49,    50,    51,    52,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    66,    67,    68,
      -1,    70,    -1,    -1,    -1,     3,    -1,    -1,    -1,    -1,
      79,     9,    -1,    -1,    83,    -1,    -1,    -1,    -1,    -1,
      89,    -1,    -1,    92,    93,    94,    -1,    -1,    97,    98,
      -1,   100,    -1,   102,    32,    33,    34,    35,    36,    37,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    47,
      48,    49,    50,    51,    52,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    66,    -1,
      68,    -1,    -1,    -1,    -1,    -1,     3,    -1,    -1,    -1,
      -1,    -1,     9,    -1,    -1,    83,    -1,    -1,    -1,    -1,
      -1,    89,    -1,    -1,    -1,    -1,    94,    -1,    -1,    -1,
      -1,    -1,   100,    -1,   102,    32,    33,    34,    35,    36,
      37,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      47,    48,    49,    50,    51,    52,     3,     4,     5,     6,
       7,     8,     9,    10,    -1,    12,    13,    -1,    -1,    66,
      -1,    68,     3,     4,     5,     6,     7,     8,     9,    10,
      -1,    12,    13,    -1,    -1,    -1,    83,    -1,    -1,    -1,
      -1,    -1,    89,    -1,    -1,    -1,    -1,    94,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   102,    -1,    -1,     3,     4,
       5,     6,     7,     8,     9,    10,    -1,    12,    13,    -1,
      67,    -1,    -1,    70,     3,     4,     5,     6,     7,     8,
       9,    10,    79,    12,    13,    -1,    67,    -1,    -1,    70,
      -1,    -1,    89,    -1,    -1,    92,    93,    94,    79,    -1,
      97,    98,    -1,   100,   101,   102,    -1,    -1,    89,    -1,
      -1,    92,    93,    94,    -1,    -1,    97,    98,    -1,   100,
     101,   102,    67,    -1,    -1,    70,     3,     4,     5,     6,
       7,     8,     9,    10,    79,    12,    13,    -1,    67,    -1,
      -1,    70,    -1,    -1,    89,    -1,    -1,    92,    93,    94,
      79,    -1,    97,    98,    -1,   100,   101,   102,    -1,    -1,
      89,    -1,    -1,    92,    93,    94,    -1,    -1,    97,    98,
      -1,   100,    -1,   102,     3,     4,     5,     6,     7,     8,
       9,    10,    -1,    12,    13,    -1,    -1,    -1,    -1,    -1,
      67,    -1,    -1,    70,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    79,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    89,    -1,    -1,    92,    93,    94,    -1,    -1,
      97,    98,    -1,   100,    -1,   102,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    67,    -1,
      -1,    70,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      79,     9,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      89,    -1,    -1,    92,    93,    94,    -1,    -1,    97,    98,
      -1,   100,    -1,   102,    32,    33,    34,    35,    36,    37,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    47,
      48,    49,    50,    51,    52,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,     9,    -1,    -1,    -1,    -1,    66,    -1,
      68,    -1,    -1,    -1,    72,    73,    74,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    83,    32,    33,    34,    35,
      36,    37,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    47,    48,    49,    50,    51,    52,    -1,   106,    -1,
      -1,    -1,    -1,    -1,    -1,     9,    -1,    -1,    -1,    -1,
      66,    -1,    68,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    83,    32,    33,
      34,    35,    36,    37,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    47,    48,    49,    50,    51,    52,    -1,
     106,    -1,    -1,    -1,    -1,    -1,    -1,     9,    -1,    -1,
      -1,    -1,    66,    -1,    68,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    83,
      32,    33,    34,    35,    36,    37,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    47,    48,    49,    50,    51,
      52,    -1,   106,    -1,    -1,    -1,    -1,    -1,    -1,     9,
      -1,    -1,    -1,    -1,    66,    -1,    68,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    83,    32,    33,    34,    35,    36,    37,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    47,    48,    49,
      50,    51,    52,    -1,   106,    -1,    -1,    -1,    -1,    -1,
      -1,     9,    -1,    -1,    -1,    -1,    66,    -1,    68,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    83,    32,    33,    34,    35,    36,    37,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    47,
      48,    49,    50,    51,    52,    -1,   106,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    66,    -1,
      68,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,     9,    -1,    83,    -1,    -1,    -1,    -1,
      -1,    89,    -1,    -1,    -1,    -1,    94,    -1,    -1,    -1,
      -1,    -1,   100,    -1,   102,   103,    32,    33,    34,    35,
      36,    37,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    47,    48,    49,    50,    51,    52,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      66,    -1,    68,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,     9,    -1,    83,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   103,    32,    33,
      34,    35,    36,    37,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    47,    48,    49,    50,    51,    52,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    66,    -1,    68,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,     9,    -1,    83,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   103,
      32,    33,    34,    35,    36,    37,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    47,    48,    49,    50,    51,
      52,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    66,    -1,    68,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     9,
      -1,    83,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   103,    32,    33,    34,    35,    36,    37,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    47,    48,    49,
      50,    51,    52,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    66,    -1,    68,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
       9,    -1,    -1,    83,    -1,    -1,    -1,    -1,    -1,    89,
      -1,    -1,    -1,    -1,    94,    -1,    -1,    -1,    -1,    -1,
     100,    -1,   102,    32,    33,    34,    35,    36,    37,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    47,    48,
      49,    50,    51,    52,    53,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,     9,    -1,    -1,    -1,    -1,    66,    -1,    68,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    83,    32,    33,    34,    35,    36,
      37,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      47,    48,    49,    50,    51,    52,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    66,
      -1,    68,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    83
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     3,     4,     5,     6,     7,     8,     9,    10,    12,
      13,    32,    33,    34,    35,    36,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    47,    48,    49,    50,
      51,    52,    66,    67,    68,    70,    79,    83,    89,    92,
      93,    94,    97,    98,   100,   102,   107,   109,   110,   111,
     112,   113,   114,   116,   117,   118,   119,   120,   121,   122,
     123,   124,   125,   126,   127,   128,   129,   131,   133,   134,
     135,   141,   145,   146,   147,   150,   151,   152,   153,   158,
     161,   168,   169,    79,    79,     9,   102,   116,   102,   116,
     116,     3,     9,   105,     3,     9,   105,     3,     9,   105,
       3,     9,   162,   116,   117,     9,    66,   146,   178,    67,
      70,   117,   117,   117,   117,   117,   117,   101,   131,   178,
       0,   111,    11,    12,    13,    99,   100,   102,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    84,   130,
      94,    95,    96,    92,    93,    14,    15,    16,    17,    90,
      91,    18,    19,    89,    88,    87,    20,    21,    85,   104,
     107,     3,    89,    94,   102,   107,   147,   148,   149,   169,
     170,   171,   172,     3,     9,   178,   105,   146,   154,   155,
     105,   154,   105,     3,   159,   160,   107,    86,   163,     3,
     100,   102,   172,   179,   180,   102,   105,   129,   181,   117,
     178,   102,   103,   103,     3,     3,   131,   103,   115,   129,
     129,   117,   117,   117,   118,   118,   119,   119,   120,   120,
     120,   120,   121,   121,   122,   123,   124,   125,   126,   131,
     129,   152,   173,   174,   173,   170,   104,   107,    84,   105,
     138,   100,   102,   170,    79,    79,   103,   154,   156,   157,
     170,   106,   155,   154,   106,   159,    84,   104,   106,    72,
      73,    74,   162,   164,   165,   104,   105,   101,   128,   132,
     103,   146,   175,   176,   177,   179,   180,   100,   102,   103,
     181,   182,   102,   181,   103,   175,   117,   101,   103,   104,
      86,   152,   103,   149,   170,   181,     3,    54,    55,    56,
      58,    59,    60,    61,    62,    63,    64,    65,    82,   106,
     136,   137,   138,   139,   140,   141,   142,   143,   144,   145,
     146,   101,   132,   103,   175,   106,   104,   107,    86,   106,
     106,   132,   160,   162,   164,   166,   101,   102,   170,   172,
     179,   103,   104,   103,   101,   132,   103,   175,   104,   106,
     103,   138,   103,   129,   128,    86,   132,    86,   102,   102,
     102,   136,   102,     3,   107,   107,   107,   131,   102,   106,
     140,   101,   103,   157,   132,   106,   112,   145,   165,   167,
      53,   177,   101,   103,   106,   181,   138,   136,    86,   136,
     131,   131,   131,    59,   141,   145,   107,   107,   131,    86,
     136,   103,   103,   103,   102,   141,   141,   103,   136,   136,
     136,   131,   103,   131,   103,   131,   136,    57,   103,   136,
     103,   136,   103,   136,   107,   136,   136
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
# if YYLTYPE_IS_TRIVIAL
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


/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;

/* Number of syntax errors so far.  */
int yynerrs;



/*-------------------------.
| yyparse or yypush_parse.  |
`-------------------------*/

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
  if (yyn == YYPACT_NINF)
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
      if (yyn == 0 || yyn == YYTABLE_NINF)
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

/* Line 1455 of yacc.c  */
#line 129 "parser.y"
    { root = (yyvsp[(1) - (1)].node); ;}
    break;

  case 3:

/* Line 1455 of yacc.c  */
#line 133 "parser.y"
    { (yyval.node) = create_node(NODE_TRANSLATION_UNIT); (yyval.node)->data.items_list = create_list_node((yyvsp[(1) - (1)].node)); ;}
    break;

  case 4:

/* Line 1455 of yacc.c  */
#line 134 "parser.y"
    { if ((yyvsp[(2) - (2)].node)) { (yyvsp[(1) - (2)].node)->data.items_list = append_to_list((yyvsp[(1) - (2)].node)->data.items_list, (yyvsp[(2) - (2)].node)); } (yyval.node) = (yyvsp[(1) - (2)].node); ;}
    break;

  case 5:

/* Line 1455 of yacc.c  */
#line 138 "parser.y"
    { (yyval.node) = (yyvsp[(1) - (1)].node); ;}
    break;

  case 6:

/* Line 1455 of yacc.c  */
#line 139 "parser.y"
    { (yyval.node) = (yyvsp[(1) - (1)].node); ;}
    break;

  case 7:

/* Line 1455 of yacc.c  */
#line 140 "parser.y"
    { (yyval.node) = (yyvsp[(1) - (1)].node); ;}
    break;

  case 8:

/* Line 1455 of yacc.c  */
#line 144 "parser.y"
    { (yyval.node) = create_function_definition_node((yyvsp[(1) - (3)].list), (yyvsp[(2) - (3)].node), (yyvsp[(3) - (3)].node)); ;}
    break;

  case 9:

/* Line 1455 of yacc.c  */
#line 149 "parser.y"
    { (yyval.node) = create_identifier_node((yyvsp[(1) - (1)].str)); ;}
    break;

  case 10:

/* Line 1455 of yacc.c  */
#line 150 "parser.y"
    { (yyval.node) = (yyvsp[(1) - (1)].node); ;}
    break;

  case 11:

/* Line 1455 of yacc.c  */
#line 151 "parser.y"
    { (yyval.node) = create_constant_node((yyvsp[(1) - (1)].str)); ;}
    break;

  case 12:

/* Line 1455 of yacc.c  */
#line 152 "parser.y"
    { (yyval.node) = create_string_literal_node((yyvsp[(1) - (1)].str)); ;}
    break;

  case 13:

/* Line 1455 of yacc.c  */
#line 153 "parser.y"
    { (yyval.node) = create_constant_node((yyvsp[(1) - (1)].str)); ;}
    break;

  case 14:

/* Line 1455 of yacc.c  */
#line 154 "parser.y"
    { (yyval.node) = create_constant_node((yyvsp[(1) - (1)].str)); ;}
    break;

  case 15:

/* Line 1455 of yacc.c  */
#line 155 "parser.y"
    { (yyval.node) = create_constant_node((yyvsp[(1) - (1)].str)); ;}
    break;

  case 16:

/* Line 1455 of yacc.c  */
#line 156 "parser.y"
    { (yyval.node) = (yyvsp[(2) - (3)].node); ;}
    break;

  case 17:

/* Line 1455 of yacc.c  */
#line 157 "parser.y"
    { (yyval.node) = (yyvsp[(1) - (1)].node); ;}
    break;

  case 19:

/* Line 1455 of yacc.c  */
#line 162 "parser.y"
    { (yyval.node) = create_array_subscript_node((yyvsp[(1) - (4)].node), (yyvsp[(3) - (4)].node)); ;}
    break;

  case 20:

/* Line 1455 of yacc.c  */
#line 163 "parser.y"
    { (yyval.node) = create_func_call_node((yyvsp[(1) - (3)].node), NULL); ;}
    break;

  case 21:

/* Line 1455 of yacc.c  */
#line 164 "parser.y"
    { (yyval.node) = create_func_call_node((yyvsp[(1) - (4)].node), (yyvsp[(3) - (4)].list)); ;}
    break;

  case 22:

/* Line 1455 of yacc.c  */
#line 165 "parser.y"
    { (yyval.node) = create_member_access_node((yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].str), 0); ;}
    break;

  case 23:

/* Line 1455 of yacc.c  */
#line 166 "parser.y"
    { (yyval.node) = create_member_access_node((yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].str), 1); ;}
    break;

  case 24:

/* Line 1455 of yacc.c  */
#line 168 "parser.y"
    { (yyval.node) = create_unary_expr_node(INC_OP, (yyvsp[(1) - (2)].node)); (yyval.node)->type = NODE_POSTFIX_UNARY_EXPR; ;}
    break;

  case 25:

/* Line 1455 of yacc.c  */
#line 169 "parser.y"
    { (yyval.node) = create_unary_expr_node(DEC_OP, (yyvsp[(1) - (2)].node)); (yyval.node)->type = NODE_POSTFIX_UNARY_EXPR; ;}
    break;

  case 26:

/* Line 1455 of yacc.c  */
#line 174 "parser.y"
    { (yyval.list) = create_list_node((yyvsp[(1) - (1)].node)); ;}
    break;

  case 27:

/* Line 1455 of yacc.c  */
#line 176 "parser.y"
    { (yyval.list) = append_to_list((yyvsp[(1) - (3)].list), (yyvsp[(3) - (3)].node)); ;}
    break;

  case 29:

/* Line 1455 of yacc.c  */
#line 181 "parser.y"
    { (yyval.node) = create_unary_expr_node(INC_OP, (yyvsp[(2) - (2)].node)); (yyval.node)->type = NODE_PREFIX_UNARY_EXPR; ;}
    break;

  case 30:

/* Line 1455 of yacc.c  */
#line 182 "parser.y"
    { (yyval.node) = create_unary_expr_node(DEC_OP, (yyvsp[(2) - (2)].node)); (yyval.node)->type = NODE_PREFIX_UNARY_EXPR; ;}
    break;

  case 31:

/* Line 1455 of yacc.c  */
#line 183 "parser.y"
    { (yyval.node) = create_unary_expr_node('&', (yyvsp[(2) - (2)].node)); ;}
    break;

  case 32:

/* Line 1455 of yacc.c  */
#line 184 "parser.y"
    { (yyval.node) = create_unary_expr_node('*', (yyvsp[(2) - (2)].node)); ;}
    break;

  case 33:

/* Line 1455 of yacc.c  */
#line 185 "parser.y"
    { (yyval.node) = create_unary_expr_node('+', (yyvsp[(2) - (2)].node)); ;}
    break;

  case 34:

/* Line 1455 of yacc.c  */
#line 186 "parser.y"
    { (yyval.node) = create_unary_expr_node('-', (yyvsp[(2) - (2)].node)); ;}
    break;

  case 35:

/* Line 1455 of yacc.c  */
#line 187 "parser.y"
    { (yyval.node) = create_unary_expr_node('~', (yyvsp[(2) - (2)].node)); ;}
    break;

  case 36:

/* Line 1455 of yacc.c  */
#line 188 "parser.y"
    { (yyval.node) = create_unary_expr_node('!', (yyvsp[(2) - (2)].node)); ;}
    break;

  case 37:

/* Line 1455 of yacc.c  */
#line 189 "parser.y"
    { (yyval.node) = create_unary_expr_node(SIZEOF, (yyvsp[(2) - (2)].node)); ;}
    break;

  case 38:

/* Line 1455 of yacc.c  */
#line 190 "parser.y"
    { (yyval.node) = create_unary_expr_node(SIZEOF, (yyvsp[(3) - (4)].node)); ;}
    break;

  case 39:

/* Line 1455 of yacc.c  */
#line 191 "parser.y"
    { (yyval.node) = (yyvsp[(1) - (1)].node); ;}
    break;

  case 40:

/* Line 1455 of yacc.c  */
#line 192 "parser.y"
    { (yyval.node) = (yyvsp[(1) - (1)].node); ;}
    break;

  case 42:

/* Line 1455 of yacc.c  */
#line 206 "parser.y"
    { (yyval.node) = create_cast_expr_node((yyvsp[(2) - (4)].node), (yyvsp[(4) - (4)].node)); ;}
    break;

  case 44:

/* Line 1455 of yacc.c  */
#line 211 "parser.y"
    { (yyval.node) = create_binary_expr_node((yyvsp[(1) - (3)].node), '*', (yyvsp[(3) - (3)].node)); ;}
    break;

  case 45:

/* Line 1455 of yacc.c  */
#line 212 "parser.y"
    { (yyval.node) = create_binary_expr_node((yyvsp[(1) - (3)].node), '/', (yyvsp[(3) - (3)].node)); ;}
    break;

  case 46:

/* Line 1455 of yacc.c  */
#line 213 "parser.y"
    { (yyval.node) = create_binary_expr_node((yyvsp[(1) - (3)].node), '%', (yyvsp[(3) - (3)].node)); ;}
    break;

  case 48:

/* Line 1455 of yacc.c  */
#line 218 "parser.y"
    { (yyval.node) = create_binary_expr_node((yyvsp[(1) - (3)].node), '+', (yyvsp[(3) - (3)].node)); ;}
    break;

  case 49:

/* Line 1455 of yacc.c  */
#line 219 "parser.y"
    { (yyval.node) = create_binary_expr_node((yyvsp[(1) - (3)].node), '-', (yyvsp[(3) - (3)].node)); ;}
    break;

  case 51:

/* Line 1455 of yacc.c  */
#line 224 "parser.y"
    { (yyval.node) = create_binary_expr_node((yyvsp[(1) - (3)].node), LSHIFT_OP, (yyvsp[(3) - (3)].node)); ;}
    break;

  case 52:

/* Line 1455 of yacc.c  */
#line 225 "parser.y"
    { (yyval.node) = create_binary_expr_node((yyvsp[(1) - (3)].node), RSHIFT_OP, (yyvsp[(3) - (3)].node)); ;}
    break;

  case 54:

/* Line 1455 of yacc.c  */
#line 230 "parser.y"
    { (yyval.node) = create_binary_expr_node((yyvsp[(1) - (3)].node), '<', (yyvsp[(3) - (3)].node)); ;}
    break;

  case 55:

/* Line 1455 of yacc.c  */
#line 231 "parser.y"
    { (yyval.node) = create_binary_expr_node((yyvsp[(1) - (3)].node), '>', (yyvsp[(3) - (3)].node)); ;}
    break;

  case 56:

/* Line 1455 of yacc.c  */
#line 232 "parser.y"
    { (yyval.node) = create_binary_expr_node((yyvsp[(1) - (3)].node), LE_OP, (yyvsp[(3) - (3)].node)); ;}
    break;

  case 57:

/* Line 1455 of yacc.c  */
#line 233 "parser.y"
    { (yyval.node) = create_binary_expr_node((yyvsp[(1) - (3)].node), GE_OP, (yyvsp[(3) - (3)].node)); ;}
    break;

  case 59:

/* Line 1455 of yacc.c  */
#line 238 "parser.y"
    { (yyval.node) = create_binary_expr_node((yyvsp[(1) - (3)].node), EQ_OP, (yyvsp[(3) - (3)].node)); ;}
    break;

  case 60:

/* Line 1455 of yacc.c  */
#line 239 "parser.y"
    { (yyval.node) = create_binary_expr_node((yyvsp[(1) - (3)].node), NE_OP, (yyvsp[(3) - (3)].node)); ;}
    break;

  case 62:

/* Line 1455 of yacc.c  */
#line 244 "parser.y"
    { (yyval.node) = create_binary_expr_node((yyvsp[(1) - (3)].node), '&', (yyvsp[(3) - (3)].node)); ;}
    break;

  case 64:

/* Line 1455 of yacc.c  */
#line 249 "parser.y"
    { (yyval.node) = create_binary_expr_node((yyvsp[(1) - (3)].node), '^', (yyvsp[(3) - (3)].node)); ;}
    break;

  case 66:

/* Line 1455 of yacc.c  */
#line 254 "parser.y"
    { (yyval.node) = create_binary_expr_node((yyvsp[(1) - (3)].node), '|', (yyvsp[(3) - (3)].node)); ;}
    break;

  case 68:

/* Line 1455 of yacc.c  */
#line 259 "parser.y"
    { (yyval.node) = create_binary_expr_node((yyvsp[(1) - (3)].node), LAND_OP, (yyvsp[(3) - (3)].node)); ;}
    break;

  case 70:

/* Line 1455 of yacc.c  */
#line 264 "parser.y"
    { (yyval.node) = create_binary_expr_node((yyvsp[(1) - (3)].node), LOR_OP, (yyvsp[(3) - (3)].node)); ;}
    break;

  case 72:

/* Line 1455 of yacc.c  */
#line 269 "parser.y"
    { (yyval.node) = create_conditional_expr_node((yyvsp[(1) - (5)].node), (yyvsp[(3) - (5)].node), (yyvsp[(5) - (5)].node)); ;}
    break;

  case 74:

/* Line 1455 of yacc.c  */
#line 274 "parser.y"
    { (yyval.node) = create_assignment_node((yyvsp[(1) - (3)].node), (yyvsp[(2) - (3)].token), (yyvsp[(3) - (3)].node)); ;}
    break;

  case 75:

/* Line 1455 of yacc.c  */
#line 278 "parser.y"
    { (yyval.token) = '='; ;}
    break;

  case 76:

/* Line 1455 of yacc.c  */
#line 279 "parser.y"
    { (yyval.token) = MUL_ASSIGN; ;}
    break;

  case 77:

/* Line 1455 of yacc.c  */
#line 280 "parser.y"
    { (yyval.token) = DIV_ASSIGN; ;}
    break;

  case 78:

/* Line 1455 of yacc.c  */
#line 281 "parser.y"
    { (yyval.token) = MOD_ASSIGN; ;}
    break;

  case 79:

/* Line 1455 of yacc.c  */
#line 282 "parser.y"
    { (yyval.token) = ADD_ASSIGN; ;}
    break;

  case 80:

/* Line 1455 of yacc.c  */
#line 283 "parser.y"
    { (yyval.token) = SUB_ASSIGN; ;}
    break;

  case 81:

/* Line 1455 of yacc.c  */
#line 284 "parser.y"
    { (yyval.token) = LSHIFT_ASSIGN; ;}
    break;

  case 82:

/* Line 1455 of yacc.c  */
#line 285 "parser.y"
    { (yyval.token) = RSHIFT_ASSIGN; ;}
    break;

  case 83:

/* Line 1455 of yacc.c  */
#line 286 "parser.y"
    { (yyval.token) = AND_ASSIGN; ;}
    break;

  case 84:

/* Line 1455 of yacc.c  */
#line 287 "parser.y"
    { (yyval.token) = XOR_ASSIGN; ;}
    break;

  case 85:

/* Line 1455 of yacc.c  */
#line 288 "parser.y"
    { (yyval.token) = OR_ASSIGN; ;}
    break;

  case 87:

/* Line 1455 of yacc.c  */
#line 293 "parser.y"
    { (yyval.node) = create_binary_expr_node((yyvsp[(1) - (3)].node), ',', (yyvsp[(3) - (3)].node)); ;}
    break;

  case 89:

/* Line 1455 of yacc.c  */
#line 302 "parser.y"
    { (yyval.node) = create_new_expr_node((yyvsp[(2) - (4)].node), NULL); ;}
    break;

  case 90:

/* Line 1455 of yacc.c  */
#line 303 "parser.y"
    { (yyval.node) = create_new_expr_node((yyvsp[(2) - (2)].node), NULL); ;}
    break;

  case 91:

/* Line 1455 of yacc.c  */
#line 304 "parser.y"
    { (yyval.node) = create_new_expr_node((yyvsp[(2) - (3)].node), (yyvsp[(3) - (3)].node)); ;}
    break;

  case 92:

/* Line 1455 of yacc.c  */
#line 305 "parser.y"
    { (yyval.node) = create_new_expr_node((yyvsp[(3) - (5)].node), NULL); /* Mark as global */ ;}
    break;

  case 93:

/* Line 1455 of yacc.c  */
#line 306 "parser.y"
    { (yyval.node) = create_new_expr_node((yyvsp[(3) - (3)].node), NULL); /* Mark as global */ ;}
    break;

  case 94:

/* Line 1455 of yacc.c  */
#line 307 "parser.y"
    { (yyval.node) = create_new_expr_node((yyvsp[(3) - (4)].node), (yyvsp[(4) - (4)].node)); /* Mark as global */ ;}
    break;

  case 95:

/* Line 1455 of yacc.c  */
#line 311 "parser.y"
    { (yyval.node) = create_delete_expr_node((yyvsp[(2) - (2)].node)); ;}
    break;

  case 96:

/* Line 1455 of yacc.c  */
#line 312 "parser.y"
    { (yyval.node) = create_delete_expr_node((yyvsp[(3) - (3)].node)); /* Mark as global */ ;}
    break;

  case 97:

/* Line 1455 of yacc.c  */
#line 316 "parser.y"
    { (yyval.node) = create_lambda_node(NULL, (yyvsp[(4) - (6)].list), (yyvsp[(6) - (6)].node)); ;}
    break;

  case 98:

/* Line 1455 of yacc.c  */
#line 317 "parser.y"
    { (yyval.node) = create_lambda_node(NULL, NULL, (yyvsp[(5) - (5)].node)); ;}
    break;

  case 99:

/* Line 1455 of yacc.c  */
#line 322 "parser.y"
    { (yyval.node) = (yyvsp[(1) - (1)].node); ;}
    break;

  case 100:

/* Line 1455 of yacc.c  */
#line 323 "parser.y"
    { (yyval.node) = (yyvsp[(1) - (1)].node); ;}
    break;

  case 101:

/* Line 1455 of yacc.c  */
#line 324 "parser.y"
    { (yyval.node) = (yyvsp[(1) - (1)].node); ;}
    break;

  case 102:

/* Line 1455 of yacc.c  */
#line 325 "parser.y"
    { (yyval.node) = (yyvsp[(1) - (1)].node); ;}
    break;

  case 103:

/* Line 1455 of yacc.c  */
#line 326 "parser.y"
    { (yyval.node) = (yyvsp[(1) - (1)].node); ;}
    break;

  case 104:

/* Line 1455 of yacc.c  */
#line 327 "parser.y"
    { (yyval.node) = (yyvsp[(1) - (1)].node); ;}
    break;

  case 105:

/* Line 1455 of yacc.c  */
#line 331 "parser.y"
    { (yyval.node) = create_labeled_statement_node((yyvsp[(1) - (3)].str), (yyvsp[(3) - (3)].node)); ;}
    break;

  case 106:

/* Line 1455 of yacc.c  */
#line 332 "parser.y"
    { (yyval.node) = create_case_statement_node((yyvsp[(2) - (4)].node), (yyvsp[(4) - (4)].node)); ;}
    break;

  case 107:

/* Line 1455 of yacc.c  */
#line 333 "parser.y"
    { (yyval.node) = create_default_statement_node((yyvsp[(3) - (3)].node)); ;}
    break;

  case 108:

/* Line 1455 of yacc.c  */
#line 337 "parser.y"
    { (yyval.node) = create_compound_statement_node(NULL); ;}
    break;

  case 109:

/* Line 1455 of yacc.c  */
#line 338 "parser.y"
    { (yyval.node) = create_compound_statement_node((yyvsp[(2) - (3)].list)); ;}
    break;

  case 110:

/* Line 1455 of yacc.c  */
#line 343 "parser.y"
    { (yyval.list) = create_list_node((yyvsp[(1) - (1)].node)); ;}
    break;

  case 111:

/* Line 1455 of yacc.c  */
#line 345 "parser.y"
    { (yyval.list) = append_to_list((yyvsp[(1) - (2)].list), (yyvsp[(2) - (2)].node)); ;}
    break;

  case 112:

/* Line 1455 of yacc.c  */
#line 349 "parser.y"
    { (yyval.node) = (yyvsp[(1) - (1)].node); ;}
    break;

  case 113:

/* Line 1455 of yacc.c  */
#line 350 "parser.y"
    { (yyval.node) = (yyvsp[(1) - (1)].node); ;}
    break;

  case 114:

/* Line 1455 of yacc.c  */
#line 354 "parser.y"
    { (yyval.node) = create_expression_statement_node(NULL); ;}
    break;

  case 115:

/* Line 1455 of yacc.c  */
#line 355 "parser.y"
    { (yyval.node) = create_expression_statement_node((yyvsp[(1) - (2)].node)); ;}
    break;

  case 116:

/* Line 1455 of yacc.c  */
#line 359 "parser.y"
    { (yyval.node) = create_if_statement_node((yyvsp[(3) - (5)].node), (yyvsp[(5) - (5)].node), NULL); ;}
    break;

  case 117:

/* Line 1455 of yacc.c  */
#line 360 "parser.y"
    { (yyval.node) = create_if_statement_node((yyvsp[(3) - (7)].node), (yyvsp[(5) - (7)].node), (yyvsp[(7) - (7)].node)); ;}
    break;

  case 118:

/* Line 1455 of yacc.c  */
#line 361 "parser.y"
    { (yyval.node) = create_switch_statement_node((yyvsp[(3) - (5)].node), (yyvsp[(5) - (5)].node)); ;}
    break;

  case 119:

/* Line 1455 of yacc.c  */
#line 365 "parser.y"
    { (yyval.node) = create_while_statement_node((yyvsp[(3) - (5)].node), (yyvsp[(5) - (5)].node)); ;}
    break;

  case 120:

/* Line 1455 of yacc.c  */
#line 367 "parser.y"
    { (yyval.node) = create_until_statement_node((yyvsp[(3) - (5)].node), (yyvsp[(5) - (5)].node)); ;}
    break;

  case 121:

/* Line 1455 of yacc.c  */
#line 368 "parser.y"
    { (yyval.node) = create_do_while_statement_node((yyvsp[(2) - (7)].node), (yyvsp[(5) - (7)].node)); ;}
    break;

  case 122:

/* Line 1455 of yacc.c  */
#line 369 "parser.y"
    { (yyval.node) = create_for_statement_node((yyvsp[(3) - (6)].node), (yyvsp[(4) - (6)].node), NULL, (yyvsp[(6) - (6)].node)); ;}
    break;

  case 123:

/* Line 1455 of yacc.c  */
#line 370 "parser.y"
    { (yyval.node) = create_for_statement_node((yyvsp[(3) - (7)].node), (yyvsp[(4) - (7)].node), (yyvsp[(5) - (7)].node), (yyvsp[(7) - (7)].node)); ;}
    break;

  case 124:

/* Line 1455 of yacc.c  */
#line 371 "parser.y"
    { (yyval.node) = create_for_statement_node((yyvsp[(3) - (6)].node), (yyvsp[(4) - (6)].node), NULL, (yyvsp[(6) - (6)].node)); ;}
    break;

  case 125:

/* Line 1455 of yacc.c  */
#line 372 "parser.y"
    { (yyval.node) = create_for_statement_node((yyvsp[(3) - (7)].node), (yyvsp[(4) - (7)].node), (yyvsp[(5) - (7)].node), (yyvsp[(7) - (7)].node)); ;}
    break;

  case 126:

/* Line 1455 of yacc.c  */
#line 376 "parser.y"
    { (yyval.node) = create_goto_statement_node((yyvsp[(2) - (3)].str)); ;}
    break;

  case 127:

/* Line 1455 of yacc.c  */
#line 377 "parser.y"
    { (yyval.node) = create_node(NODE_CONTINUE_STATEMENT); ;}
    break;

  case 128:

/* Line 1455 of yacc.c  */
#line 378 "parser.y"
    { (yyval.node) = create_node(NODE_BREAK_STATEMENT); ;}
    break;

  case 129:

/* Line 1455 of yacc.c  */
#line 379 "parser.y"
    { (yyval.node) = create_return_statement_node(NULL); ;}
    break;

  case 130:

/* Line 1455 of yacc.c  */
#line 380 "parser.y"
    { (yyval.node) = create_return_statement_node((yyvsp[(2) - (3)].node)); ;}
    break;

  case 131:

/* Line 1455 of yacc.c  */
#line 385 "parser.y"
    { (yyval.node) = create_declaration_node((yyvsp[(1) - (2)].list), NULL); ;}
    break;

  case 132:

/* Line 1455 of yacc.c  */
#line 386 "parser.y"
    { (yyval.node) = create_declaration_node((yyvsp[(1) - (3)].list), (yyvsp[(2) - (3)].list)); ;}
    break;

  case 133:

/* Line 1455 of yacc.c  */
#line 387 "parser.y"
    { add_typename((yyvsp[(2) - (3)].str)); (yyval.node) = create_class_node((yyvsp[(2) - (3)].str), NULL, NULL); ;}
    break;

  case 134:

/* Line 1455 of yacc.c  */
#line 392 "parser.y"
    { (yyval.list) = create_list_node((yyvsp[(1) - (1)].node)); ;}
    break;

  case 135:

/* Line 1455 of yacc.c  */
#line 393 "parser.y"
    { (yyval.list) = append_to_list((yyvsp[(1) - (2)].list), (yyvsp[(2) - (2)].node)); ;}
    break;

  case 136:

/* Line 1455 of yacc.c  */
#line 397 "parser.y"
    { (yyval.node) = (yyvsp[(1) - (1)].node); ;}
    break;

  case 137:

/* Line 1455 of yacc.c  */
#line 398 "parser.y"
    { (yyval.node) = (yyvsp[(1) - (1)].node); ;}
    break;

  case 138:

/* Line 1455 of yacc.c  */
#line 399 "parser.y"
    { (yyval.node) = (yyvsp[(1) - (1)].node); ;}
    break;

  case 139:

/* Line 1455 of yacc.c  */
#line 400 "parser.y"
    { (yyval.node) = create_specifier_node(FRIEND); ;}
    break;

  case 140:

/* Line 1455 of yacc.c  */
#line 405 "parser.y"
    { (yyval.list) = create_list_node((yyvsp[(1) - (1)].node)); ;}
    break;

  case 141:

/* Line 1455 of yacc.c  */
#line 407 "parser.y"
    { (yyval.list) = append_to_list((yyvsp[(1) - (3)].list), (yyvsp[(3) - (3)].node)); ;}
    break;

  case 142:

/* Line 1455 of yacc.c  */
#line 411 "parser.y"
    { (yyval.node) = create_init_declarator_node((yyvsp[(1) - (1)].node), NULL); ;}
    break;

  case 143:

/* Line 1455 of yacc.c  */
#line 412 "parser.y"
    { (yyval.node) = create_init_declarator_node((yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node)); ;}
    break;

  case 144:

/* Line 1455 of yacc.c  */
#line 416 "parser.y"
    { (yyval.node) = create_specifier_node(TYPEDEF); ;}
    break;

  case 145:

/* Line 1455 of yacc.c  */
#line 417 "parser.y"
    { (yyval.node) = create_specifier_node(EXTERN); ;}
    break;

  case 146:

/* Line 1455 of yacc.c  */
#line 418 "parser.y"
    { (yyval.node) = create_specifier_node(STATIC); ;}
    break;

  case 147:

/* Line 1455 of yacc.c  */
#line 419 "parser.y"
    { (yyval.node) = create_specifier_node(AUTO); ;}
    break;

  case 148:

/* Line 1455 of yacc.c  */
#line 420 "parser.y"
    { (yyval.node) = create_specifier_node(REGISTER); ;}
    break;

  case 149:

/* Line 1455 of yacc.c  */
#line 424 "parser.y"
    { (yyval.node) = create_specifier_node(VOID); ;}
    break;

  case 150:

/* Line 1455 of yacc.c  */
#line 425 "parser.y"
    { (yyval.node) = create_specifier_node(CHAR); ;}
    break;

  case 151:

/* Line 1455 of yacc.c  */
#line 426 "parser.y"
    { (yyval.node) = create_specifier_node(SHORT); ;}
    break;

  case 152:

/* Line 1455 of yacc.c  */
#line 427 "parser.y"
    { (yyval.node) = create_specifier_node(INT); ;}
    break;

  case 153:

/* Line 1455 of yacc.c  */
#line 428 "parser.y"
    { (yyval.node) = create_specifier_node(LONG); ;}
    break;

  case 154:

/* Line 1455 of yacc.c  */
#line 429 "parser.y"
    { (yyval.node) = create_specifier_node(FLOAT); ;}
    break;

  case 155:

/* Line 1455 of yacc.c  */
#line 430 "parser.y"
    { (yyval.node) = create_specifier_node(DOUBLE); ;}
    break;

  case 156:

/* Line 1455 of yacc.c  */
#line 431 "parser.y"
    { (yyval.node) = create_specifier_node(SIGNED); ;}
    break;

  case 157:

/* Line 1455 of yacc.c  */
#line 432 "parser.y"
    { (yyval.node) = create_specifier_node(UNSIGNED); ;}
    break;

  case 158:

/* Line 1455 of yacc.c  */
#line 433 "parser.y"
    { (yyval.node) = create_specifier_node(BOOL); ;}
    break;

  case 159:

/* Line 1455 of yacc.c  */
#line 434 "parser.y"
    { (yyval.node) = create_specifier_node(STRING); ;}
    break;

  case 160:

/* Line 1455 of yacc.c  */
#line 435 "parser.y"
    { (yyval.node) = create_specifier_node(FILE_TYPE); ;}
    break;

  case 161:

/* Line 1455 of yacc.c  */
#line 436 "parser.y"
    { (yyval.node) = (yyvsp[(1) - (1)].node); ;}
    break;

  case 162:

/* Line 1455 of yacc.c  */
#line 437 "parser.y"
    { (yyval.node) = (yyvsp[(1) - (1)].node); ;}
    break;

  case 163:

/* Line 1455 of yacc.c  */
#line 438 "parser.y"
    { (yyval.node) = (yyvsp[(1) - (1)].node); ;}
    break;

  case 164:

/* Line 1455 of yacc.c  */
#line 439 "parser.y"
    { (yyval.node) = create_typename_node((yyvsp[(1) - (1)].str)); ;}
    break;

  case 165:

/* Line 1455 of yacc.c  */
#line 443 "parser.y"
    { (yyval.node) = create_specifier_node(CONST); ;}
    break;

  case 166:

/* Line 1455 of yacc.c  */
#line 444 "parser.y"
    { (yyval.node) = create_specifier_node(VOLATILE); ;}
    break;

  case 167:

/* Line 1455 of yacc.c  */
#line 448 "parser.y"
    { add_typename((yyvsp[(2) - (5)].str)); (yyval.node) = create_struct_or_union_specifier_node(STRUCT, (yyvsp[(2) - (5)].str), (yyvsp[(4) - (5)].list)); ;}
    break;

  case 168:

/* Line 1455 of yacc.c  */
#line 449 "parser.y"
    { add_typename((yyvsp[(2) - (5)].str)); (yyval.node) = create_struct_or_union_specifier_node(UNION, (yyvsp[(2) - (5)].str), (yyvsp[(4) - (5)].list)); ;}
    break;

  case 169:

/* Line 1455 of yacc.c  */
#line 450 "parser.y"
    { (yyval.node) = create_struct_or_union_specifier_node(STRUCT, NULL, (yyvsp[(3) - (4)].list)); ;}
    break;

  case 170:

/* Line 1455 of yacc.c  */
#line 451 "parser.y"
    { (yyval.node) = create_struct_or_union_specifier_node(UNION, NULL, (yyvsp[(3) - (4)].list)); ;}
    break;

  case 171:

/* Line 1455 of yacc.c  */
#line 452 "parser.y"
    { (yyval.node) = create_struct_or_union_specifier_node(STRUCT, (yyvsp[(2) - (2)].str), NULL); ;}
    break;

  case 172:

/* Line 1455 of yacc.c  */
#line 453 "parser.y"
    { (yyval.node) = create_struct_or_union_specifier_node(UNION, (yyvsp[(2) - (2)].str), NULL); ;}
    break;

  case 173:

/* Line 1455 of yacc.c  */
#line 458 "parser.y"
    { (yyval.list) = create_list_node((yyvsp[(1) - (1)].node)); ;}
    break;

  case 174:

/* Line 1455 of yacc.c  */
#line 460 "parser.y"
    { (yyval.list) = append_to_list((yyvsp[(1) - (2)].list), (yyvsp[(2) - (2)].node)); ;}
    break;

  case 175:

/* Line 1455 of yacc.c  */
#line 464 "parser.y"
    { (yyval.node) = create_declaration_node((yyvsp[(1) - (3)].list), (yyvsp[(2) - (3)].list)); ;}
    break;

  case 176:

/* Line 1455 of yacc.c  */
#line 469 "parser.y"
    { (yyval.list) = create_list_node((yyvsp[(1) - (1)].node)); ;}
    break;

  case 177:

/* Line 1455 of yacc.c  */
#line 471 "parser.y"
    { (yyval.list) = append_to_list((yyvsp[(1) - (3)].list), (yyvsp[(3) - (3)].node)); ;}
    break;

  case 178:

/* Line 1455 of yacc.c  */
#line 475 "parser.y"
    { (yyval.node) = create_init_declarator_node((yyvsp[(1) - (1)].node), NULL); ;}
    break;

  case 179:

/* Line 1455 of yacc.c  */
#line 477 "parser.y"
    { /* Bitfield support, can ignore for now */ (yyval.node) = (yyvsp[(1) - (3)].node); ;}
    break;

  case 180:

/* Line 1455 of yacc.c  */
#line 481 "parser.y"
    { (yyval.node) = create_enum_specifier_node(NULL, (yyvsp[(3) - (4)].list)); ;}
    break;

  case 181:

/* Line 1455 of yacc.c  */
#line 482 "parser.y"
    { add_typename((yyvsp[(2) - (5)].str)); (yyval.node) = create_enum_specifier_node((yyvsp[(2) - (5)].str), (yyvsp[(4) - (5)].list)); ;}
    break;

  case 182:

/* Line 1455 of yacc.c  */
#line 483 "parser.y"
    { (yyval.node) = create_enum_specifier_node((yyvsp[(2) - (2)].str), NULL); ;}
    break;

  case 183:

/* Line 1455 of yacc.c  */
#line 488 "parser.y"
    { (yyval.list) = create_list_node((yyvsp[(1) - (1)].node)); ;}
    break;

  case 184:

/* Line 1455 of yacc.c  */
#line 490 "parser.y"
    { (yyval.list) = append_to_list((yyvsp[(1) - (3)].list), (yyvsp[(3) - (3)].node)); ;}
    break;

  case 185:

/* Line 1455 of yacc.c  */
#line 494 "parser.y"
    { (yyval.node) = create_enumerator_node((yyvsp[(1) - (1)].str), NULL); ;}
    break;

  case 186:

/* Line 1455 of yacc.c  */
#line 495 "parser.y"
    { (yyval.node) = create_enumerator_node((yyvsp[(1) - (3)].str), (yyvsp[(3) - (3)].node)); ;}
    break;

  case 187:

/* Line 1455 of yacc.c  */
#line 499 "parser.y"
    { add_typename((yyvsp[(2) - (6)].node)->data.stringValue); (yyval.node) = create_class_node((yyvsp[(2) - (6)].node)->data.stringValue, (yyvsp[(3) - (6)].list), (yyvsp[(5) - (6)].list)); ;}
    break;

  case 188:

/* Line 1455 of yacc.c  */
#line 503 "parser.y"
    { (yyval.node) = create_identifier_node((yyvsp[(1) - (1)].str)); ;}
    break;

  case 189:

/* Line 1455 of yacc.c  */
#line 504 "parser.y"
    { (yyval.node) = create_typename_node((yyvsp[(1) - (1)].str)); ;}
    break;

  case 190:

/* Line 1455 of yacc.c  */
#line 508 "parser.y"
    { (yyval.list) = NULL; ;}
    break;

  case 191:

/* Line 1455 of yacc.c  */
#line 509 "parser.y"
    { (yyval.list) = create_list_node((yyvsp[(2) - (2)].node)); ;}
    break;

  case 192:

/* Line 1455 of yacc.c  */
#line 510 "parser.y"
    { (yyval.list) = append_to_list((yyvsp[(1) - (3)].list), (yyvsp[(3) - (3)].node)); ;}
    break;

  case 193:

/* Line 1455 of yacc.c  */
#line 514 "parser.y"
    { (yyval.node) = create_base_class_node((yyvsp[(1) - (2)].node)->data.specifier, (yyvsp[(2) - (2)].node)); ;}
    break;

  case 194:

/* Line 1455 of yacc.c  */
#line 515 "parser.y"
    { (yyval.node) = create_base_class_node(PRIVATE, (yyvsp[(1) - (1)].node)); ;}
    break;

  case 195:

/* Line 1455 of yacc.c  */
#line 519 "parser.y"
    { (yyval.node) = create_access_specifier_node(PUBLIC); ;}
    break;

  case 196:

/* Line 1455 of yacc.c  */
#line 520 "parser.y"
    { (yyval.node) = create_access_specifier_node(PRIVATE); ;}
    break;

  case 197:

/* Line 1455 of yacc.c  */
#line 521 "parser.y"
    { (yyval.node) = create_access_specifier_node(PROTECTED); ;}
    break;

  case 198:

/* Line 1455 of yacc.c  */
#line 525 "parser.y"
    { (yyval.list) = NULL; ;}
    break;

  case 199:

/* Line 1455 of yacc.c  */
#line 526 "parser.y"
    { (yyval.list) = (yyvsp[(1) - (2)].list) ? append_to_list((yyvsp[(1) - (2)].list), (yyvsp[(2) - (2)].node)) : create_list_node((yyvsp[(2) - (2)].node)); ;}
    break;

  case 202:

/* Line 1455 of yacc.c  */
#line 532 "parser.y"
    { (yyval.node) = (yyvsp[(1) - (2)].node); ;}
    break;

  case 203:

/* Line 1455 of yacc.c  */
#line 537 "parser.y"
    { (yyval.list) = create_list_node(create_identifier_node((yyvsp[(1) - (2)].str))); ;}
    break;

  case 204:

/* Line 1455 of yacc.c  */
#line 538 "parser.y"
    { (yyval.list) = create_list_node(create_typename_node((yyvsp[(1) - (2)].str))); ;}
    break;

  case 205:

/* Line 1455 of yacc.c  */
#line 539 "parser.y"
    { (yyval.list) = append_to_list((yyvsp[(1) - (3)].list), create_identifier_node((yyvsp[(2) - (3)].str))); ;}
    break;

  case 206:

/* Line 1455 of yacc.c  */
#line 540 "parser.y"
    { (yyval.list) = append_to_list((yyvsp[(1) - (3)].list), create_typename_node((yyvsp[(2) - (3)].str))); ;}
    break;

  case 207:

/* Line 1455 of yacc.c  */
#line 544 "parser.y"
    { (yyval.node) = create_qualified_id_node((yyvsp[(1) - (2)].list), create_identifier_node((yyvsp[(2) - (2)].str))); ;}
    break;

  case 208:

/* Line 1455 of yacc.c  */
#line 545 "parser.y"
    { (yyval.node) = create_qualified_id_node((yyvsp[(1) - (2)].list), create_typename_node((yyvsp[(2) - (2)].str))); ;}
    break;

  case 209:

/* Line 1455 of yacc.c  */
#line 549 "parser.y"
    { 
                                                    // Check what kind of node the ptr_operator created
                                                    if ((yyvsp[(1) - (2)].node)->type == NODE_POINTER) {
                                                        (yyval.node) = create_pointer_declarator_node((yyvsp[(1) - (2)].node), (yyvsp[(2) - (2)].node));
                                                    } else { // It must be a reference
                                                        (yyval.node) = create_reference_declarator_node((yyvsp[(2) - (2)].node));
                                                        free((yyvsp[(1) - (2)].node)); // Free the temporary specifier node
                                                    }
                                                ;}
    break;

  case 211:

/* Line 1455 of yacc.c  */
#line 563 "parser.y"
    { (yyval.node) = create_identifier_node((yyvsp[(1) - (1)].str)); ;}
    break;

  case 212:

/* Line 1455 of yacc.c  */
#line 564 "parser.y"
    { (yyval.node) = (yyvsp[(1) - (1)].node); ;}
    break;

  case 213:

/* Line 1455 of yacc.c  */
#line 565 "parser.y"
    { (yyval.node) = (yyvsp[(2) - (3)].node); ;}
    break;

  case 214:

/* Line 1455 of yacc.c  */
#line 566 "parser.y"
    { (yyval.node) = create_array_declarator_node((yyvsp[(1) - (4)].node), (yyvsp[(3) - (4)].node)); ;}
    break;

  case 215:

/* Line 1455 of yacc.c  */
#line 567 "parser.y"
    { (yyval.node) = create_array_declarator_node((yyvsp[(1) - (3)].node), NULL); ;}
    break;

  case 216:

/* Line 1455 of yacc.c  */
#line 568 "parser.y"
    { (yyval.node) = create_function_declarator_node((yyvsp[(1) - (4)].node), (yyvsp[(3) - (4)].list)); ;}
    break;

  case 217:

/* Line 1455 of yacc.c  */
#line 569 "parser.y"
    { (yyval.node) = create_function_declarator_node((yyvsp[(1) - (3)].node), NULL); ;}
    break;

  case 218:

/* Line 1455 of yacc.c  */
#line 574 "parser.y"
    { (yyval.node) = create_pointer_node((yyvsp[(2) - (2)].list), NULL); ;}
    break;

  case 219:

/* Line 1455 of yacc.c  */
#line 575 "parser.y"
    { (yyval.node) = create_specifier_node('&'); ;}
    break;

  case 220:

/* Line 1455 of yacc.c  */
#line 579 "parser.y"
    { (yyval.list) = NULL; ;}
    break;

  case 221:

/* Line 1455 of yacc.c  */
#line 580 "parser.y"
    { (yyval.list) = (yyvsp[(1) - (1)].list); ;}
    break;

  case 222:

/* Line 1455 of yacc.c  */
#line 585 "parser.y"
    { (yyval.list) = create_list_node((yyvsp[(1) - (1)].node)); ;}
    break;

  case 223:

/* Line 1455 of yacc.c  */
#line 587 "parser.y"
    { (yyval.list) = append_to_list((yyvsp[(1) - (2)].list), (yyvsp[(2) - (2)].node)); ;}
    break;

  case 225:

/* Line 1455 of yacc.c  */
#line 592 "parser.y"
    { (yyval.list) = (yyvsp[(1) - (3)].list); ;}
    break;

  case 226:

/* Line 1455 of yacc.c  */
#line 597 "parser.y"
    { (yyval.list) = create_list_node((yyvsp[(1) - (1)].node)); ;}
    break;

  case 227:

/* Line 1455 of yacc.c  */
#line 599 "parser.y"
    { (yyval.list) = append_to_list((yyvsp[(1) - (3)].list), (yyvsp[(3) - (3)].node)); ;}
    break;

  case 228:

/* Line 1455 of yacc.c  */
#line 604 "parser.y"
    { (yyval.node) = create_parameter_declaration_node((yyvsp[(1) - (2)].list), (yyvsp[(2) - (2)].node)); ;}
    break;

  case 229:

/* Line 1455 of yacc.c  */
#line 606 "parser.y"
    { (yyval.node) = create_parameter_declaration_node((yyvsp[(1) - (2)].list), (yyvsp[(2) - (2)].node)); ;}
    break;

  case 230:

/* Line 1455 of yacc.c  */
#line 608 "parser.y"
    { (yyval.node) = create_parameter_declaration_node((yyvsp[(1) - (1)].list), NULL); ;}
    break;

  case 231:

/* Line 1455 of yacc.c  */
#line 612 "parser.y"
    { (yyval.node) = create_declaration_node((yyvsp[(1) - (2)].list), create_list_node((yyvsp[(2) - (2)].node))); ;}
    break;

  case 232:

/* Line 1455 of yacc.c  */
#line 613 "parser.y"
    { (yyval.node) = create_declaration_node((yyvsp[(1) - (1)].list), NULL); ;}
    break;

  case 234:

/* Line 1455 of yacc.c  */
#line 618 "parser.y"
    { (yyval.node) = create_pointer_declarator_node((yyvsp[(1) - (2)].node), (yyvsp[(2) - (2)].node)); ;}
    break;

  case 236:

/* Line 1455 of yacc.c  */
#line 623 "parser.y"
    { (yyval.node) = (yyvsp[(2) - (3)].node); ;}
    break;

  case 237:

/* Line 1455 of yacc.c  */
#line 624 "parser.y"
    { (yyval.node) = create_array_declarator_node(NULL, NULL); ;}
    break;

  case 238:

/* Line 1455 of yacc.c  */
#line 625 "parser.y"
    { (yyval.node) = create_array_declarator_node(NULL, (yyvsp[(2) - (3)].node)); ;}
    break;

  case 239:

/* Line 1455 of yacc.c  */
#line 626 "parser.y"
    { (yyval.node) = create_array_declarator_node((yyvsp[(1) - (3)].node), NULL); ;}
    break;

  case 240:

/* Line 1455 of yacc.c  */
#line 627 "parser.y"
    { (yyval.node) = create_array_declarator_node((yyvsp[(1) - (4)].node), (yyvsp[(3) - (4)].node)); ;}
    break;

  case 241:

/* Line 1455 of yacc.c  */
#line 628 "parser.y"
    { (yyval.node) = create_function_declarator_node(NULL, NULL); ;}
    break;

  case 242:

/* Line 1455 of yacc.c  */
#line 629 "parser.y"
    { (yyval.node) = create_function_declarator_node(NULL, (yyvsp[(2) - (3)].list)); ;}
    break;

  case 243:

/* Line 1455 of yacc.c  */
#line 630 "parser.y"
    { (yyval.node) = create_function_declarator_node((yyvsp[(1) - (3)].node), NULL); ;}
    break;

  case 244:

/* Line 1455 of yacc.c  */
#line 631 "parser.y"
    { (yyval.node) = create_function_declarator_node((yyvsp[(1) - (4)].node), (yyvsp[(3) - (4)].list)); ;}
    break;

  case 246:

/* Line 1455 of yacc.c  */
#line 636 "parser.y"
    { (yyval.node) = create_node(NODE_INITIALIZER_LIST); (yyval.node)->data.items_list = (yyvsp[(2) - (3)].list); ;}
    break;

  case 247:

/* Line 1455 of yacc.c  */
#line 637 "parser.y"
    { (yyval.node) = create_node(NODE_INITIALIZER_LIST); (yyval.node)->data.items_list = (yyvsp[(2) - (4)].list); ;}
    break;

  case 248:

/* Line 1455 of yacc.c  */
#line 642 "parser.y"
    { (yyval.list) = create_list_node((yyvsp[(1) - (1)].node)); ;}
    break;

  case 249:

/* Line 1455 of yacc.c  */
#line 644 "parser.y"
    { (yyval.list) = append_to_list((yyvsp[(1) - (3)].list), (yyvsp[(3) - (3)].node)); ;}
    break;



/* Line 1455 of yacc.c  */
#line 3895 "parser.tab.c"
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



/* Line 1675 of yacc.c  */
#line 646 "parser.y"


void yyerror(const char *s) {
    fprintf(stderr, "Syntax Error on line %d near '%s': %s\n", yylineno, yytext, s);
    parse_errors++;
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

    if (parse_errors == 0) {
        printf("\n--- Performing syntax analysis ---\n");
        printf("\n--- Parsing Successful ---\n");
        printf("\n--- Printing Abstract SYntax Tree after syntax analysis phase ---\n");
        if (root) print_ast(root, 0);
        printf("\n--- Performing Semantic Analysis ---\n");
        if (analyze_ast(root)) {
            printf("--- Semantic Analysis Successful ---\n");
            // printf("\n--- Final Symbol Table ---\n");
            // print_symbol_table(get_current_scope(), 0); // <-- ADD THIS
            // printf("--- End of Symbol Table ---\n\n");
            printf("\n--- Printing Abstract Syntax Tree after semantic analysis phase ---\n");
            if (root) print_ast(root, 0);
            
            // printf("\n--- Generating Three-Address Code ---\n");
            // generate_tac(root);
            // print_tac();
            // print_symbol_table(current_scope, 0);

        } else {
            printf("\n--- Semantic Analysis Failed ---\n");
        }
    } else {
        printf("\n--- Parsing Failed ---\n");
    }
    
    if (yyin != stdin) {
        fclose(yyin);
    }
    
    return (parse_errors > 0);
}

