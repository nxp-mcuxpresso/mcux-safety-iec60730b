/*
 * Copyright 2025 NXP
 * SPDX-License-Identifier: Apache-2.0
 */

#include "iec60730b_test.h"
#include <iec60730b.h>
#include <iec60730b_core.h>
#include <zephyr/drivers/counter.h>

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
    static LPTMR_Type *lptmr_base;
#endif

static uint32_t counter_ticks;
static uint32_t counter_ticks_limit_high;
static uint32_t counter_ticks_limit_low;
static bool clock_test_start;

/* This function is called from the system clock interrupt handler to capture precise counter values at regular intervals.*/
static void iec60730b_test_clock_timer_handler(struct k_timer *timer)
{
    /* This runs in interrupt context */

#ifdef CONFIG_COUNTER_MCUX_LPTMR
    FS_CLK_LPTMR((fs_lptmr_t *)lptmr_base, &counter_ticks);
#endif
    clock_test_start = true; /* to prevent checking of result before execution */
}

/* Timer definition for the clock test, triggers the handler at regular intervals */
static K_TIMER_DEFINE(iec60730b_test_clock_timer, iec60730b_test_clock_timer_handler, NULL);

/* Initialize clock test for IEC 60730 Class B compliance */
int iec60730b_test_clock_init(const struct device *counter, k_timeout_t timer_period, uint32_t tolerance_percent)
{
    uint32_t clock_test_tolerance;
    uint32_t clock_test_expected;
    uint32_t counter_frequency;
    uint32_t timer_period_ms;

    /* Convert the timer period from ticks to milliseconds */
    timer_period_ms = k_ticks_to_ms_floor32(timer_period.ticks);
    /* Get the frequency of the reference counter in Hz */
    counter_frequency = counter_get_frequency(counter);
    /* Calculate the expected number of reference counter ticks during one timer period */
    clock_test_expected = (counter_frequency * timer_period_ms) / 1000U;
    /* Calculate the tolerance value as a percentage of the expected counter value */
    clock_test_tolerance = (clock_test_expected * tolerance_percent) / (uint32_t)100;

    counter_ticks_limit_high = clock_test_expected + clock_test_tolerance;
    counter_ticks_limit_low = clock_test_expected - clock_test_tolerance;
    clock_test_start    = false; /* clock test result will be processed after the first interrupt occurs */

#ifdef CONFIG_COUNTER_MCUX_LPTMR
    lptmr_base = ((struct mcux_lptmr_config*)counter->config)->base;
    FS_CLK_Init(&counter_ticks);
#else
    return IEC60730B_TEST_NOT_SUPPORTED;
#endif

    /* Start the reference counter */
    counter_start(counter);

    /* Start the periodic timer */
    k_timer_start(&iec60730b_test_clock_timer, timer_period, timer_period);

    return IEC60730B_TEST_OK;
}

/* Perform clock test for IEC 60730 Class B compliance */
int iec60730b_test_clock(void)
{

    if (clock_test_start == true) { /* condition is valid after the first timer interrupt */
        printk("counter_ticks=0x%x [0x%x 0x%x]\n", counter_ticks, counter_ticks_limit_low, counter_ticks_limit_high);
#ifdef CONFIG_COUNTER_MCUX_LPTMR
        if (FS_CLK_Check(counter_ticks, counter_ticks_limit_low, counter_ticks_limit_high) == FS_FAIL_CLK) {
           return IEC60730B_TEST_CLOCK_ERROR;
        }
#else
    return IEC60730B_TEST_NOT_SUPPORTED;
#endif
    }

    return IEC60730B_TEST_OK;
}
#endif /* CONFIG_IEC60730B_TEST_CLOCK */
