#include <stdint.h>
#include <stdbool.h>
#include <hal.h>
#include "i2c_slave.h"

volatile uint8_t i2c_slave_reg[I2C_SLAVE_REG_COUNT];

void i2c_slave_init(uint8_t address) {
    // TODO: stub implementation to allow SPLIT_KEYBOARD compilation
}

void i2c_slave_stop(void){
    // TODO: stub implementation to allow SPLIT_KEYBOARD compilation
}
