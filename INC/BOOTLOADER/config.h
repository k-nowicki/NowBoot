//------------------------------------------------------------------------------
// File:          config.h
// Function:      Bootloader configuration
// Supported chip(s):
//    - AT91SAM7XC128
//    - AT91SAM7XC256
// Supported toolchain(s):
//    - IAR Embedded Workbench
// Date created:  09 May 2006
// Created by:    JJo
//------------------------------------------------------------------------------
// Notes:
//
// The config.h file defines the configuration of the bootloader, namely;
//  - communication protocol used
//  - media driver used
//  - debug driver used (if any)
//  - memory type
//  - trigger condition
//  - timing measurement peripheral (if any)
//  - optional features (such as firmware encryption)
//
// To modify this file, simply comment/uncomment the desired lines. For example,
// if you want to select the DBGU as the debug driver, then uncomment all lines
// under the Debug category except #define USE_DBGU.
// In some cases, there are parameters to adjust, e.g. the baudrate of the DBGU
// and USART line. Note that you do not need to uncomment parameters when the
// option is not selected, but you may do so.
//------------------------------------------------------------------------------

#ifndef BOOTLOADER_CONFIG_H
#define BOOTLOADER_CONFIG_H

//------------------------------------------------------------------------------
// Modes
//------------------------------------------------------------------------------

/**
 * Communication protocol
 *  -> Xon/Xoff (USE_XON_XOFF)
 */
#define USE_XON_XOFF

/**
 * Media layer
 *  -> Usart #0 (USE_US0)
 */
#define USE_US0
    #define US0_BAUDRATE   115200

/**
 * Debug
 *  -> Dbgu (USE_DBGU)
 */
#define USE_DBGU
    #define DBGU_BAUDRATE  115200

/**
 * Memory type
 *  -> Flash (USE_FLASH)
 */
#define USE_FLASH

/**
 * Trigger condition
 *  -> Dummy/Always load (USE_DUMMY)
 *  -> Hardware switches (USE_SWITCHES)
 */
#define USE_DUMMY
//#define USE_SWITCHES

/**
 * Timing measurement
 *  -> Timer #0 (USE_TIMER0)
 */
#define USE_TIMER0

//------------------------------------------------------------------------------
// Options
//------------------------------------------------------------------------------

/**
 * Name:    USE_BOOT_REGION_LOCKING
 * Effect:  Locks the boot region, denying write access on it.
 *          The boot segment must occupate a whole region, or it will
 *          not be possible to correctly program the application.
 *           => BOOTLOADEREND multiple of 0x4000
 */
#define USE_BOOT_REGION_LOCKING

/**
 * Name:    USE_ENCRYPTION
 * Effect:  The firmware must be transmitted in an encrypted form when this
 *          option is selected.
 * Parameters:
 *  - Algorithm used for encryption
 *      -> AES (ENCRYPTION_AES_LTC, ENCRYPTION_AES_REF)
 *  - Mode of encryption
 *      -> CTR (ENCRYPTION_CTR)
 *  - Length of key in bytes (ENCRYPTION_KEY_LENGTH)
 *  - Key, string representation of its hexadecimal value (ENCRYPTION_KEY)
 *  - Length of block in bytes (ENCRYPTION_BLOCK_LENGTH)
 *  - Initialization vector (ENCRYPTION_IV)
 */
/*#define USE_ENCRYPTION
    #define ENCRYPTION_AES_HARD
    #define ENCRYPTION_ECB
    #define ENCRYPTION_KEY_LENGTH    16
    #define ENCRYPTION_KEY           "C0DEC0DEC0DEC0DEC0DEC0DEC0DEC0DE"
    #define ENCRYPTION_BLOCK_LENGTH  16
    #define ENCRYPTION_IV            "C0DEC0DEC0DEC0DEC0DEC0DEC0DEC0DE"*/

/**
 * Name:    USE_MEMORY_PARTIONING
 * Effect:  When active, the application space in flash is divided into two
 *          regions. One act as a buffer to receive a firmware upgrade, while
 *          the other contains the code which will be executed. This makes it
 *          possible to have, at all time, at least one working firmware in the
 *          device (even after an upgrade failure).
 * Parameters:
 *          Size of one region (REGION_SIZE)
 *            -> Note that REGION_SIZE should be equal to
 *               (flash size - bootloader size - page size) / 2
 *            -> One page is reserved to store information about regions
 *               -> Signatures, length of firmware, etc.
 *            -> Ex with SAM7XC256, BOOTSIZE = 0x8000
 *               -> (0x40000 - 0x8000 - 0x100) / 2 = 0x1BF00
 *            -> Regions must be aligned on page and multiple of MEMORY_PAGE_SIZE
 */
/*#define USE_MEMORY_PARTITIONING
    #define REGION_SIZE     0x1BF00*/

#endif // BOOTLOADER_CONFIG_H
