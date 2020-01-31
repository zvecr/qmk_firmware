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

typedef struct _transport_buffer_t {
    uint8_t      smatrix_change_count;
    matrix_row_t mmatrix[ROWS_PER_HAND];
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
} transport_buffer_t;

// TODO: add method to set transport_data on i2c_slave rather than this pointer stuff
#if defined(USE_I2C)
static transport_buffer_t *const transport_data = (transport_buffer_t *)i2c_slave_reg;
#else
static transport_buffer_t transport_data = {0};
#endif

uint8_t volatile dummy = 0;
uint8_t volatile status_backlight = 0;
uint8_t volatile status_rgblight = 0;

enum transaction_id {
    GET_SLAVE_MATRIX,
    GET_SLAVE_STATUS,
    PUT_MASTER_GET_SLAVE_STATUS,
#ifdef BACKLIGHT_ENABLE
    PUT_BACKLIGHT,
#endif
#if defined(RGBLIGHT_ENABLE) && defined(RGBLIGHT_SPLIT)
    PUT_RGBLIGHT,
#endif
#ifdef ENCODER_ENABLE
    GET_ENCODER
#endif
};

SSTD_t transactions[] = {
    [GET_SLAVE_MATRIX] = 
        {
            (uint8_t *)&dummy,
            0,
            NULL,
            sizeof(transport_data.smatrix),
            (uint8_t *)transport_data.smatrix
        },
    [GET_SLAVE_STATUS] =
        {
            (uint8_t *)&dummy,
            0,
            NULL,
            sizeof(transport_data.smatrix_change_count),
            &transport_data.smatrix_change_count
        },
    [PUT_MASTER_GET_SLAVE_STATUS] =
        {
            (uint8_t *)&dummy,
            sizeof(transport_data.mmatrix),
            (uint8_t *)transport_data.mmatrix,
            sizeof(transport_data.smatrix_change_count),
            &transport_data.smatrix_change_count
        },
#ifdef BACKLIGHT_ENABLE
    [PUT_BACKLIGHT] =
        {
            (uint8_t *)&dummy,
            sizeof(transport_data.mmatrix),
            (uint8_t *)transport_data.mmatrix,
            sizeof(transport_data.smatrix_change_count),
            &transport_data.smatrix_change_count
        },
#endif
#if defined(RGBLIGHT_ENABLE) && defined(RGBLIGHT_SPLIT)
    [PUT_RGBLIGHT] =
        {
            (uint8_t *)&dummy,
            sizeof(transport_data.rgblight_sync),
            (uint8_t *)&transport_data.rgblight_sync,
            0,
            NULL  // no slave to master transfer
        },
#endif
#ifdef ENCODER_ENABLE
    [GET_ENCODER] = 
        {
            (uint8_t *)&dummy,
            0,
            NULL,
            sizeof(transport_data.encoder_state),
            (uint8_t *)transport_data.encoder_state
        },
#endif
};

void transport_master_init(void) { soft_serial_initiator_init(transactions, TID_LIMIT(transactions)); }

void transport_slave_init(void) { soft_serial_target_init(transactions, TID_LIMIT(transactions)); }

#if defined(BACKLIGHT_ENABLE)

// backlight synchronization information communication.

void transport_backlight_master(void) {
    uint8_t level = is_backlight_enabled() ? get_backlight_level() : 0;
    if (level != transport_data.backlight_level) {
        if (soft_serial_transaction(PUT_BACKLIGHT) == TRANSACTION_END) {
            transport_data.backlight_level = level;
        }
    }
}

void transport_backlight_slave(void) {
    backlight_set(transport_data.backlight_level);
}

#else
#    define transport_backlight_master()
#    define transport_backlight_slave()
#endif

#if defined(RGBLIGHT_ENABLE) && defined(RGBLIGHT_SPLIT)

// rgblight synchronization information communication.

void transport_rgblight_master(void) {
    if (rgblight_get_change_flags()) {
        rgblight_get_syncinfo((rgblight_syncinfo_t *)&transport_data.rgblight_sync);
        if (soft_serial_transaction(PUT_RGBLIGHT) == TRANSACTION_END) {
            rgblight_clear_change_flags();
        }
    }
}

void transport_rgblight_slave(void) {
    if (transport_data.rgblight_sync.change_flags != 0) {
        rgblight_update_sync((rgblight_syncinfo_t *)&transport_data.rgblight_sync, false);
        transport_data.rgblight_sync.change_flags = 0;
    }
}

#else
#    define transport_rgblight_master()
#    define transport_rgblight_slave()
#endif

#if defined(ENCODER_ENABLE)

// encoder synchronization information communication.

void transport_encoder_master(void) {
    if (soft_serial_transaction(GET_ENCODER) == TRANSACTION_END) {
        encoder_update_raw((uint8_t *)transport_data.encoder_state);
    }
}

void transport_encoder_slave(void) {
    encoder_state_raw((uint8_t *)transport_data.encoder_state);
}

#else
#    define transport_encoder_master()
#    define transport_encoder_slave()
#endif

bool transport_master(matrix_row_t matrix[]) {
#ifndef SERIAL_USE_MULTI_TRANSACTION
    if (soft_serial_transaction() != TRANSACTION_END) {
        return false;
    }
#else
    transport_backlight_master();
    transport_rgblight_master();
    transport_encoder_master();
    if (soft_serial_transaction(GET_SLAVE_MATRIX) != TRANSACTION_END) {
        return false;
    }
#endif

    // TODO:  if MATRIX_COLS > 8 change to unpack()
    for (int i = 0; i < ROWS_PER_HAND; ++i) {
        matrix[i] = transport_data.smatrix[i];
    }

    return true;
}

void transport_slave(matrix_row_t matrix[]) {
    transport_backlight_slave();
    transport_rgblight_slave();
    transport_encoder_slave();

    // TODO: if MATRIX_COLS > 8 change to pack()
    for (int i = 0; i < ROWS_PER_HAND; ++i) {
        transport_data.smatrix[i] = matrix[i];
    }
}

// GLUE I2C onto serial interface
#if defined(USE_I2C)
#    include "i2c_master.h"
#    include "i2c_slave.h"

#    ifndef SLAVE_I2C_ADDRESS
#        define SLAVE_I2C_ADDRESS 0x32
#    endif

#    define TIMEOUT 100

static SSTD_t *Transaction_table      = NULL;
static uint8_t Transaction_table_size = 0;

#    define get_offset(ptr) (ptr - (uint8_t *)&transport_data)

void soft_serial_initiator_init(SSTD_t *sstd_table, int sstd_table_size) {
    Transaction_table      = sstd_table;
    Transaction_table_size = (uint8_t)sstd_table_size;

    i2c_init();
}

void soft_serial_target_init(SSTD_t *sstd_table, int sstd_table_size) {
    // todo
    i2c_slave_init(SLAVE_I2C_ADDRESS);
}

#    ifndef SERIAL_USE_MULTI_TRANSACTION
int soft_serial_transaction(void) {
    int sstd_index = 0;
#    else
int soft_serial_transaction(int sstd_index) {
#    endif

    if (sstd_index > Transaction_table_size) return TRANSACTION_TYPE_ERROR;
    SSTD_t *trans = &Transaction_table[sstd_index];

    if (trans->initiator2target_buffer_size) {
        if (i2c_writeReg(SLAVE_I2C_ADDRESS, get_offset(trans->initiator2target_buffer), trans->initiator2target_buffer, trans->initiator2target_buffer_size, TIMEOUT) >= 0) {
            dprintf("i2c_writeReg NO_RESPONSE\n");
            return TRANSACTION_NO_RESPONSE;
        }
    }

    if (trans->target2initiator_buffer_size) {
        if (i2c_readReg(SLAVE_I2C_ADDRESS, get_offset(trans->target2initiator_buffer), trans->target2initiator_buffer, trans->target2initiator_buffer_size, TIMEOUT) >= 0) {
            dprintf("i2c_readReg NO_RESPONSE\n");
            return TRANSACTION_NO_RESPONSE;
        }
    }

    return TRANSACTION_END;
}
#endif
