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
 * df_spi.h
 *
 *  Created on: 21-10-2010
 *      Author: Karol Nowicki
 *
 * 	Plik ng³ówkowy sterownika pamiêci DataFlash dla sam7
 *  Bazowany na df_spi.h firmy Atmel
 *
 */

#ifndef DF_SPI_H_
#define DF_SPI_H_


/* Standard includes. */
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <inttypes.h>



#include "common/conv.h"


/**************************************************
 * 		Zapis jednego bajtu do SPI DataFlasha
 */
uint8_t dfSPIDataTransfer(uint8_t dat);


/**************************************************
 * 		Ustawienie predkosci transmisji
 */
void dfSPISetSpeed(uint8_t speed);


/**************************************************
 * 	Inicjalizacja komunikacji z DataFlashem
 */
uint8_t dfSPIInit(void);


/**************************************************
 * 	Przepisanie strony Z bufora flasha do flasha -uzywane przez dfWriteBlock
 * 	-(Nr strony flash)
 */
u16	sflash_wr_page(u32 BlockAddress);

/**************************************************
 * 	Przepisanie strony Z flasha do bufora flasha -uzywane przez dfReadBlock
 * 	-(Nr strony flash)
 */
u16	sflash_rd_page(u32 BlockAddress);

/**************************************************
 * 	Zapis bloku do flasha (tak naprawde to strony)-( / addr flash/ bufor danych/)
 */
u08 dfWriteBlock(u32 BlockAddress, u08 *Buffer);

/**************************************************
 * 	Odczyt bloku Z flasha (tak naprawde to strony)-( / addr flash/ bufor danych/)
 */
//odczyt bloku Z flasha-(bufor wyjsciowy/addr flash start/ ilosc bajtow)
u08	dfReadBlock(u32 BlockAddress, u08 *Buffer);



/**************************************************
 * 	Odczyt bloków Z flasha (tak naprawde to stron)-( / addr flash/ ilosc blokow/ bufor danych/)
 */
uint8_t dfReadBlocks(uint32_t BlockAddress, uint8_t blocks, uint8_t *Buffer);

/**************************************************
 * 	Zapis bloków do flasha (tak naprawde to stron)-( / addr flash/ ilosc blokow/ bufor danych/)
 *
 */
uint8_t dfWriteBlocks(uint32_t BlockAddress, uint8_t blocks, uint8_t *Buffer);




#endif /* DF_SPI_H_ */
