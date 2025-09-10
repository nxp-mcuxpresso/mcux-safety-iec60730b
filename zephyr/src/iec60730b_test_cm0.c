/*
 * Copyright 2025 NXP
 * SPDX-License-Identifier: Apache-2.0
 */

#include "iec60730b_test.h"

/*!
 * @brief CPU Registers tests.
 */
int iec60730b_test_cpu_reg(void)
{
    /* stacked CPU registers */
    if (FS_CM0_CPU_Register() == FS_FAIL_CPU_REGISTER){
        return IEC60730B_TEST_CPU_REG_ERROR;
    }
    /* non-stacked CPU registers */
    if (FS_CM0_CPU_NonStackedRegister() == FS_FAIL_CPU_NONSTACKED_REGISTER){
        return IEC60730B_TEST_CPU_NONSTACKED_ERROR;
    }
    /* CONTROL */
    if (FS_CM0_CPU_Control() == FS_FAIL_CPU_CONTROL){
        return IEC60730B_TEST_CPU_CONTROL_ERROR;
    }
    /* SP main */
    FS_CM0_CPU_SPmain();

    /* SP process */
    FS_CM0_CPU_SPprocess();

    /* PRIMASK */
    if (FS_CM0_CPU_Primask() == FS_FAIL_CPU_PRIMASK){
        return IEC60730B_TEST_CPU_PRIMASK_ERROR;
    }

    return IEC60730B_TEST_OK;
}