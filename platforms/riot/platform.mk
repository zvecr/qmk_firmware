# Hey Emacs, this is a -*- makefile -*-
##############################################################################
# Project, sources and paths
#

# avoid 'Entering|Leaving directory' messages
MAKEFLAGS += --no-print-directory

RIOTBASE = $(TOP_DIR)/lib/riot

# When required - bail out early without producing a wall of random errors
ifeq ("$(wildcard $(RIOTBASE)/Makefile.include)","")
    $(error RIOT submodule missing)
endif

RM=/usr/bin/rm -f
ARFLAGS = rcTs
export RM
export ARFLAGS
LINKFLAGPREFIX ?= -Wl,

##############################################################################
# Compiler settings
#
CC = arm-none-eabi-gcc
OBJCOPY = arm-none-eabi-objcopy
OBJDUMP = arm-none-eabi-objdump
SIZE = arm-none-eabi-size
AR = arm-none-eabi-ar
NM = arm-none-eabi-nm
LD = arm-none-eabi-ld
HEX = $(OBJCOPY) -O $(FORMAT)
EEP =
BIN = $(OBJCOPY) -O binary

##############################################################################
# Board support selection.
#

KEYBOARD_BOARD_DIR = $(PLATFORM_COMMON_DIR)/boards/

ifneq ("$(wildcard $(KEYBOARD_PATH_5)/boards/$(BOARD)/Makefile)","")
    KEYBOARD_BOARD_DIR = $(KEYBOARD_PATH_5)/boards/
else ifneq ("$(wildcard $(KEYBOARD_PATH_4)/boards/$(BOARD)/Makefile)","")
    KEYBOARD_BOARD_DIR = $(KEYBOARD_PATH_4)/boards/
else ifneq ("$(wildcard $(KEYBOARD_PATH_3)/boards/$(BOARD)/Makefile)","")
    KEYBOARD_BOARD_DIR = $(KEYBOARD_PATH_3)/boards/
else ifneq ("$(wildcard $(KEYBOARD_PATH_2)/boards/$(BOARD)/Makefile)","")
    KEYBOARD_BOARD_DIR = $(KEYBOARD_PATH_2)/boards/
else ifneq ("$(wildcard $(KEYBOARD_PATH_1)/boards/$(BOARD)/Makefile)","")
    KEYBOARD_BOARD_DIR = $(KEYBOARD_PATH_1)/boards/
endif

ifneq ("$(wildcard $(KEYBOARD_PATH_5)/lib/boards/$(BOARD)/Makefile)","")
    KEYBOARD_BOARD_DIR = $(KEYBOARD_PATH_5)/lib/boards/
else ifneq ("$(wildcard $(KEYBOARD_PATH_4)/lib/boards/$(BOARD)/Makefile)","")
    KEYBOARD_BOARD_DIR = $(KEYBOARD_PATH_4)/lib/boards/
else ifneq ("$(wildcard $(KEYBOARD_PATH_3)/lib/boards/$(BOARD)/Makefile)","")
    KEYBOARD_BOARD_DIR = $(KEYBOARD_PATH_3)/lib/boards/
else ifneq ("$(wildcard $(KEYBOARD_PATH_2)/lib/boards/$(BOARD)/Makefile)","")
    KEYBOARD_BOARD_DIR = $(KEYBOARD_PATH_2)/lib/boards/
else ifneq ("$(wildcard $(KEYBOARD_PATH_1)/lib/boards/$(BOARD)/Makefile)","")
    KEYBOARD_BOARD_DIR = $(KEYBOARD_PATH_1)/lib/boards/
endif

EXTERNAL_BOARD_DIRS = $(abspath $(KEYBOARD_BOARD_DIR))

#
# Project, sources and paths
##############################################################################

OPT_DEFS += -DPROTOCOL_RIOT

OPT_DEFS += -include $(KEYMAP_OUTPUT)/$(BOARD)/riotbuild/riotbuild.h -include usb_config_adapter.h

EXTRAINCDIRS = $(PLATFORM_PATH)/$(PLATFORM_KEY)/vendors/$(MCU_FAMILY)

PLATFORM_SRC = \
        $(PLATFORM_COMMON_DIR)/usb_config_adapter.c \

##############################################################################
# RIOT-OS Bodges
#

_RIOT_DUMP_ := $(shell $(MAKE) -C $(PLATFORM_COMMON_DIR)/stub BOARD=$(BOARD) BINDIRBASE=$(abspath $(KEYMAP_OUTPUT)) EXTERNAL_BOARD_DIRS=$(EXTERNAL_BOARD_DIRS) dump_vars)

# TODO: dirty reuse of chibios variable to workaround RIOT includes needing to be searched last
INIT_HOOK_CFLAGS := $(shell cat $(KEYMAP_OUTPUT)/riot_inc.txt)

MCUFLAGS := $(shell cat $(KEYMAP_OUTPUT)/riot_cflags.txt)

##############################################################################
# Make targets
#

# TODO: Remove using RIOT for linking
$(BUILD_DIR)/$(TARGET).elf: $(KEYMAP_OUTPUT)/qmk.o $(KEYMAP_OUTPUT)/riot_stub.o
	$(SILENT) || printf "$(MSG_LINKING) $(PLATFORM_COMMON_DIR)/stub" | $(AWK_CMD)
	$(eval CMD=$(MAKE) -C$(PLATFORM_COMMON_DIR)/stub BOARD=$(BOARD) BINDIRBASE=$(abspath $(KEYMAP_OUTPUT)) EXTERNAL_BOARD_DIRS=$(EXTERNAL_BOARD_DIRS) EXTRA_LIB=$(abspath $(KEYMAP_OUTPUT)/qmk.o) elffile)
	$(BUILD_CMD)
	$(COPY) $(KEYMAP_OUTPUT)/$(BOARD)/riot_stub.elf $(BUILD_DIR)/$(TARGET).elf

$(KEYMAP_OUTPUT)/riot_stub.o:
	$(SILENT) || printf "$(MSG_COMPILING) $(PLATFORM_COMMON_DIR)/stub" | $(AWK_CMD)
	$(eval CMD=$(MAKE) -C$(PLATFORM_COMMON_DIR)/stub BOARD=$(BOARD) BINDIRBASE=$(abspath $(KEYMAP_OUTPUT)) EXTERNAL_BOARD_DIRS=$(EXTERNAL_BOARD_DIRS) uber_lib)
	$(BUILD_CMD)

bin: $(BUILD_DIR)/$(TARGET).bin sizeafter
	$(COPY) $(BUILD_DIR)/$(TARGET).bin $(TARGET).bin;
