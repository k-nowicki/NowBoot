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
 * spi.c
 *
 *  Created on: 02-10-2010
 *      Author: Karol Nowicki
 *
 *	Standardowa implementacja sterownika SPI dla SAM7, rozszerzona
 *	o multipleksowanie urz¹dzen SPI
 */


#include "storage/spi/spi.h"



/*
 * Mikrosekundowe odpóznienia wymagane tylko tutaj
 * (System nie zapewnia tak krotkich opoznien, przelaczanie trwa dluzej)
 */
static inline void delay_us_core(void);

void delay_us(uint16_t us){
	uint16_t i;
	for(i=0;i<us;i++) delay_us_core();
}

static inline void delay_us_core(void){
	uint32_t i;
	//2 ticks for branches, and rest for 48Mhz
	for(i=0;i<46;i++) asm volatile("nop");
}


//=================================================================================================
//Configuration area
//=================================================================================================

    #define SPI                 AT91C_BASE_SPI0
    #define SPI_ID              AT91C_ID_SPI0
    #define SPI_PIO             AT91C_BASE_PIOA
    #define SPI_PIO_PERIPH      0 //zero for Peripheral A, 1 for Peripheral B
    #define MOSI_MASK           AT91C_PA17_SPI0_MOSI
    #define MISO_MASK           AT91C_PA16_SPI0_MISO
    #define SPCK_MASK            AT91C_PA18_SPI0_SPCK
	#define NPCS_PIO			AT91C_BASE_PIOA
	
//=================================================================================================


#define SPI_PINS_MASK           (MOSI_MASK|MISO_MASK|SPCK_MASK)

const uint32_t NPCS_CODE_MASK[4] = { 0<<16, 1<<16, 3<<16, 7<<16 };


void spiInit(void){
    //set PIO pins to be controlled by SPI
    SPI_PIO->PIO_PDR = SPI_PINS_MASK;
	
    #if SPI_PIO_PERIPH == 0
        SPI_PIO->PIO_ASR = SPI_PINS_MASK;
    #else
        SPI_PIO->PIO_BSR = SPI_PINS_MASK;
    #endif
    
    //enable SPI clock
    AT91C_BASE_PMC->PMC_PCER = ( 1 << SPI_ID );
	delay_us(1);
    //spi enable and reset
    SPI->SPI_CR = AT91C_SPI_SWRST | AT91C_SPI_SPIEN;

	//delay between chip select
	const uint32_t SPI_MR_DLBCS = (0x10 << 24);

    //SPI Mode Register: Fault detection disable, variable periph. chip select
    SPI->SPI_MR  = AT91C_SPI_MSTR | AT91C_SPI_MODFDIS | AT91C_SPI_PS | SPI_MR_DLBCS;

    //enable SPI
    SPI->SPI_CR = AT91C_SPI_SPIEN;

}

void spiFast(void){
	s_pSpi->SPI_CSR[1]  = 0x01010402;           //8bit, CPOL=0, ClockPhase=1, SCLK = 48Mhz/32*31 = 48kHz
}

void dfspiFast(void){
	s_pSpi->SPI_CSR[0]  = 0x01010402;           //8bit, CPOL=0, ClockPhase=1, SCLK = 48Mhz/32*31 = 48kHz
}

void spiDmaTransfer(void *buffer, uint16_t len, uint8_t channel){
	//make a copy of SPI_MR
	uint32_t SPI_MR_backup = SPI->SPI_MR;
	
	//make a copy of SPI_CSR[0]
	uint32_t SPI_CSR0_backup = SPI->SPI_CSR[0];
	
	//clear PCS field and variable peripheral selection
	SPI->SPI_MR &= ~(AT91C_SPI_PCS | AT91C_SPI_PS);
	
	//activate desired channel
	SPI->SPI_MR |= NPCS_CODE_MASK[channel];
	
	//clear BITS field in CSR0 register
	SPI->SPI_CSR[0] &= ~AT91C_SPI_BITS;
	
	//copy BITS field from channel's CSR to CSR0
	SPI->SPI_CSR[0] |= SPI->SPI_CSR[channel] & AT91C_SPI_BITS;
	
	//prepare DMA transfer
	SPI->SPI_TPR = (uint32_t)buffer;
	SPI->SPI_TNCR = 0;
	SPI->SPI_TCR = len;
	
	//start DMA transfer
	SPI->SPI_PTCR = AT91C_PDC_TXTEN;
	
	//wait for end of transfer
	while( !( SPI->SPI_SR & AT91C_SPI_TXEMPTY ) );
	
	//restore previous SPI_MR state
	SPI->SPI_MR  = SPI_MR_backup;
	
	//restore previous SPI_CSR0 state
	SPI->SPI_CSR[0] = SPI_CSR0_backup;
	
	//dummy read clears RDRF flag
	volatile uint8_t dummy;
	dummy = SPI->SPI_RDR;
}



uint16_t spiTransfer(uint16_t dataToSend, uint8_t channel){
	uint16_t receivedData;
	
	while( !( SPI->SPI_SR & AT91C_SPI_TDRE ) );
	SPI->SPI_TDR = NPCS_CODE_MASK[channel] | dataToSend;
	
	while( !( SPI->SPI_SR & AT91C_SPI_RDRF ) );
	receivedData = (uint16_t)( SPI->SPI_RDR );
	
	return(receivedData);
}

