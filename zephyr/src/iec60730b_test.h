/*
 * Copyright 2025 NXP
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef _IEC60730B_TEST_H_
#define _IEC60730B_TEST_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*!
 * @name Safety test error codes
 * @brief Error code definitions for IEC 60730 Class B safety test failures
 */
#define IEC60730B_TEST_OK                       (0)      /* No error */
#define IEC60730B_TEST_ERROR                    (-1)     /* General error */
#define IEC60730B_TEST_CPU_REG_ERROR            (-2)     /* CPU registers test fault */
#define IEC60730B_TEST_CPU_REG_PRIMASK_ERROR    (-3)     /* PRIMASK test fault */
#define IEC60730B_TEST_CPU_REG_NONSTACKED_ERROR (-4)     /* Non-stacked CPU test fault */
#define IEC60730B_TEST_CPU_REG_FLOAT_ERROR      (-5)     /* Floating point registers test fault */
#define IEC60730B_TEST_CPU_REG_CONTROL_ERROR    (-6)     /* CONTROL register test fault */
#define IEC60730B_TEST_CPU_REG_SPECIAL_ERROR    (-7)     /* Special CPU registers test fault */
#define IEC60730B_TEST_WDT_ERROR                (-8)     /* Watchdog error */
#define IEC60730B_TEST_FLASH_ERROR              (-9)     /* Flash test fault */
#define IEC60730B_TEST_RAM_ERROR                (-10)    /* RAM test fault */
#define IEC60730B_TEST_STACK_ERROR              (-11)    /* Stack test fault */
#define IEC60730B_TEST_CLOCK_ERROR              (-12)    /* Clock test fault */
#define IEC60730B_TEST_PC_ERROR                 (-13)    /* Program counter test fault */

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************
 * API
 ******************************************************************************/

/*!
 * @brief Test CPU registers for IEC 60730 Class B compliance
 * 
 * This function performs a comprehensive test of CPU registers to detect
 * stuck-at faults and ensure proper register functionality as required
 * by IEC 60730 Class B safety standards.
 * 
 * @return 0 on success, negative on failure
 */
int iec60730b_test_cpu_reg(void);

#ifdef __cplusplus
}
#endif

#endif /* _IEC60730B_TEST_H_ */
