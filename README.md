## Overview

Movesense sensor is a programmable multi-purpose device comprising accelerometer, gyroscope, magnetometer and thermometer together with optional hearthrate/IBI and intelligent gear ID APIs. The APIs follow the well known REST principle and can be used both internally by the partner app or externally via the iOS/Android libraries. API specification ([link to folder](https://bitbucket.org/suunto/movesense-device-lib/src/master/MovesenseCoreLib/resources/core/)), is based on Swagger 2.0 syntax.

## API
**/DataLogger** Generic logger capable of logging max. 8 different resources (even custom ones) to the external EEPROM.

**/Logbook** Generic Logbook from where the logged data can be read (ref. /DataLogger).

**/Device/Measurement/AngularVelocity** API enabling reading or subscribing angular velocity data (this is based on readings from the gyroscope).

**/Device/Measurement/LinearAcceleration** API enabling reading or subscribing linear acceleration data (this is based on readings from the accelerometer).

**/Device/Measurement/MagneticField** API enabling reading or subscribing magnetic field data (this is based on readings from the magnetometer).

**/Device/Measurement/Temperature** API enabling reading or subscribing temperature data (this is based on readings from the thermometer).

**/Info** API for accessing generic device information.

**/Device/UserInteraction/Indication** API for controlling the LED.

**/Device/Battery** API for reading the battery state.

**/Dev/Time** API for accessing the local time (EPOCH).

**/Device/SystemEvent** API for accessing system event log.

**/Device/System/Mode** API for controlling the main system state (e.g. factory sleep). 

## Installation

*Movesense Core library* for the Movesense device is available from this git repository. Just unpack the core lib with the example app to an arbitrary folder. For convenience, the whole SW is also provided as a .zip file (see the Download section).  

To compile the applications you also need the following tools and programs:

 * [GNU Toolchain for ARM Embedded](https://launchpad.net/gcc-arm-embedded)
 * [Ninja build tool](https://ninja-build.org/) (download the ninja.exe and put it in the path)
 * [cmake >=3.6](https://cmake.org/download/)

and if you want to flash the device firmware directly from build process using the *Movesense Programming Jig* need also:

 * Nordic Semiconductor's [nRF5x-Command-Line-Tools-Win32](https://www.nordicsemi.com/eng/Products/Bluetooth-low-energy/nRF52-DK)

## Example App

To build the "Example app" project, you will need to clone the repo or download both *Movesense Core library* and *Example App*, and run the following commands from a build directory of your choosing.

Generate build file:
```
cmake -G Ninja -DMOVESENSE_CORE_LIBRARY=<path_to_movesense_library> -DCMAKE_TOOLCHAIN_FILE=<path_to_movesense_library>/toolchain/gcc-nrf52.cmake <path_to_example_app>
```

Build the example app:
```
ninja
```

When the build completes the application will be found in the build folder with name *Movesense.hex*

if you are using a *Movesense programming jig* you can flash the resulting .hex-file (and the nRF52 SoftDevice) with:
```
ninja flash
```
or you can use any other nRF52 compatible programming software such as *nRFGo Studio*.

## Frequently Asked Questions

FAQ can be found via http://stackoverflow.com/questions/tagged/movesense. If your problem is not addressed there, please [post](http://stackoverflow.com/questions/ask) a new question, and tag it with 'movesense' so that it will added to the FAQ collection.

## Bug Reports

Please report bugs by [raising](https://bitbucket.org/suunto/movesense-device-lib/issues/new) an Issue via Bitbucket.

## Contributions
Your input is appreciated and we encourage you to post your contributions as pull requests to this repository.

## License

See the LICENSE file for more info.