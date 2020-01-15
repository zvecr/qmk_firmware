#include "quantum.h"
#include "uart.h"

void usart_init(void) {

}

usart_status_t usart_transmit(uint8_t address, const uint8_t* data, uint16_t length, uint16_t timeout) {
    return USART_STATUS_ERROR;
}

usart_status_t usart_receive(uint8_t address, uint8_t* data, uint16_t length, uint16_t timeout) {
    return USART_STATUS_ERROR;
}
