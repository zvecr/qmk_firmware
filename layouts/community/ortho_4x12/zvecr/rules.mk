BOOTMAGIC_ENABLE = lite
RGBLIGHT_ENABLE = yes
BACKLIGHT_ENABLE = no
NKRO_ENABLE = yes
COMMAND_ENABLE = no
CONSOLE_ENABLE = no
MOUSEKEY_ENABLE = no
AUDIO_ENABLE = no

# for some reason, bootmagic causes lets_split_eh right side to stop working
ifeq ("$(KEYBOARD)","lets_split_eh")
  BOOTMAGIC_ENABLE = no
endif

# disable nkro for V-USB boards
ifeq ("$(KEYBOARD)","jj40")
  NKRO_ENABLE = no
endif
