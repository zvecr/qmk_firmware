// Copyright 2022 QMK
// SPDX-License-Identifier: GPL-2.0-or-later

#include "isrpipe.h"
#include "isrpipe/read_timeout.h"

#include "usb/usbus.h"
#include "usb/usbus/hid.h"
#include "usb/usbus/hid_io.h"
#include "usb/usbus/control.h"

#include "usb_driver.h"
#include "report.h"

// Disable any logging - lines are kept to minimise diffs
#undef DEBUG
#define DEBUG(...)

// **********************************************
// Partially based on:
//   lib/riot/sys/include/usb/usbus/hid.h

/**
 * @brief Composite HID device context struct forward declaration
 */
typedef struct comp_hid_device comp_hid_device_t;

/**
 * @brief HID data callback.
 *
 * Callback for received data from the USB host
 *
 * @param[in]   hid     HID handler context
 * @param[in]   data    ptr to the data
 * @param[in]   len     Length of the received data
 */
typedef void (*comp_hid_cb_t)(comp_hid_device_t *hid, uint8_t *data, size_t len);

/**
 * @brief Composite HID device context struct
 */
struct comp_hid_device {
    usbus_handler_t        handler_ctrl; /**< control handler */
    usbus_interface_t      iface;        /**< HID interface */
    usbus_endpoint_t *     ep_out;       /**< OUT endpoint */
    usbus_endpoint_t *     ep_in;        /**< IN  endpoint */
    usbus_descr_gen_t      hid_descr;    /**< HID descriptor generator */
    usbus_t *              usbus;        /**< USBUS reference */
    comp_hid_device_conf_t conf;         /**< stashed config */
    size_t                 occupied;     /**< Number of bytes for the host */
    comp_hid_cb_t          cb;           /**< Callback for data handlers */
    event_t                tx_ready;     /**< Transmit ready event */
    mutex_t                in_lock;      /**< mutex used for locking hid send */

    /**
     * @brief Host to device data buffer
     */
    usbdev_ep_buf_t out_buf[CONFIG_USBUS_HID_INTERRUPT_EP_SIZE];

    /**
     * @brief Device to host data buffer
     */
    usbdev_ep_buf_t in_buf[CONFIG_USBUS_HID_INTERRUPT_EP_SIZE];
};

// **********************************************
// Partially based on:
//   lib/riot/sys/usb/usbus/hid/hid.c

static void _init(usbus_t *usbus, usbus_handler_t *handler);
static void _event_handler(usbus_t *usbus, usbus_handler_t *handler, usbus_event_usb_t event);
static int  _control_handler(usbus_t *usbus, usbus_handler_t *handler, usbus_control_request_state_t state, usb_setup_t *setup);
static void _transfer_handler(usbus_t *usbus, usbus_handler_t *handler, usbdev_ep_t *ep, usbus_event_transfer_t event);

static void _handle_tx_ready(event_t *ev);

static const usbus_handler_driver_t hid_driver = {
    .init             = _init,            //
    .event_handler    = _event_handler,   //
    .control_handler  = _control_handler, //
    .transfer_handler = _transfer_handler //
};

static size_t _gen_hid_descriptor(usbus_t *usbus, void *arg);

static const usbus_descr_gen_funcs_t _hid_descriptor = {
    .fmt_post_descriptor = _gen_hid_descriptor,                   //
    .len                 = {.fixed_len = sizeof(usb_desc_hid_t)}, //
    .len_type            = USBUS_DESCR_LEN_FIXED                  //
};

static size_t _gen_hid_descriptor(usbus_t *usbus, void *arg) {
    comp_hid_device_t *hid_dev = arg;

    usb_desc_hid_t hid_desc;
    hid_desc.length        = sizeof(usb_desc_hid_t);
    hid_desc.desc_type     = USB_HID_DESCR_HID;
    hid_desc.bcd_hid       = USB_HID_VERSION_BCD;
    hid_desc.country_code  = USB_HID_COUNTRY_CODE_NOTSUPPORTED;
    hid_desc.num_descrs    = 0x01;
    hid_desc.report_type   = USB_HID_DESCR_REPORT;
    hid_desc.report_length = hid_dev->conf.report_desc_size;

    usbus_control_slicer_put_bytes(usbus, (uint8_t *)&hid_desc, sizeof(hid_desc));
    return sizeof(usb_desc_hid_t);
}

static void _handle_tx_ready(event_t *ev) {
    comp_hid_device_t *hid = container_of(ev, comp_hid_device_t, tx_ready);

    usbdev_ep_xmit(hid->ep_in->ep, hid->in_buf, hid->occupied);
}

static void usbdrv_init_interface(usbus_t *usbus, comp_hid_device_t *hid, comp_hid_cb_t cb, comp_hid_device_conf_t *conf) {
    hid->usbus = usbus;
    mutex_init(&hid->in_lock);
    hid->handler_ctrl.driver = &hid_driver;
    hid->cb                  = cb;

    memcpy(&hid->conf, conf, sizeof(comp_hid_device_conf_t));

    DEBUG("hid_init: %d %d \n", report_desc_size, report_desc[0]);
    usbus_register_event_handler(usbus, &hid->handler_ctrl);
}

static void _init(usbus_t *usbus, usbus_handler_t *handler) {
    DEBUG("USB_HID: initialization\n");
    comp_hid_device_t *hid = container_of(handler, comp_hid_device_t, handler_ctrl);

    hid->tx_ready.handler = _handle_tx_ready;

    hid->hid_descr.next  = NULL;
    hid->hid_descr.funcs = &_hid_descriptor;
    hid->hid_descr.arg   = hid;

    /*
       Configure Interface as USB_HID interface
     */
    hid->iface.class     = USB_CLASS_HID;
    hid->iface.subclass  = hid->conf.subclass;
    hid->iface.protocol  = hid->conf.protocol;
    hid->iface.descr_gen = &hid->hid_descr;
    hid->iface.handler   = handler;

    /* IN endpoint to send data to host */
    hid->ep_in = usbus_add_endpoint(usbus, &hid->iface, USB_EP_TYPE_INTERRUPT, USB_EP_DIR_IN, hid->conf.ep_size);

    /* interrupt endpoint polling rate in ms */
    hid->ep_in->interval = hid->conf.interval;

    usbus_enable_endpoint(hid->ep_in);

    usbus_add_interface(usbus, &hid->iface);
}

report_keyboard_t keyboard_report_sent = {{0}};
report_mouse_t    mouse_report_sent    = {0};

union {
    uint8_t           report_id;
    report_keyboard_t keyboard;
#ifdef EXTRAKEY_ENABLE
    report_extra_t extra;
#endif
#ifdef MOUSE_ENABLE
    report_mouse_t mouse;
#endif
#ifdef DIGITIZER_ENABLE
    report_digitizer_t digitizer;
#endif
#ifdef JOYSTICK_ENABLE
    report_joystick_t joystick;
#endif
} universal_report_blank = {0};

uint8_t keyboard_idle __attribute__((aligned(2)))     = 0;
uint8_t keyboard_protocol __attribute__((aligned(2))) = 1;

static uint8_t keyboard_led_state = 0;
uint8_t        usbdrv_keyboard_leds(void) {
    return keyboard_led_state;
}

static void _event_handler(usbus_t *usbus, usbus_handler_t *handler, usbus_event_usb_t event) {
    (void)usbus;
    (void)handler;

    switch (event) {
        case USBUS_EVENT_USB_RESET:
            keyboard_protocol = 1;
            break;
        default:
            DEBUG("USB HID unhandled event: 0x%x\n", event);
            break;
    }
}

static int _control_handler(usbus_t *usbus, usbus_handler_t *handler, usbus_control_request_state_t state, usb_setup_t *setup) {
    comp_hid_device_t *hid = (comp_hid_device_t *)handler;

    DEBUG("USB_HID: request: %d type: %d value: %d length: %d state: %d \n", setup->request, setup->type, setup->value >> 8, setup->length, state);

    /* Requests defined in USB HID 1.11 spec section 7 */
    switch (setup->request) {
        case USB_SETUP_REQ_GET_DESCRIPTOR: {
            uint8_t desc_type = setup->value >> 8;
            if (desc_type == USB_HID_DESCR_REPORT) {
                usbus_control_slicer_put_bytes(usbus, hid->conf.report_desc, hid->conf.report_desc_size);
                // Bodge to get windows to get past "IRP USBD_STATUS: USBD_STATUS_CANCELED"
                uint8_t status[2] = {0};
                usbus_control_slicer_put_bytes(usbus, status, sizeof(status));
            } else if (desc_type == USB_HID_DESCR_HID) {
                _gen_hid_descriptor(usbus, hid);
            }
            break;
        }
        case USB_HID_REQUEST_GET_REPORT: {
            uint8_t report_id = setup->value & 0xFF;
            switch (setup->index) {
#ifndef KEYBOARD_SHARED_EP
                case KEYBOARD_INTERFACE:
                    usbus_control_slicer_put_bytes(usbus, (uint8_t *)&keyboard_report_sent, KEYBOARD_REPORT_SIZE);
                    break;
#endif
#if defined(MOUSE_ENABLE) && !defined(MOUSE_SHARED_EP)
                case MOUSE_INTERFACE:
                    usbus_control_slicer_put_bytes(usbus, (uint8_t *)&mouse_report_sent, sizeof(mouse_report_sent));
                    break;
#endif
#ifdef SHARED_EP_ENABLE
                case SHARED_INTERFACE:
#    ifdef KEYBOARD_SHARED_EP
                    if (report_id == REPORT_ID_KEYBOARD) {
                        usbus_control_slicer_put_bytes(usbus, (uint8_t *)&keyboard_report_sent, KEYBOARD_REPORT_SIZE);
                        break;
                    }
#    endif
#    ifdef MOUSE_SHARED_EP
                    if (report_id == REPORT_ID_MOUSE) {
                        usbus_control_slicer_put_bytes(usbus, (uint8_t *)&mouse_report_sent, sizeof(mouse_report_sent));
                        break;
                    }
#    endif
#endif /* SHARED_EP_ENABLE */
                default:
                    universal_report_blank.report_id = report_id;
                    usbus_control_slicer_put_bytes(usbus, (uint8_t *)&universal_report_blank, setup->length);
                    break;
            }
            break;
        }
        case USB_HID_REQUEST_GET_IDLE:
            if (setup->index == KEYBOARD_INTERFACE) {
                usbus_control_slicer_put_bytes(usbus, &keyboard_idle, sizeof(keyboard_idle));
            }
            break;
        case USB_HID_REQUEST_GET_PROTOCOL:
            if (setup->index == KEYBOARD_INTERFACE) {
                usbus_control_slicer_put_bytes(usbus, &keyboard_protocol, sizeof(keyboard_protocol));
            }
            break;
        case USB_HID_REQUEST_SET_REPORT:
            if ((state == USBUS_CONTROL_REQUEST_STATE_OUTDATA)) {
                size_t   size = 0;
                uint8_t *data = usbus_control_get_out_data(usbus, &size);
                if (setup->index == KEYBOARD_INTERFACE) {
                    if (size == 2) {
                        uint8_t report_id = data[0];
                        if (report_id == REPORT_ID_KEYBOARD || report_id == REPORT_ID_NKRO) {
                            keyboard_led_state = data[1];
                        }
                    } else if (size == 1) {
                        keyboard_led_state = data[0];
                    }
                } else {
                    if (size > 0) {
                        hid->cb(hid, data, size);
                    }
                }
            }
            break;
        case USB_HID_REQUEST_SET_IDLE:
            if (setup->index == KEYBOARD_INTERFACE) {
                keyboard_idle = ((setup->value & 0xFF00) >> 8);
            }
            break;
        case USB_HID_REQUEST_SET_PROTOCOL:
            if (setup->index == KEYBOARD_INTERFACE) {
                keyboard_protocol = (setup->value & 0xFF);
            }
            break;
        default:
            DEBUG("USB_HID: unknown request %d \n", setup->request);
            return -1;
    }
    return 1;
}

static void _transfer_handler(usbus_t *usbus, usbus_handler_t *handler, usbdev_ep_t *ep, usbus_event_transfer_t event) {
    (void)usbus;
    (void)event;
    DEBUG("USB_HID: transfer_handler\n");

    comp_hid_device_t *hid = (comp_hid_device_t *)handler;

    if ((ep->dir == USB_EP_DIR_IN) && (ep->type == USB_EP_TYPE_INTERRUPT)) {
        mutex_unlock(&hid->in_lock);
        hid->occupied = 0;
    } else if ((ep->dir == USB_EP_DIR_OUT) && (ep->type == USB_EP_TYPE_INTERRUPT)) {
        size_t len;
        usbdev_ep_get(ep, USBOPT_EP_AVAILABLE, &len, sizeof(size_t));
        if (len > 0) {
            hid->cb(hid, hid->out_buf, len);
        }
        usbdev_ep_xmit(ep, hid->out_buf, CONFIG_USBUS_HID_INTERRUPT_EP_SIZE);
    }
}

// **********************************************
// **********************************************
// Partially based on:
//   lib/riot/sys/usb/usbus/hid/hid_io.c

static uint8_t   _hid_rx_buf_mem[CONFIG_USBUS_HID_INTERRUPT_EP_SIZE];
static isrpipe_t _hid_stdio_isrpipe = ISRPIPE_INIT(_hid_rx_buf_mem);

static void _hid_rx_pipe(comp_hid_device_t *dev, uint8_t *data, size_t len) {
    (void)dev;
    isrpipe_write(&_hid_stdio_isrpipe, data, len);

    // if (_rx_cb) {
    //     _rx_cb(_rx_cb_arg);
    // }
#ifdef RAW_ENABLE
    if (dev->conf.id == RAW_INTERFACE) {
        void raw_hid_dump(uint8_t * data, uint8_t length);
        raw_hid_dump(data, len);
    }
#endif
#ifdef XAP_ENABLE
    if (dev->conf.id == XAP_INTERFACE) {
        void xap_dump(uint8_t * data, uint8_t length);
        xap_dump(data, len);
    }
#endif
}

static comp_hid_device_t hid_interfaces[TOTAL_INTERFACES];

usbus_t g_usbus;

void usbdrv_init(comp_hid_device_conf_t *config, size_t len) {
    usbdev_t *usbdev = usbdev_get_ctx(0);

    usbus_init(&g_usbus, usbdev);

    for (uint8_t id = 0; id < len; id++) {
        usbdrv_init_interface(&g_usbus, &hid_interfaces[id], _hid_rx_pipe, &config[id]);
    }

    static char _stack[USBUS_STACKSIZE];
    usbus_create(_stack, USBUS_STACKSIZE, USBUS_PRIO, USBUS_TNAME, &g_usbus);
}

size_t usbdrv_write_timeout(uint8_t id, const void *buffer, size_t len, uint32_t timeout) {
    comp_hid_device_t *hid = &hid_interfaces[id];

    uint8_t *buffer_ep = hid->in_buf;
    uint16_t max_size  = hid->ep_in->maxpacketsize;
    size_t   offset    = 0;

    while (len) {
        if (timeout == 0) {
            mutex_lock(&hid->in_lock);
        }
        // TODO: Validate timeout implementation???
        else if (ztimer_mutex_lock_timeout(ZTIMER_USEC, &hid->in_lock, timeout) != 0) {
            return 0;
        }

        if (len > max_size) {
            memmove(buffer_ep, (uint8_t *)buffer + offset, max_size);
            offset += max_size;
            hid->occupied = max_size;
            len -= max_size;
        } else {
            memmove(buffer_ep, (uint8_t *)buffer + offset, len);
            hid->occupied = len;
            len           = 0;
        }
        usbus_event_post(hid->usbus, &hid->tx_ready);
    }
    return len;
}

void usbdrv_write(uint8_t id, const void *buffer, size_t len) {
    usbdrv_write_timeout(id, buffer, len, 0);
}

// TODO: handle remote wakeup
//       This functionality is currently MIA within RIOT
#ifdef QMK_MCU_SAMD51
#    include "sam_usb.h"
void usbdrv_wake(void) {
    // mimic udc_remotewakeup/usb_device_send_remote_wake_up from lib/arm_atsam
    usbdev_t *         usbdev = usbdev_get_ctx(0);
    sam0_common_usb_t *dev    = (sam0_common_usb_t *)usbdev;

    uint8_t try = 5;
    while (2 != dev->config->device->FSMSTATUS.reg && try--) {
        dev->config->device->CTRLB.reg |= USB_DEVICE_CTRLB_UPRSM;
    }
}
#else
void usbdrv_wake(void) {
#    pragma message("remote wakeup not currently supported")
}
#endif
