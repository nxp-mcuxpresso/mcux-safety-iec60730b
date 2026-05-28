/*
 * Copyright 2025-2026 NXP
 * SPDX-License-Identifier: Apache-2.0
 */

#include "iec60730b_test.h"
#include <zephyr/drivers/counter.h>

#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(test_clock, CONFIG_IEC60730B_TEST_CLOCK_LOG_LEVEL);

/* NOTE: Some timers do not auto-freeze when you pause at a breakpoint.
 * Disable breakpoints, otherwise the clock test may fail.
 */

#ifdef CONFIG_IEC60730B_TEST_CLOCK

/*
 * Elapsed counter ticks captured during the most recent timer period.
 * Written inside the k_timer ISR, read by iec60730b_test_clock().
 * Declared volatile so the compiler does not cache the value in a register
 * across the periodic check call.
 */
static volatile uint32_t counter_ticks_elapsed;

/* Acceptable counter-tick window computed once during init */
static uint32_t counter_ticks_limit_high;
static uint32_t counter_ticks_limit_low;

/*
 * Flag set to true after the first timer interrupt fires.
 * Prevents iec60730b_test_clock() from evaluating a result before the
 * first valid measurement has been captured.
 */
static bool clock_test_started;

/* Pointer to the Zephyr counter device used as the reference clock source */
static const struct device *clock_counter_dev;

/*
 * Periodic k_timer callback — runs in interrupt context.
 * Reads the current counter value (= ticks elapsed since the last reset),
 * stores it, then resets the counter.
 */
static void iec60730b_test_clock_timer_handler(struct k_timer *timer)
{
	uint32_t current_ticks;
	int ret;

	ret = counter_get_value(clock_counter_dev, &current_ticks);
	if (ret != 0) {
		LOG_ERR("counter_get_value() error: %d", ret);
		return;
	}

	/* current_ticks is the elapsed count since the last counter_reset() */
	counter_ticks_elapsed = current_ticks;

	/* Reset the counter to zero without stopping it */
	ret = counter_reset(clock_counter_dev);
	if (ret != 0) {
		LOG_ERR("counter_reset() error: %d", ret);
	}

	clock_test_started = true;
}

/* Periodic timer that drives the clock test measurements */
static K_TIMER_DEFINE(iec60730b_test_clock_timer, iec60730b_test_clock_timer_handler, NULL);

/*
 * Initialize clock test for IEC 60730 Class B compliance.
 *
 * Configures the reference counter, computes the expected tick window, and
 * starts both the counter and the periodic measurement timer.
 */
int iec60730b_test_clock_init(const struct device *counter, uint32_t timer_period_ms,
			      uint32_t tolerance_percent)
{
	uint32_t counter_frequency;
	uint32_t clock_test_expected;
	uint32_t clock_test_tolerance;
	int ret;

	if (!device_is_ready(counter)) {
		LOG_ERR("Counter device not ready");
		return IEC60730B_TEST_ERROR;
	}

	/* Store device reference for use inside the timer callback */
	clock_counter_dev = counter;

	/* Get the frequency of the reference counter in Hz */
	counter_frequency = counter_get_frequency(counter);
	if (counter_frequency == 0U) {
		LOG_ERR("Counter frequency is 0");
		return IEC60730B_TEST_ERROR;
	}

	/* Expected counter ticks during one timer period */
	clock_test_expected = (uint32_t)(((uint64_t)counter_frequency * timer_period_ms) / 1000U);

	/* Tolerance as an absolute tick count */
	clock_test_tolerance =
		(uint32_t)(((uint64_t)clock_test_expected * tolerance_percent) / 100U);

	counter_ticks_limit_high = clock_test_expected + clock_test_tolerance;
	counter_ticks_limit_low = clock_test_expected - clock_test_tolerance;

	LOG_DBG("Expected ticks=%u [%u %u]", clock_test_expected, counter_ticks_limit_low,
		counter_ticks_limit_high);

	/* Reset state */
	counter_ticks_elapsed = 0U;
	clock_test_started = false;

	/* Start the reference counter from zero */
	ret = counter_start(counter);
	if (ret != 0) {
		LOG_ERR("counter_start() error: %d", ret);
		return IEC60730B_TEST_ERROR;
	}

	/* Start the periodic measurement timer */
	k_timer_start(&iec60730b_test_clock_timer, K_MSEC(timer_period_ms),
		      K_MSEC(timer_period_ms));

	return IEC60730B_TEST_OK;
}

/*
 * Perform clock test for IEC 60730 Class B compliance.
 *
 * Should be called periodically from the application after
 * iec60730b_test_clock_init().
 */
int iec60730b_test_clock(void)
{
	if (!clock_test_started) {
		/* Wait for the first timer interrupt before evaluating */
		return IEC60730B_TEST_OK;
	}

	uint32_t elapsed = counter_ticks_elapsed;

	LOG_DBG("Elapsed ticks=%u [%u %u]", elapsed, counter_ticks_limit_low,
		counter_ticks_limit_high);

	if ((elapsed < counter_ticks_limit_low) || (elapsed > counter_ticks_limit_high)) {
		return IEC60730B_TEST_CLOCK_ERROR;
	}

	return IEC60730B_TEST_OK;
}

#endif /* CONFIG_IEC60730B_TEST_CLOCK */
