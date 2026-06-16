/*
 * Copyright 2025 NXP
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

#ifdef CONFIG_IEC60730B_TEST_FPU
/*
 * Test FPU registers for IEC 60730 Class B compliance
 */
int iec60730b_test_fpu(void)
{
#if defined(CONFIG_CPU_CORTEX_M4) || defined(CONFIG_CPU_CORTEX_M7)

	/* group 1 of FPU registers */
	if (FS_CM4_CM7_CPU_Float1() == FS_FAIL_CPU_FLOAT_1) {
		return IEC60730B_TEST_FPU_ERROR;
	}
	/* group 2 of FPU registers */
	if (FS_CM4_CM7_CPU_Float2() == FS_FAIL_CPU_FLOAT_2) {
		return IEC60730B_TEST_FPU_ERROR;
	}

#elif defined(CONFIG_CPU_CORTEX_M33)

	if (FS_CM33_CPU_Float1() == FS_FAIL_CPU_FLOAT_1) {
		return IEC60730B_TEST_FPU_ERROR;
	}

	if (FS_CM33_CPU_Float2() == FS_FAIL_CPU_FLOAT_2) {
		return IEC60730B_TEST_FPU_ERROR;
	}

#else
	return IEC60730B_TEST_NOT_SUPPORTED;
#endif

	return IEC60730B_TEST_OK;
}
#endif /* CONFIG_IEC60730B_TEST_FPU */
