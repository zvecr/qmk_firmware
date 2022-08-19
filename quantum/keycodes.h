// Copyright 2022 QMK
// SPDX-License-Identifier: GPL-2.0-or-later
#pragma once

#include "generated_keycodes.h"

// TODO: ranges?
#define QK_LCTL      0x0100
#define QK_LSFT      0x0200
#define QK_LALT      0x0400
#define QK_LGUI      0x0800
#define QK_RMODS_MIN 0x1000
#define QK_RCTL      0x1100
#define QK_RSFT      0x1200
#define QK_RALT      0x1400
#define QK_RGUI      0x1800

// taken from keycodes.h?
#define IS_ERROR(code) (KC_ROLL_OVER <= (code) && (code) <= KC_UNDEFINED)
#define IS_ANY(code) (KC_A <= (code) && (code) <= 0xFF)
#define IS_KEY(code) (KC_A <= (code) && (code) <= KC_EXSEL)
#define IS_MOD(code) (KC_LEFT_CTRL <= (code) && (code) <= KC_RIGHT_GUI)

#define IS_SPECIAL(code) ((0xA5 <= (code) && (code) <= 0xDF) || (0xE8 <= (code) && (code) <= 0xFF))
#define IS_SYSTEM(code) (KC_PWR <= (code) && (code) <= KC_WAKE)
#define IS_CONSUMER(code) (KC_MUTE <= (code) && (code) <= KC_BRID)

#define IS_MOUSEKEY(code) (KC_MS_UP <= (code) && (code) <= KC_MS_ACCEL2)
#define IS_MOUSEKEY_MOVE(code) (KC_MS_UP <= (code) && (code) <= KC_MS_RIGHT)
#define IS_MOUSEKEY_BUTTON(code) (KC_MS_BTN1 <= (code) && (code) <= KC_MS_BTN8)
#define IS_MOUSEKEY_WHEEL(code) (KC_MS_WH_UP <= (code) && (code) <= KC_MS_WH_RIGHT)
#define IS_MOUSEKEY_ACCEL(code) (KC_MS_ACCEL0 <= (code) && (code) <= KC_MS_ACCEL2)

#define MOD_BIT(code) (1 << MOD_INDEX(code))
#define MOD_INDEX(code) ((code)&0x07)

#define MOD_MASK_CTRL (MOD_BIT(KC_LEFT_CTRL) | MOD_BIT(KC_RIGHT_CTRL))
#define MOD_MASK_SHIFT (MOD_BIT(KC_LEFT_SHIFT) | MOD_BIT(KC_RIGHT_SHIFT))
#define MOD_MASK_ALT (MOD_BIT(KC_LEFT_ALT) | MOD_BIT(KC_RIGHT_ALT))
#define MOD_MASK_GUI (MOD_BIT(KC_LEFT_GUI) | MOD_BIT(KC_RIGHT_GUI))
#define MOD_MASK_CS (MOD_MASK_CTRL | MOD_MASK_SHIFT)
#define MOD_MASK_CA (MOD_MASK_CTRL | MOD_MASK_ALT)
#define MOD_MASK_CG (MOD_MASK_CTRL | MOD_MASK_GUI)
#define MOD_MASK_SA (MOD_MASK_SHIFT | MOD_MASK_ALT)
#define MOD_MASK_SG (MOD_MASK_SHIFT | MOD_MASK_GUI)
#define MOD_MASK_AG (MOD_MASK_ALT | MOD_MASK_GUI)
#define MOD_MASK_CSA (MOD_MASK_CTRL | MOD_MASK_SHIFT | MOD_MASK_ALT)
#define MOD_MASK_CSG (MOD_MASK_CTRL | MOD_MASK_SHIFT | MOD_MASK_GUI)
#define MOD_MASK_CAG (MOD_MASK_CTRL | MOD_MASK_ALT | MOD_MASK_GUI)
#define MOD_MASK_SAG (MOD_MASK_SHIFT | MOD_MASK_ALT | MOD_MASK_GUI)
#define MOD_MASK_CSAG (MOD_MASK_CTRL | MOD_MASK_SHIFT | MOD_MASK_ALT | MOD_MASK_GUI)


// Keycode modifiers & aliases
#define LCTL(kc) (QK_LCTL | (kc))
#define LSFT(kc) (QK_LSFT | (kc))
#define LALT(kc) (QK_LALT | (kc))
#define LGUI(kc) (QK_LGUI | (kc))
#define LOPT(kc) LALT(kc)
#define LCMD(kc) LGUI(kc)
#define LWIN(kc) LGUI(kc)
#define RCTL(kc) (QK_RCTL | (kc))
#define RSFT(kc) (QK_RSFT | (kc))
#define RALT(kc) (QK_RALT | (kc))
#define RGUI(kc) (QK_RGUI | (kc))
#define ALGR(kc) RALT(kc)
#define ROPT(kc) RALT(kc)
#define RCMD(kc) RGUI(kc)
#define RWIN(kc) RGUI(kc)

#define HYPR(kc) (QK_LCTL | QK_LSFT | QK_LALT | QK_LGUI | (kc))
#define MEH(kc) (QK_LCTL | QK_LSFT | QK_LALT | (kc))
#define LCAG(kc) (QK_LCTL | QK_LALT | QK_LGUI | (kc))
#define LSG(kc) (QK_LSFT | QK_LGUI | (kc))
#define SGUI(kc) LSG(kc)
#define SCMD(kc) LSG(kc)
#define SWIN(kc) LSG(kc)
#define LAG(kc) (QK_LALT | QK_LGUI | (kc))
#define RSG(kc) (QK_RSFT | QK_RGUI | (kc))
#define RAG(kc) (QK_RALT | QK_RGUI | (kc))
#define LCA(kc) (QK_LCTL | QK_LALT | (kc))
#define LSA(kc) (QK_LSFT | QK_LALT | (kc))
#define RSA(kc) (QK_RSFT | QK_RALT | (kc))
#define RCS(kc) (QK_RCTL | QK_RSFT | (kc))
#define SAGR(kc) RSA(kc)

#define MOD_HYPR 0xF
#define MOD_MEH 0x7

// US ANSI shifted keycode aliases
#define KC_TILDE LSFT(KC_GRAVE) // ~
#define KC_TILD KC_TILDE

#define KC_EXCLAIM LSFT(KC_1) // !
#define KC_EXLM KC_EXCLAIM

#define KC_AT LSFT(KC_2) // @

#define KC_HASH LSFT(KC_3) // #

#define KC_DOLLAR LSFT(KC_4) // $
#define KC_DLR KC_DOLLAR

#define KC_PERCENT LSFT(KC_5) // %
#define KC_PERC KC_PERCENT

#define KC_CIRCUMFLEX LSFT(KC_6) // ^
#define KC_CIRC KC_CIRCUMFLEX

#define KC_AMPERSAND LSFT(KC_7) // &
#define KC_AMPR KC_AMPERSAND

#define KC_ASTERISK LSFT(KC_8) // *
#define KC_ASTR KC_ASTERISK

#define KC_LEFT_PAREN LSFT(KC_9) // (
#define KC_LPRN KC_LEFT_PAREN

#define KC_RIGHT_PAREN LSFT(KC_0) // )
#define KC_RPRN KC_RIGHT_PAREN

#define KC_UNDERSCORE LSFT(KC_MINUS) // _
#define KC_UNDS KC_UNDERSCORE

#define KC_PLUS LSFT(KC_EQUAL) // +

#define KC_LEFT_CURLY_BRACE LSFT(KC_LEFT_BRACKET) // {
#define KC_LCBR KC_LEFT_CURLY_BRACE

#define KC_RIGHT_CURLY_BRACE LSFT(KC_RIGHT_BRACKET) // }
#define KC_RCBR KC_RIGHT_CURLY_BRACE

#define KC_LEFT_ANGLE_BRACKET LSFT(KC_COMMA) // <
#define KC_LABK KC_LEFT_ANGLE_BRACKET
#define KC_LT KC_LEFT_ANGLE_BRACKET

#define KC_RIGHT_ANGLE_BRACKET LSFT(KC_DOT) // >
#define KC_RABK KC_RIGHT_ANGLE_BRACKET
#define KC_GT KC_RIGHT_ANGLE_BRACKET

#define KC_COLON LSFT(KC_SEMICOLON) // :
#define KC_COLN KC_COLON

#define KC_PIPE LSFT(KC_BACKSLASH) // |

#define KC_QUESTION LSFT(KC_SLASH) // ?
#define KC_QUES KC_QUESTION

#define KC_DOUBLE_QUOTE LSFT(KC_QUOTE) // "
#define KC_DQUO KC_DOUBLE_QUOTE
#define KC_DQT KC_DOUBLE_QUOTE

#define KC_DELT KC_DELETE // Del key (four letter code)

// Modified keycode aliases
#define C(kc) LCTL(kc)
#define S(kc) LSFT(kc)
#define A(kc) LALT(kc)
#define G(kc) LGUI(kc)

// GOTO layer - 16 layers max
// when:
// ON_PRESS    = 1
// ON_RELEASE  = 2
// Unless you have a good reason not to do so, prefer  ON_PRESS (1) as your default.
// In fact, we changed it to assume ON_PRESS for sanity/simplicity. If needed, you can add your own
// keycode modeled after the old version, kept below for this.
/* #define TO(layer, when) (QK_TO | (when << 0x4) | (layer & 0xFF)) */
#define TO(layer) (QK_TO | (ON_PRESS << 0x4) | ((layer)&0xFF))

// Momentary switch layer - 256 layer max
#define MO(layer) (QK_MOMENTARY | ((layer)&0xFF))

// Set default layer - 256 layer max
#define DF(layer) (QK_DEF_LAYER | ((layer)&0xFF))

// Toggle to layer - 256 layer max
#define TG(layer) (QK_TOGGLE_LAYER | ((layer)&0xFF))

// One-shot layer - 256 layer max
#define OSL(layer) (QK_ONE_SHOT_LAYER | ((layer)&0xFF))

// L-ayer M-od: Momentary switch layer with modifiers active - 16 layer max, left mods only
#define LM(layer, mod) (QK_LAYER_MOD | (((layer)&0xF) << 4) | ((mod)&0xF))

// One-shot mod
#define OSM(mod) (QK_ONE_SHOT_MOD | ((mod)&0xFF))

// Layer tap-toggle
#define TT(layer) (QK_LAYER_TAP_TOGGLE | ((layer)&0xFF))

// L-ayer, T-ap - 256 keycode max, 16 layer max
#define LT(layer, kc) (QK_LAYER_TAP | (((layer)&0xF) << 8) | ((kc)&0xFF))

// M-od, T-ap - 256 keycode max
#define MT(mod, kc) (QK_MOD_TAP | (((mod)&0x1F) << 8) | ((kc)&0xFF))

#define LCTL_T(kc) MT(MOD_LCTL, kc)
#define RCTL_T(kc) MT(MOD_RCTL, kc)
#define CTL_T(kc) LCTL_T(kc)

#define LSFT_T(kc) MT(MOD_LSFT, kc)
#define RSFT_T(kc) MT(MOD_RSFT, kc)
#define SFT_T(kc) LSFT_T(kc)

#define LALT_T(kc) MT(MOD_LALT, kc)
#define RALT_T(kc) MT(MOD_RALT, kc)
#define LOPT_T(kc) LALT_T(kc)
#define ROPT_T(kc) RALT_T(kc)
#define ALGR_T(kc) RALT_T(kc)
#define ALT_T(kc) LALT_T(kc)
#define OPT_T(kc) LOPT_T(kc)

#define LGUI_T(kc) MT(MOD_LGUI, kc)
#define RGUI_T(kc) MT(MOD_RGUI, kc)
#define LCMD_T(kc) LGUI_T(kc)
#define LWIN_T(kc) LGUI_T(kc)
#define RCMD_T(kc) RGUI_T(kc)
#define RWIN_T(kc) RGUI_T(kc)
#define GUI_T(kc) LGUI_T(kc)
#define CMD_T(kc) LCMD_T(kc)
#define WIN_T(kc) LWIN_T(kc)

#define C_S_T(kc) MT(MOD_LCTL | MOD_LSFT, kc)                        // Left Control + Shift e.g. for gnome-terminal
#define MEH_T(kc) MT(MOD_LCTL | MOD_LSFT | MOD_LALT, kc)             // Meh is a less hyper version of the Hyper key -- doesn't include GUI, so just Left Control + Shift + Alt
#define LCAG_T(kc) MT(MOD_LCTL | MOD_LALT | MOD_LGUI, kc)            // Left Control + Alt + GUI
#define RCAG_T(kc) MT(MOD_RCTL | MOD_RALT | MOD_RGUI, kc)            // Right Control + Alt + GUI
#define HYPR_T(kc) MT(MOD_LCTL | MOD_LSFT | MOD_LALT | MOD_LGUI, kc) // see http://brettterpstra.com/2012/12/08/a-useful-caps-lock-key/
#define LSG_T(kc) MT(MOD_LSFT | MOD_LGUI, kc)                        // Left Shift + GUI
#define SGUI_T(kc) LSG_T(kc)
#define SCMD_T(kc) LSG_T(kc)
#define SWIN_T(kc) LSG_T(kc)
#define LAG_T(kc) MT(MOD_LALT | MOD_LGUI, kc) // Left Alt + GUI
#define RSG_T(kc) MT(MOD_RSFT | MOD_RGUI, kc) // Right Shift + GUI
#define RAG_T(kc) MT(MOD_RALT | MOD_RGUI, kc) // Right Alt + GUI
#define LCA_T(kc) MT(MOD_LCTL | MOD_LALT, kc) // Left Control + Alt
#define LSA_T(kc) MT(MOD_LSFT | MOD_LALT, kc) // Left Shift + Alt
#define RSA_T(kc) MT(MOD_RSFT | MOD_RALT, kc) // Right Shift + Alt
#define RCS_T(kc) MT(MOD_RCTL | MOD_RSFT, kc) // Right Control + Shift
#define SAGR_T(kc) RSA_T(kc)

#define ALL_T(kc) HYPR_T(kc)

// Dedicated keycode versions for Hyper and Meh, if you want to use them as standalone keys rather than mod-tap
#define KC_HYPR HYPR(KC_NO)
#define KC_MEH MEH(KC_NO)

// Unicode aliases
// UNICODE_ENABLE - Allows Unicode input up to 0x7FFF
#define UC(c) (QK_UNICODE | (c))
// UNICODEMAP_ENABLE - Allows Unicode input up to 0x10FFFF, requires unicode_map
#define X(i) (QK_UNICODEMAP | (i))
#define XP(i, j) (QK_UNICODEMAP_PAIR | ((i)&0x7F) | (((j)&0x7F) << 7)) // 127 max i and j

// Swap Hands
#define SH_T(kc) (QK_SWAP_HANDS | (kc))
#define SH_TG (QK_SWAP_HANDS | OP_SH_TOGGLE)
#define SH_TT (QK_SWAP_HANDS | OP_SH_TAP_TOGGLE)
#define SH_OS (QK_SWAP_HANDS | OP_SH_ONESHOT)
#define SH_MON (QK_SWAP_HANDS | OP_SH_ON_OFF)
#define SH_MOFF (QK_SWAP_HANDS | OP_SH_OFF_ON)
#define SH_ON (QK_SWAP_HANDS | OP_SH_ON)
#define SH_OFF (QK_SWAP_HANDS | OP_SH_OFF)

// TODO: migrate to DD
// Sequencer
#include "sequencer.h"

#define SQ_ON 0x7200
#define SQ_OFF 0x7201
#define SQ_TOG 0x7202

#define SQ_TMPD 0x7203
#define SQ_TMPU 0x7204

#define SQ_RESD 0x7205
#define SQ_RESU 0x7206

#define SQ_SALL 0x7207
#define SQ_SCLR 0x7208

#define SEQUENCER_STEP_MIN 0x7210
#define SEQUENCER_STEP_MAX (SEQUENCER_STEP_MIN + SEQUENCER_STEPS)

#define SEQUENCER_RESOLUTION_MIN (SEQUENCER_STEP_MAX + 1)
#define SEQUENCER_RESOLUTION_MAX (SEQUENCER_RESOLUTION_MIN + SEQUENCER_RESOLUTIONS)

#define SEQUENCER_TRACK_MIN (SEQUENCER_RESOLUTION_MAX + 1)
#define SEQUENCER_TRACK_MAX (SEQUENCER_TRACK_MIN + SEQUENCER_TRACKS)

#define SQ_S(n) (n < SEQUENCER_STEPS ? SEQUENCER_STEP_MIN + n : KC_NO)
#define SQ_R(n) (n < SEQUENCER_RESOLUTIONS ? SEQUENCER_RESOLUTION_MIN + n : KC_NO)
#define SQ_T(n) (n < SEQUENCER_TRACKS ? SEQUENCER_TRACK_MIN + n : KC_NO)

// TODO: remove?
// #include "quantum_keycodes_legacy.h"
