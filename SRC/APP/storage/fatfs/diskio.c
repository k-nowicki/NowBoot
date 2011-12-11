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
 * diskio.c
 *
 *  Created on: 02-10-2010
 *      Author: Karol Nowicki
 *
 *	Sterownik dopasowujacy uniwersalne sterowniki DataFlash i SD_card
 *	do systemu plików FatFs
 *
 */


#include "storage/fatfs/ffconf.h"		/* FatFs configuration options */
#include "storage/sdcard/sd_spi.h"
#include "storage/fatfs/diskio.h"


DSTATUS disk_initialize ( BYTE Drive           /* Physical drive number */
		)
{
	switch (Drive){
	case 1:
		if(sdCardInit()==OK) return(STA_OK); else return(STA_NOINIT);
		return(0);
	case 0:
		if(dfSPIInit()==OK)  return(STA_OK); else return(STA_NOINIT);
	default:
		return(RES_PARERR);
	}

}


DSTATUS disk_shutdown (BYTE Drive           /* Physical drive number */)
{
	return(STA_OK);
}


DSTATUS disk_status (BYTE Drive           /* Physical drive number */)
{
	return(STA_OK);
}


DRESULT disk_read (	BYTE Drive,          /* Physical drive number */
					BYTE* buf,
					DWORD lba,
					BYTE count)
{
	switch (Drive){
	case 1:
		sdSPIInit();
		if(sdReadBlocks(lba,count,buf)==OK) return(RES_OK); else return(RES_ERROR);
	case 0:
		dfSPIInit();
		if(dfReadBlocks(lba,count,buf)==OK) return(RES_OK); else return(RES_ERROR);
	default:
		return(RES_PARERR);
	}

}


#if	_FS_READONLY == 0
DRESULT disk_write (BYTE Drive,          /* Physical drive number */
					const BYTE* buf,
					DWORD lba,
					BYTE count
					)
{
	switch (Drive){
	case 1:
		sdSPIInit();
		if(sdWriteBlocks(lba,count,(uint8_t*)buf)==OK) return(RES_OK); else return(RES_ERROR);
	case 0:
		dfSPIInit();
		if(dfWriteBlocks(lba,count,(uint8_t*)buf)==OK) return(RES_OK); else return(RES_ERROR);
	default:
		return(RES_PARERR);
	}
}
#endif



/*-----------------------------------------------------------------------*/
/* Wait for card ready                                                   */
/*-----------------------------------------------------------------------*/

static
BYTE wait_ready (void)
{
	BYTE res;
	sdSPIInit();
	sdSPIDataTransfer(0xff);
	do
		res = sdSPIDataTransfer(0xff);
	while (res != 0xFF);

	return res;
}



DRESULT disk_ioctl (
		  BYTE Drive,      /* Drive number */
		  BYTE Command,    /* Control command code */
		  void* Buffer     /* Parameter and data buffer */
		)
{
	DWORD NumberOfBlocks;
	WORD BlockSize;
	DRESULT res = RES_ERROR;

	switch (Drive){
		case 1:
			sdSPIInit();
			switch(Command){
			case CTRL_SYNC:
				/*	Make sure that the disk drive has finished pending write process.
				 * 	When the disk I/O module has a write back cache, flush the dirty sector immediately.
				 * 	This command is not used in read-only configuration.
				 */
				if (wait_ready() == 0xFF)
					res = RES_OK;
				break;
			case GET_SECTOR_SIZE:
				/*	Returns sector size of the drive into the WORD variable pointed by Buffer.
				 * 	This command is not used in fixed sector size configuration, _MAX_SS is 512.
				 */
				*(WORD*)Buffer = 512;
				res = RES_OK;
				break;

			case GET_SECTOR_COUNT:
				/*	Returns number of available sectors on the drive into the DWORD variable pointed by Buffer.
				 * 	This command is used by only f_mkfs function to determine the volume size to be created.
				 */
				sdGetSizeInfo( &NumberOfBlocks, &BlockSize);
				*(DWORD*)Buffer = NumberOfBlocks;
				res = RES_OK;
				break;

			case GET_BLOCK_SIZE:
				/*	Returns erase block size of the flash memory in unit of sector into the
				 * 	DWORD variable pointed by Buffer. The allowable value is 1 to 32768 in power of 2.
				 * 	Return 1 if the erase block size is unknown or disk devices.
				 * 	This command is used by only f_mkfs function and it attempts to align data area
				 * 	to the erase block boundary.
				 */
				sdGetSizeInfo( &NumberOfBlocks, &BlockSize);
				*(DWORD*)Buffer = BlockSize;
				res = RES_OK;
				break;

			case CTRL_ERASE_SECTOR:
				/*	Erases a part of the flash memory specified by a DWORD array
				 * 	{<start sector>, <end sector>} pointed by Buffer.
				 * 	When this feature is not supported or not a flash memory media,
				 * 	this command has no effect.
				 * 	The FatFs does not check the result code and the file function is not affected even
				 * 	if the sectors are not erased well.
				 * 	This command is called on removing a cluster chain when _USE_ERASE is 1.
				 */
				res = RES_PARERR;
				break;
			default:
				/* RES_NOTRDY
				 * The disk drive has not been initialized
				 */
				return(RES_PARERR);
			}
			return res;
			break;
		case 0:
			dfSPIInit();
			switch(Command){
			case CTRL_SYNC:
				/*	Make sure that the disk drive has finished pending write process.
				 * 	When the disk I/O module has a write back cache, flush the dirty sector immediately.
				 * 	This command is not used in read-only configuration.
				 */
				//if (wait_ready() == 0xFF)
					res = RES_OK;
				break;
			case GET_SECTOR_SIZE:
				/*	Returns sector size of the drive into the WORD variable pointed by Buffer.
				 * 	This command is not used in fixed sector size configuration, _MAX_SS is 512.
				 */
				*(WORD*)Buffer = 512;
				res = RES_OK;
				break;

			case GET_SECTOR_COUNT:
				/*	Returns number of available sectors on the drive into the DWORD variable pointed by Buffer.
				 * 	This command is used by only f_mkfs function to determine the volume size to be created.
				 */
				//dfGetSizeInfo( &NumberOfBlocks, &BlockSize);
				*(DWORD*)Buffer = 8192;		//tyle stron (sektorow) ma DataFlash w urzadzeniu
				res = RES_OK;
				break;

			case GET_BLOCK_SIZE:
				/*	Returns erase block size of the flash memory in unit of sector into the
				 * 	DWORD variable pointed by Buffer. The allowable value is 1 to 32768 in power of 2.
				 * 	Return 1 if the erase block size is unknown or disk devices.
				 * 	This command is used by only f_mkfs function and it attempts to align data area
				 * 	to the erase block boundary.
				 */
				//dfGetSizeInfo( &NumberOfBlocks, &BlockSize);
				*(DWORD*)Buffer = 1;
				res = RES_OK;
				break;

			case CTRL_ERASE_SECTOR:
				/*	Erases a part of the flash memory specified by a DWORD array
				 * 	{<start sector>, <end sector>} pointed by Buffer.
				 * 	When this feature is not supported or not a flash memory media,
				 * 	this command has no effect.
				 * 	The FatFs does not check the result code and the file function is not affected even
				 * 	if the sectors are not erased well.
				 * 	This command is called on removing a cluster chain when _USE_ERASE is 1.
				 */
				res = RES_PARERR;
				break;
			default:
				/* RES_NOTRDY
				 * The disk drive has not been initialized
				 */
				return(RES_PARERR);
			}
			return res;
			break;
		default:
			return(RES_PARERR);
		}
}



#if _FS_READONLY !=1


//I have not RTC in bootloader module. If you have that, You can just return time and date formated like below:
DWORD get_fattime(void)	/* 31-25: Year(0-127 +1980), 24-21: Month(1-12), 20-16: Day(1-31) */
							/* 15-11: Hour(0-23), 10-5: Minute(0-59), 4-0: Second(0-29 *2) */
{
	u32 temp; u32 fattime = 0;

	return fattime;	//...here
}
#endif


