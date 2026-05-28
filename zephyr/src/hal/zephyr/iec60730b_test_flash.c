/*
 * Copyright 2025-2026 NXP
 * SPDX-License-Identifier: Apache-2.0
 */

#include "iec60730b_test.h"

#ifdef CONFIG_IEC60730B_TEST_FLASH

/* Polynomial = x16 + x12 + x5 + 1 */
#define CRC16_POLYNOMIAL 0x1021U
/* Polynomial = x32 + x26 + x23 + x22 + x16 + x12 + x11 + x10 + x8 + x7 +
 *              x5 + x4 + x2 + x + 1
 */
#define CRC32_POLYNOMIAL 0x04C11DB7U

#ifdef CONFIG_IEC60730B_TEST_FLASH_CRC16
/*
 * Test flash memory integrity using CRC16 for IEC 60730 Class B compliance
 */
int iec60730b_test_flash_crc16(const void *start, size_t size, uint16_t crc_expected)
{
	uint8_t *p = (uint8_t *)start;
	uint16_t crc = (uint16_t)CONFIG_IEC60730B_TEST_FLASH_CRC_SEED;
	uint8_t i;

	while (size--) {
		crc ^= ((uint16_t)*p++ << 8);
		for (i = 8; i; i--) {
			crc = (crc & 0x8000) ? ((crc << 1) ^ CRC16_POLYNOMIAL) : (crc << 1);
		}
	}

	if (crc != crc_expected) {
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
	uint8_t *p = (uint8_t *)start;
	uint32_t crc = (uint32_t)CONFIG_IEC60730B_TEST_FLASH_CRC_SEED;
	uint8_t i;

	while (size--) {
		crc ^= ((uint32_t)*p++ << 24);
		for (i = 8; i; i--) {
			crc = (crc & 0x80000000) ? ((crc << 1) ^ CRC32_POLYNOMIAL) : (crc << 1);
		}
	}

	if (crc != crc_expected) {
		return IEC60730B_TEST_FLASH_ERROR;
	}

	return IEC60730B_TEST_OK;
}
#endif /* CONFIG_IEC60730B_TEST_FLASH_CRC32 */

#endif /* CONFIG_IEC60730B_TEST_FLASH */
