
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
#line 1 "parse.y"


#include <stdio.h>
#include <string.h>
#include <memory.h>

extern FILE *yyin;
FILE *yyout;
extern int yylex();
extern int yyerror(const char *s);
extern int lex_init();


int linenum = 1;
int length=0;//该变量用于表示字段长度
char* tbname;
char* cbname;
char* Tstr;
char* name;
char* primary_name;
char* writestr[1000];
char* tablenames[255];
char* columnnames[255];
char* columnxlist[255];
char* columnzlist[255];
char* names[255];
char* selects[255];
const char* column_type;// convert string const to char*
char types[255];
const char* p=",";// convert string const to char*
char* s1;
char* s2;
char* s3;
char* selectclause;
char* ordertab;

static unsigned int attrcnt;

static int funcRnt;

static int curPos;
static char recordstr[4096];

int condcnt;
int op;

static int is_ares;
static int is_uni;
static int is_order;

static char errmsg[4096];
//static Table* _proctable;

//static FILE* LOG;


/* Line 189 of yacc.c  */
#line 130 "y.tab.c"

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

/* Line 214 of yacc.c  */
#line 57 "parse.y"

	int intval;
	char* strval;



/* Line 214 of yacc.c  */
#line 241 "y.tab.c"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif


/* Copy the second part of user declarations.  */


/* Line 264 of yacc.c  */
#line 253 "y.tab.c"

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
#define YYFINAL  25
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   84

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  44
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  28
/* YYNRULES -- Number of rules.  */
#define YYNRULES  56
/* YYNRULES -- Number of states.  */
#define YYNSTATES  106

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   289

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
      40,    41,    10,     8,    42,     9,    43,    11,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,    39,
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
       5,     6,     7,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    32,    33,    34,    35,    36,    37,    38
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint8 yyprhs[] =
{
       0,     0,     3,     5,     7,     9,    11,    13,    15,    17,
      19,    27,    31,    33,    35,    39,    43,    44,    47,    49,
      54,    56,    58,    63,    75,    79,    81,    83,    85,    89,
      93,    99,   106,   107,   110,   112,   116,   121,   123,   125,
     127,   128,   132,   134,   135,   138,   140,   144,   148,   152,
     156,   160,   162,   166,   168,   170,   174
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] =
{
      45,     0,    -1,    46,    -1,    38,    -1,    39,    -1,    47,
      -1,    54,    -1,    55,    -1,    58,    -1,    59,    -1,    23,
      24,    48,    40,    49,    41,    39,    -1,    48,    42,     3,
      -1,     3,    -1,    50,    -1,    49,    42,    50,    -1,    52,
      53,    51,    -1,    -1,    27,    28,    -1,     3,    -1,    33,
      40,     5,    41,    -1,    34,    -1,    35,    -1,    25,    24,
      48,    39,    -1,    29,    30,    48,    40,    56,    41,    31,
      40,    57,    41,    39,    -1,    56,    42,     3,    -1,     3,
      -1,     5,    -1,     4,    -1,    57,    42,     5,    -1,    57,
      42,     4,    -1,    32,    14,    48,    65,    39,    -1,    60,
      14,    48,    65,    63,    39,    -1,    -1,    12,    61,    -1,
       3,    -1,    61,    42,     3,    -1,    13,    40,    62,    41,
      -1,    20,    -1,    10,    -1,    10,    -1,    -1,    16,    17,
      64,    -1,     3,    -1,    -1,    15,    66,    -1,    67,    -1,
      66,     7,    67,    -1,    68,     6,    69,    -1,    68,     6,
       5,    -1,    68,     6,     4,    -1,    68,     6,    70,    -1,
       3,    -1,    71,    43,    68,    -1,     3,    -1,     3,    -1,
      71,    43,    70,    -1,     3,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,    87,    87,    98,   104,   108,   109,   110,   111,   112,
     116,   137,   143,   152,   153,   157,   165,   167,   173,   183,
     193,   199,   209,   225,   241,   247,   256,   263,   270,   277,
     289,   305,   326,   328,   334,   341,   348,   351,   352,   361,
     371,   373,   378,   387,   390,   396,   404,   415,   419,   424,
     429,   437,   441,   450,   457,   461,   469
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "NAME", "STRING", "NUMBER",
  "COMPARISION", "AND", "'+'", "'-'", "'*'", "'/'", "SELECT", "COUNT",
  "FROM", "WHERE", "ORDER", "BY", "ASC", "DESC", "ALL", "UNIQUE",
  "DISTINCT", "CREATE", "TABLE", "DROP", "INDEX", "PRIMARY", "KEY",
  "INSERT", "INTO", "VALUES", "DELETE", "CHARACTER", "INTEGER", "DATE",
  "SHOW", "TABLES", "EXIT", "';'", "'('", "')'", "','", "'.'", "$accept",
  "sql_start", "sql_func", "table_def", "table", "table_attr_list",
  "column_def", "yusu", "column", "data_type", "table_drop", "insert_stat",
  "column_list", "insert_list", "delete_stat", "select_stat",
  "select_clause", "uniqueaa", "star_list", "order_clause", "ordertabs",
  "where_clause", "condition", "expr", "columnx", "columny", "columnz",
  "columntable", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,    43,    45,
      42,    47,   263,   264,   265,   266,   267,   268,   269,   270,
     271,   272,   273,   274,   275,   276,   277,   278,   279,   280,
     281,   282,   283,   284,   285,   286,   287,   288,   289,    59,
      40,    41,    44,    46
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    44,    45,    45,    46,    46,    46,    46,    46,    46,
      47,    48,    48,    49,    49,    50,    51,    51,    52,    53,
      53,    53,    54,    55,    56,    56,    57,    57,    57,    57,
      58,    59,    60,    60,    61,    61,    61,    61,    61,    62,
      63,    63,    64,    65,    65,    66,    66,    67,    67,    67,
      67,    68,    68,    69,    70,    70,    71
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     1,     1,     1,     1,     1,     1,     1,
       7,     3,     1,     1,     3,     3,     0,     2,     1,     4,
       1,     1,     4,    11,     3,     1,     1,     1,     3,     3,
       5,     6,     0,     2,     1,     3,     4,     1,     1,     1,
       0,     3,     1,     0,     2,     1,     3,     3,     3,     3,
       3,     1,     3,     1,     1,     3,     1
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
      32,     0,     0,     0,     0,     0,     3,     4,     0,     2,
       5,     6,     7,     8,     9,     0,    34,    38,     0,    37,
      33,     0,     0,     0,     0,     1,     0,     0,     0,    12,
       0,     0,     0,    43,    43,    39,     0,    35,     0,     0,
      22,     0,     0,     0,    40,    36,    18,     0,    13,     0,
      11,    25,     0,    51,    44,    45,     0,     0,    30,     0,
       0,     0,     0,     0,    20,    21,    16,     0,     0,     0,
       0,     0,     0,    31,    10,    14,     0,     0,    15,     0,
      24,    46,    53,    49,    48,    47,    50,     0,    52,    42,
      41,     0,    17,     0,     0,    19,    27,    26,     0,    54,
      55,     0,     0,    23,    29,    28
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
      -1,     8,     9,    10,    30,    47,    48,    78,    49,    66,
      11,    12,    52,    98,    13,    14,    15,    20,    36,    60,
      90,    43,    54,    55,    56,    85,    86,    57
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -66
static const yytype_int8 yypact[] =
{
     -12,    -1,    -3,     1,     4,    26,   -66,   -66,    45,   -66,
     -66,   -66,   -66,   -66,   -66,    32,   -66,   -66,     7,   -66,
       6,    46,    46,    46,    46,   -66,    46,    40,    48,   -66,
     -26,   -24,    -7,   -14,   -14,   -66,    11,   -66,    50,    51,
     -66,    52,    53,    18,    42,   -66,   -66,   -38,   -66,   -11,
     -66,   -66,    -5,    16,    54,   -66,    56,    17,   -66,    47,
      24,    28,    50,    25,   -66,   -66,    41,    35,    66,    53,
      27,    53,    67,   -66,   -66,   -66,    68,    43,   -66,    36,
     -66,   -66,    16,   -66,   -66,   -66,   -66,    29,   -66,   -66,
     -66,    33,   -66,    34,    72,   -66,   -66,   -66,     0,    16,
     -66,    38,    39,   -66,   -66,   -66
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -66,   -66,   -66,   -66,   -16,   -66,    19,   -66,   -66,   -66,
     -66,   -66,   -66,   -66,   -66,   -66,   -66,   -66,   -66,   -66,
     -66,    44,   -66,    10,     9,   -66,   -10,   -65
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -57
static const yytype_int8 yytable[] =
{
       1,    42,    16,    61,    62,    87,    31,    32,    33,    17,
      34,     2,    18,     3,    38,    40,    39,     4,    39,    19,
       5,    21,    63,    64,    65,    22,     6,     7,    39,    87,
      82,    83,    84,    41,    23,    39,    67,    68,    96,    97,
      24,   101,   102,   104,   105,    25,    26,    27,    28,    29,
      35,    37,    45,    46,    50,    51,    53,    58,    59,   -56,
      71,    69,    70,    73,    72,    76,    79,    74,    77,    80,
      89,    92,    94,    91,    95,    99,    93,   103,    44,    81,
      88,    75,     0,     0,   100
};

static const yytype_int8 yycheck[] =
{
      12,    15,     3,    41,    42,    70,    22,    23,    24,    10,
      26,    23,    13,    25,    40,    39,    42,    29,    42,    20,
      32,    24,    33,    34,    35,    24,    38,    39,    42,    94,
       3,     4,     5,    40,    30,    42,    41,    42,     4,     5,
      14,    41,    42,     4,     5,     0,    14,    40,    42,     3,
      10,     3,    41,     3,     3,     3,     3,    39,    16,    43,
      43,     7,     6,    39,    17,    40,    31,    39,    27,     3,
       3,    28,    43,     5,    41,     3,    40,    39,    34,    69,
      71,    62,    -1,    -1,    94
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    12,    23,    25,    29,    32,    38,    39,    45,    46,
      47,    54,    55,    58,    59,    60,     3,    10,    13,    20,
      61,    24,    24,    30,    14,     0,    14,    40,    42,     3,
      48,    48,    48,    48,    48,    10,    62,     3,    40,    42,
      39,    40,    15,    65,    65,    41,     3,    49,    50,    52,
       3,     3,    56,     3,    66,    67,    68,    71,    39,    16,
      63,    41,    42,    33,    34,    35,    53,    41,    42,     7,
       6,    43,    17,    39,    39,    50,    40,    27,    51,    31,
       3,    67,     3,     4,     5,    69,    70,    71,    68,     3,
      64,     5,    28,    40,    43,    41,     4,     5,    57,     3,
      70,    41,    42,    39,     4,     5
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
#line 88 "parse.y"
    {
		if(funcRnt == 0)
			printf("> Success.\n");
		else if(funcRnt < 0)
		{
			printf("input error");
			//printf("> Failure:%d> %s\n",funcRnt,errmsg);
		}
		return 0;
	}
    break;

  case 3:

/* Line 1455 of yacc.c  */
#line 99 "parse.y"
    {
		printf("exit\n");
	}
    break;

  case 4:

/* Line 1455 of yacc.c  */
#line 105 "parse.y"
    {
		funcRnt = 100;
	}
    break;

  case 10:

/* Line 1455 of yacc.c  */
#line 117 "parse.y"
    {
		strcat(writestr, "6");
		strcat(writestr, "\n");
		Tstr = "create";
		strcat(writestr, Tstr);
		strcat(writestr, "\n");
		strcat(writestr, "table:");
		strcat(writestr,tbname);
		strcat(writestr, "\n");
		strcat(writestr, "column:");
		strcat(writestr, names);
		strcat(writestr, "\n");
		strcat(writestr, "data_type:");
		strcat(writestr, types);
		strcat(writestr, "\n");
		strcat(writestr, "primary_key:");
		strcat(writestr,primary_name);
		fputs(writestr, yyout);

        //printf("create table successfully\n");
		//printf("%d CREATE --> CREATE TABLE -->Table -->Table_attr_list\n",linenum++);
        //printf("%d Table :%s\n",linenum++,tbname);
       // printf("%d Table_attr_list --> column_def\n",linenum++);
        //printf("%d column_def --> column --> data_type\n",linenum++);
        //printf("%d column :%s\n",linenum++,names);
        //printf("%d data_type :%s\n",linenum++,types);
		//printf(" primary_key:%s\n",primary_name);
		
		
		//memset(primary_name,'\0',sizeof(primary_name));
		memset(names,'\0',sizeof(names));
		memset(types,'\0',sizeof(types));
		memset(tablenames,'\0',sizeof(tablenames));
		memset(writestr, '\0', sizeof(writestr));
		
	}
    break;

  case 11:

/* Line 1455 of yacc.c  */
#line 138 "parse.y"
    {
		tbname=(yyvsp[(3) - (3)].strval);
		strcat(tablenames,p);
		strcat(tablenames,tbname);
	}
    break;

  case 12:

/* Line 1455 of yacc.c  */
#line 144 "parse.y"
    {
		tbname = (yyvsp[(1) - (1)].strval);
		strcat(tablenames,p);
		strcat(tablenames,tbname);
	}
    break;

  case 15:

/* Line 1455 of yacc.c  */
#line 158 "parse.y"
    {
		attrcnt++;
		
	}
    break;

  case 16:

/* Line 1455 of yacc.c  */
#line 165 "parse.y"
    {
	}
    break;

  case 17:

/* Line 1455 of yacc.c  */
#line 168 "parse.y"
    {
		primary_name=name;
	}
    break;

  case 18:

/* Line 1455 of yacc.c  */
#line 174 "parse.y"
    {
		name=(yyvsp[(1) - (1)].strval);
		//strcat(names,p);
		strcat(names,name);	
		strcat(names,p);
	}
    break;

  case 19:

/* Line 1455 of yacc.c  */
#line 184 "parse.y"
    {
		//printf("NUMBER=%s\n",$3);
		length=(yyvsp[(3) - (4)].strval);
		column_type="CHARACTER";
		//strcat(types,p);
		strcat(types,column_type);
		strcat(types," ");
		strcat(types,length);		
	}
    break;

  case 20:

/* Line 1455 of yacc.c  */
#line 194 "parse.y"
    {
		column_type="INTEGER";
		strcat(types,p);
		strcat(types,column_type);
	}
    break;

  case 21:

/* Line 1455 of yacc.c  */
#line 200 "parse.y"
    {
		column_type="DATE";
		strcat(types,p);
		strcat(types,column_type);
	}
    break;

  case 22:

/* Line 1455 of yacc.c  */
#line 210 "parse.y"
    {	
		
		Tstr = "drop";
		strcat(writestr, "3");
		strcat(writestr, "\n");
		strcat(writestr, Tstr);
		strcat(writestr, "\n");
		strcat(writestr, "table:");
		strcat(writestr, tbname);
		fputs(writestr, yyout);
		//printf("drop table successfully.\n");
		//printf("%d DROP --> drop table --> table\n",linenum++);
		//printf("%d table --> %s\n",linenum++,tbname);
		
		//memset(names,'\0',sizeof(names));
		memset(tablenames,'\0',sizeof(tablenames));
		memset(writestr, '\0', sizeof(writestr));
	}
    break;

  case 23:

/* Line 1455 of yacc.c  */
#line 226 "parse.y"
    {
		strcat(writestr, "5");
		strcat(writestr, "\n");
		Tstr = "insert";
		strcat(writestr, Tstr);
		strcat(writestr, "\n");
		strcat(writestr, "table:");
		strcat(writestr, tbname);
		strcat(writestr, "\n");
		strcat(writestr, "column:");
		strcat(writestr, columnnames);
	    strcat(writestr, "\n");
		strcat(writestr, "listnode:");
		strcat(writestr, names);
		fputs(writestr, yyout);

        //printf("insert values into table successfully.\n");
       // printf("%d INSERT --> insert --> into --> table --> insert_list\n",linenum++);
       // printf("%d table -->:%s\n",linenum++,tbname);
		//printf(" table_colonm_list: %s\n",columnnames);
       // printf("%d insert_list --> :listnode(%d)\n",linenum++,attrcnt);//attrcnt表示插入节点个数
        //printf("%d listnode :%s\n",linenum++,names);
		
		attrcnt=0;
		memset(columnnames,'\0',sizeof(columnnames));
		memset(names,'\0',sizeof(names));
        memset(tablenames,'\0',sizeof(tablenames));
		memset(writestr, '\0', sizeof(writestr));
	}
    break;

  case 24:

/* Line 1455 of yacc.c  */
#line 242 "parse.y"
    {
		cbname=(yyvsp[(3) - (3)].strval);
		strcat(columnnames,p);
		strcat(columnnames,cbname);
	}
    break;

  case 25:

/* Line 1455 of yacc.c  */
#line 248 "parse.y"
    {
		cbname = (yyvsp[(1) - (1)].strval);
		strcat(columnnames,p);
		strcat(columnnames,cbname);
	}
    break;

  case 26:

/* Line 1455 of yacc.c  */
#line 257 "parse.y"
    {
		attrcnt++;
		name=(yyvsp[(1) - (1)].strval);
		strcat(names,p);
		strcat(names,name);
	}
    break;

  case 27:

/* Line 1455 of yacc.c  */
#line 264 "parse.y"
    {
		attrcnt++;
		name=(yyvsp[(1) - (1)].strval);
		strcat(names,p);
		strcat(names,name);
	}
    break;

  case 28:

/* Line 1455 of yacc.c  */
#line 271 "parse.y"
    {
		attrcnt++;
		name=(yyvsp[(3) - (3)].strval);
		strcat(names,p);
		strcat(names,name);
	}
    break;

  case 29:

/* Line 1455 of yacc.c  */
#line 278 "parse.y"
    {
		attrcnt++;
		name=(yyvsp[(3) - (3)].strval);
		strcat(names,p);
		strcat(names,name);
	}
    break;

  case 30:

/* Line 1455 of yacc.c  */
#line 290 "parse.y"
    {
			 strcat(writestr, "4");
			 strcat(writestr, "\n");
			 Tstr = "delete";
			 strcat(writestr, Tstr);
			 strcat(writestr, "\n");
			 strcat(writestr, "table:");
			 strcat(writestr, tbname);
			 strcat(writestr, "\n");
			 strcat(writestr, "condition:");
			 strcat(writestr, names);
			 fputs(writestr, yyout);

            //printf("delete successfully.\n");
            //printf("%d DELETE ->delete->from->table->where->where_clause \n",linenum++);
          //  printf("%d table->%s\n",linenum++,tbname);
           // printf("%d where_clause --> condition(%d)\n",linenum++,condcnt);
			//printf("%d condition %s\n",linenum++,names);
			
			condcnt=0;
			memset(names,'\0',sizeof(names));
			memset(tablenames,'\0',sizeof(tablenames));
			memset(writestr, '\0', sizeof(writestr));
		}
    break;

  case 31:

/* Line 1455 of yacc.c  */
#line 306 "parse.y"
    {
		strcat(writestr, "6");
		strcat(writestr, "\n");
		Tstr = "select";
		strcat(writestr, Tstr);
		strcat(writestr, "\n");
		strcat(writestr, "selects:");
		strcat(writestr, selects);
		strcat(writestr, "\n");
		strcat(writestr, "table:");
		strcat(writestr, tablenames);
		strcat(writestr, "\n");
		strcat(writestr, "where_clause:");
		strcat(writestr, names);
		strcat(writestr, "\n");
		strcat(writestr, "order_clause:");
		if (ordertab == NULL){
			strcat(writestr, "NULL");
		}
		else{
			strcat(writestr, ordertab);
		}
		fputs(writestr, yyout);

        //printf("select successfully.\n");        
        //printf("%d SELECT --> select_clause --> table --> where_clause --> order_clause: \n",linenum++);
        //printf("%d select_clause --> :listnode(%d)\n",linenum++,attrcnt);
        //printf("%d listnode :%s\n",linenum++,selects);
        //
        //printf("%d table :%s \n",linenum++,tablenames);//此处将tname改成了tbalenames，以便支持多表连接
        //printf("%d where_clause --> :condition(%d)\n",linenum++,condcnt);
        //printf("%d where_clause :%s \n",linenum++,names);      
        //printf("%d order_clause --> :%s \n",linenum++,ordertab);
		
		condcnt=0;attrcnt=0;
		memset(selects,'\0',sizeof(selects));
		memset(tablenames,'\0',sizeof(tablenames));
		memset(names,'\0',sizeof(names));
		memset(columnzlist,'\0',sizeof(columnzlist));
		memset(columnxlist,'\0',sizeof(columnxlist));
		memset(writestr, '\0', sizeof(writestr));
		
	}
    break;

  case 33:

/* Line 1455 of yacc.c  */
#line 329 "parse.y"
    {
		is_ares = 1;
	}
    break;

  case 34:

/* Line 1455 of yacc.c  */
#line 335 "parse.y"
    {
		attrcnt++;
		selectclause=(yyvsp[(1) - (1)].strval);
		strcat(selects,p);
		strcat(selects,selectclause);
	}
    break;

  case 35:

/* Line 1455 of yacc.c  */
#line 342 "parse.y"
    {
		attrcnt++;
		selectclause=(yyvsp[(3) - (3)].strval);
		strcat(selects,p);
		strcat(selects,selectclause);
	}
    break;

  case 36:

/* Line 1455 of yacc.c  */
#line 349 "parse.y"
    {
	}
    break;

  case 38:

/* Line 1455 of yacc.c  */
#line 353 "parse.y"
    {
		attrcnt++;
		selectclause="*";
		strcat(selects,p);
		strcat(selects,selectclause);
	}
    break;

  case 39:

/* Line 1455 of yacc.c  */
#line 362 "parse.y"
    {
		attrcnt++;
		selectclause="count(*)";
		strcat(selects,selectclause);
	}
    break;

  case 40:

/* Line 1455 of yacc.c  */
#line 371 "parse.y"
    {
	}
    break;

  case 41:

/* Line 1455 of yacc.c  */
#line 374 "parse.y"
    {
	}
    break;

  case 42:

/* Line 1455 of yacc.c  */
#line 379 "parse.y"
    {
		ordertab=(yyvsp[(1) - (1)].strval);
	}
    break;

  case 43:

/* Line 1455 of yacc.c  */
#line 387 "parse.y"
    {
		is_order = 1;
	}
    break;

  case 44:

/* Line 1455 of yacc.c  */
#line 391 "parse.y"
    {
		is_order = 1;
	}
    break;

  case 45:

/* Line 1455 of yacc.c  */
#line 397 "parse.y"
    {
		condcnt++;
		strcat(names,p);
		strcat(names,s1);
		strcat(names,s2);
		strcat(names,s3);
	}
    break;

  case 46:

/* Line 1455 of yacc.c  */
#line 405 "parse.y"
    {
		condcnt++;
		strcat(names,p);
		strcat(names,s1);
		strcat(names,s2);
		strcat(names,s3);
	}
    break;

  case 47:

/* Line 1455 of yacc.c  */
#line 416 "parse.y"
    {
		s2=(yyvsp[(2) - (3)].strval);
	}
    break;

  case 48:

/* Line 1455 of yacc.c  */
#line 420 "parse.y"
    {
		s2=(yyvsp[(2) - (3)].strval);
		s3=(yyvsp[(3) - (3)].strval);
	}
    break;

  case 49:

/* Line 1455 of yacc.c  */
#line 425 "parse.y"
    {
		s2=(yyvsp[(2) - (3)].strval);
		s3=(yyvsp[(3) - (3)].strval);
	}
    break;

  case 50:

/* Line 1455 of yacc.c  */
#line 430 "parse.y"
    {
		s1=columnxlist;
		s2=(yyvsp[(2) - (3)].strval);
		s3=columnzlist;
	}
    break;

  case 51:

/* Line 1455 of yacc.c  */
#line 438 "parse.y"
    {
		s1=(yyvsp[(1) - (1)].strval);
	}
    break;

  case 52:

/* Line 1455 of yacc.c  */
#line 442 "parse.y"
    {
		strcat(columnxlist,tbname);
		strcat(columnxlist,".");
		strcat(columnxlist,s1);
		
	}
    break;

  case 53:

/* Line 1455 of yacc.c  */
#line 451 "parse.y"
    {
		s3=(yyvsp[(1) - (1)].strval);	
	}
    break;

  case 54:

/* Line 1455 of yacc.c  */
#line 458 "parse.y"
    {
		s1=(yyvsp[(1) - (1)].strval);	
	}
    break;

  case 55:

/* Line 1455 of yacc.c  */
#line 462 "parse.y"
    {
		strcat(columnzlist,tbname);
		strcat(columnzlist,".");
		strcat(columnzlist,s1);
	}
    break;

  case 56:

/* Line 1455 of yacc.c  */
#line 470 "parse.y"
    {
		tbname=(yyvsp[(1) - (1)].strval);
	}
    break;



/* Line 1455 of yacc.c  */
#line 2066 "y.tab.c"
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
#line 474 "parse.y"


void parser_init()
{
	lex_init();
	attrcnt = 0;
	tbname = NULL;
	funcRnt = 0;
	curPos = 0;
	condcnt = 0;
	op =0;
	return;
}

int main()
{
	//yyin = stdin;
	yyin = fopen("C:\\Users\\mark\\Desktop\\in.txt", "r");
	yyout = fopen("C:\\Users\\mark\\Desktop\\out.txt", "w+");
	parser_init();
	yyparse();
	return 0;
}




