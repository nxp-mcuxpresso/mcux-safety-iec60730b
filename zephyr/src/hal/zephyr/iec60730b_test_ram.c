/*
 * Copyright 2026 NXP
 * SPDX-License-Identifier: Apache-2.0
 */

#ifdef CONFIG_IEC60730B_TEST_RAM

#include "iec60730b_test.h"
#include <zephyr/sys/__assert.h>

/* Pattern 0x55 used in March algorithms */
#define PATTERN_55 0x55
/* Pattern 0xAA used in March algorithms */
#define PATTERN_AA 0xAA

/* Per-block March algorithm signature shared by ram_march_c / ram_march_x. */
typedef int (*march_fn)(uint8_t *start_address, size_t block_size);

/* March C algorithm for RAM testing */
static int ram_march_c(uint8_t *start_address, size_t block_size)
{
	volatile uint8_t *addr;
	volatile uint8_t *const base = start_address;
	volatile uint8_t *const end_address = base + block_size;

	/* March C - Step 1: Write 0x55 to all locations (ascending) */
	for (addr = base; addr < end_address; addr++) {
		*addr = PATTERN_55;
	}

	/* March C - Step 2: Read 0x55 and write 0xAA (ascending) */
	for (addr = base; addr < end_address; addr++) {
		if (*addr != PATTERN_55) {
			return IEC60730B_TEST_RAM_ERROR;
		}
		*addr = PATTERN_AA;
	}

	/* March C - Step 3: Read 0xAA and write 0x55 (ascending) */
	for (addr = base; addr < end_address; addr++) {
		if (*addr != PATTERN_AA) {
			return IEC60730B_TEST_RAM_ERROR;
		}
		*addr = PATTERN_55;
	}

	/* March C - Step 4: Read 0x55 and write 0xAA (descending) */
	for (addr = end_address; addr-- > base; ) {
		if (*addr != PATTERN_55) {
			return IEC60730B_TEST_RAM_ERROR;
		}
		*addr = PATTERN_AA;
	}

	/* March C - Step 5: Read 0xAA and write 0x55 (descending) */
	for (addr = end_address; addr-- > base; ) {
		if (*addr != PATTERN_AA) {
			return IEC60730B_TEST_RAM_ERROR;
		}
		*addr = PATTERN_55;
	}

	/* March C - Step 6: Read 0x55 (descending) */
	for (addr = end_address; addr-- > base; ) {
		if (*addr != PATTERN_55) {
			return IEC60730B_TEST_RAM_ERROR;
		}
	}

	return IEC60730B_TEST_OK;
}

/* March X algorithm for RAM testing */
static int ram_march_x(uint8_t *start_address, size_t block_size)
{
	volatile uint8_t *addr;
	volatile uint8_t *const base = start_address;
	volatile uint8_t *const end_address = base + block_size;

	/* March X - Step 1: Write 0x55 to all locations (ascending) */
	for (addr = base; addr < end_address; addr++) {
		*addr = PATTERN_55;
	}

	/* March X - Step 2: Read 0x55 and write 0xAA (ascending) */
	for (addr = base; addr < end_address; addr++) {
		if (*addr != PATTERN_55) {
			return IEC60730B_TEST_RAM_ERROR;
		}
		*addr = PATTERN_AA;
	}

	/* March X - Step 3: Read 0xAA and write 0x55 (descending) */
	for (addr = end_address; addr-- > base; ) {
		if (*addr != PATTERN_AA) {
			return IEC60730B_TEST_RAM_ERROR;
		}
		*addr = PATTERN_55;
	}

	/* March X - Step 4: Read 0x55 (descending) */
	for (addr = end_address; addr-- > base; ) {
		if (*addr != PATTERN_55) {
			return IEC60730B_TEST_RAM_ERROR;
		}
	}

	return IEC60730B_TEST_OK;
}

/* Shared worker for the March-C and March-X algorithms. */
static int test_ram_march(uint8_t *ram, size_t ram_size, uint8_t *backup, size_t backup_size,
			  march_fn march_algo)
{
	int result;
	size_t offset;

	if (ram == NULL || backup == NULL || backup_size == 0 || ram_size == 0) {
		return IEC60730B_TEST_ERROR;
	}

	/* The backup area is used as scratch storage and any overlap with the
	 * tested RAM range would silently corrupt either the test or the saved
	 * data. Caller error - flagged at build/dev time via assert.
	 */
	__ASSERT_NO_MSG(((uintptr_t)backup + backup_size <= (uintptr_t)ram) ||
			((uintptr_t)ram + ram_size <= (uintptr_t)backup));

	/* March test of Backup area */
	result = march_algo(backup, backup_size);
	if (result != IEC60730B_TEST_OK) {
		return result;
	}

	/* Tests entire RAM area in blocks */
	for (offset = 0; offset < ram_size; ) {
		size_t block = MIN(backup_size, ram_size - offset);

		/* Copy block to backup area */
		memcpy(backup, ram + offset, block);

		/* March test of block */
		result = march_algo(ram + offset, block);

		/* Copy from backup to block */
		memcpy(ram + offset, backup, block);

		/* Check test result */
		if (result != IEC60730B_TEST_OK) {
			return result;
		}

		offset += block;
	}

	return IEC60730B_TEST_OK;
}

/* Test RAM memory using March C algorithm for IEC 60730 Class B compliance. */
int iec60730b_test_ram_march_c(uint8_t *ram, size_t ram_size, uint8_t *backup, size_t backup_size)
{
	return test_ram_march(ram, ram_size, backup, backup_size, ram_march_c);
}

/* Test RAM memory using March X algorithm for IEC 60730 Class B compliance. */
int iec60730b_test_ram_march_x(uint8_t *ram, size_t ram_size, uint8_t *backup, size_t backup_size)
{
	return test_ram_march(ram, ram_size, backup, backup_size, ram_march_x);
}

#endif /* CONFIG_IEC60730B_TEST_RAM */
