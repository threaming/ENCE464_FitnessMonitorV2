# ENCE464 Fitness Tracker Project

This repository contains the code and build system for the ENCE464 fitness
tracker project, to run on the
[Texas Instruments TM4C123G LaunchPad Evaluation Kit](https://www.ti.com/tool/EK-TM4C123GXL).
The repository is structured as follows.

* `docs`: Instructions for building and running the project code, installing the
  development toolchain on your own computer, and additional project help.
* `labs`: Supporting code for your labs.
* `target`: Code that targets the LaunchPad; contains the main project code,
  libraries, and a few simple demo programs.
* `tests`: Tests targeting the host (development) machine.
* `.vscode`: Visual Studio Code (VS Code) configuration files.

## 🚀 Getting started

If you want to build the project on your personal machine, firstly read the
toolchain installation guide for your operating system:

* [Windows](docs/toolchain.md#windows)
* [Linux](docs/toolchain.md#linux)
* [macOS](docs/toolchain.md#macos)

Otherwise, the computers in the ESL have the toolchain pre-installed.

This projects is set-up to use the CMake build system. You can use the command
line or VS Code to build the code and program your device. See the below links
for instructions.

* [VS Code](docs/vs-code.md)
* [Command line](docs/cli.md)

See the [CMake guide](docs/cmake.md) for info on performing certain tasks with
the CMake build system.

For tips on debugging and fixing common issues, including some common FreeRTOS
errors, see the [FAQ](docs/faq.md).

## 🌐 Additional resources

* [Tiva TM4C123GH6PM datasheet (PDF)](https://www.ti.com/lit/ds/symlink/tm4c123gh6pm.pdf)
* [Tiva LaunchPad manual (PDF)](https://www.ti.com/lit/ug/spmu296/spmu296.pdf)
* [Orbit BoosterPack manual (PDF)](https://digilent.com/reference/_media/orbit_boosterpack/orbit_boosterpack_rm.pdf)
* [TivaWare API documentation (PDF)](https://www.ti.com/lit/ug/spmu298e/spmu298e.pdf)
* [FreeRTOS Documentation](https://www.freertos.org/Documentation/RTOS_book.html)
* [Unity test framework documentation](https://github.com/ThrowTheSwitch/Unity/blob/master/docs/UnityGettingStartedGuide.md)
    - [Assertions cheatsheet (PDF)](https://github.com/ThrowTheSwitch/Unity/blob/master/docs/UnityAssertionsCheatSheetSuitableforPrintingandPossiblyFraming.pdf)
* [CMake documentation](https://cmake.org/cmake/help/latest/)
