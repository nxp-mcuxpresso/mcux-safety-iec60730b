.. _SafetyIec60730bSampleREADME:

###################################
Safety IEC60730B sample for Zephyr
###################################

Demonstrates the Safety IEC 60730-B module functionality for safety-critical applications.

.. contents::
   :local:
   :depth: 2


Overview
********

This sample demonstrates the use of the Safety IEC 60730-B library within the Zephyr RTOS environment.
The IEC 60730-B standard defines safety requirements for automatic electrical controls used in household
appliances and similar equipment.

The sample implements various safety tests including:

* CPU register tests
* Program Counter (PC) tests
* RAM tests (March algorithms)
* Flash/ROM tests (CRC/Checksum)
* Clock frequency tests
* Analog-to-Digital Converter (ADC) tests
* Digital I/O tests

These tests help ensure the microcontroller operates correctly and can detect hardware failures that
could compromise safety in appliances and industrial control systems.

Requirements
************

The following boards are supported out of the box with device-tree overlays:

* ``frdm_mcxa156``
* ``frdm_mcxa266``
* ``frdm_mcxa344``
* ``frdm_mcxc444``
* ``frdm_mcxn236``
* ``frdm_mcxn947_mcxn947_cpu0``
* ``frdm_imxrt1186/mimxrt1186/cm33``
* ``frdm_mcxw23``
* ``frdm_rw612``
* ``mimxrt1170_evk/mimxrt1176/cm7``

Building and Running
********************

Build and flash safety_iec60730b as follows, changing ``frdm_rw612`` for your board:

.. code-block:: console

   west build -p -b frdm_rw612 modules/safety/iec60730b/zephyr/samples/safety
   west flash

After flashing, the application will:

1. Execute startup safety tests (CPU, PC, Flash/ROM)
2. Begin periodic runtime safety tests in the safety thread.
3. Start the user application.
4. Indicate safety status through blinking LED indicator (if available).

Sample Output
*************

The console output will show the safety test execution status:

.. code-block:: console

   main: Set up button at gpio@0 pin 11
   main: Set up led at gpio@0 pin 12
   *** Booting Zephyr OS build v4.4.0-1693-g0d65cac7acf7 ***
   safety: Perform startup safety tests:
   safety: == Executing Start-up tests ==
   safety: - PASS - CPU Registers test
   safety: - PASS - FPU Registers test
   safety: - PASS - RAM test
   safety: - PASS - PC test
   safety: - PASS - Flash CRC16 test
   safety: - PASS - Flash CRC32 test
   main: Set up button at gpio@0 pin 11
   main: Set up led at gpio@0 pin 12
   *** Booting Zephyr OS build v4.4.0-1693-g0d65cac7acf7 ***
   safety: Perform startup safety tests:
   safety: == Executing Start-up tests ==
   safety: - PASS - CPU Registers test
   safety: - PASS - FPU Registers test
   safety: - PASS - RAM test
   safety: - PASS - PC test
   safety: - PASS - Flash CRC16 test
   safety: - PASS - Flash CRC32 test
   safety: - PASS - Watchdog test
   safety: - PASS - Stack test initialization
   safety: - PASS - Clock test initialization
   safety: Task WDT channel 0 added with timeout 10000 ms
   main: LED state: OFF
   safety: Safety test thread started
   safety: == Executing Run-time tests ==
   safety: - PASS - CPU Registers test
   safety: - PASS - FPU Registers test
   safety: - PASS - RAM test
   safety: - PASS - PC test
   safety: - PASS - Stack test
   safety: - PASS - Flash CRC16 test
   safety: - PASS - Flash CRC32 test
   safety: - SKIP - DIO Input test
   safety: - PASS - DIO Output test
   safety: - PASS - Clock test
   main: LED state: ON

Debugging
*********

When debugging the application or setting breakpoints, the watchdog and clock tests may fail
because the processor is halted by the debugger. To prevent test failures during debugging,
disable these time-sensitive tests in your ``prj.conf`` or board configuration::

   CONFIG_IEC60730B_TEST_CLOCK=n
   CONFIG_IEC60730B_TEST_WDOG=n

Safety Considerations
*********************

When using this sample in safety-critical applications:

* Configure appropriate test intervals based on your safety requirements.
* Implement proper error handling and safe state transitions.
* Consider certification requirements for your specific application domain.
* Validate the complete system including this safety library implementation.

For production use, consult the NXP Safety IEC 60730-B library documentation and
consider engaging with safety certification experts.

References
**********

* `IEC 60730-1 Standard <https://webstore.iec.ch/publication/66089>`_
* `NXP Safety IEC 60730-B Library Documentation`
* `Zephyr Safety Documentation <https://docs.zephyrproject.org/latest/safety/>`_
