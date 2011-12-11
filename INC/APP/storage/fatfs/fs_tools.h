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
/-----------------------------------------------------------------------------


/*
 * fs_tools.h
 *
 *  Created on: 02-10-2010
 *      Author: Karol Nowicki
 *
 *	Narzedzia pomocne przy obsludze plikow
 *	jest tu jedynie wyswitlanie na dbgu nazwy bledu.
 *
 */


#ifndef __FS_TOOLS__
#define __FS_TOOLS__

#include "common/common.h"
#include "storage/fatfs/ff.h"

#define F_LSEEK_EOF				0xFFFFFFFF
#define FS_FILE_PRESENT			0
#define FS_NO_FILE				FR_NO_FILE

#define RES_TEXT_BUFFER_SIZE	0x40
#define FILE_BUFFER_SIZE		0x100



//debug
void fsDispFRESULT(FRESULT res, char* buf);



#endif

