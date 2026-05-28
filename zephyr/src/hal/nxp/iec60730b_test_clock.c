/*
 * Copyright 2025-2026 NXP
 * SPDX-License-Identifier: Apache-2.0
 */

#include "iec60730b_test.h"
#include <iec60730b.h>
#include <iec60730b_core.h>
#include <zephyr/drivers/counter.h>

#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(test_clock, CONFIG_IEC60730B_TEST_CLOCK_LOG_LEVEL);

/* NOTE: Some timers do not auto-freeze when you pause at a breakpoint.
 * Disable breakpoints, otherwise the clock test may fail.
 */

#ifdef CONFIG_IEC60730B_TEST_CLOCK

/* Zephyr counter driver configuration */
#ifdef CONFIG_COUNTER_MCUX_LPTMR
/* from zephyr/drivers/counter/counter_mcux_lptmr.c */
#include <fsl_lptmr.h>
struct mcux_lptmr_config {
	struct counter_config_info info;
	LPTMR_Type *base;
	lptmr_prescaler_clock_select_t clk_source;
	lptmr_prescaler_glitch_value_t prescaler_glitch;
	bool bypass_prescaler_glitch;
	lptmr_timer_mode_t mode;
	lptmr_pin_select_t pin;
	lptmr_pin_polarity_t polarity;
	void (*irq_config_func)(const struct device *dev);
};
static void *lptmr_base;
#endif
#ifdef CONFIG_COUNTER_MCUX_GPT
/* from zephyr/drivers/counter/counter_mcux_gpt.c */
#include <zephyr/drivers/clock_control.h>
#define DEV_CFG(_dev) ((const struct mcux_gpt_config *)(_dev)->config)
struct mcux_gpt_config {
	struct counter_config_info info;

	DEVICE_MMIO_NAMED_ROM(gpt_mmio);
	const struct device *clock_dev;
	clock_control_subsys_t clock_subsys;
	bool enable_free_run;
	void (*irq_config_func)(void);
};
static void *gpt_base;
#endif
#ifdef CONFIG_COUNTER_MCUX_CTIMER
/* from zephyr/drivers/counter/counter_mcux_ctimer.c */
#include <zephyr/drivers/clock_control.h>
#include <fsl_ctimer.h>
struct mcux_lpc_ctimer_config {
	struct counter_config_info info;
	CTIMER_Type *base;
	const struct device *clock_dev;
	clock_control_subsys_t clock_subsys;
	ctimer_timer_mode_t mode;
	ctimer_capture_channel_t input;
	uint32_t prescale;
	void (*irq_config_func)(const struct device *dev);
};
static void *ctimer_base;
#endif

/*
 * Elapsed counter ticks captured during the most recent timer period.
 * Written inside the k_timer ISR, read by iec60730b_test_clock().
 * Declared volatile so the compiler does not cache the value in a register
 * across the periodic check call.
 */
static uint32_t counter_ticks_elapsed;

/* Acceptable counter-tick window computed once during init */
static uint32_t counter_ticks_limit_high;
static uint32_t counter_ticks_limit_low;

/*
 * Flag set to true after the first timer interrupt fires.
 * Prevents iec60730b_test_clock() from evaluating a result before the
 * first valid measurement has been captured.
 */
static bool clock_test_started;

/*
 * Periodic k_timer callback — runs in interrupt context.
 * Reads the current counter value (= ticks elapsed since the last reset),
 * stores it, then resets the counter.
 */
static void iec60730b_test_clock_timer_handler(struct k_timer *timer)
{

#ifdef CONFIG_COUNTER_MCUX_LPTMR
	if (lptmr_base) {
		FS_CLK_LPTMR(lptmr_base, &counter_ticks_elapsed);
	} else
#endif
#ifdef CONFIG_COUNTER_MCUX_GPT
		if (gpt_base) {
		FS_CLK_GPT(gpt_base, &counter_ticks_elapsed);
	} else
#endif
#ifdef CONFIG_COUNTER_MCUX_CTIMER
		if (ctimer_base) {
		FS_CLK_CTIMER(ctimer_base, &counter_ticks_elapsed);
	} else
#endif
	{
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

	/* Initialize the base pointer for the hardware counter peripheral */
#ifdef CONFIG_COUNTER_MCUX_LPTMR
	if (strstr(counter->name, "lptmr") != NULL) {
		lptmr_base = ((struct mcux_lptmr_config *)counter->config)->base;
	} else
#endif
#ifdef CONFIG_COUNTER_MCUX_GPT
		if (strstr(counter->name, "gpt") != NULL) {
		gpt_base = (void *)DEVICE_MMIO_NAMED_GET(counter, gpt_mmio);
	} else
#endif
#ifdef CONFIG_COUNTER_MCUX_CTIMER
		if (strstr(counter->name, "ctimer") != NULL) {
		ctimer_base = ((struct mcux_lpc_ctimer_config *)counter->config)->base;
	} else
#endif
		return IEC60730B_TEST_NOT_SUPPORTED;

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
	FS_CLK_Init(&counter_ticks_elapsed);
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
#if defined(CONFIG_COUNTER_MCUX_LPTMR) || defined(CONFIG_COUNTER_MCUX_GPT) ||                      \
	defined(CONFIG_COUNTER_MCUX_CTIMER)
	if (!clock_test_started) {
		/* Wait for the first timer interrupt before evaluating */
		return IEC60730B_TEST_OK;
	}

	uint32_t elapsed = counter_ticks_elapsed;

	LOG_DBG("Elapsed ticks=%u [%u %u]", elapsed, counter_ticks_limit_low,
		counter_ticks_limit_high);

	if (FS_CLK_Check(counter_ticks_elapsed, counter_ticks_limit_low,
			 counter_ticks_limit_high) == FS_FAIL_CLK) {
		return IEC60730B_TEST_CLOCK_ERROR;
	}

	return IEC60730B_TEST_OK;
#else
	return IEC60730B_TEST_NOT_SUPPORTED;
#endif
}

#endif /* CONFIG_IEC60730B_TEST_CLOCK */
