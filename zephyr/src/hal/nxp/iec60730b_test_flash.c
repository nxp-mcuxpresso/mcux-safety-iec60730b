/*
 * Copyright 2025-2026 NXP
 * SPDX-License-Identifier: Apache-2.0
 */

#ifdef CONFIG_IEC60730B_TEST_FLASH

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

#if defined(CONFIG_CRC_DRIVER_NXP) || defined(CONFIG_CRC_DRIVER_NXP_LPC)
#include <zephyr/device.h>
/* Mirror of struct crc_nxp_config / struct crc_nxp_lpc_config from
 * zephyr/drivers/crc/crc_nxp.c and crc_nxp_lpc.c. Only the first member
 * (the peripheral base address) is needed and its layout matches both
 * drivers. Typed as void * to avoid pulling in <fsl_crc.h> here; the
 * pointer is cast to FS_CRC_Type * / FS_CRC_L_Type * at the call site.
 */
struct iec60730b_crc_nxp_config {
	void *base;
};
#endif

#ifdef CONFIG_IEC60730B_TEST_FLASH_CRC16
/*
 * Test flash memory integrity using CRC16 for IEC 60730 Class B compliance.
 *
 * Algorithm: CRC-16/CCITT, polynomial 0x1021, no reflection, no XOR-out.
 */
int iec60730b_test_flash_crc16(const void *start, size_t size, uint16_t crc_expected)
{
	uint16_t crc_result;

#if defined(CONFIG_CRC_DRIVER_NXP)
	/*
	 * Hardware-accelerated CRC-16 using the NXP Kinetis/MCX-style CRC
	 * peripheral (DT compatible "nxp,crc").
	 */
	{
		static const struct device *const crc_dev = DEVICE_DT_GET(DT_CHOSEN(zephyr_crc));
		const struct iec60730b_crc_nxp_config *cfg;
		uint16_t crc = (uint16_t)CONFIG_IEC60730B_TEST_FLASH_CRC16_SEED;

		if (!device_is_ready(crc_dev)) {
			return IEC60730B_TEST_ERROR;
		}
		cfg = crc_dev->config;

		/* Program GPOLY for CRC-16/CCITT (0x1021). */
		((FS_CRC_Type *)(uintptr_t)cfg->base)->GPOLY = 0x1021U;

		if (FS_FLASH_C_HW16_K((uint32_t)start, (uint32_t)size,
				      (FS_CRC_Type *)(uintptr_t)cfg->base, &crc) != FS_PASS) {
			return IEC60730B_TEST_FLASH_ERROR;
		}
		crc_result = crc;
	}
#elif defined(CONFIG_CRC_DRIVER_NXP_LPC)
	/*
	 * Hardware-accelerated CRC-16 using the NXP LPC-style CRC peripheral
	 * (DT compatible "nxp,lpc-crc").
	 */
	{
		static const struct device *const crc_dev = DEVICE_DT_GET(DT_CHOSEN(zephyr_crc));
		const struct iec60730b_crc_nxp_config *cfg;
		uint16_t crc = (uint16_t)CONFIG_IEC60730B_TEST_FLASH_CRC16_SEED;

		if (!device_is_ready(crc_dev)) {
			return IEC60730B_TEST_ERROR;
		}
		cfg = crc_dev->config;

		if (FS_FLASH_C_HW16_L((uint32_t)start, (uint32_t)size,
				      (FS_CRC_L_Type *)(uintptr_t)cfg->base, &crc) != FS_PASS) {
			return IEC60730B_TEST_FLASH_ERROR;
		}
		crc_result = crc;
	}
#elif defined(CONFIG_CPU_CORTEX_M0PLUS)
	crc_result = FS_CM0_FLASH_SW16((uint32_t)start, size, 0,
				       (uint16_t)CONFIG_IEC60730B_TEST_FLASH_CRC16_SEED);
#elif defined(CONFIG_CPU_CORTEX_M4) || defined(CONFIG_CPU_CORTEX_M7)
	crc_result = FS_CM4_CM7_FLASH_SW16((uint32_t)start, size, 0,
					   (uint16_t)CONFIG_IEC60730B_TEST_FLASH_CRC16_SEED);
#elif defined(CONFIG_CPU_CORTEX_M33)
	crc_result = FS_CM33_FLASH_SW16((uint32_t)start, size, 0,
					(uint16_t)CONFIG_IEC60730B_TEST_FLASH_CRC16_SEED);
#else
	return IEC60730B_TEST_NOT_SUPPORTED;
#endif

	if (crc_result != crc_expected) {
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

#if defined(CONFIG_CRC_DRIVER_NXP)
	/*
	 * Hardware-accelerated CRC-32 using the NXP Kinetis/MCX-style CRC
	 * peripheral (DT compatible "nxp,crc").
	 */
	{
		static const struct device *const crc_dev = DEVICE_DT_GET(DT_CHOSEN(zephyr_crc));
		const struct iec60730b_crc_nxp_config *cfg;
		uint32_t crc = (uint32_t)CONFIG_IEC60730B_TEST_FLASH_CRC32_SEED;

		if (!device_is_ready(crc_dev)) {
			return IEC60730B_TEST_ERROR;
		}
		cfg = crc_dev->config;

		if (FS_FLASH_C_HW32_K((uint32_t)start, (uint32_t)size,
				      (FS_CRC_Type *)(uintptr_t)cfg->base, &crc) != FS_PASS) {
			return IEC60730B_TEST_FLASH_ERROR;
		}
		crc_result = crc;
	}
#elif defined(CONFIG_CPU_CORTEX_M4) || defined(CONFIG_CPU_CORTEX_M7)
	crc_result = FS_CM4_CM7_FLASH_SW32((uint32_t)start, size, 0,
					   (uint32_t)CONFIG_IEC60730B_TEST_FLASH_CRC32_SEED);
#elif defined(CONFIG_CPU_CORTEX_M33)
	crc_result = FS_CM33_FLASH_SW32((uint32_t)start, size, 0,
					(uint32_t)CONFIG_IEC60730B_TEST_FLASH_CRC32_SEED);
#else
	return IEC60730B_TEST_NOT_SUPPORTED;
#endif

	if (crc_result != crc_expected) {
		return IEC60730B_TEST_FLASH_ERROR;
	}

	return IEC60730B_TEST_OK;
}
#endif /* CONFIG_IEC60730B_TEST_FLASH_CRC32 */

#endif /* CONFIG_IEC60730B_TEST_FLASH */
