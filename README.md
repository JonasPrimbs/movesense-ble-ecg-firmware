## Announcement ##
**API CHANGES!** Before you start to write an app check a new API for accelerometer, gyroscope, magnetic field!

## Overview ##

Movesense sensor is a programmable multi-purpose device comprising accelerometer, gyroscope, magnetometer and thermometer together with optional hearthrate/IBI and intelligent gear ID APIs. The APIs follow the well known REST principle and can be used both internally by the partner app or externally via the iOS/Android libraries. API specification ([link to folder](https://bitbucket.org/suunto/movesense-device-lib/src/master/MovesenseCoreLib/resources/core/)), is based on Swagger 2.0 syntax.

## API ##
Resource | Description|Implemented
---------|------------|--------------
/Info|API for accessing generic device information.| YES
/DataLogger|Generic logger capable of logging max. 8 different resources| NO! Bug will be fixed soon!
/Logbook|Generic Logbook from where the logged data can be read| NO! Bug will be fixed soon!
**/Meas/Gyro**|API enabling subscribing angular velocity data (based on gyroscope).| YES
**/Meas/Acc**|API enabling subscribing linear acceleration data (based on accelerometer).| YES
**/Meas/Magn**|API enabling subscribing magnetic field data (based on magnetometer).| YES
/Device/Measurement/Temperature|API enabling reading or subscribing temperature data (based on thermometer).| YES
/Device/Component/Led|API for controlling the LED.| YES
/Device/UserInteraction/Indication|API for controlling the LED.| NO
/Device/Power/BatteryLevel|API for reading the battery state.| YES
/Dev/Time|API for accessing different time related services.| YES*
/Device/SystemEvent|API for accessing system event log.| NO
/Device/System/Mode|API for controlling the main system state (e.g. factory sleep).| YES

## Prerequisite ##

 * [GNU Toolchain for ARM Embedded 5-2016-q3](https://developer.arm.com/open-source/gnu-toolchain/gnu-rm/downloads)
 * [Ninja build tool](https://ninja-build.org/) - remember to add to path
 * [cmake >=3.6](https://cmake.org/download/)
 * [Visual Studio Redistributable 2015](https://www.microsoft.com/en-us/download/details.aspx?id=48145)
 * [nrfutil package & python 2.7 (for OTA firmware update package creation)](https://github.com/NordicSemiconductor/pc-nrfutil)
 
Optionally: 

 * Nordic Semiconductor's [nRF5x-Command-Line-Tools-Win32](https://www.nordicsemi.com/eng/Products/Bluetooth-low-energy/nRF52-DK) - needed for flashing using JIG

## Example App Build Flow ##

```
#!python
> git clone git@bitbucket.org:suunto/movesense-device-lib.git
> cd movesense-device-lib
> mkdir myBuild
> cd myBuild

> cmake -G Ninja -DMOVESENSE_CORE_LIBRARY=../MovesenseCoreLib -DCMAKE_TOOLCHAIN_FILE=../MovesenseCoreLib/toolchain/gcc-nrf52.cmake ../samples/hello_world_app
or
> cmake -G Ninja -DMOVESENSE_CORE_LIBRARY=../MovesenseCoreLib -DCMAKE_TOOLCHAIN_FILE=../MovesenseCoreLib/toolchain/gcc-nrf52.cmake ../samples/accelerometer_app
or
> cmake -G Ninja -DMOVESENSE_CORE_LIBRARY=../MovesenseCoreLib -DCMAKE_TOOLCHAIN_FILE=../MovesenseCoreLib/toolchain/gcc-nrf52.cmake ../samples/blinky_app

> ninja
```
*Note: As default, the SERIAL interface is enabled. If flashed/used without the JIG, remember to disable it to avoid excessive power consumption ([link to instructions](https://bitbucket.org/suunto/movesense-device-lib/src/master/MovesenseCoreLib/documentation/PowerOptimization.md?at=master&fileviewer=file-view-default))*

### Updating sensor "over the air" ###

If you want to create OTA firmware update package run command
```
ninja dfupkg
```

which places the OTA package with name *movesense_dfu.zip* in the build folder.

### Updating sensor with programming jig ###

if you are using a *Movesense programming JIG* you can flash the resulting .hex-file (and the nRF52 SoftDevice) with:
```
ninja flash
```
or you can use any other nRF52 compatible programming software such as *nRFGo Studio*.



## What's New ##
For information about releases, see the [CHANGES.md](CHANGES.md) file in repository.

## Frequently Asked Questions ##

For FAQ please check http://stackoverflow.com/search?q=movesense

If your problem is not addressed there, please [post](http://stackoverflow.com/questions/ask) a new question, and tag it with 'movesense' (i.e. include [tag:movesense] in the question body).

## Bug Reports ##

Please report bugs by [raising](https://bitbucket.org/suunto/movesense-device-lib/issues/new) an Issue via Bitbucket.

## Contributions ##
Your input is appreciated and we encourage you to post your contributions as pull requests to this repository.

## License ##

See the LICENSE file for more info.