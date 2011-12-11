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
 * conv.h
 *
 *  Created on: 02-10-2010
 *      Author: Karol Nowicki
 *
 *	Funkcje konwertujace	-plik naglowkowy
 *	Zawartosc posk³adana z najró¿niejszych Ÿróde³ OpenSource
 *	+ w³asne modyfikacje i fragmenty
 *
 */

#ifndef __CONV_H__
#define __CONV_H__

#include "common/types.h"



/*@{*/

char* itoa(int value, char* buffer, int base);

/**
 Zamiana kodu BCD na binarny
 bity BCD: 7654 - dziesiatki, 3210 -jednosci
 \param bcd wartoœæ w kodzie BCD
 \return kod binarny z kodu BCD
 \see uint8_t bin2bcd(uint8_t bin);
*/
uint8_t bcd2bin(uint8_t bcd);

/**
 Zamiana kodu binarnego na BCD
 \param bin wartoœæ binarna z zakresu 0-99
 \return kod BCD, bity: 7654 - dziesiatki, 3210 -jednosci
 \see uint8_t bcd2bin(uint8_t bcd);
*/
uint8_t bin2bcd(uint8_t bin);

/**
 Zamiana wartoœci binarnej na znak ASCII w kodzie szesnastkowym
 \param val wartoœæ binarna z zakresu 0-15
 \return znak ASCII w kodzie szesnastkowym 0-F
*/
char nibble2hex(uint8_t val);

/**
 Zamiana wartoœci binarnej na tekst ASCII w kodzie szesnastkowym
 \param val wartoœæ binarna z zakresu 0-255
 \param *s wskaznik do zwracanego ci¹gu ASCII w kodzie szesnastkowym 00-FF
*/
void byte2hex(uint8_t val, char *s);

/**
 Zamiana wartoœci binarnej na tekst ASCII w kodzie szesnastkowym
 \param val wartoœæ binarna z zakresu 0-65535
 \param *s wskaznik do zwracanego ci¹gu ASCII w kodzie szesnastkowym 0000-FFFF
*/
void word2hex(u16 val, char *s);


/**
 Zamiana wartoœci binarnej na tekst ASCII w kodzie dziesiêtnym
 \param val wartoœæ binarna z zakresu 0-99
 \param *s wskaznik do zwracanego ci¹gu ASCII w kodzie dziesiêtnym 0-99
*/
void byte2dec(uint8_t val, char *s);


//przetworzenie stringu ascii z liczba hex, na liczbe int
int hatoi(char *);

//przetworzenie stringu ascii z liczba hex (do 0xff), na liczbe byte
uint8_t hatobyte(char *s);

/*@}*/

#endif // __CONV_H__
