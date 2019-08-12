RGBLIGHT_ENABLE = yes
BACKLIGHT_ENABLE = no
NKRO_ENABLE = yes
COMMAND_ENABLE = no
CONSOLE_ENABLE = no
MOUSEKEY_ENABLE = no

ifeq ("$(BOOTLOADER)","caterina")
  BOOTLOADER = atmel-dfu
endif

# disable nkro for V-USB boards
ifeq ("$(KEYBOARD)","jj40")
  NKRO_ENABLE = no
endif

ifeq ("$(KEYBOARD)","handwired/split_blackpill")
  RGBLIGHT_ENABLE = no
endif
