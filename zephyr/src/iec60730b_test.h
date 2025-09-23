/*
 * Copyright 2025 NXP
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/types.h>

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

/*!
 * @brief RAM test algorithm types for IEC 60730 Class B compliance
 * 
 * This enumeration defines the available RAM test algorithms that can be
 * used to detect memory faults as required by IEC 60730 Class B safety
 * standards.
 */
typedef enum {
    IEC60730B_TEST_RAM_TYPE_NONE = 0,    /*!< No RAM test algorithm */
    /**
     * March-C algorithm for RAM testing
     * 
     * Complexity: O(10n) - 10 operations per memory cell
     * 
     * Fault coverage:
     * - Stuck-at faults (SAF): Detects bits permanently stuck at 0 or 1
     * - Address decoder faults (AF): Detects incorrect address decoding
     * - Transition faults (TF): Detects failures when changing bit values
     * - Coupling faults (CF): Detects when one bit affects another
     * - Bridging faults (BF): Detects short circuits between memory lines
     * 
     * Recommended for IEC 60730 Class B due to comprehensive fault coverage
     */
    IEC60730B_TEST_RAM_TYPE_MARCH_C,
    /**
     * March-X algorithm for RAM testing
     * 
     * Complexity: O(6n) - 6 operations per memory cell  
     * 
     * Fault coverage:
     * - Stuck-at faults (SAF): Detects bits permanently stuck at 0 or 1
     * - Address decoder faults (AF): Detects incorrect address decoding
     * - Transition faults (TF): Detects failures when changing bit values
     * - Limited coupling faults (CF): Basic coupling fault detection
     * 
     * Faster execution than March-C but with reduced fault coverage.
     * Suitable when execution time is critical and basic fault detection
     * is sufficient for the safety requirements.
     */
    IEC60730B_TEST_RAM_TYPE_MARCH_X
} iec60730b_test_ram_type_t;


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

/*!
 * @brief Test RAM memory for IEC 60730 Class B compliance
 * 
 * This function performs comprehensive RAM testing using the specified algorithm
 * to detect memory faults such as stuck-at bits, coupling faults, and addressing
 * faults as required by IEC 60730 Class B.
 * 
 * @param ram Pointer to the RAM memory area to be tested
 * @param ram_size Size of the RAM memory area in bytes
 * @param backup Pointer to backup memory area for data preservation
 * @param backup_size Size of the backup memory area in bytes
 * @param type RAM test algorithm type to be used
 * 
 * @return 0 on success, negative on failure
 */
int iec60730b_test_ram(uint8_t *ram, size_t ram_size, uint8_t *backup, size_t backup_size, iec60730b_test_ram_type_t type);

#ifdef __cplusplus
}
#endif

#endif /* _IEC60730B_TEST_H_ */
