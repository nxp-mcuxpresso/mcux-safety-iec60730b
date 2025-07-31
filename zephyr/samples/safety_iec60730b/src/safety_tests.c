/*
 * Copyright 2025 NXP
 * SPDX-License-Identifier: Apache-2.0
 */

#include "safety_tests.h"
#include <zephyr/kernel.h>

/*******************************************************************************
* Variables
******************************************************************************/
uint32_t m_safety_error_code; /* Global error code */

/*******************************************************************************
 * Code
 ******************************************************************************/

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
#ifdef CONFIG_APP_SAFETY_ERROR_ACTION_INFINITE_LOOP
    (void)irq_lock(); /* Disable interrupts */
    while (1){
    }
#endif
}
