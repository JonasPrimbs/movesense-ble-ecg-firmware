## Announcements
  
**(26th Jan 2022) Sensor firmware 2.1.2 is out!**  

This is a bugfix release that fixes an issue that was found out just after 2.1.1 release. Please test 
your software thoroughly before committing to the 2.x since the sensors cannot be rolled back to 1.9.x!

**Known issues**  
Please refer to [CHANGES.md](CHANGES.md) for a list of changes & known issues.

**NOTE: The build environment was changed to be docker based in release 2.0. Please read the instruction at http://movesense.com/docs/esw/getting_started/**


## Overview

[Movesense sensor](https://www.movesense.com/developers/technology/#developers) is a programmable multi-purpose device comprising of accelerometer, gyroscope, magnetometer and thermometer together with optional heartrate/IBI (inter-beat interval) and intelligent gear ID APIs.  

Movesense APIs follow well-known REST principle and can be used both internally by the sensor app or externally via the iOS/Android libraries. The [API specification](https://bitbucket.org/suunto/movesense-device-lib/src/master/MovesenseCoreLib/resources/movesense-api/) is based on Swagger 2.0 syntax.

You can order Movesense sensors and Movesense Developer Kit on our online [shop](https://www.movesense.com/shop/).

Check also [Movesense mobile library](https://bitbucket.org/suunto/movesense-mobile-lib) for developing own Android and iOS applications to interact with the Movesense sensor.  

## Documentation

Movesense developer documentation can be found in:

[README.md](README.md): this document, with latest important information and links to other documentation.  
[CHANGES.md](CHANGES.md): version history of Movesense releases.  
[Movesense developer documentation](https://movesense.com/docs).  

## Movesense APIs

The main documentation of sensor API's is in [our documentation pages](http://movesense.com/docs/esw/api_reference/).

However here's a short and incomplete list on what is available:

Resource | Description
---------|------------
/Comm/Ble|API for managing BLE.  
/Component/Eeprom|API for writing and reading the EEPROM memory/ies. 
/Info|API for accessing generic device information.
/Meas/Acc|API enabling subscribing linear acceleration data (based on accelerometer).
/Meas/ECG|API for the electrocardiography measurement.
/Meas/Gyro|API enabling subscribing angular velocity data (based on gyroscope).
/Meas/HR|API enabling subscribing heart rate data.
/Meas/Imu|API for synchronous access to motion data (accelerometer, gyroscope, magnetometer)
/Meas/Magn|API enabling subscribing magnetic field data (based on magnetometer).
/Meas/Temp|API enabling reading or subscribing temperature data (based on thermometer).
/Mem/DataLogger|Generic logger capable of logging max. 8 different resources.
/Mem/Logbook|Generic Logbook from where the logged data can be read.
/Misc/Gear| API to get the globally unique Movesense ID associated with the physical gear.
/System/Debug|API for subscribing messages from device.
/System/Energy|API for reading the battery state.
/System/Memory|API for reading memory state.
/System/Mode|API for controlling the main system state (e.g. factory sleep).
/System/Settings| Settings API for a Movesense device.
/System/State|API for reading some states.
/Time|API for accessing different time related services.
/Ui/Ind|API for controlling the LED.

## Setting up the development environment  

The latest information about setting up the toolchain can be found in [documentation](http://movesense.com/docs/esw/getting_started/).

## Bug reports and other feedback

Please report all bugs by [raising an Issue](https://bitbucket.org/suunto/movesense-device-lib/issues/new) on Bitbucket. 

You can also discuss Movesense related topics on [stack overflow](http://stackoverflow.com/search?q=movesense). If your topic is not already addressed, 
please [post](http://stackoverflow.com/questions/ask) a new one, tagging it with 'movesense' (i.e. include [tag:movesense] in the question body).  

All feedback is welcome and helps to improve our offering!  

## Contributions to the source code

Your input is appreciated and we encourage you to post your contributions as pull requests to this repository.

## License

See [LICENSE.pdf](LICENSE.pdf) for details on Movesense license.