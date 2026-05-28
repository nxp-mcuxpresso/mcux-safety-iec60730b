/*
 * Copyright 2025-2026 NXP
 * SPDX-License-Identifier: Apache-2.0
 */

#include "iec60730b_test.h"

#if defined(CONFIG_IEC60730B_TEST_PC) && defined(CONFIG_CPU_CORTEX)

/*
 * Verification handler for Program Counter test
 *
 * This handler is called from the instruction pattern function to verify
 * that the Program Counter value was correctly captured. It compares the
 * expected PC value with the actual value and sets the test result accordingly.
 */
__attribute__((naked, used)) static int iec60730b_test_pc_cortex_verify_handler(void)
{
	__asm__ volatile(".syntax unified                  \n"
			 "ADDS   R4, #4                    \n" /* increment pattern addr */
			 "CMP    R4, R7                    \n" /* compare with PC value */
			 "BNE    PCErrorCM7                \n" /* branch if different */
			 "MOVS   R0, %[pass]               \n" /* pass return */
			 "STR    R0, [R2]                  \n" /* clear the flag */
			 "BX     LR                        \n"

			 "PCErrorCM7:                      \n"
			 "LDR    R0, =%[fail_pc]           \n" /* set result to false */
			 "BX     LR                        \n"

			 : /* No outputs */
			 : [pass] "i"(IEC60730B_TEST_OK), [fail_pc] "i"(IEC60730B_TEST_PC_ERROR)
			 : /* No clobbers - naked function */
	);
}

/*
 * Instruction pattern function for Program Counter test
 *
 * This function executes a specific instruction pattern to test the Program
 * Counter register. It captures the current PC value and then jumps to the
 * verification handler to validate the captured value.
 */
__attribute__((naked, used)) static void iec60730b_test_pc_cortex_instruction_pattern(void)
{
	__asm__ volatile(".syntax unified                  \n"
			 "MOV    R7, PC                    \n" /* copy PC into R7 */
			 "MOV    PC, R6                    \n" /* jump to handler */
	);
}

/*
 * Main Program Counter test function for Cortex-M processors
 *
 * This function orchestrates the Program Counter test by setting up the necessary
 * registers and calling the instruction pattern function. It prepares the test
 * environment, executes the PC capture sequence, and returns the test result.
 */
__attribute__((naked)) static int iec60730b_test_pc_cortex(uint32_t *flagAddr)
{
	__asm__ volatile(".syntax unified                  \n"

			 /* test pattern......R0 */
			 /* flag address......R1 */

			 "PUSH   {R4, R5, R6, R7, LR}      \n"

			 "MOV    R2, R0                    \n" /* flag address -> R2 */
			 "LDR    R4, =iec60730b_test_pc_cortex_instruction_pattern\n"
			 "MOVS   R3, #1                    \n" /* Thumb bit mask */
			 "BICS   R4, R4, R3                \n" /* clear Thumb bit */
			 "LDR    R0, =%[fail_pc]           \n" /* pre-set to false */
			 "LDR    R6, =iec60730b_test_pc_cortex_verify_handler\n"
			 "STR    R3, [R2]                  \n" /* set the flag */

			 "MOV    R5, PC                    \n" /* Program counter */
			 "ADDS   R5, R5, #0x5              \n" /* offset for LR */
			 "MOV    LR, R5                    \n" /* fill LR */
			 "MOV    PC, R4                    \n" /* jump to pattern */

			 "POP    {R4, R5, R6, R7, PC}      \n"

			 : /* No outputs */
			 : [fail_pc] "i"(IEC60730B_TEST_PC_ERROR)
			 : /* No clobbers - naked function */
	);
}

/*
 * Test Program Counter for IEC 60730 Class B compliance
 */
int iec60730b_test_pc(void)
{
	uint32_t pc_test_flag = 0xFFFFFFFF;
	int res;

	res = iec60730b_test_pc_cortex(&pc_test_flag);

	/* Check if PC test failed or flag was not cleared */
	if ((res != IEC60730B_TEST_OK) || (pc_test_flag != 0)) {
		return IEC60730B_TEST_PC_ERROR;
	}

	return IEC60730B_TEST_OK;
}

#endif /* CONFIG_IEC60730B_TEST_PC */
