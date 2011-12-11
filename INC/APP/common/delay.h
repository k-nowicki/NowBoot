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
 * delay.h
 *
 *  Created on: 02-10-2010
 *      Author: Karol Nowicki
 *
 *  Proste opóŸnienia (nie s¹ zbyt dok³adne)
 *  chyba, ze uzywamy FreeRTOSa i przelaczymy uzywane funkcje, jak w opisach
 */

#ifndef DELAY_H_
#define DELAY_H_

#include "common/common.h"


//czeka w milisekundach, ale nie zatrzymuje innych procesow
void wait_ms(int ms);

//czeka w sekundach, ale nie zatrzymuje innych procesow
void wait_s(int s);

//zastyga na ms. nie zatrzymuje tasku
void stop_on(u32 ms);

#endif /* DELAY_H_ */
