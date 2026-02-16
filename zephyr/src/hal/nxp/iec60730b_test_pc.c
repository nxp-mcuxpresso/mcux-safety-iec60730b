/*
 * Copyright 2025-2026 NXP
 * SPDX-License-Identifier: Apache-2.0
 */

#include "iec60730b_test.h"
#include <iec60730b.h>
#include <iec60730b_core.h>

#ifdef CONFIG_IEC60730B_TEST_PC
/*
 * Test Program Counter for IEC 60730 Class B compliance
 */
int iec60730b_test_pc(void)
{
    static uint32_t pc_test_in_ram;
    uint32_t pc_test_flag = 0;
    FS_RESULT res;

#if defined(CONFIG_MPU) && (defined(CONFIG_CPU_CORTEX_M0PLUS) || defined(CONFIG_CPU_CORTEX_M4) || defined(CONFIG_CPU_CORTEX_M7) || defined(CONFIG_CPU_CORTEX_M33))
    /* Temporarily disable memory protection 
     * to allow execution/modification of RAM pattern address */
    extern void arm_core_mpu_disable(void);
    arm_core_mpu_disable();
#endif

#if defined(CONFIG_CPU_CORTEX_M0PLUS)
    res = FS_CM0_PC_Test((uint32_t)&pc_test_in_ram, FS_PC_Object, &pc_test_flag);
#elif defined(CONFIG_CPU_CORTEX_M4) || defined(CONFIG_CPU_CORTEX_M7)
    res = FS_CM4_CM7_PC_Test((uint32_t)&pc_test_in_ram, FS_PC_Object, &pc_test_flag);
#elif defined(CONFIG_CPU_CORTEX_M33)
    res = FS_CM33_PC_Test((uint32_t)&pc_test_in_ram, FS_PC_Object, &pc_test_flag);
#else
    return IEC60730B_TEST_NOT_SUPPORTED;
#endif

#ifdef CONFIG_MPU
    /* Re-enable memory protection */
    extern void arm_core_mpu_enable(void);
    arm_core_mpu_enable();
#endif

    if(res == FS_FAIL_PC) {
        return IEC60730B_TEST_PC_ERROR;
    }

    return IEC60730B_TEST_OK;
}
#endif /* CONFIG_IEC60730B_TEST_PC */
