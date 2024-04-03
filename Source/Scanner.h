/*
************************************************************
* COMPILERS COURSE - Algonquin College
* Code version: Winter, 2022
* Author: Svillen Ranev - Paulo Sousa
* Professors: Paulo Sousa / George Kriger / Abdullah Kadri
************************************************************
*/

/*
************************************************************
* File name: scanner.h
* Compiler: MS Visual Studio 2022
* Author: Paulo Sousa
* Course: CST 8152 – Compilers, Lab Section: [011, 012, 013, 014]
* Assignment: A22, A32.
* Date: Feb 21 2022
* Purpose: This file is the main header for Scanner (.h)
* Function list: (...).
*************************************************************/

#ifndef COMPILERS_H_
#include "Compilers.h"
#endif

#ifndef SCANNER_H_
#define SCANNER_H_

#ifndef NULL
#include <_null.h> /* NULL pointer constant is defined there */
#endif

/*#pragma warning(1:4001) */	/*to enforce C89 type comments  - to make //comments an warning */
/*#pragma warning(error:4001)*/	/* to enforce C89 comments - to make // comments an error */

/* Constants */
#define VID_LEN 16   /* variable identifier length */
#define ERR_LEN 40  /* error message length */
#define NUM_LEN 5   /* maximum number of digits for IL */

#define RTE_CODE 1   /* Value for run-time error */

enum TOKENS {
	ERR_T,		/*  0: Error token */
	VID_T,		/*  1: Variable and method token*/
	STR_T,		/*  2: String literal token */
	IL_T,		/*  3: Interger literal token */
	FPL_T,		/*  4: Floating point token */
	LPR_T,		/*  5: Left parenthesis token */
	RPR_T,		/*  3: Right parenthesis token */
	LBR_T,		/*  7: Left brace token */
	RBR_T,		/*  8: Right brace token */
	KW_T,		/*  9: Keyword token */
	EOS_T,		/*  10: End of statement (semicolon) */
	RTE_T,		/* 11: Run-time error token */
	SEOF_T,		/* 12: Source end-of-file token */
	EQ_T,		/* 13: Equals token*/
	ISEQ_T,		/* 14: Is Equal token*/
	LT_T,		/* 15: Less than token*/
	GT_T,		/* 16: Greater than token*/
	NOT_T,		/* 17: Not token*/
	NE_T,		/* 18: Not equals token*/
	OR_T,		/* 19: Or token*/
	CMA_T,		/* 20: Comma token*/
	RSB_T,		/* 21: Right Square bracket*/
	LSB_T,		/* 22: Left Square bracket*/
	COM_T,		/* 23: Comment Token*/
	DIV_T,		/**/
	MULT_T,		/**/
	ADD_T,		/**/
	SUB_T,		/**/
	POW_T,		/**/
	AND_T,		/**/
	DIVE_T,		/**/
	MULTE_T,	/**/
	ADDE_T,		/**/
	SUBE_T,		/**/
	POWE_T,		/**/
	MNID_T
};

typedef enum ArithmeticOperators { OP_ADD, OP_SUB, OP_MUL, OP_DIV, OP_POW, OP_ADDE, OP_SUBE, OP_MULE, OP_DIVE, OP_POWE} AriOperator;
typedef enum RelationalOperators { OP_EQ, OP_NE, OP_GT, OP_LT } RelOperator;
typedef enum LogicalOperators { OP_AND, OP_OR, OP_NOT } LogOperator;
typedef enum SourceEndOfFile { SEOF_0, SEOF_255 } EofOperator;

typedef union TokenAttribute {
	mtyped_int codeType;      /* integer attributes accessor */
	AriOperator arithmeticOperator;		/* arithmetic operator attribute code */
	RelOperator relationalOperator;		/* relational operator attribute code */
	LogOperator logicalOperator;		/* logical operator attribute code */
	EofOperator seofType;				/* source-end-of-file attribute code */
	mtyped_int intValue;						/* integer literal attribute (value) */
	mtyped_int keywordIndex;					/* keyword index in the keyword table */
	mtyped_int contentString;				/* string literal offset from the beginning of the string literal buffer (stringLiteralTable->content) */
	mtyped_flt floatValue;					/* floating-point literal attribute (value) */
	mtyped_chr idLexeme[VID_LEN + 1];		/* variable identifier token attribute */
	mtyped_chr errLexeme[ERR_LEN + 1];		/* error token attribite */
	mtyped_string commentString;
} TokenAttribute;

typedef struct idAttibutes {
	mtyped_flg flags;			/* Flags information */
	union {
		mtyped_int intValue;			/* Integer value */
		mtyped_flt floatValue;			/* Float value */
		mtyped_void* stringContent;		/* String value */
	} values;
} IdAttibutes;

/* Token declaration */
typedef struct Token {
	mtyped_int code;				/* token code */
	TokenAttribute attribute;	/* token attribute */
	IdAttibutes   idAttribute;	/* not used in this scanner implementation - for further use */
} Token;

///////////////////////////////////////////////////////////////////////////////////////////////////////

/* EOF definitions */
#define CHARSEOF0 '\0'
#define CHARSEOF255 0xFF

#define ES  12		/* Error state with no retract */
#define ER  13		/* Error state with retract */
#define IS -1		/* Illegal state */

#define TABLE_COLUMNS 8

#define CHRCOL2 '"'
#define CHRCOL3 '.'

static mtyped_int transitionTable[][TABLE_COLUMNS] = {
/*    [a-z], [A-Z], [LCUC], [0-9], ["], [.], [eof], other
 
	   LC(0),  UC(1), L(2), D(3), Q(4), P(5), E(6), O[7] */
	{    ES,    ES,    1,    7,     5,     8,  ES,   ES}, // S0: NOAS c 0, 0
	{    ES,    ES,    1,    1,   ES,   ES,    ES,    2}, // S1: NOAS
	{    IS,    IS,   IS,   IS,   IS,   IS,   IS,    IS}, // S2: ASWR (VID)
	{    IS,    IS,   IS,   IS,   IS,   IS,   IS,    IS}, // S3: ASWR (MVID)
	{    IS,    IS,   IS,   IS,   IS,   IS,   IS,    IS}, // S4: ASWR (KEY)
	{     5,     5,    5,    5,    6,    5,   ES,     5}, // S5: NOAS
	{    IS,    IS,   IS,   IS,   IS,   IS,   IS,    IS}, // S6: ASNR (SL)
	{    ES,    ES,    8,    7,    8,    9,    8,     8}, // S7: NOAS
	{    IS,    IS,   IS,   IS,   IS,   IS,   IS,    IS}, // S8: ASNR (IL)
	{    ES,    ES,   ES,   10,   ES,   ES,   ES,    ES}, // S9: NOAS
	{    ES,    ES,   11,   10,   11,   11,   11,    11}, // S10: NOAS
	{    IS,    IS,   IS,   IS,   IS,   IS,   IS,    IS}, // S11: ASWR(FPL)
	{    IS,    IS,   IS,   IS,   IS,   IS,   IS,    IS}, // S12: ESNR
	{    IS,    IS,   IS,   IS,   IS,   IS,   IS,    IS}  // S13: ERWR
};

/* Define accepting states types */
#define NOAS	0		/* not accepting state */
#define ASNR	1		/* accepting state with no retract */
#define ASWR	2		/* accepting state with retract */

static mtyped_int stateType[] = {
	NOAS, /* 00 */
	NOAS, /* 01 */
	ASWR, /* 02 (VID) */
	ASWR, /* 03 (MVID) */
	ASWR, /* 04 (KEY) */
	NOAS, /* 05 */
	ASNR, /* 06 (SL) */
	NOAS, /* 07 */
	ASWR, /* 08 (IL)*/
	NOAS, /* 09 */
	NOAS, /* 10 */
	ASWR, /*11 (FPL) */
	ASNR, /* 12 (Err1 - no retract) */
	ASWR  /* 13 (Err2 - retract) */
};

mtyped_int startScanner(BufferPointer psc_buf);
static mtyped_int nextClass(mtyped_chr c);					/* character class function */
static mtyped_int nextState(mtyped_int, mtyped_chr);		/* state machine function */

/*
-------------------------------------------------
Automata definitions
-------------------------------------------------
*/

typedef Token(*PTR_ACCFUN)(mtyped_chr* lexeme);

/* Declare accepting states functions */
Token funcID    (mtyped_chr lexeme[]);
Token funcKEY   (mtyped_chr lexeme[]);
Token funcSL    (mtyped_chr lexeme[]);
Token funcIL    (mtyped_chr lexeme[]);
Token funcFPL   (mtyped_chr lexeme[]);
Token funcErr	(mtyped_chr lexeme[]);

/* 
 * Accepting function (action) callback table (array) definition 
 * If you do not want to use the typedef, the equvalent declaration is:
 */

static PTR_ACCFUN finalStateTable[] = {
	NULL,		/* -    [00] */
	NULL,		/* -    [01] */
	funcID,	    /* VID	[02] */
	funcID,		/* MVID [03]*/
	funcKEY,	/* KEY  [04] */
	NULL,		/* -    [05] */
	funcSL,		/* SL   [06] */
	NULL,		/* -    [07] */
	funcIL,		/* IL   [08] */
	NULL,		/* -    [09] */
	NULL,		/* -    [10] */
	funcFPL,	/* FPL  [11] */
	funcErr,	/* ERR1 [12] */
	funcErr		/* ERR2 [13] */
};

/*
-------------------------------------------------
Language keywords
-------------------------------------------------
*/

#define KWT_SIZE 17

static mtyped_chr* keywordTable[KWT_SIZE] = {
	"while",
	"if",
	"elseif",
	"else",
	"do",
	"switch",
	"case",
	"break",
	"return",
	"for",
	"int",
	"float",
	"string",
	"true",
	"false",
	"bool",
	"main",
};

#endif
