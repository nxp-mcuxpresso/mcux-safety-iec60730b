/*
 * Copyright 2026 NXP
 * SPDX-License-Identifier: Apache-2.0
 */

#include "iec60730b_test.h"

#ifdef CONFIG_IEC60730B_TEST_RAM

/* Pattern 0x55 used in March algorithms */
#define PATTERN_55 0x55
/* Pattern 0xAA used in March algorithms */
#define PATTERN_AA 0xAA

/* Function pointer type for RAM march test algorithms */
typedef int (*march_test_func_t)(uint8_t *start_address, size_t block_size);

/* March C algorithm for RAM testing */
static int iec60730b_test_ram_march_c(uint8_t *start_address, size_t block_size)
{
	volatile uint8_t *addr;
	volatile uint8_t read_value;
	uint8_t *end_address;

	end_address = start_address + block_size;

	/* March C - Step 1: Write 0x55 to all locations (ascending) */
	for (addr = (volatile uint8_t *)start_address;
	     addr < (volatile uint8_t *)end_address; addr++) {
		*addr = PATTERN_55;
	}

	/* March C - Step 2: Read 0x55 and write 0xAA (ascending) */
	for (addr = (volatile uint8_t *)start_address;
	     addr < (volatile uint8_t *)end_address; addr++) {
		read_value = *addr;
		if (read_value != PATTERN_55) {
			return IEC60730B_TEST_RAM_ERROR;
		}
		*addr = PATTERN_AA;
	}

	/* March C - Step 3: Read 0xAA and write 0x55 (ascending) */
	for (addr = (volatile uint8_t *)start_address;
	     addr < (volatile uint8_t *)end_address; addr++) {
		read_value = *addr;
		if (read_value != PATTERN_AA) {
			return IEC60730B_TEST_RAM_ERROR;
		}
		*addr = PATTERN_55;
	}

	/* March C - Step 4: Read 0x55 and write 0xAA (descending) */
	for (addr = (volatile uint8_t *)(end_address - 1);
	     addr >= (volatile uint8_t *)start_address; addr--) {
		read_value = *addr;
		if (read_value != PATTERN_55) {
			return IEC60730B_TEST_RAM_ERROR;
		}
		*addr = PATTERN_AA;
	}

	/* March C - Step 5: Read 0xAA and write 0x55 (descending) */
	for (addr = (volatile uint8_t *)(end_address - 1);
	     addr >= (volatile uint8_t *)start_address; addr--) {
		read_value = *addr;
		if (read_value != PATTERN_AA) {
			return IEC60730B_TEST_RAM_ERROR;
		}
		*addr = PATTERN_55;
	}

	/* March C - Step 6: Read 0x55 (descending) */
	for (addr = (volatile uint8_t *)(end_address - 1);
	     addr >= (volatile uint8_t *)start_address; addr--) {
		read_value = *addr;
		if (read_value != PATTERN_55) {
			return IEC60730B_TEST_RAM_ERROR;
		}
	}

	return IEC60730B_TEST_OK;
}

/* March X algorithm for RAM testing */
static int iec60730b_test_ram_march_x(uint8_t *start_address, size_t block_size)
{
	volatile uint8_t *addr;
	volatile uint8_t read_value;
	uint8_t *end_address;

	end_address = start_address + block_size;

	/* March X - Step 1: Write 0x55 to all locations (ascending) */
	for (addr = (volatile uint8_t *)start_address;
	     addr < (volatile uint8_t *)end_address; addr++) {
		*addr = PATTERN_55;
	}

	/* March X - Step 2: Read 0x55 and write 0xAA (ascending) */
	for (addr = (volatile uint8_t *)start_address;
	     addr < (volatile uint8_t *)end_address; addr++) {
		read_value = *addr;
		if (read_value != PATTERN_55) {
			return IEC60730B_TEST_RAM_ERROR;
		}
		*addr = PATTERN_AA;
	}

	/* March X - Step 3: Read 0xAA and write 0x55 (descending) */
	for (addr = (volatile uint8_t *)(end_address - 1);
	     addr >= (volatile uint8_t *)start_address; addr--) {
		read_value = *addr;
		if (read_value != PATTERN_AA) {
			return IEC60730B_TEST_RAM_ERROR;
		}
		*addr = PATTERN_55;
	}

	/* March X - Step 4: Read 0x55 (descending) */
	for (addr = (volatile uint8_t *)(end_address - 1);
	     addr >= (volatile uint8_t *)start_address; addr--) {
		read_value = *addr;
		if (read_value != PATTERN_55) {
			return IEC60730B_TEST_RAM_ERROR;
		}
	}

	return IEC60730B_TEST_OK;
}

/* Test RAM memory using specified algorithm for IEC 60730 Class B compliance. */
int iec60730b_test_ram(uint8_t *ram, size_t ram_size, uint8_t *backup,
		       size_t backup_size, iec60730b_test_ram_type_t type)
{
	int result;
	size_t remaining_size;
	march_test_func_t march_func;
	uint8_t *end_address = ram + ram_size;
	uint8_t *actual_address = ram;
	size_t current_block_size = backup_size;

	switch (type) {
	case IEC60730B_TEST_RAM_TYPE_MARCH_C:
		march_func = iec60730b_test_ram_march_c;
		break;
	case IEC60730B_TEST_RAM_TYPE_MARCH_X:
		march_func = iec60730b_test_ram_march_x;
		break;
	default:
		return IEC60730B_TEST_NOT_SUPPORTED;
	}

	/* March test of Backup area */
	result = march_func(backup, backup_size);
	if (result != IEC60730B_TEST_OK) {
		return IEC60730B_TEST_RAM_ERROR;
	}

	/* Tests entire RAM area in blocks */
	while (actual_address < end_address) {
		/* Copy block to Backup area */
		memcpy(backup, actual_address, current_block_size);

		/* March test of block */
		result = march_func(actual_address, current_block_size);

		/* Copy from backup to block */
		memcpy(actual_address, backup, current_block_size);

		/* Check test result */
		if (result != IEC60730B_TEST_OK) {
			return IEC60730B_TEST_RAM_ERROR;
		}

		actual_address += current_block_size;
		remaining_size = end_address - actual_address;

		/* Adjust block size for last block if needed */
		if (remaining_size < backup_size) {
			current_block_size = remaining_size;
		}
	}

	return IEC60730B_TEST_OK;
}

#endif /* CONFIG_IEC60730B_TEST_RAM */
