/*
 * Copyright 2025 NXP
 * SPDX-License-Identifier: Apache-2.0
 */

#include "safety_tests.h"

/*!
 * @brief   After-reset CPU registers test.
 *
 *          This function calls the CPU test functions from IEC60730 library.
 *          All the registers are tested at once.
 *          In case of incorrect flash test result, it updates the safetyErrors variable accordingly.
 *          See IEC60730 library documentation for CPU errors handling !
 *
 * @param   psSafetyCommon - The pointer of the Common Safety structure
 *
 * @return  None
 */
void SafetyCpuAfterResetTest(safety_common_t *psSafetyCommon)
{
    /* stacked CPU registers */
    psSafetyCommon->CPU_reg_test_result = FS_CM4_CM7_CPU_Register();
    if (psSafetyCommon->CPU_reg_test_result == FS_FAIL_CPU_REGISTER)
    {
        psSafetyCommon->safetyErrors |= CPU_REGISTERS_ERROR;
        SafetyErrorHandling(psSafetyCommon);
    }
    /* non-stacked CPU registers */
    psSafetyCommon->CPU_non_stacked_test_result = FS_CM4_CM7_CPU_NonStackedRegister();
    if (psSafetyCommon->CPU_non_stacked_test_result == FS_FAIL_CPU_NONSTACKED_REGISTER)
    {
        psSafetyCommon->safetyErrors |= CPU_NONSTACKED_ERROR;
        SafetyErrorHandling(psSafetyCommon);
    }
    /* CONTROL */
#ifdef CONFIG_SAFETY_IEC60730B_TEST_CPU_REG_FPU
    psSafetyCommon->CPU_control_test_result = FS_CM4_CM7_CPU_ControlFpu();
#else
    psSafetyCommon->CPU_control_test_result = FS_CM4_CM7_CPU_Control();
#endif

    if (psSafetyCommon->CPU_control_test_result == FS_FAIL_CPU_CONTROL)
    {
        psSafetyCommon->safetyErrors |= CPU_CONTROL_ERROR;
        SafetyErrorHandling(psSafetyCommon);
    }
    /* SP main */
    FS_CM4_CM7_CPU_SPmain();

    /* SP process */
    FS_CM4_CM7_CPU_SPprocess();

    /* PRIMASK */
    psSafetyCommon->CPU_primask_test_result = FS_CM4_CM7_CPU_Primask();
    if (psSafetyCommon->CPU_primask_test_result == FS_FAIL_CPU_PRIMASK)
    {
        psSafetyCommon->safetyErrors |= CPU_PRIMASK_ERROR;
        SafetyErrorHandling(psSafetyCommon);
    }
    /* Special CPU registers  */
    psSafetyCommon->CPU_special_test_result = FS_CM4_CM7_CPU_Special();
    if (psSafetyCommon->CPU_special_test_result == FS_FAIL_CPU_SPECIAL)
    {
        psSafetyCommon->safetyErrors |= CPU_SPECIAL_ERROR;
        SafetyErrorHandling(psSafetyCommon);
    }
    /* group 1 of FPU registers */
#ifdef CONFIG_SAFETY_IEC60730B_TEST_CPU_REG_FPU
    psSafetyCommon->CPU_float_test_1_result = FS_CM4_CM7_CPU_Float1();
    if (psSafetyCommon->CPU_float_test_1_result == FS_FAIL_CPU_FLOAT_1)
    {
        psSafetyCommon->safetyErrors |= CPU_FLOAT_1_ERROR;
        SafetyErrorHandling(psSafetyCommon);
    }
    /* group 2 of FPU registers */
    psSafetyCommon->CPU_float_test_2_result = FS_CM4_CM7_CPU_Float2();
    if (psSafetyCommon->CPU_float_test_2_result == FS_FAIL_CPU_FLOAT_2)
    {
        psSafetyCommon->safetyErrors |= CPU_FLOAT_2_ERROR;
        SafetyErrorHandling(psSafetyCommon);
    }
#endif
}

/*!
 * @brief   Interruptible test of CPU registers.
 *
 *          This function calls the CPU test functions from IEC60730 library.
 *          The function can be called from the background loop.
 *          In case of incorrect flash test result, it updates the safetyErrors variable accordingly.
 *          See IEC60730 library documentation for CPU errors handling !
 *
 * @param   psSafetyCommon - The pointer of the Common Safety structure
 *
 * @return  None
 */
void SafetyCpuBackgroundTest(safety_common_t *psSafetyCommon)
{
    psSafetyCommon->CPU_reg_test_result = FS_CM4_CM7_CPU_Register();
    if (psSafetyCommon->CPU_reg_test_result == FS_FAIL_CPU_REGISTER)
    {
        psSafetyCommon->safetyErrors |= CPU_REGISTERS_ERROR;
        SafetyErrorHandling(psSafetyCommon);
    }

    psSafetyCommon->CPU_non_stacked_test_result = FS_CM4_CM7_CPU_NonStackedRegister();
    if (psSafetyCommon->CPU_non_stacked_test_result == FS_FAIL_CPU_NONSTACKED_REGISTER)
    {
        psSafetyCommon->safetyErrors |= CPU_NONSTACKED_ERROR;
        SafetyErrorHandling(psSafetyCommon);
    }
#ifdef CONFIG_SAFETY_IEC60730B_TEST_CPU_REG_FPU
    psSafetyCommon->CPU_float_test_1_result = FS_CM4_CM7_CPU_Float1();
    if (psSafetyCommon->CPU_float_test_1_result == FS_FAIL_CPU_FLOAT_1)
    {
        psSafetyCommon->safetyErrors |= CPU_FLOAT_1_ERROR;
        SafetyErrorHandling(psSafetyCommon);
    }

    psSafetyCommon->CPU_float_test_2_result = FS_CM4_CM7_CPU_Float2();
    if (psSafetyCommon->CPU_float_test_2_result == FS_FAIL_CPU_FLOAT_2)
    {
        psSafetyCommon->safetyErrors |= CPU_FLOAT_2_ERROR;
        SafetyErrorHandling(psSafetyCommon);
    }
#endif
}
