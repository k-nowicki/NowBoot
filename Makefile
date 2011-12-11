#/*
#/-----------------------------------------------------------------------------/
#/ NowBoot module is a Bootloader module for small embedded systems which
#/ working with FreeRTOS.
#/ With few changes in boot.s containing exception vectors
#/ it can be use with any other firmware.
#/  2010, Karol Nowicki 
#/
#/ NowBoot contain some code which is provided by third-parties like:
#/ 	-FatFs
#/	-Atmel peripherial drivers
#/	-FreeRTOS (sample boot.s startup file)
#/ AND IS LICENSED SEPARATELY FROM NowBoot.
#/
#/
#/
#/ This is a free software that opened for education, research and commercial
#/ developments under license policy of following terms.
#/
#/
#/ * The NowBoot module is a free software and there is NO WARRANTY.
#/ * No restriction on use. You can use, modify and redistribute it for
#/   personal, non-profit or commercial products UNDER YOUR RESPONSIBILITY.
#/ * Redistributions of source code must retain the above copyright notice.
#/
#/-----------------------------------------------------------------------------/
#		Some parts of this File were written by other authors.
#		These sections are marked with a comment containing the name of the author.
#
#
#*/

SOURCE_DIR= SRC
RTOS_SOURCE_DIR= $(SOURCE_DIR)/RTOS
APP_SOURCE_DIR= $(SOURCE_DIR)/APP
DEMO_COMMON_DIR= $(SOURCE_DIR)/APP/common
DEMO_INCLUDE_DIR= 
UIP_COMMON_DIR=

# List all default directories to look for include files here
HDIR=	  INC/ \
	      INC/RTOS/ \
	      INC/APP/ \
		  INC/APP/SrcAtmel/


#Use this for CodeSoursery
CC=arm-none-eabi-gcc
CP=arm-none-eabi-gcc
OBJCOPY=arm-none-eabi-objcopy
SIZE= arm-none-eabi-size

#Or this for gnu-gcc
#CC=arm-elf-gcc
#CP=arm-elf-gcc
#OBJCOPY=arm-elf-objcopy
#SIZE=arm-elf-size


LDSCRIPT= LNK/atmel-rom.ld

LINKER_FLAGS=-mthumb -nostartfiles -Xlinker -o$(TARGET_NAME).elf -Xlinker -M -Xlinker -Map=$(TARGET_NAME).map

DEBUG=-g
OPTIM=-Os

INCDIR  = $(patsubst %,-I%,$(HDIR))

CFLAGS= $(DEBUG) \
		$(OPTIM) \
		-T$(LDSCRIPT) \
		-I INC\
		-I INC/BOOTLOADER\
		-I INC/RTOS\
		-I INC/APP\
		-I INC/APP/SrcAtmel\
		-I INC/APP/common\
		-I INC/APP/leon\
		-I INC/APP/usart\
		-D SAM7_GCC \
		-D THUMB_INTERWORK \
		-mcpu=arm7tdmi \
		-D PACK_STRUCT_END=__attribute\(\(packed\)\) \
		-D ALIGN_STRUCT_END=__attribute\(\(aligned\(4\)\)\) \
		-fomit-frame-pointer \
		-mthumb-interwork \
		-fno-strict-aliasing   
#		-fno-dwarf2-cfi-asm

#		-I $(RTOS_SOURCE_DIR)/include \
#		-I $(RTOS_SOURCE_DIR)/portable/GCC/ARM7_AT91SAM7S \
#		-I ./SrcAtmel \


##############   BOOTLOADER   ###############
BOOTL_DIR = $(SOURCE_DIR)/BOOTLOADER
BOOTL_SRC = \
	$(BOOTL_DIR)/flash.c
############## FAT FILESYSTEM ###############
FAT_DIR = $(APP_SOURCE_DIR)/storage/fatfs
TH_FAT_SRC = \
	$(FAT_DIR)/ff.c \
	$(FAT_DIR)/ccsbcs.c \
	$(FAT_DIR)/diskio.c \
	$(FAT_DIR)/fs_tools.c


############## USER APP FILES ###############
#		$(TH_RTOS_SRC)\

THUMB_SOURCE= \
		$(BOOTL_SRC) \
		$(TH_FAT_SRC)\
		$(APP_SOURCE_DIR)/common/common.c\
		$(APP_SOURCE_DIR)/common/conv.c\
		$(APP_SOURCE_DIR)/common/delay.c\
		$(APP_SOURCE_DIR)/usart/dbgu.c\
		$(APP_SOURCE_DIR)/storage/spi/spi.c\
		$(APP_SOURCE_DIR)/storage/sdcard/sd_spi.c\
		$(APP_SOURCE_DIR)/storage/dataflash/df_spi.c\
		$(APP_SOURCE_DIR)/storage/storage.c\


ARM_SOURCE= \
		$(SOURCE_DIR)/main.c \
		$(APP_SOURCE_DIR)/SrcAtmel/Cstartup_SAM7.c \
		$(APP_SOURCE_DIR)/common/tc.c\


THUMBCPP_OBJS = $(THUMBCPP_SOURCE:.cpp=.o)
THUMB_OBJS = $(THUMB_SOURCE:.c=.o)
ARM_OBJS = $(ARM_SOURCE:.c=.o)

TARGET_NAME =Bootloader

all: $(TARGET_NAME).bin $(TARGET_NAME).lst print_size

$(TARGET_NAME).bin : $(TARGET_NAME).hex 
	$(OBJCOPY) $(TARGET_NAME).elf -O binary $(TARGET_NAME).bin
	
$(TARGET_NAME).hex : $(TARGET_NAME).elf
	$(OBJCOPY) $(TARGET_NAME).elf -O ihex $(TARGET_NAME).hex
	
$(TARGET_NAME).lst : $(TARGET_NAME).elf
	arm-none-eabi-objdump -h -S -C $(TARGET_NAME).elf > $(TARGET_NAME).lst 

#$(TARGET_NAME).o : $(TARGET_NAME).elf
#	$(OBJCOPY) $(TARGET_NAME).elf -O $(TARGET_NAME).o

$(TARGET_NAME).elf : $(THUMB_OBJS) $(ARM_OBJS) boot.s Makefile
	$(CP) $(CFLAGS) $(ARM_OBJS) $(THUMB_OBJS) $(THUMBCPP_OBJS) $(LIBS) boot.s $(LINKER_FLAGS)

$(THUMB_OBJS) : %.o : %.c 
	$(CC) -c $(CFLAGS) -mthumb  $< -o $@

$(THUMBCPP_OBJS) : %.o : %.cpp 
	$(CC) -c $(CFLAGS) -mthumb  $< -o $@

$(ARM_OBJS) : %.o : %.c 
	$(CC) -c $(CFLAGS) $< -o $@

clean :
	rm $(THUMB_OBJS)
	rm $(ARM_OBJS)
	touch Makefile
	rm $(TARGET_NAME).elf
	rm $(TARGET_NAME).bin
	rm $(TARGET_NAME).map
	rm $(TARGET_NAME).hex
	rm $(TARGET_NAME).lst

#-----------------------------------------------------------------------------#
# print the size of the objects and the .elf file
#-----------------------------------------------------------------------------#
print_size :
	arm-none-eabi-size -B -t --common $(THUMB_OBJS) $(ARM_OBJS)
	#arm-elf-size -B -t --common $(THUMB_OBJS) $(ARM_OBJS)
	
	$(SIZE) -B -t -d $(TARGET_NAME).elf
#	$(SIZE) -A $(TARGET_NAME).o

# **********************************************************************************************
#                            FLASH PROGRAMMING                                         
#
# Alternate make target for flash programming only
#
# You must create a special Eclipse make target (program) to run this part of the makefile 
# (Project -> Create Make Target...  then set the Target Name and Make Target to "program")
#
# OpenOCD is run in "batch" mode with a special configuration file and a script file containing
# the flash commands. When flash programming completes, OpenOCD terminates.
#
# Note that the script file of flash commands (script.ocd) is part of the project
#
# Programmers: Martin Thomas, Joseph M Dupre, James P Lynch
# **********************************************************************************************

# specify output filename here (must be *.bin file)
#TARGET = out.bin 
TARGET = $(TARGET_NAME).bin

# specify the directory where openocd executable and configuration files reside
#OPENOCD_DIR =

# specify OpenOCD executable (pp is for the wiggler, ftd2xx is for the USB debuggers)
OPENOCD = openocd
#OPENOCD = $(OPENOCD_DIR)openocd-ftd2xx.exe

# specify OpenOCD configuration file (pick the one for your device)
OPENOCD_CFG =  -f interface/parport.cfg -f target/sam7x256.cfg -c init -f script.ocd
#OPENOCD_CFG = $(OPENOCD_DIR)at91sam7s256-jtagkey-flash-program.cfg
#OPENOCD_CFG = $(OPENOCD_DIR)at91sam7s256-armusbocd-flash-program.cfg

# program the AT91SAM7S256 internal flash memory
program: $(TARGET)
	@echo "Flash Programming with OpenOCD..."			# display a message on the console
	$(OPENOCD) $(OPENOCD_CFG)						# program the onchip FLASH here
	@echo "Flash Programming Finished."					# display a message on the console





