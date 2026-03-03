/*
 * Copyright 2025 NXP
 * SPDX-License-Identifier: Apache-2.0
 */

#include "iec60730b_test.h"
#include <iec60730b.h>
#include <zephyr/drivers/adc.h>

#ifdef CONFIG_IEC60730B_TEST_AIO

/* ADC Types
* CONFIG_ADC_MCUX_12B1MSPS_SAR  RT1050,RT1060
* CONFIG_ADC_MCUX_ADC12         KE17Z512,KE1xZ,KE1xF
* CONFIG_ADC_MCUX_ADC16         mcxc242,mcxc444,mcxe247,KV11,K32L2x,K22F
* CONFIG_ADC_MCUX_GAU           RW612
* CONFIG_ADC_MCUX_LPADC         LPC55S36,mcxaxx6,mcxa15x,RT1170
* CONFIG_ADC_MCUX_SAR_ADC       iMX93,mcxe31b
*/
/*******************************************************************************
* Definitions
******************************************************************************/
/* Maximum ADC raw value based on resolution */
#define ADC_MAX(resolution)                         (uint32_t)((1 << (resolution)) - 1)
/* Raw percentage value of ADC range */
#define ADC_RANGE_PERCENT(resolution, percent)       (uint32_t)(((ADC_MAX(resolution)) * (percent)) / 100)
/* Get raw adc value from voltage in mV */
#define ADC_VOLTAGE_TO_RAW(voltage, vref, resolution) (uint32_t)(((voltage) * (ADC_MAX(resolution))) / (vref))
/*******************************************************************************
* Prototypes
******************************************************************************/
/*!
 * @brief Set tolerance range limits (raw adc value) for adc channel according to expected voltage and ±deviation percentage.
 * 
 * @kconfig_dep{CONFIG_IEC60730B_TEST_AIO}
 * 
 * This function sets upper and lower limits for a specific adc channel according to the channel_expected_voltage_mv parameter,
 * and allowed deviation percentage defined for the function. The calculated limit values are saved to the limits.
 * 
 * @param channel Pointer to the ADC channel specification with parameters from board overlay or entered by user application.
 * @param channel_expected_voltage_mv Voltage signal value in mV that is connected to the measuring ADC channel,
 * and expected to be read by ADC conversion.
 * @param channel_limits Pointer to the structure for allowed limits for the specific channel.
 * 
 * @return 0 on success, negative on failure
 */
static int iec60730b_aio_set_channel_limits(struct iec60730b_adc_channel* channel,  uint32_t channel_expected_voltage_mv);

/*******************************************************************************
* Functions
******************************************************************************/
static int iec60730b_aio_set_channel_limits(struct iec60730b_adc_channel* channel,  uint32_t channel_expected_voltage_mv)
{
    /* Get channel limits structure pointer from channel struct */
    iec60730b_adc_limits_t *channel_limits = &channel->limits;
    /* Get allowed deviation % of max ADC value */
    uint8_t allowed_deviation = (uint8_t)(channel->allowed_deviation);
    /* Get ADC resolution from channel struct */
    uint8_t resolution = (uint8_t)(channel->resolution);
    /* Get reference voltage from channel struct in mV */
    uint16_t vref_mv = (uint16_t)(channel->vref_mv);
    /* Error if expected voltage is bigger than reference voltage */
    if (channel_expected_voltage_mv > vref_mv) {
        return -1;
    }

    /* Get raw value from expected voltage */
    uint32_t adc_channnel_expected_raw = ADC_VOLTAGE_TO_RAW(channel_expected_voltage_mv, vref_mv, resolution);

    /* Check if expected raw value is bigger than allowed deviation % of ADC range to avoid underflow */
    if (adc_channnel_expected_raw > ADC_RANGE_PERCENT(resolution, allowed_deviation)) {
        /* Set MIN ADC raw value limit to expected_raw minus deviation percent */
        channel_limits->low = adc_channnel_expected_raw - ADC_RANGE_PERCENT(resolution, allowed_deviation);
    } else {
        /* Set MIN ADC raw value limit to 0U */
        channel_limits->low = 0U;
    }

    /* Check if expected raw value is smaller than (100 - deviation) % to avoid overflow */
    if (adc_channnel_expected_raw < ADC_RANGE_PERCENT(resolution, (100 - allowed_deviation))) {
        /* Set MAX ADC raw value limit to expected_raw plus deviation percent */
        channel_limits->high = adc_channnel_expected_raw + ADC_RANGE_PERCENT(resolution, allowed_deviation);
    } else {
        /* Set MAX ADC raw value limit to 100% of ADC range */
        channel_limits->high = ADC_RANGE_PERCENT(resolution, 100);
    }

    return 0;
}

int iec60730b_aio_init_channel(const struct device* dev, struct iec60730b_adc_channel* channel, uint32_t channel_expected_voltage_mv)
{
    int err;
    /* Check if device points to valid ADC device */
    if (dev == NULL) {
        return IEC60730B_TEST_NOT_SUPPORTED;
    }

    err = adc_channel_setup(dev, &channel->channel_cfg);
    if (err < 0) {
        /* Could not setup channel */
        return IEC60730B_TEST_AIO_ERROR;
    }
    
    err = iec60730b_aio_set_channel_limits(channel, channel_expected_voltage_mv);
    if (err < 0) {
        /* Could not set channel limits for channel */
        return IEC60730B_TEST_AIO_ERROR;
    }
    return IEC60730B_TEST_OK;
}

/* Test analog I/O pins for IEC 60730 Class B compliance */
int iec60730b_test_aio(const struct device* dev, struct iec60730b_adc_channel* channel)
{
    int err;
    FS_RESULT limit_ret;
    
    /* Check if device points to valid ADC device */
    if (dev == NULL) {
        return IEC60730B_TEST_NOT_SUPPORTED;
    }

    /* Initialize ADC sequence structure with channel configuration */
    uint16_t buf;
    struct adc_sequence sequence = {
        .buffer = &buf,
        /* buffer size in bytes, not number of samples */
        .buffer_size = sizeof(buf),
        .channels = BIT(channel->channel_cfg.channel_id),
        .resolution = channel->resolution,
    #if CONFIG_SAMPLE_ADC_CALIBRATE_REQUIRED
        .calibrate = true,
    #endif
    };

    /* Read ADC conversion measurement on the ADC channel from sequence structure */
    err = adc_read(dev, &sequence);
    if (err < 0) {
        /* Could not read channel */
        return IEC60730B_TEST_AIO_ERROR;
    }

    /* Check if adc result is within allowed LIMITS */
    FS_RESULT adc_test_state = FS_AIO_SCAN_COMPLETE;
    limit_ret = FS_AIO_LimitCheck(buf, (fs_aio_limits_t*)&channel->limits, &adc_test_state);
    if (limit_ret != FS_PASS) {
        /* Raw (voltage) Limit check did not pass, return error */
        return IEC60730B_TEST_AIO_ERROR;
    }
    return IEC60730B_TEST_OK;
}
#endif /* CONFIG_IEC60730B_TEST_AIO */
