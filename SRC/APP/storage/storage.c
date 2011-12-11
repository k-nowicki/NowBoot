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
 * storage.c
 *
 *  Created on: 16-10-2010
 *      Author: Karol Nowicki
 *
 *  Plik wysokopoziomej obslugi pamieci masowych
 *
 */

#include "storage/storage.h"

//globalne dla tego pliku
static	FATFS SDFatFs, DFFatFs;	/* File system object for SD Card and for DataFlash*/
static	FIL fileR,fileW, Logfile;		/* File object */
static	FRESULT res;         	/* FatFs function common result code */
static	UINT br, bw, Logb;         	/* File read/write count */
static	FILINFO fileInfo;		// file/dir info object
static	BYTE fsbuffer[1024];   	/* file r/w buffer */
static char resTextBuffer[RES_TEXT_BUFFER_SIZE]; //buffer for result text description




//inicjalizacja karty SD i systemu fat na karcie
u08 StorageSdInit(void){
	//inicjalizacja karty
	if(sdCardInit()==OK){
		/* Enable FatFs module */
		memset(&SDFatFs, 0, sizeof(FATFS));
		FATFS* SDFat = &SDFatFs;
		debug_msg("Montuje system plikow...");
		res = f_mount ( 1, SDFat );
		fsDispFRESULT( res, resTextBuffer);
		if(res == FR_OK){
			debug_msg("SD Card initialized OK!");
			return FR_OK;
		}
		else{
			debug_msg("No Filesystem!");
			//heere must mount file system on the card
			//and try again
			return FR_NO_FILESYSTEM;
		}
	}
	else{
		debug_msg("Card Error!");
		//here can try again and if three probes fail,
		//then below
		return FR_DISK_ERR;
	}
}

//wysyla na debug opis podany jako desc, a nastepnie
//zawartosc bufora w przystepnej formie
void debug_dpack(u08 *desc, u08 *buff){
	debug_msg( desc );
	debug_array( &(buff[1]) , buff[0] );
	}


//Formatowamie DataFlasha
u08 StorageDfFormat(void){
	dfSPIInit();
	debug_msg("Formatuje pamiec DataFlash... ");
	res = f_mkfs( 	0,		/* Logical drive number */
					0, 		/* Partitioning rule */
					8*512	/* Size of the allocation unit */
				);
	if(res ==FR_OK){
		debug_msg("Udalo sie !!! ");
	}
	else{
		debug_msg("Dupa.");
		fsDispFRESULT( res, resTextBuffer);
	}
	return res;
}



//inicjalizacja DataFlasha i systemu fat na nim
u08 StorageDfInit(void){
	//inicjalizacja karty
	if(dfSPIInit()==OK){
		/* Enable FatFs module */
		memset(&DFFatFs, 0, sizeof(FATFS));
		FATFS* DFFat = &DFFatFs;;
		debug_msg("Montuje system plikow...");
		res = f_mount ( 0, DFFat );
		fsDispFRESULT( res, resTextBuffer);
		if(res == FR_OK){
			debug_msg("DataFlash initialized OK!");
			return FR_OK;
		}
		else{
			debug_msg("No Filesystem!");
			//heere must mount file system on the card
			//and try again
			return FR_NO_FILESYSTEM;
		}
	}
	else{
		debug_msg("DataFlash Error!");
		//here can try again and if three probes fail,
		//then below
		while(1) wait_s(120);	//never ending loop
		return FR_DISK_ERR;
	}
}
