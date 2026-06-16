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

#ifdef CONFIG_IEC60730B_TEST_CPU
/*
 * Test CPU registers for IEC 60730 Class B compliance
 */
int iec60730b_test_cpu(void)
{
#if defined(CONFIG_CPU_CORTEX_M0PLUS)

	/* stacked CPU registers */
	if (FS_CM0_CPU_Register() == FS_FAIL_CPU_REGISTER) {
		return IEC60730B_TEST_CPU_ERROR;
	}
	/* non-stacked CPU registers */
	if (FS_CM0_CPU_NonStackedRegister() == FS_FAIL_CPU_NONSTACKED_REGISTER) {
		return IEC60730B_TEST_CPU_ERROR;
	}
	/* CONTROL */
	if (FS_CM0_CPU_Control() == FS_FAIL_CPU_CONTROL) {
		return IEC60730B_TEST_CPU_ERROR;
	}
	/* SP main */
	FS_CM0_CPU_SPmain();

	/* SP process */
	FS_CM0_CPU_SPprocess();

	/* PRIMASK */
	if (FS_CM0_CPU_Primask() == FS_FAIL_CPU_PRIMASK) {
		return IEC60730B_TEST_CPU_ERROR;
	}

#elif defined(CONFIG_CPU_CORTEX_M4) || defined(CONFIG_CPU_CORTEX_M7)

	/* stacked CPU registers */
	if (FS_CM4_CM7_CPU_Register() == FS_FAIL_CPU_REGISTER) {
		return IEC60730B_TEST_CPU_ERROR;
	}

	/* non-stacked CPU registers */
	if (FS_CM4_CM7_CPU_NonStackedRegister() == FS_FAIL_CPU_NONSTACKED_REGISTER) {
		return IEC60730B_TEST_CPU_ERROR;
	}

	/* CONTROL */
#ifdef CONFIG_CPU_HAS_FPU
	if (FS_CM4_CM7_CPU_ControlFpu() == FS_FAIL_CPU_CONTROL) {
		return IEC60730B_TEST_CPU_ERROR;
	}
#else
	if (FS_CM4_CM7_CPU_Control() == FS_FAIL_CPU_CONTROL) {
		return IEC60730B_TEST_CPU_ERROR;
	}
#endif

	/* SP main */
	FS_CM4_CM7_CPU_SPmain();

	/* SP process */
	FS_CM4_CM7_CPU_SPprocess();

	/* PRIMASK */
	if (FS_CM4_CM7_CPU_Primask() == FS_FAIL_CPU_PRIMASK) {
		return IEC60730B_TEST_CPU_ERROR;
	}

	/* Special CPU registers  */
	if (FS_CM4_CM7_CPU_Special() == FS_FAIL_CPU_SPECIAL) {
		return IEC60730B_TEST_CPU_ERROR;
	}

#elif defined(CONFIG_CPU_CORTEX_M33)

	/* stacked CPU registers */
#ifdef CONFIG_ARMV8_M_DSP
	if (FS_CM33_CPU_Register() == FS_FAIL_CPU_REGISTER) {
		return IEC60730B_TEST_CPU_ERROR;
	}
#else
	if (FS_CM33_CPU_Register_NDSP() == FS_FAIL_CPU_REGISTER) {
		return IEC60730B_TEST_CPU_ERROR;
	}
#endif

	/* non-stacked CPU registers */
	if (FS_CM33_CPU_NonStackedRegister() == FS_FAIL_CPU_NONSTACKED_REGISTER) {
		return IEC60730B_TEST_CPU_ERROR;
	}

	/* PRIMASK Secure */
	if (FS_CM33_CPU_Primask_S() == FS_FAIL_CPU_PRIMASK) {
		return IEC60730B_TEST_CPU_ERROR;
	}

#ifdef CONFIG_ARM_SECURE_FIRMWARE
	/* PRIMASK Non-Secure */
	if (FS_CM33_CPU_Primask_NS() == FS_FAIL_CPU_PRIMASK) {
		return IEC60730B_TEST_CPU_ERROR;
	}
#endif

	/* SP main Secure */
	FS_CM33_CPU_SPmain_S();

#ifdef CONFIG_ARM_SECURE_FIRMWARE
	/* SP main Non-Secure */
	FS_CM33_CPU_SPmain_NS();
#endif

	/* SP main limit Secure */
	FS_CM33_CPU_SPmain_Limit_S();

#ifdef CONFIG_ARM_SECURE_FIRMWARE
	/* SP main limit Non-Secure */
	FS_CM33_CPU_SPmain_Limit_NS();
#endif

	/* SP process Secure */
	FS_CM33_CPU_SPprocess_S();

#ifdef CONFIG_ARM_SECURE_FIRMWARE
	/* SP process Non-Secure */
	FS_CM33_CPU_SPprocess_NS();
#endif

	/* SP process limit Secure */
	FS_CM33_CPU_SPprocess_Limit_S();

#ifdef CONFIG_ARM_SECURE_FIRMWARE
	/* SP process limit Non-Secure */
	FS_CM33_CPU_SPprocess_Limit_NS();
#endif

#ifdef CONFIG_ARM_SECURE_FIRMWARE
	if (FS_CM33_CPU_Control_S() == FS_FAIL_CPU_CONTROL) {
		return IEC60730B_TEST_CPU_ERROR;
	}
#else
#ifdef CONFIG_CPU_HAS_FPU
	if (FS_CM33_CPU_Control() == FS_FAIL_CPU_CONTROL) {
		return IEC60730B_TEST_CPU_ERROR;
	}
#else
	if (FS_CM33_CPU_Control_NFPU() == FS_FAIL_CPU_CONTROL) {
		return IEC60730B_TEST_CPU_ERROR;
	}
#endif
#endif

#ifdef CONFIG_ARM_SECURE_FIRMWARE
	/* CONTROL Non-Secure */
	if (FS_CM33_CPU_Control_NS() == FS_FAIL_CPU_CONTROL) {
		return IEC60730B_TEST_CPU_ERROR;
	}
#endif

	/* Special Secure */
	if (FS_CM33_CPU_Special8PriorityLevels_S() == FS_FAIL_CPU_SPECIAL) {
		return IEC60730B_TEST_CPU_ERROR;
	}

#ifdef CONFIG_ARM_SECURE_FIRMWARE
	/* Special Non-Secure */
	if (FS_CM33_CPU_Special8PriorityLevels_NS() == FS_FAIL_CPU_SPECIAL) {
		return IEC60730B_TEST_CPU_ERROR;
	}
#endif

#else
	return IEC60730B_TEST_NOT_SUPPORTED;
#endif

	return IEC60730B_TEST_OK;
}
#endif /* CONFIG_IEC60730B_TEST_CPU */
