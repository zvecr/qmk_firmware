#pragma once

typedef enum usart_status_t {
    USART_STATUS_SUCCESS = 0,
    USART_STATUS_ERROR = -1
};

void usart_init(void);
usart_status_t usart_transmit(uint8_t address, const uint8_t* data, uint16_t length, uint16_t timeout);
usart_status_t usart_receive(uint8_t address, uint8_t* data, uint16_t length, uint16_t timeout);
