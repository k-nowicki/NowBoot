/*

/-----------------------------------------------------------------------------/
/ NowBoot module is a Bootloader module for small embedded systems which
/ working with FreeRTOS.
/ With few changes in boot.s containing exception vectors
/ it can be use with any other firmware.
/  2010, Karol Nowicki
/
/ NowBoot contain some code which is provided by third-parties (like:
/ 	-FatFs
/	-Atmel peripherial drivers
/	-FreeRTOS (boot.s sample startup file) )
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
 * conv.c
 *
 *  Created on: 02-10-2010
 *      Author: Karol Nowicki
 *
 *	Funkcje konwertujace
 *	Zawartosc posk³adana z najró¿niejszych zróde³ OpenSource
 *	+ w³asne modyfikacje i fragmenty
 *
 */


#include <stdint.h>
#include "conv.h"

static void __reverse(char* begin,char* end);

// itoa over std (shortly)
char* itoa(int value, char* buf, int base)
{
	static const char digits[]="0123456789ABCDEF";

	char* buf_cp = buf;
	int32_t sign = 0;
	int32_t quot, rem;

	if ((base>=2)&&(base<=16)){
		if (base == 10 && (sign = value) < 0)
			value = -value;

		do{
			quot= value/base;
			rem= value%base;
			*buf++ = digits[rem];
		} while ( (value=quot) );	//while not 0 (this is not comparision)

		if (sign<0) // append the sign if negative
			*buf++= '-';

		__reverse( buf_cp, (buf-1) );		// reverse the string
	}

	*buf='\0';
	return buf_cp;
}


//for itoa
static void __reverse(char* begin,char* end)
{
	char temp;

	while (end>begin)
	{
		temp=*end;
		*end--=*begin;
		*begin++=temp;
	}
}


// Other functions

u08 bcd2bin(u08 bcd)                    
{
#ifdef OPTIMIZE_SPEED
  return (10*(bcd>>4)|(bcd&0x0f));
#else
  u08 Temp = bcd & 0x0F; 
  while (bcd>=0x10) 
  { 
    Temp += 10; 
    bcd -= 0x10; 
  } 
  return Temp; 
#endif
}

u08 bin2bcd(u08 bin)                    
{
#ifdef OPTIMIZE_SPEED
  return (((bin/10)<<4)|(bin%10));
#else
  u08 Temp = 0; 
  while(bin>9) 
  { 
    Temp += 0x10; 
    bin-=10; 
  } 
  return Temp+bin; 
#endif
}

void byte2dec(u08 val, char *s)  
{
  if(val>99){
	  u08 temp = (val/100);
	  s[0]='0'+ temp;
	  s[1]='0'+ (( val- temp*100 ) /10 );
	  s[2]='0'+(val%10);
	  s[3]=0;
  }
  else{
	  s[0]='0'+(val/10);
  	  s[1]='0'+(val%10);
  	  s[2]=0;
  }
}


void byte2hex(u08 val, char *s)  
{
  s[0]=nibble2hex(val>>4);
  s[1]=nibble2hex(val);
  s[2]=0;
}


char nibble2hex(u08 val)  
{
  char s;
  s='0'+(val&0xf);
  if (s>'9') s+='A'-'9'-1;
  return s;
}


void word2dec(u16 val, char *s)  
{
  s[0]='0'+(val/10000);
  s[1]='0'+(val%1000);
  s[2]='0'+(val%100);
  s[3]='0'+(val%10);
  s[4]=0;
}


void word2hex(u16 val, char *s)  
{
  s[0]=nibble2hex(val>>12);
  s[1]=nibble2hex(val>>8);
  s[2]=nibble2hex(val>>4);
  s[3]=nibble2hex(val);
  s[4]=0;
}

int hatoi(char *s)
	{
	unsigned long out=0;
	u08 ind;
	for(ind=0;out<30000;ind++){
		out<<=4;
		if((s[ind]>='0')&&(s[ind]<='9'))
			out+=(s[ind])^('0');
		else if((s[ind]>='A')&&(s[ind]<='F'))
			out+=(s[ind]-55);
		else if((s[ind]>='a')&&(s[ind]<='f'))
			out+=(s[ind]-87);
		else break;
	}
	out>>=4;
	return out;
	}


u08 hatobyte(char *s)
	{
	unsigned int out=0;
	u08 ind;
	for(ind=0;ind<3;ind++){
		out<<=4;
		if((s[ind]>='0')&&(s[ind]<='9'))
			out+=(s[ind])^('0');
		else if((s[ind]>='A')&&(s[ind]<='F'))
			out+=(s[ind]-55);
		else if((s[ind]>='a')&&(s[ind]<='f'))
			out+=(s[ind]-87);
		else break;
	
	}
	out>>=4;
	return 0x00ff& out;
	}
