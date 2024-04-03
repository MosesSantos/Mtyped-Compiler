/*
************************************************************
* COMPILERS COURSE - Algonquin College
* Code version: Winter, 2022
* Author: Svillen Ranev - Paulo Sousa
* Professors: Paulo Sousa / George Kriger / Abdullah Kadri
************************************************************
*/

/*
***********************************************************
* File name: buffer.c
* Compiler: MS Visual Studio 2022
* Author: Paulo Sousa
* Course: CST 8152 – Compilers, Lab Section: [011, 012, 013, 014]
* Assignment: A12.
* Date: Jan 01 2022
* Professor: Paulo Sousa
* Purpose: This file is the main code for Buffer (A12)
************************************************************
*/

#ifndef COMPILERS_H_
#include "Compilers.h"
#endif

#ifndef BUFFER_H_
#include "Buffer.h"
#endif

/*
***********************************************************
* Function name: bCreate
* Purpose: Creates the buffer according to capacity, increment
	factor and operational mode ('f', 'a', 'm')
* Author: Svillen Ranev / Paulo Sousa
* History/Versions: Ver 2.1
* Called functions: calloc(), malloc()
* Parameters:
*   size = initial capacity
*   increment = increment factor
*   mode = operational mode
* Return value: bPointer (pointer to Buffer)
* Algorithm: Allocation of memory according to inicial (default) values.
* TO_DO: 
*	- Adjust datatypes for your LANGUAGE.
*   - Use defensive programming
*	- Check boundary conditions
*	- Check flags.
*************************************************************
*/

BufferPointer bCreate(mtyped_int size, mtyped_int increment, mtyped_int mode) {

	BufferPointer b = (BufferPointer)mtyped_null;

	if (mode == MODE_ADDIT || mode == MODE_FIXED || mode == MODE_MULTI) {/*check if the mode is of correct type*/
		
		b = (BufferPointer)calloc(1, sizeof(Buffer));/*creates the buffer structure*/
		if (b) b->mode = mode;
	} 
	if(!b) {/*returns null if the buffer if the mode is of incorrect type*/
		return (BufferPointer)BUFFER_ERROR;/*return no buffer*/
	}
	if (size == 0) {

		b->size = BUFFER_DEFAULT_SIZE;
		b->increment = BUFFER_DEFAULT_INCREMENT;
	} else if (size < 0) {

		printf("Warning can't have negative size, setting to defaut values\n");
		b->size = BUFFER_DEFAULT_SIZE;
		b->increment = BUFFER_DEFAULT_INCREMENT;

	} else if (size > MTYPED_MAX_SIZE) {/*Idk about this*/
		printf("Can't set buffer size greater than max size");
		/*b->size = MTYPED_MAX_SIZE;*/
	} else {
		b->size = size;
	}

	if (increment == 0) {
		b->mode = MODE_FIXED;
	} else {
		b->increment = increment;
	}

	b->flags = MTYPED_EMP;
	b->string = (mtyped_string)calloc(b->size, sizeof(char));/*creates the string which holds the values of the of the read data in the buffer given the size*/
	return b;
}


/*
***********************************************************
* Function name: bAddChar
* Purpose: Adds a char to buffer
* Parameters:
*   pBuffer = pointer to Buffer Entity
*   ch = char to be added
* Return value:
*	bPointer (pointer to Buffer)
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/

BufferPointer bAddChar(BufferPointer const pBuffer, mtyped_chr ch) {

	mtyped_int newSize = 0;
	mtyped_string newString = (mtyped_string)mtyped_null;

	if (pBuffer == mtyped_null) {
		return (BufferPointer)BUFFER_ERROR;
	}
	
	pBuffer->flags = pBuffer->flags & MTYPED_DEFAULT_FLAG;
	switch (pBuffer->mode) {
	case MODE_FIXED:

		if (bGetWritePos(pBuffer) == pBuffer->size) {/*for if the input is larger than the size of the buffer in fixed mode*/
			pBuffer->flags = pBuffer->flags | MTYPED_FUL;
		} 
		break;
	case MODE_ADDIT:
		newSize = pBuffer->size + pBuffer->increment;
		break;
	case MODE_MULTI:
		newSize = pBuffer->size * pBuffer->increment;
		break;
	}

	if (bGetWritePos(pBuffer) == pBuffer->size && (newSize >= 0 && newSize < MTYPED_MAX_SIZE) && ch != (mtyped_chr)mtyped_null && pBuffer->mode != MODE_FIXED) {/*allocating space if the mode and bonds are correct*/
		
		newString = (mtyped_string)realloc(pBuffer->string, newSize);
		if (pBuffer->string != newString) {
			pBuffer->flags = pBuffer->flags | MTYPED_REL;/*setting realoaction bit*/
			pBuffer->string = newString;
		}
		pBuffer->size = newSize;
	} else if (newSize < 0 || newSize > MTYPED_MAX_SIZE && (pBuffer->flags & MTYPED_REL) == 143) {/*can't allocate more space*/
		return mtyped_null;
	}

	if ((pBuffer->flags & MTYPED_FUL) != 47 && ch != (mtyped_chr)mtyped_null && pBuffer->string) {/*Adds the char based on if the buffer is not full and the next char to add is not null*/
		pBuffer->string[pBuffer->position.writePos++] = ch;
	}
	return pBuffer;
}

/*
***********************************************************
* Function name: bClear
* Purpose: Clears the buffer
* Parameters:
*   pBuffer = pointer to Buffer Entity
* Return value:
*	Boolean value about operation success
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
mtyped_bol bClear(BufferPointer const pBuffer) {

	if (pBuffer == mtyped_null) {
		return mtyped_false;
	}
	pBuffer->position.writePos = pBuffer->position.markPos = pBuffer->position.readPos = 0;
	pBuffer->flags = MTYPED_EMP;
	return mtyped_true;//for now
}

/*
***********************************************************
* Function name: bDestroy
* Purpose: Releases the buffer address
* Parameters:
*   pBuffer = pointer to Buffer Entity
* Return value:
*	Boolean value about operation success
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
mtyped_bol bDestroy(BufferPointer const pBuffer) {

	if (pBuffer == mtyped_null) {
		return (mtyped_bol)BUFFER_ERROR;
	}
	free(pBuffer->string);
	free(pBuffer);
	return mtyped_true;
}

/*
***********************************************************
* Function name: bIsFull
* Purpose: Checks if buffer is full
* Parameters:
*   pBuffer = pointer to Buffer Entity
* Return value:
*	Boolean value about operation success
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
mtyped_bol bIsFull(BufferPointer const pBuffer) {

	if (pBuffer == mtyped_null) {
		return (mtyped_bol)BUFFER_ERROR;
	}
	if ((pBuffer->flags & MTYPED_FUL) == 47) {
		return mtyped_true;
	} else {
		return mtyped_false;
	}
}

/*
***********************************************************
* Function name: bGetWritePos
* Purpose: Returns the position of char to be added
* Parameters:
*   pBuffer = pointer to Buffer Entity
* Return value:
*	addcPosition value
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
mtyped_int bGetWritePos(BufferPointer const pBuffer) {

	if (pBuffer == mtyped_null){
		return BUFFER_ERROR;	
	}
	return pBuffer->position.writePos;
}

/*
***********************************************************
* Function name: bGetSize
* Purpose: Returns the current buffer capacity
* Parameters:
*   pBuffer = pointer to Buffer Entity
* Return value:
*	Size of buffer.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
mtyped_int bGetSize(BufferPointer const pBuffer) {

	if (pBuffer == mtyped_null) {
		return BUFFER_ERROR;
	}
	return pBuffer->size;
}

/*
***********************************************************
* Function name: bGetMode
* Purpose: Returns the operational mode
* Parameters:
*   pBuffer = pointer to Buffer Entity
* Return value:
*	operational mode.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
mtyped_int bGetMode(BufferPointer const pBuffer) {

	if (pBuffer == mtyped_null) {//ask about returning null
		return BUFFER_ERROR;
	}
	return pBuffer->mode;
}


/*
***********************************************************
* Function name: bGetMarkPos
* Purpose: Returns the position of mark in the buffer
* Parameters:
*   pBuffer = pointer to Buffer Entity
* Return value:
*	mark offset.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
mtyped_int bGetMarkPos(BufferPointer const pBuffer) {
	
	if (pBuffer == mtyped_null) {//ask about returning null
		return BUFFER_ERROR;
	}
	return pBuffer->position.markPos;
}


/*
***********************************************************
* Function name: bSetMark
* Purpose: Adjust the position of mark in the buffer
* Parameters:
*   pBuffer = pointer to Buffer Entity
*   mark = mark position for char
* Return value:
*	Boolean value about operation success
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
mtyped_bol bSetMark(BufferPointer const pBuffer, mtyped_int mark) {

	if (pBuffer == mtyped_null) {//ask about returning null
		return (mtyped_bol)BUFFER_ERROR;
	}
	pBuffer->position.markPos = mark;
	return mtyped_true;
}

/*
***********************************************************
* Function name: bPrint
* Purpose: Prints the string in the buffer.
* Parameters:
*   pBuffer = pointer to Buffer Entity
* Return value:
*	Number of chars printed.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
mtyped_int bPrint(BufferPointer const pBuffer) {

	mtyped_int size = 1;
	mtyped_chr c;

	if (pBuffer == mtyped_null) {
		return BUFFER_ERROR;
	}
	c = bGetChar(pBuffer);
	while (c > 0) {//for now

		if ((pBuffer->flags & MTYPED_EOB) != 79) {/*has not reached the end of the buffer*/
			size++;
			printf("%c", c);
			c = bGetChar(pBuffer);
		} else {
			break;
		}
	}
	return size;
}

/*
***********************************************************
* Function name: bLoad
* Purpose: Loads the string in the buffer with the content of
	an specific file.
* Parameters:
*   pBuffer = pointer to Buffer Entity
*   fi = pointer to file descriptor
* Return value:
*	Number of chars read and put in buffer.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
mtyped_int bLoad(BufferPointer const pBuffer, FILE* const fi) {

	mtyped_int size = 1;
	mtyped_chr c;

	if (pBuffer == mtyped_null) {
		return BUFFER_ERROR;
	}
	c = (mtyped_chr)fgetc(fi);
	while (!feof(fi)) {
		if (!bAddChar(pBuffer, c)) {	
			ungetc(c, fi);
			return BUFFER_ERROR;
		}
		c = (char)fgetc(fi);
		size++;
	}
	return size;
}

/*
***********************************************************
* Function name: bIsEmpty
* Purpose: Checks if buffer is empty.
* Parameters:
*   pBuffer = pointer to Buffer Entity
* Return value:
*	Boolean value about operation success
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
mtyped_bol bIsEmpty(BufferPointer const pBuffer) {

	if (pBuffer == mtyped_null) {
		return (mtyped_bol)BUFFER_ERROR;
	}
	if ((pBuffer->flags & MTYPED_EMP) == 31) {
		return mtyped_true;
	} else {
		return mtyped_false;
	}
}

/*
***********************************************************
* Function name: bGetChar
* Purpose: Returns the char in the getC position.
* Parameters:
*   pBuffer = pointer to Buffer Entity
* Return value:
*	Char in the getC position.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
mtyped_chr bGetChar(BufferPointer const pBuffer) {

	if (pBuffer == mtyped_null) { 
		return (mtyped_chr)BUFFER_ERROR;
	}

	if (pBuffer->position.readPos == pBuffer->position.writePos) {
		pBuffer->flags = pBuffer->flags | MTYPED_EOB;
		return BUFFER_EOF;
	} else {
		pBuffer->flags = pBuffer->flags & MTYPED_EOB;
	}

	return pBuffer->string[pBuffer->position.readPos++];
}


/*
***********************************************************
* Function name: bRecover
* Purpose: Rewinds the buffer.
* Parameters:
*   pBuffer = pointer to Buffer Entity
* Return value
*	Boolean value about operation success
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
mtyped_bol bRecover(BufferPointer const pBuffer) {

	if (pBuffer == mtyped_null) {
		return (mtyped_bol)BUFFER_ERROR;
	}
	pBuffer->position.readPos = 0;
	pBuffer->position.markPos = 0;
	return mtyped_true;
}


/*
***********************************************************
* Function name: bRetract
* Purpose: Retracts the buffer to put back the char in buffer.
* Parameters:
*   pBuffer = pointer to Buffer Entity
* Return value:
*	Boolean value about operation success
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
mtyped_bol bRetract(BufferPointer const pBuffer) {

	if (pBuffer == mtyped_null) {
		return (mtyped_bol)BUFFER_ERROR;
	}
	if (pBuffer->position.readPos > 0) {
		pBuffer->position.readPos--;
		return mtyped_true;
	}
	return mtyped_false;
}


/*
***********************************************************
* Function name: bRestore
* Purpose: Resets the buffer.
* Parameters:
*   pBuffer = pointer to Buffer Entity
* Return value:
*	Boolean value about operation success
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
mtyped_bol bRestore(BufferPointer const pBuffer) {

	if (pBuffer == mtyped_null) {
		return (mtyped_bol)BUFFER_ERROR;

	}
	pBuffer->position.readPos = pBuffer->position.markPos;
	return mtyped_true;
}


/*
***********************************************************
* Function name: bGetReadPos
* Purpose: Returns the value of getCPosition.
* Parameters:
*   pBuffer = pointer to Buffer Entity
* Return value:
*	The readPos offset.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
mtyped_int bGetReadPos(BufferPointer const pBuffer) {

	if (pBuffer == mtyped_null) {
		return BUFFER_ERROR;
	}
	return pBuffer->position.readPos;
}


/*
***********************************************************
* Function name: bGetIncrement
* Purpose: Returns the buffer increment.
* Parameters:
*   pBuffer = pointer to Buffer Entity
* Return value:
*	The Buffer increment.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
mtyped_int bGetIncrement(BufferPointer const pBuffer) {

	if (pBuffer == mtyped_null) {
		return BUFFER_ERROR;
	}
	return pBuffer->increment;
}


/*
***********************************************************
* Function name: bGetContent
* Purpose: Returns the pointer to String.
* Parameters:
*   pBuffer = pointer to Buffer Entity
*   pos = position to get the pointer
* Return value:
*	Position of string char.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
mtyped_string bGetContent(BufferPointer const pBuffer, mtyped_int pos) {/*Currently working*/
	
	if ((pos >= 0) && (pos < pBuffer->position.writePos)) {	
		return pBuffer->string + pos;/*returning value in the buffer of index pos*/
	} else {
		return (mtyped_string)BUFFER_ERROR;
	}
}


/*
***********************************************************
* Function name: bGetFlags
* Purpose: Returns the entire flags of Buffer.
* Parameters:
*   pBuffer = pointer to Buffer Entity
* Return value:
*	Flags from Buffer.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
#define FLAGS_
#undef FLAGS_
#ifndef FLAGS_
mtyped_flg bGetFlags(BufferPointer const pBuffer) {
	/* TO_DO: Defensive programming */
	if (pBuffer == mtyped_null) {
		return (mtyped_flg)BUFFER_ERROR;
	}
	return pBuffer->flags;
}
#else
#define bGetFlags(pBuffer) ((pBuffer)?(pBuffer->flags):(RT_FAIL_1))
#endif
