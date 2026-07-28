# PorscheMobileCharger

[![Native Unit Tests](https://github.com/steftri/LvglSkeleton/actions/workflows/native-unit-tests.yml/badge.svg?event=push)](https://github.com/steftri/LvglSkeleton/actions/workflows/native-unit-tests.yml)
[![Crowpanel Binary Build](https://github.com/steftri/LvglSkeleton/actions/workflows/crowpanel-binary-build.yml/badge.svg?event=push)](https://github.com/steftri/LvglSkeleton/actions/workflows/crowpanel-binary-build.yml)
[![Posix Binary Build](https://github.com/steftri/LvglSkeleton/actions/workflows/posix-binary-build.yml/badge.svg?event=push)](https://github.com/steftri/LvglSkeleton/actions/workflows/posix-binary-build.yml)
[![CMake Unit Tests](https://github.com/steftri/LvglSkeleton/actions/workflows/cmake-unit-tests.yml/badge.svg?event=push)](https://github.com/steftri/LvglSkeleton/actions/workflows/cmake-unit-tests.yml)

PorscheMobileCharger is a cross-platform LVGL application with a shared application core and platform-specific hardware abstraction layers for desktop and ESP32 targets.

## Overview

The project combines:

* a shared C++ application core in `src/`
* an MVC-style structure with `Model`, `View`, and `Controller`
* FreeRTOS-based execution for the desktop SDL2 simulator and the ESP32 target
* hardware abstraction layers in `hal/` for display, storage, Wi-Fi, MQTT, and lightstrip integration
* unit tests for reusable libraries under `lib/`

The current setup is intended for developing and validating an LVGL UI on the desktop first and then deploying the same application logic to a CrowPanel ESP32 5 inch V2 device.

## Supported Targets

### PlatformIO environments

* `desktop_sdl2_posix`: desktop build for macOS/Linux using SDL2 and the POSIX FreeRTOS port
* `desktop_sdl2_windows`: desktop build for Windows using SDL2
* `crowpanel_esp32_5inch_v2`: ESP32 build for the CrowPanel 5 inch V2 hardware
* `native`: native test-oriented environment without the SDL2 UI stack

### Toolchains and frameworks

* C++17
* PlatformIO
* LVGL 9.3
* FreeRTOS
* SDL2 for the desktop simulator
* Arduino framework for ESP32
* GoogleTest and Unity for unit tests

## Project Layout

* `src/`: shared application code
* `lib/`: reusable modules such as CRC, data handling, MQTT, Wi-Fi, worker, and surveillance logic
* `hal/`: target-specific implementations for desktop and ESP32
* `test/`: Unity and GoogleTest based unit and integration tests
* `images/`: source images used for LVGL asset generation
* `tools/`: helper scripts such as the LVGL image converter

## Getting Started

### Prerequisites

Install the following tools locally:

* PlatformIO
* Python 3
* CMake 3.14 or newer
* a C++ toolchain such as GCC or Clang
* GoogleTest development files for the CMake unit tests
* SDL2 development files for desktop builds
* Doxygen if API documentation should be generated

On macOS, SDL2 can for example be installed via Homebrew:

```bash
brew install sdl2
```

On Ubuntu, the CMake unit tests can be prepared with:

```bash
sudo apt-get update
sudo apt-get install --yes build-essential cmake libgtest-dev
```

### Run the CMake unit tests

The GoogleTest-based CMake unit tests run directly on the host system and do not require Docker.

Configure and execute them with:

```bash
cmake -S . -B build-ci -DBUILD_TESTING=ON
cmake --build build-ci --target UnitTests --parallel
```

This builds and runs the current CMake unit test executables:

* `UnitTestCrc32`
* `UnitTestData`

### Build the desktop simulator

Build the POSIX SDL2 target:

```bash
pio run -e desktop_sdl2_posix
```

Build and execute it directly:

```bash
pio run -e desktop_sdl2_posix -t execute
```

### Build and upload the ESP32 target

```bash
pio run -e crowpanel_esp32_5inch_v2
pio run -e crowpanel_esp32_5inch_v2 -t upload
```

### CMake build

The repository also contains a CMake-based build for selected native targets and GoogleTest-based unit tests:

```bash
cmake -S . -B build
cmake --build build
```

## Testing

### PlatformIO tests

Run the native Unity-based tests:

```bash
pio test -e native
```

### CMake and GoogleTest

Configure and run the GoogleTest-based unit tests:

```bash
cmake -S . -B build
cmake --build build --target UnitTests
ctest --test-dir build --output-on-failure
```

The repository currently contains unit tests for at least the CRC and data modules, plus integration tests for target-oriented flows.

## Debugging

### ESP32

For the `crowpanel_esp32_5inch_v2` environment, debugging is configured for `esp-prog`.

Relevant `platformio.ini` settings:

* `debug_tool = esp-prog`
* `debug_init_break = tbreak setup`

The usual JTAG wiring between ESP-Prog and the target ESP32 board is required.

### Desktop

The desktop target is useful for iterating on UI logic and task behavior without flashing hardware. On macOS, SDL2 window creation and event handling must run on the main thread.

## Images

LVGL image assets can be generated from PNG files with:

```bash
python3 tools/LVGLImage.py --ofmt C --cf I4 --name <target_name> -o src/view/assets images/<source.png>
```

## Documentation

If Doxygen is installed, the CMake target can be used to generate API documentation:

```bash
cmake --build build --target Doxygen
```

## Notes

* The desktop build uses SDL2 together with the POSIX FreeRTOS port.
* The ESP32 build pulls in additional hardware-specific libraries such as LovyanGFX, ArduinoMqttClient, Adafruit NeoPixel, and 8BitFonts.
* The shared application flow is initialized in the platform-specific `main.cpp` entry points and then handed over to `Controller`, `Model`, and `View`.

## Repository

Further information and CI runs are available on GitHub:

https://github.com/steftri/LvglSkeleton
