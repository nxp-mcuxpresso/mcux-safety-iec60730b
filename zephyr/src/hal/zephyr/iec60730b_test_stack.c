/*
 * Copyright 2025-2026 NXP
 * SPDX-License-Identifier: Apache-2.0
 */

#include "iec60730b_test.h"

#ifdef CONFIG_IEC60730B_TEST_STACK

/*
 * Initialize stack testing for IEC 60730 Class B compliance
 * Fills guard areas before and after the stack with a guard pattern
 */
int iec60730b_test_stack_init(void *stack_start, size_t stack_size, size_t guard_size,
			      uint32_t guard_pattern)
{
	uint32_t num_words = guard_size / sizeof(uint32_t);
	uint32_t *first_address = (uint32_t *)((uint8_t *)stack_start - sizeof(guard_pattern));
	uint32_t *second_address = (uint32_t *)((uint8_t *)stack_start + stack_size);

	/* Fill area in front of stack (going backwards) */
	for (uint32_t *addr = first_address; addr > first_address - num_words; addr--) {
		*addr = guard_pattern;
	}

	/* Fill area behind stack (going forwards) */
	for (uint32_t *addr = second_address; addr < second_address + num_words; addr++) {
		*addr = guard_pattern;
	}

	return IEC60730B_TEST_OK;
}

/*
 * Test stack integrity for IEC 60730 Class B compliance
 * Verifies that guard areas before and after the stack contain the expected pattern
 */
int iec60730b_test_stack(void *stack_start, size_t stack_size, size_t guard_size,
			 uint32_t guard_pattern)
{
	uint32_t num_words = guard_size / sizeof(uint32_t);
	uint32_t *first_address = (uint32_t *)((uint8_t *)stack_start - sizeof(guard_pattern));
	uint32_t *second_address = (uint32_t *)((uint8_t *)stack_start + stack_size);

	/* Check area in front of stack (going backwards) */
	for (uint32_t *addr = first_address; addr > first_address - num_words; addr--) {
		if (*addr != guard_pattern) {
			return IEC60730B_TEST_STACK_ERROR;
		}
	}

	/* Check area behind stack (going forwards) */
	for (uint32_t *addr = second_address; addr < second_address + num_words; addr++) {
		if (*addr != guard_pattern) {
			return IEC60730B_TEST_STACK_ERROR;
		}
	}

	return IEC60730B_TEST_OK;
}

#endif /* CONFIG_IEC60730B_TEST_STACK */
