# ENCE464 Fitness Tracker Project

See [TOOLCHAIN](./TOOLCHAIN.md) for information on installing the toolchain on
your personal machine.

See [CLI](#cli) for instructions on building and running via the command line.

See [VS Code](#vs-code) for instructions on building and running using Visual
Studio Code.

For (the FAQ)[./FAQ.md] for tips on debugging FreeRTOS and other issues.

[[_TOC_]]

## CLI

### Building for microcontroller (target)

```bash
cd target
cmake --toolchain cmake/tm4c.cmake -B build -G "Unix Makefiles" .
cmake --build build --parallel
```

As an alternative to the last command, you can `cd` into the `build` directory
and run `make` directly:

```bash
cd build
make -j  # builds `all` target, -j flag uses multiple processes to build
```

To disable building demo apps, add `-DBUILD_DEMO_APPS=off` to above `cmake`
command, or edit `build/CMakeCache.txt` and change line with `BUILD_DEMO_APPS`
to `off`.

To program the main program to the microcontroller, run the `program` make target:

```bash
# From the target directory:
cmake --build build --target program

# ...or, from the `target/build` directory:
make program
```

The demo programs can be programmed using the `program-[demo name]` targets,
run `cmake -B build` to see a list of all programming and debug targets.

### Building and running tests on host machine

```
cd tests
cmake -B build -G "Unix Makefiles" .
cmake --build build --parallel
ctest --test-dir build
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

To configure target toolchain, open `target/CMakeLists.txt` in the editor, and
via the command palette (Ctrl+Shift+P), run `CMake: select a kit` and select the
`TM4C123` kit. Then run `CMake: Configure`. To build, press F7 or run `CMake:
Build` command.

To configure host tests, open `tests/CMakeLists.txt` in the editor. Run `CMake:
select a kit` and choose a `GCC` compiler for x86 or x86_64. Then run configure
and build commands as before.

To run the tests, run `CMake: Refresh Tests` via the command palette, then open
the test explorer from the sidebar on the left (the flask icon). Press the play
button next to the `tests` line to run all tests, or run a specific test by
clicking the play button next to its name.

*TODO*: programming and debugging instructions for VS Code.
