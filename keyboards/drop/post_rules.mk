ifeq ($(strip $(XAP_ENABLE)), yes)
	SRC +=  lib/xap_kb.c
	CONFIG_H += lib/xap_kb.h
endif
