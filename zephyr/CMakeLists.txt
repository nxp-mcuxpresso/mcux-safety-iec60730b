# Copyright 2025 NXP
# SPDX-License-Identifier: Apache-2.0

if(CONFIG_SAFETY_IEC60730B)

    # Set path varriables
    set(SAFETY_IEC60730B_DIR ${ZEPHYR_CURRENT_MODULE_DIR})
    set(SAFETY_IEC60730B_SRC_DIR ${SAFETY_IEC60730B_DIR}/source)
    set(SAFETY_IEC60730B_ZEPHYR_SRC_DIR ${ZEPHYR_CURRENT_MODULE_DIR}/zephyr/src)

    zephyr_library_named(safety_iec60730b)

    zephyr_include_directories(
        ${SAFETY_IEC60730B_SRC_DIR}
        ${SAFETY_IEC60730B_SRC_DIR}/common_test/aio
        ${SAFETY_IEC60730B_SRC_DIR}/common_test/clock
        ${SAFETY_IEC60730B_SRC_DIR}/common_test/dio
        ${SAFETY_IEC60730B_SRC_DIR}/common_test/flash
        ${SAFETY_IEC60730B_SRC_DIR}/common_test/tsi
        ${SAFETY_IEC60730B_SRC_DIR}/common_test/wdog
        ${SAFETY_IEC60730B_SRC_DIR}/compiler
        ${SAFETY_IEC60730B_ZEPHYR_SRC_DIR}
    )

    # Tests for Cortex-M0+ CPU
    if(CONFIG_CPU_CORTEX_M0PLUS)

        zephyr_include_directories(
            ${SAFETY_IEC60730B_SRC_DIR}/core_test/cm0
            ${SAFETY_IEC60730B_SRC_DIR}/core_test/cm0/flash
            ${SAFETY_IEC60730B_SRC_DIR}/core_test/cm0/programCounter
            ${SAFETY_IEC60730B_SRC_DIR}/core_test/cm0/ram
            ${SAFETY_IEC60730B_SRC_DIR}/core_test/cm0/register
            ${SAFETY_IEC60730B_SRC_DIR}/core_test/cm0/stack
        )

        zephyr_library_sources_ifdef(CONFIG_SAFETY_IEC60730B_TEST_CPU_REG ${SAFETY_IEC60730B_SRC_DIR}/core_test/cm0/register/iec60730b_cm0_reg.S)
        zephyr_library_sources(${SAFETY_IEC60730B_ZEPHYR_SRC_DIR}/safety_tests_cm0.c)

    # Tests for Cortex-M4/M7 CPU
    elseif(CONFIG_CPU_CORTEX_M4 OR CONFIG_CPU_CORTEX_M7)

        zephyr_include_directories(
            ${SAFETY_IEC60730B_SRC_DIR}/core_test/cm4_cm7
            ${SAFETY_IEC60730B_SRC_DIR}/core_test/cm4_cm7/flash
            ${SAFETY_IEC60730B_SRC_DIR}/core_test/cm4_cm7/programCounter
            ${SAFETY_IEC60730B_SRC_DIR}/core_test/cm4_cm7/ram
            ${SAFETY_IEC60730B_SRC_DIR}/core_test/cm4_cm7/register
            ${SAFETY_IEC60730B_SRC_DIR}/core_test/cm4_cm7/stack
        )

        zephyr_library_sources_ifdef(CONFIG_SAFETY_IEC60730B_TEST_CPU_REG ${SAFETY_IEC60730B_SRC_DIR}/core_test/cm4_cm7/register/iec60730b_cm4_cm7_reg.S)
        zephyr_library_sources_ifdef(CONFIG_SAFETY_IEC60730B_TEST_CPU_REG_FPU ${SAFETY_IEC60730B_SRC_DIR}/core_test/cm4_cm7/register/iec60730b_cm4_cm7_reg_fpu.S)
        zephyr_library_sources(${SAFETY_IEC60730B_ZEPHYR_SRC_DIR}/safety_tests_cm4_cm7.c)

    # Tests for Cortex-M33 CPU
    elseif(CONFIG_CPU_CORTEX_M33)

        zephyr_include_directories(
            ${SAFETY_IEC60730B_SRC_DIR}/core_test/cm33
            ${SAFETY_IEC60730B_SRC_DIR}/core_test/cm33/flash
            ${SAFETY_IEC60730B_SRC_DIR}/core_test/cm33/programCounter
            ${SAFETY_IEC60730B_SRC_DIR}/core_test/cm33/ram
            ${SAFETY_IEC60730B_SRC_DIR}/core_test/cm33/register
            ${SAFETY_IEC60730B_SRC_DIR}/core_test/cm33/stack
        )

        zephyr_library_sources_ifdef(CONFIG_SAFETY_IEC60730B_TEST_CPU_REG ${SAFETY_IEC60730B_SRC_DIR}/core_test/cm33/register/iec60730b_cm33_reg.S)
        zephyr_library_sources_ifdef(CONFIG_SAFETY_IEC60730B_TEST_CPU_REG_FPU ${SAFETY_IEC60730B_SRC_DIR}/core_test/cm33/register/iec60730b_cm33_reg_fpu.S)
        zephyr_library_sources_ifdef(CONFIG_SAFETY_IEC60730B_TEST_CPU_REG_DSP ${SAFETY_IEC60730B_SRC_DIR}/core_test/cm33/register/iec60730b_cm33_reg_dsp_ext.S)
        zephyr_library_sources(${SAFETY_IEC60730B_ZEPHYR_SRC_DIR}/safety_tests_cm33.c)

    else()
        message(FATAL_ERROR "Platform is not supported!")
    endif()

else() # CONFIG_SAFETY_IEC60730B=n
    message("Safety iec60730b library excluded from the build. ")
endif()
