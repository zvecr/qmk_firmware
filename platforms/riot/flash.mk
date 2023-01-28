# Hey Emacs, this is a -*- makefile -*-
##############################################################################
# Architecture or project specific options
#

MDLOADER_CLI ?= mdloader

define EXEC_MDLOADER
	$(MDLOADER_CLI) --first --download $(BUILD_DIR)/$(TARGET).bin --restart
endef

define EXEC_UF2_UTIL_DEPLOY
	if ! $(UF2CONV) --deploy $(BUILD_DIR)/$(TARGET).uf2 2>/dev/null; then \
		printf "$(MSG_BOOTLOADER_NOT_FOUND_QUICK_RETRY)" ;\
		sleep $(BOOTLOADER_RETRY_TIME) ;\
		while ! $(UF2CONV) --deploy $(BUILD_DIR)/$(TARGET).uf2  2>/dev/null; do \
			printf "." ;\
			sleep $(BOOTLOADER_RETRY_TIME) ;\
		done ;\
		printf "\n" ;\
	fi
endef

flash: $(FIRMWARE_FORMAT)
ifneq ($(strip $(PROGRAM_CMD)),)
	$(UNSYNC_OUTPUT_CMD) && $(PROGRAM_CMD)
else ifeq ($(strip $(BOOTLOADER)), md-boot)
	$(UNSYNC_OUTPUT_CMD) && $(call EXEC_MDLOADER)
else ifeq ($(strip $(BOOTLOADER)), sam-ba)
	$(UNSYNC_OUTPUT_CMD) && $(call EXEC_UF2_UTIL_DEPLOY)
else ifeq ($(strip $(BOOTLOADER)), adafruit-nrf52)
	$(UNSYNC_OUTPUT_CMD) && $(call EXEC_UF2_UTIL_DEPLOY)
else
	$(PRINT_OK); $(SILENT) || printf "$(MSG_FLASH_ARCH)"
endif
