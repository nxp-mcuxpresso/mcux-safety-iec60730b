/*
 * Copyright 2025 NXP
 * SPDX-License-Identifier: Apache-2.0
 */

#include <iec60730b_test.h>

#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/device.h>
#include <zephyr/drivers/watchdog.h>
#include <zephyr/task_wdt/task_wdt.h>
#include <zephyr/sys/reboot.h>

/*******************************************************************************
* Declarations
******************************************************************************/
static int safety_init(void);
static void safety_thread(void *arg1, void *arg2, void *arg3);
static void safety_error_handling(int error_code);
static void safety_startup_tests(void);
static void safety_rutime_tests(void);

#ifdef CONFIG_APP_SAFETY_TASK_WATCHDOG
static void safety_task_wdt_callback(int channel_id, void *user_data);

#if DT_NODE_HAS_STATUS_OKAY(DT_ALIAS(watchdog0))
#define WDT_NODE DT_ALIAS(watchdog0)
#else
#define WDT_NODE DT_INVALID_NODE
#endif

/* Task watchdog channel ID for safety tests monitoring. */
int safety_task_wdt_id;
#endif /* CONFIG_APP_SAFETY_TASK_WATCHDOG */

int safety_error_code; /* Global error code. */

/*!
 * @brief Register safety_tests module for logging
 */
LOG_MODULE_REGISTER(safety, CONFIG_APP_SAFETY_LOG_LEVEL);

/*!
 * @brief   System initialization hook for safety tests after reset.
 */
SYS_INIT(safety_init, APPLICATION, CONFIG_KERNEL_INIT_PRIORITY_DEFAULT);

/*!
 * @brief   Safety test thread definition and automatic startup configuration.
 */
K_THREAD_DEFINE(safety, CONFIG_APP_SAFETY_THREAD_STACK_SIZE,
                safety_thread, NULL, NULL, NULL,
                CONFIG_APP_SAFETY_THREAD_PRIORITY, 0, 0);

/*******************************************************************************
 * Code
 ******************************************************************************/

/*!
 * @brief   Safety tests initialization function executed during system startup.
 */
static int safety_init(void)
{
    int result;

#ifdef CONFIG_APP_SAFETY_TASK_WATCHDOG
    const struct device *const hw_wdt_dev = DEVICE_DT_GET_OR_NULL(WDT_NODE);

    if (!device_is_ready(hw_wdt_dev)) {
        LOG_WRN("Hardware watchdog not ready");
        result = task_wdt_init(NULL);
    } else {
        result = task_wdt_init(hw_wdt_dev);
    }

    if (result != 0) {
        LOG_ERR("task wdt init failure: %d\n", result);
        safety_error_handling(IEC60730B_TEST_ERROR);
    }

    /* Add a new task watchdog channel with the safety callback function */
    safety_task_wdt_id = task_wdt_add(CONFIG_APP_SAFETY_TASK_WATCHDOG_TIMEOUT_MS, safety_task_wdt_callback, NULL);
    if (safety_task_wdt_id < 0) {
        LOG_ERR("Failed to add task WDT channel");
        safety_error_handling(IEC60730B_TEST_ERROR);
    }
    LOG_INF("Task WDT channel %d added with timeout %d ms", safety_task_wdt_id, CONFIG_APP_SAFETY_TASK_WATCHDOG_TIMEOUT_MS);
#endif /* CONFIG_APP_SAFETY_TASK_WATCHDOG */

    /* Perform startup safety tests */
    safety_startup_tests();

    return 0;
}

/*!
 * @brief   Safety test thread function that executes periodic safety tests.
 */
static void safety_thread(void *arg1, void *arg2, void *arg3)
{
    ARG_UNUSED(arg1);
    ARG_UNUSED(arg2);
    ARG_UNUSED(arg3);

    LOG_INF("Safety test thread started automatically");

    /* Thread function runs indefinitely */
    while (1) {
        /* Perform runtime safety tests */
        safety_rutime_tests();

        /* Sleep before next iteration */
        k_msleep(CONFIG_APP_SAFETY_PERIOD_MS);
    }
}

/*!
 * @brief   Handling with a safety error.
 */
static void safety_error_handling(int error_code)
{
    safety_error_code = error_code;
    LOG_ERR("Safety error detected: 0x%08x", safety_error_code);

#ifdef CONFIG_APP_SAFETY_ERROR_ACTION_INFINITE_LOOP
    LOG_INF("Entering infinite loop");
    (void)irq_lock(); /* Disable interrupts */
    while (1){
#ifdef CONFIG_APP_SAFETY_TASK_WATCHDOG
        task_wdt_feed(safety_task_wdt_id);
#endif /* CONFIG_APP_SAFETY_TASK_WATCHDOG */
    }
#elif defined(CONFIG_APP_SAFETY_ERROR_ACTION_RESET)
    LOG_INF("Performing immediate cold reset");
    sys_reboot(SYS_REBOOT_COLD);
#endif
}

#ifdef CONFIG_APP_SAFETY_TASK_WATCHDOG
/*!
 * @brief   Callback function for task watchdog timeout events.
 */
static void safety_task_wdt_callback(int channel_id, void *user_data)
{
    ARG_UNUSED(user_data);
    LOG_WRN("Task watchdog channel %d timeout", channel_id);

    safety_error_handling(IEC60730B_TEST_WDT_ERROR);
}
#endif

/*******************************************************************************
 * ADD YOUR SAFETY TESTS HERE
 ******************************************************************************/
/*!
 * @brief   Executes safety tests during system startup.
 *
 * This function performs various safety tests that are required to be executed
 * during system initialization.
 * If any test fails, the safety error handling mechanism is invoked.
 */
static void safety_startup_tests(void)
{
    int result;

#ifdef CONFIG_IEC60730B_TEST_CPU_REG
    LOG_INF("Executing CPU Registers tests");
    result = iec60730b_test_cpu_reg();
    if(result < 0){
        safety_error_handling(result);
    }
#endif

#ifdef CONFIG_APP_SAFETY_TASK_WATCHDOG
    task_wdt_feed(safety_task_wdt_id);
#endif
}

/*!
 * @brief   Executes safety tests during system runtime.
 *
 * This function performs various safety tests that are required to be executed
 * during normal system operation.
 * If any test fails, the safety error handling mechanism is invoked.
 */
static void safety_rutime_tests(void)
{
   int result;

#ifdef CONFIG_IEC60730B_TEST_CPU_REG
    LOG_INF("Executing CPU Registers tests");
    result = iec60730b_test_cpu_reg();
    if(result < 0){
        safety_error_handling(result);
    }
#endif

#ifdef CONFIG_APP_SAFETY_TASK_WATCHDOG
    task_wdt_feed(safety_task_wdt_id);
#endif
}

