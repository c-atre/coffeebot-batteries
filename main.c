/*
 * Copyright (C) 2022 Martine S. Lenders <mail@martine-lenders.eu>
 *
 * This file is subject to the terms and conditions of the GNU Lesser General
 * Public License v2.1. See the file LICENSE in the top level directory for more
 * details.
 */

#include "periph/gpio.h"
#include "shell.h"
#include "ztimer.h"

#define LED_0_PIN           (GPIO_PIN(1, 9))
#define LED_1_PIN           (GPIO_PIN(0, 8))
#define LED_2_PIN           (GPIO_PIN(0, 6))

#define BTN_0_PIN           (GPIO_PIN(0, 27))
#define BTN_1_PIN           (GPIO_PIN(0, 26))

#define DEBOUNCE_INTERVAL   (2)

static ztimer_now_t _last_btn0, _last_btn1;
static int _led_count = 0;
static gpio_t _leds[] = {
    LED_0_PIN,
    LED_1_PIN,
    LED_2_PIN,
};


static void _btn_cb(void *arg)
{
    ztimer_now_t now = ztimer_now(ZTIMER_MSEC);
    int btn = (int)arg;

    switch (btn) {
    case BTN_0_PIN:
        if ((now - _last_btn0) >= DEBOUNCE_INTERVAL) {
            _last_btn0 = ztimer_now(ZTIMER_MSEC);
            if (--_led_count < 0) {
                _led_count = 0;
            }
        }
        break;
    case BTN_1_PIN:
        if ((now - _last_btn1) >= DEBOUNCE_INTERVAL) {
            _last_btn1 = ztimer_now(ZTIMER_MSEC);
            if (++_led_count > (int)sizeof(_leds)) {
                _led_count = sizeof(_leds);
            }
        }
        break;
    default:
        break;
    }
    for (unsigned i = 0; i < sizeof(_leds); i++) {
        gpio_clear(_leds[i]);
    }
    for (int i = 0; i < _led_count; i++) {
        gpio_set(_leds[i]);
    }
}


int main(void)
{
    static char line_buf[SHELL_DEFAULT_BUFSIZE];

    for (unsigned i = 0; i < sizeof(_leds); i++) {
        if (gpio_init(_leds[i], GPIO_OUT) < 0) {
            printf("Unable to initialize LED_%d", i - 1);
            return 1;
        }
    }
    if (gpio_init_int(BTN_0_PIN, GPIO_IN_PD, GPIO_FALLING, _btn_cb, (void *)BTN_0_PIN) < 0) {
        puts("Unable to initialize BTN0");
        return 1;
    }
    if (gpio_init_int(BTN_1_PIN, GPIO_IN_PD, GPIO_FALLING, _btn_cb, (void *)BTN_1_PIN) < 0) {
        puts("Unable to initialize BTN");
        return 1;
    }
    _last_btn0 = ztimer_now(ZTIMER_MSEC);
    _last_btn1 = ztimer_now(ZTIMER_MSEC);
    shell_run(NULL, line_buf, SHELL_DEFAULT_BUFSIZE);

    return 0;
}
