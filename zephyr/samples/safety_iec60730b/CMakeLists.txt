# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.20.0)
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(safety_blinky)

target_include_directories(app PRIVATE src/)
target_sources(app PRIVATE
    src/main.c
    src/safety_tests.c
)

# Add tests for Cortex-M0+ CPU
if(CONFIG_CPU_CORTEX_M0PLUS)
    target_sources(app PRIVATE
        src/safety_tests_cm0.c
    )
# Add tests for Cortex-M4/M7 CPU
elseif(CONFIG_CPU_CORTEX_M4 OR CONFIG_CPU_CORTEX_M7)
    target_sources(app PRIVATE
        src/safety_tests_cm4_cm7.c
    )
# Add Cortex-M33 specific safety tests
elseif(CONFIG_CPU_CORTEX_M33)
    target_sources(app PRIVATE
        src/safety_tests_cm33.c
    )
endif()

