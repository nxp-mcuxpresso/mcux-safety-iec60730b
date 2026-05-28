/*
 * Copyright 2025 NXP
 * SPDX-License-Identifier: Apache-2.0
 *
 * This header is included by the iec60730b bare-metal library
 */

#ifndef _DEVICE_INFORMATION_H_
#define _DEVICE_INFORMATION_H_

/* Enabled the hardware Floating Point Unit (FPU) support. */
#ifdef CONFIG_IEC60730B_TEST_FPU
#define FPU_SUPPORT 1
#else
#define FPU_SUPPORT 0
#endif

/* Enabled the ARMv8-M DSP Extension. */
#ifdef CONFIG_ARMV8_M_DSP
#define DSP_SUPPORT 1
#else
#define DSP_SUPPORT 0
#endif

/* Enabled ARM TrustZone-M support. */
#ifdef CONFIG_ARM_TRUSTZONE_M
#define TZ_SUPPORT 1
#else
#define TZ_SUPPORT 0
#endif

#endif /* _DEVICE_INFORMATION_H_ */
