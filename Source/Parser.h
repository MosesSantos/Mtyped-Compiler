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
* File name: parser.h
* Compiler: MS Visual Studio 2022
* Author: Paulo Sousa
* Course: CST 8152 – Compilers, Lab Section: [011, 012, 013, 014]
* Assignment: A12, A22, A32.
* Date: Jan 01 2022
* Professor: Paulo Sousa
* Purpose: This file is the main header for Parser (.h)
*************************************************************/

#ifndef PARSER_H_
#define PARSER_H_

/* Inclusion section */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#ifndef COMPILERS_H_
#include "Compilers.h"
#endif
#ifndef BUFFER_H_
#include "Buffer.h"
#endif
#ifndef SCANNER_H_
#include "Scanner.h"
#endif

/* Global vars */
static Token lookahead;
mtyped_int syntaxErrorNumber = 0;
extern Buffer* stringLiteralTable;
extern mtyped_int line;
extern Token tokenizer();
extern mtyped_chr* keywordTable[];

#define STR_LANGNAME "mtyped"

/* Constants */
enum KEYWORDS {
	NO_ATTR = -1,
	WHITE,
	IF,
	ELSEIF,
	ELSE,
	DO,
	SWITCH,
	CASE,
	BREAK,
	RETURN,
	FOR,
	INT,
	FLOAT,
	STRING,
	TRUE,
	FALSE,
	BOOL,
	MAIN
};

/* Function definitions */
mtyped_void startParser();
mtyped_void matchToken(mtyped_int, mtyped_int);
mtyped_void syncErrorHandler(mtyped_int);
mtyped_void printError();

/* non-terminal function declarations */

mtyped_void program();
mtyped_void codeSession();

mtyped_void optVarListDeclarations();
mtyped_void varListsDeclaration();
mtyped_void varListDeclaration();

mtyped_void optionalStatements();
mtyped_void statement();

mtyped_void assignmentExpression();
mtyped_void assignmentStatement();

mtyped_void arithmeticExpression();
mtyped_void unaryArithmeticExpression();

mtyped_void additiveArithmeticExpression();
mtyped_void multiplicativeArithmeticExpression();
mtyped_void primaryArithmeticExpression();

mtyped_void primaryStringExpression();

mtyped_void outputStatement();

mtyped_void comments();
#endif
