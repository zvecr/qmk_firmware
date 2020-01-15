#include "quantum.h"
#include "usart.h"

void usart_init(void) {

}

usart_status_t usart_transmit(const uint8_t* data, uint16_t length, uint16_t timeout) {
    return USART_STATUS_ERROR;
}

usart_status_t usart_receive(uint8_t* data, uint16_t length, uint16_t timeout) {
    return USART_STATUS_ERROR;
}
