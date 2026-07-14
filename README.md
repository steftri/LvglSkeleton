# LvglSkeleton

[![Native Unit Tests](https://github.com/steftri/LvglSkeleton/actions/workflows/native-unit-tests.yml/badge.svg?event=push)](https://github.com/steftri/LvglSkeleton/actions/workflows/native-unit-tests.yml)
[![Crowpanel Binary Build](https://github.com/steftri/LvglSkeleton/actions/workflows/crowpanel-binary-build.yml/badge.svg?event=push)](https://github.com/steftri/LvglSkeleton/actions/workflows/crowpanel-binary-build.yml)
[![Posix Binary Build](https://github.com/steftri/LvglSkeleton/actions/workflows/posix-binary-build.yml/badge.svg?event=push)](https://github.com/steftri/LvglSkeleton/actions/workflows/posix-binary-build.yml)

LvglSkeleton is a cross-platform LVGL application skeleton with a shared application core and platform-specific hardware abstraction layers for desktop and ESP32 targets.

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
* SDL2 development files for desktop builds
* Doxygen if API documentation should be generated

On macOS, SDL2 can for example be installed via Homebrew:

```bash
brew install sdl2
```

### Build the desktop simulator

Build the POSIX SDL2 target:

```bash
python3 tools/LVGLImage.py --ofmt C --cf I4 --name <target name> -o src/view/assets images/<source image in PNG format>
```

## Known Anomalies

* Debugging via ESP-Prog does not work with ESP8266
* Debugging via ESP-Prog does not work with Lolin32 (ESP32-D0WDQ6 rev 1.1)

## Changelog

### 1.1.0

* Linting with SonarCube added
* platformio.ini cleaned up
* Integration test added, which is only executed on real targets
* Debugging option for Arduino UNO/MEGA2560 added
* Debugging option for ESP32 added

### 1.0.0

* Initial release

# Additional information

## Create a new project based on this Arduino library skeleton

To use *ArduinoLibrarySkeleton_dev* as a template for a new project, it has to be forked locally.

Two repositories need to be created. One is the repository which is referenced in the projects which uses the library. The other one contains the environment which is usefull for development and debugging as well as the unit tests. It references the real library repository as a GIT submodule. 


### On **GitHub:**

1. Create a new library repository, i.e. *my-arduino-library*

2. Create a new development repository, i.e. *my-arduino-library_dev*

### Locally within a **Git Bash:**

1. Clone the *ArduinoLibrarySkeleton* and *ArduinoLibrarySkeleton_dev* skeleton as a **bare repository**:
   ```bash
      git clone --bare https://github.com/steftri/ArduinoLibrarySkeleton.git
      git clone --bare https://github.com/steftri/ArduinoLibrarySkeleton_dev.git
   ```
2. Replace origin with the one for your new project (i.e. project *my-arduino-library*, with *your-name* as GitHub user name):
   ```bash
      cd ./ArduinoLibrarySkeleton.git
      git remote rm origin
      git remote add origin https://github.com/your-name/my-arduino-library.git
      cd ..

      cd ./ArduinoLibrarySkeleton_dev.git
      git remote rm origin
      git remote add origin https://github.com/your-name/my-arduino-library_dev.git
      cd ..
   ```
3. Push both bare repos as a **mirror** to your new origins:
   ```bash
      git -C ArduinoLibrarySkeleton.git push --mirror
      git -C ArduinoLibrarySkeleton_dev.git push --mirror
   ```
4. Clone the new project (i.e. project *my-arduino-library*, with *your-name* as GitHub user name):
   ```bash
      git clone -b develop https://github.com/your-name/my-arduino-library_dev.git
   ```
5. Remove the bare -*ArduinoLibrarySkeleton* and *ArduinoLibrarySkeleton_dev* template projects:
   ```bash
      rm -rf ./ArduinoLibrarySkeleton.git
      rm -rf ./ArduinoLibrarySkeleton_dev.git
   ```
6. In the new *my-arduino-library_dev.git* repository, replace the submodule by target one
   ```bash
      cd ./my-arduino-library_dev
      git rm lib/ArduinoLibrarySkeleton
      git submodule add -b develop https://github.com/your-name/my-arduino-library.git lib/my-arduino-library
      git commit -m "Skeleton submodule replaced by target one"
      git push
   ```   

7. Checkout new submodule
   ```bash
      git submodule update --init --recursive
   ```    

   
## Website

Further information can be found on [GitHub](https://github.com/steftri/ArduinoLibrarySkeleton_dev).
