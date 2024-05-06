[[_TOC_]]

# Visual Studio Code

Visit https://code.visualstudio.com/download to download latest version.

On Windows you may be able to use `winget` to update or install VS Code:

```
winget install --id Microsoft.VisualStudioCode
```

On macOS or Linux, you could install VS Code via your package manager, but it is
probably better to use one of the download methods listed at
https://code.visualstudio.com/download to get the most up-to-date version.

# Windows

## Build system

Install [CMake](https://cmake.org/download/). Make sure
`C:\Program Files\CMake\bin` is added to the `PATH` environment variable
(see below, the installer may do this automatically).

CMake can also be installed via winget:

```
winget install -e --id Kitware.CMake
```

To add variables to `PATH`, search for **Edit the system environment variables**
in the Start Menu, then open **Environment variables...** from the settings
window, then double-click on the `Path` variable in either the user or system
variables boxes. Then click **New** to add each path.

![Environment variables](screenshots/windows-env-vars.png)

## In-circuit debugger driver

You will need to download the [Stellaris ICDI Drivers](https://www.ti.com/tool/STELLARIS_ICDI_DRIVERS).
Follow the instructions in the installation guide in the link.

## Embedded toolchain

For the toolchain (compiler, OpenOCD), there are two options:

1. Use the ENCE464 toolchain directory from the lab PCs (located at
   `C:\ENCE464`; you could transfer the folder to your PC using a flash drive).
   Add the following directories to `PATH`:

   * `C:\ence464\tool-chain\gcc-arm-none-eabi-13.2\bin`
   * `C:\ence464\tool-chain\OpenOCD-0.10.0\bin`
   * `C:\ence464\tool-chain\msys64\usr\bin`

   This is the same setup as the Windows machines in the ESL. It also includes
   the necessary host programs for building host tests.

2. Just install [MSYS2](https://www.msys2.org/) and install the toolchain and
   OpenOCD using `pacman`:

   ```
   pacman -S \
      make \
      mingw-w64-x86_64-arm-none-eabi-toolchain \
      mingw-w64-x86_64-gdb-multiarch \
      mingw-w64-x86_64-openocd
   ```

   Ensure that the MSYS2 binary directory is added to `PATH` (the installer
   should do this).

   (**Important**: do not use MSYS2 to install CMake - you should install this
   using the official installer or winget as described above.)

## Host toolchain

**Note**: if you have the `C:\ence464` toolchain folder on your machine, you can
skip this section.

MinGW GCC is a port of GCC for Windows. It can be installed via MSYS:

```
pacman -S mingw-w64-x86_64-gcc
```

[Ruby](https://www.ruby-lang.org/en/downloads/) is also required for test
discovery. This can also be installed via
[winget](https://winget.run/pkg/RubyInstallerTeam/Ruby.3.1):

```
winget install -e --id RubyInstallerTeam.Ruby.3.1
```

Or in MSYS:

```
pacman -S mingw-w64-x86_64-ruby
```

# MacOS

Use [Homebrew](https://brew.sh/) to install CMake, OpenOCD, and the embedded
toolchain:

```
brew install cmake openocd
brew install --cask gcc-arm-embedded
```

(If the second command doesn't work, you may have an older version of Homebrew -
run `brew cast install gcc-arm-embedded` instead.)

For building host tests, install the native LLVM toolchain and Ruby via
Homebrew:

```
brew install llvm ruby
```

(If you already have Xcode installed, you can skip installing `llvm`.)

# Linux

For Debian-based systems such as Ubuntu:

```
sudo apt-get update
sudo apt-get install \
    cmake \
    gcc-arm-none-eabi \
    libnewlib-arm-none-eabi \
    gdb-multiarch \
    openocd
```

For building host tests, install the native compiler toolchain and Ruby:

```
sudo apt-get install build-essential ruby
```

(Other Linux distributions with different package managers will have
similarly-named packages.)
