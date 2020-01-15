#include "quantum.h"
#include "uart.h"

#include "ch.h"
#include "hal.h"

static const SerialConfig serialcfg = {
  USART_SPEED
};

void usart_init(void) {
    sdStart(&USART_DRIVER, serialcfg);

#if defined(USE_GPIOV1)
    // palSetPadMode(USART1_TX_BANK, USART1_TX, PAL_MODE_STM32_ALTERNATE_OPENDRAIN);
    // palSetPadMode(USART1_RX_BANK, USART1_RX, PAL_MODE_STM32_ALTERNATE_OPENDRAIN);
#else
    palSetPadMode(USART1_TX_BANK, USART1_TX, PAL_MODE_ALTERNATE(USART1_TX_PAL_MODE));
    palSetPadMode(USART1_RX_BANK, USART1_RX, PAL_MODE_ALTERNATE(USART1_RX_PAL_MODE));
#endif
}

usart_status_t usart_transmit(uint8_t address, const uint8_t* data, uint16_t length, uint16_t timeout) {
    msg_t res = sdWriteTimeout(&USART_DRIVER, data, length, MS2ST(TIMEOUT));

    return ret  == MSG_OK ? USART_STATUS_SUCCESS : USART_STATUS_ERROR;
}

usart_status_t usart_receive(uint8_t address, uint8_t* data, uint16_t length, uint16_t timeout) {
    int msg_t = sdReadTimeout(&USART_DRIVER, data, length, MS2ST(TIMEOUT));

    return ret == MSG_OK ? USART_STATUS_SUCCESS : USART_STATUS_ERROR;
}
