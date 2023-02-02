QUANTUM_LIB_SRC += i2c_master.c
SRC += usb2422.c analog.c shift_register.c lib/i2c_multi_bus.c lib/shifter.c lib/samd_common.c

RGB_MATRIX_DRIVER = IS31FL3733

CUSTOM_MATRIX = lite
QUANTUM_LIB_SRC += spi_master.c
SRC += matrix.c
