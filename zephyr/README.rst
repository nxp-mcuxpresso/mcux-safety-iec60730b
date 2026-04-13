.. _SafetyIec60730bREADME:
.. Safety iec60730b module

###################################
Safety IEC60730B module for Zephyr
###################################

.. contents::
   :local:
   :depth: 2


******************************************
Safety IEC60730B module environment setup
******************************************

Follow these steps to set up a development environment on your machine.

#. If you haven't already, please follow `this guide <https://docs.zephyrproject.org/latest/develop/getting_started/index.html>`_ to set up a Zephyr development environment and its dependencies first.

#. Get the Safety IEC60730B module. You can pick either of the options listed below.

    * **Freestanding Zephyr iec60730b module** — pulls in only the dependencies it needs including Zephyr itself. Run::

            west init -m <iec60730b_repository_URL> --mr <iec60730b_revision_or_branch> <folder_name>
            cd <folder_name>
            west update

    * **Included Zephyr iec60730b module** — if you already have your Zephyr environment set up.

        * To include the Zephyr iec60730b module, add the following entry to a Zephyr submanifest
          (e.g. ``zephyr/submanifests/iec60730b.yaml``), or add it as a West project in your
          project's ``west.yaml`` manifest:

            .. code-block:: yaml

               manifest:
                 projects:
                   - name: safety_iec60730b
                     url: <iec60730b_repository_url>
                     revision: <iec60730b_revision_or_branch>
                     path: modules/safety/iec60730b

        * Now west recognizes ``safety_iec60730b`` as a module and will include its Kconfig and
          CMakeLists.txt in the build system.

        * Then update the module::

            west update safety_iec60730b

#. Use the IEC60730B module in your application.

    * Enable the IEC60730B module in your Zephyr project ``prj.conf`` file::

        CONFIG_IEC60730B=y

    * The user API is defined in ``modules/safety/iec60730b/zephyr/src/iec60730b_test.h``.
      Include this header in your application and call the provided functions.
      You can use the ``safety`` sample as a reference implementation.

    * The module provides two HAL implementations for the safety tests:

      - **NXP Vendor HAL** (``src/hal/nxp/``) — calls the certified NXP IEC60730B bare-metal
        library directly. This is the default for NXP SoCs.

      - **Zephyr HAL** (``src/hal/zephyr/``) — uses standard Zephyr device driver APIs
        and is more portable across vendors and boards. Enable with::

            CONFIG_IEC60730B_HAL_ZEPHYR=y

        .. warning::

           The Zephyr HAL is **EXPERIMENTAL**. This HAL does **NOT** use any
           vendor-certified libraries. None of the code has been certified against
           IEC 60730B or any other safety standard. Users are solely responsible
           for evaluating, validating, and certifying all code for their specific
           application and target platform before use in any safety-critical product.

*******************************
Build and run ``safety`` sample
*******************************

See Zephyr's `Building, Flashing and Debugging <https://docs.zephyrproject.org/latest/develop/west/build-flash-debug.html>`_
guide if you aren't familiar with it yet.

Supported boards
================

The following boards are supported out of the box with device-tree overlays:

* ``frdm_mcxa156``
* ``frdm_mcxa266``
* ``frdm_mcxa344``
* ``frdm_mcxc444``
* ``frdm_mcxn236``
* ``frdm_mcxn947/mcxn947/cpu0``
* ``frdm_imxrt1186/mimxrt1186/cm33``
* ``frdm_mcxw23``
* ``frdm_rw612``
* ``mimxrt1170_evk/mimxrt1176/cm7``

Building
========

#. To **build** the sample, run from the workspace root::

        west build -p -b <board> modules/safety/iec60730b/zephyr/samples/safety

   For example, to build for ``frdm_rw612``::

        west build -p -b frdm_rw612 modules/safety/iec60730b/zephyr/samples/safety

   .. note::
      All paths are relative to the workspace root (where ``west.yaml`` is located).

Flashing
========

#. To **flash** the built image::

        west flash

****************
Folder structure
****************
::

    iec60730b/
    ├─── ...
    └─── zephyr/                    All Zephyr related files.
        ├── samples/
        │   └── safety/             Safety sample application.
        │       ├── boards/         Board-specific device-tree overlays.
        │       ├── src/            Sample application source code.
        │       ├── CMakeLists.txt
        │       ├── Kconfig         Sample-specific Kconfig options.
        │       ├── prj.conf        Default project configuration.
        │       └── README.rst      Sample-specific documentation.
        ├── src/                    Zephyr specific library source code.
        │   └── hal/                HAL implementations (nxp/ and zephyr/).
        ├── module.yml              Defines module name, CMake and Kconfig locations.
        ├── CMakeLists.txt          Defines module's build process.
        ├── Kconfig                 Defines module's configuration.
        └── README.rst              This file.
