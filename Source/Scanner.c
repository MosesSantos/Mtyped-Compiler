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
* Professor: Paulo Sousa
* Purpose: This file contains all functionalities from Scanner.
* Function list: (...).
************************************************************
*/

 /* The #define _CRT_SECURE_NO_WARNINGS should be used in MS Visual Studio projects
  * to suppress the warnings about using "unsafe" functions like fopen()
  * and standard sting library functions defined in string.h.
  * The define does not have any effect in Borland compiler projects.
  */
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>   /* standard input / output */
#include <ctype.h>   /* conversion functions */
#include <stdlib.h>  /* standard library functions and constants */
#include <string.h>  /* string functions */
#include <limits.h>  /* integer types constants */
#include <float.h>   /* floating-point types constants */

/* #define NDEBUG        to suppress assert() call */
#include <assert.h>  /* assert() prototype */

/* project header files */

#ifndef COMPILERS_H_
#include "Compilers.h"
#endif

#ifndef BUFFER_H_
#include "Buffer.h"
#endif

#ifndef SCANNER_H_
#include "Scanner.h"
#endif


/* Global objects - variables */
/* This buffer is used as a repository for string literals. */
extern BufferPointer stringLiteralTable;	/* String literal table */
mtyped_int line;							/* Current line number of the source code */
extern mtyped_int errorNumber;				/* Defined in platy_st.c - run-time error number */

extern mtyped_int stateType[];
extern mtyped_chr* keywordTable[];
extern PTR_ACCFUN finalStateTable[];
extern mtyped_int transitionTable[][TABLE_COLUMNS];

/* Local(file) global objects - variables */
static BufferPointer lexemeBuffer;			/* Pointer to temporary lexeme buffer */
static BufferPointer sourceBuffer;			/* Pointer to input source buffer */

/*
 ************************************************************
 * Intitializes scanner
 *		This function initializes the scanner using defensive programming.
 ***********************************************************
 */

mtyped_int startScanner(BufferPointer psc_buf) {
	/* in case the buffer has been read previously  */
	bRecover(psc_buf);
	bClear(stringLiteralTable);
	line = 1;
	sourceBuffer = psc_buf;
	return EXIT_SUCCESS; /*0*/
}

/*
 ************************************************************
 * Process Token
 *		Main function of buffer, responsible to classify a char (or sequence
 *		of chars). In the first part, a specific sequence is detected (reading
 *		from buffer). In the second part, a pattern (defined by Regular Expression)
 *		is recognized and the appropriate function is called (related to final states 
 *		in the Transition Diagram).
 ***********************************************************
 */

Token tokenizer(void) {

	Token currentToken = { 0 }; /* token to return after pattern recognition. Set all structure members to 0 */
	mtyped_chr c;	            /* input symbol */
	mtyped_int state = 0;		/* initial state of the FSM */
	mtyped_int lexStart;		/* start offset of a lexeme in the input char buffer (array) */
	mtyped_int lexEnd;		    /* end offset of a lexeme in the input char buffer (array)*/

	mtyped_int lexLength;		/* token length */
	mtyped_int i;				/* counter */
	mtyped_chr newc;			/* new char */
	
	while (1) { /* endless loop broken by token returns it will generate a warning */
		c = bGetChar(sourceBuffer);
		/* ------------------------------------------------------------------------
			Part 1: Implementation of token driven scanner.
			Every token is possessed by its own dedicated code
			-----------------------------------------------------------------------
		*/

		switch (c) {

		/* Cases for spaces */
		case ' ':
		case '\t':
		case '\f':
			break;
		case '\n':
			line++;
			break;

		/* Cases for symbols */
		case '=':
			currentToken.attribute.idLexeme[0] = c;
			newc = bGetChar(sourceBuffer) == '=';
			if (newc ==  '=') {
				currentToken.code = ISEQ_T;
				currentToken.attribute.arithmeticOperator = OP_EQ;
				currentToken.attribute.idLexeme[1] = newc;
				bGetChar(sourceBuffer);
			} else {
				currentToken.code = EQ_T;	
			}
			bRetract(sourceBuffer);
			return currentToken;
			break;
		case '<':
			currentToken.code = LT_T;
			currentToken.attribute.arithmeticOperator = OP_LT;
			return currentToken;
			break;
		case '>':
			currentToken.code = GT_T;
			currentToken.attribute.arithmeticOperator = OP_GT;
			return currentToken;
			break;
		case '!':
			if (bGetChar(sourceBuffer) == '=') {
				currentToken.code = NE_T;
				currentToken.attribute.relationalOperator = OP_NE;
				bGetChar(sourceBuffer);
			} else {
				currentToken.code = NOT_T;
				currentToken.attribute.logicalOperator = OP_NOT;
			}
			bRetract(sourceBuffer);
			return currentToken;
			break;
		case '|':
			if (bGetChar(sourceBuffer) == '|') {
				currentToken.code = OR_T;
				currentToken.attribute.logicalOperator = OP_OR;
			}
			return currentToken;
			break;
		case '&':
			if (bGetChar(sourceBuffer) == '&') {
				currentToken.code = AND_T;
				currentToken.attribute.logicalOperator = OP_AND;
			}
			return currentToken;
			break;
		case ',':
			currentToken.code = CMA_T;
			return currentToken;
			break;
		case '[':
			currentToken.code = LSB_T;
			return currentToken;
			break;
		case ']':
			currentToken.code = RSB_T;
			return currentToken;
			break;
		case ';':
			currentToken.code = EOS_T;
			currentToken.attribute.idLexeme[0] = c;
			currentToken.attribute.idLexeme[VID_LEN] = CHARSEOF0;
			return currentToken;
		case '(':
			currentToken.code = LPR_T;
			return currentToken;
		case ')':
			currentToken.code = RPR_T;
			return currentToken;
		case '{':
			currentToken.code = LBR_T;
			return currentToken;
		case '}':
			currentToken.code = RBR_T;
			return currentToken;
		case '/':
			newc = bGetChar(sourceBuffer);
			if (newc == '/') {

				int count = 0;
				currentToken.code = COM_T;
				bSetMark(sourceBuffer, bGetReadPos(sourceBuffer));

				while (newc != '\n' && newc != CHARSEOF0) {
					newc = bGetChar(sourceBuffer);
					count++;
				}
				currentToken.attribute.commentString = (mtyped_string)calloc((size_t)count + 3, sizeof(char));		
				bRestore(sourceBuffer);

				if (!currentToken.attribute.commentString) {
					return currentToken;
				}

				for (int i = 0; i < count - 1; i++) {
					currentToken.attribute.commentString[i] = bGetChar(sourceBuffer);
				}
			} else if(newc == '='){
				currentToken.code = DIVE_T;
				currentToken.attribute.arithmeticOperator = OP_DIVE;	
			} else {
				currentToken.code = DIV_T;
				currentToken.attribute.arithmeticOperator = OP_DIV;
				bRetract(sourceBuffer);
			}
			return currentToken;
			break;	
		case '^' :

			if (bGetChar(sourceBuffer) == '=') {
				currentToken.code = POWE_T;
				currentToken.attribute.arithmeticOperator = OP_POWE;
			} else {
				currentToken.code = POW_T;
				currentToken.attribute.arithmeticOperator = OP_POW;
			}
			return currentToken;
			break;
		case '*':
			if (bGetChar(sourceBuffer) == '=') {
				currentToken.code = MULTE_T;
				currentToken.attribute.arithmeticOperator = OP_MULE;
			} else {
				currentToken.code = MULT_T;
				currentToken.attribute.arithmeticOperator = OP_MUL;
			}
			return currentToken;
			break;
		case '+':
			if (bGetChar(sourceBuffer) == '=') {
				currentToken.code = ADDE_T;
				currentToken.attribute.arithmeticOperator = OP_ADDE;
			} else {
				currentToken.code = ADD_T;
				currentToken.attribute.arithmeticOperator = OP_ADD;
			}
			return currentToken;
			break;
		case '-':
			if (bGetChar(sourceBuffer) == '=') {	
				currentToken.code = SUBE_T;
				currentToken.attribute.arithmeticOperator = OP_SUBE;
			} else {
				currentToken.code = SUB_T;
				currentToken.attribute.arithmeticOperator = OP_SUB;
			}
			bRetract(sourceBuffer);
			return currentToken;
			break;
		/* Cases for END OF FILE */
		case CHARSEOF0:
			currentToken.code = SEOF_T;
			currentToken.attribute.seofType = SEOF_0;
			return currentToken;
		case CHARSEOF255:
			currentToken.code = SEOF_T;
			currentToken.attribute.seofType = SEOF_255;
			return currentToken;

		/* ------------------------------------------------------------------------
			Part 2: Implementation of Finite State Machine (DFA) or Transition Table driven Scanner
			Note: Part 2 must follow Part 1 to catch the illegal symbols
			-----------------------------------------------------------------------
		*/

		default: // general case
			state = nextState(state, c);
			lexStart = bGetReadPos(sourceBuffer) - 1;
			bSetMark(sourceBuffer, lexStart);
			int pos = 0;
			while (stateType[state] == NOAS) {
				c = bGetChar(sourceBuffer);
				state = nextState(state, c);
				pos++;
			}
			if (stateType[state] == ASWR) {
				bRetract(sourceBuffer);
			}
				
			lexEnd = bGetReadPos(sourceBuffer);
			lexLength = lexEnd - lexStart;
			lexemeBuffer = bCreate((short)lexLength + 3, 0, MODE_FIXED);
			if (!lexemeBuffer) {
				fprintf(stderr, "Scanner error: Can not create buffer\n");
				exit(1);
			}
			bRestore(sourceBuffer);
			for (i = 0; i < lexLength; i++) {
				bAddChar(lexemeBuffer, bGetChar(sourceBuffer));
			}
			bAddChar(lexemeBuffer, BUFFER_EOF);
			currentToken = (*finalStateTable[state])(bGetContent(lexemeBuffer, 0));
			bDestroy(lexemeBuffer);
			return currentToken;
		} // switch
	} //while
} // tokenizer

/*
 ************************************************************
 * Get Next State
	The assert(int test) macro can be used to add run-time diagnostic to programs
	and to "defend" from producing unexpected results.
	- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	(*) assert() is a macro that expands to an if statement;
	if test evaluates to false (zero) , assert aborts the program
	(by calling abort()) and sends the following message on stderr:
	(*) Assertion failed: test, file filename, line linenum.
	The filename and linenum listed in the message are the source file name
	and line number where the assert macro appears.
	- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	If you place the #define NDEBUG directive ("no debugging")
	in the source code before the #include <assert.h> directive,
	the effect is to comment out the assert statement.
	- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	The other way to include diagnostics in a program is to use
	conditional preprocessing as shown bellow. It allows the programmer
	to send more details describing the run-time problem.
	Once the program is tested thoroughly #define DEBUG is commented out
	or #undef DEBUG is used - see the top of the file.
 ***********************************************************
 */

mtyped_int nextState(mtyped_int state, mtyped_chr c) {

	mtyped_int col;
	mtyped_int next;

	col = nextClass(c); /* 2 */
	next = transitionTable[state][col];
	
	if (DEBUG)
		printf("Input symbol: %c Row: %d Column: %d Next: %d \n", c, state, col, next);
	assert(next != IS);
	if (DEBUG)
		if (next == IS) {
			printf("Scanner Error: Illegal state:\n");
			printf("Input symbol: %c Row: %d Column: %d\n", c, state, col);
			exit(1);
		}
	return next;
}

/*
 ************************************************************
 * Get Next Token Class
	* Create a function to return the column number in the transition table:
	* Considering an input char c, you can identify the "class".
	* For instance, a letter should return the column for letters, etc.
 ***********************************************************
 */

mtyped_int nextClass(mtyped_chr c) {

	mtyped_int val = -1;

	switch (c) {
		case CHRCOL2:
			val = 4;
			break;
		case CHRCOL3:
			val = 5;
			break;

		case CHARSEOF0:
		case CHARSEOF255:
			val = 6;
			break;

		default:
			if (isalpha(c)) {		
				val = 2;	
			}
			else if (isdigit(c))
				val = 3;
			else
				val = 7;
	}
	return val;
}


/*
 ************************************************************
 * Acceptance State Function ID
 *		In this function, the pattern for IDs must be recognized.
 *		Since keywords obey the same pattern, is required to test if
 *		the current lexeme matches with KW from language.
 *	- Remember to respect the limit defined for lexemes (VID_LEN) and
 *	  set the lexeme to the corresponding attribute (vidLexeme).
 *    Remember to end each token with the \0.
 *  - Suggestion: Use "strncpy" function.
 ***********************************************************
 */

Token funcID(mtyped_chr lexeme[]) {

	Token currentToken = { 0 };
	size_t length = strlen(lexeme);

	Token temp = funcKEY(lexeme);

	if (temp.code == KW_T || temp.code == MNID_T) {
		strncpy(temp.attribute.idLexeme, lexeme, length);
		temp.attribute.idLexeme[VID_LEN] = CHARSEOF0;
		return temp;
	}
	/*if (lexeme[0] < 97) {	
		return currentToken;		
	}*/
	currentToken.code = VID_T;
	strncpy(currentToken.attribute.idLexeme, lexeme, length);
	currentToken.attribute.idLexeme[VID_LEN] = CHARSEOF0;
	
	return currentToken;
}


/*
************************************************************
 * Acceptance State Function SL
 *		Function responsible to identify SL (string literals).
 * - The lexeme must be stored in the String Literal Table 
 *   (stringLiteralTable). You need to include the literals in 
 *   this structure, using offsets. Remember to include \0 to
 *   separate the lexemes. Remember also to incremente the line.
 ***********************************************************
 */

Token funcSL(mtyped_chr lexeme[]) {

	Token currentToken = { 0 };
	mtyped_int i = 0, len = (mtyped_int)strlen(lexeme);

	currentToken.attribute.contentString = bGetWritePos(stringLiteralTable);
	for (i = 1; i < len - 1; i++) {
		if (lexeme[i] == '\n')
			line++;
		if (!bAddChar(stringLiteralTable, lexeme[i])) {
			currentToken.code = RTE_T;
			strcpy(currentToken.attribute.errLexeme, "Run Time Error:");
			errorNumber = RTE_CODE;
			return currentToken;
		}
	}
    if (!bAddChar(stringLiteralTable, CHARSEOF0)) {

		currentToken.code = RTE_T;
		strcpy(currentToken.attribute.errLexeme, "Run Time Error:");
		errorNumber = RTE_CODE;
		return currentToken;
	}
	currentToken.code = STR_T;
	return currentToken;
}

Token funcIL(mtyped_chr lexeme[]) {
	
	Token currentToken = { 0 };

	currentToken.code = IL_T;
	currentToken.attribute.intValue = atoi(lexeme);
	return currentToken;
}

Token funcFPL(mtyped_chr lexeme[]) {

	Token currentToken = { 0 };

	currentToken.code = FPL_T;
	currentToken.attribute.floatValue = (float)atof(lexeme);
	return currentToken;
}


/*
************************************************************
 * This function checks if one specific lexeme is a keyword.
 * - Tip: Remember to use the keywordTable to check the keywords.
 ***********************************************************
 */

Token funcKEY(mtyped_chr lexeme[]) {

	Token currentToken = { 0 };

	for (mtyped_int j = 0; j < KWT_SIZE; j++) {

		if (strcmp(lexeme, &keywordTable[j][0]) == 0) {

			if (strncmp(lexeme, "main", 4) == 0) {
				currentToken.code = MNID_T;
			} else {
				currentToken.code = KW_T;
			}
			currentToken.attribute.codeType = j;
			return currentToken;
		}
	}
	return currentToken;
}


/*
************************************************************
 * Acceptance State Function Error
 *		Function responsible to deal with ERR token.
 * - This function uses the errLexeme, respecting the limit given
 *   by ERR_LEN. If necessary, use three dots (...) to use the
 *   limit defined. The error lexeme contains line terminators,
 *   so remember to increment line.
 ***********************************************************
 */

Token funcErr(mtyped_chr lexeme[]) {

	Token currentToken = { 0 };
	mtyped_int i = 0, len = (mtyped_int)strlen(lexeme);

	if (len > ERR_LEN) {
		strncpy(currentToken.attribute.errLexeme, lexeme, ERR_LEN - 3);
		currentToken.attribute.errLexeme[ERR_LEN - 3] = CHARSEOF0;
		strcat(currentToken.attribute.errLexeme, "...");
	}
	else {
		strcpy(currentToken.attribute.errLexeme, lexeme);
	}
	for (i = 0; i < len; i++)
		if (lexeme[i] == '\n')
			line++;
	currentToken.code = ERR_T;
	return currentToken;
}


/*
 ************************************************************
 * The function prints the token returned by the scanner
 ***********************************************************
 */

mtyped_void printToken(Token t) {

	extern mtyped_chr* keywordTable[]; /* link to keyword table in */

	switch (t.code) {
	case RTE_T:
		printf("RTE_T\t\t%s", t.attribute.errLexeme);
		/* Call here run-time error handling component */
		if (errorNumber) {
			printf("%d", errorNumber);
			exit(errorNumber);
		}
		printf("\n");
		break;
	case ERR_T:
		printf("ERR_T\t\t%s\n", t.attribute.errLexeme);
		break;
	case SEOF_T:
		printf("SEOF_T\t\t%d\t\n", t.attribute.seofType);
		break;
	case VID_T:
		printf("VID_T\t\t%s\n", t.attribute.idLexeme);
		break;
	case STR_T:
		printf("STR_T\t\t%d\t ", (short)t.attribute.codeType);
		mtyped_chr content = (mtyped_chr)bGetContent(stringLiteralTable, (short)t.attribute.codeType);
		if (content != -1) {/*Beware of this. This checks for empty strings the integer value of the empty string("") was -1, this is danger */
			printf("%s\n", bGetContent(stringLiteralTable, (short)t.attribute.codeType));
		} else {
			printf("\n");
		}
		break;
	case IL_T:
		printf("IL_T\t\t%d\n", t.attribute.intValue);
		break;
	case FPL_T:
		printf("FPL_T\t\t%f\n", t.attribute.floatValue);
		break;
	case LPR_T:
		printf("LPR_T\n");
		break;
	case RPR_T:
		printf("RPR_T\n");
		break;
	case LBR_T:
		printf("LBR_T\n");
		break;
	case RBR_T:
		printf("RBR_T\n");
		break;
	case KW_T:
		printf("KW_T\t\t%s\n", t.attribute.idLexeme);
		break;
	case EOS_T:
		printf("EOS_T\n");
		break;
	case EQ_T:
		printf("EQ_T\n");
		break;
	case ISEQ_T:
		printf("ISEQ_T\n");
		break;
	case LT_T:
		printf("LT_T\n");
		break;
	case GT_T:
		printf("GT_T\n");
		break;
	case NE_T:
		printf("NE_T\t\tOP_NE\n");
		break;
	case NOT_T:
		printf("NOT_T\t\tOP_NOT\n");
		break;
	case OR_T:
		printf("OR_T\t\tOP_OR\n");
		break;
	case CMA_T:
		printf("CMA_T\n");
		break;
	case RSB_T:
		printf("RSB_T\n");
		break;
		case LSB_T:
		printf("LSB_T\n");
		break;
	case COM_T:
		printf("COM_T\t\t");	
		printf("%s\n", t.attribute.commentString);		
		break;
	case DIV_T:
		printf("DIV_T\n");
		break;
	case MULT_T:
		printf("MULT_T\n");
		break;
	case ADD_T:
		printf("ADD_T\n");
		break;
	case SUB_T:
		printf("SUB_T\n");
		break;
	case POW_T:
		printf("POW_T\n");
		break;
	case AND_T:
		printf("AND_T\n");
		break;
	case DIVE_T:
		printf("DIVE_T\n");
		break;
	case MULTE_T:
		printf("MULTE_T\n");
		break;
	case ADDE_T:
		printf("ADDE_T\n");
		break;
	case SUBE_T:
		printf("SUBE_T\n");
		break;
	case POWE_T:
		printf("POWE_T\n");
		break;
	case MNID_T:
		printf("MNID_T\t\t%s\n", t.attribute.idLexeme);
		break;

	default:
		printf("Scanner error: invalid token code: %d\n", t.code);
	}
}

