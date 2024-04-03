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
* File name: mainBuffer.c
* Compiler: MS Visual Studio 2022
* Author: Paulo Sousa
* Course: CST 8152 – Compilers, Lab Section: [011, 012, 013, 014]
* Assignment: A12, A22, A32.
* Date: Jan 01 2022
* Professor: Paulo Sousa
* Purpose: This file contains all functionalities from Parser.
* Function list: (...).
************************************************************
*/

/* TODO: Adjust the function header */

#ifndef COMPILERS_H_
#include "Compilers.h"
#endif

#ifndef PARSER_H_
#include "Parser.h"
#endif

/*
************************************************************
 * Process Parser
 ***********************************************************
 */
/* TODO: This is the function to start the parser - check your program definition */

mtyped_void startParser() {
	lookahead = tokenizer();
	if (lookahead.code != SEOF_T) {
		program();
	}
	matchToken(SEOF_T, NO_ATTR);
	printf("%s%s\n", STR_LANGNAME, ": Source file parsed");
}


/*
 ************************************************************
 * Match Token
 ***********************************************************
 */
/* TODO: This is the main code for match - check your definition */
mtyped_void matchToken(mtyped_int tokenCode, mtyped_int tokenAttribute) {
	mtyped_int matchFlag = 1;
	switch (lookahead.code) {
	case KW_T:
	case VID_T:
	default:
		if (lookahead.code != tokenCode)
			matchFlag = 0;
	}
	if (matchFlag && lookahead.code == SEOF_T)
		return;
	if (matchFlag) {
		lookahead = tokenizer();
		if (lookahead.code == ERR_T) {
			printError();
			lookahead = tokenizer();
			syntaxErrorNumber++;
		}
	}
	else
		syncErrorHandler(tokenCode);
}

/*
 ************************************************************
 * Syncronize Error Handler
 ***********************************************************
 */
/* TODO: This is the function to handler error - adjust basically datatypes */
mtyped_void syncErrorHandler(mtyped_int syncTokenCode) {
	printError();
	syntaxErrorNumber++;
	while (lookahead.code != syncTokenCode) {
		if (lookahead.code == SEOF_T)
			exit(syntaxErrorNumber);
		lookahead = tokenizer();
	}
	if (lookahead.code != SEOF_T)
		lookahead = tokenizer();
}

/*
 ************************************************************
 * Print Error
 ***********************************************************
 */
/* TODO: This is the function to error printing - adjust basically datatypes */
mtyped_void printError() {
	Token t = lookahead;
	printf("%s%s%3d\n", STR_LANGNAME, ": Syntax error:  Line:", line);
	printf("*****  Token code:%3d Attribute: ", t.code);
	switch (t.code) {
	case ERR_T:
		printf("%s\n", t.attribute.errLexeme);
		break;
	case SEOF_T:
		printf("SEOF_T\t\t%d\t\n", t.attribute.seofType);
		break;
	case MNID_T:
		printf("%s\n", t.attribute.idLexeme);
		break;
	case STR_T:
		printf("%s\n", bGetContent(stringLiteralTable, t.attribute.contentString));
		break;
	case KW_T:
		printf("%s\n", keywordTable[t.attribute.codeType]);
		break;
	case LPR_T:
	case RPR_T:
	case LBR_T:
	case RBR_T:
	case EOS_T:
		printf("NA\n");
		break;
	default:
		printf("%s%s%d\n", STR_LANGNAME, ": Scanner error: invalid token code: ", t.code);
	}
}

/*
 ************************************************************
 * Program statement
 * BNF: <program> -> PLATYPUS { <opt_statements> }
 * FIRST(<program>)= {KW_T (MAIN)}.
 ***********************************************************
 */
mtyped_void program() {

	comments();
	switch (lookahead.code) {
	case MNID_T:
		if (lookahead.code == MNID_T) {
			matchToken(MNID_T, MAIN);
			matchToken(LPR_T, NO_ATTR);
			matchToken(RPR_T, NO_ATTR);
			matchToken(LBR_T, NO_ATTR);
			codeSession();
			matchToken(RBR_T, NO_ATTR);
			break;
		} else {
			printError();
		}
	case SEOF_T:
		; // Empty
		break;
	default:
		printError();
	}
	printf("%s%s\n", STR_LANGNAME, ": Program parsed");
}


/*
 ************************************************************
 * dataSession
 * BNF: <dataSession> -> DATA { <opt_varlist_declarations> }
 * FIRST(<program>)= {KW_T (DATA)}.
 ***********************************************************
 */
//mtyped_void dataSession() {
//	matchToken(KW_T, DATA);
//	matchToken(LBR_T, NO_ATTR);
//	optVarListDeclarations();
//	matchToken(RBR_T, NO_ATTR);
//	printf("%s%s\n", STR_LANGNAME, ": Data Session parsed");
//}

/*
 ************************************************************
 * Optional Var List Declarations
 * BNF: <opt_varlist_declarations> -> <varlist_declarations> | e
 * FIRST(<opt_varlist_declarations>) = { e, KW_T (INT), KW_T (FLOAT), KW_T (STRING)}.
 ***********************************************************
 */
mtyped_void optVarListDeclarations() {
	
	switch (lookahead.code) {
	case KW_T:
		varListsDeclaration();
		break;
	default:
		return;
	}
	printf("%s%s\n", STR_LANGNAME, ": Optional Varable List Declerations parsed");
}
mtyped_void varListsDeclaration() {

	if (lookahead.code == KW_T) {

		if (strncmp(lookahead.attribute.idLexeme, "int", 3) == 0) {
			matchToken(KW_T, INT);
		} else if (strncmp(lookahead.attribute.idLexeme, "float", 5) == 0) {
			matchToken(KW_T, FLOAT);
		} else if (strncmp(lookahead.attribute.idLexeme, "string", 6) == 0) {
			matchToken(KW_T, STRING);
		}
		varListDeclaration();
		
		if (strncmp(lookahead.attribute.idLexeme, "=", 1) == 0) {
			assignmentExpression();
		}
		matchToken(EOS_T, NO_ATTR);
	}
	else {
		return;
	}
	printf("%s%s\n", STR_LANGNAME, ": Varable Lists Declerations parsed");
}
mtyped_void varListDeclaration() {

	matchToken(VID_T, NO_ATTR);

	if (lookahead.code == CMA_T) {
		matchToken(CMA_T, NO_ATTR);
		varListDeclaration();
	} else {
		return;
	}
	printf("%s%s\n", STR_LANGNAME, ": Varable Lists Declerations parsed");
}

/*
 ************************************************************
 * codeSession statement
 * BNF: <codeSession> -> CODE { <opt_statements> }
 * FIRST(<codeSession>)= {KW_T (CODE)}.
 ***********************************************************
 */
mtyped_void codeSession() {


	if (lookahead.code != RBR_T) {
		if (lookahead.code == COM_T) {
			comments();
			codeSession();
		}
		optVarListDeclarations();
		optionalStatements();
		codeSession();
	} else {
		printf("%s%s\n", STR_LANGNAME, ": Code Session parsed");
	}
	return;
}

/* TODO_205: Continue the development (all non-terminal functions) */

/*
 ************************************************************
 * Optional statement
 * BNF: <opt_statements> -> <statements> | ϵ
 * FIRST(<opt_statements>) = { ϵ , IVID_T, FVID_T, SVID_T, KW_T(IF),
 *				KW_T(WHILE), KW_T(READ), KW_T(WRITE) }
 ***********************************************************
 */
mtyped_void optionalStatements() {


	if (lookahead.code != KW_T && lookahead.code != RBR_T && lookahead.code != COM_T) {
		statement();
	} else if (lookahead.code == COM_T) {
		comments();
	}
	printf("%s%s\n", STR_LANGNAME, ": Optional statements parsed");
}


/*
 ************************************************************
 * Single statement
 * BNF: <statement> ->  <assignment statement> | <selection statement> |
 *	<iteration statement> | <input statement> | <output statement>
 * FIRST(<statement>) = { IVID_T, FVID_T, SVID_T, KW_T(IF), KW_T(WHILE),
 *			KW_T(READ), KW_T(WRITE) }
 ***********************************************************
 */
mtyped_void statement() {

	switch (lookahead.code) {
	case VID_T:
		if (strncmp(lookahead.attribute.idLexeme, "print", 5) == 0) {
			matchToken(VID_T, NO_ATTR);
			matchToken(LPR_T, NO_ATTR);
			outputStatement();
			matchToken(RPR_T, NO_ATTR);
			matchToken(EOS_T, NO_ATTR);
		//output statment
		} else if (strncmp(lookahead.attribute.idLexeme, "read", 4) == 0) {
			matchToken(VID_T, NO_ATTR);
			matchToken(LPR_T, NO_ATTR);
			matchToken(VID_T, NO_ATTR);
			matchToken(RPR_T, NO_ATTR);
			matchToken(EOS_T, NO_ATTR);
		//input statment
		} else {
			matchToken(VID_T, NO_ATTR);
			assignmentExpression();
			matchToken(EOS_T, NO_ATTR);
		}
		break;
	default:
		printError();
		return;
	}
	printf("%s%s\n", STR_LANGNAME, ": Statement parsed");
}

/*
 ************************************************************
 * Assignment Statement
 * BNF: <assignment statement> -> <assignment expression>
 * FIRST(<assignment statement>) = { IVID_T, FVID_T, SVID_T, KW_T(IF),
 *		KW_T(WHILE), KW_T(READ), KW_T(WRITE) }
 ***********************************************************
 */
mtyped_void assignmentStatement() {
	assignmentExpression();
	printf("%s%s\n", STR_LANGNAME, ": Assignment statement parsed");
}

mtyped_void comments() {
	
	if (lookahead.code == COM_T) {
		matchToken(COM_T, NO_ATTR);
		comments();
	} else {
		printf("%s%s\n", STR_LANGNAME, ": Comment statements parsed");
		return;
	}
}

/*
 ************************************************************
 * Assignment Expression
 * BNF: <assignment expression> -> <integer_variable> = <arithmetic expression>
 *						| <float_variable> = <arithmetic expression>
 *						| <string_variable>= <string expression>
 * FIRST(<assignment expression>) = { IVID_T, FVID_T, SVID_T }
 ***********************************************************
 */
mtyped_void assignmentExpression() {
	
	switch (lookahead.code) {
		case EQ_T:
			matchToken(EQ_T, NO_ATTR);
			if (lookahead.code == STR_T) {
				primaryStringExpression();
			} else {
				arithmeticExpression();
			}
			break;
	default:
		printError();
	}
	printf("%s%s\n", STR_LANGNAME, ": Assignment expression parsed");
}

mtyped_void arithmeticExpression() {

	unaryArithmeticExpression();
	additiveArithmeticExpression();
	printf("%s%s\n", STR_LANGNAME, ": Arithmetic Expression parsed");
}

mtyped_void unaryArithmeticExpression() {

	if (lookahead.code == SUB_T) {
		matchToken(SUB_T, NO_ATTR);
		printf("%s%s\n", STR_LANGNAME, ": Unary Arithmetic Expression parsed");
	} 
}

mtyped_void additiveArithmeticExpression() {

	multiplicativeArithmeticExpression();
	if (lookahead.code == ADD_T) {
		matchToken(ADD_T, NO_ATTR);
		multiplicativeArithmeticExpression();
		if (lookahead.code == ADD_T || lookahead.code == SUB_T) {
			matchToken(lookahead.code, NO_ATTR);
			multiplicativeArithmeticExpression();
		}
	} else if (lookahead.code == SUB_T) {
		matchToken(SUB_T, NO_ATTR);
		multiplicativeArithmeticExpression();
		if (lookahead.code == ADD_T || lookahead.code == SUB_T) {
			matchToken(lookahead.code, NO_ATTR);
			multiplicativeArithmeticExpression();
		}
	} else {
		return;
	}
	printf("%s%s\n", STR_LANGNAME, ": Additive Arithmetic Expression parsed");
}
mtyped_void multiplicativeArithmeticExpression() {

	primaryArithmeticExpression();
	if (lookahead.code == MULT_T) {
		matchToken(MULT_T, NO_ATTR);
		primaryArithmeticExpression();
		if (lookahead.code == MULT_T || lookahead.code == DIV_T) {

			matchToken(lookahead.code, NO_ATTR);
			multiplicativeArithmeticExpression();
		}
	} else if (lookahead.code == DIV_T) {
		matchToken(DIV_T, NO_ATTR);
		primaryArithmeticExpression();
		if (lookahead.code == MULT_T || lookahead.code == DIV_T) {

			matchToken(lookahead.code, NO_ATTR);
			multiplicativeArithmeticExpression();
		}
	} else {
		return;
	}
	printf("%s%s\n", STR_LANGNAME, ": Multiplicative Arithmetic Expression parsed");
}

mtyped_void primaryArithmeticExpression() {

	if (lookahead.code == VID_T) {
		matchToken(VID_T, NO_ATTR);
	} else if (lookahead.code == IL_T || lookahead.code == FPL_T) {
		switch (lookahead.code) {
		case IL_T:
			matchToken(IL_T, NO_ATTR);
			break;
		case FPL_T:
			matchToken(FPL_T, NO_ATTR);
			break;
		}
	} else {
		if (lookahead.code == LPR_T) {
			matchToken(LPR_T, NO_ATTR);
			arithmeticExpression();
			matchToken(RPR_T, NO_ATTR);
		} else {
			arithmeticExpression();
		}
	}
	printf("%s%s\n", STR_LANGNAME, ": Primary Arithmetic Expression parsed");
}
/**/

mtyped_void primaryStringExpression(){

	matchToken(STR_T, NO_ATTR);
	printf("%s%s\n", STR_LANGNAME, ": Primary String Expression parsed");
}
mtyped_void outputStatement() {

	switch (lookahead.code) {

	case VID_T:
		matchToken(VID_T, NO_ATTR);
		break;
	case STR_T:
		matchToken(STR_T, NO_ATTR);
		break;
	case IL_T:
		matchToken(IL_T, NO_ATTR);
		break;
	case FPL_T:
		matchToken(FPL_T, NO_ATTR);
		break;
	default:
		printError();
		break;
	}
	printf("%s%s\n", STR_LANGNAME, ": Output Statment  parsed");
}