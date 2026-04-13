/*
 * Copyright 2025 NXP
 * SPDX-License-Identifier: Apache-2.0
 */

#include "iec60730b_test.h"

#ifdef CONFIG_IEC60730B_TEST_DIO

/* Test digital input pin for IEC 60730 Class B compliance */
int iec60730b_test_dio_input(const struct device *port, gpio_pin_t pin, bool pin_expected_value)
{
	int pin_value = gpio_pin_get_raw(port, pin);

	if (pin_value != (int)pin_expected_value) {
		return IEC60730B_TEST_DIO_ERROR;
	}

	return IEC60730B_TEST_OK;
}

/* Test digital output pin for IEC 60730 Class B compliance */
int iec60730b_test_dio_output(const struct device *port, gpio_pin_t pin)
{
	int pin_value = gpio_pin_get_raw(port, pin); /* Save the pin value before test */

	if (pin_value < 0) {
		return IEC60730B_TEST_DIO_ERROR;
	}

	/* Set pin to 1 */
	if (gpio_pin_set_raw(port, pin, 1) < 0) {
		return IEC60730B_TEST_DIO_ERROR;
	}

	/* Check if pin is set */
	if (gpio_pin_get_raw(port, pin) != 1) {
		return IEC60730B_TEST_DIO_ERROR;
	}

	/* Set pin to 0 */
	if (gpio_pin_set_raw(port, pin, 0) < 0) {
		return IEC60730B_TEST_DIO_ERROR;
	}

	/* Check if pin is cleared = short to VCC circuit */
	if (gpio_pin_get_raw(port, pin) != 0) {
		return IEC60730B_TEST_DIO_ERROR;
	}

	/* Set original value of the pin */
	if (gpio_pin_set_raw(port, pin, pin_value) < 0) {
		return IEC60730B_TEST_DIO_ERROR;
	}

	return IEC60730B_TEST_OK;
}

#endif /* CONFIG_IEC60730B_TEST_DIO */
