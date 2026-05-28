/*
 * Copyright 2026 NXP
 * SPDX-License-Identifier: Apache-2.0
 */

#include "iec60730b_test.h"

#include <zephyr/drivers/hwinfo.h>
#include <zephyr/drivers/watchdog.h>
#include <zephyr/drivers/counter.h>

#include <zephyr/logging/log.h>
#include <zephyr/logging/log_ctrl.h>
LOG_MODULE_REGISTER(test_wdog, CONFIG_IEC60730B_TEST_WDOG_LOG_LEVEL);

#ifdef CONFIG_IEC60730B_TEST_WDOG

/* Counter ticks measured during watchdog timeout period.
 * This variable stores the number of reference counter ticks that occurred
 * during the watchdog timeout period. It is preserved across resets using
 * the __noinit attribute to verify watchdog timing accuracy after a
 * watchdog-triggered reset.
 */
static __noinit uint32_t iec60730b_test_wdog_counter_ticks;

int iec60730b_test_wdog(const struct device *wdog, uint32_t wdog_timeout_ms,
			const struct device *counter, uint32_t tolerance_percent)
{
#if CONFIG_HWINFO_HAS_DRIVER
	int32_t ret;
	uint32_t cause;

	/* This function performs a two-phase watchdog test:
	 * 1. On first run: Configures and starts the watchdog timer, then waits
	 *    for a watchdog reset while counting reference counter ticks.
	 * 2. After watchdog reset: Verifies that the measured counter ticks
	 *    during the watchdog timeout period are within the tolerance range.
	 */
	ret = hwinfo_get_reset_cause(&cause);
	(void)hwinfo_clear_reset_cause();
	if (ret == 0) {
		/* Check if the reset was caused by a watchdog timeout.
		 * If true, verify that the reference counter ticks are within
		 * the acceptable tolerance range.
		 */
		if (cause & RESET_WATCHDOG) {
			uint32_t clock_test_tolerance;
			uint32_t clock_test_expected;
			uint32_t counter_frequency;
			uint32_t counter_ticks_limit_high;
			uint32_t counter_ticks_limit_low;

			LOG_DBG("Reset due to RESET_WATCHDOG");

			/* Get the frequency of the reference counter in Hz */
			counter_frequency = counter_get_frequency(counter);
			/* Calculate the expected number of reference counter ticks
			 * during one timer period
			 */
			clock_test_expected =
				((uint64_t)counter_frequency * wdog_timeout_ms) / 1000;
			/* Calculate the tolerance value as a percentage of the
			 * expected counter value
			 */
			clock_test_tolerance =
				((uint64_t)clock_test_expected * tolerance_percent) / 100;
			/* Calculate the upper and lower limits for the counter
			 * ticks based on the expected value and tolerance
			 */
			counter_ticks_limit_high = clock_test_expected + clock_test_tolerance;
			counter_ticks_limit_low = clock_test_expected - clock_test_tolerance;

			LOG_DBG("Counter ticks=%u [%u %u]", iec60730b_test_wdog_counter_ticks,
				counter_ticks_limit_low, counter_ticks_limit_high);
			/* Check if the counter ticks are within the acceptable
			 * range. If the counter ticks exceed the high limit or
			 * fall below the low limit, the watchdog test has failed.
			 */
			if ((iec60730b_test_wdog_counter_ticks > counter_ticks_limit_high) ||
			    (iec60730b_test_wdog_counter_ticks < counter_ticks_limit_low)) {
				return IEC60730B_TEST_WDT_ERROR;
			}
		} else {
			/* Reset was not caused by watchdog. Proceed to configure
			 * and start the watchdog test.
			 */
			struct wdt_timeout_cfg wdog_config = {
				/* Reset SoC when watchdog timer expires */
				.flags = WDT_FLAG_RESET_SOC,
				/* Expire watchdog after max window */
				.window.max = wdog_timeout_ms,
			};

			iec60730b_test_wdog_counter_ticks = 0;

			LOG_DBG("Setup watchdog");
			ret = wdt_install_timeout(wdog, &wdog_config);
			if (ret < 0) {
				LOG_ERR("Watchdog install timeout error: %d", ret);
				return IEC60730B_TEST_WDT_ERROR;
			}
			ret = wdt_setup(wdog, WDT_OPT_PAUSE_HALTED_BY_DBG);
			if (ret < 0) {
				LOG_ERR("Watchdog setup error: %d", ret);
				return IEC60730B_TEST_WDT_ERROR;
			}
			LOG_DBG("Start counter and waiting for reset...");
			ret = counter_start(counter);
			if (ret < 0) {
				LOG_ERR("Counter start error: %d", ret);
				return IEC60730B_TEST_WDT_ERROR;
			}

			LOG_DBG("Waiting for reset...");
			while (1) {
				ret = counter_get_value(counter,
							&iec60730b_test_wdog_counter_ticks);
				if (ret < 0) {
					LOG_ERR("Counter get value error: %d", ret);
					return IEC60730B_TEST_WDT_ERROR;
				}
			}
		}
	} else if (ret == -ENOSYS) {
		LOG_WRN("hwinfo_get_reset_cause() is not supported");
		return IEC60730B_TEST_NOT_SUPPORTED;
	} else {
		LOG_ERR("hwinfo_get_reset_cause() error: %d", ret);
		return IEC60730B_TEST_WDT_ERROR;
	}

	return IEC60730B_TEST_OK;
#else
	return IEC60730B_TEST_NOT_SUPPORTED;
#endif /* CONFIG_HWINFO_HAS_DRIVER */
}

#endif /* CONFIG_IEC60730B_TEST_WDOG */
