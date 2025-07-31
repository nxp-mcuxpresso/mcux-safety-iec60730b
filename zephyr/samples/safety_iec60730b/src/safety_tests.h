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
#define CPU_TEST_ERROR           0x1     /* CPU test fault flag */
#define FLASH_TEST_ERROR         0x2     /* Flash test fault flag */
#define RAM_TEST_ERROR           0x4     /* RAM test fault flag */
#define STACK_TEST_ERROR         0x8     /* Stack test fault flag */
#define CLOCK_TEST_ERROR         0x10    /* Clock test fault flag */
#define PC_TEST_ERROR            0x20    /* Program counter test fault flag */
#define CPU_PRIMASK_ERROR        0x40    /* PRIMASK test fault flag */
#define CPU_REGISTERS_ERROR      0x80    /* CPU registers test fault flag */
#define CPU_NONSTACKED_ERROR     0x100   /* non-stacked CPU test fault flag */
#define CPU_FLOAT_1_ERROR        0x200   /* floating point registers test fault flag */
#define CPU_FLOAT_2_ERROR        0x400   /* floating point registers test fault flag */
#define CPU_CONTROL_ERROR        0x800   /* CONTROL register test fault flag */
#define CPU_SPECIAL_ERROR        0x1000  /* special CPU registers test fault flag */

#define FS_FLASH_PASS     0x0        /* Flash test pass return */
#define FS_FLASH_FAIL     0x00000301 /* Flash test fail return */
#define FS_FLASH_PROGRESS 0x00000302 /* Flash test inprogress  */

/*! @brief Safety tests */
typedef struct _safety_common
{
    uint32_t fastIsrSafetySwitch;
    uint32_t safetyErrors;
    uint32_t CLOCK_test_result;
    uint32_t FLASH_test_result;
    uint32_t RAM_test_result;
    uint32_t PC_test_result;
    uint32_t CPU_primask_test_result;
    uint32_t CPU_reg_test_result;
    uint32_t CPU_non_stacked_test_result;
    uint32_t CPU_control_test_result;
    uint32_t CPU_fpu_test_result;
    uint32_t CPU_special_test_result;
    uint32_t CPU_float_test_1_result;
    uint32_t CPU_float_test_2_result;
    uint32_t STACK_test_result;
    uint32_t refClkFreq;
#if TZ_SUPPORT
    uint32_t CPU_primask_ns_test_result;
    uint32_t CPU_control_ns_test_result;
    uint32_t CPU_special_ns_test_result;
#endif
} safety_common_t;

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * @name Safety Class B tests handling functions
 * @{
 */

/*******************************************************************************
 * API
 ******************************************************************************/
void SafetyCpuAfterResetTest(safety_common_t *psSafetyCommon);
void SafetyCpuBackgroundTest(safety_common_t *psSafetyCommon);

void SafetyErrorHandling(safety_common_t *psSafetyCommon);

#ifdef __cplusplus
}
#endif

#endif /* _SAFETY_CM33_IMXRT_H_ */
