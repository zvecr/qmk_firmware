/* Copyright 2020 Vladislav Kucheriavykh
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdio.h>
#include "quantum.h"
#include "process_tap_term_keys.h"

#ifndef TAP_TERM_INCREMENT
#    define TAP_TERM_INCREMENT 5
#endif

extern uint16_t g_tapping_term;

static void tapping_term_report(void) {
    char display[8];

    snprintf(display, 8, "%d", g_tapping_term);

    send_string((const char *)display);
}

uint16_t get_global_tapping_term(void) { return g_tapping_term; }

bool process_tap_term_keys(uint16_t keycode, keyrecord_t *record) {
    if (record->event.pressed) {
        switch (keycode) {
            case TK_PRNT:
                tapping_term_report();
                return false;

            case TK_UP:
                g_tapping_term += TAP_TERM_INCREMENT;
                return false;

            case TK_DOWN:
                g_tapping_term -= TAP_TERM_INCREMENT;
                return false;
        }
    }
    return true;
}
