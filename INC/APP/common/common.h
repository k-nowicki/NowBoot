/*

/-----------------------------------------------------------------------------/
/ NowBoot module is a Bootloader module for small embedded systems which
/ working with FreeRTOS.
/ With few changes in boot.s containing exception vectors
/ it can be use with any other firmware.
/  2010, Karol Nowicki
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
 * common.h
 *
 *  Created on: 02-10-2010
 *      Author: Karol Nowicki
 *
 *	Wiekszosc plikow projektu wlacza ten plik, i w nim sa
 *	zawarte wszystkie uniwersalne headery, makra, definicje etc
 *
 */



#ifndef COMMON_H_
#define COMMON_H_


/*-------------------------------------------------------------------
 * 	header includes
 *------------------------------------------------------------------*/

/* Standard includes. */
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <inttypes.h>


//Application Drivers includes
//very-low-level
#include "Board.h"				//hardware specified
#include "common/conv.h"
#include "common/tc.h"		//delay functions and macros
#include "usart/dbgu.h"				//debug com port

#include "tc.h"
#include "common/delay.h"		//delay functions and macros

//system plikow, a w zasadznie obsluga karty i dataflasha
#include "storage/spi/spi.h"
#include "storage/sdcard/sd_spi.h"
#include "storage/dataflash/df_spi.h"

#include "storage/fatfs/fs_tools.h"


//Project application includes



/*-------------------------------------------------------------------
 * 	global functions (ex for stdlib ) unused as yet
 *------------------------------------------------------------------*/
signed int fputc(signed int c, FILE *pStream);
signed int fputs(const char *pStr, FILE *pStream);



/*-------------------------------------------------------------------
 * 	global constans and macros
 *------------------------------------------------------------------*/

// Hardware constans and macros

//RELAY_1
#define RELAY_1_PIO		AT91C_BASE_PIOB
#define RELAY_1_MASK	AT91C_PIO_PB16
//RELAY_1s interface defs
#define RELAY_1_INIT()		{RELAY_1_PIO->PIO_PER = RELAY_1_MASK; RELAY_1_PIO->PIO_OER = RELAY_1_MASK; RELAY_1_PIO->PIO_OWER = RELAY_1_MASK;}
#define RELAY_1_ON()		RELAY_1_PIO->PIO_SODR  = RELAY_1_MASK
#define RELAY_1_OFF()		RELAY_1_PIO->PIO_CODR  = RELAY_1_MASK
#define RELAY_1_TOGGLE()	RELAY_1_PIO->PIO_ODSR ^= RELAY_1_MASK

//RELAY_2
#define RELAY_2_PIO		AT91C_BASE_PIOB
#define RELAY_2_MASK	AT91C_PIO_PB17
//RELAY_2s interface defs
#define RELAY_2_INIT()		{RELAY_2_PIO->PIO_PER = RELAY_2_MASK; RELAY_2_PIO->PIO_OER = RELAY_2_MASK; RELAY_2_PIO->PIO_OWER = RELAY_2_MASK;}
#define RELAY_2_ON()		RELAY_2_PIO->PIO_SODR  = RELAY_2_MASK
#define RELAY_2_OFF()		RELAY_2_PIO->PIO_CODR  = RELAY_2_MASK
#define RELAY_2_TOGGLE()	RELAY_2_PIO->PIO_ODSR ^= RELAY_2_MASK

//OUT_3
#define OUTX_3_PIO		AT91C_BASE_PIOB
#define OUTX_3_MASK		AT91C_PIO_PB18
//OUT_3s interface defs
#define OUTX_3_INIT()		{OUTX_3_PIO->PIO_PER = OUTX_3_MASK; OUTX_3_PIO->PIO_OER = OUTX_3_MASK; OUTX_3_PIO->PIO_OWER = OUTX_3_MASK;}
#define OUTX_3_ON()			OUTX_3_PIO->PIO_SODR  = OUTX_3_MASK
#define OUTX_3_OFF()		OUTX_3_PIO->PIO_CODR  = OUTX_3_MASK
#define OUTX_3_TOGGLE()		OUTX_3_PIO->PIO_ODSR ^= OUTX_3_MASK

//OUT_4
#define OUTX_4_PIO		AT91C_BASE_PIOB
#define OUTX_4_MASK		AT91C_PIO_PB19
//OUT_4s interface defs
#define OUTX_4_INIT()		{OUTX_4_PIO->PIO_PER = OUTX_4_MASK; OUTX_4_PIO->PIO_OER = OUTX_4_MASK; OUTX_4_PIO->PIO_OWER = OUTX_4_MASK;}
#define OUTX_4_ON()			OUTX_4_PIO->PIO_SODR  = OUTX_4_MASK
#define OUTX_4_OFF()		OUTX_4_PIO->PIO_CODR  = OUTX_4_MASK
#define OUTX_4_TOGGLE()		OUTX_4_PIO->PIO_ODSR ^= OUTX_4_MASK




/*-------------------------------------------------------------------
 * 	global varebles declarations
 *------------------------------------------------------------------*/






/*-------------------------------------------------------------------
 * 	others
 *------------------------------------------------------------------*/

#define OK				1
#define FAIL			0


#endif /* COMMON_H_ */
