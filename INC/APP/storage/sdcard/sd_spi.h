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
 * sd_spi.h
 *
 *  Created on: 02-10-2010
 *      Author: Karol Nowicki
 *
 *	Sterownik karty SD w trybie SPI- plik nag³ówkowy
 *	Posk³adany z ró¿nych fragmentow OpenSource dostepnych w sieci
 */



#ifndef __SD_SPI_H__
#define __SD_SPI_H__

#include "common/common.h"

void swSPIsd(void);
uint8_t sdCardInit(void);
uint8_t sdGetSizeInfo(uint32_t *NumberOfBlocks, uint16_t *BlockSize);
uint8_t sdReadBlock(uint32_t BlockAddress, uint8_t *Buffer);
uint8_t sdWriteBlock(uint32_t BlockAddress, uint8_t *Buffer);
uint8_t sdReadBlocks(uint32_t BlockAddress, uint8_t blocks, uint8_t *Buffer);	//just a wrapper to sdReadBlock
uint8_t sdWriteBlocks(uint32_t BlockAddress, uint8_t blocks, uint8_t *Buffer);	//just a wrapper to sdWriteBlock



#endif



