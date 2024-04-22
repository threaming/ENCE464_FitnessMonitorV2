[[_TOC_]]

# IDE

Visit https://code.visualstudio.com/download to download latest version.

On Windows you may be able to use `winget` to update or install VS Code:

```
winget upgrade --id Microsoft.VisualStudioCode
```

On macOS or Linux, you could install VS Code via your package manager, but it is
probably better to use one of the download methods listed at
https://code.visualstudio.com/download to get the most up-to-date version.

# Windows

## Build system

Install [CMake](https://cmake.org/download/) and add to `PATH`.

CMake can also be installed via winget:

```
winget install -e --id Kitware.CMake
```

## Embedded toolchain

For the toolchain (compiler, OpenOCD), there are three options:

1. Use the ENCE461 toolchain directory from the lab PCs (located at
   `C:\ENCE461`; you could transfer the folder to your PC using a flash drive).
   Add the following directories to `PATH`:

   * `C:\ence461\tool-chain\gcc-arm-none-eabi-9-2019-q4\bin`
   * `C:\ence461\tool-chain\OpenOCD-0.10.0\bin`
   * `C:\ence461\tool-chain\msys64\usr\bin`

2. Download the various requirements separately:

   * [OpenOCD](https://gnutoolchains.com/arm-eabi/openocd/)
   * [GNU Arm Embedded Toolchain (arm-none-eabi)](https://developer.arm.com/downloads/-/arm-gnu-toolchain-downloads)
   * [MSYS2](https://www.msys2.org/)

   Add the above installed packages to `PATH` (they may be already added by
   their respective installers).

3. Just download [MSYS2](https://www.msys2.org/) and install [the
   toolchain](https://packages.msys2.org/package/mingw-w64-x86_64-arm-none-eabi-gcc)
   and [OpenOCD](https://packages.msys2.org/package/mingw-w64-x86_64-openocd)
   using its package manager:

   ```
   pacman -S \
      mingw-w64-x86_64-arm-none-eabi-toolchain \
      mingw-w64-x86_64-gdb-multiarch \
      mingw-w64-x86_64-openocd
   ```

   Ensure that the MSYS2 package directory is added to `PATH` (the installer may
   do this automatically).

   (**Important**: do not use MSYS2 to install CMake as seems to have issues
   with the Windows filesystem when run from native shells like CMD or
   PowerShell. Rather, install CMake from the official installer linked above.)

(To add variables to `PATH`, search for 'Edit the system environment variables'
in the Start Menu, then open 'Environment variables...' from the settings
window, then double-click on the `Path` variable in either the user or system
variables boxes.)

## Host toolchain

We could use MSVC to build unit tests to run on the host, but it may be better
to use MinGW GCC since students will be more familiar with GCC from other
embedded systems courses. This can be installed via MSYS:

```
pacman -S mingw-w64-x86_64-gcc
```

[Ruby](https://www.ruby-lang.org/en/downloads/) is also required for test
discovery. This can also be installed via
[winget](https://winget.run/pkg/RubyInstallerTeam/Ruby.3.1):

```
winget install -e --id RubyInstallerTeam.Ruby.3.1
```

# MacOS

Use [Homebrew](https://formulae.brew.sh/) to install toolchain:

```
brew install openocd cmake
brew install --cask gcc-arm-embedded
```

(For older versions of Homebrew, the second command is `brew cast install
gcc-arm-embedded`.)

To install a host compiler, run `cc` from a terminal and you will be prompted to
install Xcode if it is not already installed. This will install the entire macOS
development toolchain, so if you just want to install a compiler it may be
easier to just install the LLVM toolchain via Homebrew:

```
brew install llvm
```

# Linux

For Ubuntu/Debian-based systems:

```
sudo apt update
sudo apt install \
    cmake \
    gcc-arm-none-eabi \
    libnewlib-arm-none-eabi \
    libstdc++-arm-none-eabi-newlib \
    gdb-multiarch \
    openocd
```

To install the host GCC:

```
sudo apt install build-essential
```

(Other Linux distributions with different package managers will have similar
packages.)
