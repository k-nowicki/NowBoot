/*

/-----------------------------------------------------------------------------/
/ NowBoot is a Bootloader module for small embedded systems which
/ working with FreeRTOS.
/ With few changes in boot.s containing exception vectors
/ it can be use with any other firmware.
/ C 2010, Karol Nowicki
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
 * main.c
 *
 *  Created on: 02-10-2010
 *      Author: Karol Nowicki
 *
 *  Program NowBoot V1.0
 *  karol@knowicki.pl
 *
 *  Compiller: ARM-GCC
 *
 *  Bootloader dla procesorów SAM7 firmy Atmel.
 *  Program ładowany jest z karty SD podłaczonej do SPI_1
 *  Moze byc takze ladowany z pamieci DataFlash, podlaczonej do SPI_0
 *  W dokumentacji znajdziesz znacznie wiecej informacji.
 *
 */



/* Standard includes. */
#include <stdlib.h>


/* Application includes. */
#include "common/common.h"

//#include "partest.h"
#include "usart/dbgu.h"
#include "storage/storage.h"

#include "flash.h"


//Local functions
	void prvSetupHardware( void );
	void vPortSoftReset( void );
	int StorageInit(void);


//	Global datas

//files
	//firmware and other
	FIL firmwarefile;		// File object
	FRESULT res;
    FRESULT res;         /* FatFs function common result code */
    UINT br, bw;         /* File read/write count */

	//logging
	FIL Logfile;		// File object
	UINT Logb;
	u08 CardInclude =0;
	u08 logging_buf[1562];	 //buffer for log text message

// Variables for flash write
  u08 page[MEMORY_PAGE_SIZE];
  u32 bytes;
  void * vStart = (void *)0x00108000;
  u08 * pCurrent;


	//Used as a loop counter to create a very crude delay.
    #define mainDELAY_LOOP_COUNT		( 0xffffff )


/*-----------------------------------------------------------*/

// Main

int main( void )
{
	volatile unsigned long ul;

	// Initialize address and notify host
  	pCurrent = (u08*)vStart;
  	u32 kopycounter=0;

	prvSetupHardware( );

    // Version Controll
    debug_msg( "NowBoot V1.2; Compiled at: "__DATE__" "__TIME__"\r");
	wait_ms(1000);
    debug_msg("NowBoot: Running drives and filesystem.");
    StorageInit();
	wait_ms(100);
    debug_msg("NowBoot: Checking for new firmware...");
    //f_rename ( "1:fmw/fmw_n.old", "fmw/fmw_n.bin" );	//For tests only
	res = f_open(&firmwarefile, "1:fmw/fmw_n.bin", FA_OPEN_EXISTING | FA_READ);
	if(res != FR_OK){
		f_close(&firmwarefile);
    	debug_msg("NowBoot: Can`t find new firmware.");
        debug_msg("NowBoot: Checking for in-flash firmware");
        if((*((u08*)vStart+4)==0xFF)&&(*((u08*)(vStart+5))==0xff)){
        	debug_msg("NowBoot: Flash is empty!");
        	debug_msg("NowBoot: Opening default firmware!");
        	res = f_open(&firmwarefile, "1:fmw/fmw_0.bin", FA_OPEN_EXISTING | FA_READ);
        	if(res != FR_OK){
        		f_close(&firmwarefile);
            	debug_msg("NowBoot: Can`t find any firmware file... Nothing to do.");
            	for(;;)AT91C_BASE_SYS->WDTC_WDCR = 0xA5000001;	//endless loop
        	}
        	goto fmw_prog;
        }
        else{
        	debug_msg("NowBoot: Flash contain firmware.");
        	goto app_now;
        }
	}else{
    	debug_msg("NowBoot: Found new firmware.");
    	fmw_prog:
    	debug_msg("NowBoot: Start programming...");
		wait_ms(300);
		debug_txt("\rCopying page: ");
		for (;;) {
			debug_txt("\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\bKopiuje strone: "); debug32_tdec(kopycounter++);
        	res = f_read(&firmwarefile, page, sizeof(page), &br);    /* Read a chunk of src file */
        	if (res || br == 0) {
        		wait_ms(100);
        		debug_msg("NowBoot: Copying finshed.");
        		break; /* error or eof */
        	}
        	//memset(page,0xff,sizeof(page));		//for tests only
        	flash_write(pCurrent , (void *) page );
        	pCurrent+= MEMORY_PAGE_SIZE;
    	}

	}
	f_close(&firmwarefile);

/*
    TO DO:
		MD5 verification
*/


    wait_ms(300);
    debug_msg("NowBoot: Firmware upgrade successful.");
    debug_msg("NowBoot: Removing firmware file.");
    f_unlink ( "1:fmw/fmw_n.old" );	//removeing old firmware
    f_rename ( "1:fmw/fmw_n.bin", "fmw/fmw_n.old" );

app_now:
	wait_ms(100);
    debug_msg("NowBoot: End of Boot.");
    spiInit();
    wait_ms(1000);
    AT91C_BASE_SYS->WDTC_WDCR = 0xA5000001;	//WDT restart
    asm volatile(
    	"LDR pc, =0x00108000"		//Jump to firmware start
    );
    debug_msg("NowBoot: Something bad happened! I should never be here.");
    wait_ms(100);
	vPortSoftReset();

  	return 0;
}


/*-----------------------------------------------------------*/
void prvSetupHardware( void )
{
	//portDISABLE_INTERRUPTS();
	/* When using the JTAG debugger the hardware is not always initialised to
	the correct default state.  This line just ensures that this does not
	cause all interrupts to be masked at the start. */
	AT91C_BASE_AIC->AIC_EOICR = 0;
	/* Most setup is performed by the low level init function called from the
	startup asm file. */
	/* Enable the peripheral clock. */
	OUT1_OFF; OUT2_OFF; OUT3_OFF; OUT4_OFF;
    AT91C_BASE_PMC->PMC_PCER = 1 << AT91C_ID_PIOA;
    AT91C_BASE_PMC->PMC_PCER = 1 << AT91C_ID_PIOB;
	//AT91C_BASE_PMC->PMC_PCER = 1 << AT91C_ID_EMAC;
	/* Initialise the LED outputs for use by the demo application tasks. */
    //IOS_INIT;
    debug_init_default();
}


/*-----------------------------------------------------------*/
// WDT powered Reset
void vPortSoftReset( void ){
	unsigned int Mode = AT91C_WDTC_WDRSTEN| 1<<15 |1;
	AT91C_BASE_SYS->WDTC_WDMR = Mode;
	while (AT91C_BASE_RSTC->RSTC_RSR & AT91C_RSTC_SRCMP);
	(*(void(*)())0)();
}
/*-----------------------------------------------------------*/


//Disks initialization
int StorageInit(void){
	FRESULT res;         	 // FatFs function common result code
	u08 fr_res_buf[50];	 	 //buffer for error text message
	u08* frame = fr_res_buf; //buffer for received frame (same as above)
	u08 logname[30];

	wait_ms(1);
	spiInit();
	wait_s(1);
	//SD initializing
	res = StorageSdInit();
	if(res !=FR_OK){
		debug_msg("Storage: SD Card Error!");
		//here can try again and if three probes fail,
		//then below
		fsDispFRESULT(res, fr_res_buf);
		//while(1) wait_s(120);	//never ending loop
	}else{
		debug_msg("Storage: SD Card Initialized");
	}
}

