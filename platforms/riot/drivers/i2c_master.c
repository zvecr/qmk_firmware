// Copyright 2023 QMK
// SPDX-License-Identifier: GPL-2.0-or-later

#include "periph/i2c.h"
#include "i2c_master.h"
#undef i2c_init

#ifndef I2C_BUS_ID
#    define I2C_BUS_ID (I2C_DEV(0))
#endif

__attribute__((weak)) void _i2c_init(void) {
    for (uint8_t i = 0; i < I2C_NUMOF; i++)
        i2c_init(I2C_DEV(i));
}

i2c_status_t _i2c_start(uint8_t devaddr, uint16_t timeout) {
    i2c_acquire(I2C_BUS_ID);
    return I2C_STATUS_SUCCESS;
}

i2c_status_t _i2c_transmit(uint8_t devaddr, const uint8_t* data, uint16_t length, uint16_t timeout) {
    return (i2c_write_bytes(I2C_BUS_ID, (devaddr >> 1), data, length, 0) == 0) ? I2C_STATUS_SUCCESS : I2C_STATUS_ERROR;
}

i2c_status_t _i2c_receive(uint8_t devaddr, uint8_t* data, uint16_t length, uint16_t timeout) {
    return (i2c_read_bytes(I2C_BUS_ID, (devaddr >> 1), data, length, 0) == 0) ? I2C_STATUS_SUCCESS : I2C_STATUS_ERROR;
}

i2c_status_t _i2c_writeReg(uint8_t devaddr, uint8_t regaddr, const uint8_t* data, uint16_t length, uint16_t timeout) {
    return (i2c_write_regs(I2C_BUS_ID, (devaddr >> 1), regaddr, data, length, 0) == 0) ? I2C_STATUS_SUCCESS : I2C_STATUS_ERROR;
}

i2c_status_t _i2c_readReg(uint8_t devaddr, uint8_t regaddr, uint8_t* data, uint16_t length, uint16_t timeout) {
    return (i2c_read_regs(I2C_BUS_ID, (devaddr >> 1), regaddr, data, length, 0) == 0) ? I2C_STATUS_SUCCESS : I2C_STATUS_ERROR;
}

void _i2c_stop(void) {
    i2c_release(I2C_BUS_ID);
}
