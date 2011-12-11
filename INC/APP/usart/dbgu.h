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
 * dbgu.h
 *
 *  Created on: 21-10-2010
 *      Author: Karol Nowicki
 *
 * 	Sterownika portu dbgu	-plik naglowkowy
 *  posk³adany z najró¿niejszych czêsci:
 *  	najnizsza obsluga dbgu- Ÿród³a Atmela+ ewentualne w³asne modyfikacje
 *  	reszta dopisana lub posk³adana z gotowców openSource
 *
 *  Wsparcie w³asciwie tylko w stronê wysylki- do odbioru jest jedynie _inkey() i _waitkey()
 */

#ifndef __DBGU_H__
#define __DBGU_H__
#include <inttypes.h>

#define _LOGGING_ON_

void debug_init_default(void);
void debug_init(AT91PS_DBGU paramDbgu, AT91PS_PIO paramPio, uint32_t pinMask, uint8_t periph);
void debug_chr(char chr);
char debug_inkey(void);
char debug_waitkey(void);
void debug_ascii(uint8_t b);
void debug_msg(char *str);
void debug_txt(char *str);
void debug_txt_limit(char *str, uint8_t len);
void debug8_t(uint8_t val);
void debug16_t (uint16_t val);
void debug32_t (uint32_t val);
void debug_value( const char* s, uint32_t value);
void debug_array( void *array, uint16_t size );

//displays dec number from val in format 99
void debug8_tdec(uint8_t val);
//displays dec number from val in format 9999999999
void debug32_tdec(int val);

#endif


