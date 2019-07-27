BOOTMAGIC_ENABLE = lite
RGBLIGHT_ENABLE = yes
BACKLIGHT_ENABLE = no
NKRO_ENABLE = yes
COMMAND_ENABLE = no
CONSOLE_ENABLE = no
MOUSEKEY_ENABLE = no
AUDIO_ENABLE = no
LINK_TIME_OPTIMIZATION_ENABLE = yes

ifeq ("$(BOOTLOADER)","caterina")
  BOOTLOADER = atmel-dfu
endif

# disable nkro for V-USB boards
ifeq ("$(KEYBOARD)","jj40")
  NKRO_ENABLE = no
endif
