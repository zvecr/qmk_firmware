#include <string.h>
#include "quantum.h"
#include "matrix.h"

#define ROWS_PER_HAND (MATRIX_ROWS / 2)

#ifdef ENCODER_ENABLE
static pin_t encoders_pad[] = ENCODERS_PAD_A;
#    define NUMBER_OF_ENCODERS (sizeof(encoders_pad) / sizeof(pin_t))
#endif

typedef struct _slave_buffer_t {
    matrix_row_t smatrix[ROWS_PER_HAND];
#ifdef BACKLIGHT_ENABLE
    uint8_t backlight_level;
#endif
#if defined(RGBLIGHT_ENABLE) && defined(RGBLIGHT_SPLIT)
    rgblight_syncinfo_t rgblight_sync;
#endif
#ifdef ENCODER_ENABLE
    uint8_t encoder_state[NUMBER_OF_ENCODERS];
#endif
} slave_buffer_t;

#define TRANSPORT_KEYMAP_ID offsetof(slave_buffer_t, smatrix)
#define TRANSPORT_BACKLIGHT_ID offsetof(slave_buffer_t, backlight_level)
#define TRANSPORT_RGB_ID offsetof(slave_buffer_t, rgblight_sync)
#define TRANSPORT_ENCODER_ID offsetof(slave_buffer_t, encoder_state)

// forward declare transport specific
static slave_buffer_t *const slave_buffer;

bool transport_write(uint8_t id, const uint8_t *data, uint16_t length);
bool transport_read(uint8_t id, uint8_t *data, uint16_t length);

//****************************************************************************

bool transport_master(matrix_row_t matrix[]) {
    // Get rows from other half
    transport_read(TRANSPORT_KEYMAP_ID, (void *)matrix, sizeof(slave_buffer->smatrix));

    // write backlight info
#ifdef BACKLIGHT_ENABLE
    uint8_t level = is_backlight_enabled() ? get_backlight_level() : 0;
    if (level != slave_buffer->backlight_level) {
        if (transport_write(TRANSPORT_BACKLIGHT_ID, (void *)&level, sizeof(level))) {
            slave_buffer->backlight_level = level;
        }
    }
#endif

#if defined(RGBLIGHT_ENABLE) && defined(RGBLIGHT_SPLIT)
    if (rgblight_get_change_flags()) {
        rgblight_syncinfo_t rgblight_sync;
        rgblight_get_syncinfo(&rgblight_sync);
        if (transport_write(TRANSPORT_RGB_ID, (void *)&rgblight_sync, sizeof(rgblight_sync))) {
            rgblight_clear_change_flags();
        }
    }
#endif

#ifdef ENCODER_ENABLE
    transport_read(TRANSPORT_ENCODER_ID, (void *)slave_buffer->encoder_state, sizeof(slave_buffer->encoder_state));
    encoder_update_raw(slave_buffer->encoder_state);
#endif

    return true;
}

void transport_slave(matrix_row_t matrix[]) {
    // Copy matrix to buffer
    memcpy((void *)slave_buffer->smatrix, (void *)matrix, sizeof(slave_buffer->smatrix));

// Read Backlight Info
#ifdef BACKLIGHT_ENABLE
    backlight_set(slave_buffer->backlight_level);
#endif

#if defined(RGBLIGHT_ENABLE) && defined(RGBLIGHT_SPLIT)
    // Update the RGB with the new data
    if (slave_buffer->rgblight_sync.status.change_flags != 0) {
        rgblight_update_sync(&slave_buffer->rgblight_sync, false);
        slave_buffer->rgblight_sync.status.change_flags = 0;
    }
#endif

#ifdef ENCODER_ENABLE
    encoder_state_raw(slave_buffer->encoder_state);
#endif
}

//****************************************************************************

#if defined(USE_I2C)
#    include "i2c_master.h"
#    include "i2c_slave.h"

#    ifndef SLAVE_I2C_ADDRESS
#        define SLAVE_I2C_ADDRESS 0x32
#    endif
#    ifndef SLAVE_TIMEOUT
#        define SLAVE_TIMEOUT 100
#    endif

static slave_buffer_t *const slave_buffer = (slave_buffer_t *)i2c_slave_reg;

bool transport_write(uint8_t id, const uint8_t *data, uint16_t length) { return i2c_writeReg(SLAVE_I2C_ADDRESS, id, data, length, SLAVE_TIMEOUT) >= 0; }
bool transport_read(uint8_t id, uint8_t *data, uint16_t length) { return i2c_readReg(SLAVE_I2C_ADDRESS, id, data, length, SLAVE_TIMEOUT) >= 0; }

void transport_master_init(void) { i2c_init(); }
void transport_slave_init(void) { i2c_slave_init(SLAVE_I2C_ADDRESS); }
#else
#    include "serial.h"

static slave_buffer_t        serial_slave_buffer;
static slave_buffer_t *const slave_buffer = &serial_slave_buffer;

uint8_t volatile status0 = 0;

// clang-format off
SSTD_t transactions[] = {
    [TRANSPORT_KEYMAP_ID] =
        {
            (uint8_t *)&status0,
            0,
            NULL,
            sizeof(serial_slave_buffer.smatrix),
            (uint8_t *)&serial_slave_buffer.smatrix,
        },
#ifdef BACKLIGHT_ENABLE
    [TRANSPORT_BACKLIGHT_ID] =
        {
            (uint8_t *)&status0,
            sizeof(serial_slave_buffer.backlight_level),
            (uint8_t *)&serial_slave_buffer.backlight_level,
            0,
            NULL,
        },
#endif
#    if defined(RGBLIGHT_ENABLE) && defined(RGBLIGHT_SPLIT)
    [TRANSPORT_RGB_ID] =
        {
            (uint8_t *)&status0,
            sizeof(serial_slave_buffer.rgblight_sync),
            (uint8_t *)&serial_slave_buffer.rgblight_sync,
            0,
            NULL,
        },
#endif
#ifdef ENCODER_ENABLE
    [TRANSPORT_ENCODER_ID] =
        {
            (uint8_t *)&status0,
            0,
            NULL,
            sizeof(serial_slave_buffer.encoder_state),
            (uint8_t *)&serial_slave_buffer.encoder_state,
        },
#endif
};
// clang-format on

bool transport_write(uint8_t id, const uint8_t *data, uint16_t length) {
    // copy data to slave_buffer, transport_id -> serial_id, soft_serial_transaction

    memcpy((void *)&slave_buffer + id, data, length);

#    ifdef SERIAL_USE_MULTI_TRANSACTION
    if (soft_serial_transaction(id) == TRANSACTION_END) {
#    else
    if (soft_serial_transaction() == TRANSACTION_END) {
#    endif
        // already exists in data
        return true;
    }
    return false;
}
bool transport_read(uint8_t id, uint8_t *data, uint16_t length) {
    // transport_id -> serial_id, soft_serial_transaction, copy slave_buffer to data

#    ifdef SERIAL_USE_MULTI_TRANSACTION
    if (soft_serial_transaction(id) == TRANSACTION_END) {
#    else
    if (soft_serial_transaction() == TRANSACTION_END) {
#    endif
        // copy slave_buffer to data - done as part of transaction
        return true;
    }
    return false;
}

void transport_master_init(void) { soft_serial_initiator_init(transactions, TID_LIMIT(transactions)); }
void transport_slave_init(void) { soft_serial_target_init(transactions, TID_LIMIT(transactions)); }
#endif
