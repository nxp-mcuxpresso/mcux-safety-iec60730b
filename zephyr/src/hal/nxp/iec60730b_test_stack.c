/*
 * Copyright 2025-2026 NXP
 * SPDX-License-Identifier: Apache-2.0
 */

#include "iec60730b_test.h"

/*
 * Some NXP SoC register headers (pulled in transitively by the Zephyr driver
 * headers above, e.g. on RT600/RT500) define a short object-like macro "CMP"
 * for the analog comparator peripheral instance. That collides with the "CMP"
 * field of the STM register-layout struct in the iec60730b bare-metal library
 * (iec60730b_types.h), breaking its compilation.
 * So drop the macro here before the library headers are included.
 */
#ifdef CMP
#undef CMP
#endif

#include <iec60730b.h>
#include <iec60730b_core.h>

#ifdef CONFIG_IEC60730B_TEST_STACK
/*
 * Initialize stack testing for IEC 60730 Class B compliance
 */
int iec60730b_test_stack_init(void *stack_start, size_t stack_size, size_t guard_size,
			      uint32_t guard_pattern)
{
#if defined(CONFIG_CPU_CORTEX_M0PLUS)
	FS_CM0_STACK_Init(guard_pattern, (uint32_t)stack_start - sizeof(guard_pattern),
			  (uint32_t)stack_start + stack_size, guard_size);
#elif defined(CONFIG_CPU_CORTEX_M4) || defined(CONFIG_CPU_CORTEX_M7)
	FS_CM4_CM7_STACK_Init(guard_pattern, (uint32_t)stack_start - sizeof(guard_pattern),
			      (uint32_t)stack_start + stack_size, guard_size);
#elif defined(CONFIG_CPU_CORTEX_M33)
	FS_CM33_STACK_Init(guard_pattern, (uint32_t)stack_start - sizeof(guard_pattern),
			   (uint32_t)stack_start + stack_size, guard_size);
#else
	return IEC60730B_TEST_NOT_SUPPORTED;
#endif

	return IEC60730B_TEST_OK;
}

/*
 * Test stack integrity for IEC 60730 Class B compliance
 */
int iec60730b_test_stack(void *stack_start, size_t stack_size, size_t guard_size,
			 uint32_t guard_pattern)
{
	FS_RESULT res;

#if defined(CONFIG_CPU_CORTEX_M0PLUS)
	res = FS_CM0_STACK_Test(guard_pattern, (uint32_t)stack_start - sizeof(guard_pattern),
				(uint32_t)stack_start + stack_size, guard_size);
#elif defined(CONFIG_CPU_CORTEX_M4) || defined(CONFIG_CPU_CORTEX_M7)
	res = FS_CM4_CM7_STACK_Test(guard_pattern, (uint32_t)stack_start - sizeof(guard_pattern),
				    (uint32_t)stack_start + stack_size, guard_size);
#elif defined(CONFIG_CPU_CORTEX_M33)
	res = FS_CM33_STACK_Test(guard_pattern, (uint32_t)stack_start - sizeof(guard_pattern),
				 (uint32_t)stack_start + stack_size, guard_size);
#else
	return IEC60730B_TEST_NOT_SUPPORTED;
#endif

	if (res == FS_FAIL_STACK) {
		return IEC60730B_TEST_STACK_ERROR;
	}

	return IEC60730B_TEST_OK;
}
#endif /* CONFIG_IEC60730B_TEST_STACK */
