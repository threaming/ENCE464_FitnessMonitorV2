# ENCE464 Fitness Tracker Project

## CLI:

```
mkdir build
cd build
cmake -DCMAKE_TOOLCHAIN_FILE=../cmake/tm4c.cmake -G "Unix Makefiles" ..
make
```

To disable building demo apps, add `-DBUILD_DEMO_APPS=off` to above `cmake`
command, or edit `build/CMakeCache.txt` and change line with `BUILD_DEMO_APPS`
to `off`.

## VS Code

Requires 'CMake Tools' extension (VS Code should prompt you to install).

On first configuration, ensure that top-level `build/` directory is deleted.

Open VS Code in this directory:

1. From the command palette (and for all subsequent commands), open
   `CMake: Select a kit` and select `TM4C123`.
2. Run `CMake: Configure`.
3. Run `CMake: Build` or press F7.
4. To program the main program, run `CMake: Build target` or press Shift+F7 and
   select `program` target.
