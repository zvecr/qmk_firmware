#include "velocikey.h"
#include "timer.h"
#include "eeconfig.h"
#include "eeprom.h"
#include "debug.h"

#ifndef MIN
#    define MIN(a, b) (((a) < (b)) ? (a) : (b))
#endif
#ifndef MAX
#    define MAX(a, b) (((a) > (b)) ? (a) : (b))
#endif

#define TYPING_SPEED_MAX_VALUE 200
uint8_t typing_speed = 0;

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

void velocikey_accelerate(void) {
    if (typing_speed < TYPING_SPEED_MAX_VALUE) typing_speed += (TYPING_SPEED_MAX_VALUE / 100);
}

void velocikey_decelerate(void) {
    static uint16_t decay_timer = 0;

    if (timer_elapsed(decay_timer) > 500 || decay_timer == 0) {
        if (typing_speed > 0) typing_speed -= 1;
        // Decay a little faster at half of max speed
        if (typing_speed > TYPING_SPEED_MAX_VALUE / 2) typing_speed -= 1;
        // Decay even faster at 3/4 of max speed
        if (typing_speed > TYPING_SPEED_MAX_VALUE / 4 * 3) typing_speed -= 2;
        decay_timer = timer_read();
    }
}

#include "wpm.h"
#define TYPING_SPEED_MAX_WPM 100

uint8_t velocikey_match_speed(uint8_t minValue, uint8_t maxValue) {
    /// uint8_t ret =  MAX(minValue, maxValue - (maxValue - minValue) * ((float)typing_speed / TYPING_SPEED_MAX_VALUE));
    // dprintf("velocikey_match_speed:%u\n", ret);
    float scale = ((float)get_current_wpm() / TYPING_SPEED_MAX_WPM);
    if (scale > 1) scale = 1;

    uint8_t ret = MAX(minValue, maxValue - (maxValue - minValue) * scale);

    return ret;
}
