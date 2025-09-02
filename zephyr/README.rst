.. _SafetyIec60730bREADME:
.. Safety iec60730b library module

#################
Safety iec60730b library for Zephyr
#################
* Based on and tested with Zephyr v4.2.0

.. contents::
   :local:
   :depth: 2


**************************
Safety iec60730b library environment setup
**************************

Follow these steps to set up a development environment on your machine.

#. If you haven't already, please follow `this guide <https://docs.zephyrproject.org/latest/develop/getting_started/index.html>`_ to set up a Zephyr development environment and its dependencies first.

#. Get the Safety iec60730b library. You can pick either of the options listed below.

    * Freestanding iec60730b module - pulls in only the dependencies it needs including Zephyr itself.

         Run::

            1. west init -m <iec60730b_repository_URL> --mr <iec60730b_revision_or_branch> <folder_name>
            2. cd <folder_name>
            3. west update

    * iec60730b as a Zephyr module - if you already have your Zephyr environment set up.

        To include iec60730b into Zephyr, update Zephyr's ``west.yml`` file::

            projects:
            - name: safety_iec60730b
              url: <iec60730b_repository_url>
              revision: <iec60730b_revision_or_branch>
              path: modules/safety/iec60730b

        Then run ``west update safety_iec60730b`` command.

*****************************
Build and run iec60730b example
*****************************

See Zephyr's `Building, Flashing and Debugging <https://docs.zephyrproject.org/latest/develop/west/build-flash-debug.html>`_ guide if you aren't familiar with it yet.

#. To **build** a project, run:

    ::

        west build -b <board> -<path to example> -p

    For example, this compiles the iec60730b example for a frdm-rw612 board::

        1. cd modules/safety/iec60730b/zephyr
        2. west build -b frdm_rw612 samples/safety_iec60730b -p

#. To **run** a project, run:

    ::

        west flash

****************
Folder structure
****************

::

    iec60730b/
    ├─── ...
    └─── zephyr/                All Zephyr related files.
        ├── samples/            Sample examples.
        ├── src/                Zephyr specific library source code.
        ├── module.yml          Defines module name, Cmake and Kconfig locations.
        ├── CMakeList.txt       Defines module's build process.
        ├── Kconfig             Defines module's configuration.
        └── ...
