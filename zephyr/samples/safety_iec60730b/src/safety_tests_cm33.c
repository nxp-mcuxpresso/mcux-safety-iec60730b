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
#ifdef CONFIG_SAFETY_IEC60730B_TEST_CPU_REG_DSP
    /* stacked CPU registers */
    psSafetyCommon->CPU_reg_test_result = FS_CM33_CPU_Register();
    if (psSafetyCommon->CPU_reg_test_result == FS_FAIL_CPU_REGISTER)
    {
        psSafetyCommon->safetyErrors |= CPU_REGISTERS_ERROR;
        SafetyErrorHandling(psSafetyCommon);
    }
#else   
    /* stacked CPU registers */
    psSafetyCommon->CPU_reg_test_result = FS_CM33_CPU_Register_NDSP();
    if (psSafetyCommon->CPU_reg_test_result == FS_FAIL_CPU_REGISTER)
    {
        psSafetyCommon->safetyErrors |= CPU_REGISTERS_ERROR;
        SafetyErrorHandling(psSafetyCommon);
    }
#endif
    
    /* non-stacked CPU registers */
    psSafetyCommon->CPU_non_stacked_test_result = FS_CM33_CPU_NonStackedRegister();
    if (psSafetyCommon->CPU_non_stacked_test_result == FS_FAIL_CPU_NONSTACKED_REGISTER)
    {
        psSafetyCommon->safetyErrors |= CPU_NONSTACKED_ERROR;
        SafetyErrorHandling(psSafetyCommon);
    }
    
    /* PRIMASK Secure */
    psSafetyCommon->CPU_primask_test_result = FS_CM33_CPU_Primask_S();
    if (psSafetyCommon->CPU_primask_test_result == FS_FAIL_CPU_PRIMASK)
    {
        psSafetyCommon->safetyErrors |= CPU_PRIMASK_ERROR;
        SafetyErrorHandling(psSafetyCommon);
    }
    
#ifdef CONFIG_SAFETY_IEC60730B_TEST_CPU_REG_TZ
    /* PRIMASK Non-Secure */
    psSafetyCommon->CPU_primask_ns_test_result = FS_CM33_CPU_Primask_NS();
    if (psSafetyCommon->CPU_primask_ns_test_result == FS_FAIL_CPU_PRIMASK)
    {
        psSafetyCommon->safetyErrors |= CPU_PRIMASK_ERROR;
        SafetyErrorHandling(psSafetyCommon);
    }
#endif
    
    /* SP main Secure */
    FS_CM33_CPU_SPmain_S();
    
#ifdef CONFIG_SAFETY_IEC60730B_TEST_CPU_REG_TZ
    /* SP main Non-Secure */
    FS_CM33_CPU_SPmain_NS();
#endif

    /* SP main limit Secure */
    FS_CM33_CPU_SPmain_Limit_S();
    
#ifdef CONFIG_SAFETY_IEC60730B_TEST_CPU_REG_TZ
    /* SP main limit Non-Secure */
    FS_CM33_CPU_SPmain_Limit_NS();
#endif

    /* SP process Secure */
    FS_CM33_CPU_SPprocess_S();
    
#ifdef CONFIG_SAFETY_IEC60730B_TEST_CPU_REG_TZ
    /* SP process Non-Secure */
    FS_CM33_CPU_SPprocess_NS();
#endif

    /* SP process limit Secure */
    FS_CM33_CPU_SPprocess_Limit_S();
    
#ifdef CONFIG_SAFETY_IEC60730B_TEST_CPU_REG_TZ
    /* SP process limit Non-Secure */
    FS_CM33_CPU_SPprocess_Limit_NS();
#endif

    psSafetyCommon->CPU_control_test_result = FS_CM33_CPU_Control_S();
    if (psSafetyCommon->CPU_control_test_result == FS_FAIL_CPU_CONTROL)
    {
        psSafetyCommon->safetyErrors |= CPU_CONTROL_ERROR;
        SafetyErrorHandling(psSafetyCommon);
    }
    
#ifdef CONFIG_SAFETY_IEC60730B_TEST_CPU_REG_TZ
    /* CONTROL Non-Secure */
    psSafetyCommon->CPU_control_ns_test_result = FS_CM33_CPU_Control_NS();
    if (psSafetyCommon->CPU_control_ns_test_result == FS_FAIL_CPU_CONTROL)
    {
        psSafetyCommon->safetyErrors |= CPU_CONTROL_ERROR;
        SafetyErrorHandling(psSafetyCommon);
    }
#endif

    /* Special Secure */
    psSafetyCommon->CPU_special_test_result = FS_CM33_CPU_Special8PriorityLevels_S();
    if (psSafetyCommon->CPU_special_test_result == FS_FAIL_CPU_SPECIAL)
    {
        psSafetyCommon->safetyErrors |= CPU_SPECIAL_ERROR;
        SafetyErrorHandling(psSafetyCommon);
    }
    
#ifdef CONFIG_SAFETY_IEC60730B_TEST_CPU_REG_TZ
    /* Special Non-Secure */
    psSafetyCommon->CPU_special_ns_test_result = FS_CM33_CPU_Special8PriorityLevels_NS();
    if (psSafetyCommon->CPU_special_ns_test_result == FS_FAIL_CPU_SPECIAL)
    {
        psSafetyCommon->safetyErrors |= CPU_SPECIAL_ERROR;
        SafetyErrorHandling(psSafetyCommon);
    }
#endif

#ifdef CONFIG_SAFETY_IEC60730B_TEST_CPU_REG_FPU
    psSafetyCommon->CPU_fpu_test_result = FS_CM33_CPU_Float1();
    if (psSafetyCommon->CPU_fpu_test_result == FS_FAIL_CPU_FLOAT_1)
    {
        psSafetyCommon->safetyErrors |= CPU_FLOAT_1_ERROR;
        SafetyErrorHandling(psSafetyCommon);
    }

    psSafetyCommon->CPU_fpu_test_result = FS_CM33_CPU_Float2();
    if (psSafetyCommon->CPU_fpu_test_result == FS_FAIL_CPU_FLOAT_2)
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
#ifdef CONFIG_SAFETY_IEC60730B_TEST_CPU_REG_DSP
    /* stacked CPU registers */
    psSafetyCommon->CPU_reg_test_result = FS_CM33_CPU_Register();
    if (psSafetyCommon->CPU_reg_test_result == FS_FAIL_CPU_REGISTER)
    {
        psSafetyCommon->safetyErrors |= CPU_REGISTERS_ERROR;
        SafetyErrorHandling(psSafetyCommon);
    }
#else   
    /* stacked CPU registers */
    psSafetyCommon->CPU_reg_test_result = FS_CM33_CPU_Register_NDSP();
    if (psSafetyCommon->CPU_reg_test_result == FS_FAIL_CPU_REGISTER)
    {
        psSafetyCommon->safetyErrors |= CPU_REGISTERS_ERROR;
        SafetyErrorHandling(psSafetyCommon);
    }
#endif

    psSafetyCommon->CPU_non_stacked_test_result = FS_CM33_CPU_NonStackedRegister();
    if (psSafetyCommon->CPU_non_stacked_test_result == FS_FAIL_CPU_NONSTACKED_REGISTER)
    {
        psSafetyCommon->safetyErrors |= CPU_NONSTACKED_ERROR;
        SafetyErrorHandling(psSafetyCommon);
    }
    
#ifdef CONFIG_SAFETY_IEC60730B_TEST_CPU_REG_FPU
    psSafetyCommon->CPU_fpu_test_result = FS_CM33_CPU_Float1();
    if (psSafetyCommon->CPU_fpu_test_result == FS_FAIL_CPU_FLOAT_1)
    {
        psSafetyCommon->safetyErrors |= CPU_FLOAT_1_ERROR;
        SafetyErrorHandling(psSafetyCommon);
    }

    psSafetyCommon->CPU_fpu_test_result = FS_CM33_CPU_Float2();
    if (psSafetyCommon->CPU_fpu_test_result == FS_FAIL_CPU_FLOAT_2)
    {
        psSafetyCommon->safetyErrors |= CPU_FLOAT_2_ERROR;
        SafetyErrorHandling(psSafetyCommon);
    }
#endif
}
