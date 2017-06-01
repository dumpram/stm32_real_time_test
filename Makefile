################################################################################
# Project: RealTimeTest
# 
# Module: top module
# 
# Author: Ivan Pavic (@dumpram)
#
# version: 0.0.0
################################################################################

MODULE_NAME = rtt

# STLINK path
STLINK=~/stlink

# Toolchain
TOOLCHAIN_ROOT = /home/dumpram/arm-compilers/gcc-arm-none-eabi-5_4-2016q3
PREFIX = ~/arm-compilers/gcc-arm-none-eabi-5_4-2016q3/bin/arm-none-eabi-

CC = $(PREFIX)gcc
AS = $(PREFIX)gcc
LD = $(PREFIX)gcc
AR = $(PREFIX)ar
BIN = $(PREFIX)objcopy

LIB_C = $(TOOLCHAIN_ROOT)/arm-none-eabi/lib
LIB_GCC = $(TOOLCHAIN_ROOT)/lib/gcc/arm-none-eabi/5.4.1

# C source flags
CFLAGS = -Wall \
         -Werror \
         -mcpu=cortex-m4 \
         -mthumb \
         -mfloat-abi=softfp \
         -fomit-frame-pointer \
         -fno-strict-aliasing \
         -specs=nosys.specs \
         -specs=nano.specs

# Assembler flags
AFLAGS += -x assembler-with-cpp

# Linker flags
LDFLAGS = -specs=nosys.specs -specs=nano.specs -mcpu=cortex-m4 -mthumb

INC =  -I Drivers/STM32F4xx_HAL_Driver/Inc
INC += -I Drivers/CMSIS/Include
INC += -I Drivers/CMSIS/Device/ST/STM32F4xx/Include
INC += -I Inc

CFLAGS += $(INC)
CFLAGS += -g -D STM32F407xx -D USE_HAL_DRIVER

SRC  = $(wildcard Src/*.c)
SRC += $(wildcard Drivers/STM32F4xx_HAL_Driver/Src/*.c)

ASRC = $(wildcard SW4STM32/*.s)


OBJ = $(patsubst %.c,%.o,$(SRC))
AOBJ = $(patsubst %.s,%.o,$(ASRC))

LINKER_SCRIPT = SW4STM32/RealTimeTest/STM32F407VGTx_FLASH.ld

# ObjCopy flags
BINFLAGS = -O binary

.PRECIOUS: %.c %.o

all: app.bin

%.o: %.c Makefile
	@$(CC) $(CFLAGS) -c $< -o $@
	@echo "$(MODULE_NAME): Compiled $< successfully!"

%.o : %.s Makefile
	@$(AS) $(CFLAGS) $(AFLAGS) -c $< -o $@
	@echo "$(MODULE_NAME): Compiled "$<" successfully!"

app.bin : app.elf
	@$(BIN) $< $(BINFLAGS) $@
	@echo "$(MODULE_NAME): Generated binary successfully!"

%.elf : $(AOBJ) $(OBJ)
	@$(LD) $(AOBJ) $(OBJ) \
	$(LDFLAGS) -o $@ \
	-L$(LIB_GCC) \
	-lc  \
	-T$(LINKER_SCRIPT)
	@echo "$(MODULE_NAME): Linked app successfully!"

clean :
	rm -f app.elf app.bin
	rm $(OBJ)

burn: app.bin
	$(STLINK)/st-flash write app.bin 0x8000000
