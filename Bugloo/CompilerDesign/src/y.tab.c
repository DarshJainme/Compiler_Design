/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

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

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output, and Bison version.  */
#define YYBISON 30802

/* Bison version string.  */
#define YYBISON_VERSION "3.8.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 1 "parser.y"

    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>

    extern int yylex();
    extern int yylineno;
    extern FILE *yyin;
    void yyerror(const char *s);

#line 82 "y.tab.c"

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

/* Use api.header.include to #include this header
   instead of duplicating it here.  */
#ifndef YY_YY_Y_TAB_H_INCLUDED
# define YY_YY_Y_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token kinds.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    YYEMPTY = -2,
    YYEOF = 0,                     /* "end of file"  */
    YYerror = 256,                 /* error  */
    YYUNDEF = 257,                 /* "invalid token"  */
    KEYWORD_INT = 258,             /* KEYWORD_INT  */
    KEYWORD_CHAR = 259,            /* KEYWORD_CHAR  */
    KEYWORD_FLOAT = 260,           /* KEYWORD_FLOAT  */
    KEYWORD_DOUBLE = 261,          /* KEYWORD_DOUBLE  */
    KEYWORD_VOID = 262,            /* KEYWORD_VOID  */
    KEYWORD_IF = 263,              /* KEYWORD_IF  */
    KEYWORD_ELSE = 264,            /* KEYWORD_ELSE  */
    KEYWORD_FOR = 265,             /* KEYWORD_FOR  */
    KEYWORD_WHILE = 266,           /* KEYWORD_WHILE  */
    KEYWORD_RETURN = 267,          /* KEYWORD_RETURN  */
    KEYWORD_BREAK = 268,           /* KEYWORD_BREAK  */
    KEYWORD_CONTINUE = 269,        /* KEYWORD_CONTINUE  */
    KEYWORD_DO = 270,              /* KEYWORD_DO  */
    KEYWORD_SWITCH = 271,          /* KEYWORD_SWITCH  */
    KEYWORD_CASE = 272,            /* KEYWORD_CASE  */
    KEYWORD_DEFAULT = 273,         /* KEYWORD_DEFAULT  */
    KEYWORD_GOTO = 274,            /* KEYWORD_GOTO  */
    KEYWORD_ENUM = 275,            /* KEYWORD_ENUM  */
    KEYWORD_STRUCT = 276,          /* KEYWORD_STRUCT  */
    KEYWORD_UNION = 277,           /* KEYWORD_UNION  */
    KEYWORD_TYPEDEF = 278,         /* KEYWORD_TYPEDEF  */
    KEYWORD_STATIC = 279,          /* KEYWORD_STATIC  */
    KEYWORD_AUTO = 280,            /* KEYWORD_AUTO  */
    KEYWORD_CONST = 281,           /* KEYWORD_CONST  */
    KEYWORD_SIZEOF = 282,          /* KEYWORD_SIZEOF  */
    KEYWORD_CLASS = 283,           /* KEYWORD_CLASS  */
    KEYWORD_PUBLIC = 284,          /* KEYWORD_PUBLIC  */
    KEYWORD_PRIVATE = 285,         /* KEYWORD_PRIVATE  */
    KEYWORD_PROTECTED = 286,       /* KEYWORD_PROTECTED  */
    KEYWORD_NEW = 287,             /* KEYWORD_NEW  */
    KEYWORD_DELETE = 288,          /* KEYWORD_DELETE  */
    KEYWORD_USING = 289,           /* KEYWORD_USING  */
    KEYWORD_NAMESPACE = 290,       /* KEYWORD_NAMESPACE  */
    KEYWORD_BOOL = 291,            /* KEYWORD_BOOL  */
    KEYWORD_LONG = 292,            /* KEYWORD_LONG  */
    KEYWORD_TRUE = 293,            /* KEYWORD_TRUE  */
    KEYWORD_FALSE = 294,           /* KEYWORD_FALSE  */
    IDENTIFIER = 295,              /* IDENTIFIER  */
    INTEGER_LITERAL = 296,         /* INTEGER_LITERAL  */
    FLOAT_LITERAL = 297,           /* FLOAT_LITERAL  */
    CHAR_LITERAL = 298,            /* CHAR_LITERAL  */
    STRING_LITERAL = 299,          /* STRING_LITERAL  */
    OP_PLUS = 300,                 /* OP_PLUS  */
    OP_MINUS = 301,                /* OP_MINUS  */
    OP_MULTIPLY = 302,             /* OP_MULTIPLY  */
    OP_DIVIDE = 303,               /* OP_DIVIDE  */
    OP_MODULO = 304,               /* OP_MODULO  */
    OP_INCREMENT = 305,            /* OP_INCREMENT  */
    OP_DECREMENT = 306,            /* OP_DECREMENT  */
    OP_ASSIGN = 307,               /* OP_ASSIGN  */
    OP_ADD_ASSIGN = 308,           /* OP_ADD_ASSIGN  */
    OP_SUB_ASSIGN = 309,           /* OP_SUB_ASSIGN  */
    OP_MUL_ASSIGN = 310,           /* OP_MUL_ASSIGN  */
    OP_DIV_ASSIGN = 311,           /* OP_DIV_ASSIGN  */
    OP_MOD_ASSIGN = 312,           /* OP_MOD_ASSIGN  */
    OP_LSHIFT = 313,               /* OP_LSHIFT  */
    OP_RSHIFT = 314,               /* OP_RSHIFT  */
    OP_EQUAL = 315,                /* OP_EQUAL  */
    OP_NOT_EQUAL = 316,            /* OP_NOT_EQUAL  */
    OP_LESS_THAN = 317,            /* OP_LESS_THAN  */
    OP_GREATER_THAN = 318,         /* OP_GREATER_THAN  */
    OP_LESS_THAN_EQUAL = 319,      /* OP_LESS_THAN_EQUAL  */
    OP_GREATER_THAN_EQUAL = 320,   /* OP_GREATER_THAN_EQUAL  */
    OP_LOGICAL_AND = 321,          /* OP_LOGICAL_AND  */
    OP_LOGICAL_OR = 322,           /* OP_LOGICAL_OR  */
    OP_LOGICAL_NOT = 323,          /* OP_LOGICAL_NOT  */
    OP_BITWISE_AND = 324,          /* OP_BITWISE_AND  */
    OP_BITWISE_OR = 325,           /* OP_BITWISE_OR  */
    OP_BITWISE_XOR = 326,          /* OP_BITWISE_XOR  */
    OP_BITWISE_NOT = 327,          /* OP_BITWISE_NOT  */
    OP_AND = 328,                  /* OP_AND  */
    OP_OR = 329,                   /* OP_OR  */
    OP_XOR = 330,                  /* OP_XOR  */
    OP_NOT = 331,                  /* OP_NOT  */
    SEMICOLON = 332,               /* SEMICOLON  */
    COMMA = 333,                   /* COMMA  */
    OPEN_PARENTHESIS = 334,        /* OPEN_PARENTHESIS  */
    CLOSE_PARENTHESIS = 335,       /* CLOSE_PARENTHESIS  */
    OPEN_BRACKET = 336,            /* OPEN_BRACKET  */
    CLOSE_BRACKET = 337,           /* CLOSE_BRACKET  */
    OPEN_CURLY_BRACKET = 338,      /* OPEN_CURLY_BRACKET  */
    CLOSE_CURLY_BRACKET = 339,     /* CLOSE_CURLY_BRACKET  */
    COLON = 340,                   /* COLON  */
    POINTER_ARROW = 341,           /* POINTER_ARROW  */
    DOT = 342,                     /* DOT  */
    REFERENCE_AMPERSAND = 343,     /* REFERENCE_AMPERSAND  */
    POINTER_STAR = 344,            /* POINTER_STAR  */
    QUESTION_MARK = 345,           /* QUESTION_MARK  */
    PREPROCESSOR_DIRECTIVE = 346,  /* PREPROCESSOR_DIRECTIVE  */
    SINGLE_LINE_COMMENT = 347,     /* SINGLE_LINE_COMMENT  */
    MULTI_LINE_COMMENT = 348,      /* MULTI_LINE_COMMENT  */
    NEWLINE = 349,                 /* NEWLINE  */
    WHITESPACE = 350,              /* WHITESPACE  */
    ERROR_TOKEN = 351,             /* ERROR_TOKEN  */
    UNKNOWN = 352,                 /* UNKNOWN  */
    END_OF_FILE = 353              /* END_OF_FILE  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif
/* Token kinds.  */
#define YYEMPTY -2
#define YYEOF 0
#define YYerror 256
#define YYUNDEF 257
#define KEYWORD_INT 258
#define KEYWORD_CHAR 259
#define KEYWORD_FLOAT 260
#define KEYWORD_DOUBLE 261
#define KEYWORD_VOID 262
#define KEYWORD_IF 263
#define KEYWORD_ELSE 264
#define KEYWORD_FOR 265
#define KEYWORD_WHILE 266
#define KEYWORD_RETURN 267
#define KEYWORD_BREAK 268
#define KEYWORD_CONTINUE 269
#define KEYWORD_DO 270
#define KEYWORD_SWITCH 271
#define KEYWORD_CASE 272
#define KEYWORD_DEFAULT 273
#define KEYWORD_GOTO 274
#define KEYWORD_ENUM 275
#define KEYWORD_STRUCT 276
#define KEYWORD_UNION 277
#define KEYWORD_TYPEDEF 278
#define KEYWORD_STATIC 279
#define KEYWORD_AUTO 280
#define KEYWORD_CONST 281
#define KEYWORD_SIZEOF 282
#define KEYWORD_CLASS 283
#define KEYWORD_PUBLIC 284
#define KEYWORD_PRIVATE 285
#define KEYWORD_PROTECTED 286
#define KEYWORD_NEW 287
#define KEYWORD_DELETE 288
#define KEYWORD_USING 289
#define KEYWORD_NAMESPACE 290
#define KEYWORD_BOOL 291
#define KEYWORD_LONG 292
#define KEYWORD_TRUE 293
#define KEYWORD_FALSE 294
#define IDENTIFIER 295
#define INTEGER_LITERAL 296
#define FLOAT_LITERAL 297
#define CHAR_LITERAL 298
#define STRING_LITERAL 299
#define OP_PLUS 300
#define OP_MINUS 301
#define OP_MULTIPLY 302
#define OP_DIVIDE 303
#define OP_MODULO 304
#define OP_INCREMENT 305
#define OP_DECREMENT 306
#define OP_ASSIGN 307
#define OP_ADD_ASSIGN 308
#define OP_SUB_ASSIGN 309
#define OP_MUL_ASSIGN 310
#define OP_DIV_ASSIGN 311
#define OP_MOD_ASSIGN 312
#define OP_LSHIFT 313
#define OP_RSHIFT 314
#define OP_EQUAL 315
#define OP_NOT_EQUAL 316
#define OP_LESS_THAN 317
#define OP_GREATER_THAN 318
#define OP_LESS_THAN_EQUAL 319
#define OP_GREATER_THAN_EQUAL 320
#define OP_LOGICAL_AND 321
#define OP_LOGICAL_OR 322
#define OP_LOGICAL_NOT 323
#define OP_BITWISE_AND 324
#define OP_BITWISE_OR 325
#define OP_BITWISE_XOR 326
#define OP_BITWISE_NOT 327
#define OP_AND 328
#define OP_OR 329
#define OP_XOR 330
#define OP_NOT 331
#define SEMICOLON 332
#define COMMA 333
#define OPEN_PARENTHESIS 334
#define CLOSE_PARENTHESIS 335
#define OPEN_BRACKET 336
#define CLOSE_BRACKET 337
#define OPEN_CURLY_BRACKET 338
#define CLOSE_CURLY_BRACKET 339
#define COLON 340
#define POINTER_ARROW 341
#define DOT 342
#define REFERENCE_AMPERSAND 343
#define POINTER_STAR 344
#define QUESTION_MARK 345
#define PREPROCESSOR_DIRECTIVE 346
#define SINGLE_LINE_COMMENT 347
#define MULTI_LINE_COMMENT 348
#define NEWLINE 349
#define WHITESPACE 350
#define ERROR_TOKEN 351
#define UNKNOWN 352
#define END_OF_FILE 353

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef int YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;


int yyparse (void);


#endif /* !YY_YY_Y_TAB_H_INCLUDED  */
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_KEYWORD_INT = 3,                /* KEYWORD_INT  */
  YYSYMBOL_KEYWORD_CHAR = 4,               /* KEYWORD_CHAR  */
  YYSYMBOL_KEYWORD_FLOAT = 5,              /* KEYWORD_FLOAT  */
  YYSYMBOL_KEYWORD_DOUBLE = 6,             /* KEYWORD_DOUBLE  */
  YYSYMBOL_KEYWORD_VOID = 7,               /* KEYWORD_VOID  */
  YYSYMBOL_KEYWORD_IF = 8,                 /* KEYWORD_IF  */
  YYSYMBOL_KEYWORD_ELSE = 9,               /* KEYWORD_ELSE  */
  YYSYMBOL_KEYWORD_FOR = 10,               /* KEYWORD_FOR  */
  YYSYMBOL_KEYWORD_WHILE = 11,             /* KEYWORD_WHILE  */
  YYSYMBOL_KEYWORD_RETURN = 12,            /* KEYWORD_RETURN  */
  YYSYMBOL_KEYWORD_BREAK = 13,             /* KEYWORD_BREAK  */
  YYSYMBOL_KEYWORD_CONTINUE = 14,          /* KEYWORD_CONTINUE  */
  YYSYMBOL_KEYWORD_DO = 15,                /* KEYWORD_DO  */
  YYSYMBOL_KEYWORD_SWITCH = 16,            /* KEYWORD_SWITCH  */
  YYSYMBOL_KEYWORD_CASE = 17,              /* KEYWORD_CASE  */
  YYSYMBOL_KEYWORD_DEFAULT = 18,           /* KEYWORD_DEFAULT  */
  YYSYMBOL_KEYWORD_GOTO = 19,              /* KEYWORD_GOTO  */
  YYSYMBOL_KEYWORD_ENUM = 20,              /* KEYWORD_ENUM  */
  YYSYMBOL_KEYWORD_STRUCT = 21,            /* KEYWORD_STRUCT  */
  YYSYMBOL_KEYWORD_UNION = 22,             /* KEYWORD_UNION  */
  YYSYMBOL_KEYWORD_TYPEDEF = 23,           /* KEYWORD_TYPEDEF  */
  YYSYMBOL_KEYWORD_STATIC = 24,            /* KEYWORD_STATIC  */
  YYSYMBOL_KEYWORD_AUTO = 25,              /* KEYWORD_AUTO  */
  YYSYMBOL_KEYWORD_CONST = 26,             /* KEYWORD_CONST  */
  YYSYMBOL_KEYWORD_SIZEOF = 27,            /* KEYWORD_SIZEOF  */
  YYSYMBOL_KEYWORD_CLASS = 28,             /* KEYWORD_CLASS  */
  YYSYMBOL_KEYWORD_PUBLIC = 29,            /* KEYWORD_PUBLIC  */
  YYSYMBOL_KEYWORD_PRIVATE = 30,           /* KEYWORD_PRIVATE  */
  YYSYMBOL_KEYWORD_PROTECTED = 31,         /* KEYWORD_PROTECTED  */
  YYSYMBOL_KEYWORD_NEW = 32,               /* KEYWORD_NEW  */
  YYSYMBOL_KEYWORD_DELETE = 33,            /* KEYWORD_DELETE  */
  YYSYMBOL_KEYWORD_USING = 34,             /* KEYWORD_USING  */
  YYSYMBOL_KEYWORD_NAMESPACE = 35,         /* KEYWORD_NAMESPACE  */
  YYSYMBOL_KEYWORD_BOOL = 36,              /* KEYWORD_BOOL  */
  YYSYMBOL_KEYWORD_LONG = 37,              /* KEYWORD_LONG  */
  YYSYMBOL_KEYWORD_TRUE = 38,              /* KEYWORD_TRUE  */
  YYSYMBOL_KEYWORD_FALSE = 39,             /* KEYWORD_FALSE  */
  YYSYMBOL_IDENTIFIER = 40,                /* IDENTIFIER  */
  YYSYMBOL_INTEGER_LITERAL = 41,           /* INTEGER_LITERAL  */
  YYSYMBOL_FLOAT_LITERAL = 42,             /* FLOAT_LITERAL  */
  YYSYMBOL_CHAR_LITERAL = 43,              /* CHAR_LITERAL  */
  YYSYMBOL_STRING_LITERAL = 44,            /* STRING_LITERAL  */
  YYSYMBOL_OP_PLUS = 45,                   /* OP_PLUS  */
  YYSYMBOL_OP_MINUS = 46,                  /* OP_MINUS  */
  YYSYMBOL_OP_MULTIPLY = 47,               /* OP_MULTIPLY  */
  YYSYMBOL_OP_DIVIDE = 48,                 /* OP_DIVIDE  */
  YYSYMBOL_OP_MODULO = 49,                 /* OP_MODULO  */
  YYSYMBOL_OP_INCREMENT = 50,              /* OP_INCREMENT  */
  YYSYMBOL_OP_DECREMENT = 51,              /* OP_DECREMENT  */
  YYSYMBOL_OP_ASSIGN = 52,                 /* OP_ASSIGN  */
  YYSYMBOL_OP_ADD_ASSIGN = 53,             /* OP_ADD_ASSIGN  */
  YYSYMBOL_OP_SUB_ASSIGN = 54,             /* OP_SUB_ASSIGN  */
  YYSYMBOL_OP_MUL_ASSIGN = 55,             /* OP_MUL_ASSIGN  */
  YYSYMBOL_OP_DIV_ASSIGN = 56,             /* OP_DIV_ASSIGN  */
  YYSYMBOL_OP_MOD_ASSIGN = 57,             /* OP_MOD_ASSIGN  */
  YYSYMBOL_OP_LSHIFT = 58,                 /* OP_LSHIFT  */
  YYSYMBOL_OP_RSHIFT = 59,                 /* OP_RSHIFT  */
  YYSYMBOL_OP_EQUAL = 60,                  /* OP_EQUAL  */
  YYSYMBOL_OP_NOT_EQUAL = 61,              /* OP_NOT_EQUAL  */
  YYSYMBOL_OP_LESS_THAN = 62,              /* OP_LESS_THAN  */
  YYSYMBOL_OP_GREATER_THAN = 63,           /* OP_GREATER_THAN  */
  YYSYMBOL_OP_LESS_THAN_EQUAL = 64,        /* OP_LESS_THAN_EQUAL  */
  YYSYMBOL_OP_GREATER_THAN_EQUAL = 65,     /* OP_GREATER_THAN_EQUAL  */
  YYSYMBOL_OP_LOGICAL_AND = 66,            /* OP_LOGICAL_AND  */
  YYSYMBOL_OP_LOGICAL_OR = 67,             /* OP_LOGICAL_OR  */
  YYSYMBOL_OP_LOGICAL_NOT = 68,            /* OP_LOGICAL_NOT  */
  YYSYMBOL_OP_BITWISE_AND = 69,            /* OP_BITWISE_AND  */
  YYSYMBOL_OP_BITWISE_OR = 70,             /* OP_BITWISE_OR  */
  YYSYMBOL_OP_BITWISE_XOR = 71,            /* OP_BITWISE_XOR  */
  YYSYMBOL_OP_BITWISE_NOT = 72,            /* OP_BITWISE_NOT  */
  YYSYMBOL_OP_AND = 73,                    /* OP_AND  */
  YYSYMBOL_OP_OR = 74,                     /* OP_OR  */
  YYSYMBOL_OP_XOR = 75,                    /* OP_XOR  */
  YYSYMBOL_OP_NOT = 76,                    /* OP_NOT  */
  YYSYMBOL_SEMICOLON = 77,                 /* SEMICOLON  */
  YYSYMBOL_COMMA = 78,                     /* COMMA  */
  YYSYMBOL_OPEN_PARENTHESIS = 79,          /* OPEN_PARENTHESIS  */
  YYSYMBOL_CLOSE_PARENTHESIS = 80,         /* CLOSE_PARENTHESIS  */
  YYSYMBOL_OPEN_BRACKET = 81,              /* OPEN_BRACKET  */
  YYSYMBOL_CLOSE_BRACKET = 82,             /* CLOSE_BRACKET  */
  YYSYMBOL_OPEN_CURLY_BRACKET = 83,        /* OPEN_CURLY_BRACKET  */
  YYSYMBOL_CLOSE_CURLY_BRACKET = 84,       /* CLOSE_CURLY_BRACKET  */
  YYSYMBOL_COLON = 85,                     /* COLON  */
  YYSYMBOL_POINTER_ARROW = 86,             /* POINTER_ARROW  */
  YYSYMBOL_DOT = 87,                       /* DOT  */
  YYSYMBOL_REFERENCE_AMPERSAND = 88,       /* REFERENCE_AMPERSAND  */
  YYSYMBOL_POINTER_STAR = 89,              /* POINTER_STAR  */
  YYSYMBOL_QUESTION_MARK = 90,             /* QUESTION_MARK  */
  YYSYMBOL_PREPROCESSOR_DIRECTIVE = 91,    /* PREPROCESSOR_DIRECTIVE  */
  YYSYMBOL_SINGLE_LINE_COMMENT = 92,       /* SINGLE_LINE_COMMENT  */
  YYSYMBOL_MULTI_LINE_COMMENT = 93,        /* MULTI_LINE_COMMENT  */
  YYSYMBOL_NEWLINE = 94,                   /* NEWLINE  */
  YYSYMBOL_WHITESPACE = 95,                /* WHITESPACE  */
  YYSYMBOL_ERROR_TOKEN = 96,               /* ERROR_TOKEN  */
  YYSYMBOL_UNKNOWN = 97,                   /* UNKNOWN  */
  YYSYMBOL_END_OF_FILE = 98,               /* END_OF_FILE  */
  YYSYMBOL_YYACCEPT = 99,                  /* $accept  */
  YYSYMBOL_translation_unit = 100,         /* translation_unit  */
  YYSYMBOL_declaration_list = 101,         /* declaration_list  */
  YYSYMBOL_declaration = 102,              /* declaration  */
  YYSYMBOL_typedef_declaration = 103,      /* typedef_declaration  */
  YYSYMBOL_class_declaration = 104,        /* class_declaration  */
  YYSYMBOL_enum_declaration = 105,         /* enum_declaration  */
  YYSYMBOL_typedef_name = 106,             /* typedef_name  */
  YYSYMBOL_class_name = 107,               /* class_name  */
  YYSYMBOL_enum_name = 108,                /* enum_name  */
  YYSYMBOL_template_id = 109,              /* template_id  */
  YYSYMBOL_template_argument_list = 110,   /* template_argument_list  */
  YYSYMBOL_template_argument = 111         /* template_argument  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;




#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

/* Work around bug in HP-UX 11.23, which defines these macros
   incorrectly for preprocessor constants.  This workaround can likely
   be removed in 2023, as HPE has promised support for HP-UX 11.23
   (aka HP-UX 11i v2) only through the end of 2022; see Table 2 of
   <https://h20195.www2.hpe.com/V2/getpdf.aspx/4AA4-7673ENW.pdf>.  */
#ifdef __hpux
# undef UINT_LEAST8_MAX
# undef UINT_LEAST16_MAX
# define UINT_LEAST8_MAX 255
# define UINT_LEAST16_MAX 65535
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))


/* Stored state numbers (used for stacks). */
typedef yytype_int8 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif


#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YY_USE(E) ((void) (E))
#else
# define YY_USE(E) /* empty */
#endif

/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
#if defined __GNUC__ && ! defined __ICC && 406 <= __GNUC__ * 100 + __GNUC_MINOR__
# if __GNUC__ * 100 + __GNUC_MINOR__ < 407
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")
# else
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# endif
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

#if !defined yyoverflow

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
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
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
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* !defined yyoverflow */

#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  18
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   20

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  99
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  13
/* YYNRULES -- Number of rules.  */
#define YYNRULES  21
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  34

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   353


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK                     \
   ? YY_CAST (yysymbol_kind_t, yytranslate[YYX])        \
   : YYSYMBOL_YYUNDEF)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
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
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      95,    96,    97,    98
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int8 yyrline[] =
{
       0,    44,    44,    48,    49,    53,    54,    55,    59,    63,
      67,    70,    71,    75,    76,    80,    84,    88,    89,    93,
      94,    95
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if YYDEBUG || 0
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "\"end of file\"", "error", "\"invalid token\"", "KEYWORD_INT",
  "KEYWORD_CHAR", "KEYWORD_FLOAT", "KEYWORD_DOUBLE", "KEYWORD_VOID",
  "KEYWORD_IF", "KEYWORD_ELSE", "KEYWORD_FOR", "KEYWORD_WHILE",
  "KEYWORD_RETURN", "KEYWORD_BREAK", "KEYWORD_CONTINUE", "KEYWORD_DO",
  "KEYWORD_SWITCH", "KEYWORD_CASE", "KEYWORD_DEFAULT", "KEYWORD_GOTO",
  "KEYWORD_ENUM", "KEYWORD_STRUCT", "KEYWORD_UNION", "KEYWORD_TYPEDEF",
  "KEYWORD_STATIC", "KEYWORD_AUTO", "KEYWORD_CONST", "KEYWORD_SIZEOF",
  "KEYWORD_CLASS", "KEYWORD_PUBLIC", "KEYWORD_PRIVATE",
  "KEYWORD_PROTECTED", "KEYWORD_NEW", "KEYWORD_DELETE", "KEYWORD_USING",
  "KEYWORD_NAMESPACE", "KEYWORD_BOOL", "KEYWORD_LONG", "KEYWORD_TRUE",
  "KEYWORD_FALSE", "IDENTIFIER", "INTEGER_LITERAL", "FLOAT_LITERAL",
  "CHAR_LITERAL", "STRING_LITERAL", "OP_PLUS", "OP_MINUS", "OP_MULTIPLY",
  "OP_DIVIDE", "OP_MODULO", "OP_INCREMENT", "OP_DECREMENT", "OP_ASSIGN",
  "OP_ADD_ASSIGN", "OP_SUB_ASSIGN", "OP_MUL_ASSIGN", "OP_DIV_ASSIGN",
  "OP_MOD_ASSIGN", "OP_LSHIFT", "OP_RSHIFT", "OP_EQUAL", "OP_NOT_EQUAL",
  "OP_LESS_THAN", "OP_GREATER_THAN", "OP_LESS_THAN_EQUAL",
  "OP_GREATER_THAN_EQUAL", "OP_LOGICAL_AND", "OP_LOGICAL_OR",
  "OP_LOGICAL_NOT", "OP_BITWISE_AND", "OP_BITWISE_OR", "OP_BITWISE_XOR",
  "OP_BITWISE_NOT", "OP_AND", "OP_OR", "OP_XOR", "OP_NOT", "SEMICOLON",
  "COMMA", "OPEN_PARENTHESIS", "CLOSE_PARENTHESIS", "OPEN_BRACKET",
  "CLOSE_BRACKET", "OPEN_CURLY_BRACKET", "CLOSE_CURLY_BRACKET", "COLON",
  "POINTER_ARROW", "DOT", "REFERENCE_AMPERSAND", "POINTER_STAR",
  "QUESTION_MARK", "PREPROCESSOR_DIRECTIVE", "SINGLE_LINE_COMMENT",
  "MULTI_LINE_COMMENT", "NEWLINE", "WHITESPACE", "ERROR_TOKEN", "UNKNOWN",
  "END_OF_FILE", "$accept", "translation_unit", "declaration_list",
  "declaration", "typedef_declaration", "class_declaration",
  "enum_declaration", "typedef_name", "class_name", "enum_name",
  "template_id", "template_argument_list", "template_argument", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-72)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int8 yypact[] =
{
     -19,   -33,   -32,   -30,    11,   -19,   -72,   -72,   -72,   -72,
     -72,   -71,   -49,   -61,   -72,   -49,   -69,   -72,   -72,   -72,
     -67,   -38,   -72,   -66,   -72,   -49,   -72,   -72,   -63,   -72,
     -72,   -72,   -38,   -72
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       0,     0,     0,     0,     0,     2,     4,     5,     6,     7,
      15,     0,    11,     0,    12,    13,     0,    14,     1,     3,
       0,     0,     8,     0,    10,    19,    20,    21,     0,    17,
       9,    16,     0,    18
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -72,   -72,   -72,    14,   -72,   -72,   -72,   -72,   -72,   -72,
       3,   -72,   -12
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
       0,     4,     5,     6,     7,     8,     9,    13,    16,    11,
      27,    28,    29
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int8 yytable[] =
{
      31,     1,    25,    26,     2,    14,    17,    10,    12,     3,
      15,    18,    20,    21,    23,    32,    22,    24,    30,    19,
      33
};

static const yytype_int8 yycheck[] =
{
      63,    20,    40,    41,    23,     2,     3,    40,    40,    28,
      40,     0,    83,    62,    83,    78,    77,    84,    84,     5,
      32
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,    20,    23,    28,   100,   101,   102,   103,   104,   105,
      40,   108,    40,   106,   109,    40,   107,   109,     0,   102,
      83,    62,    77,    83,    84,    40,    41,   109,   110,   111,
      84,    63,    78,   111
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    99,   100,   101,   101,   102,   102,   102,   103,   104,
     105,   106,   106,   107,   107,   108,   109,   110,   110,   111,
     111,   111
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     2,     1,     1,     1,     1,     3,     4,
       4,     1,     1,     1,     1,     1,     4,     1,     3,     1,
       1,     1
};


enum { YYENOMEM = -2 };

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYNOMEM         goto yyexhaustedlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == YYEMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (Value);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        goto yybackup;                                            \
      }                                                           \
    else                                                          \
      {                                                           \
        yyerror (YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Backward compatibility with an undocumented macro.
   Use YYerror or YYUNDEF. */
#define YYERRCODE YYUNDEF


/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)




# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Kind, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  FILE *yyoutput = yyo;
  YY_USE (yyoutput);
  if (!yyvaluep)
    return;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo,
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  yy_symbol_value_print (yyo, yykind, yyvaluep);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp,
                 int yyrule)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       YY_ACCESSING_SYMBOL (+yyssp[yyi + 1 - yynrhs]),
                       &yyvsp[(yyi + 1) - (yynrhs)]);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args) ((void) 0)
# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
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






/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg,
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep)
{
  YY_USE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/* Lookahead token kind.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;




/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    yy_state_fast_t yystate = 0;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus = 0;

    /* Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* Their size.  */
    YYPTRDIFF_T yystacksize = YYINITDEPTH;

    /* The state stack: array, bottom, top.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss = yyssa;
    yy_state_t *yyssp = yyss;

    /* The semantic value stack: array, bottom, top.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs = yyvsa;
    YYSTYPE *yyvsp = yyvs;

  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yychar = YYEMPTY; /* Cause a token to be read.  */

  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END
  YY_STACK_PRINT (yyss, yyssp);

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    YYNOMEM;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        YYNOMEM;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          YYNOMEM;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */


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

  /* YYCHAR is either empty, or end-of-input, or a valid lookahead.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token\n"));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = YYEOF;
      yytoken = YYSYMBOL_YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else if (yychar == YYerror)
    {
      /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
      yychar = YYUNDEF;
      yytoken = YYSYMBOL_YYerror;
      goto yyerrlab1;
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
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
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
| yyreduce -- do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {

#line 1457 "y.tab.c"

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
  YY_SYMBOL_PRINT ("-> $$ =", YY_CAST (yysymbol_kind_t, yyr1[yyn]), &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYSYMBOL_YYEMPTY : YYTRANSLATE (yychar);
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
      yyerror (YY_("syntax error"));
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
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;
  ++yynerrs;

  /* Do not reclaim the symbols of the rule whose action triggered
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
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  /* Pop stack until we find a state that shifts the error token.  */
  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYSYMBOL_YYerror;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYSYMBOL_YYerror)
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
                  YY_ACCESSING_SYMBOL (yystate), yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", YY_ACCESSING_SYMBOL (yyn), yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturnlab;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturnlab;


/*-----------------------------------------------------------.
| yyexhaustedlab -- YYNOMEM (memory exhaustion) comes here.  |
`-----------------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  goto yyreturnlab;


/*----------------------------------------------------------.
| yyreturnlab -- parsing is finished, clean up and return.  |
`----------------------------------------------------------*/
yyreturnlab:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif

  return yyresult;
}

#line 98 "parser.y"


/* Error handler */
void yyerror(const char *s) {
    fprintf(stderr, "Parse Error at line %d: %s\n", yylineno, s);
}

/* simple driver */
int main(int argc, char **argv) {
    if (argc > 1) {
        yyin = fopen(argv[1], "r");
        if (!yyin) {
            perror("fopen");
            return 1;
        }
    } else {
        yyin = stdin;
    }
    yyparse();
    if (yyin && yyin != stdin) fclose(yyin);
    return 0;
}
