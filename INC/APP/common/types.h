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
 * types.h
 *
 *  Created on: 02-10-2010
 *      Author: Karol Nowicki
 *
 *  definicje typow danych uzywanych w projekcie
 *
 */

#ifndef TYPES_H_
#define TYPES_H_



typedef unsigned char      u08;	///< Typ 8 bitowy bez znaku
typedef          char      s08;	///< Typ 8 bitowy ze znakiem
typedef unsigned short     u16;	///< Typ 16 bitowy bez znaku
typedef          short     s16;	///< Typ 16 bitowy ze znakiem
typedef unsigned long      u32;	///< Typ 32 bitowy bez znaku
typedef          long      s32;	///< Typ 32 bitowy ze znakiem
typedef unsigned long long u64;	///< Typ 64 bitowy bez znaku
typedef          long long s64;	///< Typ 64 bitowy ze znakiem

// ---------------------------------------------------------------

typedef unsigned char      UCHAR;	///< Typ 8 bitowy bez znaku
typedef unsigned short     WORD;	///< Typ 16 bitowy bez znaku
typedef unsigned long      DWORD;	///< Typ 32 bitowy bez znaku
typedef char*		     LPCTSTR;	///< WskaŸnik do ³añcucha znaków
// ---------------------------------------------------------------



/// Typ przechowuj¹cy czas
typedef struct
{
  u08 second;   ///< Sekunda
  u08 minute;   ///< Minuta
  u08 hour;     ///< Godzina
  u08 day;	///< Dzieñ miesi¹ca
  u08 month;    ///< Miesi¹c
  u16 year;     ///< Rok
} struct_time;



#endif /* TYPES_H_ */
