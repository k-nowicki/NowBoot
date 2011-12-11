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
 * storage.h
 *
 *  Created on: 16-10-2010
 *      Author: Karol Nowicki
 *
 *  Plik naglowkowy wysokopoziomej obslugi pamieci masowych
 *
 */

#ifndef STORAGE_H_
#define STORAGE_H_

#include "storage/fatfs/ff.h"
#include "common/common.h"

#define SD 0x01
#define DF 0x00

#define SecureDigital 	SD
#define DataFlash 		DF
#define SdCard 	SD
#define DataF	DF
#define Card 	SD
#define Df 		DF


//Formatowamie DataFlasha
u08 StorageDfFormat(void);
//inicjalizacja DataFlasha i systemu fat na nim
u08 StorageDfInit(void);

//inicjalizacja karty SD i systemu fat na karcie
u08 StorageSdInit(void);


#endif /* STORAGE_H_ */
