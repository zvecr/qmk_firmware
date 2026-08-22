// Copyright 2026 QMK
// SPDX-License-Identifier: GPL-2.0-or-later
#include QMK_KEYBOARD_H

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {LAYOUT_ortho_1x1(KC_A)};

void keyboard_post_init_user(void) {
    // Customise these values to desired behaviour
    debug_enable = true;
    debug_matrix = true;
    // debug_keyboard=true;
    // debug_mouse=true;
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (record->event.pressed) {
        eeconfig_update_user(eeconfig_read_user() + 1);
    }
    return true;
}

uint16_t nvm_eeconfig_compute_magic(void);

void housekeeping_task_user(void) {
    static uint32_t last_sync = 0;
    if (timer_elapsed32(last_sync) > 1000) {
        last_sync = timer_read32();

        uint32_t magic = nvm_eeconfig_compute_magic();
        dprintf("Magic: %ld\n", magic);
        dprintf("User: %ld\n", eeconfig_read_user());
    }
}
