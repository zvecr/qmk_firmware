ifeq ($(strip $(AUDIO_ENABLE)), yes)
    SRC += muse.c
endif

STENO_ENABLE=yes
STENO_PROTOCOL=plover_hid
