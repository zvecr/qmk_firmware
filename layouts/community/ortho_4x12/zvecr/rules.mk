RGBLIGHT_ENABLE = yes
BACKLIGHT_ENABLE = no
NKRO_ENABLE = yes
COMMAND_ENABLE = no
CONSOLE_ENABLE = no
MOUSEKEY_ENABLE = no

# for some reason, bootmagic causes lets_split_eh right side to stop working
ifeq ("$(KEYBOARD)","lets_split")
  BOOTMAGIC_ENABLE = lite
endif
