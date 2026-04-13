# Copyright 2025 NXP
# SPDX-License-Identifier: Apache-2.0

if(CONFIG_IEC60730B)

    # Set path varriables
    set(IEC60730B_DIR ${ZEPHYR_CURRENT_MODULE_DIR})
    set(IEC60730B_ZEPHYR_SRC_DIR ${ZEPHYR_CURRENT_MODULE_DIR}/zephyr/src)
    set(IEC60730B_ZEPHYR_HAL_NXP_SRC_DIR ${IEC60730B_ZEPHYR_SRC_DIR}/hal/nxp)
    set(IEC60730B_ZEPHYR_HAL_ZEPHYR_SRC_DIR ${IEC60730B_ZEPHYR_SRC_DIR}/hal/zephyr)

    zephyr_library_named(safety_iec60730b)

    zephyr_include_directories(${IEC60730B_ZEPHYR_SRC_DIR})
    zephyr_library_sources(${IEC60730B_ZEPHYR_SRC_DIR}/iec60730b_test.c)

    # Include HAL-specific implementations
    add_subdirectory_ifdef(CONFIG_IEC60730B_HAL_NXP ${IEC60730B_ZEPHYR_HAL_NXP_SRC_DIR})
    add_subdirectory_ifdef(CONFIG_IEC60730B_HAL_ZEPHYR ${IEC60730B_ZEPHYR_HAL_ZEPHYR_SRC_DIR})

else() # CONFIG_IEC60730B=n
    message("Safety iec60730b library excluded from the build. ")
endif()
