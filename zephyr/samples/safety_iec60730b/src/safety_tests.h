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
 * These flags are used in the safetyErrors field of the safety_common_t structure
 * to indicate which safety tests have failed. Multiple flags can be combined using
 * bitwise OR operations to represent multiple test failures.
 */
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

/*!
 * @brief Safety common structure containing test results and error flags
 * 
 * This structure holds the safety test results and error status for various
 * IEC 60730 Class B safety tests including CPU register tests, floating point
 * unit tests, and TrustZone tests when enabled.
 */
typedef struct _safety_common
{
    uint32_t safetyErrors;          /*!< Bitfield containing safety test error flags */
#ifdef CONFIG_SAFETY_IEC60730B_TEST_CPU_REG
    uint32_t CPU_primask_test_result;   /*!< Result of PRIMASK register safety test */
    uint32_t CPU_reg_test_result;       /*!< Result of CPU registers safety test */
    uint32_t CPU_non_stacked_test_result; /*!< Result of non-stacked CPU registers safety test */
    uint32_t CPU_control_test_result;   /*!< Result of CONTROL register safety test */
    uint32_t CPU_special_test_result;   /*!< Result of special CPU registers safety test */
#ifdef CONFIG_SAFETY_IEC60730B_TEST_CPU_REG_FPU
    uint32_t CPU_fpu_test_result;       /*!< Result of floating point unit safety test */
    uint32_t CPU_float_test_1_result;   /*!< Result of floating point registers safety test 1 */
    uint32_t CPU_float_test_2_result;   /*!< Result of floating point registers safety test 2 */
#endif    
#ifdef CONFIG_SAFETY_IEC60730B_TEST_CPU_REG_TZ
    uint32_t CPU_primask_ns_test_result;    /*!< Result of non-secure PRIMASK register safety test */
    uint32_t CPU_control_ns_test_result;    /*!< Result of non-secure CONTROL register safety test */
    uint32_t CPU_special_ns_test_result;    /*!< Result of non-secure special CPU registers safety test */
#endif
#endif /* CONFIG_SAFETY_IEC60730B_TEST_CPU_REG */
    //TBD uint32_t fastIsrSafetySwitch;
    //TBD uint32_t CLOCK_test_result;
    //TBD uint32_t FLASH_test_result;
    //TBD uint32_t RAM_test_result;
    //TBD uint32_t PC_test_result;
    //TBD uint32_t STACK_test_result;
    //TBD uint32_t refClkFreq;
} safety_common_t;

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
 * @param psSafetyCommon Pointer to safety common structure to store test results
 */
void SafetyCpuAfterResetTest(safety_common_t *psSafetyCommon);
/*!
 * @brief Performs CPU safety tests that should be executed during background operation
 * 
 * This function executes IEC 60730 Class B CPU safety tests that are typically
 * run periodically during normal system operation to continuously verify CPU 
 * functionality and detect potential faults.
 * 
 * @param psSafetyCommon Pointer to safety common structure to store test results
 */
void SafetyCpuBackgroundTest(safety_common_t *psSafetyCommon);

/*!
 * @brief Handles safety errors detected during IEC 60730 Class B tests
 * 
 * This function processes safety errors that occur during the execution of
 * IEC 60730 Class B safety tests and implements appropriate error handling
 * procedures based on the detected fault conditions.
 * 
 * @param psSafetyCommon Pointer to safety common structure containing test results and error information
 */
void SafetyErrorHandling(safety_common_t *psSafetyCommon);

#ifdef __cplusplus
}
#endif

#endif /* _SAFETY_CM33_IMXRT_H_ */
