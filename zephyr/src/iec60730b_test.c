/*
 * Copyright 2026 NXP
 * SPDX-License-Identifier: Apache-2.0
 */

#include "iec60730b_test.h"

#ifdef CONFIG_IEC60730B_TEST_CPU_REG
__weak int iec60730b_test_cpu_reg(void)
{
    return IEC60730B_TEST_NOT_SUPPORTED;
}
#endif /* CONFIG_IEC60730B_TEST_CPU_REG */

#ifdef CONFIG_IEC60730B_TEST_RAM
__weak int iec60730b_test_ram(uint8_t *ram, size_t ram_size, uint8_t *backup, size_t backup_size, iec60730b_test_ram_type_t type)
{
    return IEC60730B_TEST_NOT_SUPPORTED;
}
#endif /* CONFIG_IEC60730B_TEST_RAM */

#ifdef CONFIG_IEC60730B_TEST_PC
__weak int iec60730b_test_pc(void)
{
    return IEC60730B_TEST_NOT_SUPPORTED;
}
#endif /* CONFIG_IEC60730B_TEST_PC */

#ifdef CONFIG_IEC60730B_TEST_STACK
__weak int iec60730b_test_stack_init(void *stack_start, size_t stack_size, size_t guard_size, uint32_t guard_pattern)
{
    return IEC60730B_TEST_NOT_SUPPORTED;
}

__weak int iec60730b_test_stack(void *stack_start, size_t stack_size, size_t guard_size, uint32_t guard_pattern)
{
    return IEC60730B_TEST_NOT_SUPPORTED;
}
#endif /* CONFIG_IEC60730B_TEST_STACK */

#ifdef CONFIG_IEC60730B_TEST_FLASH
__weak int iec60730b_test_flash_crc(const void *start, size_t size, iec60730b_flash_crc_t crc_expected)
{
    return IEC60730B_TEST_NOT_SUPPORTED;
}
#endif /* CONFIG_IEC60730B_TEST_FLASH */

#ifdef CONFIG_IEC60730B_TEST_DIO
__weak int iec60730b_test_dio_input(const struct device *port, gpio_pin_t pin, bool pin_expected_value)
{
    return IEC60730B_TEST_NOT_SUPPORTED;
}

__weak int iec60730b_test_dio_output(const struct device *port, gpio_pin_t pin)
{
    return IEC60730B_TEST_NOT_SUPPORTED;
}
#endif /* CONFIG_IEC60730B_TEST_DIO */

#ifdef CONFIG_IEC60730B_TEST_CLOCK
__weak int iec60730b_test_clock_init(const struct device *counter, k_timeout_t timer_period, uint32_t tolerance_percent)
{
    return IEC60730B_TEST_NOT_SUPPORTED;
}

__weak int iec60730b_test_clock(void)
{
    return IEC60730B_TEST_NOT_SUPPORTED;
}
#endif /* CONFIG_IEC60730B_TEST_CLOCK */