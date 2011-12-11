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
 * dbgu.c
 *
 *  Created on: 21-10-2010
 *      Author: Karol Nowicki
 *
 * 	Sterownika portu dbgu
 *  posk³adany z najró¿niejszych czêsci:
 *  	najnizsza obsluga dbgu- ¼ród³a Atmela+ ewentualne w³asne modyfikacje
 *  	reszta dopisana lub posk³adana z gotowców openSource
 *
 *  Wsparcie w³asciwie tylko w stronê wysylki- do odbioru jest jedynie _inkey() i _waitkey()
 *
 */

#include <string.h>
#include "Board.h"
#include "dbgu.h"


//#include "lib_AT91SAM7X256.h"
//*----------------------------------------------------------------------------
//* \fn    AT91F_US_Baudrate
//* \brief Caluculate baud_value according to the main clock and the baud rate
//*----------------------------------------------------------------------------
__inline unsigned int AT91F_US_Baudrate (
	const unsigned int main_clock, // \arg peripheral clock
	const unsigned int baud_rate)  // \arg UART baudrate
{
	unsigned int baud_value = ((main_clock*10)/(baud_rate * 16));
	if ((baud_value % 10) >= 5)
		baud_value = (baud_value / 10) + 1;
	else
		baud_value /= 10;
	return baud_value;
}



#if defined AT91SAM7X128 || defined AT91SAM7X256 || defined AT91SAM7XC256 || defined AT91SAM7XC128
	#define DBGU_PIO				AT91C_BASE_PIOA
	#define DBGU_RX_MASK			AT91C_PA27_DRXD
	#define DBGU_TX_MASK			AT91C_PA28_DTXD
#endif

#if defined AT91SAM7S64 || defined AT91SAM7S128 || defined AT91SAM7S256
	#define DBGU_PIO				AT91C_BASE_PIOA
	#define DBGU_RX_MASK			AT91C_PA9_DRXD
	#define DBGU_TX_MASK			AT91C_PA10_DTXD
#endif

#define DBGU_BAUDRATE			115200
AT91PS_DBGU 					pDbgu;

//private tools
static uint8_t debug_dec2hex(uint8_t val);

static uint8_t debug_hex2dec(uint8_t val);
static uint32_t *debug_base_address;


void debug_init_default(void){
	debug_init(AT91C_BASE_DBGU, DBGU_PIO, DBGU_RX_MASK | DBGU_TX_MASK, 0);
}

//initializes pDbgu port for simple UART operation on 115200 baud (platform dependent)
//requires:
//paramDbgu: base address of DBGU,
//paramPio: base address of PIO pins used for RX and TX signals
//pinMask: mask for RX and TX pins
//periph: zero for peripheral A, non-zero for peripheral B
void debug_init(AT91PS_DBGU paramDbgu, AT91PS_PIO paramPio, uint32_t pinMask, uint8_t periph){
	//copy base address of USART to the global variable pDbgu
	pDbgu = paramDbgu;
	//disable PIO on pDbgu pins
	paramPio->PIO_PDR = pinMask;
	if(periph==0){
		//select peripheral A on pDbgu pins
		paramPio->PIO_ASR = pinMask;
	}
	else{
		//select peripheral B on pDbgu pins
		paramPio->PIO_BSR = pinMask;
	}
	//reset and disable transmitter and receiver
	pDbgu->DBGU_CR = AT91C_US_RSTRX | AT91C_US_RSTTX | AT91C_US_RXDIS | AT91C_US_TXDIS;
	//set mode
	pDbgu->DBGU_MR = AT91C_US_PAR_NONE | AT91C_US_CHRL_8_BITS;
	//set baud rate
	pDbgu->DBGU_BRGR = AT91F_US_Baudrate(MCK,DBGU_BAUDRATE);
	//enable transmitter and receiver
	pDbgu->DBGU_CR = AT91C_US_RXEN | AT91C_US_TXEN;
}



//send chr via pDbgu UART (platform dependent)
void debug_chr(char chr){
	while ( !(pDbgu->DBGU_CSR & AT91C_US_TXRDY) );	//Wait for empty TX buffer
	pDbgu->DBGU_THR = chr;								//Transmit char
}


//returns ascii value of last char received
//returns 0 if no char was received since last debug_inkey call
//(platform dependent)
char debug_inkey(void){
	if( pDbgu->DBGU_CSR & AT91C_US_RXRDY )
		return( pDbgu->DBGU_RHR );
	return(0);
}



//===================================================
//platform independent funcs



//prints text starting at str
//adds new line first
void debug_msg(char *str){
	char *astr= str;
	debug_chr('\r');
	debug_chr('\n');
	debug_txt(astr);
}

//prints text starting at str
void debug_txt(char *str){
	while(*str) debug_chr(*str++);
}


//prints text starting at str
//prints exactly len chars
void debug_txt_limit(char *str, uint8_t len){
	while(len){
		debug_ascii(*str);
		str++;
		len--;
	}
}



//halts program/task execution until char is received
char debug_waitkey(void){
	char chr;
	do{
		chr = debug_inkey();
	}while(chr==0);
	return(chr);
}


//sends char b over pDbgu UART. Replaces values that can change cursor pos. on terminal
void debug_ascii(uint8_t b){
	switch(b){
		case 0:{
			debug_chr('.');	//replace 0 with dot
			break;
		}
		case 8:
		case 9:
		case 10:
		case 13:{
			//avoid other chars that modify terminal cursor
			//replace them with space
			debug_chr(' ');
			break;
		}
		default:{
			debug_chr(b);
		}
	}//switch(b)
}

//displays hex number from val in format FF
void debug8_t(uint8_t val){
	debug_chr(debug_dec2hex(val>>4));
	debug_chr(debug_dec2hex(val&0x0F));
}

//displays dec number from val in format 99
void debug8_tdec(uint8_t val){
	debug32_tdec((int)val);
}


//displays dec number from val in format 0-9999999999
void debug32_tdec(int val){
	char s[12];
	itoa(val, s, 10);
	debug_txt(s);
}


//displays hex number from val in format FFFF
void debug16_t (uint16_t val){
	debug8_t(val>>(1*8));
	debug8_t(val>>(0*8));
}

//displays hex number from val in format FFFFFFFF
void debug32_t (uint32_t val){
	debug8_t(val>>(3*8));
	debug8_t(val>>(2*8));
	debug8_t(val>>(1*8));
	debug8_t(val>>(0*8));
}




//converts decimal digit from into ascii hex digit
//returns ascii char of hex digit
static uint8_t debug_dec2hex(uint8_t val){
	if(val<10) val+='0';
	else val=val-10+'A';
	return(val);
}

//converts hex digit in ASCII format (val) into integer (return)
static uint8_t debug_hex2dec(uint8_t val){
	if((val>='a')&&(val<='f'))
		return(val+10-'a');
	if((val>='A')&&(val<='F'))
		return(val+10-'A');
	return(val-'0');
}

//prints text from *s and value
void debug_value( const char* s, uint32_t value){
    debug_txt("\n\r");
    debug_txt((char*)s);
    debug_txt("0x");
    if(value>0x00FFFFFF) debug8_t(value>>(3*8));
    if(value>0x0000FFFF) debug8_t(value>>(2*8));
    if(value>0x000000FF) debug8_t(value>>(1*8));
    debug8_t(value>>(0*8));
}

//prints array contents byte-by-byte in ASCII hex format
//provide starting address in *array and number of items to display in size
void debug_array( void *array, uint16_t size ){
	uint8_t *s = array;
    debug_txt("\n\r");
	debug_txt("Length="); debug16_t(size); debug_txt(", Contents:\n\r");
	do{
		debug_chr(' '); debug8_t(*s++);
		size--;
	}while(size);
    debug_txt(" end");
}




void debug_dump(void *address, uint16_t len){
	uint8_t *buf = address;
	const uint16_t bytesInLine = 16;
	const uint16_t spaceBetweenDumpAndASCII = 4;
	uint16_t i, counter=len;
	
	//header
	debug_msg("Memory dump, starting address = "); debug32_t((uint32_t)buf); debug_txt(", length = "); debug16_t(len);
	debug_msg("Address  Offs:   Data");
	//contents
	while(1){
		//insert last line (may be shorter than full line)
		if(counter < bytesInLine){
			debug_txt("\r\n"); debug32_t((uint32_t)buf); debug_txt(" "); debug16_t(len-counter); debug_txt(":   ");
			
			//contents in hex
			for(i=0;i<bytesInLine;i++){
				if(i<counter){
					debug8_t(buf[i]);
					debug_chr(' ');
				}
				else{
					debug_txt("   ");
				}
				if(i%8==7) debug_chr(' ');
			}
			
			//space
			for(i=0;i<spaceBetweenDumpAndASCII;i++){
				debug_chr(' ');
			}
			
			//contents in ASCII
			for(i=0;i<bytesInLine;i++){
				if(i<counter){
					debug_ascii(buf[i]);
				}
				else{
					debug_chr(' ');
				}
			}
			break;
		}
		
		debug_txt("\r\n"); debug32_t((uint32_t)buf); debug_txt(" "); debug16_t(len-counter); debug_txt(":   ");
		
		for(i=0;i<bytesInLine;i++){
			debug8_t(buf[i]); debug_chr(' ');
			if(i%8==7) debug_chr(' ');
		}
		
		//space
		for(i=0;i<spaceBetweenDumpAndASCII;i++){
			debug_chr(' ');
		}
		
		//contents in ASCII
		for(i=0;i<bytesInLine;i++){
			debug_ascii(buf[i]);
		}
		
		buf += bytesInLine;
		if(counter >= bytesInLine){
			counter -= bytesInLine;
		}
		
		if(counter == 0) break;
		
	}
	debug_msg("End of dump");
}
