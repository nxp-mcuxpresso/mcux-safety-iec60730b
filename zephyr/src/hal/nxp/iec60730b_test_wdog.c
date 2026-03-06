/*
 * Copyright 2026 NXP
 * SPDX-License-Identifier: Apache-2.0
 */

#include "iec60730b_test.h"
#include <iec60730b.h>
#include <iec60730b_core.h>

#include <zephyr/drivers/hwinfo.h>
#include <zephyr/drivers/watchdog.h>
#include <zephyr/drivers/counter.h>

#include <zephyr/logging/log.h>
#include <zephyr/logging/log_ctrl.h>
LOG_MODULE_REGISTER(test_wdog, CONFIG_IEC60730B_TEST_WDOG_LOG_LEVEL);

#ifdef CONFIG_IEC60730B_TEST_WDOG

/* Zephyr driver configuration */
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
#endif
#ifdef CONFIG_WDT_MCUX_WWDT
    /* from zephyr/drivers/watchdog/wdt_mcux_wwdt.c */
    #include <fsl_wwdt.h>
    struct mcux_wwdt_config {
        WWDT_Type *base;
        uint8_t clk_divider;
        void (*irq_config_func)(const struct device *dev);
    };
#endif


static __noinit fs_wdog_test_t WatchdogBackup;

int iec60730b_test_wdog(const struct device *wdog, uint32_t wdog_timeout_ms, const struct device *counter, uint32_t tolerance_percent)
{
#if CONFIG_HWINFO_HAS_DRIVER
    int32_t ret;
    uint32_t cause;

    /* This function performs a two-phase watchdog test:
     * 1. On first run: Configures and starts the watchdog timer, then waits for a watchdog reset
     *    while counting reference counter ticks.
     * 2. After watchdog reset: Verifies that the measured counter ticks during the watchdog timeout
     *    period are within the specified tolerance range.
     */
    ret = hwinfo_get_reset_cause(&cause);
    (void)hwinfo_clear_reset_cause();
    if (ret == 0) {
        /* Check if the reset was caused by a watchdog timeout.
         * If true, verify that the reference counter ticks are within the acceptable tolerance range.
         */
        if (cause & RESET_WATCHDOG) {
            uint32_t clock_test_tolerance;
            uint32_t clock_test_expected;
            uint32_t counter_frequency;
            uint32_t counter_ticks_limit_high;
            uint32_t counter_ticks_limit_low;
            /* Set up dummy reset detection register and mask for FS_WDOG_Check() validation.
             * The actual reset cause has already been verified/cleared by hwinfo_get/clear_reset_cause().
             */
            uint32_t reset_detect_register = 1;
            WatchdogBackup.pResetDetectRegister = (uint32_t)&reset_detect_register;
            WatchdogBackup.ResetDetectMask = reset_detect_register;

            LOG_DBG("Reset due to RESET_WATCHDOG");

            /* Get the frequency of the reference counter in Hz */
            counter_frequency = counter_get_frequency(counter);
            /* Calculate the expected number of reference counter ticks during one timer period */
            clock_test_expected = ((uint64_t)counter_frequency * wdog_timeout_ms) / 1000;
            /* Calculate the tolerance value as a percentage of the expected counter value */
            clock_test_tolerance = ((uint64_t)clock_test_expected * tolerance_percent) / 100;
            /* Calculate the upper and lower limits for the counter ticks based on the expected value and tolerance */
            counter_ticks_limit_high = clock_test_expected + clock_test_tolerance;
            counter_ticks_limit_low = clock_test_expected - clock_test_tolerance;

            LOG_DBG("Counter ticks=%u [%u %u]\n", WatchdogBackup.counter, counter_ticks_limit_low, counter_ticks_limit_high);
            if(FS_WDOG_Check(counter_ticks_limit_high, counter_ticks_limit_low, 2, FALSE, &WatchdogBackup, FALSE, FALSE) != FS_PASS) {
                return IEC60730B_TEST_WDT_ERROR;
            }
        /* Reset was not caused by watchdog. Proceed to configure and start the watchdog test. */
        } else {
        #ifdef CONFIG_COUNTER_MCUX_CTIMER
            const char *counter_name = counter->name;
            if (strstr(counter_name, "ctimer") != NULL) {
                WatchdogBackup.RefTimerBase = (uint32_t)((struct mcux_lpc_ctimer_config*)counter->config)->base;
            #ifdef CONFIG_WDT_MCUX_WWDT
                WatchdogBackup.WdogBase = (uint32_t)((const struct mcux_wwdt_config *)wdog->config)->base;

                struct wdt_timeout_cfg wdog_config = {
                    .flags = WDT_FLAG_RESET_SOC,    /* Reset SoC when watchdog timer expires */
                    .window.max = wdog_timeout_ms,  /* Expire watchdog after max window */
                };
                WatchdogBackup.counter = 0;

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
                LOG_DBG("Start counter and waiting for reset...\n");
                FS_WDOG_Setup_WWDT_CTIMER(&WatchdogBackup);
            #else /* CONFIG_WDT_MCUX_WWDT */
                return IEC60730B_TEST_NOT_SUPPORTED;
            #endif
            }
            else
        #endif /* CONFIG_COUNTER_MCUX_CTIMER */
                return IEC60730B_TEST_NOT_SUPPORTED;
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
