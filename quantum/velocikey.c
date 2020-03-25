#include "velocikey.h"
#include "timer.h"
#include "eeconfig.h"
#include "eeprom.h"
#include "wpm.h"

#ifndef MIN
#    define MIN(a, b) (((a) < (b)) ? (a) : (b))
#endif
#ifndef MAX
#    define MAX(a, b) (((a) > (b)) ? (a) : (b))
#endif

#ifndef TYPING_SPEED_MAX_WPM
#    define TYPING_SPEED_MAX_WPM 100
#endif

static bool enabled = false;

bool velocikey_enabled(void) { return enabled; }

void velocikey_toggle_noeeprom(void) { enabled ^= 1; }

void velocikey_toggle(void) {
    velocikey_toggle_noeeprom();

    eeprom_update_byte(EECONFIG_VELOCIKEY, enabled);
}

void velocikey_enable(void) { enabled = true; }

void velocikey_disable(void) { enabled = false; }

void velocikey_init(void) {
    static bool init = false;
    if (init) {
        return;
    }

    enabled = eeprom_read_byte(EECONFIG_VELOCIKEY) == 1;

    init = true;
}

uint8_t velocikey_match_speed(uint8_t minValue, uint8_t maxValue) {
    float scale = ((float)get_current_wpm() / TYPING_SPEED_MAX_WPM);
    if (scale > 1) scale = 1;

    uint8_t ret = MAX(minValue, maxValue - (maxValue - minValue) * scale);

    return ret;
}
