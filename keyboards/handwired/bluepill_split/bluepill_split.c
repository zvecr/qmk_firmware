#include "bluepill_split.h"

bool is_keyboard_master(void) {
    #pragma message("bodge for keyboard master detection")

    #define SPLIT_MASTER_PIN B12
  
    setPinInputHigh(SPLIT_MASTER_PIN);
    return readPin(SPLIT_MASTER_PIN);
}
