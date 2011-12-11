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
 * fs_tools.c
 *
 *  Created on: 02-10-2010
 *      Author: Karol Nowicki
 *
 *	Narzedzia pomocne przy obsludze plikow
 *	jest tu jedynie wyswitlanie na dbgu nazwy bledu.
 *
 */


#include "Board.h"
#include "storage/fatfs/fs_tools.h"
#include "storage/sdcard/sd_spi.h"


//=================================================================================================
//								DEBUG UTILS
//=================================================================================================


void fsDispFRESULT(FRESULT res, char *buf)
{
	debug_msg("FRESULT = ");
	switch(res)
	{
		case FR_OK: strcpy(buf, "FR_OK"); break;				/* (0) Succeeded */
		case FR_DISK_ERR: strcpy(buf, "FR_DISK_ERR"); break;			/* (1) A hard error occured in the low level disk I/O layer */
		case FR_INT_ERR: strcpy(buf, "FR_INT_ERR"); break;				/* (2) Assertion failed */
		case FR_NOT_READY: strcpy(buf, "FR_NOT_READY"); break;			/* (3) The physical drive cannot work */
		case FR_NO_FILE: strcpy(buf, "FR_NO_FILE"); break;				/* (4) Could not find the file */
		case FR_NO_PATH: strcpy(buf, "FR_NO_PATH"); break;				/* (5) Could not find the path */
		case FR_INVALID_NAME: strcpy(buf, "FR_INVALID_NAME"); break;		/* (6) The path name format is invalid */
		case FR_DENIED: strcpy(buf, "FR_DENIED"); break;				/* (7) Acces denied due to prohibited access or directory full */
		case FR_EXIST: strcpy(buf, "FR_EXIST"); break;				/* (8) Acces denied due to prohibited access */
		case FR_INVALID_OBJECT: strcpy(buf, "FR_INVALID_OBJECT"); break;		/* (9) The file/directory object is invalid */
		case FR_WRITE_PROTECTED: strcpy(buf, "FR_WRITE_PROTECTED"); break;		/* (10) The physical drive is write protected */
		case FR_INVALID_DRIVE: strcpy(buf, "FR_INVALID_DRIVE"); break;		/* (11) The logical drive number is invalid */
		case FR_NOT_ENABLED: strcpy(buf, "FR_NOT_ENABLED"); break;			/* (12) The volume has no work area */
		case FR_NO_FILESYSTEM: strcpy(buf, "FR_NO_FILESYSTEM"); break;		/* (13) There is no valid FAT volume on the physical drive */
		case FR_MKFS_ABORTED: strcpy(buf, "FR_MKFS_ABORTED"); break;		/* (14) The f_mkfs() aborted due to any parameter error */
		case FR_TIMEOUT: strcpy(buf, "FR_TIMEOUT"); break;				/* (15) Could not get a grant to access the volume within defined period */
		case FR_LOCKED: strcpy(buf, "FR_LOCKED"); break;				/* (16) The operation is rejected according to the file shareing policy */
		case FR_NOT_ENOUGH_CORE: strcpy(buf, "FR_NOT_ENOUGH_CORE"); break;		/* (17) LFN working buffer could not be allocated */
		case FR_TOO_MANY_OPEN_FILES: strcpy(buf, "FR_TOO_MANY_OPEN_FILES"); break;
		default: strcpy(buf, "!!! UNDEFINED CODE");
	}
	debug_txt((char*)buf);
}




