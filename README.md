## Overview

Movesense sensor is a programmable multi-purpose device comprising accelerometer, gyroscope, magnetometer and thermometer together with optional hearthrate/IBI and intelligent gear ID APIs. The APIs follow the well known REST principle and can be used both internally by the partner app or externally via the iOS/Android libraries. API specification ([link to folder](https://bitbucket.org/suunto/movesense-device-lib/src/master/MovesenseCoreLib/resources/core/)), is based on Swagger 2.0 syntax.

## API

Resource | Description
---------|------------
/Info|API for accessing generic device information.
/DataLogger|Generic logger capable of logging max. 8 different resources
/Logbook|Generic Logbook from where the logged data can be read
/Device/Measurement/AngularVelocity|API enabling reading or subscribing angular velocity data (based on gyroscope).
/Device/Measurement/LinearAcceleration|API enabling reading or subscribing linear acceleration data (based accelerometer).
/Device/Measurement/MagneticField|API enabling reading or subscribing magnetic field data (based on magnetometer).
/Device/Measurement/Temperature|API enabling reading or subscribing temperature data (based on thermometer).
/Device/UserInteraction/Indication|API for controlling the LED.
/Device/Battery|API for reading the battery state.
/Dev/Time|API for accessing the local time (EPOCH).
/Device/SystemEvent|API for accessing system event log.
/Device/System/Mode|API for controlling the main system state (e.g. factory sleep). 

## Prerequisite

 * [GNU Toolchain for ARM Embedded](https://launchpad.net/gcc-arm-embedded)
 * [Ninja build tool](https://ninja-build.org/)
 * [cmake >=3.6](https://cmake.org/download/)
 * [Visual Studio Redistributable 2015](https://www.microsoft.com/en-us/download/details.aspx?id=48145)

Optionally: 

 * Nordic Semiconductor's [nRF5x-Command-Line-Tools-Win32](https://www.nordicsemi.com/eng/Products/Bluetooth-low-energy/nRF52-DK) - needed for flashing using JIG

## Example App Build Flow

```
#!python
> git clone git@bitbucket.org:suunto/movesense-device-lib.git
> cd movesense-device-lib
> mkdir myBuild
> cd myBuild
> cmake -G Ninja -DMOVESENSE_CORE_LIBRARY=../MovesenseCoreLib -DCMAKE_TOOLCHAIN_FILE=../MovesenseCoreLib/toolchain/gcc-nrf52.cmake ../SampleApp
> ninja
```
If successful, application binary can be found in the build folder with name *Movesense.hex*

if you are using a *Movesense programming JIG* you can flash the resulting .hex-file (and the nRF52 SoftDevice) with:
```
ninja flash
```
or you can use any other nRF52 compatible programming software such as *nRFGo Studio*.

## Frequently Asked Questions

For FAQ please check http://stackoverflow.com/search?q=movesense

If your problem is not addressed there, please [post](http://stackoverflow.com/questions/ask) a new question, and tag it with 'movesense' so that it will added to the FAQ collection.

## Bug Reports

Please report bugs by [raising](https://bitbucket.org/suunto/movesense-device-lib/issues/new) an Issue via Bitbucket.

## Contributions
Your input is appreciated and we encourage you to post your contributions as pull requests to this repository.

## License

See the LICENSE file for more info.