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
 * df_spi.c
 *
 *  Created on: 21-10-2010
 *      Author: Karol Nowicki
 *
 * 	Plik sterownika pamiêci DataFlash dla sam7
 *  Bazowany na df_spi.c firmy Atmel
 *
 */



#include "common/common.h"
#include "storage/spi/spi.h"
#include "storage/dataflash/df_spi.h"

void dfspiFast(void);

//chip select settings
#define DF_SPI				AT91C_BASE_SPI0
#define DF_NPCS_PIO_PIN     12              //pin number
#define DF_NPCS_PIO         AT91C_BASE_PIOA //pointer to pio controller
#define DF_NPCS_PIO_PERIPH  0               //zero for peripheral A, 1 for peripheral B
#define DF_NPCS_NUMBER      0               //number of NPCS register (valid 0...3)
#define DF_NPCS_SELECT      0               //PCSDEC field, direct connection to CS pin of DF card
#define DF_NPCS_PIN_MASK    (1 <<DF_NPCS_PIO_PIN)

#define SPI_MAX_SPEED        2


/**************************************************
 * 		Zapis jednego bajtu do SPI DataFlasha
 */
uint8_t dfSPIDataTransfer(uint8_t dat){
	return(spiTransfer(dat,DF_NPCS_NUMBER));
}


/**************************************************
 * 		Ustawienie predkosci transmisji
 */
void dfSPISetSpeed(uint8_t speed)
{
	uint32_t reg;

	if ( speed < SPI_MAX_SPEED ) speed = SPI_MAX_SPEED;
	if ( speed > 1 ) speed &= 0xFE;

	reg = DF_SPI->SPI_CSR[DF_NPCS_NUMBER];                          //copy
	reg = ( reg & ~(AT91C_SPI_SCBR) ) | ( (uint32_t)speed << 8 );   //modify
	DF_SPI->SPI_CSR[DF_NPCS_NUMBER] = reg;                          //write modified
}

/**************************************************
 * 		Inicjalizacja komunikacji z DataFlashem
 */
uint8_t dfSPIInit(void)
{
    //NPCS is still controlled by PIO:
    DF_NPCS_PIO->PIO_PER=  DF_NPCS_PIN_MASK;
    DF_NPCS_PIO->PIO_SODR= DF_NPCS_PIN_MASK;
    DF_NPCS_PIO->PIO_OER=  DF_NPCS_PIN_MASK;

    //DF SPI Settings
    DF_SPI->SPI_CSR[DF_NPCS_NUMBER] =   AT91C_SPI_CPOL | AT91C_SPI_BITS_8;
    //set low speed for init
    dfSPISetSpeed(0x16);

    //send 20 spi commands for init
    uint8_t i;
	for(i=0;i<21;i++){
		dfSPIDataTransfer(0xFF);
	}

    //from now on DF NPCS pin may be controlled by SPI harware
    #if DF_NPCS_PIO_PERIPH==0
	//	DF_NPCS_PIO->PIO_ASR |= DF_NPCS_PIN_MASK;
	#else
		DF_NPCS_PIO->PIO_BSR = DF_NPCS_PIN_MASK;
	#endif
	//	DF_NPCS_PIO->PIO_PDR = DF_NPCS_PIN_MASK;	//nie wylaczamy, sterujemy recznie, DF tego wymaga (paranoja)

    dfSPISetSpeed(0x02);

    return(1);
}




/**************************************************
 * 	Przepisanie strony Z bufora flasha do flasha -uzywane przez dfWriteBlock
 * 	-(Nr strony flash)
 */
u16	sflash_wr_page(u32 BlockAddress){
	u32 faddr = BlockAddress <<3;		//przesun bitowo o 3 adres, bo taki trzeba wyslac

	DF_NPCS_PIO->PIO_CODR= DF_NPCS_PIN_MASK;
	wait_ms(5);								//poczekaj az przepisze
	dfSPIDataTransfer(0x83);			//rozkaz zapisz bufor do flash
	dfSPIDataTransfer((faddr&0xff00)>>8);	//wyslij starsza czesc adresu
	dfSPIDataTransfer(faddr&0x00FF);		//wyslij mlodsza czesc adresu
	dfSPIDataTransfer(0xFF);				//i reszte adresu ktorej niema wyslij
	DF_NPCS_PIO->PIO_SODR= DF_NPCS_PIN_MASK;
	wait_ms(15);								//poczekaj az przepisze
	return 0; 			//zwroc 0
}

/**************************************************
 * 	Przepisanie strony Z flasha do bufora flasha -uzywane przez dfReadBlock
 * 	-(Nr strony flash)
 */
u16	sflash_rd_page(u32 BlockAddress){
	u32 faddr = BlockAddress << 3;		//przesun bitowo o 3 adres, bo taki trzeba wyslac;

	//wait_ms(1);								//poczekaj na poprzednie operacje
	DF_NPCS_PIO->PIO_CODR= DF_NPCS_PIN_MASK;
	dfSPIDataTransfer(0x53);				//rozkaz zapisz bufor danymi z flash
	dfSPIDataTransfer((faddr&0xff00)>>8);	//wyslij starsza czesc adresu
	dfSPIDataTransfer(faddr&0x00FF);		//wyslij mlodsza czesc adresu
	dfSPIDataTransfer(0xFF);				//i reszte adresu ktorej niema wyslij
	DF_NPCS_PIO->PIO_SODR= DF_NPCS_PIN_MASK;
	wait_ms(7);								//poczekaj az przepisze
	return 0; //zwroc 0
}

/**************************************************
 * 	Zapis bloku do flasha (tak naprawde to strony)-( / addr flash/ bufor danych/)
 */
u08 dfWriteBlock(u32 BlockAddress, u08 *Buffer){
	u16 dat_ind=0;

	DF_NPCS_PIO->PIO_CODR= DF_NPCS_PIN_MASK;
	dfSPIDataTransfer(0x84);
	dfSPIDataTransfer(0xFF);				//najstarsza czesc adresu zawsze 0xff
	dfSPIDataTransfer(0x00);		//starsza czesc adresu w buforze strony -zawsze 0
	dfSPIDataTransfer(0x00);		//mlodsza czesc adresu w buforze strony -zawsze 0
	for(dat_ind=0; dat_ind<512; dat_ind++){
		dfSPIDataTransfer(Buffer[dat_ind]);
	}
	DF_NPCS_PIO->PIO_SODR= DF_NPCS_PIN_MASK;
	return (u08)sflash_wr_page(BlockAddress);
}

/**************************************************
 * 	Odczyt bloku Z flasha (tak naprawde to strony)-( / addr flash/ bufor danych/)
 */
//odczyt bloku Z flasha-(bufor wyjsciowy/addr flash start/ ilosc bajtow)
u08	dfReadBlock(u32 BlockAddress, u08 *Buffer){
	u16 dat_ind=0;
	u08 result =0;

	result= (u08)sflash_rd_page( BlockAddress);
	if(result)
		return result;	//zwroc blad
	DF_NPCS_PIO->PIO_CODR= DF_NPCS_PIN_MASK;
	dfSPIDataTransfer(0xD4);// (0x9F);//
	dfSPIDataTransfer(0xFF);				//najstarsza czesc adresu zawsze 0xff
	dfSPIDataTransfer(0x00);		//starsza czesc adresu w buforze strony -zawsze 0
	dfSPIDataTransfer(0x00);		//mlodsza czesc adresu w buforze strony -zawsze 0
	dfSPIDataTransfer(0xFF);
	for(dat_ind=0; dat_ind<512; dat_ind++){
		Buffer[dat_ind]= dfSPIDataTransfer(0xFF);	//odbierz bajt
	}
	DF_NPCS_PIO->PIO_SODR= DF_NPCS_PIN_MASK;
	return result;	//zwroc 0
}



/**************************************************
 * 	Odczyt bloków Z flasha (tak naprawde to stron)-( / addr flash/ ilosc blokow/ bufor danych/)
 */
uint8_t dfReadBlocks(uint32_t BlockAddress, uint8_t blocks, uint8_t *Buffer){
	uint8_t i;

	for(i=0;i<blocks;i++){
		dfReadBlock(BlockAddress+i,Buffer);
		Buffer+=512;
	}
	return(OK);
}

/**************************************************
 * 	Zapis bloków do flasha (tak naprawde to stron)-( / addr flash/ ilosc blokow/ bufor danych/)
 *
 */
uint8_t dfWriteBlocks(uint32_t BlockAddress, uint8_t blocks, uint8_t *Buffer){
	uint8_t i;

	for(i=0;i<blocks;i++){
		dfWriteBlock(BlockAddress+i,Buffer);
		Buffer+=512;
	}
	return(OK);
}




