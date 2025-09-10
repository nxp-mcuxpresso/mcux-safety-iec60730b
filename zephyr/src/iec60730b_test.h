/*
 * Copyright 2025 NXP
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef _SAFETY_TESTS_H_
#define _SAFETY_TESTS_H_

#include "iec60730b.h"
#include "iec60730b_core.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*!
 * @name Safety test error flags
 * @brief Error flag definitions for IEC 60730 Class B safety tests
 */
#define IEC60730B_TEST_OK                       (0)      /* No error */
#define IEC60730B_TEST_ERROR                    (-1)     /* General error */
#define IEC60730B_TEST_CPU_ERROR                (-2)     /* CPU test fault */
#define IEC60730B_TEST_FLASH_ERROR              (-3)     /* Flash test fault */
#define IEC60730B_TEST_RAM_ERROR                (-4)     /* RAM test fault */
#define IEC60730B_TEST_STACK_ERROR              (-5)     /* Stack test fault */
#define IEC60730B_TEST_CLOCK_ERROR              (-6)     /* Clock test fault */
#define IEC60730B_TEST_PC_ERROR                 (-7)     /* Program counter test fault */
#define IEC60730B_TEST_CPU_PRIMASK_ERROR        (-8)     /* PRIMASK test fault */
#define IEC60730B_TEST_CPU_REG_ERROR            (-9)     /* CPU registers test fault */
#define IEC60730B_TEST_CPU_NONSTACKED_ERROR     (-10)    /* Non-stacked CPU test fault */
#define IEC60730B_TEST_CPU_FLOAT_1_ERROR        (-11)    /* Floating point registers test fault */
#define IEC60730B_TEST_CPU_FLOAT_2_ERROR        (-12)    /* Floating point registers test fault */
#define IEC60730B_TEST_CPU_CONTROL_ERROR        (-13)    /* CONTROL register test fault */
#define IEC60730B_TEST_CPU_SPECIAL_ERROR        (-14)    /* Special CPU registers test fault */
#define IEC60730B_TEST_WDT_ERROR                (-15)    /* Watchdog error */

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************
 * API
 ******************************************************************************/
/*!
 * @brief Performs CPU Registers safety tests
 * 
 * This function executes IEC 60730 Class B CPU Registers safety tests that are typically
 * run during system initialization after a reset to verify CPU functionality
 * before normal operation begins.
 * 
 * @return Test result status - 0 on success, negative on failure.
 */
int iec60730b_test_cpu_reg(void);

#ifdef __cplusplus
}
#endif

#endif /* _SAFETY_TESTS_H_ */
