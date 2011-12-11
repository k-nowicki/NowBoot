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
 * delay.c
 *
 *  Created on: 02-10-2010
 *      Author: Karol Nowicki
 *
 *  Proste opóznienia (nie sa zbyt dok³adne)
 *  chyba, ze uzywamy FreeRTOSa i przelaczymy uzywane funkcje, jak w opisach
 */


#include "delay.h"

void stop_on(u32 ms){
	volatile unsigned long cnt, cnta;
	for(cnt=0; cnt < (3000); cnt++) {
		for(cnta=0; cnta < ms; cnta++);
	}
}



void wait_ms(int ms){
	stop_on(ms);//If You use FreeRTOS, just replace this in the following:
	//vTaskDelay(ms/portTICK_RATE_MS);
}

void wait_s(int s){
	unsigned int ms;
	ms = s*1000;

	wait_ms(s);//If You use FreeRTOS, just replace this in the following:
	//vTaskDelay(ms/portTICK_RATE_MS);
}

