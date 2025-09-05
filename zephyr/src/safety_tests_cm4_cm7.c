/*
 * Copyright 2025 NXP
 * SPDX-License-Identifier: Apache-2.0
 */

#include "safety_tests.h"

/*!
 * @brief CPU Registers test.
 */
int sf_cpu_reg_test(void)
{
    /* stacked CPU registers */
    if (FS_CM4_CM7_CPU_Register() == FS_FAIL_CPU_REGISTER) {
        return FS_TEST_CPU_REG_ERROR;
    }

    /* non-stacked CPU registers */
    if (FS_CM4_CM7_CPU_NonStackedRegister() == FS_FAIL_CPU_NONSTACKED_REGISTER){
        return FS_TEST_CPU_NONSTACKED_ERROR;
    }

    /* CONTROL */
#ifdef CONFIG_SAFETY_IEC60730B_TEST_CPU_REG_FPU
    if (FS_CM4_CM7_CPU_ControlFpu() == FS_FAIL_CPU_CONTROL){
        return FS_TEST_CPU_CONTROL_ERROR;
    }
#else
    if (FS_CM4_CM7_CPU_Control() == FS_FAIL_CPU_CONTROL){
        return FS_TEST_CPU_CONTROL_ERROR;
    }
#endif

    /* SP main */
    FS_CM4_CM7_CPU_SPmain();

    /* SP process */
    FS_CM4_CM7_CPU_SPprocess();

    /* PRIMASK */
    if (FS_CM4_CM7_CPU_Primask() == FS_FAIL_CPU_PRIMASK){
        return FS_TEST_CPU_PRIMASK_ERROR;
    }

    /* Special CPU registers  */
    if (FS_CM4_CM7_CPU_Special() == FS_FAIL_CPU_SPECIAL){
        return FS_TEST_CPU_SPECIAL_ERROR;
    }

    /* group 1 of FPU registers */
#ifdef CONFIG_SAFETY_IEC60730B_TEST_CPU_REG_FPU
    if (FS_CM4_CM7_CPU_Float1() == FS_FAIL_CPU_FLOAT_1){
        return FS_TEST_CPU_FLOAT_1_ERROR;
    }
    /* group 2 of FPU registers */
    if (FS_CM4_CM7_CPU_Float2() == FS_FAIL_CPU_FLOAT_2){
        return FS_TEST_CPU_FLOAT_2_ERROR;
    }
#endif

    return FS_TEST_OK;
}
