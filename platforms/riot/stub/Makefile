# name of your application
APPLICATION = riot_stub

# If no BOARD is found in the environment, use this default:
BOARD ?= native

# This has to be the absolute path to the RIOT base directory:
RIOTBASE ?= $(CURDIR)/../../../lib/riot/

# This is usually provided
BINDIRBASE ?= $(CURDIR)/../../../.build/riotbleh

# We handle linking ourselves - this also avoids having to specify a main?
# RIOTNOLINK = 1

# Comment this out to disable code in RIOT that does safety checking
# which is not needed in a production environment but helps in the
# development process:
# DEVELHELP ?= 1

# Change this to 0 show compiler invocation lines by default:
QUIET = 1

# Change this to 0 show module compilation lines by default:
QUIETER = 1

# CFLAGS_OPT = -O2
# LTO = 1

# Default to compiling only modules which have been bound to QMK
FEATURES_OPTIONAL += periph_i2c
FEATURES_OPTIONAL += periph_flashpage # For eeprom emulation
FEATURES_OPTIONAL += periph_adc
FEATURES_OPTIONAL += periph_spi
FEATURES_OPTIONAL += periph_uart
FEATURES_OPTIONAL += periph_uart_modecfg
FEATURES_OPTIONAL += periph_wdt
FEATURES_OPTIONAL += periph_cpuid
DISABLE_MODULE += auto_init_usbus
USEMODULE += usbus
USEMODULE += usbus_hid
USEMODULE += xtimer
USEMODULE += ztimer_xtimer_compat
USEMODULE += ztimer64_xtimer_compat
USEMODULE += usb_board_reset

DISABLE_MODULE += stdio
USEMODULE += stdio_null

# Bind USB Config
CFLAGS += -include $(CURDIR)/../usb_config_adapter.h

include $(RIOTBASE)/Makefile.include

ifneq ($(EXTRA_LIB),)
ARCHIVES += $(EXTRA_LIB)
endif

dump_vars: $(RIOTBUILD_CONFIG_HEADER_C)
	@echo $(INCLUDES) > $(BINDIRBASE)/riot_inc.txt
	@echo $(CFLAGS_CPU) > $(BINDIRBASE)/riot_cflags.txt

uber_lib: $(BASELIBS) $(ARCHIVES)
	$(Q)$(LINK) -r -nodefaultlibs -o $(BINDIRBASE)/$(APPLICATION).o $(shell find $(BASELIBS:%.module=$(BINDIR)/%/) -name "*.o" 2> /dev/null | grep -v syscalls.o | sort)
