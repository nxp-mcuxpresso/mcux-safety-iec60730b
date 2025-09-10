/*
 * Copyright 2025 NXP
 * SPDX-License-Identifier: Apache-2.0
 */

#include "iec60730b_test.h"
#include <iec60730b.h>
#include <iec60730b_core.h>

/*!
 * @brief Test CPU registers for IEC 60730 Class B compliance.
 */
int iec60730b_test_cpu_reg(void)
{
    /* stacked CPU registers */
    if (FS_CM4_CM7_CPU_Register() == FS_FAIL_CPU_REGISTER) {
        return IEC60730B_TEST_CPU_REG_ERROR;
    }

    /* non-stacked CPU registers */
    if (FS_CM4_CM7_CPU_NonStackedRegister() == FS_FAIL_CPU_NONSTACKED_REGISTER){
        return IEC60730B_TEST_CPU_REG_NONSTACKED_ERROR;
    }

    /* CONTROL */
#ifdef CONFIG_IEC60730B_TEST_CPU_REG_FPU
    if (FS_CM4_CM7_CPU_ControlFpu() == FS_FAIL_CPU_CONTROL){
        return IEC60730B_TEST_CPU_REG_CONTROL_ERROR;
    }
#else
    if (FS_CM4_CM7_CPU_Control() == FS_FAIL_CPU_CONTROL){
        return IEC60730B_TEST_CPU_REG_CONTROL_ERROR;
    }
#endif

    /* SP main */
    FS_CM4_CM7_CPU_SPmain();

    /* SP process */
    FS_CM4_CM7_CPU_SPprocess();

    /* PRIMASK */
    if (FS_CM4_CM7_CPU_Primask() == FS_FAIL_CPU_PRIMASK){
        return IEC60730B_TEST_CPU_REG_PRIMASK_ERROR;
    }

    /* Special CPU registers  */
    if (FS_CM4_CM7_CPU_Special() == FS_FAIL_CPU_SPECIAL){
        return IEC60730B_TEST_CPU_REG_SPECIAL_ERROR;
    }

    /* group 1 of FPU registers */
#ifdef CONFIG_IEC60730B_TEST_CPU_REG_FPU
    if (FS_CM4_CM7_CPU_Float1() == FS_FAIL_CPU_FLOAT_1){
        return IEC60730B_TEST_CPU_REG_FLOAT_ERROR;
    }
    /* group 2 of FPU registers */
    if (FS_CM4_CM7_CPU_Float2() == FS_FAIL_CPU_FLOAT_2){
        return IEC60730B_TEST_CPU_REG_FLOAT_ERROR;
    }
#endif

    return IEC60730B_TEST_OK;
}
