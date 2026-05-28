/*
 * Copyright 2025-2026 NXP
 * SPDX-License-Identifier: Apache-2.0
 */

#include "iec60730b_test.h"

#if defined(CONFIG_IEC60730B_TEST_CPU) && defined(CONFIG_CPU_CORTEX_M)

/*
 * Pattern testing: Using alternating bit patterns (0x55555555 = 01010101...
 * and 0xAAAAAAAA = 10101010...) helps detect stuck bits that always read as
 * 0 or 1
 */
static const uint32_t pattern1 = 0x55555555U;
static const uint32_t pattern2 = 0xAAAAAAAAU;

/* Mask for the APSR register */
#if !defined(CONFIG_ARMV7_M_ARMV8_M_MAINLINE)
#define APSR_BITMASK 0xF0000000U
#elif defined(CONFIG_ARMV8_M_DSP)
#define APSR_BITMASK 0xF80F0000U
#else
#define APSR_BITMASK 0xF8000000U
#endif

/* Mask for the BASEPRI register */
#define BASEPRI_BITMASK ((0xFFU << (8U - NUM_IRQ_PRIO_BITS)) & 0xFFU)

/* Mask for the PRIMASK register */
#define PRIMASK_BITMASK 0x00000001U

/* Mask for the MSP (Main Stack Pointer) register. */
#define MSP_BITMASK 0xFFFFFFFCU

/* Mask for the PSP (Process Stack Pointer) register */
#define PSP_BITMASK 0xFFFFFFFCU

/* Mask for the MSPLIM (Main Stack Pointer Limit) register */
#define MSPLIM_BITMASK 0xFFFFFFF8U

/* Mask for the PSPLIM (Process Stack Pointer Limit) register */
#define PSPLIM_BITMASK 0xFFFFFFF8U

/* Mask for the CONTROL register */
#if defined(CONFIG_ARM_TRUSTZONE_M) && defined(CONFIG_CPU_HAS_FPU)
#define CONTROL_BITMASK 0x0000000EU /* ARMv8-M with TrustZone and FPU */
#elif defined(CONFIG_CPU_HAS_FPU)
#define CONTROL_BITMASK 0x00000006U /* With FPU */
#else
#define CONTROL_BITMASK 0x00000002U /* Without FPU */
#endif

/*******************************************************************************
 * Core Register Access Functions - R0-R12
 ******************************************************************************/
__STATIC_FORCEINLINE uint32_t __get_R0(void)
{
	uint32_t result;

	__ASM volatile("MOV %0, r0" : "=r"(result));
	return result;
}

__STATIC_FORCEINLINE void __set_R0(uint32_t r0)
{
	__ASM volatile("MOV r0, %0" : : "r"(r0) : "r0");
}

__STATIC_FORCEINLINE uint32_t __get_R1(void)
{
	uint32_t result;

	__ASM volatile("MOV %0, r1" : "=r"(result));
	return result;
}

__STATIC_FORCEINLINE void __set_R1(uint32_t r1)
{
	__ASM volatile("MOV r1, %0" : : "r"(r1) : "r1");
}

__STATIC_FORCEINLINE uint32_t __get_R2(void)
{
	uint32_t result;

	__ASM volatile("MOV %0, r2" : "=r"(result));
	return result;
}

__STATIC_FORCEINLINE void __set_R2(uint32_t r2)
{
	__ASM volatile("MOV r2, %0" : : "r"(r2) : "r2");
}

__STATIC_FORCEINLINE uint32_t __get_R3(void)
{
	uint32_t result;

	__ASM volatile("MOV %0, r3" : "=r"(result));
	return result;
}

__STATIC_FORCEINLINE void __set_R3(uint32_t r3)
{
	__ASM volatile("MOV r3, %0" : : "r"(r3) : "r3");
}

__STATIC_FORCEINLINE uint32_t __get_R4(void)
{
	uint32_t result;

	__ASM volatile("MOV %0, r4" : "=r"(result));
	return result;
}

__STATIC_FORCEINLINE void __set_R4(uint32_t r4)
{
	__ASM volatile("MOV r4, %0" : : "r"(r4) : "r4");
}

__STATIC_FORCEINLINE uint32_t __get_R5(void)
{
	uint32_t result;

	__ASM volatile("MOV %0, r5" : "=r"(result));
	return result;
}

__STATIC_FORCEINLINE void __set_R5(uint32_t r5)
{
	__ASM volatile("MOV r5, %0" : : "r"(r5) : "r5");
}

__STATIC_FORCEINLINE uint32_t __get_R6(void)
{
	uint32_t result;

	__ASM volatile("MOV %0, r6" : "=r"(result));
	return result;
}

__STATIC_FORCEINLINE void __set_R6(uint32_t r6)
{
	__ASM volatile("MOV r6, %0" : : "r"(r6) : "r6");
}

__STATIC_FORCEINLINE uint32_t __get_R7(void)
{
	uint32_t result;

	__ASM volatile("MOV %0, r7" : "=r"(result));
	return result;
}

__STATIC_FORCEINLINE void __set_R7(uint32_t r7)
{
	__ASM volatile("MOV r7, %0" : : "r"(r7) : "r7");
}

__STATIC_FORCEINLINE uint32_t __get_R8(void)
{
	uint32_t result;

	__ASM volatile("MOV %0, r8" : "=r"(result));
	return result;
}

__STATIC_FORCEINLINE void __set_R8(uint32_t r8)
{
	__ASM volatile("MOV r8, %0" : : "r"(r8) : "r8");
}

__STATIC_FORCEINLINE uint32_t __get_R9(void)
{
	uint32_t result;

	__ASM volatile("MOV %0, r9" : "=r"(result));
	return result;
}

__STATIC_FORCEINLINE void __set_R9(uint32_t r9)
{
	__ASM volatile("MOV r9, %0" : : "r"(r9) : "r9");
}

__STATIC_FORCEINLINE uint32_t __get_R10(void)
{
	uint32_t result;

	__ASM volatile("MOV %0, r10" : "=r"(result));
	return result;
}

__STATIC_FORCEINLINE void __set_R10(uint32_t r10)
{
	__ASM volatile("MOV r10, %0" : : "r"(r10) : "r10");
}

__STATIC_FORCEINLINE uint32_t __get_R11(void)
{
	uint32_t result;

	__ASM volatile("MOV %0, r11" : "=r"(result));
	return result;
}

__STATIC_FORCEINLINE void __set_R11(uint32_t r11)
{
	__ASM volatile("MOV r11, %0" : : "r"(r11) : "r11");
}

__STATIC_FORCEINLINE uint32_t __get_R12(void)
{
	uint32_t result;

	__ASM volatile("MOV %0, r12" : "=r"(result));
	return result;
}

__STATIC_FORCEINLINE void __set_R12(uint32_t r12)
{
	__ASM volatile("MOV r12, %0" : : "r"(r12) : "r12");
}

__STATIC_FORCEINLINE uint32_t __get_LR(void)
{
	uint32_t result;

	__ASM volatile("MOV %0, lr" : "=r"(result));
	return result;
}

__STATIC_FORCEINLINE void __set_LR(uint32_t lr)
{
	__ASM volatile("MOV lr, %0" : : "r"(lr) : "lr");
}

/* Set the APSR (Application Program Status Register). */
__STATIC_FORCEINLINE void __set_APSR(uint32_t apsr)
{
#if defined(CONFIG_ARMV8_M_DSP) /* With DSP */
	__ASM volatile("MSR APSR_nzcvqg, %0" : : "r"(apsr) : "cc", "memory");
#else /* Without DSP */
	__ASM volatile("MSR APSR_nzcvq, %0" : : "r"(apsr) : "cc", "memory");
#endif
}

/*
 * Test CPU ARM Cortex registers R0-R12, LR, and APSR.
 *
 * This function tests the core CPU registers by writing test patterns
 * and verifying the values.
 */
static inline int iec60730b_test_cpu_cortex(void)
{
	uint32_t backup_lr;
	uint32_t backup_apsr;

	/* Critical registers (R0, R1, LR, APSR): If these fail, the code
	 * enters an infinite loop with interrupts disabled because
	 * the system cannot safely continue
	 */

	/* Test R0 and R1 first (critical for endless loop detection) */
	__set_R0(pattern1);
	__set_R1(pattern1);
	if (__get_R0() != __get_R1()) {
		goto endless_loop;
	}

	__set_R0(pattern2);
	__set_R1(pattern2);
	if (__get_R0() != __get_R1()) {
		goto endless_loop;
	}

	/* APSR */
	backup_apsr = __get_APSR();
	__set_APSR(pattern1 & APSR_BITMASK);
	if (__get_APSR() != (pattern1 & APSR_BITMASK)) {
		goto endless_loop;
	}

	__set_APSR(pattern2 & APSR_BITMASK);
	if (__get_APSR() != (pattern2 & APSR_BITMASK)) {
		goto endless_loop;
	}
	__set_APSR(backup_apsr);

	/* Test LR (R14) */
	backup_lr = __get_LR();

	__set_LR(pattern1);
	if (__get_LR() != pattern1) {
		goto endless_loop;
	}

	__set_LR(pattern2);
	if (__get_LR() != pattern2) {
		goto endless_loop;
	}

	__set_LR(backup_lr);

	/* Non-critical registers (R2-R12): If these fail,
	 * the function returns an error code that the calling code can handle.
	 */

	/* Test R2 */
	__set_R2(pattern1);
	if (__get_R2() != pattern1) {
		return IEC60730B_TEST_CPU_ERROR;
	}
	__set_R2(pattern2);
	if (__get_R2() != pattern2) {
		return IEC60730B_TEST_CPU_ERROR;
	}

	/* Test R3 */
	__set_R3(pattern1);
	if (__get_R3() != pattern1) {
		return IEC60730B_TEST_CPU_ERROR;
	}
	__set_R3(pattern2);
	if (__get_R3() != pattern2) {
		return IEC60730B_TEST_CPU_ERROR;
	}

	/* Test R4 */
	__set_R4(pattern1);
	if (__get_R4() != pattern1) {
		return IEC60730B_TEST_CPU_ERROR;
	}
	__set_R4(pattern2);
	if (__get_R4() != pattern2) {
		return IEC60730B_TEST_CPU_ERROR;
	}

	/* Test R5 */
	__set_R5(pattern1);
	if (__get_R5() != pattern1) {
		return IEC60730B_TEST_CPU_ERROR;
	}
	__set_R5(pattern2);
	if (__get_R5() != pattern2) {
		return IEC60730B_TEST_CPU_ERROR;
	}

	/* Test R6 */
	__set_R6(pattern1);
	if (__get_R6() != pattern1) {
		return IEC60730B_TEST_CPU_ERROR;
	}
	__set_R6(pattern2);
	if (__get_R6() != pattern2) {
		return IEC60730B_TEST_CPU_ERROR;
	}

	/* Test R7 */
	__set_R7(pattern1);
	if (__get_R7() != pattern1) {
		return IEC60730B_TEST_CPU_ERROR;
	}
	__set_R7(pattern2);
	if (__get_R7() != pattern2) {
		return IEC60730B_TEST_CPU_ERROR;
	}

	/* Test R8 */
	__set_R8(pattern1);
	if (__get_R8() != pattern1) {
		return IEC60730B_TEST_CPU_ERROR;
	}
	__set_R8(pattern2);
	if (__get_R8() != pattern2) {
		return IEC60730B_TEST_CPU_ERROR;
	}

	/* Test R9 */
	__set_R9(pattern1);
	if (__get_R9() != pattern1) {
		return IEC60730B_TEST_CPU_ERROR;
	}
	__set_R9(pattern2);
	if (__get_R9() != pattern2) {
		return IEC60730B_TEST_CPU_ERROR;
	}

	/* Test R10 */
	__set_R10(pattern1);
	if (__get_R10() != pattern1) {
		return IEC60730B_TEST_CPU_ERROR;
	}
	__set_R10(pattern2);
	if (__get_R10() != pattern2) {
		return IEC60730B_TEST_CPU_ERROR;
	}

	/* Test R11 */
	__set_R11(pattern1);
	if (__get_R11() != pattern1) {
		return IEC60730B_TEST_CPU_ERROR;
	}
	__set_R11(pattern2);
	if (__get_R11() != pattern2) {
		return IEC60730B_TEST_CPU_ERROR;
	}

	/* Test R12 */
	__set_R12(pattern1);
	if (__get_R12() != pattern1) {
		return IEC60730B_TEST_CPU_ERROR;
	}
	__set_R12(pattern2);
	if (__get_R12() != pattern2) {
		return IEC60730B_TEST_CPU_ERROR;
	}

	return IEC60730B_TEST_OK;

endless_loop:
	/* Endless loop for case R0, R1, LR, or APSR is corrupted */
	__disable_irq();
	while (1) {
		__NOP();
	}
}

/* Test PRIMASK register */
static inline int iec60730b_test_cpu_cortex_primask(void)
{
	uint32_t primask_backup;
	uint32_t test_value;

	primask_backup = __get_PRIMASK();

	__set_PRIMASK(pattern1 & PRIMASK_BITMASK);
	test_value = __get_PRIMASK();
	if (test_value != (pattern1 & PRIMASK_BITMASK)) {
		return IEC60730B_TEST_CPU_ERROR;
	}

	__set_PRIMASK(pattern2 & PRIMASK_BITMASK);
	test_value = __get_PRIMASK();
	__set_PRIMASK(primask_backup);

	if (test_value != (pattern2 & PRIMASK_BITMASK)) {
		return IEC60730B_TEST_CPU_ERROR;
	}

	return IEC60730B_TEST_OK;
}

#if CONFIG_ARM_TRUSTZONE_M
/* Test PRIMASK register (Non-Secure) */
static inline int iec60730b_test_cpu_cortex_primask_ns(void)
{
	uint32_t primask_backup;
	uint32_t test_value;

	primask_backup = __TZ_get_PRIMASK_NS();

	__TZ_set_PRIMASK_NS(pattern1 & PRIMASK_BITMASK);
	test_value = __TZ_get_PRIMASK_NS();
	if (test_value != (pattern1 & PRIMASK_BITMASK)) {
		return IEC60730B_TEST_CPU_ERROR;
	}

	__TZ_set_PRIMASK_NS(pattern2 & PRIMASK_BITMASK);
	test_value = __TZ_get_PRIMASK_NS();
	__TZ_set_PRIMASK_NS(primask_backup);

	if (test_value != (pattern2 & PRIMASK_BITMASK)) {
		return IEC60730B_TEST_CPU_ERROR;
	}

	return IEC60730B_TEST_OK;
}
#endif /* CONFIG_ARM_TRUSTZONE_M */

/* Test Main Stack Pointer */
static inline int iec60730b_test_cpu_cortex_msp(void)
{
	uint32_t msp_backup;
	uint32_t test_value;

	msp_backup = __get_MSP();

	__set_MSP(pattern1 & MSP_BITMASK);
	test_value = __get_MSP();
	if (test_value != (pattern1 & MSP_BITMASK)) {
		goto error_sp_main_s;
	}

	__set_MSP(pattern2 & MSP_BITMASK);
	test_value = __get_MSP();
	if (test_value != (pattern2 & MSP_BITMASK)) {
		goto error_sp_main_s;
	}

	__set_MSP(msp_backup);
	return IEC60730B_TEST_OK;

error_sp_main_s:
	__disable_irq();
	while (1) {
		;
	}
}

#if CONFIG_ARM_TRUSTZONE_M
/* Test Main Stack Pointer (Non-Secure) */
static inline int iec60730b_test_cpu_cortex_msp_ns(void)
{
	uint32_t msp_backup;
	uint32_t test_value;

	msp_backup = __TZ_get_MSP_NS();

	__TZ_set_MSP_NS(pattern1 & MSP_BITMASK);
	test_value = __TZ_get_MSP_NS();
	if (test_value != (pattern1 & MSP_BITMASK)) {
		goto error_sp_main_ns;
	}

	__TZ_set_MSP_NS(pattern2 & MSP_BITMASK);
	test_value = __TZ_get_MSP_NS();
	if (test_value != (pattern2 & MSP_BITMASK)) {
		goto error_sp_main_ns;
	}

	__TZ_set_MSP_NS(msp_backup);
	return IEC60730B_TEST_OK;

error_sp_main_ns:
	__disable_irq();
	while (1) {
		;
	}
}
#endif /* CONFIG_ARM_TRUSTZONE_M */

#if defined(CONFIG_ARMV8_M_MAINLINE)
/* Test Main Stack Pointer Limit */
static inline int iec60730b_test_cpu_cortex_msplim(void)
{
	uint32_t msplim_backup;
	uint32_t test_value;

	msplim_backup = __get_MSPLIM();

	__set_MSPLIM(pattern1 & MSPLIM_BITMASK);
	test_value = __get_MSPLIM();
	if (test_value != (pattern1 & MSPLIM_BITMASK)) {
		goto error_sp_main_limit_s;
	}

	__set_MSPLIM(pattern2 & MSPLIM_BITMASK);
	test_value = __get_MSPLIM();
	if (test_value != (pattern2 & MSPLIM_BITMASK)) {
		goto error_sp_main_limit_s;
	}

	__set_MSPLIM(msplim_backup);
	return IEC60730B_TEST_OK;

error_sp_main_limit_s:
	__disable_irq();
	while (1) {
		;
	}
}
#endif /* CONFIG_ARMV8_M_MAINLINE */

#if CONFIG_ARM_TRUSTZONE_M
/* Test Main Stack Pointer Limit (Non-Secure) */
static inline int iec60730b_test_cpu_cortex_msplim_ns(void)
{
	uint32_t msplim_backup;
	uint32_t test_value;

	msplim_backup = __TZ_get_MSPLIM_NS();

	__TZ_set_MSPLIM_NS(pattern1 & MSPLIM_BITMASK);
	test_value = __TZ_get_MSPLIM_NS();
	if (test_value != (pattern1 & MSPLIM_BITMASK)) {
		goto error_sp_main_limit_ns;
	}

	__TZ_set_MSPLIM_NS(pattern2 & MSPLIM_BITMASK);
	test_value = __TZ_get_MSPLIM_NS();
	if (test_value != (pattern2 & MSPLIM_BITMASK)) {
		goto error_sp_main_limit_ns;
	}

	__TZ_set_MSPLIM_NS(msplim_backup);
	return IEC60730B_TEST_OK;

error_sp_main_limit_ns:
	__disable_irq();
	while (1) {
		;
	}
}
#endif /* CONFIG_ARM_TRUSTZONE_M */

/* Test Process Stack Pointer */
static inline int iec60730b_test_cpu_cortex_psp(void)
{
	uint32_t psp_backup;
	uint32_t test_value;

	psp_backup = __get_PSP();

	__set_PSP(pattern1 & PSP_BITMASK);
	test_value = __get_PSP();
	if (test_value != (pattern1 & PSP_BITMASK)) {
		goto error_sp_process_s;
	}

	__set_PSP(pattern2 & PSP_BITMASK);
	test_value = __get_PSP();
	if (test_value != (pattern2 & PSP_BITMASK)) {
		goto error_sp_process_s;
	}

	__set_PSP(psp_backup);
	return IEC60730B_TEST_OK;

error_sp_process_s:
	__disable_irq();
	while (1) {
		;
	}
}

#if CONFIG_ARM_TRUSTZONE_M
/* Test Process Stack Pointer (Non-Secure) */
static inline int iec60730b_test_cpu_cortex_psp_ns(void)
{
	uint32_t psp_backup;
	uint32_t test_value;

	psp_backup = __TZ_get_PSP_NS();

	__TZ_set_PSP_NS(pattern1 & PSP_BITMASK);
	test_value = __TZ_get_PSP_NS();
	if (test_value != (pattern1 & PSP_BITMASK)) {
		goto error_sp_process_ns;
	}

	__TZ_set_PSP_NS(pattern2 & PSP_BITMASK);
	test_value = __TZ_get_PSP_NS();
	if (test_value != (pattern2 & PSP_BITMASK)) {
		goto error_sp_process_ns;
	}

	__TZ_set_PSP_NS(psp_backup);
	return IEC60730B_TEST_OK;

error_sp_process_ns:
	__disable_irq();
	while (1) {
		;
	}
}
#endif /* CONFIG_ARM_TRUSTZONE_M */

#if defined(CONFIG_ARMV8_M_MAINLINE)
/* Test Process Stack Pointer Limit */
static inline int iec60730b_test_cpu_cortex_psplim(void)
{
	uint32_t psplim_backup;
	uint32_t test_value;

	psplim_backup = __get_PSPLIM();

	__set_PSPLIM(pattern1 & PSPLIM_BITMASK);
	test_value = __get_PSPLIM();
	if (test_value != (pattern1 & PSPLIM_BITMASK)) {
		goto error_sp_process_limit_s;
	}

	__set_PSPLIM(pattern2 & PSPLIM_BITMASK);
	test_value = __get_PSPLIM();
	if (test_value != (pattern2 & PSPLIM_BITMASK)) {
		goto error_sp_process_limit_s;
	}

	__set_PSPLIM(psplim_backup);
	return IEC60730B_TEST_OK;

error_sp_process_limit_s:
	__disable_irq();
	while (1) {
		;
	}
}
#endif /* CONFIG_ARMV8_M_MAINLINE */

#ifdef CONFIG_ARM_TRUSTZONE_M
/* Test Process Stack Pointer Limit (Non-Secure) */
static inline int iec60730b_test_cpu_cortex_psplim_ns(void)
{
	uint32_t psplim_backup;
	uint32_t test_value;

	psplim_backup = __TZ_get_PSPLIM_NS();

	__TZ_set_PSPLIM_NS(pattern1 & PSPLIM_BITMASK);
	test_value = __TZ_get_PSPLIM_NS();
	if (test_value != (pattern1 & PSPLIM_BITMASK)) {
		goto error_sp_process_limit_ns;
	}

	__TZ_set_PSPLIM_NS(pattern2 & PSPLIM_BITMASK);
	test_value = __TZ_get_PSPLIM_NS();
	if (test_value != (pattern2 & PSPLIM_BITMASK)) {
		goto error_sp_process_limit_ns;
	}

	__TZ_set_PSPLIM_NS(psplim_backup);
	return IEC60730B_TEST_OK;

error_sp_process_limit_ns:
	__disable_irq();
	while (1) {
		;
	}
}
#endif /* CONFIG_ARM_TRUSTZONE_M */

/* Test CONTROL register */
static inline int iec60730b_test_cpu_cortex_control(void)
{
	uint32_t msp_backup;
	uint32_t control_backup;
	uint32_t test_value;

	msp_backup = __get_MSP();
	control_backup = __get_CONTROL();

	__set_CONTROL(pattern1 & CONTROL_BITMASK);
	test_value = __get_CONTROL();
	if (test_value != (pattern1 & CONTROL_BITMASK)) {
		__set_MSP(msp_backup);
		__set_CONTROL(control_backup);
		__ISB();
		return IEC60730B_TEST_CPU_ERROR;
	}

	__set_CONTROL(pattern2 & CONTROL_BITMASK);
	test_value = __get_CONTROL();
	if (test_value != (pattern2 & CONTROL_BITMASK)) {
		__set_MSP(msp_backup);
		__set_CONTROL(control_backup);
		__ISB();
		return IEC60730B_TEST_CPU_ERROR;
	}

	__set_CONTROL(control_backup);
	__ISB();
	return IEC60730B_TEST_OK;
}

#if defined(CONFIG_ARMV7_M_ARMV8_M_MAINLINE)

/* Test BASEPRI and FAULTMASK special registers */
static inline int iec60730b_test_cpu_cortex_special(void)
{
	uint32_t basepri_backup;
	uint32_t faultmask_backup;
	uint32_t test_value;

	/* Test BASEPRI */
	basepri_backup = __get_BASEPRI();
	__set_BASEPRI(0x00);

	__set_BASEPRI(pattern1 & BASEPRI_BITMASK);
	test_value = __get_BASEPRI();
	if (test_value != (pattern1 & BASEPRI_BITMASK)) {
		return IEC60730B_TEST_CPU_ERROR;
	}

	__set_BASEPRI(pattern2 & BASEPRI_BITMASK);
	test_value = __get_BASEPRI();
	__set_BASEPRI(basepri_backup);

	if (test_value != (pattern2 & BASEPRI_BITMASK)) {
		return IEC60730B_TEST_CPU_ERROR;
	}

	/* Test FAULTMASK */
	faultmask_backup = __get_FAULTMASK();

	__set_FAULTMASK(0x1);
	test_value = __get_FAULTMASK();
	if (test_value != 0x1) {
		return IEC60730B_TEST_CPU_ERROR;
	}

	__set_FAULTMASK(0x0);
	test_value = __get_FAULTMASK();
	__set_FAULTMASK(faultmask_backup);

	if (test_value != 0x0) {
		return IEC60730B_TEST_CPU_ERROR;
	}

	return IEC60730B_TEST_OK;
}
#endif /* CONFIG_ARMV7_M_ARMV8_M_MAINLINE */

#if CONFIG_ARM_TRUSTZONE_M
/* Test BASEPRI and FAULTMASK special registers (Non-Secure) */
static inline int iec60730b_test_cpu_cortex_special_ns(void)
{
	uint32_t basepri_backup;
	uint32_t faultmask_backup;
	uint32_t test_value;

	/* Test BASEPRI */
	basepri_backup = __TZ_get_BASEPRI_NS();
	__TZ_set_BASEPRI_NS(0x00);

	__TZ_set_BASEPRI_NS(pattern1 & BASEPRI_BITMASK);
	test_value = __TZ_get_BASEPRI_NS();
	if (test_value != (pattern1 & BASEPRI_BITMASK)) {
		return IEC60730B_TEST_CPU_ERROR;
	}

	__TZ_set_BASEPRI_NS(pattern2 & BASEPRI_BITMASK);
	test_value = __TZ_get_BASEPRI_NS();
	__TZ_set_BASEPRI_NS(basepri_backup);

	if (test_value != (pattern2 & BASEPRI_BITMASK)) {
		return IEC60730B_TEST_CPU_ERROR;
	}

	/* Test FAULTMASK */
	faultmask_backup = __TZ_get_FAULTMASK_NS();

	__TZ_set_FAULTMASK_NS(0x1);
	test_value = __TZ_get_FAULTMASK_NS();
	if (test_value != 0x1) {
		return IEC60730B_TEST_CPU_ERROR;
	}

	__TZ_set_FAULTMASK_NS(0x0);
	test_value = __TZ_get_FAULTMASK_NS();
	__TZ_set_FAULTMASK_NS(faultmask_backup);

	if (test_value != 0x0) {
		return IEC60730B_TEST_CPU_ERROR;
	}

	return IEC60730B_TEST_OK;
}
#endif /* CONFIG_ARM_TRUSTZONE_M */

/*******************************************************
 * Test CPU registers for IEC 60730 Class B compliance
 ******************************************************/
int iec60730b_test_cpu(void)
{
	if (iec60730b_test_cpu_cortex() != IEC60730B_TEST_OK) {
		return IEC60730B_TEST_CPU_ERROR;
	}

	/* CONTROL */
	if (iec60730b_test_cpu_cortex_control() != IEC60730B_TEST_OK) {
		return IEC60730B_TEST_CPU_ERROR;
	}

	/* SP main */
	if (iec60730b_test_cpu_cortex_msp() != IEC60730B_TEST_OK) {
		return IEC60730B_TEST_CPU_ERROR;
	}

	/* SP process */
	if (iec60730b_test_cpu_cortex_psp() != IEC60730B_TEST_OK) {
		return IEC60730B_TEST_CPU_ERROR;
	}

	/* PRIMASK */
	if (iec60730b_test_cpu_cortex_primask() != IEC60730B_TEST_OK) {
		return IEC60730B_TEST_CPU_ERROR;
	}

#if defined(CONFIG_ARMV7_M_ARMV8_M_MAINLINE)

	/* ASEPRI and FAULTMASK */
	if (iec60730b_test_cpu_cortex_special() != IEC60730B_TEST_OK) {
		return IEC60730B_TEST_CPU_ERROR;
	}

#if defined(CONFIG_ARMV8_M_MAINLINE)

	/* SP main limit */
	if (iec60730b_test_cpu_cortex_msplim() != IEC60730B_TEST_OK) {
		return IEC60730B_TEST_CPU_ERROR;
	}

	/* SP process limit */
	if (iec60730b_test_cpu_cortex_psplim() != IEC60730B_TEST_OK) {
		return IEC60730B_TEST_CPU_ERROR;
	}

#if defined(CONFIG_ARM_TRUSTZONE_M)

	/* PRIMASK Non-Secure */
	if (iec60730b_test_cpu_cortex_primask_ns() != IEC60730B_TEST_OK) {
		return IEC60730B_TEST_CPU_ERROR;
	}

	/* SP main Non-Secure */
	if (iec60730b_test_cpu_cortex_msp_ns() != IEC60730B_TEST_OK) {
		return IEC60730B_TEST_CPU_ERROR;
	}

	/* SP main limit Non-Secure */
	if (iec60730b_test_cpu_cortex_msplim_ns() != IEC60730B_TEST_OK) {
		return IEC60730B_TEST_CPU_ERROR;
	}

	/* SP process Non-Secure */
	if (iec60730b_test_cpu_cortex_psp_ns() != IEC60730B_TEST_OK) {
		return IEC60730B_TEST_CPU_ERROR;
	}

	/* SP process limit Non-Secure */
	if (iec60730b_test_cpu_cortex_psplim_ns() != IEC60730B_TEST_OK) {
		return IEC60730B_TEST_CPU_ERROR;
	}

	/* Special Non-Secure */
	if (iec60730b_test_cpu_cortex_special_ns() != IEC60730B_TEST_OK) {
		return IEC60730B_TEST_CPU_ERROR;
	}
#endif /* CONFIG_ARM_TRUSTZONE_M */
#endif /* CONFIG_ARMV8_M_MAINLINE */
#endif /* CONFIG_ARMV7_M_ARMV8_M_MAINLINE */

	return IEC60730B_TEST_OK;
}
#endif /* CONFIG_IEC60730B_TEST_CPU */
