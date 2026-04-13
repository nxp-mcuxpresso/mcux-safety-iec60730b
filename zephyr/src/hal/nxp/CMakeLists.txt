# Copyright 2026 NXP
# SPDX-License-Identifier: Apache-2.0

if(CONFIG_IEC60730B_HAL_NXP)

    set(IEC60730B_SRC_DIR ${IEC60730B_DIR}/source)

    # Tests for Cortex-M0+ CPU
    if(CONFIG_CPU_CORTEX_M0PLUS)
        zephyr_include_directories(
            ${IEC60730B_SRC_DIR}/core_test/cm0
            ${IEC60730B_SRC_DIR}/core_test/cm0/flash
            ${IEC60730B_SRC_DIR}/core_test/cm0/programCounter
            ${IEC60730B_SRC_DIR}/core_test/cm0/ram
            ${IEC60730B_SRC_DIR}/core_test/cm0/register
            ${IEC60730B_SRC_DIR}/core_test/cm0/stack
        )
        zephyr_library_sources_ifdef(CONFIG_IEC60730B_TEST_CPU ${IEC60730B_SRC_DIR}/core_test/cm0/register/iec60730b_cm0_reg.S)
        zephyr_library_sources_ifdef(CONFIG_IEC60730B_TEST_RAM ${IEC60730B_SRC_DIR}/core_test/cm0/ram/iec60730b_cm0_ram.S)
        zephyr_library_sources_ifdef(CONFIG_IEC60730B_TEST_PC ${IEC60730B_SRC_DIR}/core_test/cm0/programCounter/iec60730b_cm0_pc.S
                                                            ${IEC60730B_SRC_DIR}/core_test/cm0/programCounter/iec60730b_cm0_pc_object.S)
        zephyr_library_sources_ifdef(CONFIG_IEC60730B_TEST_FLASH ${IEC60730B_SRC_DIR}/core_test/cm0/flash/iec60730b_cm0_flash.S)
        zephyr_library_sources_ifdef(CONFIG_IEC60730B_TEST_STACK ${IEC60730B_SRC_DIR}/core_test/cm0/stack/iec60730b_cm0_stack.S)
    # Tests for Cortex-M4/M7 CPU
    elseif(CONFIG_CPU_CORTEX_M4 OR CONFIG_CPU_CORTEX_M7)
        zephyr_include_directories(
            ${IEC60730B_SRC_DIR}/core_test/cm4_cm7
            ${IEC60730B_SRC_DIR}/core_test/cm4_cm7/flash
            ${IEC60730B_SRC_DIR}/core_test/cm4_cm7/programCounter
            ${IEC60730B_SRC_DIR}/core_test/cm4_cm7/ram
            ${IEC60730B_SRC_DIR}/core_test/cm4_cm7/register
            ${IEC60730B_SRC_DIR}/core_test/cm4_cm7/stack
        )
        zephyr_library_sources_ifdef(CONFIG_IEC60730B_TEST_CPU ${IEC60730B_SRC_DIR}/core_test/cm4_cm7/register/iec60730b_cm4_cm7_reg.S)
        zephyr_library_sources_ifdef(CONFIG_IEC60730B_TEST_FPU ${IEC60730B_SRC_DIR}/core_test/cm4_cm7/register/iec60730b_cm4_cm7_reg_fpu.S)
        zephyr_library_sources_ifdef(CONFIG_IEC60730B_TEST_RAM ${IEC60730B_SRC_DIR}/core_test/cm4_cm7/ram/iec60730b_cm4_cm7_ram.S)
        zephyr_library_sources_ifdef(CONFIG_IEC60730B_TEST_PC ${IEC60730B_SRC_DIR}/core_test/cm4_cm7/programCounter/iec60730b_cm4_cm7_pc.S
                                                            ${IEC60730B_SRC_DIR}/core_test/cm4_cm7/programCounter/iec60730b_cm4_cm7_pc_object.S)
        zephyr_library_sources_ifdef(CONFIG_IEC60730B_TEST_FLASH ${IEC60730B_SRC_DIR}/core_test/cm4_cm7/flash/iec60730b_cm4_cm7_flash.S)
        zephyr_library_sources_ifdef(CONFIG_IEC60730B_TEST_STACK ${IEC60730B_SRC_DIR}/core_test/cm4_cm7/stack/iec60730b_cm4_cm7_stack.S)
    # Tests for Cortex-M33 CPU
    elseif(CONFIG_CPU_CORTEX_M33)
        zephyr_include_directories(
            ${IEC60730B_SRC_DIR}/core_test/cm33
            ${IEC60730B_SRC_DIR}/core_test/cm33/flash
            ${IEC60730B_SRC_DIR}/core_test/cm33/programCounter
            ${IEC60730B_SRC_DIR}/core_test/cm33/ram
            ${IEC60730B_SRC_DIR}/core_test/cm33/register
            ${IEC60730B_SRC_DIR}/core_test/cm33/stack
        )
        zephyr_library_sources_ifdef(CONFIG_IEC60730B_TEST_CPU ${IEC60730B_SRC_DIR}/core_test/cm33/register/iec60730b_cm33_reg.S)
        zephyr_library_sources_ifdef(CONFIG_IEC60730B_TEST_FPU ${IEC60730B_SRC_DIR}/core_test/cm33/register/iec60730b_cm33_reg_fpu.S)
        zephyr_library_sources_ifdef(CONFIG_ARMV8_M_DSP ${IEC60730B_SRC_DIR}/core_test/cm33/register/iec60730b_cm33_reg_dsp_ext.S)
        zephyr_library_sources_ifdef(CONFIG_IEC60730B_TEST_RAM ${IEC60730B_SRC_DIR}/core_test/cm33/ram/iec60730b_cm33_ram.S)
        zephyr_library_sources_ifdef(CONFIG_IEC60730B_TEST_PC ${IEC60730B_SRC_DIR}/core_test/cm33/programCounter/iec60730b_cm33_pc.S
                                                            ${IEC60730B_SRC_DIR}/core_test/cm33/programCounter/iec60730b_cm33_pc_object.S)
        zephyr_library_sources_ifdef(CONFIG_IEC60730B_TEST_FLASH ${IEC60730B_SRC_DIR}/core_test/cm33/flash/iec60730b_cm33_flash.S)
        zephyr_library_sources_ifdef(CONFIG_IEC60730B_TEST_STACK ${IEC60730B_SRC_DIR}/core_test/cm33/stack/iec60730b_cm33_stack.S)
    else()
        message(FATAL_ERROR "Platform is not supported!")
    endif()

    # Common tests
    zephyr_include_directories(
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${IEC60730B_SRC_DIR}
        ${IEC60730B_SRC_DIR}/common_test/aio
        ${IEC60730B_SRC_DIR}/common_test/clock
        ${IEC60730B_SRC_DIR}/common_test/dio
        ${IEC60730B_SRC_DIR}/common_test/flash
        ${IEC60730B_SRC_DIR}/common_test/tsi
        ${IEC60730B_SRC_DIR}/common_test/wdog
        ${IEC60730B_SRC_DIR}/compiler
        ${IEC60730B_ZEPHYR_SRC_DIR}
    )

    zephyr_library_sources_ifdef(CONFIG_IEC60730B_TEST_CPU iec60730b_test_cpu.c)
    zephyr_library_sources_ifdef(CONFIG_IEC60730B_TEST_FPU iec60730b_test_fpu.c)
    if(CONFIG_IEC60730B_TEST_AIO)
        zephyr_library_sources(${IEC60730B_SRC_DIR}/common_test/aio/iec60730b_aio.c)
        zephyr_library_sources(iec60730b_test_aio.c)
        zephyr_library_compile_options(-Wno-parentheses) # disable warning
    endif()
    if(CONFIG_IEC60730B_TEST_FLASH)
        zephyr_library_sources(iec60730b_test_flash.c)
        zephyr_library_sources(${IEC60730B_SRC_DIR}/common_test/flash/iec60730b_invariable_memory.c)
        zephyr_library_compile_options(-Wno-maybe-uninitialized) # disable warning
    endif()
    zephyr_library_sources_ifdef(CONFIG_IEC60730B_TEST_STACK iec60730b_test_stack.c)
    zephyr_library_sources_ifdef(CONFIG_IEC60730B_TEST_PC iec60730b_test_pc.c)
    zephyr_library_sources_ifdef(CONFIG_IEC60730B_TEST_RAM iec60730b_test_ram.c)
    zephyr_library_sources_ifdef(CONFIG_IEC60730B_TEST_CLOCK iec60730b_test_clock.c
                                                            ${IEC60730B_SRC_DIR}/common_test/clock/iec60730b_clock.c)
    zephyr_library_sources_ifdef(CONFIG_IEC60730B_TEST_DIO iec60730b_test_dio.c
                                                        ${IEC60730B_SRC_DIR}/common_test/dio/iec60730b_dio.c
                                                        ${IEC60730B_SRC_DIR}/common_test/dio/iec60730b_dio_ext.c )
    zephyr_library_sources_ifdef(CONFIG_IEC60730B_TEST_TSI ${IEC60730B_SRC_DIR}/common_test/tsi/iec60730b_tsi.c)
    zephyr_library_sources_ifdef(CONFIG_IEC60730B_TEST_WDOG iec60730b_test_wdog.c
                                                            ${IEC60730B_SRC_DIR}/common_test/wdog/iec60730b_wdog.c)

    # On boards where zephyr,sram is external volatile memory (SDRAM, HyperRAM)
    # the memory controller is re-initialised on every reset, destroying
    # all RAM contents — including __noinit variables. This defeats the purpose
    # of iec60730b_test_wdog_counter_ticks which must survive a watchdog reset.
    #
    # CONFIG_IEC60730B_TEST_WDOG_DTCM_NOINIT (set in the per-board .conf) relocates
    # iec60730b_test_wdog.c noinit data to DTCM_NOINIT (on-chip Data TCM),
    # which retains its contents across watchdog resets.
    # See CONFIG_IEC60730B_TEST_WDOG_DTCM_NOINIT in src/hal/nxp/Kconfig for details.
    if(CONFIG_IEC60730B_TEST_WDOG_DTCM_NOINIT)
        zephyr_code_relocate(
            FILES ${CMAKE_CURRENT_SOURCE_DIR}/iec60730b_test_wdog.c
            LOCATION DTCM_NOINIT)
    endif()

endif() # CONFIG_IEC60730B_HAL_NXP
