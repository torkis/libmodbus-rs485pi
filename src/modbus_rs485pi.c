//
// Created by Torkis on 2020.02.08.
//

#include "modbus_rs485pi.h"
#include "gpio.h"

#include <stdint.h>
#include <string.h>
#include <sys/types.h>

// Physical Memory Allocation, from raspberrypi/userland demo.
#include "mailbox.h"

// maximum number of RTU handlers, RPI4 has 5 UARTS
#define RTU_MAX        5

typedef struct {
    modbus_t *ctx;
    u_int8_t rts_pin;
} gpio_handler;

// this is faster than a map or whatever because we have small number of items
static gpio_handler g_gpio_handlers[RTU_MAX] = {0};

static volatile uint32_t *g_gpio_reg = NULL;

static inline gpio_handler *findHandler(modbus_t *ctx) {
    gpio_handler *pHandler = g_gpio_handlers;
    for (int i = 0; i < RTU_MAX; i++, pHandler++) {
        if (pHandler->ctx == ctx) {
            return pHandler;
        }
    }

    return NULL;
}

static void modbus_rtu_pi_set_rts(modbus_t *ctx, int on) {
    gpio_handler *pHandler = findHandler(ctx);
    if (pHandler != NULL) {
        set_gpio_output(g_gpio_reg, pHandler->rts_pin, on);
        modbus_flush(ctx);
    }
}

MODBUS_API modbus_t *modbus_new_rtu_rs485pi(const char *device, int baud,
                                            char parity, int data_bit, int stop_bit, char rts_mode, int rts_pin,
                                            int rts_delayus) {
    modbus_t *ctx = modbus_new_rtu(device, baud, parity, data_bit, stop_bit);
    if (ctx == NULL) {
        return NULL;
    }

    if (rts_mode == 'N') {
        return ctx;
    }

    if (!g_gpio_reg) {
        g_gpio_reg = get_gpio_reg();
        if (!g_gpio_reg) {
            return ctx;
        }
    }

    // modbus_rtu_set_serial_mode(ctx, MODBUS_RTU_RS485);
    modbus_rtu_set_rts(ctx, rts_mode == 'U' ? MODBUS_RTU_RTS_UP : MODBUS_RTU_RTS_DOWN);

    // find free slot for gpio
    gpio_handler *pHandler = g_gpio_handlers;
    for (int i = 0; i < RTU_MAX; i++, pHandler++) {
        if (!pHandler->ctx) {
            init_gpio_output(g_gpio_reg, rts_pin);
            set_gpio_output(g_gpio_reg, rts_pin, rts_mode != 'U');

            modbus_rtu_set_custom_rts(ctx, &modbus_rtu_pi_set_rts);
            if (rts_delayus > 0) {
                modbus_rtu_set_rts_delay(ctx, rts_delayus);
            }

            pHandler->ctx = ctx;
            pHandler->rts_pin = rts_pin;

            return ctx;
        }
    }

    // not free gpio slot
    return ctx;
}

MODBUS_API void modbus_free_rtu_rs485pi(modbus_t *ctx) {
    gpio_handler *pHandler = findHandler(ctx);
    if (pHandler != NULL) {
        pHandler->ctx = NULL;
    }
}
