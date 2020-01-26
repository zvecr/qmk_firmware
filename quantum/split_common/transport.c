#include <string.h>
#include <stddef.h>

#include "config.h"
#include "matrix.h"
#include "quantum.h"

// we currently reuse serial transaction structure for i2c as well
// TODO: move from serial to "common"
#include "serial.h"

#ifdef RGBLIGHT_ENABLE
#    include "rgblight.h"
#endif

#ifdef BACKLIGHT_ENABLE
#    include "backlight.h"
#endif

#ifdef ENCODER_ENABLE
#    include "encoder.h"
static pin_t encoders_pad[] = ENCODERS_PAD_A;
#    define NUMBER_OF_ENCODERS (sizeof(encoders_pad) / sizeof(pin_t))
#endif

#define ROWS_PER_HAND (MATRIX_ROWS / 2)

typedef struct {
    // TODO: if MATRIX_COLS > 8 change to uint8_t packed_matrix[] for pack/unpack
    matrix_row_t smatrix[ROWS_PER_HAND];

#ifdef ENCODER_ENABLE
    uint8_t encoder_state[NUMBER_OF_ENCODERS];
#endif
} s2m_buffer_t;

typedef struct {
#ifdef BACKLIGHT_ENABLE
    uint8_t backlight_level;
#endif
} m2s_buffer_t;

#if defined(RGBLIGHT_ENABLE) && defined(RGBLIGHT_SPLIT)
typedef struct {
    rgblight_syncinfo_t rgblight_sync;
} m2s_rgblight_t;
#endif

typedef struct
{
#if defined(RGBLIGHT_ENABLE) && defined(RGBLIGHT_SPLIT)
    m2s_rgblight_t m2s_rgblight;
    uint8_t status_rgblight;
#endif
    s2m_buffer_t s2m_buffer;
    m2s_buffer_t m2s_buffer;
    uint8_t status0;
} transport_data_t;

volatile transport_data_t transport_data = { };

enum transaction_id {
    GET_SLAVE_MATRIX = 0,
    PUT_RGBLIGHT,
};

SSTD_t transactions[] = {
    [GET_SLAVE_MATRIX] =
        {
            (uint8_t *)&transport_data.status0,
            sizeof(transport_data.m2s_buffer),
            (uint8_t *)&transport_data.m2s_buffer,
            sizeof(transport_data.s2m_buffer),
            (uint8_t *)&transport_data.s2m_buffer,
        },
#if defined(RGBLIGHT_ENABLE) && defined(RGBLIGHT_SPLIT)
    [PUT_RGBLIGHT] =
        {
            (uint8_t *)&transport_data.status_rgblight, sizeof(transport_data.m2s_rgblight), (uint8_t *)&transport_data.m2s_rgblight, 0, NULL  // no slave to master transfer
        },
#endif
};

void transport_master_init(void) { soft_serial_initiator_init(transactions, TID_LIMIT(transactions)); }

void transport_slave_init(void) { soft_serial_target_init(transactions, TID_LIMIT(transactions)); }

#if defined(RGBLIGHT_ENABLE) && defined(RGBLIGHT_SPLIT)

// rgblight synchronization information communication.

void transport_rgblight_master(void) {
    if (rgblight_get_change_flags()) {
        rgblight_get_syncinfo((rgblight_syncinfo_t *)&transport_data.m2s_rgblight.rgblight_sync);
        if (soft_serial_transaction(PUT_RGBLIGHT) == TRANSACTION_END) {
            rgblight_clear_change_flags();
        }
    }
}

void transport_rgblight_slave(void) {
    if (status_rgblight == TRANSACTION_ACCEPTED) {
        rgblight_update_sync((rgblight_syncinfo_t *)&transport_data.m2s_rgblight.rgblight_sync, false);
        status_rgblight = TRANSACTION_END;
    }
}

#else
#    define transport_rgblight_master()
#    define transport_rgblight_slave()
#endif

bool transport_master(matrix_row_t matrix[]) {
#ifndef SERIAL_USE_MULTI_TRANSACTION
    if (soft_serial_transaction() != TRANSACTION_END) {
        return false;
    }
#else
    transport_rgblight_master();
    if (soft_serial_transaction(GET_SLAVE_MATRIX) != TRANSACTION_END) {
        return false;
    }
#endif

    // TODO:  if MATRIX_COLS > 8 change to unpack()
    for (int i = 0; i < ROWS_PER_HAND; ++i) {
        matrix[i] = transport_data.s2m_buffer.smatrix[i];
    }

#ifdef BACKLIGHT_ENABLE
    // Write backlight level for slave to read
    transport_data.m2s_buffer.backlight_level = is_backlight_enabled() ? get_backlight_level() : 0;
#endif

#ifdef ENCODER_ENABLE
    encoder_update_raw((uint8_t *)transport_data.s2m_buffer.encoder_state);
#endif

    return true;
}

void transport_slave(matrix_row_t matrix[]) {
    transport_rgblight_slave();
    // TODO: if MATRIX_COLS > 8 change to pack()
    for (int i = 0; i < ROWS_PER_HAND; ++i) {
        transport_data.s2m_buffer.smatrix[i] = matrix[i];
    }
#ifdef BACKLIGHT_ENABLE
    backlight_set(transport_data.m2s_buffer.backlight_level);
#endif

#ifdef ENCODER_ENABLE
    encoder_state_raw((uint8_t *)transport_data.s2m_buffer.encoder_state);
#endif
}


// GLUE I2C onto serial interface
#if defined(USE_I2C)
#    include "i2c_master.h"
#    include "i2c_slave.h"

#    ifndef SLAVE_I2C_ADDRESS
#        define SLAVE_I2C_ADDRESS 0x32
#    endif

#    define TIMEOUT 100

static SSTD_t* Transaction_table      = NULL;
static uint8_t Transaction_table_size = 0;

#define get_offset(ptr) (ptr - (uint8_t *)&transport_data)

void soft_serial_initiator_init(SSTD_t* sstd_table, int sstd_table_size) {
    Transaction_table      = sstd_table;
    Transaction_table_size = (uint8_t)sstd_table_size;

    i2c_init();
}

void soft_serial_target_init(SSTD_t* sstd_table, int sstd_table_size) {
    Transaction_table      = sstd_table;
    Transaction_table_size = (uint8_t)sstd_table_size;

    i2c_slave_init(SLAVE_I2C_ADDRESS, (uint8_t *)&transport_data);
}

#ifndef SERIAL_USE_MULTI_TRANSACTION
int soft_serial_transaction(void) {
    int sstd_index = 0;
#else
int soft_serial_transaction(int sstd_index) {
#endif

    if (sstd_index > Transaction_table_size) return TRANSACTION_TYPE_ERROR;
    SSTD_t* trans = &Transaction_table[sstd_index];

    if (trans->initiator2target_buffer_size) {
        if(i2c_writeReg(SLAVE_I2C_ADDRESS, get_offset(trans->initiator2target_buffer), trans->initiator2target_buffer, trans->initiator2target_buffer_size, TIMEOUT) >= 0) {
            dprintf("i2c_transmit NO_RESPONSE\n");
            return TRANSACTION_NO_RESPONSE;
        }
    }

    if (trans->target2initiator_buffer_size) {
        if(i2c_readReg(SLAVE_I2C_ADDRESS, get_offset(trans->target2initiator_buffer), trans->target2initiator_buffer, trans->target2initiator_buffer_size, TIMEOUT) >= 0) {
            dprintf("i2c_transmit NO_RESPONSE\n");
            return TRANSACTION_NO_RESPONSE;
        }
    }

    return TRANSACTION_END;
}
#endif
