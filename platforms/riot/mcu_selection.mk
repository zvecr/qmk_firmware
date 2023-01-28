ifneq ($(findstring SAMD21, $(MCU)),)
  # Cortex version
  MCU = cortex-m0plus

  MCU_FAMILY = SAM
  MCU_SERIES = SAMD21

  # Mapped to riots BOARD arguement
  BOARD ?= feather-m0

  # UF2 settings
  UF2_FAMILY ?= SAMD21

  ROM_OFFSET=0x2000
endif

ifneq ($(findstring SAMD51, $(MCU)),)
  # Cortex version
  MCU = cortex-m4

  MCU_FAMILY = SAM
  MCU_SERIES = SAMD51

  # Mapped to riots BOARD arguement
  BOARD ?= feather-m4

  # UF2 settings
  UF2_FAMILY ?= SAMD51

  ROM_OFFSET=0x4000
endif

ifneq ($(findstring nRF52840, $(MCU)),)
  # Cortex version
  MCU = cortex-m4

  MCU_FAMILY = NRF
  MCU_SERIES = NRF52

  # Mapped to riots BOARD arguement
  BOARD ?= adafruit-itsybitsy-nrf52

  # UF2 settings
  UF2_FAMILY ?= NRF52840
endif
