# Command line instructions

[[_TOC_]]

## ⚙️ Configure CMake

After cloning this repository, you need to run the CMake configure command to
generate the build system:

```sh
# Run this once to configure the project:
cd target
cmake --toolchain cmake/tm4c.cmake -B build -G "Unix Makefiles" .
```

This will create a `build` directory in the `target` directory that contains
Makefiles for building the project.

**Note**: You should only have to run this command once after cloning the
repository or if you delete the `build` directory. Subsequent build commands
will check for any changes to cmake files in the project and automatically
reconfigure.

## 🔧 🚀 Build and upload a demo program

To start off, we will build and upload the `blinky` program, which just flashes
the LED on the LaunchPad using FreeRTOS.

```sh
# From the target directory:
cmake --build build --target blinky

# ...or, inside `build` directory:
make blinky
```

Then, to upload the program to the microcontroller, with the LaunchPad connected
to your machine via a Micro USB cable:

```sh
# From the target directory:
cmake --build build --target program-blinky

# ...or, inside `build` directory:
make program-blinky
```

(This command will also re-build the `blinky` program if any of the source code
has changed, so you actually don't have to run the `blinky` command separately
if you also want to upload it to the device.)

The other demo programs can be programmed using the `program-[demo name]`
targets, run `cmake -B build` to see a list of all programming and debug
targets.

## 🐞 Debugging via the command line

(You may find it easier to use VS Code for a GUI debugger - see the 'Debugging'
section in the [VS Code guide](vs-code.md#-debugging).)

To program the microcontroller and attach a command-line GDB session to the
program, run the `debug-[demo name]` targets for the demo programs or the
`debug` target for the main fitness tracker program.

```sh
# E.g. to debug the fitness tracker program
cmake --build build --target debug

# ...or, inside the `build` directory:
make debug
```

To pause the debugger while the program is running, press `Ctrl+C`. Some useful
GDB commands are:

* `break [filename]:[line-number]`/`b ...`: Set a breakpoint at a specific line
  in a file.
* `break [function]`/`b ...`: Set a breakpoint at a specific function.
* `info breakpoints`/`i b`: Show a list of all breakpoints.
* `delete [breakpoint-number]`/`d ...`: Delete a breakpoint using the number
  listed in the `i b` command.
* `continue`/`c`: Keep running the program until the next breakpoint.
* `next`/`n`: Run the next line of code within the same function (equivalent to
  the **Step Over** command in VS Code.)
* `step`/`s`: Run the next line of code, stepping into any function calls
  (equivalent to the **Step Into** command in VS Code.)
* `finish`/`f`: Run until the current function returns to the calling scope
  (equivalent to the **Step Out** command in VS Code.)
* `print [expr]`/`p ...`: Print the value of a variable or expression.
* `bt`: Print the program backtrace (a list of all the active function calls).
* `exit` (or press `Ctrl+D`) followed by `yes`: Quit the debugger.

Pressing `Enter` without a command will repeat the last command; this is useful
for stepping through multiple statements in quick succession (e.g. run `next`
first, then just `Enter` thereafter).

Another useful feature is the 'terminal user interface' (TUI), which shows the
source code in a split window in the terminal. Run `tui enable` in the debugger
console to show the TUI.

See this useful [GDB cheatsheet](https://darkdust.net/files/GDB%20Cheat%20Sheet.pdf)
for more commands.

## 🧪 Building and running tests on host machine

Since the unit tests will be run on the host (development) machine rather than
the microcontroller and thus require a different compiler toolchain, CMake
requires that we use a separate `build` directory.

```sh
# Run this once to configure the project:
cd tests
cmake -B build -G "Unix Makefiles" .

# ...then run this whenever you want to rebuild and re-run tests
cmake --build build
ctest --test-dir build --output-on-failure

# ...alternatively run directly with make from the build directory:
cd build
make
ctest --output-on-failure
```

To re-run only failing tests, run `ctest` with the `--rerun-failed` flag.

You can also run the test executables directly; for example:

```sh
./build/test-example
```

This is useful, for example, to run tests with a debugger:

```sh
gdb ./build/test-example
```
