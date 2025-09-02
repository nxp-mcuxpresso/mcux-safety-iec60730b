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
#define FS_TEST_OK                       0x0     /* No error */
#define FS_TEST_CPU_ERROR                0x1     /* CPU test fault flag */
#define FS_TEST_FLASH_ERROR              0x2     /* Flash test fault flag */
#define FS_TEST_RAM_ERROR                0x4     /* RAM test fault flag */
#define FS_TEST_STACK_ERROR              0x8     /* Stack test fault flag */
#define FS_TEST_CLOCK_ERROR              0x10    /* Clock test fault flag */
#define FS_TEST_PC_ERROR                 0x20    /* Program counter test fault flag */
#define FS_TEST_CPU_PRIMASK_ERROR        0x40    /* PRIMASK test fault flag */
#define FS_TEST_CPU_REG_ERROR            0x80    /* CPU registers test fault flag */
#define FS_TEST_CPU_NONSTACKED_ERROR     0x100   /* non-stacked CPU test fault flag */
#define FS_TEST_CPU_FLOAT_1_ERROR        0x200   /* floating point registers test fault flag */
#define FS_TEST_CPU_FLOAT_2_ERROR        0x400   /* floating point registers test fault flag */
#define FS_TEST_CPU_CONTROL_ERROR        0x800   /* CONTROL register test fault flag */
#define FS_TEST_CPU_SPECIAL_ERROR        0x1000  /* special CPU registers test fault flag */

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************
 * API
 ******************************************************************************/
/*!
 * @brief Performs CPU safety tests that should be executed after system reset
 * 
 * This function executes IEC 60730 Class B CPU safety tests that are typically
 * run during system initialization after a reset to verify CPU functionality
 * before normal operation begins.
 * 
 * @return Test result status - FS_TEST_OK on success, or an error flag on failure
 */
int sf_cpu_reg_test_init(void);

/*!
 * @brief Performs CPU safety tests that should be executed during background operation
 * 
 * This function executes IEC 60730 Class B CPU safety tests that are typically
 * run periodically during normal system operation to continuously verify CPU 
 * functionality and detect potential faults.
 * 
 * @return Test result status - FS_TEST_OK on success, or an error flag on failure
 */
int sf_cpu_reg_test(void);


#ifdef __cplusplus
}
#endif

#endif /* _SAFETY_CM33_IMXRT_H_ */
