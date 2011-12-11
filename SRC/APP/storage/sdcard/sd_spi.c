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
 * sd_spi.c
 *
 *  Created on: 02-10-2010
 *      Author: Karol Nowicki
 *
 *	Sterownik karty SD w trybie SPI
 *	Posk³adany z ró¿nych fragmentow OpenSource dostepnych w sieci
 */


#include "common.h"
#include "storage/spi/spi.h"
#include "storage/sdcard/sd_spi.h"

//chip select settings
#define SD_SPI				AT91C_BASE_SPI0
#define SD_NPCS_PIO_PIN     13              //pin number
#define SD_NPCS_PIO         AT91C_BASE_PIOA //pointer to pio controller
#define SD_NPCS_PIO_PERIPH  0               //zero for peripheral A, 1 for peripheral B
#define SD_NPCS_NUMBER      1               //number of NPCS register (valid 0...3)
#define SD_NPCS_SELECT      0               //PCSDEC field, direct connection to CS pin of SD card


#define SD_NPCS_PIN_MASK    (1<<SD_NPCS_PIO_PIN)

#define SPI_MAX_SPEED        2

#define	SPI_CMD_READ		17
#define	SPI_CMD_WRITE		24
#define	SPI_CMD_READ_CSD   	9




//local function declarations
uint8_t  sdSPIInit(void);
inline void AT91F_PIO_SelectPeripheral(AT91PS_PIO pPio, uint32_t Mask, uint8_t PeriphSelect);
void 	 sdSPISetSpeed(uint8_t speed);
uint8_t	 sdResponse8_t(void);
uint16_t sdResponse16_t(void);
uint8_t  sdSPIDataTransfer(uint8_t dat);
uint8_t  sdCommand(uint8_t cmd, uint16_t paramx, uint16_t paramy);


void swSPIsd(void){
	spiFast();
	sdSPISetSpeed(0x02);//spiFast();
}

uint8_t sdCardInit(void){
	swSPIsd();
	int16_t i;
	uint8_t sdResponse;
    
    sdSPIInit();

    i=100;
	do {
		sdCommand(0, 0, 0);
		sdResponse=sdResponse8_t();
	}
	while(sdResponse!=1 && i--);
	
	if(sdResponse!=1){
		if(sdResponse==0xff){
			return(0);
		}
		else{
			return(0);
		}
	}
	
	i=32000;
	do{
		sdCommand(1, 0, 0);
		sdResponse=sdResponse8_t();
	}
	while(sdResponse==1 && i--);
	if(sdResponse!=0){
		return(0);
	}
	return(1);
}



uint8_t sdSPIInit(void){
    //NPCS is still controlled by PIO:
    SD_NPCS_PIO->PIO_PER=SD_NPCS_PIN_MASK;
    SD_NPCS_PIO->PIO_SODR=SD_NPCS_PIN_MASK;
    SD_NPCS_PIO->PIO_OER=SD_NPCS_PIN_MASK;
    
    //SD SPI Settings
    SD_SPI->SPI_CSR[SD_NPCS_NUMBER] = AT91C_SPI_CPOL | AT91C_SPI_BITS_8;
    //set low speed for init
    sdSPISetSpeed(0xFE);
    
    //send 20 spi commands for init
    uint8_t i;
	for(i=0;i<21;i++){
		sdSPIDataTransfer(0xFF);
	}
    
    //from now on SD NPCS pin may be controlled by SPI harware
    #if SD_NPCS_PIO_PERIPH==0
		SD_NPCS_PIO->PIO_ASR |= SD_NPCS_PIN_MASK;
	#else
		SD_NPCS_PIO->PIO_BSR = SD_NPCS_PIN_MASK;
	#endif
    SD_NPCS_PIO->PIO_PDR = SD_NPCS_PIN_MASK;
    sdSPISetSpeed(0x02);
    
    return(1);
}




inline void AT91F_PIO_SelectPeripheral(AT91PS_PIO pPio, uint32_t Mask, uint8_t PeriphSelect){

    if(PeriphSelect==0){
        pPio->PIO_ASR=Mask;
    }
    else{
        pPio->PIO_BSR=Mask;
    }
}



void sdSPISetSpeed(uint8_t speed){
	uint32_t reg;

	if ( speed < SPI_MAX_SPEED ) speed = SPI_MAX_SPEED;
	if ( speed > 1 ) speed &= 0xFE;

	reg = SD_SPI->SPI_CSR[SD_NPCS_NUMBER];                          //copy
	reg = ( reg & ~(AT91C_SPI_SCBR) ) | ( (uint32_t)speed << 8 );   //modify
	SD_SPI->SPI_CSR[SD_NPCS_NUMBER] = reg;                          //write modified
}


uint8_t sdSPIDataTransfer(uint8_t dat){
	return(spiTransfer(dat,SD_NPCS_NUMBER));
}


uint8_t sdCommand(uint8_t cmd, uint16_t paramx, uint16_t paramy){
	sdSPIDataTransfer(0xff);
	sdSPIDataTransfer(0x40 | cmd);
	sdSPIDataTransfer((uint8_t) (paramx >> 8));
	sdSPIDataTransfer((uint8_t) (paramx));
	sdSPIDataTransfer((uint8_t) (paramy >> 8));
	sdSPIDataTransfer((uint8_t) (paramy));
	sdSPIDataTransfer(0x95);
	sdSPIDataTransfer(0xff);
    return(1);
}


uint8_t sdResponse8_t(void){
	uint8_t i;
	uint8_t sdResponse;
	
	for ( i=0; i<8; i++ ) {
		sdResponse = sdSPIDataTransfer(0xff);
		if(sdResponse != 0xff)
			return(sdResponse);
	}
		
	return(sdResponse);
}


uint16_t sdResponse16_t(void){
	uint16_t sdResponse;
	
	sdResponse = ( sdResponse8_t() << 8 ) & 0xff00;
	sdResponse |= sdSPIDataTransfer(0xff);
	
	return(sdResponse);
}


uint8_t sdGetSizeInfo(uint32_t *NumberOfBlocks, uint16_t *BlockSize)
{
    
	uint8_t sdCardResponse, i, temp;
	uint8_t contents[16];	//CSD contents
	uint16_t size, sizeMultiplier;
	
	sdCommand(SPI_CMD_READ_CSD, 0, 0);
	
	do {
		sdCardResponse =sdResponse8_t();
	} while ( sdCardResponse != 0xFE );
	
	for( i=0; i<16; i++){
		contents[i] = sdSPIDataTransfer(0xff);
	}
	
	sdSPIDataTransfer(0xff);
	sdSPIDataTransfer(0xff);
	
	size = contents[6] & 0x03;
	size <<= 10;
	size |= (uint16_t)contents[7]<<2;
	size |= contents[8]>>6;
	
	temp=contents[9] & 0x03;
	temp <<= 1;
	temp |= contents[10] >> 7;
	
	sizeMultiplier = 1;
	sizeMultiplier <<= (2+temp);
	
	uint16_t tempBlockSize;
	tempBlockSize = 1 << (contents[5] & 0x0F);
	
	//copy to output
    *BlockSize=tempBlockSize;
    *NumberOfBlocks=sizeMultiplier*(size+1);
    
    return(1);
}


uint8_t sdReadBlock(uint32_t BlockAddress, uint8_t *Buffer){
	
	uint8_t sdCardResponse;
	uint8_t firstblock;
	uint16_t timeout=0xffff;
	uint32_t i;
	uint32_t byteAddress;
	
	byteAddress=512*BlockAddress;
	sdCommand(SPI_CMD_READ, (uint16_t) (byteAddress >> 16), (uint16_t) byteAddress);
	
	sdCardResponse=sdResponse8_t();
	
	do
		firstblock=sdResponse8_t(); 
	while(firstblock==0xff && timeout--);
	
	if(sdCardResponse!=0x00 || firstblock!=0xfe){
		return(0);
	}
	
	for(i=0;i<512;i++){
        Buffer[i] = sdSPIDataTransfer(0xff);;
	}
	
	//ignore checksum
	sdSPIDataTransfer(0xff);
	sdSPIDataTransfer(0xff);

	return(1);

}


uint8_t sdWriteBlock(uint32_t BlockAddress, uint8_t *Buffer){

	uint32_t byteAddress;
	uint16_t i;
	
	byteAddress=512*BlockAddress;
	sdCommand(SPI_CMD_WRITE, (uint16_t) (byteAddress >> 16), (uint16_t) byteAddress);
	
	sdResponse8_t();
	
	sdSPIDataTransfer(0xfe);
	for(i=0;i<512;i++){
        sdSPIDataTransfer(Buffer[i]);
    }
	sdSPIDataTransfer(0xff);	//checksum
	sdSPIDataTransfer(0xff);	//checksum
	sdSPIDataTransfer(0xff);
	while(sdSPIDataTransfer(0xff)!=0xff);	//wait for response
	
	return(1);
}



uint8_t sdReadBlocks(uint32_t BlockAddress, uint8_t blocks, uint8_t *Buffer){

	uint8_t i;
	for(i=0;i<blocks;i++){
		sdReadBlock(BlockAddress+i,Buffer);
		Buffer+=512;
	}
	return(OK);
}


uint8_t sdWriteBlocks(uint32_t BlockAddress, uint8_t blocks, uint8_t *Buffer){

	uint8_t i;
	for(i=0;i<blocks;i++){

		sdWriteBlock(BlockAddress+i,Buffer);
		Buffer+=512;
	}
	return(OK);
}


