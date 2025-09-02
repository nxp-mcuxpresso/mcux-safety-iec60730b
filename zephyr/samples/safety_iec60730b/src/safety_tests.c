/*
 * Copyright 2025 NXP
 * SPDX-License-Identifier: Apache-2.0
 */

#include "safety_tests.h"
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

/* TBD:
    - Add Non-Interruptable CPU registers test
    - Add other safety tests.
    - ?Add watchdog?
    - ?Enable Zephyr stack protection?
*/

/*******************************************************************************
* Variables
******************************************************************************/
int safety_error_code; /* Global error code. It should be allocated in the 
                          dedicated RAM memory that is deleted only after POR. */

static int safety_tests_init(void);
static void safety_tests_thread(void *arg1, void *arg2, void *arg3);
static void safety_error_handling(int error_code);

/*!
 * @brief Register safety_tests module for logging
 */
LOG_MODULE_REGISTER(safety_tests, CONFIG_APP_SAFETY_TESTS_LOG_LEVEL);

/*!
 * @brief   System initialization hook for safety tests after reset.
 *
 *          This system initialization call ensures that IEC 60730-B compliant safety tests
 *          are executed automatically during system startup after the kernel is initialized.
 *          The safety tests run at the default kernel initialization priority level to
 *          verify system integrity before normal application execution begins.
 */
SYS_INIT(safety_tests_init, POST_KERNEL /* EARLY */, CONFIG_KERNEL_INIT_PRIORITY_DEFAULT);

/*!
 * @brief   Safety test thread definition and automatic startup configuration.
 */
K_THREAD_DEFINE(safety_tests, CONFIG_APP_SAFETY_TESTS_THREAD_STACK_SIZE,
                safety_tests_thread, NULL, NULL, NULL,
                CONFIG_APP_SAFETY_TESTS_THREAD_PRIORITY, 0, 0);

/*******************************************************************************
 * Code
 ******************************************************************************/

/*!
 * @brief   Safety tests initialization function executed during system startup.
 *
 *          This function performs IEC 60730-B compliant safety tests that must be
 *          executed during system initialization after reset to ensure safety 
 *          tests complete before application code execution.
 *          Additional safety tests can be added.
 *
 * @return  0 on successful completion of initialization safety tests
 */
static int safety_tests_init(void)
{
    int result;

#ifdef CONFIG_SAFETY_IEC60730B_TEST_CPU_REG
    LOG_INF("Executing CPU after reset test");
    result = sf_cpu_reg_test_init();
    if(result != FS_TEST_OK){
        safety_error_handling(result);
    }
#endif

    /* === ADD YOUR SAFETY TESTS HERE === */

    return 0;
}

/*!
 * @brief   Safety test thread function that executes periodic safety tests.
 *
 *          This thread function runs continuously to perform background
 *          IEC 60730-B compliant safety tests at regular intervals.
 */
static void safety_tests_thread(void *arg1, void *arg2, void *arg3)
{
    ARG_UNUSED(arg1);
    ARG_UNUSED(arg2);
    ARG_UNUSED(arg3);

    int result;

    LOG_INF("Safety test thread started automatically");

    /* Thread function runs indefinitely */
    while (1) {
#ifdef CONFIG_SAFETY_IEC60730B_TEST_CPU_REG
        /* Interruptable CPU registers test */
        LOG_INF("Executing CPU background test");
        result = sf_cpu_reg_test();
        if(result != FS_TEST_OK){
            safety_error_handling(result);
        }
#endif

        /* === ADD YOUR SAFETY TESTS HERE ==*/

        /* Sleep before next iteration */
        k_msleep(CONFIG_APP_SAFETY_TESTS_PERIOD_MS);
    }
}

/*!
 * @brief   Handling with a safety error.
 *
 *          This function stores the code of recognized safety error into the dedicated RAM memory that is deleted only
 *          after POR.
 *          If CONFIG_APP_SAFETY_ERROR_ACTION_INFINITE_LOOP macro is defined, interrupts are disabled and function waits
 *          for watchdog reset.
 *
 * @param   psSafetyCommon - The pointer of the Common Safety structure
 *
 * @return  None
 */
static void safety_error_handling(int error_code)
{
    safety_error_code |= error_code;
    LOG_ERR("Safety error detected: 0x%08x", safety_error_code);

#ifdef CONFIG_APP_SAFETY_ERROR_ACTION_INFINITE_LOOP
    (void)irq_lock(); /* Disable interrupts */
    while (1){
    }
#endif
}
