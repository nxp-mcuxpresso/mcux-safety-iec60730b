/*
 * Copyright 2025 NXP
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef _SAFETY_CM33_IMXRT_H_
#define _SAFETY_CM33_IMXRT_H_

#include "iec60730b.h"
#include "iec60730b_core.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*!
 * @name Safety test error flags
 * @brief Error flag definitions for IEC 60730 Class B safety tests
 * 
 * These flags are used to indicate which safety tests have failed.
 * Multiple flags can be combined using bitwise OR operations to represent multiple test failures.
 */
#define FS_TEST_OK                       (0)       /* No error */
#define FS_TEST_CPU_ERROR                (1)    /* CPU test fault */
#define FS_TEST_FLASH_ERROR              (2)    /* Flash test fault */
#define FS_TEST_RAM_ERROR                (3)    /* RAM test fault */
#define FS_TEST_STACK_ERROR              (4)    /* Stack test fault */
#define FS_TEST_CLOCK_ERROR              (5)    /* Clock test fault */
#define FS_TEST_PC_ERROR                 (6)    /* Program counter test fault */
#define FS_TEST_CPU_PRIMASK_ERROR        (7)    /* PRIMASK test fault */
#define FS_TEST_CPU_REG_ERROR            (8)    /* CPU registers test fault */
#define FS_TEST_CPU_NONSTACKED_ERROR     (9)    /* Non-stacked CPU test fault */
#define FS_TEST_CPU_FLOAT_1_ERROR        (10)   /* Floating point registers test fault */
#define FS_TEST_CPU_FLOAT_2_ERROR        (11)   /* Floating point registers test fault */
#define FS_TEST_CPU_CONTROL_ERROR        (12)   /* CONTROL register test fault */
#define FS_TEST_CPU_SPECIAL_ERROR        (13)   /* Special CPU registers test fault */

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
 * @return Test result status - 0 on success, non-zero on failure.
 */
int sf_cpu_reg_test(void);

#ifdef __cplusplus
}
#endif

#endif /* _SAFETY_CM33_IMXRT_H_ */
