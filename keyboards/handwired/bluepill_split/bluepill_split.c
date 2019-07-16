#include "bluepill_split.h"

bool is_keyboard_master(void) {
    #pragma message("bodge for keyboard master detection")

    // #define SPLIT_MASTER_PIN B12
  
    // setPinInputHigh(SPLIT_MASTER_PIN);
    // return readPin(SPLIT_MASTER_PIN);
    static uint8_t is_master = 0xff;
    if(is_master == 0xff) {

        is_master = false;
        for(uint8_t i = 0; i < 25; i++) {
            if (usbGetDriverStateI(&USBD1) == USB_ACTIVE) {
                is_master = true;
                break;
            }

  //dprintf("is_keyboard_master::is_master false %d\n");
            wait_ms(100);
        }
    }
    return !!is_master;
}
