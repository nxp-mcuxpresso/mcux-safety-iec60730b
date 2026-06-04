/*
 * Copyright 2025-2026 NXP
 * SPDX-License-Identifier: Apache-2.0
 */

#ifdef CONFIG_IEC60730B_TEST_RAM
#include "iec60730b_test.h"
#include <iec60730b.h>
#include <iec60730b_core.h>
#include <zephyr/sys/__assert.h>

/*
 * Resolve the per-core RAM test function and March-algorithm selectors.
 * On unsupported cores TEST_RAM_MARCH_SUPPORTED stays 0 and the public
 * functions return IEC60730B_TEST_NOT_SUPPORTED at runtime.
 */
#if defined(CONFIG_CPU_CORTEX_M0PLUS)
#define TEST_RAM_MARCH                FS_CM0_RAM_AfterReset
#define TEST_RAM_MARCH_C              FS_CM0_RAM_SegmentMarchC
#define TEST_RAM_MARCH_X              FS_CM0_RAM_SegmentMarchX
#define TEST_RAM_MARCH_SUPPORTED      1
#elif defined(CONFIG_CPU_CORTEX_M4) || defined(CONFIG_CPU_CORTEX_M7)
#define TEST_RAM_MARCH                FS_CM4_CM7_RAM_AfterReset
#define TEST_RAM_MARCH_C              FS_CM4_CM7_RAM_SegmentMarchC
#define TEST_RAM_MARCH_X              FS_CM4_CM7_RAM_SegmentMarchX
#define TEST_RAM_MARCH_SUPPORTED      1
#elif defined(CONFIG_CPU_CORTEX_M33)
#define TEST_RAM_MARCH                FS_CM33_RAM_AfterReset
#define TEST_RAM_MARCH_C              FS_CM33_RAM_SegmentMarchC
#define TEST_RAM_MARCH_X              FS_CM33_RAM_SegmentMarchX
#define TEST_RAM_MARCH_SUPPORTED      1
#else
#define TEST_RAM_MARCH_SUPPORTED      0
#endif

#if TEST_RAM_MARCH_SUPPORTED
/* Shared worker for the March-C and March-X algorithms. */
static int test_ram_march(uint8_t *ram, size_t ram_size, uint8_t *backup, size_t backup_size,
			  tFcn march_algo)
{
	size_t block_size;

	if (ram == NULL || backup == NULL || ram_size == 0 || backup_size == 0) {
		return IEC60730B_TEST_ERROR;
	}

	block_size = MIN(backup_size, ram_size);

	/* The backup area is used as scratch storage and any overlap with the
	 * tested RAM range would silently corrupt either the test or the saved
	 * data.
	 */
	__ASSERT_NO_MSG(((uintptr_t)backup + backup_size <= (uintptr_t)ram) ||
			((uintptr_t)ram + ram_size <= (uintptr_t)backup));

	FS_RESULT res = TEST_RAM_MARCH((uint32_t)ram, (uint32_t)(ram + ram_size),
				       (uint32_t)block_size, (uint32_t)backup, march_algo);

	if (res != FS_PASS) {
		return IEC60730B_TEST_RAM_ERROR;
	}

	return IEC60730B_TEST_OK;
}
#endif /* TEST_RAM_MARCH_SUPPORTED */

/* Test RAM memory using March C algorithm for IEC 60730 Class B compliance. */
int iec60730b_test_ram_march_c(uint8_t *ram, size_t ram_size, uint8_t *backup, size_t backup_size)
{
#if TEST_RAM_MARCH_SUPPORTED
	return test_ram_march(ram, ram_size, backup, backup_size, TEST_RAM_MARCH_C);
#else
	ARG_UNUSED(ram); ARG_UNUSED(ram_size); ARG_UNUSED(backup); ARG_UNUSED(backup_size);
	return IEC60730B_TEST_NOT_SUPPORTED;
#endif
}

/* Test RAM memory using March X algorithm for IEC 60730 Class B compliance. */
int iec60730b_test_ram_march_x(uint8_t *ram, size_t ram_size, uint8_t *backup, size_t backup_size)
{
#if TEST_RAM_MARCH_SUPPORTED
	return test_ram_march(ram, ram_size, backup, backup_size, TEST_RAM_MARCH_X);
#else
	ARG_UNUSED(ram); ARG_UNUSED(ram_size); ARG_UNUSED(backup); ARG_UNUSED(backup_size);
	return IEC60730B_TEST_NOT_SUPPORTED;
#endif
}
#endif /* CONFIG_IEC60730B_TEST_RAM */
