/*
 * Copyright 2025-2026 NXP
 * SPDX-License-Identifier: Apache-2.0
 */

#include "iec60730b_test.h"
#include <zephyr/sys/crc.h>

#ifdef CONFIG_IEC60730B_TEST_FLASH

#ifdef CONFIG_IEC60730B_TEST_FLASH_CRC16
/*
 * Test flash memory integrity using CRC16 for IEC 60730 Class B compliance.
 *
 * Algorithm: MSB-first, polynomial 0x1021, seed CONFIG_IEC60730B_TEST_FLASH_CRC16_SEED,
 * no reflection, no XOR-out. With the default seed of 0xFFFF this matches
 * CRC-16/CCITT-FALSE, equivalent to Zephyr's crc16_itu_t().
 */
int iec60730b_test_flash_crc16(const void *start, size_t size, uint16_t crc_expected)
{
	uint16_t crc = crc16_itu_t((uint16_t)CONFIG_IEC60730B_TEST_FLASH_CRC16_SEED,
				   (const uint8_t *)start, size);

	if (crc != crc_expected) {
		return IEC60730B_TEST_FLASH_ERROR;
	}

	return IEC60730B_TEST_OK;
}
#endif /* CONFIG_IEC60730B_TEST_FLASH_CRC16 */

#ifdef CONFIG_IEC60730B_TEST_FLASH_CRC32
/*
 * Test flash memory integrity using CRC32 for IEC 60730 Class B compliance.
 *
 * Algorithm: MSB-first, polynomial 0x04C11DB7, seed CONFIG_IEC60730B_TEST_FLASH_CRC32_SEED,
 * no reflection, no XOR-out. With the default seed of 0xFFFFFFFF this matches
 * CRC-32/MPEG-2, equivalent to Zephyr's crc32_mpeg2().
 */
int iec60730b_test_flash_crc32(const void *start, size_t size, uint32_t crc_expected)
{
	uint32_t crc = crc32_mpeg2_update((uint32_t)CONFIG_IEC60730B_TEST_FLASH_CRC32_SEED,
					  (const uint8_t *)start, size);

	if (crc != crc_expected) {
		return IEC60730B_TEST_FLASH_ERROR;
	}

	return IEC60730B_TEST_OK;
}
#endif /* CONFIG_IEC60730B_TEST_FLASH_CRC32 */

#endif /* CONFIG_IEC60730B_TEST_FLASH */
