
/* A Bison parser, made by GNU Bison 2.4.1.  */

/* Skeleton interface for Bison's Yacc-like parsers in C
   
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


/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     NAME = 258,
     STRING = 259,
     NUMBER = 260,
     COMPARISION = 261,
     AND = 262,
     SELECT = 263,
     COUNT = 264,
     FROM = 265,
     WHERE = 266,
     ORDER = 267,
     BY = 268,
     ASC = 269,
     DESC = 270,
     ALL = 271,
     UNIQUE = 272,
     DISTINCT = 273,
     CREATE = 274,
     TABLE = 275,
     DROP = 276,
     INDEX = 277,
     PRIMARY = 278,
     KEY = 279,
     INSERT = 280,
     INTO = 281,
     VALUES = 282,
     DELETE = 283,
     CHARACTER = 284,
     INTEGER = 285,
     DATE = 286,
     SHOW = 287,
     TABLES = 288,
     EXIT = 289
   };
#endif
/* Tokens.  */
#define NAME 258
#define STRING 259
#define NUMBER 260
#define COMPARISION 261
#define AND 262
#define SELECT 263
#define COUNT 264
#define FROM 265
#define WHERE 266
#define ORDER 267
#define BY 268
#define ASC 269
#define DESC 270
#define ALL 271
#define UNIQUE 272
#define DISTINCT 273
#define CREATE 274
#define TABLE 275
#define DROP 276
#define INDEX 277
#define PRIMARY 278
#define KEY 279
#define INSERT 280
#define INTO 281
#define VALUES 282
#define DELETE 283
#define CHARACTER 284
#define INTEGER 285
#define DATE 286
#define SHOW 287
#define TABLES 288
#define EXIT 289




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{

/* Line 1676 of yacc.c  */
#line 56 "parse.y"

	int intval;
	char* strval;



/* Line 1676 of yacc.c  */
#line 127 "y.tab.h"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE yylval;


