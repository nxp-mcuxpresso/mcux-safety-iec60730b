/*
 * Copyright 2025 NXP
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef _IEC60730B_TEST_H_
#define _IEC60730B_TEST_H_

#include <zephyr/types.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/adc.h>

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*!
 * @name Safety test error codes
 * @brief Error code definitions for IEC 60730 Class B safety test failures
 */
#define IEC60730B_TEST_OK                       (0)      /* No error */
#define IEC60730B_TEST_NOT_SUPPORTED            (1)      /* Not supported */
#define IEC60730B_TEST_ERROR                    (-1)     /* General error */
#define IEC60730B_TEST_CPU_ERROR                (-2)     /* CPU registers test fault */
#define IEC60730B_TEST_FPU_ERROR                (-3)     /* Floating point registers test fault */
#define IEC60730B_TEST_WDT_ERROR                (-4)     /* Watchdog error */
#define IEC60730B_TEST_FLASH_ERROR              (-5)     /* Flash test fault */
#define IEC60730B_TEST_RAM_ERROR                (-6)     /* RAM test fault */
#define IEC60730B_TEST_STACK_ERROR              (-7)     /* Stack test fault */
#define IEC60730B_TEST_CLOCK_ERROR              (-8)     /* Clock test fault */
#define IEC60730B_TEST_PC_ERROR                 (-9)     /* Program counter test fault */
#define IEC60730B_TEST_DIO_ERROR                (-10)    /* Digital Input/Output test fault */
#define IEC60730B_TEST_AIO_ERROR                (-11)    /* Analog Input/Output test fault */

/*!
 * @brief RAM test algorithm types for IEC 60730 Class B compliance
 *
 * @kconfig_dep{CONFIG_IEC60730B_TEST_RAM}
 * 
 * This enumeration defines the available RAM test algorithms that can be
 * used to detect memory faults as required by IEC 60730 Class B safety
 * standards.
 */
typedef enum {
    IEC60730B_TEST_RAM_TYPE_NONE = 0,    /*!< No RAM test algorithm */
    /**
     * March-C algorithm for RAM testing
     * 
     * Complexity: O(10n) - 10 operations per memory cell
     * 
     * Fault coverage:
     * - Stuck-at faults (SAF): Detects bits permanently stuck at 0 or 1
     * - Address decoder faults (AF): Detects incorrect address decoding
     * - Transition faults (TF): Detects failures when changing bit values
     * - Coupling faults (CF): Detects when one bit affects another
     * - Bridging faults (BF): Detects short circuits between memory lines
     * 
     * Recommended for IEC 60730 Class B due to comprehensive fault coverage
     */
    IEC60730B_TEST_RAM_TYPE_MARCH_C,
    /**
     * March-X algorithm for RAM testing
     * 
     * Complexity: O(6n) - 6 operations per memory cell  
     * 
     * Fault coverage:
     * - Stuck-at faults (SAF): Detects bits permanently stuck at 0 or 1
     * - Address decoder faults (AF): Detects incorrect address decoding
     * - Transition faults (TF): Detects failures when changing bit values
     * - Limited coupling faults (CF): Basic coupling fault detection
     * 
     * Faster execution than March-C but with reduced fault coverage.
     * Suitable when execution time is critical and basic fault detection
     * is sufficient for the safety requirements.
     */
    IEC60730B_TEST_RAM_TYPE_MARCH_X
} iec60730b_test_ram_type_t;


/*!
 * @brief ADC channel limits structure for allowed ADC result
 * used in Analog test for IEC 60730 Class B compliance
 *
 * @kconfig_dep{CONFIG_IEC60730B_TEST_AIO}
 * 
 * This structure provides allowed upper, lower limits
 * for raw ADC conversion result for Analog test to be valid
 * for ADC module testing.
 * 
 */
typedef struct
{
    uint32_t low;
    uint32_t high;
} iec60730b_adc_limits_t;

/*!
 * @brief Adc channel specification structure used in Analog test for IEC 60730 Class B compliance
 *
 * @kconfig_dep{CONFIG_IEC60730B_TEST_AIO}
 * 
 * This structure provides the channel specification data for ADC module testing.
 * 
 */
struct iec60730b_adc_channel {
    
    /* ADC resolution to be used for that channel. */
    uint8_t resolution;
    
    /* Voltage of the reference selected for the channel
     * specified in devicetree or by user application.
     */
    uint16_t vref_mv;
    
    /* Configuration of the associated ADC channel
     * specified in devicetree or by user application.
     */
    struct adc_channel_cfg channel_cfg;
    
    /* ADC channel limits (high,low) specifying
     * allowed guards for raw ADC conversion result
     * with deviation accounted.
     */
    iec60730b_adc_limits_t limits;
    
    /* Allowed deviation range of measured value,
     * percentage from maximum ADC value
     * recommended deviation range value is 5 %
     */
    uint8_t allowed_deviation;
};

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************
 * API
 ******************************************************************************/
/*!
 * @brief Test CPU registers for IEC 60730 Class B compliance
 *
 * @kconfig_dep{CONFIG_IEC60730B_TEST_CPU}
 * 
 * This function performs a comprehensive test of CPU registers to detect
 * stuck-at faults and ensure proper register functionality as required
 * by IEC 60730 Class B.
 * 
 * @return 0 on success, negative on failure
 */
int iec60730b_test_cpu(void);

/**
 * @brief Test FPU (Floating Point Unit) registers
 * 
 * @kconfig_dep{CONFIG_IEC60730B_TEST_FPU}
 *
 * This function performs IEC 60730 Class B safety test on the FPU registers
 * to detect stuck-at faults and ensure proper operation of floating point
 * hardware.
 *
 * @return 0 if test passes, non-zero error code if test fails
 */
int iec60730b_test_fpu(void);

/*!
 * @brief Test RAM memory for IEC 60730 Class B compliance
 *
 * @kconfig_dep{CONFIG_IEC60730B_TEST_RAM}
 * 
 * This function performs comprehensive RAM testing using the specified algorithm
 * to detect memory faults such as stuck-at bits, coupling faults, and addressing
 * faults as required by IEC 60730 Class B.
 * 
 * @param ram Pointer to the RAM memory area to be tested
 * @param ram_size Size of the RAM memory area in bytes
 * @param backup Pointer to backup memory area for data preservation
 * @param backup_size Size of the backup memory area in bytes
 * @param type RAM test algorithm type to be used
 * 
 * @return 0 on success, negative on failure
 */
int iec60730b_test_ram(uint8_t *ram, size_t ram_size, uint8_t *backup, size_t backup_size, iec60730b_test_ram_type_t type);

/*!
 * @brief Test Program Counter for IEC 60730 Class B compliance
 *
 * @kconfig_dep{CONFIG_IEC60730B_TEST_PC}
 * 
 * This function performs Program Counter testing to detect execution flow
 * corruption and ensure proper program execution as required by IEC 60730
 * Class B. The test validates that the processor correctly
 * handles jumps, calls, and returns.
 * 
 * @return 0 on success, negative error code on failure
 */
int iec60730b_test_pc(void);

/*!
 * @brief Initialize stack testing for IEC 60730 Class B compliance
 *
 * @kconfig_dep{CONFIG_IEC60730B_TEST_STACK}
 * 
 * This function initializes the stack testing mechanism by setting up
 * guard patterns and boundaries to detect stack overflow and underflow
 * conditions as required by IEC 60730 Class B.
 * 
 * @param stack_start Starting address of the stack memory area.
 * @param stack_size Size of the stack memory area in bytes.
 * @param guard_size Size of the guard area in bytes. The guard zones provide additional protection
 *                   by reserving memory before and after the actual thread stack.
 * @param guard_pattern Pattern value used for guard area initialization
 * 
 * @return 0 on success, negative on failure
 */
int iec60730b_test_stack_init(void *stack_start, size_t stack_size, size_t guard_size, uint32_t guard_pattern);

/*!
 * @brief Test stack integrity for IEC 60730 Class B compliance
 *
 * @kconfig_dep{CONFIG_IEC60730B_TEST_STACK}
 * 
 * This function performs stack integrity testing by checking guard patterns
 * and boundaries to detect stack overflow and underflow conditions as
 * required by IEC 60730 Class B.
 * 
 * @param stack_start Starting address of the stack memory area.
 * @param stack_size Size of the stack memory area in bytes.
 * @param guard_size Size of the guard area in bytes. The guard zones provide additional protection
 *                   by reserving memory before and after the actual thread stack.
 * @param guard_pattern Pattern value used for guard area verification.
 * 
 * @return 0 on success, negative on failure
 */
int iec60730b_test_stack(void *stack_start, size_t stack_size, size_t guard_size, uint32_t guard_pattern);

/*!
 * @brief Test flash memory using CRC16 for IEC 60730 Class B compliance
 *
 * @kconfig_dep{CONFIG_IEC60730B_TEST_FLASH_CRC16}
 * 
 * This function performs flash memory integrity testing using CRC16 checksum
 * calculation to detect memory corruption and ensure data integrity as
 * required by IEC 60730 Class B. The test calculates a CRC16 checksum over
 * the specified memory region and compares it against the expected value.
 * 
 * @param start Starting address of the flash memory area to test.
 * @param size Size of the flash memory area in bytes.
 * @param crc_expected Expected CRC16 checksum value for comparison.
 * 
 * @return 0 on success, negative on failure
 */
int iec60730b_test_flash_crc16(const void *start, size_t size, uint16_t crc_expected);

/*!
 * @brief Test flash memory using CRC32 for IEC 60730 Class B compliance
 *
 * @kconfig_dep{CONFIG_IEC60730B_TEST_FLASH_CRC32}
 * 
 * This function performs flash memory integrity testing using CRC32 checksum
 * calculation to detect memory corruption and ensure data integrity as
 * required by IEC 60730 Class B. The test calculates a CRC32 checksum over
 * the specified memory region and compares it against the expected value.
 * 
 * @param start Starting address of the flash memory area to test.
 * @param size Size of the flash memory area in bytes.
 * @param crc_expected Expected CRC32 checksum value for comparison.
 * 
 * @return 0 on success, negative on failure
 */
int iec60730b_test_flash_crc32(const void *start, size_t size, uint32_t crc_expected);

/*!
 * @brief Test digital input pin for IEC 60730 Class B compliance
 *
 * @kconfig_dep{CONFIG_IEC60730B_TEST_DIO}
 * 
 * This function performs digital input testing by reading the state
 * of a specified GPIO pin and comparing it against the expected value to
 * detect hardware faults as required by IEC 60730 Class B.
 * 
 * @param port Pointer to the GPIO device structure.
 * @param pin GPIO pin number to test.
 * @param pin_expected_value Expected raw state of the pin (true for high, false for low).
 * 
 * @return 0 on success, negative on failure
 */
int iec60730b_test_dio_input(const struct device *port, gpio_pin_t pin, bool pin_expected_value);

/*!
 * @brief Test digital output pin for IEC 60730 Class B compliance
 *
 * @kconfig_dep{CONFIG_IEC60730B_TEST_DIO}
 * 
 * This function performs digital output testing by controlling the state
 * of a specified GPIO pin to detect hardware faults as required by 
 * IEC 60730 Class B.
 * 
 * @param port Pointer to the GPIO device structure.
 * @param pin GPIO pin number to test.
 * 
 * @return 0 on success, negative on failure
 */
int iec60730b_test_dio_output(const struct device *port, gpio_pin_t pin);

/*!
 * @brief Initialize clock test for IEC 60730 Class B compliance
 * 
 * @kconfig_dep{CONFIG_IEC60730B_TEST_CLOCK}
 * 
 * This function initializes the clock testing mechanism by configuring
 * a counter device and timer parameters to monitor system clock integrity
 * as required by IEC 60730 Class B. The test verifies that the system
 * clock operates within acceptable tolerance limits.
 * 
 * @param counter Pointer to the counter device structure used for clock monitoring.
 * @param timer_period_ms Timer period for clock test measurements in milliseconds.
 * @param tolerance_percent Acceptable clock deviation tolerance as a percentage.
 * 
 * @return 0 on success, negative on failure
 */
int iec60730b_test_clock_init(const struct device *counter, uint32_t timer_period_ms, uint32_t tolerance_percent);

/*!
 * @brief Perform clock test for IEC 60730 Class B compliance
 *
 * @kconfig_dep{CONFIG_IEC60730B_TEST_CLOCK}
 * 
 * This function executes the clock test to verify that the system clock
 * operates within the tolerance limits configured during initialization.
 * It should be called periodically after iec60730b_test_clock_init() to
 * ensure continuous monitoring of clock integrity as required by IEC 60730 Class B.
 * 
 * @return 0 on success, negative on failure
 */
int iec60730b_test_clock(void);

/*!
 * @brief Initialize ADC channel for Safety AIO Test required by IEC 60730 Class B.
 * 
 * @kconfig_dep{CONFIG_IEC60730B_TEST_AIO}
 * 
 * This function verifies that the adc device is ready (was initialized by zephyr),
 * then sets up the specific ADC channel and calls function to set the channel limits.
 * 
 * @param dev Pointer to the ADC device structure used for measuring voltage to test the function of ADC module.
 * @param channel Pointer to the ADC channel specification with parameters from board overlay or entered by user application.
 * @param channel_expected_voltage_mv Voltage signal value in mV that is connected to the measuring ADC channel,
 * and expected to be read by ADC conversion.
 * 
 * @return 0 on success, negative on failure
 */
int iec60730b_aio_init_channel(const struct device* dev, struct iec60730b_adc_channel* channel, uint32_t channel_expected_voltage_mv);

/*!
 * @brief Test analog input pins for IEC 60730 Class B compliance
 * 
 * @kconfig_dep{CONFIG_IEC60730B_TEST_AIO}
 * 
 * This function performs analog input testing to detect hardware faults as required by
 * IEC 60730 Class B. Function measures analog voltage on a specified ADC channel,
 * then compares the measured value against the expected voltage value, counting
 * with deviation tolerance to ensure the ADC hardware is functioning correctly.
 * 
 * @param dev Pointer to the ADC device structure used for measuring voltage
 * to test the function of ADC module.
 * @param channel Pointer to the ADC channel specification with parameters from user application or board overlay.
 * 
 * @return 0 on success, negative on failure
 */
int iec60730b_test_aio(const struct device* dev, struct iec60730b_adc_channel* channel);


/*!
 * @brief Test watchdog timer for IEC 60730 Class B compliance
 * 
 * @kconfig_dep{CONFIG_IEC60730B_TEST_WDOG}
 * 
 * This function performs watchdog timer testing to verify that the watchdog
 * hardware is functioning correctly as required by IEC 60730 Class B. It uses
 * a counter device to measure the actual watchdog timeout period and compares
 * it against the expected timeout value within the specified tolerance.
 * 
 * @param wdog Pointer to the watchdog device structure to be tested.
 * @param wdog_timeout_ms Expected watchdog timeout period in milliseconds.
 * @param counter Pointer to the counter device structure used for measuring the actual timeout.
 * @param tolerance_percent Acceptable deviation tolerance as a percentage of the expected timeout.
 * 
 * @return 0 on success, negative on failure
 */
int iec60730b_test_wdog(const struct device *wdog, uint32_t wdog_timeout_ms, const struct device *counter, uint32_t tolerance_percent);

#ifdef __cplusplus
}
#endif

#endif /* _IEC60730B_TEST_H_ */
