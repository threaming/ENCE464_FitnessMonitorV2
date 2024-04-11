# ENCE 464 TivaWare + FreeRTOS + GCC + OpenOCD Makefile Example

This project is an example of how the TM4C microcontrollers can be programmed
using GCC and OpenOCD as the primary toolchain. This includes the TivaWare and
FreeRTOS libraries also.

This repository has been adapted fron Ben Mitchell's
[original](https://eng-git.canterbury.ac.nz/bmi32/464_freertos_makefile).

See [the FAQ](./FAQ.md) for answers to some common questions and tips for
debugging common errors.

[[_TOC_]]

## Directory structure

* `fitness-tracker/`: Contains source code for the project.
* `test-apps/`: Programs for the device; contains a number of demo programs.
* `freertos`: A git submodule that links to the FreeRTOS kernel.
* `FreeRTOSConfig.h`: FreeRTOS config header (see FreeRTOS docs)
* `SW-TM4C-2.2.0.295/`: Header files, source files, and precompiled static
  libraries for the TivaWare SDK, which provides a hardware abstraction library
  for the Tiva's peripherals (e.g., PWM, GPIO).
* `scripts/`: Linker script and GDB configs
* `startup.c`: MCU initialisation code; contains fault handler functions which
  can be customised for custom error handling (e.g., toggle a GPIO).
* `tiva-freertos.mk`: Makefile fragment for building and programming using GCC
  and OpenOCD with TivaWare and FreeRTOS. Include in your top-level Makefile.
  (See Makefiles in demo programs in `test-apps` directory.)

## Note on path names

**IMPORTANT: file paths cannot have spaces in them.**

All sorts of weird issues occur using make and the VS Code build tools when your
project paths contain spaces: this includes spaces in the filenames and in all
the higher-level directories. Although it may be possible to get make and VS
Code to play nicely with paths containing spaces, it's much easier to just
remove spaces from paths either by replacing them with hyphens or underscores or
moving the project directory so that its ancestor directories do not contain
spaces.

## Requirements

This project was originally developed for use in a Linux environment.

### Linux

On Ubuntu/Debian systems, use your system's package manager:

```
sudo apt update
sudo apt upgrade
sudo apt install gcc-arm-none-eabi gdb-multiarch libnewlib-arm-none-eabi openocd
```

### macOS

Using [Homebrew](https://brew.sh/):

    brew install openocd git
    brew cask install gcc-arm-embedded

### Windows

The following packages are expected to be on PATH:

* `make`
* `arm-none-eabi-gcc`
* `arm-none-eabi-gdb`
* `openocd`

Note: if your system provides `gdb-multiarch` instead, you will need to edit
`tiva-freertos.mk` to account for this.

On the Windows machines in the Embedded Systems Laboratory run the following
command in a Command Prompt window to add the toolchain to your path:

    C:\ence461\ence461-path.bat

This will have to be run for every new CMD session you open.

To install the same toolchain on your personal Windows machine, copy
`C:\ence461` to your machine's `C:` drive. (Or some other location - just
remember to change the path of the above command.)

## Basic usage

### Makefiles

Each program you develop (e.g., vehicle simulator and ABS controller) should
include `tiva-freertos.mk` in its Makefile. Before including this file, you need
to set the `PROJECT_DIR` variable to the path of the top level directory (i.e.
this directory which contains `tiva-freertos.mk`) relative to the Makefile.

The `apps/blinky` program provides an example of a simple single-file
application:

```makefile
PROJECT_DIR = ../..

SRC = blinky.c

include $(PROJECT_DIR)/tiva-freertos.mk
```

In order to follow software engineering best practices in this project, you
should minimise code repetition between multiple apps. For example, you might
write a common PWM duty cycle reader for multiple programs that has this
directory structure:

    libs/
    ├── include
    │   └── pwm.h
    ├── libs.mk
    └── pwm.c

Then in `libs.mk`:

```makefile
VPATH += $(PROJECT_DIR)/libs
SRC += pwm.c
INCLUDES += -I"$(PROJECT_DIR)/libs/include"
```

(The [VPATH
variable](https://www.gnu.org/software/make/manual/html_node/General-Search.html)
tells make where to look for source files: `tiva-freertos.mk` already adds the
directory of the main Makefile that includes it as well as the source
directories for FreeRTOS and TivaWare.) Then add to your program's Makefile:

```makefile
include $(PROJECT_DIR)/libs/libs.mk
```

**Before** the `include $(PROJECT_DIR)/tiva-freertos.mk` line.

**This is just one way to do it**: design your project structure however you see
fit. You are more than welcome to change the structure of this entire repository
including the `tiva-freertos.mk` to suit your project; just make sure you update
this README to explain your structure.

### FreeRTOS configuration

The inclusion of FreeRTOS in your project is determined via the `USE_FREERTOS`
Makefile variable; by default this is set to `1` (true) in tiva-freertos.mk.
However, each program's makefile can override this.

The FreeRTOS architecture is set via the `FREERTOS_ARCH` Makefile variable; by
default this is set to `GCC/ARM_CM4F`.

The FreeRTOS heap implementation is set via the `FREERTOS_HEAP_IMPL` Makefile
variable; by default this is set to `heap_4`, which should be fine for this
project.

All other FreeRTOS configuration, including heap size and kernel feature flags,
is done in the `FreeRTOSConfig.h` header file.

See the FreeRTOS docs for information on the config header and the different
heap implementations.

### Command line

Navigate to any of the apps provided (where a Makefile is present) and the
following targets are available:

* `make` (or `make all`): Build the source code into a binary; build outputs are
  put in the `build/` directory by default (controlled by `BUILD_DIR` variable),
  the default output binary is located at `build/app.elf` (controlled by the
  `TARGET` variable).
* `make clean`: Delete build outputs (everything listed in the `CLEAN`
  variable).
* `make openocd`: Launch OpenOCD and connect to the microcontroller.
* `make program`: Build the program and then load it onto the microcontroller;
  OpenOCD must be running with device connected.
* `make debug`: Build the program, launch GDB and connect to the
  microcontroller; OpenOCD must be running with device connected.
* `make preprocessed`: Generate preprocessed versions of each file in `SRC` to
  `build/preprocessed` directory by default (controlled by `PREPROCESSED_DIR`
  variable); this is sometimes useful for debugging the preprocessor.

To view the complete compiler commands as they are run, run make with the
`VERBOSE=1` environment variable set.

To speed up compilation time, you could try running make with the `-j` flag,
which runs multiple jobs simultaneously (you can optionally specify the maximum
number of jobs after the flag). This can speed up compilation especially on a
multicore machine. See make's man page for more information. (This flag is
enabled by default for the VS Code build commands: see
[.vscode/tasks.json](./.vscode/tasks.json).)

Note that the `program` and `debug` commands require that OpenOCD is running in
the background. This is done by running another terminal in the background with
`make openocd` running. (On Windows this means opening another CMD window and
running `C:\ence461\ence461-path.bat` before running `make openocd`.)

### Visual Studio Code

The `.vscode` directory contains VS Code config files for running the above make
targets using VS Code's tasks interface. When you first open this project in VS
Code, install the recommended extensions (`Ctrl+Shift+P` > `show recommended
extensions`).

Note that VS Code support in this project is somewhat experimental, particularly
the debugger. I welcome any fixes!

Press `Ctrl+Shift+B` to open the tasks palette; then select or type the task to
run and press `Enter`.

* **Build**: build the source code; equivalent to running `make` in the
  directory of the active file.
* **Clean**: clean build files; equivalent to running `make clean` in the
  directory of the active file.
* **Program**: build the program and load it onto the MCU; equivalent to running
  `make program` in the directory of the active file. Requires OpenOCD to be
  running (either in a command line or via the *Launch OpenOCD* task).
* **Debug**: build the program, load it to the MCU, and open GDB in the command
  line; equivalent to running `make debug` in the directory of the active file.
  Requires OpenOCD to be running.
* **Launch OpenOCD**: launches OpenOCD; equivalent to running `make openocd` in
  the directory of the active file.

On Windows these commands will automatically run the path setup batch file
(`C:\ence461\ence461-path.bat`) so you don't need to manually run it yourself.
See the *Windows-specific information* header below.

#### Debugging

This repository supports debugging using VS Code's debug interface for visually
stepping through the code in the text editor window. To start debugging, ensure
that OpenOCD is running (via `make openocd` or the *Launch OpenOCD* VS Code
task), set your breakpoints, and go to `Run > Start Debugging` or press `F5`.
The debugger should automatically break at the beginning of your `main`
function.

The VS Code debugging behaviour is controlled in the `.vscode/launch.json` file.

#### Windows-specific information

On Windows, you have to set VS Code's default shell to Command Prompt (not
PowerShell). Open the command palette (`Ctrl+Shift+P`) and type `terminal
default`, navigate to `Terminal: Select Default Profile` (on older versions of
VS Code this may be `Terminal: Select Default Shell`), and press `Enter`, then
set `Command Prompt` as the default shell.

On Windows, the path to the `ence-path.bat` command needs to be provided in
`.vscode/settings.json`:

```json
{
    "windowsToolchainEnvCmd": "C:\\path\\to\\ence461-path.bat"
}
```

This is currently set to `"C:\\ence461\\ence461-path.bat"`, which is the correct
path for the ESL machines. If you use a different path on your personal machine,
you will need to change this. (Note that in JSON files backslashes need to be
escaped, so `\\` corresponds to a single backslash.)

For the debugger, you will also need to change the path to your debugger using
the `miDebuggerPath` option in `.vscode/launch.json` if it is in a different
location:

```json
"windows": {
    "miDebuggerPath": "C:\\path\\to\\arm-none-eabi-gdb.exe"
}
```

(Currently this is set to the correct path for the toolchain on the ESL
machines.)

## Coding style

You will note that there are a variety of coding styles present in this project.
TivaWare uses camelCase, FreeRTOS uses a prefixed camelCase, and the original
code authors use camelCase. I suggest picking whichever you prefer and sticking
with it. camelCase would be a good choice here as it is similar to the existing 
code.

According to
[FreeRTOS](https://www.freertos.org/FreeRTOS-Coding-Standard-and-Style-Guide.html),
the convention used is to prefix variables and functions with their types (or
return types). This is a form of [Hungarian
notation](https://en.wikipedia.org/wiki/Hungarian_notation) which I personally
feel is uneccesary given that our development environments give us this type
information by simply mousing over the variable in question!
