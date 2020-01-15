#pragma once

#ifdef USART1_BANK
#    define USART1_TX_BANK USART1_BANK
#    define USART1_RX_BANK USART1_BANK
#endif

#ifndef USART1_TX_BANK
#    define USART1_TX_BANK GPIOB
#endif

#ifndef USART1_RX_BANK
#    define USART1_RX_BANK GPIOB
#endif

#ifndef USART1_TX
#    define USART1_TX 6
#endif
#ifndef USART1_RX
#    define USART1_RX 7
#endif

#ifndef USE_GPIOV1
// The default PAL alternate modes are used to signal that the pins are used for USART
#    ifndef USART1_TX_PAL_MODE
#        define USART1_TX_PAL_MODE 7
#    endif
#    ifndef USART1_RX_PAL_MODE
#        define USART1_RX_PAL_MODE 7
#    endif
#endif

#ifndef USART_DRIVER
#    define USART_DRIVER SD1
#endif

#ifndef USART_SPEED
#    define USART_SPEED 38400
#endif

typedef enum usart_status_t {
    USART_STATUS_SUCCESS = 0,
    USART_STATUS_ERROR = -1
} usart_status_t;

void usart_init(void);
usart_status_t usart_transmit(const uint8_t* data, uint16_t length, uint16_t timeout);
usart_status_t usart_receive(uint8_t* data, uint16_t length, uint16_t timeout);
