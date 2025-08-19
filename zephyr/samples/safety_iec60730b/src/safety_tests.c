/*
 * Copyright 2025 NXP
 * SPDX-License-Identifier: Apache-2.0
 */

#include "safety_tests.h"
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

/*******************************************************************************
* Variables
******************************************************************************/
static void safety_tests_thread(void *arg1, void *arg2, void *arg3);

/*******************************************************************************
* Variables
******************************************************************************/
uint32_t m_safety_error_code; /* Global error code. It should be allocated in the 
                                 dedicated RAM memory that is deleted only after POR. */

/*!
 * @brief   Global safety common structure
 *
 *          This structure contains the common safety test data and error tracking
 *          information used across all IEC 60730-B safety tests. It maintains:
 *          - Safety error codes and status flags
 *          - Test execution state and results
 *          - Configuration parameters for safety tests
 *          - Runtime data for periodic and background safety checks
 *
 *          The structure is shared between different safety test modules and
 *          provides a centralized location for safety-related status information.
 */
static safety_common_t g_sSafetyCommon;

/* Register this module for logging */
LOG_MODULE_REGISTER(safety_tests, CONFIG_APP_SAFETY_TESTS_LOG_LEVEL);

/*******************************************************************************
 * Code
 ******************************************************************************/

 /* Define and automatically start the safety test thread */
K_THREAD_DEFINE(safety_tests, CONFIG_APP_SAFETY_TESTS_THREAD_STACK_SIZE,
                safety_tests_thread, NULL, NULL, NULL,
                CONFIG_APP_SAFETY_TESTS_THREAD_PRIORITY, 0, 0);

/*!
 * @brief   Safety test thread function that executes periodic safety tests.
 *
 *          This thread function runs continuously to perform IEC 60730-B compliant safety tests.
 *          It executes CPU register tests after reset (if configured) and then enters a loop
 *          to perform background safety tests at regular intervals.
 *          
 *          The thread performs the following operations:
 *          - Executes CPU after-reset test during initialization (if enabled)
 *          - Runs periodic background CPU register tests (if enabled)
 *          - Sleeps for 1 second between test iterations
 *          - Provides debug output for test execution status
 *
 * @param   arg1 - Unused thread argument (marked as unused)
 * @param   arg2 - Unused thread argument (marked as unused)  
 * @param   arg3 - Unused thread argument (marked as unused)
 *
 * @return  None (thread function runs indefinitely)
 */
static void safety_tests_thread(void *arg1, void *arg2, void *arg3)
{
    ARG_UNUSED(arg1);
    ARG_UNUSED(arg2);
    ARG_UNUSED(arg3);

    LOG_INF("Safety test thread started automatically");
    
#ifdef CONFIG_SAFETY_IEC60730B_TEST_CPU_REG
    LOG_INF("Executing CPU after reset test");
    SafetyCpuAfterResetTest(&g_sSafetyCommon);
#endif

    while (1) {
#ifdef CONFIG_SAFETY_IEC60730B_TEST_CPU_REG
        /* Interruptable CPU registers test */
        LOG_INF("Executing CPU background test");
        SafetyCpuBackgroundTest(&g_sSafetyCommon);
#endif

        /* ADD YOUR SAFETY TESTS HERE*/

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
void SafetyErrorHandling(safety_common_t *psSafetyCommon)
{
    m_safety_error_code = psSafetyCommon->safetyErrors;
    LOG_ERR("Safety error detected: 0x%08x", m_safety_error_code);

#ifdef CONFIG_APP_SAFETY_ERROR_ACTION_INFINITE_LOOP
    (void)irq_lock(); /* Disable interrupts */
    while (1){
    }
#endif
}
