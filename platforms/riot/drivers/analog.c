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
#ifdef QMK_MCU_FAMILY_SAM
    for (uint8_t line = 0; line < ADC_NUMOF; line++) {
#    ifdef ADC0
        const uint8_t adc = adc_channels[line].dev == ADC1 ? 1 : 0;
#    else
        const uint8_t adc = 0;
#    endif
        uint8_t muxpos  = (adc_channels[line].inputctrl & ADC_INPUTCTRL_MUXPOS_Msk) >> ADC_INPUTCTRL_MUXPOS_Pos;
        gpio_t  adc_pin = sam0_adc_pins[adc][muxpos];
        if (gpio_is_equal(adc_pin, pin)) {
            return line;
        }
    }
#endif
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
