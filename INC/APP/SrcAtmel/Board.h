/*----------------------------------------------------------------------------
*         ATMEL Microcontroller Software Support  -  ROUSSET  -
*----------------------------------------------------------------------------
* The software is delivered "AS IS" without warranty or condition of any
* kind, either express, implied or statutory. This includes without
* limitation any warranty or condition with respect to merchantability or
* fitness for any particular purpose, or against the infringements of
* intellectual property rights of others.
*----------------------------------------------------------------------------
* File Name           : Board.h
* Object              : AT91SAM7X Evaluation Board Features Definition File.
*
* Creation            : JG   20/Jun/2005
*----------------------------------------------------------------------------
*/
#ifndef Board_h
#define Board_h

//do uarta {
//#include "include/at91sam7x256.h"
//#undef __inline
//#define __inline static inline
//#include "include/lib_at91sam7x256.h"
// } koniec

#include "AT91SAM7X256.h"

//#include "lib_at91sam7x256.h"
#define AT91SAM7XC256


#define true	-1
#define false	0

/*-------------------------------*/
/* SAM7Board Memories Definition */
/*-------------------------------*/
// The AT91SAM7X128 embeds a 32-Kbyte SRAM bank, and 128K-Byte Flash

#define  FLASH_PAGE_NB		256
#define  FLASH_PAGE_SIZE	128

/*-----------------*/
/* Leds Definition */
/*-----------------*/
#define LED1            (1<<19)	// PB19
#define LED2            (1<<20)	// PB20
#define LED3            (1<<21)	// PB21
#define LED4            (1<<22)	// PB22
#define NB_LED			4

#define LED_MASK        (LED1|LED2|LED3|LED4)


/*--------------*/
/* Master Clock */
/*--------------*/

#define EXT_OC          18432000   // Exetrnal ocilator MAINCK
#define MCK             47923200   // MCK (PLLRC div by 2)
#define MCKKHz          (MCK/1000) //

/**************************************************************
 * 		WYJSCIA
 *  Definicje makr obslugujacych wyjscia cyfrowe
 */
///	OUT  Port IO (taki sam dla wszystkich wyjsc uniwersalnych)
#define OUT_PIO		AT91C_BASE_PIOB
///	OUT 1 Maska wyjscia 1
#define OUT1_MASK		AT91C_PIO_PB16
///	OUT 1 Inicjalizacja wyjscia 1
#define OUT1_INIT		{OUT_PIO->PIO_PER = OUT1_MASK; OUT_PIO->PIO_OER = OUT1_MASK; OUT_PIO->PIO_OWER = OUT1_MASK;}
///	OUT 1 wylaczenie wyjscia 1
#define OUT1_OFF		OUT_PIO->PIO_SODR  = OUT1_MASK
///	OUT 1 wlaczenie wyjscia 1
#define OUT1_ON			OUT_PIO->PIO_CODR  = OUT1_MASK
///	OUT 1 zmiana stanu wyjscia 1
#define OUT1_TOGGLE		OUT_PIO->PIO_ODSR ^= OUT1_MASK
///	OUT 1 wlaczene pullupa na wyjsciu 1
#define OUT1_PPE		OUT_PIO->PIO_PPUER  = OUT1_MASK
///	OUT 1 wylaczene pullupa na wyjsciu 1
#define OUT1_PPD		OUT_PIO->PIO_PPUDR  = OUT1_MASK
///	OUT 1 odczyt stanu wyjscia 1
#define IS_OUT1  ( ( OUT_PIO -> PIO_PDSR & OUT1_MASK )>>16)

///	OUT 2 Maska wyjscia 2
#define OUT2_MASK		AT91C_PIO_PB17
///	OUT 2 Inicjalizacja wyjscia 2
#define OUT2_INIT		{OUT_PIO->PIO_PER = OUT2_MASK; OUT_PIO->PIO_OER = OUT2_MASK; OUT_PIO->PIO_OWER = OUT2_MASK;}
///	OUT 2 Wylaczenie wyjscia 2
#define OUT2_OFF		OUT_PIO->PIO_SODR  = OUT2_MASK
///	OUT 2 Wlaczenie wyjscia 2
#define OUT2_ON			OUT_PIO->PIO_CODR  = OUT2_MASK
///	OUT 2 Zmiana stanu wyjscia 2
#define OUT2_TOGGLE		OUT_PIO->PIO_ODSR ^= OUT2_MASK
///	OUT 2 wylaczene pullupa na wyjsciu 2
#define OUT2_PPE		OUT_PIO->PIO_PPUER  = OUT2_MASK
///	OUT 2 wlaczene pullupa na wyjsciu 2
#define OUT2_PPD		OUT_PIO->PIO_PPUDR  = OUT2_MASK
///	OUT 2 odczyt stanu wyjscia 2
#define IS_OUT2  ( ( OUT_PIO -> PIO_PDSR & OUT2_MASK )>>17)

///	OUT 3 Maska wyjscia 3. Pozostale definicje sa analogiczne do wyjsc 1 i 2
#define OUT3_MASK		AT91C_PIO_PB18
#define OUT3_INIT		{OUT_PIO->PIO_PER = OUT3_MASK; OUT_PIO->PIO_OER = OUT3_MASK; OUT_PIO->PIO_OWER = OUT3_MASK;}
#define OUT3_OFF		OUT_PIO->PIO_SODR  = OUT3_MASK
#define OUT3_ON			OUT_PIO->PIO_CODR  = OUT3_MASK
#define OUT3_TOGGLE		OUT_PIO->PIO_ODSR ^= OUT3_MASK
//pull_up
#define OUT3_PPE		OUT_PIO->PIO_PPUER  = OUT3_MASK
#define OUT3_PPD		OUT_PIO->PIO_PPUDR  = OUT3_MASK

//	OUT 4
#define OUT4_MASK		AT91C_PIO_PB19
#define OUT4_INIT		{OUT_PIO->PIO_PER = OUT4_MASK; OUT_PIO->PIO_OER = OUT4_MASK; OUT_PIO->PIO_OWER = OUT4_MASK;}
#define OUT4_OFF		OUT_PIO->PIO_SODR  = OUT4_MASK
#define OUT4_ON			OUT_PIO->PIO_CODR  = OUT4_MASK
#define OUT4_TOGGLE		OUT_PIO->PIO_ODSR ^= OUT4_MASK
//pull_up
#define OUT4_PPE		OUT_PIO->PIO_PPUER  = OUT4_MASK
#define OUT4_PPD		OUT_PIO->PIO_PPUDR  = OUT4_MASK

///Outs mask Maska dla wszystkich wyjsc
#define OUTS_MASK	(OUT1_MASK|OUT2_MASK|OUT3_MASK|OUT4_MASK)


/**************************************************************
 * 		WEJSCIA
 *  Definicje makr obslugujacych wejscia cyfrowe
 */

///	IN  Port IO (taki sam dla wszystkich wejsc uniwersalnych)
#define IN_PIO		AT91C_BASE_PIOB
///	DIN 1 Maska uniwersalnego wejscia 1
#define IN1_MASK		AT91C_PIO_PB0
///	DIN 1 Inicjalizacja uniwersalnego wejscia 1
#define IS_IN1  !( ( IN_PIO -> PIO_PDSR & IN1_MASK )>>0)
///	DIN 2 Maska uniwersalnego wejscia 2
#define IN2_MASK		AT91C_PIO_PB1
#define IS_IN2  !( ( IN_PIO -> PIO_PDSR & IN2_MASK )>>1)
///	DIN 2 Inicjalizacja uniwersalnego wejscia 2
#define IN3_MASK		AT91C_PIO_PB2
///	DIN 4 init (analogicznie do 1 i 2)
#define IS_IN3  !( ( IN_PIO -> PIO_PDSR & IN3_MASK )>>2)
///	DIN 4 mask (analogicznie do 1 i 2)
#define IN4_MASK		AT91C_PIO_PB3
#define IS_IN4  !( ( IN_PIO -> PIO_PDSR & IN4_MASK )>>3)
///	DIN 5 mask (analogicznie do 1 i 2)
#define IN5_MASK		AT91C_PIO_PB4
///	DIN 5 init (analogicznie do 1 i 2)
#define IS_IN5  !( ( IN_PIO -> PIO_PDSR & IN5_MASK )>>4)
///	DIN 6 mask (analogicznie do 1 i 2)
#define IN6_MASK		AT91C_PIO_PB5
///	DIN 6 init (analogicznie do 1 i 2)
#define IS_IN6  !( ( IN_PIO -> PIO_PDSR & IN6_MASK )>>5)

///	DIN _ALM mask (analogicznie do 1 i 2)
#define IN_ALM_MASK		AT91C_PIO_PB6
///	DIN _ALM Sprawdzenie stanu, zwraca TRUE jesli stan 0
#define IS_IN_ALM  !( ( IN_PIO -> PIO_PDSR & IN_ALM_MASK )>>6)

///	DIN _ALU mask (analogicznie do 1 i 2)
#define IN_ALU_MASK		AT91C_PIO_PB7
///	DIN _ALU Sprawdzenie stanu, zwraca TRUE jesli stan 0
#define IS_IN_ALU  !( ( IN_PIO -> PIO_PDSR & IN_ALU_MASK )>>7)

///	DIN _PPA (wszystkie dalsze definicje sa analogiczne do poprzednich )
#define IN_PPA_MASK		AT91C_PIO_PB8
#define IS_IN_PPA  !( ( IN_PIO -> PIO_PDSR & IN_PPA_MASK )>>8)

//	DIN _PNA
#define IN_PNA_MASK		AT91C_PIO_PB9
#define IS_IN_PNA  !( ( IN_PIO -> PIO_PDSR & IN_PNA_MASK )>>9)

//	DIN _AKU_MP
#define IN_AKU_MP_MASK		AT91C_PIO_PB10
#define IS_IN_AKU_MP  !( ( IN_PIO -> PIO_PDSR & IN_AKU_MP_MASK )>>10)

//	DIN _V_ST_P
#define IN_V_ST_P_MASK		AT91C_PIO_PB11
#define IS_IN_V_ST_P  !( ( IN_PIO -> PIO_PDSR & IN_V_ST_P_MASK )>>11)

///INs mask Maski wszystkich wejsc zewnetrznych
#define INS_MASK	(IN1_MASK | IN2_MASK | IN3_MASK | IN4_MASK | IN5_MASK | IN6_MASK |\
					IN_ALM_MASK | IN_ALU_MASK | IN_PPA_MASK | IN_PNA_MASK | IN_AKU_MP_MASK |\
					IN_V_ST_P_MASK )

/* *************************************************************
 * 		WEJSCIA i WYJSCIA
 *  Definicje makr obslugujacych wszystkie i/o cyfrowe
 */

///Port wszystkich wejsc/wyjsc
#define IOS_PIO		AT91C_BASE_PIOB
/// maski wszystkich wejsc/wyjsc
#define IOS_MASK	(OUTS_MASK|INS_MASK)

///Inicjalizacja wszystkich IO
#define IOS_INIT	{IOS_PIO->PIO_PER = IOS_MASK; IOS_PIO->PIO_OER = OUTS_MASK; IOS_PIO->PIO_OWER = OUTS_MASK;}


#endif /* Board_h */
