/*
 * Copyright 2025 NXP
 * SPDX-License-Identifier: Apache-2.0
 */

#include "iec60730b_test.h"
#include <iec60730b.h>
#include <iec60730b_core.h>

#ifdef CONFIG_IEC60730B_TEST_CPU_REG
/*
 * Test CPU registers for IEC 60730 Class B compliance.
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
#endif /* CONFIG_IEC60730B_TEST_CPU_REG */

#ifdef CONFIG_IEC60730B_TEST_RAM
/*
 * Test RAM memory using specified algorithm for IEC 60730 Class B compliance.
 */
int iec60730b_test_ram(uint8_t *ram, size_t ram_size, uint8_t *backup, size_t backup_size, iec60730b_test_ram_type_t type)
{
    tFcn march_type;

    switch (type) {
        case IEC60730B_TEST_RAM_TYPE_MARCH_C:
            march_type = FS_CM4_CM7_RAM_SegmentMarchC;
            break;
        case IEC60730B_TEST_RAM_TYPE_MARCH_X:
            march_type = FS_CM4_CM7_RAM_SegmentMarchX;
            break;
        default:
            return IEC60730B_TEST_ERROR; /* Not supported */
    }

    if (FS_CM4_CM7_RAM_AfterReset((uint32_t)ram, (uint32_t)(ram + ram_size), (uint32_t)backup_size, (uint32_t)backup, march_type) == FS_FAIL_RAM){
        return IEC60730B_TEST_RAM_ERROR;
    }

    return IEC60730B_TEST_OK;
}
#endif /* CONFIG_IEC60730B_TEST_RAM */