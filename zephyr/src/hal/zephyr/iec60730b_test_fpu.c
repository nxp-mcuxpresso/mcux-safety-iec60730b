/*
 * Copyright 2026 NXP
 * SPDX-License-Identifier: Apache-2.0
 */

#include "iec60730b_test.h"

#if defined(CONFIG_IEC60730B_TEST_FPU) && defined(CONFIG_ARMV7_M_ARMV8_M_FP)

/*
 * Pattern testing: Using alternating bit patterns (0x55555555 = 01010101...
 * and 0xAAAAAAAA = 10101010...) helps detect stuck bits that always read as
 * 0 or 1
 */
static const uint32_t pattern1 = 0x55555555U;
static const uint32_t pattern2 = 0xAAAAAAAAU;

/* CPACR register mask for enabling FPU */
#define CPACR_CP10_CP11_MASK 0x00F00000U

/* Mask for the FPSCR register */
#define FPSCR_BITMASK 0xF7C0009FU

/*******************************************************************************
 * FPU Register Access Functions - S0-S31
 ******************************************************************************/
__STATIC_FORCEINLINE uint32_t __get_S0(void)
{
	uint32_t result;

	__ASM volatile("VMOV %0, s0" : "=r"(result));
	return result;
}

__STATIC_FORCEINLINE void __set_S0(uint32_t s0)
{
	__ASM volatile("VMOV s0, %0" : : "r"(s0) : "s0");
}

__STATIC_FORCEINLINE uint32_t __get_S1(void)
{
	uint32_t result;

	__ASM volatile("VMOV %0, s1" : "=r"(result));
	return result;
}

__STATIC_FORCEINLINE void __set_S1(uint32_t s1)
{
	__ASM volatile("VMOV s1, %0" : : "r"(s1) : "s1");
}

__STATIC_FORCEINLINE uint32_t __get_S2(void)
{
	uint32_t result;

	__ASM volatile("VMOV %0, s2" : "=r"(result));
	return result;
}

__STATIC_FORCEINLINE void __set_S2(uint32_t s2)
{
	__ASM volatile("VMOV s2, %0" : : "r"(s2) : "s2");
}

__STATIC_FORCEINLINE uint32_t __get_S3(void)
{
	uint32_t result;

	__ASM volatile("VMOV %0, s3" : "=r"(result));
	return result;
}

__STATIC_FORCEINLINE void __set_S3(uint32_t s3)
{
	__ASM volatile("VMOV s3, %0" : : "r"(s3) : "s3");
}

__STATIC_FORCEINLINE uint32_t __get_S4(void)
{
	uint32_t result;

	__ASM volatile("VMOV %0, s4" : "=r"(result));
	return result;
}

__STATIC_FORCEINLINE void __set_S4(uint32_t s4)
{
	__ASM volatile("VMOV s4, %0" : : "r"(s4) : "s4");
}

__STATIC_FORCEINLINE uint32_t __get_S5(void)
{
	uint32_t result;

	__ASM volatile("VMOV %0, s5" : "=r"(result));
	return result;
}

__STATIC_FORCEINLINE void __set_S5(uint32_t s5)
{
	__ASM volatile("VMOV s5, %0" : : "r"(s5) : "s5");
}

__STATIC_FORCEINLINE uint32_t __get_S6(void)
{
	uint32_t result;

	__ASM volatile("VMOV %0, s6" : "=r"(result));
	return result;
}

__STATIC_FORCEINLINE void __set_S6(uint32_t s6)
{
	__ASM volatile("VMOV s6, %0" : : "r"(s6) : "s6");
}

__STATIC_FORCEINLINE uint32_t __get_S7(void)
{
	uint32_t result;

	__ASM volatile("VMOV %0, s7" : "=r"(result));
	return result;
}

__STATIC_FORCEINLINE void __set_S7(uint32_t s7)
{
	__ASM volatile("VMOV s7, %0" : : "r"(s7) : "s7");
}

__STATIC_FORCEINLINE uint32_t __get_S8(void)
{
	uint32_t result;

	__ASM volatile("VMOV %0, s8" : "=r"(result));
	return result;
}

__STATIC_FORCEINLINE void __set_S8(uint32_t s8)
{
	__ASM volatile("VMOV s8, %0" : : "r"(s8) : "s8");
}

__STATIC_FORCEINLINE uint32_t __get_S9(void)
{
	uint32_t result;

	__ASM volatile("VMOV %0, s9" : "=r"(result));
	return result;
}

__STATIC_FORCEINLINE void __set_S9(uint32_t s9)
{
	__ASM volatile("VMOV s9, %0" : : "r"(s9) : "s9");
}

__STATIC_FORCEINLINE uint32_t __get_S10(void)
{
	uint32_t result;

	__ASM volatile("VMOV %0, s10" : "=r"(result));
	return result;
}

__STATIC_FORCEINLINE void __set_S10(uint32_t s10)
{
	__ASM volatile("VMOV s10, %0" : : "r"(s10) : "s10");
}

__STATIC_FORCEINLINE uint32_t __get_S11(void)
{
	uint32_t result;

	__ASM volatile("VMOV %0, s11" : "=r"(result));
	return result;
}

__STATIC_FORCEINLINE void __set_S11(uint32_t s11)
{
	__ASM volatile("VMOV s11, %0" : : "r"(s11) : "s11");
}

__STATIC_FORCEINLINE uint32_t __get_S12(void)
{
	uint32_t result;

	__ASM volatile("VMOV %0, s12" : "=r"(result));
	return result;
}

__STATIC_FORCEINLINE void __set_S12(uint32_t s12)
{
	__ASM volatile("VMOV s12, %0" : : "r"(s12) : "s12");
}

__STATIC_FORCEINLINE uint32_t __get_S13(void)
{
	uint32_t result;

	__ASM volatile("VMOV %0, s13" : "=r"(result));
	return result;
}

__STATIC_FORCEINLINE void __set_S13(uint32_t s13)
{
	__ASM volatile("VMOV s13, %0" : : "r"(s13) : "s13");
}

__STATIC_FORCEINLINE uint32_t __get_S14(void)
{
	uint32_t result;

	__ASM volatile("VMOV %0, s14" : "=r"(result));
	return result;
}

__STATIC_FORCEINLINE void __set_S14(uint32_t s14)
{
	__ASM volatile("VMOV s14, %0" : : "r"(s14) : "s14");
}

__STATIC_FORCEINLINE uint32_t __get_S15(void)
{
	uint32_t result;

	__ASM volatile("VMOV %0, s15" : "=r"(result));
	return result;
}

__STATIC_FORCEINLINE void __set_S15(uint32_t s15)
{
	__ASM volatile("VMOV s15, %0" : : "r"(s15) : "s15");
}

__STATIC_FORCEINLINE uint32_t __get_S16(void)
{
	uint32_t result;

	__ASM volatile("VMOV %0, s16" : "=r"(result));
	return result;
}

__STATIC_FORCEINLINE void __set_S16(uint32_t s16)
{
	__ASM volatile("VMOV s16, %0" : : "r"(s16) : "s16");
}

__STATIC_FORCEINLINE uint32_t __get_S17(void)
{
	uint32_t result;

	__ASM volatile("VMOV %0, s17" : "=r"(result));
	return result;
}

__STATIC_FORCEINLINE void __set_S17(uint32_t s17)
{
	__ASM volatile("VMOV s17, %0" : : "r"(s17) : "s17");
}

__STATIC_FORCEINLINE uint32_t __get_S18(void)
{
	uint32_t result;

	__ASM volatile("VMOV %0, s18" : "=r"(result));
	return result;
}

__STATIC_FORCEINLINE void __set_S18(uint32_t s18)
{
	__ASM volatile("VMOV s18, %0" : : "r"(s18) : "s18");
}

__STATIC_FORCEINLINE uint32_t __get_S19(void)
{
	uint32_t result;

	__ASM volatile("VMOV %0, s19" : "=r"(result));
	return result;
}

__STATIC_FORCEINLINE void __set_S19(uint32_t s19)
{
	__ASM volatile("VMOV s19, %0" : : "r"(s19) : "s19");
}

__STATIC_FORCEINLINE uint32_t __get_S20(void)
{
	uint32_t result;

	__ASM volatile("VMOV %0, s20" : "=r"(result));
	return result;
}

__STATIC_FORCEINLINE void __set_S20(uint32_t s20)
{
	__ASM volatile("VMOV s20, %0" : : "r"(s20) : "s20");
}

__STATIC_FORCEINLINE uint32_t __get_S21(void)
{
	uint32_t result;

	__ASM volatile("VMOV %0, s21" : "=r"(result));
	return result;
}

__STATIC_FORCEINLINE void __set_S21(uint32_t s21)
{
	__ASM volatile("VMOV s21, %0" : : "r"(s21) : "s21");
}

__STATIC_FORCEINLINE uint32_t __get_S22(void)
{
	uint32_t result;

	__ASM volatile("VMOV %0, s22" : "=r"(result));
	return result;
}

__STATIC_FORCEINLINE void __set_S22(uint32_t s22)
{
	__ASM volatile("VMOV s22, %0" : : "r"(s22) : "s22");
}

__STATIC_FORCEINLINE uint32_t __get_S23(void)
{
	uint32_t result;

	__ASM volatile("VMOV %0, s23" : "=r"(result));
	return result;
}

__STATIC_FORCEINLINE void __set_S23(uint32_t s23)
{
	__ASM volatile("VMOV s23, %0" : : "r"(s23) : "s23");
}

__STATIC_FORCEINLINE uint32_t __get_S24(void)
{
	uint32_t result;

	__ASM volatile("VMOV %0, s24" : "=r"(result));
	return result;
}

__STATIC_FORCEINLINE void __set_S24(uint32_t s24)
{
	__ASM volatile("VMOV s24, %0" : : "r"(s24) : "s24");
}

__STATIC_FORCEINLINE uint32_t __get_S25(void)
{
	uint32_t result;

	__ASM volatile("VMOV %0, s25" : "=r"(result));
	return result;
}

__STATIC_FORCEINLINE void __set_S25(uint32_t s25)
{
	__ASM volatile("VMOV s25, %0" : : "r"(s25) : "s25");
}

__STATIC_FORCEINLINE uint32_t __get_S26(void)
{
	uint32_t result;

	__ASM volatile("VMOV %0, s26" : "=r"(result));
	return result;
}

__STATIC_FORCEINLINE void __set_S26(uint32_t s26)
{
	__ASM volatile("VMOV s26, %0" : : "r"(s26) : "s26");
}

__STATIC_FORCEINLINE uint32_t __get_S27(void)
{
	uint32_t result;

	__ASM volatile("VMOV %0, s27" : "=r"(result));
	return result;
}

__STATIC_FORCEINLINE void __set_S27(uint32_t s27)
{
	__ASM volatile("VMOV s27, %0" : : "r"(s27) : "s27");
}

__STATIC_FORCEINLINE uint32_t __get_S28(void)
{
	uint32_t result;

	__ASM volatile("VMOV %0, s28" : "=r"(result));
	return result;
}

__STATIC_FORCEINLINE void __set_S28(uint32_t s28)
{
	__ASM volatile("VMOV s28, %0" : : "r"(s28) : "s28");
}

__STATIC_FORCEINLINE uint32_t __get_S29(void)
{
	uint32_t result;

	__ASM volatile("VMOV %0, s29" : "=r"(result));
	return result;
}

__STATIC_FORCEINLINE void __set_S29(uint32_t s29)
{
	__ASM volatile("VMOV s29, %0" : : "r"(s29) : "s29");
}

__STATIC_FORCEINLINE uint32_t __get_S30(void)
{
	uint32_t result;

	__ASM volatile("VMOV %0, s30" : "=r"(result));
	return result;
}

__STATIC_FORCEINLINE void __set_S30(uint32_t s30)
{
	__ASM volatile("VMOV s30, %0" : : "r"(s30) : "s30");
}

__STATIC_FORCEINLINE uint32_t __get_S31(void)
{
	uint32_t result;

	__ASM volatile("VMOV %0, s31" : "=r"(result));
	return result;
}

__STATIC_FORCEINLINE void __set_S31(uint32_t s31)
{
	__ASM volatile("VMOV s31, %0" : : "r"(s31) : "s31");
}

/* Push FPU registers S16-S31 to stack */
__STATIC_FORCEINLINE void __vpush_s16_s31(void)
{
	__ASM volatile("VPUSH {s16-s31}" : : : "memory");
}

/* Pop FPU registers S16-S31 from stack */
__STATIC_FORCEINLINE void __vpop_s16_s31(void)
{
	__ASM volatile("VPOP {s16-s31}" : : : "memory");
}

/* Test FPSCR (Floating-Point Status and Control Register) */
static inline int iec60730b_test_fpu_cortex_fpscr(void)
{
	uint32_t fpscr_backup;
	uint32_t test_value;

	/* Backup FPSCR */
	fpscr_backup = __get_FPSCR();

	__set_FPSCR(pattern1 & FPSCR_BITMASK);
	test_value = __get_FPSCR();
	if (test_value != (pattern1 & FPSCR_BITMASK)) {
		goto error;
	}

	__set_FPSCR(pattern2 & FPSCR_BITMASK);
	test_value = __get_FPSCR();
	if (test_value != (pattern2 & FPSCR_BITMASK)) {
		goto error;
	}

	__set_FPSCR(fpscr_backup);
	return IEC60730B_TEST_OK;
error:
	__set_FPSCR(fpscr_backup);
	return IEC60730B_TEST_FPU_ERROR;
}

/* Tests FPU registers S0-S31 and FPSCR */
static inline int iec60730b_test_fpu_cortex(void)
{
	uint32_t cpacr_backup;

	/* Backup current CPACR value */
	cpacr_backup = SCB->CPACR;

	/* Enable FPU (set CP10 and CP11 to full access) */
	SCB->CPACR |= CPACR_CP10_CP11_MASK;
	__DSB();
	__ISB();

	/* Test FPSCR */
	if (iec60730b_test_fpu_cortex_fpscr() != IEC60730B_TEST_OK) {
		goto error;
	}

	/* Test S0 */
	__set_S0(pattern1);
	if (__get_S0() != pattern1) {
		goto error;
	}
	__set_S0(pattern2);
	if (__get_S0() != pattern2) {
		goto error;
	}

	/* Test S1 */
	__set_S1(pattern1);
	if (__get_S1() != pattern1) {
		goto error;
	}
	__set_S1(pattern2);
	if (__get_S1() != pattern2) {
		goto error;
	}

	/* Test S2 */
	__set_S2(pattern1);
	if (__get_S2() != pattern1) {
		goto error;
	}
	__set_S2(pattern2);
	if (__get_S2() != pattern2) {
		goto error;
	}

	/* Test S3 */
	__set_S3(pattern1);
	if (__get_S3() != pattern1) {
		goto error;
	}
	__set_S3(pattern2);
	if (__get_S3() != pattern2) {
		goto error;
	}

	/* Test S4 */
	__set_S4(pattern1);
	if (__get_S4() != pattern1) {
		goto error;
	}
	__set_S4(pattern2);
	if (__get_S4() != pattern2) {
		goto error;
	}

	/* Test S5 */
	__set_S5(pattern1);
	if (__get_S5() != pattern1) {
		goto error;
	}
	__set_S5(pattern2);
	if (__get_S5() != pattern2) {
		goto error;
	}

	/* Test S6 */
	__set_S6(pattern1);
	if (__get_S6() != pattern1) {
		goto error;
	}
	__set_S6(pattern2);
	if (__get_S6() != pattern2) {
		goto error;
	}

	/* Test S7 */
	__set_S7(pattern1);
	if (__get_S7() != pattern1) {
		goto error;
	}
	__set_S7(pattern2);
	if (__get_S7() != pattern2) {
		goto error;
	}

	/* Test S8 */
	__set_S8(pattern1);
	if (__get_S8() != pattern1) {
		goto error;
	}
	__set_S8(pattern2);
	if (__get_S8() != pattern2) {
		goto error;
	}

	/* Test S9 */
	__set_S9(pattern1);
	if (__get_S9() != pattern1) {
		goto error;
	}
	__set_S9(pattern2);
	if (__get_S9() != pattern2) {
		goto error;
	}

	/* Test S10 */
	__set_S10(pattern1);
	if (__get_S10() != pattern1) {
		goto error;
	}
	__set_S10(pattern2);
	if (__get_S10() != pattern2) {
		goto error;
	}

	/* Test S11 */
	__set_S11(pattern1);
	if (__get_S11() != pattern1) {
		goto error;
	}
	__set_S11(pattern2);
	if (__get_S11() != pattern2) {
		goto error;
	}

	/* Test S12 */
	__set_S12(pattern1);
	if (__get_S12() != pattern1) {
		goto error;
	}
	__set_S12(pattern2);
	if (__get_S12() != pattern2) {
		goto error;
	}

	/* Test S13 */
	__set_S13(pattern1);
	if (__get_S13() != pattern1) {
		goto error;
	}
	__set_S13(pattern2);
	if (__get_S13() != pattern2) {
		goto error;
	}

	/* Test S14 */
	__set_S14(pattern1);
	if (__get_S14() != pattern1) {
		goto error;
	}
	__set_S14(pattern2);
	if (__get_S14() != pattern2) {
		goto error;
	}

	/* Test S15 */
	__set_S15(pattern1);
	if (__get_S15() != pattern1) {
		goto error;
	}
	__set_S15(pattern2);
	if (__get_S15() != pattern2) {
		goto error;
	}

	/* Push FPU preserved registers S16-S31 */
	__vpush_s16_s31();

	/* Test S16 */
	__set_S16(pattern1);
	if (__get_S16() != pattern1) {
		goto error1;
	}
	__set_S16(pattern2);
	if (__get_S16() != pattern2) {
		goto error1;
	}

	/* Test S17 */
	__set_S17(pattern1);
	if (__get_S17() != pattern1) {
		goto error1;
	}
	__set_S17(pattern2);
	if (__get_S17() != pattern2) {
		goto error1;
	}

	/* Test S18 */
	__set_S18(pattern1);
	if (__get_S18() != pattern1) {
		goto error1;
	}
	__set_S18(pattern2);
	if (__get_S18() != pattern2) {
		goto error1;
	}

	/* Test S19 */
	__set_S19(pattern1);
	if (__get_S19() != pattern1) {
		goto error1;
	}
	__set_S19(pattern2);
	if (__get_S19() != pattern2) {
		goto error1;
	}

	/* Test S20 */
	__set_S20(pattern1);
	if (__get_S20() != pattern1) {
		goto error1;
	}
	__set_S20(pattern2);
	if (__get_S20() != pattern2) {
		goto error1;
	}

	/* Test S21 */
	__set_S21(pattern1);
	if (__get_S21() != pattern1) {
		goto error1;
	}
	__set_S21(pattern2);
	if (__get_S21() != pattern2) {
		goto error1;
	}

	/* Test S22 */
	__set_S22(pattern1);
	if (__get_S22() != pattern1) {
		goto error1;
	}
	__set_S22(pattern2);
	if (__get_S22() != pattern2) {
		goto error1;
	}

	/* Test S23 */
	__set_S23(pattern1);
	if (__get_S23() != pattern1) {
		goto error1;
	}
	__set_S23(pattern2);
	if (__get_S23() != pattern2) {
		goto error1;
	}

	/* Test S24 */
	__set_S24(pattern1);
	if (__get_S24() != pattern1) {
		goto error1;
	}
	__set_S24(pattern2);
	if (__get_S24() != pattern2) {
		goto error1;
	}

	/* Test S25 */
	__set_S25(pattern1);
	if (__get_S25() != pattern1) {
		goto error1;
	}
	__set_S25(pattern2);
	if (__get_S25() != pattern2) {
		goto error1;
	}

	/* Test S26 */
	__set_S26(pattern1);
	if (__get_S26() != pattern1) {
		goto error1;
	}
	__set_S26(pattern2);
	if (__get_S26() != pattern2) {
		goto error1;
	}

	/* Test S27 */
	__set_S27(pattern1);
	if (__get_S27() != pattern1) {
		goto error1;
	}
	__set_S27(pattern2);
	if (__get_S27() != pattern2) {
		goto error1;
	}

	/* Test S28 */
	__set_S28(pattern1);
	if (__get_S28() != pattern1) {
		goto error1;
	}
	__set_S28(pattern2);
	if (__get_S28() != pattern2) {
		goto error1;
	}

	/* Test S29 */
	__set_S29(pattern1);
	if (__get_S29() != pattern1) {
		goto error1;
	}
	__set_S29(pattern2);
	if (__get_S29() != pattern2) {
		goto error1;
	}

	/* Test S30 */
	__set_S30(pattern1);
	if (__get_S30() != pattern1) {
		goto error1;
	}
	__set_S30(pattern2);
	if (__get_S30() != pattern2) {
		goto error1;
	}

	/* Test S31 */
	__set_S31(pattern1);
	if (__get_S31() != pattern1) {
		goto error1;
	}
	__set_S31(pattern2);
	if (__get_S31() != pattern2) {
		goto error1;
	}

	__vpop_s16_s31();
	SCB->CPACR = cpacr_backup;
	return IEC60730B_TEST_OK;

error1:
	__vpop_s16_s31();
error:
	SCB->CPACR = cpacr_backup;
	return IEC60730B_TEST_FPU_ERROR;
}

/*
 * Test FPU registers for IEC 60730 Class B compliance
 */
int iec60730b_test_fpu(void)
{
	if (iec60730b_test_fpu_cortex() != IEC60730B_TEST_OK) {
		return IEC60730B_TEST_FPU_ERROR;
	}

	return IEC60730B_TEST_OK;
}

#endif /* CONFIG_IEC60730B_TEST_FPU && CONFIG_ARMV7_M_ARMV8_M_FP */
