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
* File name: Buffer.h
* Compiler: MS Visual Studio 2022
* Author: Paulo Sousa
* Course: CST 8152 – Compilers, Lab Section: [011, 012, 013, 014]
* Assignment: A12.
* Date: Jan 01 2022
* Professor: Paulo Sousa
* Purpose: This file is the main header for Parser (.h)
************************************************************
*/

#ifndef COMPILERS_H_
#include "Compilers.h"
#endif

#ifndef BUFFER_H_
#define BUFFER_H_

/*#pragma warning(1:4001) *//*to enforce C89 type comments  - to make //comments an warning */

/*#pragma warning(error:4001)*//* to enforce C89 comments - to make // comments an error */

/* standard header files */
#include <stdio.h>  /* standard input/output */
#include <malloc.h> /* for dynamic memory allocation*/
#include <limits.h> /* implementation-defined data type ranges and limits */

/* CONSTANTS DEFINITION: GENERAL (NOT LANGUAGE DEPENDENT) .................................. */

/* Modes (used to create buffer) */
enum BUFFERMODES {
	MODE_FIXED = 'f',
	MODE_ADDIT = 'a',
	MODE_MULTI = 'm'
};

/* TO_DO: Adjust all constants to your Language */

#define BUFFER_ERROR (-1)						/* General error message */
#define BUFFER_EOF '\0'							/* General EOF */

/* CONSTANTS DEFINITION: PREFIXED BY LANGUAGE NAME (mtyped) .................................. */

#define BUFFER_DEFAULT_SIZE			200			/* default initial buffer capacity */
#define BUFFER_DEFAULT_INCREMENT	100			/* default increment factor */

/* Add your bit-masks constant definitions here - Defined for mtyped */
/* BITS                                (7654.3210) */
#define MTYPED_DEFAULT_FLAG 0x0F 	/* (0000.1111)_2 = (015)_10 */
#define MTYPED_REL 0x8F              
#define MTYPED_EOB 0x4F
#define MTYPED_FUL 0x2F
#define MTYPED_EMP 0x1F

/* STRUCTURES DEFINITION: SUFIXED BY LANGUAGE NAME (MTYPED) .................................. */

/* Offset declaration */
typedef struct position {
	mtyped_int markPos;			/* the offset (in chars) to the mark location */
	mtyped_int readPos;			/* the offset (in chars) to the get-character location */
	mtyped_int writePos;			/* the offset (in chars) to the add-character location */
} Position;

/* Buffer structure */
typedef struct buffer {
	mtyped_string	string;		/* pointer to the beginning of character array (character buffer) */
	mtyped_int	size;		    /* current dynamic memory size (in bytes) allocated to character buffer */
	mtyped_int	increment;		/* character array increment factor */
	mtyped_int	mode;			/* operational mode indicator */
	mtyped_flg	flags;			/* contains character array reallocation flag and end-of-buffer flag */
	Position	position;		/* Offset / position field */
} Buffer, * BufferPointer;

/* FUNCTIONS DECLARATION:  .................................. */

/* FUNCTIONS DECLARATION:  .................................. */
/* General Operations */
BufferPointer	bCreate(mtyped_int, mtyped_int, mtyped_int);
BufferPointer	bAddChar(BufferPointer const, mtyped_chr);
mtyped_bol		bRetract(BufferPointer const);
mtyped_bol		bRestore(BufferPointer const);
mtyped_bol		bRecover(BufferPointer const);
mtyped_bol		bClear(BufferPointer const);
mtyped_bol		bDestroy(BufferPointer const);
mtyped_bol		bSetMark(BufferPointer const, mtyped_int);
mtyped_bol		bIsEmpty(BufferPointer const);
mtyped_bol		bIsFull(BufferPointer const);
mtyped_bol		bIsRealocated(BufferPointer const);
mtyped_int		bPrint(BufferPointer const);
mtyped_int		bLoad(BufferPointer const, FILE* const);
/* Getters */
mtyped_int		bGetSize(BufferPointer const);
mtyped_int		bGetWritePos(BufferPointer const);
mtyped_int		bGetMarkPos(BufferPointer const);
mtyped_int		bGetReadPos(BufferPointer const);
mtyped_int		bGetIncrement(BufferPointer const);
mtyped_int		bGetMode(BufferPointer const);
mtyped_chr		bGetChar(BufferPointer const);
mtyped_chr*		bGetContent(BufferPointer const, mtyped_int);
mtyped_flg		bGetFlags(BufferPointer const);

#endif
