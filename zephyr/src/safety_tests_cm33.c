/*
 * Copyright 2025 NXP
 * SPDX-License-Identifier: Apache-2.0
 */

#include "safety_tests.h"

/*!
 * @brief   After-reset CPU registers test.
 */
int sf_cpu_reg_test_init(void)
{
#ifdef CONFIG_SAFETY_IEC60730B_TEST_CPU_REG_DSP
    /* stacked CPU registers */
    if (FS_CM33_CPU_Register() == FS_FAIL_CPU_REGISTER){
        return FS_TEST_CPU_REG_ERROR;
    }
#else   
    /* stacked CPU registers */
    if (FS_CM33_CPU_Register_NDSP() == FS_FAIL_CPU_REGISTER){
        return FS_TEST_CPU_REG_ERROR;
    }
#endif

    /* non-stacked CPU registers */
    if (FS_CM33_CPU_NonStackedRegister() == FS_FAIL_CPU_NONSTACKED_REGISTER){
        return FS_TEST_CPU_NONSTACKED_ERROR;
    }
    
    /* PRIMASK Secure */
    if (FS_CM33_CPU_Primask_S() == FS_FAIL_CPU_PRIMASK){
        return FS_TEST_CPU_PRIMASK_ERROR;
    }
    
#ifdef CONFIG_SAFETY_IEC60730B_TEST_CPU_REG_TZ
    /* PRIMASK Non-Secure */
    if (FS_CM33_CPU_Primask_NS() == FS_FAIL_CPU_PRIMASK){
        return FS_TEST_CPU_PRIMASK_ERROR;
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

    if (FS_CM33_CPU_Control_S() == FS_FAIL_CPU_CONTROL){
        return FS_TEST_CPU_CONTROL_ERROR;
    }
    
#ifdef CONFIG_SAFETY_IEC60730B_TEST_CPU_REG_TZ
    /* CONTROL Non-Secure */
    if (FS_CM33_CPU_Control_NS() == FS_FAIL_CPU_CONTROL){
        return FS_TEST_CPU_CONTROL_ERROR;
    }
#endif

    /* Special Secure */
    if (FS_CM33_CPU_Special8PriorityLevels_S() == FS_FAIL_CPU_SPECIAL){
        return FS_TEST_CPU_SPECIAL_ERROR;
    }
    
#ifdef CONFIG_SAFETY_IEC60730B_TEST_CPU_REG_TZ
    /* Special Non-Secure */
    if (FS_CM33_CPU_Special8PriorityLevels_NS() == FS_FAIL_CPU_SPECIAL){
        return FS_TEST_CPU_SPECIAL_ERROR;
    }
#endif

#ifdef CONFIG_SAFETY_IEC60730B_TEST_CPU_REG_FPU
    if (FS_CM33_CPU_Float1() == FS_FAIL_CPU_FLOAT_1){
        return FS_TEST_CPU_FLOAT_1_ERROR;
    }

    if (FS_CM33_CPU_Float2() == FS_FAIL_CPU_FLOAT_2){
        return FS_TEST_CPU_FLOAT_1_ERROR;
    }
#endif

    return FS_TEST_OK;
}

/*!
 * @brief   Interruptible test of CPU registers.
 */
int sf_cpu_reg_test(void)
{
#ifdef CONFIG_SAFETY_IEC60730B_TEST_CPU_REG_DSP
    /* stacked CPU registers */
    if (FS_CM33_CPU_Register() == FS_FAIL_CPU_REGISTER){
        return FS_TEST_CPU_REG_ERROR;
    }
#else   
    /* stacked CPU registers */
    if (FS_CM33_CPU_Register_NDSP() == FS_FAIL_CPU_REGISTER){
        return FS_TEST_CPU_REG_ERROR;
    }
#endif

    if (FS_CM33_CPU_NonStackedRegister() == FS_FAIL_CPU_NONSTACKED_REGISTER){
        return FS_TEST_CPU_NONSTACKED_ERROR;
    }
    
#ifdef CONFIG_SAFETY_IEC60730B_TEST_CPU_REG_FPU
    if (FS_CM33_CPU_Float1() == FS_FAIL_CPU_FLOAT_1){
        return FS_TEST_CPU_FLOAT_1_ERROR;
    }

    if (FS_CM33_CPU_Float2() == FS_FAIL_CPU_FLOAT_2){
        return FS_TEST_CPU_FLOAT_2_ERROR;
    }
#endif

    return FS_TEST_OK;
}
