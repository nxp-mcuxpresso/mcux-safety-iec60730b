/*
 * Copyright 2025-2026 NXP
 * SPDX-License-Identifier: Apache-2.0
 */

#include "iec60730b_test.h"
#include <iec60730b.h>
#include <iec60730b_core.h>

#if defined(CONFIG_IEC60730B_TEST_PC) && (defined(CONFIG_CPU_CORTEX_M0PLUS) || defined(CONFIG_CPU_CORTEX_M4) || defined(CONFIG_CPU_CORTEX_M7) || defined(CONFIG_CPU_CORTEX_M33))
/*
 * Test Program Counter for IEC 60730 Class B compliance
 */
int iec60730b_test_pc(void)
{
    static uint32_t pc_test_in_ram;
    uint32_t pc_test_flag = 0xFFFFFFFF;
    FS_RESULT res;

#if defined(CONFIG_MPU)
    /* Temporarily disable memory protection 
     * to allow execution/modification of RAM pattern address */
    extern void arm_core_mpu_disable(void);
    arm_core_mpu_disable();
#endif

#if defined(CONFIG_CPU_CORTEX_M0PLUS)
    res = FS_CM0_PC_Test((uint32_t)&pc_test_in_ram, FS_PC_Object, &pc_test_flag);
#elif defined(CONFIG_CPU_CORTEX_M4) || defined(CONFIG_CPU_CORTEX_M7)
    #if defined(CONFIG_CPU_CORTEX_M7)
    /* Cortex-M7 has separate I-cache and D-cache. The PC test copies a code
     * pattern into RAM via the D-cache, then branches to it. Without cache
     * maintenance the I-cache serves stale data at that address, causing an
     * undefined instruction UsageFault.
     * Clean D-cache (flush dirty lines to RAM) then disable I-cache so
     * the CPU fetches the freshly written code directly from RAM.
     */
    bool icache_was_enabled = (SCB->CCR & SCB_CCR_IC_Msk) != 0U;
    bool dcache_was_enabled = (SCB->CCR & SCB_CCR_DC_Msk) != 0U;
    if (dcache_was_enabled) {
        SCB_CleanDCache(); /* flush dirty D-cache lines to RAM */
    }
    if (icache_was_enabled) {
        SCB_DisableICache(); /* invalidate + disable I-cache */
    }
    #endif
    res = FS_CM4_CM7_PC_Test((uint32_t)&pc_test_in_ram, FS_PC_Object, &pc_test_flag);
    #if defined(CONFIG_CPU_CORTEX_M7)
    if (icache_was_enabled) {
        SCB_EnableICache(); /* restore I-cache only if it was enabled before */
    }
    #endif
#elif defined(CONFIG_CPU_CORTEX_M33)
    res = FS_CM33_PC_Test((uint32_t)&pc_test_in_ram, FS_PC_Object, &pc_test_flag);
#endif

#if defined(CONFIG_MPU)
    /* Re-enable memory protection */
    extern void arm_core_mpu_enable(void);
    arm_core_mpu_enable();
#endif

    /* Check if PC test failed or flag was not cleared */
    if((res == FS_FAIL_PC) || (pc_test_flag != 0)) {
        return IEC60730B_TEST_PC_ERROR;
    }

    return IEC60730B_TEST_OK;
}
#endif /* CONFIG_IEC60730B_TEST_PC */
