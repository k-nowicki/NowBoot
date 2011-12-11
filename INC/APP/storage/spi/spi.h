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
 * spi.h
 *
 *  Created on: 02-10-2010
 *      Author: Karol Nowicki
 *
 *	Standardowa implementacja sterownika SPI dla SAM7, rozszerzona
 *	o multipleksowanie urz¹dzen SPI- plik naglowkowy
 */


#ifndef __SPI_H__
#define __SPI_H__

#include "common/common.h"

//NCPHA/NCPOL settings
#define SPI_MODE_0				2
#define SPI_MODE_1				0
#define SPI_MODE_2				3
#define SPI_MODE_3				1

/*
Mod by karol 2010
*/

#define  s_pSpi  AT91C_BASE_SPI0
#define  s_pPio  AT91C_BASE_PIOA
#define  s_pPMC  AT91C_BASE_PMC
#define  s_pPDC  AT91C_BASE_PDC_SPI0


#define   BIT0        0x00000001
#define   BIT1        0x00000002
#define   BIT2        0x00000004
#define   BIT3        0x00000008
#define   BIT4        0x00000010
#define   BIT5        0x00000020
#define   BIT6        0x00000040
#define   BIT7        0x00000080
#define   BIT8        0x00000100
#define   BIT9        0x00000200
#define   BIT10       0x00000400
#define   BIT11       0x00000800
#define   BIT12       0x00001000
#define   BIT13       0x00002000
#define   BIT14       0x00004000
#define   BIT15       0x00008000
#define   BIT16       0x00010000
#define   BIT17       0x00020000
#define   BIT18       0x00040000
#define   BIT19       0x00080000
#define   BIT20       0x00100000
#define   BIT21       0x00200000
#define   BIT22       0x00400000
#define   BIT23       0x00800000
#define   BIT24       0x01000000
#define   BIT25       0x02000000
#define   BIT26       0x04000000
#define   BIT27       0x08000000
#define   BIT28       0x10000000
#define   BIT29       0x20000000
#define   BIT30       0x40000000
#define   BIT31       0x80000000

/*
eof Mod by karol 2010
*/

void spiFast(void);
void spiInit(void);
void spiDmaTransfer(void *buffer, uint16_t len, uint8_t channel);
uint16_t spiTransfer(uint16_t dataToSend, uint8_t channel);


#endif



