# CMake build system

This project uses [CMake](https://cmake.org/cmake/help/latest/) to manage
building the source code. Unlike Make, CMake does not directly execute the
compile and link commands to build programs; rather it *generates* Makefiles
which are then used to build the project.

CMake is quite a powerful tool that can be used to build large projects with
complex dependencies. However, for this project, you should only need to use
a small subset of its features to do what you need. This file provides recipes
for a set of common tasks.

[[_TOC_]]

## CMakeLists.txt

CMake projects are configured in files named `CMakeLists.txt` which have a
different syntax to Makefiles. In these, you declare *targets*, which are
executables or libraries, and their dependencies, include directories, and
compilation options.

`CMakeLists.txt` consist of a series of commands or functions:

```cmake
command(arg1 arg2 "argument with spaces" ...)
```

Unlike in C, command arguments are separated by whitespace, not commas. Surround
an argument with quotation marks if it contains whitespace.

Any text after a `#` character is treated as a comment:

```cmake
add_tiva_executable(example
    source1.c
    source2.c  # this is a comment
    source3.c
)
```

Variables are set using the `set` command:

```cmake
set(VARIABLE "Hello World")
```

Variables are then accessed using `${VARIABLE}`, for example:

```cmake
set(OTHER_VARIABLE "Variable = ${VARIABLE}")
```

Will set the value of `OTHER_VARIABLE` to `Variable = Hello World`. Variable
names are case-sensitive; the convention is to write them in ALL_CAPS.

## Common tasks

Examples of the following tasks can be found in `CMakeLists.txt` files
throughout this repository.

### Adding a new C file to a program

For example, to add a new C file to the main fitness tracker program, edit
`target/src/CMakeLists.txt`, adding the name of the new file to the list in the
`add_tiva_executable` command (see [Adding a new executable](#adding-a-new-executable)).

### Adding a new executable

(E.g. see `target/src/CMakeLists.txt`, or `target/demo` directories.)

Programs (executables) are declared with the `add_tiva_executable` command:

```cmake
add_tiva_executable(program-name source1.c source2.c ...)
```

Executable (target) names can contain underscores and dashes.

You only need to list the `.c` files, CMake will automatically determine each
file's header dependencies and rebuild whenever an included header file is
changed.

Executable source files paths are relative to the `CMakeLists.txt` file in which
they are listed. So if the `.c` file is in the same directory as the
`CMakeLists.txt` file, you don't need to add any directory names in front of it.

### Specifying include directories

(E.g. see `target/src/CMakeLists.txt`)

To specify directories to search for including header files when building
`target-name`:

```cmake
target_include_directories(target-name PRIVATE include_dir1 include_dir2 ...)
```

This is equivalent to passing the `-I` flag to GCC. (See
[Adding a new library](#adding-a-new-library) for info on the meaning of `PRIVATE`.)

### Linking libraries to a target

(E.g. see `target/src/CMakeLists.txt`)

To specify a library that must be linked to the target `target-name`:

```cmake
target_link_libraries(target-name PUBLIC lib1 lib2)
```

This will ensure that the specified libraries are built before `target-name`.
If any of the code for the listed libraries is changed, they will be re-built
and `target-name` will be re-linked with the updated libraries.

### Setting compilation flags

(E.g. see `target/src/CMakeLists.txt`)

For example, to enable all compiler warnings and treat warnings as errors
when compiling `target-name`:

```cmake
target_compile_options(target-name PRIVATE -Wall -Werror)
```

### Defining a preprocessor macro

(E.g. see `target/libs/freertos/CMakeLists.txt`)

To define a preprocessor macro when compiling `target-name`:

```cmake
target_compile_definitions(
    target-name
    PRIVATE
    MACRO_WITHOUT_VALUE
    MACRO_WITH_VALUE=VALUE
)
```

The above is equivalent to adding the following code in all source files
for `target-name`:

```c
#define MACRO_WITHOUT_VALUE
#define MACRO_WITH_VALUE VALUE
```

### Adding a new library

(E.g. see `target/libs` directories)

To add a new library for re-using code in multiple programs:

```cmake
add_library(library-name STATIC source1.c source2.c ...)
```

Library (target) names can contain underscores and dashes.

You can use `target_include_directories`, `target_link_libraries`,
`target_compile_options` etc. on library targets as with executables, but there
is one important distinction: the value of the second argument to these commands
(`PRIVATE` or `PUBLIC`) determines how other targets that link to the library
are treated:

```cmake
add_library(lib_example STATIC example.c)
target_include_directories(lib_example PUBLIC include_dir)

# Because 'include_dir' was added as a PUBLIC include directory, it will be
# added as an include directory when building 'program'. If it was PRIVATE,
# 'include_dir' would only be made visible to 'lib_example' when building, but
# not to any targets that depend on it.
add_tiva_executable(program program.c)
target_link_libraries(
    program
    PRIVATE # use of PRIVATE/PUBLIC here doesn't matter for executables
    lib_example
)
```

### Adding a new test program

In `tests/CMakeLists.txt`, use `add_unity_test` to create a new test executable
with the Unity test framework linked-in:

```cmake
add_unity_test(test-executable-name test-source.c)
```

This will create an executable target called `text-executable-name`, which can
be used with `target_include_directories`, `target_link_libraries` etc. as if it
were a regular executable created using `add_tiva_executable`:

```cmake
target_link_libraries(test-executable-name PRIVATE test-library)
```
