###################################
Safety IEC60730B sample for Zephyr
###################################

Demonstrates NXP Safety IEC 60730-B library functionality for safety-critical applications.

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
* Interrupt handling tests
* RAM tests (March algorithms)
* Flash/ROM tests (CRC/Checksum)
* Clock frequency tests
* Analog-to-Digital Converter (ADC) tests
* Digital I/O tests

These tests help ensure the microcontroller operates correctly and can detect hardware failures that 
could compromise safety in appliances and industrial control systems.

Requirements
************

Your board must:

* Be supported by the Safety IEC 60730-B library
* Support the required peripherals (ADC, timers, GPIO) for comprehensive testing

Building and Running
********************

Build and flash safety_iec60730b as follows, changing ``frdm_rw612`` for your board:

.. code-block:: console

   west build -b frdm_rw612 modules/safety/iec60730b/zephyr/samples/safety
   west flash

After flashing, the application will:

1. Execute startup safety tests (CPU, PC, Flash/ROM)
2. Begin periodic runtime safety tests in the safety thread.
3. Start an user application.
4. Indicate safety status through blinking LED indicator (if available).

Sample Output
*************

The console output will show the safety test execution status:

.. code-block:: console

   main: Set up button at gpio@0 pin 11
   main: Set up led at gpio@0 pin 12
   *** Booting Zephyr OS build v4.3.0-5931-g8100484e5d43 ***
   safety: Perform startup safety tests:
   safety: == Executing Start-up tests ==
   safety: PASS: CPU Registers test
   safety: PASS: FPU Registers test
   safety: PASS: RAM test
   safety: PASS: PC test
   safety: PASS: Flash test
   safety: PASS: Stack test initialization
   main: LED state: OFF
   safety: Safety test thread started
   safety: == Executing Run-time tests ==
   safety: PASS: CPU Registers test
   safety: PASS: FPU Registers test
   safety: PASS: RAM test
   safety: PASS: PC test
   safety: PASS: Stack test
   safety: PASS: Flash test
   safety: SKIP: DIO Input test
   safety: PASS: DIO Output test
   main: LED state: ON
   safety: == Executing Run-time tests ==
   safety: PASS: CPU Registers test
   safety: PASS: FPU Registers test
   safety: PASS: RAM test
   safety: PASS: PC test
   safety: PASS: Stack test
   safety: PASS: Flash test
   safety: SKIP: DIO Input test
   safety: PASS: DIO Output test
   main: LED state: OFF
   safety: == Executing Run-time tests ==
   safety: PASS: CPU Registers test
   safety: PASS: FPU Registers test
   safety: PASS: RAM test
   safety: PASS: PC test
   safety: PASS: Stack test
   safety: PASS: Flash test
   safety: SKIP: DIO Input test
   safety: PASS: DIO Output test
   main: LED state: ON

Safety Considerations
*********************

When using this sample in safety-critical applications:

* Configure appropriate test intervals based on your safety requirements
* Implement proper error handling and safe state transitions
* Consider certification requirements for your specific application domain
* Validate the complete system including this safety library implementation

For production use, consult the NXP Safety IEC 60730-B library documentation and
consider engaging with safety certification experts.

References
**********

* `IEC 60730-1 Standard <https://webstore.iec.ch/publication/3154>`_
* `NXP Safety IEC 60730-B Library Documentation`
* `Zephyr Safety Documentation <https://docs.zephyrproject.org/latest/safety/>`_
