// Copyright 2023 QMK
// SPDX-License-Identifier: GPL-2.0-or-later

#include "analog.h"
#include "periph/adc.h"
#include "periph/gpio.h"

#ifndef ADC_RESOLUTION
#    define ADC_RESOLUTION ADC_RES_12BIT
#endif

static inline void manageAdcInitialization(uint8_t index) {
    static int32_t initialized_flags = 0;
    if (!(initialized_flags & (1 << index))) {
        initialized_flags |= (1 << index);

        adc_init(ADC_LINE(index));
    }
}

__attribute__((weak)) uint8_t pinToLine(pin_t pin) {
    for (uint8_t i = 0; i < ADC_NUMOF; i++) {
        if (gpio_is_equal(adc_channels[i].pin, pin)) {
            return i;
        }
    }
    return UINT8_MAX;
}

int16_t analogReadPin(pin_t pin) {
    uint8_t index = pinToLine(pin);
    if (index == UINT8_MAX) {
        printf("pin not configured!!!\n");
        return 0;
    }

    manageAdcInitialization(index);
    return adc_sample(ADC_LINE(index), ADC_RESOLUTION);
}
