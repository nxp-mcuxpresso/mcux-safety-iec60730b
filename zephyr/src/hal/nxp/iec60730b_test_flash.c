/*
 * Copyright 2025-2026 NXP
 * SPDX-License-Identifier: Apache-2.0
 */

#include "iec60730b_test.h"
#include <iec60730b.h>
#include <iec60730b_core.h>

#ifdef CONFIG_IEC60730B_TEST_FLASH

#ifdef CONFIG_IEC60730B_TEST_FLASH_CRC16
/*
 * Test flash memory integrity using CRC16 for IEC 60730 Class B compliance
 */
int iec60730b_test_flash_crc16(const void *start, size_t size, uint16_t crc_expected)
{
    uint16_t crc_result;

#if defined(CONFIG_CPU_CORTEX_M0PLUS)
    crc_result = FS_CM0_FLASH_SW16((uint32_t) start, size, 0, (uint16_t)CONFIG_IEC60730B_TEST_FLASH_CRC_SEED);
#elif defined(CONFIG_CPU_CORTEX_M4) || defined(CONFIG_CPU_CORTEX_M7)
    crc_result = FS_CM4_CM7_FLASH_SW16((uint32_t) start, size, 0, (uint16_t)CONFIG_IEC60730B_TEST_FLASH_CRC_SEED);
#elif defined(CONFIG_CPU_CORTEX_M33)
    crc_result = FS_CM33_FLASH_SW16((uint32_t) start, size, 0, (uint16_t)CONFIG_IEC60730B_TEST_FLASH_CRC_SEED);
#else
    return IEC60730B_TEST_NOT_SUPPORTED;
#endif

    if(crc_result != crc_expected) {
        return IEC60730B_TEST_FLASH_ERROR;
    }

    return IEC60730B_TEST_OK;
}
#endif /* CONFIG_IEC60730B_TEST_FLASH_CRC16 */

#ifdef CONFIG_IEC60730B_TEST_FLASH_CRC32
/*
 * Test flash memory integrity using CRC32 for IEC 60730 Class B compliance
 */
int iec60730b_test_flash_crc32(const void *start, size_t size, uint32_t crc_expected)
{
    uint32_t crc_result;

#if defined(CONFIG_CPU_CORTEX_M4) || defined(CONFIG_CPU_CORTEX_M7)
    crc_result = FS_CM4_CM7_FLASH_SW32((uint32_t) start, size, 0, (uint32_t)CONFIG_IEC60730B_TEST_FLASH_CRC_SEED);
#elif defined(CONFIG_CPU_CORTEX_M33)
    crc_result = FS_CM33_FLASH_SW32((uint32_t) start, size, 0, (uint32_t)CONFIG_IEC60730B_TEST_FLASH_CRC_SEED);
#else
    return IEC60730B_TEST_NOT_SUPPORTED;
#endif

    if(crc_result != crc_expected) {
        return IEC60730B_TEST_FLASH_ERROR;
    }

    return IEC60730B_TEST_OK;
}
#endif /* CONFIG_IEC60730B_TEST_FLASH_CRC32 */

#endif /* CONFIG_IEC60730B_TEST_FLASH */
