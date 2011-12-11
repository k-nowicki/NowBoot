//------------------------------------------------------------------------------
// File:          flash.h
// Function:      Flash function definitions
// Supported chip(s):
//    - AT91SAM7XC128
//    - AT91SAM7XC256
// Supported toolchain(s):
//    - IAR Embedded Workbench
// Date created:  02 May 2006
// Created by:    JJo
//------------------------------------------------------------------------------

#ifndef BOOTLOADER_FLASH_H
#define BOOTLOADER_FLASH_H

//------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------

#include "common/common.h"
//#include "board.h"
#include "usart/dbgu.h"

#define USE_FLASH

#ifdef USE_FLASH

//------------------------------------------------------------------------------
// Definitions
//------------------------------------------------------------------------------

// Errors (unsigned ints)
#define FLASH_OK          0x00000000
#define FLASH_ERROR       0xFFFFFFFF



// Functions
#define memory_init            flash_init
#define memory_cleanup         flash_cleanup
#define memory_write           flash_write
#define memory_next            flash_next
#define memory_lock            flash_lock
#define memory_unlock          flash_unlock

#define FLASH_TEST_ADDRESS     ((void *) 0x00125000)
#define MEMORY_START_ADDRESS   AT91C_IFLASH
#define MEMORY_SIZE            AT91C_IFLASH_SIZE
#define MEMORY_PAGE_SIZE       AT91C_IFLASH_PAGE_SIZE
#define MEMORY_PAGE_SIZE_32    (MEMORY_PAGE_SIZE / 4)

// Region size in pages
#define MEMORY_REGION_SIZE     64
#define MEMORY_REGION_MASK     0xFFFFFFC0

//------------------------------------------------------------------------------
// Prototypes
//------------------------------------------------------------------------------

void flash_init(void)__attribute__((section(".ram_func")));
void flash_cleanup(void)__attribute__((section(".ram_func")));
unsigned int flash_write(void *, void *)__attribute__((section(".ram_func")));
void * flash_next(void *)__attribute__((section(".ram_func")));
unsigned int flash_lock(void *, void *)__attribute__((section(".ram_func")));
unsigned int flash_unlock(void *, void *)__attribute__((section(".ram_func")));

#endif // USE_FLASH
#endif // BOOTLOADER_FLASH_H
