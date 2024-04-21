# ENCE464 Fitness Tracker Project

See [TOOLCHAIN](./TOOLCHAIN.md) for information on installing the toolchain on
your personal machine.

See [CLI](#cli) for instructions on building and running via the command line.

See [VS Code](#vs-code) for instructions on building and running using Visual
Studio Code.

See [the FAQ](./FAQ.md) for tips on debugging FreeRTOS and other issues.

See [CMAKE](./CMAKE.md) for info on performing certain tasks with the CMake
build system.

[[_TOC_]]

## CLI

### Building for microcontroller (target)

```sh
# Run this once to configure the project:
cd target
cmake --toolchain cmake/tm4c.cmake -B build -G "Unix Makefiles" .

# ...then run this whenever you want to rebuild the project:
cmake --build build --parallel

# ...alternatively, you can run make directly inside the build directory:
cd build
make -j # builds `all` target, -j flag uses multiple processes to build
        # (equivalent to --parallel option with cmake)
```

To disable building demo apps, add `-DBUILD_DEMO_APPS=off` to above `cmake`
command, or edit `build/CMakeCache.txt` and change line with `BUILD_DEMO_APPS`
to `off`.

To program the main program to the microcontroller, run the `program` make target:

```sh
# From the target directory:
cmake --build build --target program

# ...or, from the `target/build` directory:
make program
```

The demo programs can be programmed using the `program-[demo name]` targets,
run `cmake -B build` to see a list of all programming and debug targets.

### Building and running tests on host machine

```sh
# Run this once to configure the project:
cd tests
cmake -B build -G "Unix Makefiles" .

# ...then run this whenever you want to rebuild and re-run tests
cmake --build build --parallel
ctest --test-dir build

# ...alternatively run directly with make from the build directory:
cd build
make
make test
```

## VS Code

Requires 'CMake Tools' extension (VS Code should prompt you to install).

Before configuring VS Code for the first time, ensure that any `target/build/`
or `tests/build/` directories are deleted.

Open VS Code in *this top-level directory*:

* Inside VS Code, open 'File' -> 'Open folder...'
* or, in a terminal, run:

  ```
  code <path to this directory>
  ```

  E.g. `code .` if you have this directory open in a terminal.

VS Code will prompt you to install a few recommended extensions, click the
'Install' button to install them.

### Building for microcontroller (target)

To configure target toolchain, open `target/CMakeLists.txt` in the editor, and
via the command palette (Ctrl+Shift+P), run `CMake: select a kit` and select the
`TM4C123` kit. Then run `CMake: Configure`. To build, press F7 or run `CMake:
Build` command.

To program the main program to the microcontroller, press Ctrl+Shift+B and
select the `Program` command. Alternatively run the `CMake: Build target`
command from the command palette or press Shift+F7 and select the `Program`
command (or any other desired target - see [CLI](#cli) section above for details
on other program/debug targets).

### Building and running tests on host machine

To configure host tests, open `tests/CMakeLists.txt` in the editor. Run `CMake:
select a kit` and choose a `GCC` compiler for x86 or x86_64. Then run configure
and build commands as before.

To run the tests, run `CMake: Refresh Tests` via the command palette, then open
the test explorer from the sidebar on the left (the flask icon). Press the play
button next to the `tests` line to run all tests, or run a specific test by
clicking the play button next to its name.

**Note**: you only have to run the configure commands once. Subsequent builds
can be triggered just via the build command (or pressing F7). You should only
have to re-configure if you delete the `build` directory.
