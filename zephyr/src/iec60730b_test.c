/*
 * Copyright 2025 NXP
 * SPDX-License-Identifier: Apache-2.0
 */

#include <iec60730b.h>
#include <iec60730b_core.h>
#include "iec60730b_test.h"


#ifdef CONFIG_IEC60730B_TEST_DIO
/* Test digital input pin for IEC 60730 Class B compliance */
int iec60730b_test_dio_input(const struct device *port, gpio_pin_t pin, bool pin_expected_value)
{
#ifdef CONFIG_GPIO_MCUX
    fs_dio_test_t tested_pin;

    /* GPIO configuration from zephyr/drivers/gpio/gpio_mcux.c */
    struct gpio_mcux_config {
        struct gpio_driver_config common;
        GPIO_Type *gpio_base;
        PORT_Type *port_base;
        unsigned int flags;
        uint32_t port_no;
    };

    tested_pin.gpio = (uint32_t)((struct gpio_mcux_config *) port->config)->gpio_base;
    tested_pin.pinNum = pin;

    if (FS_DIO_Input(&tested_pin, (bool_t)pin_expected_value) != FS_PASS) {
        return IEC60730B_TEST_DIO_ERROR;
    }
#else /* Use Zephyr general API, as FS_DIO_Input() is not implemented for other modules. */
    int pin_value = gpio_pin_get_raw(port, pin);

    if (pin_value != (int)pin_expected_value) {
        return IEC60730B_TEST_DIO_ERROR;
    }
#endif
    return IEC60730B_TEST_OK;
}
#endif /* CONFIG_IEC60730B_TEST_DIO */
