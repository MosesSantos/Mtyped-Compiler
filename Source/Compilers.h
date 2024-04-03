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
* File name: compilers.h
* Compiler: MS Visual Studio 2022
* Author: Paulo Sousa
* Course: CST 8152 – Compilers, Lab Section: [011, 012, 013, 014]
* Assignment: A12, A22, A32.
* Date: Jan 01 2022
* Professor: Paulo Sousa
* Purpose: This file defines the functions called by main function.
* Function list: mainBuffer(), mainScanner(), mainParser().
*************************************************************/

#ifndef COMPILERS_H_
#define COMPILERS_H_

#define DEBUG 0

/* Logical constants - adapt for your language */
#define mtyped_true	1
#define mtyped_false 0

/* You should add your own constant definitions here */
#define MTYPED_MAX_SIZE				SHRT_MAX-1	/* maximum capacity */ 
#define mtyped_null					NULL

/*
------------------------------------------------------------
Data types definitions
------------------------------------------------------------
*/

typedef char			mtyped_chr;
typedef char*			mtyped_string;
typedef int				mtyped_int;
typedef float			mtyped_flt;
typedef void			mtyped_void;

typedef unsigned char	mtyped_bol;
typedef unsigned char	mtyped_flg;


/*
------------------------------------------------------------
Programs:
1: Buffer - invokes MainBuffer code
2: Scanner - invokes MainScanner code
3: Parser - invokes MainParser code
------------------------------------------------------------
*/
enum PROGRAMS {
	PGM_BUFFER	= '1',
	PGM_SCANNER = '2',
	PGM_PARSER	= '3'
};

/*
------------------------------------------------------------
Main functions signatures
(Codes will be updated during next assignments)
------------------------------------------------------------
*/
mtyped_int mainBuffer(mtyped_int argc, mtyped_chr** argv);
mtyped_int mainScanner(mtyped_int argc, mtyped_chr** argv);
mtyped_int mainParser(mtyped_int argc, mtyped_chr** argv);

#endif
