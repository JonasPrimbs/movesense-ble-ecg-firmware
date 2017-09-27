## Announcements ##
**API CHANGES!** Before you start to write an app check the new API!
**Known bugs!** Check the [CHANGES.md](CHANGES.md)

## Overview ##

Movesense sensor is a programmable multi-purpose device comprising accelerometer, gyroscope, magnetometer and thermometer together with optional heartrate/IBI and intelligent gear ID APIs. The APIs follow the well known REST principle and can be used both internally by the partner app or externally via the iOS/Android libraries. API specification ([link to folder](https://bitbucket.org/suunto/movesense-device-lib/src/master/MovesenseCoreLib/resources/core/)), is based on Swagger 2.0 syntax.

## API ##
Resource | Description|Implemented
---------|------------|--------------
/Comm/Ble|API for managing BLE | YES
/Component/Eeprom|API for writing and reading the EEPROM memory/ies. | YES
/Info|API for accessing generic device information.| YES
/Meas/Acc|API enabling subscribing linear acceleration data (based on accelerometer).| YES
/Meas/ECG|API for the electrocardiography measurement.| YES
/Meas/Gyro|API enabling subscribing angular velocity data (based on gyroscope).| YES
/Meas/HR|API enabling subscribing heart rate data.| YES
/Meas/Magn|API enabling subscribing magnetic field data (based on magnetometer).| YES
/Meas/Temp|API enabling reading or subscribing temperature data (based on thermometer).| YES
/Mem/DataLogger|Generic logger capable of logging max. 8 different resources| YES
/Mem/Logbook|Generic Logbook from where the logged data can be read| YES
/Misc/Gear| API to get the globally unique Movesense ID associated with the physical gear | YES
/System/Energy|API for reading the battery state.| YES
/System/Mode|API for controlling the main system state (e.g. factory sleep).| YES
/System/Settings| Settings API for a Movesense device. | YES
/Time|API for accessing different time related services.| YES
/Ui/Ind|API for controlling the LED.| YES


## Prerequisite ##

 * [GNU Toolchain for ARM Embedded 6-2017-q2](https://developer.arm.com/open-source/gnu-toolchain/gnu-rm/downloads) 
 * [Ninja build tool](https://ninja-build.org/)
 * [cmake >=3.6](https://cmake.org/download/)
 * [nrfutil package & python 2.7 (for OTA firmware update package creation)](https://github.com/NordicSemiconductor/pc-nrfutil)
      * Windows: Nordic provides precompiled Windows executable
      * OS X: First install pip for managing Python packages: `sudo easy_install pip` and then nrfutil: `sudo pip install nrfutil`
 * Windows: [Visual Studio Redistributable 2015](https://www.microsoft.com/en-us/download/details.aspx?id=48145)
 
Optionally: 

 * For flashing using JIG:
      * Nordic Semiconductor's [nRF5x-Command-Line-Tools-XXX](https://www.nordicsemi.com/eng/Products/Bluetooth-low-energy/nRF52-DK) for Win32 or OSX
      * OS X: you also need [Segger J-Link Software and Documentation pack for MacOSX](https://www.segger.com/downloads/jlink/)
 
Path configuration:

 * Remember to add ninja to path
 * OS X: remember to add also GNU Toolchain for ARM, nrfjprog and mergehex (provided with nrfjprog) to path
 

## Example App Build Flow ##

```
#!python
> git clone git@bitbucket.org:suunto/movesense-device-lib.git
> cd movesense-device-lib
> mkdir myBuild
> cd myBuild

> cmake -G Ninja -DMOVESENSE_CORE_LIBRARY=../MovesenseCoreLib/ -DCMAKE_TOOLCHAIN_FILE=../MovesenseCoreLib/toolchain/gcc-nrf52.cmake ../samples/hello_world_app
or
> cmake -G Ninja -DMOVESENSE_CORE_LIBRARY=../MovesenseCoreLib/ -DCMAKE_TOOLCHAIN_FILE=../MovesenseCoreLib/toolchain/gcc-nrf52.cmake ../samples/accelerometer_app
or
> cmake -G Ninja -DMOVESENSE_CORE_LIBRARY=../MovesenseCoreLib/ -DCMAKE_TOOLCHAIN_FILE=../MovesenseCoreLib/toolchain/gcc-nrf52.cmake ../samples/blinky_app
or release version
> cmake -G Ninja -DMOVESENSE_CORE_LIBRARY=../MovesenseCoreLib/ -DCMAKE_TOOLCHAIN_FILE=../MovesenseCoreLib/toolchain/gcc-nrf52.cmake -DCMAKE_BUILD_TYPE=Release ../samples/hello_world_app

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