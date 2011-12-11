/*
/-----------------------------------------------------------------------------/
/ NowBoot is a Bootloader module for small embedded systems which
/ working with FreeRTOS.
/ With few changes in boot.s containing exception vectors
/ it can be use with any other firmware.
/ C 2010, Karol Nowicki
/
/ NowBoot contain some code which is provided by third-parties like:
/ 	-FatFs
/	-Atmel peripherial drivers
/	-FreeRTOS (sample boot.s startup file)
/ AND IS LICENSED SEPARATELY FROM NowBoot.
/
/
/
/ This is a free software that opened for education, research and commercial
/ developments under license policy of following terms.
/
/
/ * The NowBoot module is a free software and there is NO WARRANTY.
/ * No restriction on use. You can use, modify and redistribute it for
/   personal, non-profit or commercial products UNDER YOUR RESPONSIBILITY.
/ * Redistributions of source code must retain the above copyright notice.
/
/-----------------------------------------------------------------------------/

/*
 * common.c
 *
 *  Created on: 02-10-2010
 *      Author: Karol Nowicki
 */

#include "common/common.h"


/*-------------------------------------------------------------------
 * 	global functions (ex for stdlib ) unused as yet
 *------------------------------------------------------------------*/

//------------------------------------------------------------------------------
/// Implementation of fputc using the DBGU as the standard output. Required
/// for printf().
/// Returns the character written if successful, or -1 if the output stream is
/// not stdout or stderr.
/// \param c  Character to write.
/// \param pStream  Output stream.
//------------------------------------------------------------------------------
signed int fputc(signed int c, FILE *pStream){
    if ((pStream == stdout) || (pStream == stderr)) {
    	debug_chr(c);
        return c;
    }
    else {
        return EOF;
    }
}

//------------------------------------------------------------------------------
/// Implementation of fputs using the DBGU as the standard output. Required
/// for printf(). Does NOT currently use the PDC.
/// Returns the number of characters written if successful, or -1 if the output
/// stream is not stdout or stderr.
/// \param pStr  String to write.
/// \param pStream  Output stream.
//------------------------------------------------------------------------------
signed int fputs(const char *pStr, FILE *pStream){
    signed int num = 0;
    while (*pStr != 0) {
        if (fputc(*pStr, pStream) == -1) {
            return -1;
        }
        num++;
        pStr++;
    }
    return num;
}



