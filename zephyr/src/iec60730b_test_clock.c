/*
 * Copyright 2025 NXP
 * SPDX-License-Identifier: Apache-2.0
 */

#include "iec60730b_test.h"
#include <iec60730b.h>
#include <iec60730b_core.h>
#include <zephyr/drivers/counter.h>

/* NOTE:  CTIMERs on NXP LPC/MCX parts does not auto‑freeze when you pause at a breakpoint. 
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
    static LPTMR_Type *lptmr_base;
#elif defined(CONFIG_COUNTER_MCUX_GPT)
    /* from zephyr/drivers/counter/counter_mcux_gpt.c */
    #include <zephyr/drivers/clock_control.h>
    #define DEV_CFG(_dev) ((const struct mcux_gpt_config *)(_dev)->config)
    struct mcux_gpt_config {
        struct counter_config_info info;
        DEVICE_MMIO_NAMED_ROM(gpt_mmio);
        const struct device *clock_dev;
        clock_control_subsys_t clock_subsys;
        clock_name_t clock_source;
        void (*irq_config_func)(void);
    };
    static fs_gpt_t *gpt_base;
#elif defined(CONFIG_COUNTER_MCUX_CTIMER)
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
    static fs_ctimer_t *ctimer_base;
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
#elif defined(CONFIG_COUNTER_MCUX_GPT)
    FS_CLK_GPT(gpt_base, &counter_ticks);
#elif defined(CONFIG_COUNTER_MCUX_CTIMER)
    FS_CLK_CTIMER(ctimer_base, &counter_ticks);
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

    /* Initialize the base pointer for the hardware counter peripheral */
#ifdef CONFIG_COUNTER_MCUX_LPTMR
    lptmr_base = ((struct mcux_lptmr_config*)counter->config)->base;
#elif defined(CONFIG_COUNTER_MCUX_GPT)
    gpt_base = (fs_gpt_t *)DEVICE_MMIO_NAMED_GET(counter, gpt_mmio);
#elif defined(CONFIG_COUNTER_MCUX_CTIMER)
    ctimer_base = (fs_ctimer_t *)((struct mcux_lpc_ctimer_config*)counter->config)->base;
#else
    return IEC60730B_TEST_NOT_SUPPORTED;
#endif

    /* Convert the timer period from ticks to milliseconds */
    timer_period_ms = k_ticks_to_ms_floor32(timer_period.ticks);
    /* Get the frequency of the reference counter in Hz */
    counter_frequency = counter_get_frequency(counter);
    /* Calculate the expected number of reference counter ticks during one timer period */
    clock_test_expected = ((uint64_t)counter_frequency * timer_period_ms) / 1000;
    /* Calculate the tolerance value as a percentage of the expected counter value */
    clock_test_tolerance = ((uint64_t)clock_test_expected * tolerance_percent) / 100;

    counter_ticks_limit_high = clock_test_expected + clock_test_tolerance;
    counter_ticks_limit_low = clock_test_expected - clock_test_tolerance;
    clock_test_start    = false; /* clock test result will be processed after the first interrupt occurs */

    /* Initialize the counter ticks variable before starting the clock test */
    FS_CLK_Init(&counter_ticks);

    /* Start the reference counter */
    counter_start(counter); 

    /* Start the periodic timer */
    k_timer_start(&iec60730b_test_clock_timer, timer_period, timer_period);

    return IEC60730B_TEST_OK;
}

/* Perform clock test for IEC 60730 Class B compliance */
int iec60730b_test_clock(void)
{
#if defined(CONFIG_COUNTER_MCUX_LPTMR) || defined(CONFIG_COUNTER_MCUX_GPT) || defined(CONFIG_COUNTER_MCUX_CTIMER)
    if (clock_test_start == true) { /* condition is valid after the first timer interrupt */
        printk("counter_ticks=0x%x [0x%x 0x%x]\n", counter_ticks, counter_ticks_limit_low, counter_ticks_limit_high);
        if (FS_CLK_Check(counter_ticks, counter_ticks_limit_low, counter_ticks_limit_high) == FS_FAIL_CLK) {
            return IEC60730B_TEST_CLOCK_ERROR;
        }
    }
    return IEC60730B_TEST_OK;
#else
    return IEC60730B_TEST_NOT_SUPPORTED;
#endif
}
#endif /* CONFIG_IEC60730B_TEST_CLOCK */
