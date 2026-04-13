# Copyright 2026 NXP
# SPDX-License-Identifier: Apache-2.0

if(CONFIG_IEC60730B_HAL_ZEPHYR)

    message(WARNING "
        WARNING: IEC60730B Zephyr HAL is EXPERIMENTAL.
        This HAL does NOT use any vendor-certified libraries.
        None of the code has been certified against IEC 60730B or any other safety standard.
        Users are solely responsible for evaluating, validating, and certifying all code
        for their specific application and target platform before use in any safety-critical product.")

    zephyr_include_directories(${CMAKE_CURRENT_SOURCE_DIR})

    # Add HAL Zephyr source files
    zephyr_library_sources_ifdef(CONFIG_IEC60730B_TEST_CPU iec60730b_test_cpu.c)
    zephyr_library_sources_ifdef(CONFIG_IEC60730B_TEST_FPU iec60730b_test_fpu.c)
    zephyr_library_sources_ifdef(CONFIG_IEC60730B_TEST_PC iec60730b_test_pc.c)
    zephyr_library_sources_ifdef(CONFIG_IEC60730B_TEST_FLASH iec60730b_test_flash.c)
    zephyr_library_sources_ifdef(CONFIG_IEC60730B_TEST_STACK iec60730b_test_stack.c)
    zephyr_library_sources_ifdef(CONFIG_IEC60730B_TEST_RAM iec60730b_test_ram.c)
    zephyr_library_sources_ifdef(CONFIG_IEC60730B_TEST_DIO iec60730b_test_dio.c)
    zephyr_library_sources_ifdef(CONFIG_IEC60730B_TEST_WDOG iec60730b_test_wdog.c)
    zephyr_library_sources_ifdef(CONFIG_IEC60730B_TEST_CLOCK iec60730b_test_clock.c)

    # On boards where zephyr,sram is external volatile memory (SDRAM, HyperRAM)
    # the memory controller is re-initialised on every reset, destroying
    # all RAM contents — including __noinit variables. This defeats the purpose
    # of iec60730b_test_wdog_counter_ticks which must survive a watchdog reset.
    #
    # CONFIG_IEC60730B_TEST_WDOG_DTCM_NOINIT (set in the per-board .conf) relocates
    # iec60730b_test_wdog.c noinit data to DTCM_NOINIT (on-chip Data TCM),
    # which retains its contents across watchdog resets.
    # See CONFIG_IEC60730B_TEST_WDOG_DTCM_NOINIT in src/hal/zephyr/Kconfig for details.
    if(CONFIG_IEC60730B_TEST_WDOG_DTCM_NOINIT)
        zephyr_code_relocate(
            FILES ${CMAKE_CURRENT_SOURCE_DIR}/iec60730b_test_wdog.c
            LOCATION DTCM_NOINIT)
    endif()

endif()
