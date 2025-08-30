/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison interface for Yacc-like parsers in C

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

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

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
