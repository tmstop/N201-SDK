#
# Copyright (c) 2018-2019 Nuclei System Technology, Inc
# All rights reserved.
#

PROG	?= demo_eclic
GDB_PORT ?= 3333
PFLOAT     := 1

RISCV_ARCH := rv32iac
RISCV_ABI  := ilp32

prebuilt_tools := ../../shared/prebuilt_tools
toolchain_bin := $(prebuilt_tools)/riscv
openocd_bin   := $(prebuilt_tools)/openocd

RISCV_PATH ?= $(toolchain_bin)
PATH := $(abspath $(RISCV_PATH)/bin):$(PATH)

RISCV_OPENOCD_PATH ?= $(openocd_bin)
RISCV_OPENOCD ?= $(RISCV_OPENOCD_PATH)/bin/openocd

CROSS_COMPILE = riscv-none-embed-

CC	    := $(CROSS_COMPILE)gcc
C++	    := $(CROSS_COMPILE)g++
OBJDUMP	:= $(CROSS_COMPILE)objdump
OBJCOPY := $(CROSS_COMPILE)objcopy
AR	    := $(CROSS_COMPILE)ar
AS	    := $(CROSS_COMPILE)as
LD	    := $(CROSS_COMPILE)gcc
SIZE	:= $(CROSS_COMPILE)size
GDB	:=$(CROSS_COMPILE)gdb


OPTIM   := -O2 -g3

# Mode: LOAD, BURN, XIP
MODE	?= XIP

# Platform: 
PLAT	= N201SOC

# Code coverage analysis
#GCOV	?= 0

PLATNAME = $(shell echo $(PLAT) | tr A-Z a-z)


# Define the directories
ROOT			= ..
SRC_PATH		= ../src
BSP_SOURCE_DIR		= $(SRC_PATH)/bsp
DEMO_SOURCE_DIR		= $(SRC_PATH)/demo

# Define the source files we have

# BSP source files

STARTUP_SRCS = $(BSP_SOURCE_DIR)/start.S

BSP_SRCS = \
	$(BSP_SOURCE_DIR)/reset.c \
	$(BSP_SOURCE_DIR)/initfini.c \
	$(BSP_SOURCE_DIR)/trap.c \
	$(BSP_SOURCE_DIR)/core.c \
	$(BSP_SOURCE_DIR)/$(PLATNAME)/$(PLATNAME).c \
	$(BSP_SOURCE_DIR)/$(PLATNAME)/interrupt.c \
	$(BSP_SOURCE_DIR)/$(PLATNAME)/uart.c  

BSP_SRCS += $(BSP_SOURCE_DIR)/$(PLATNAME)/loader.c

LIBC_SRCS = \
	$(BSP_SOURCE_DIR)/lib/_close.c    \
	$(BSP_SOURCE_DIR)/lib/_exit.c     \
	$(BSP_SOURCE_DIR)/lib/_fstat.c    \
	$(BSP_SOURCE_DIR)/lib/_isatty.c   \
	$(BSP_SOURCE_DIR)/lib/_lseek.c    \
	$(BSP_SOURCE_DIR)/lib/_read.c     \
	$(BSP_SOURCE_DIR)/lib/_sbrk.c     \
	$(BSP_SOURCE_DIR)/lib/_write.c    \
	$(BSP_SOURCE_DIR)/lib/write_hex.c  

# Demo source files
DEMO_SRCS = \
	$(DEMO_SOURCE_DIR)/main.c



# compilation options

ifeq ($(MODE),LOAD)
	LDSCRIPT = $(ROOT)/$(PLATNAME)-load.ld
endif

ifeq ($(MODE),BURN)
	LDSCRIPT = $(ROOT)/$(PLATNAME)-burn.ld
endif

ifeq ($(MODE),XIP)
	LDSCRIPT = $(ROOT)/$(PLATNAME)-xip.ld
endif

INCLUDES = \
	-I$(BSP_SOURCE_DIR) \
	-I$(BSP_SOURCE_DIR)/$(PLATNAME) \
	-I$(BSP_SOURCE_DIR)/lib \
	-I$(DEMO_SOURCE_DIR)

# Define all object files.
SRCS = $(STARTUP_SRCS) $(BSP_SRCS) $(LIBC_SRCS) $(DEMO_SRCS)
OBJS = $(patsubst %.S,%.o,$(patsubst %.c,%.o,${SRCS}))

### Verbosity control. Use 'make V=1' to get verbose builds.

V=1

ifeq ($(V),1)
TRACE_CC  =
TRACE_C++ =
TRACE_LD  =
TRACE_AR  =
TRACE_AS  =
Q=
else
TRACE_CC  = @echo "  CC       " $<
TRACE_C++ = @echo "  C++      " $<
TRACE_LD  = @echo "  LD       " $@
TRACE_AR  = @echo "  AR       " $@
TRACE_AS  = @echo "  AS       " $<
Q=@
endif

# Common compilation options

CFLAGS = \
	-Wall $(OPTIM) $(INCLUDES) \
	-fomit-frame-pointer -fno-strict-aliasing -fno-builtin \
	-ffunction-sections -fdata-sections  -march=$(RISCV_ARCH) -mabi=$(RISCV_ABI) 

CXXFLAGS = $(CFLAGS)

ASFLAGS = $(CFLAGS) -c

LDFLAGS = -T$(LDSCRIPT) $(OPTIM) -static -nostartfiles -Wl,--gc-sections -march=$(RISCV_ARCH) -mabi=$(RISCV_ABI)
LDFLAGS += --specs=nano.specs --specs=nosys.specs
ifeq ($(PFLOAT),1) 
LDFLAGS += -u _printf_float 
endif

LIBS	= 


#ifeq ($(GCOV), 1)
#	CFLAGS += -DCFG_GCOV --coverage
#	LDFLAGS += -mvh --coverage
#endif


#### Retarget for C++ application using STL
#ifeq ($(filter $(DEMO), CPLUSPLUS),$(DEMO))
#	LD = $(C++)
#	LDFLAGS += -lstdc++
#endif



# Compilation rules
.SUFFIXES : %.o %.c %.S

%.o: %.c
	$(TRACE_CC)
	$(Q)$(CC) -c -MMD $(CFLAGS) -o $@ $<

%.o: %.S
	$(TRACE_CC)
	$(Q)$(CC) -c -MMD $(ASFLAGS) -o $@ $<

all: $(PROG).elf

$(PROG).elf  : $(OBJS) Makefile 
	$(TRACE_LD)
	$(Q)$(LD)  -o $@ $(LDFLAGS) $(OBJS) $(LIBS)
	$(SIZE)    $(PROG).elf 
	$(OBJDUMP) -d $(PROG).elf > $(PROG).asm
	$(OBJDUMP) -D $(PROG).elf > $(PROG).dump
	$(OBJCOPY)  $(PROG).elf  -O verilog $(PROG).verilog
	sed -i 's/@800/@000/g' $(PROG).verilog
	@echo Completed

clean :
	@rm -f $(OBJS) $(OBJS:.o=.d) $(OBJS:.o=.gcda) $(OBJS:.o=.gcno)
	@rm -f $(PROG).elf 
	@rm -f $(PROG).asm
	@rm -f $(PROG).dump
	@rm -f $(PROG).verilog

# Automatic dependency generation
ifneq ($(MAKECMDGOALS),clean)
-include $(OBJS:.o=.d)
endif





#############################################################
# Downloading a program to SPI Flash
#############################################################
OPENOCDCFG ?= ../openocd_hbird.cfg

OPENOCDARGS += -f $(OPENOCDCFG)

GDB_DOWNLOAD_ARGS ?= --batch

GDB_DOWNLOAD_CMDS += -ex "set remotetimeout 240"
GDB_DOWNLOAD_CMDS += -ex "target extended-remote localhost:$(GDB_PORT)"
GDB_DOWNLOAD_CMDS += -ex "monitor reset halt"
GDB_DOWNLOAD_CMDS += -ex "monitor flash protect 0 0 last off"
GDB_DOWNLOAD_CMDS += -ex "load"
GDB_DOWNLOAD_CMDS += -ex "monitor resume"
GDB_DOWNLOAD_CMDS += -ex "monitor shutdown"
GDB_DOWNLOAD_CMDS += -ex "quit"


download:
	$(RISCV_OPENOCD) $(OPENOCDARGS) & \
	$(GDB) $(PROG).elf $(GDB_DOWNLOAD_ARGS) $(GDB_DOWNLOAD_CMDS) && \
	echo "Successfully downloaded '$(PROG)' to board."

#############################################################
# Launching the gdb or openocd
#############################################################

run_openocd:
	$(RISCV_OPENOCD) $(OPENOCDARGS)

GDBCMDS += -ex "set remotetimeout 240"
GDBCMDS += -ex "target extended-remote localhost:$(GDB_PORT)"

run_gdb:
	$(GDB) $(PROG).elf $(GDBARGS) $(GDBCMDS)
