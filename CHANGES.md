
##Version 0.3.1.105##

###Whats new:###
- Added Temperature provider
- Added Led provider
- Fixed "MagneticField provider exessive power use" bug
- Fixed "Battery level service return -1" bug
- Improved IDLE power consumption to ~100-150 uA without serial communication
- Added documentation sub folder with initia data logging and power optimization documents

###Known Bugs:###
- /Info returns wrong version number
- /../LinearAcceleration & /../AngularVelocity & /../MagneticField don't return valid data on GET, only SUBSCRIBE

###Notes:###
- REQUIRED: Use **GNU Toolchain for ARM v. 5.4 2016q3** 
- To create DFU package for OTA update, compile application with "ninja dfupkg"


##Version 0.3.0.86 / 2##
- Added wbo files that were lost in the merge. This fix connectivity that was broken since 31.March

##Version 0.3.0.86##

###Whats new:###
- Workaround for iOS BLE connection bug
- Large MTU and DLE support for BLE 4.2 connection
- Fixed ASSERT/Reset in PUT /Dev/Time caused by stack overflow by debug printout

###Known Bugs:###
- Magnetometer has been disabled for now to avoid accelerometer large current drain of v0.2.0.
- Battery level service returns -1

###Notes:###
- REQUIRED: Use **GNU Toolchain for ARM v. 5.4 2016q3** 
- To create DFU package for OTA update, compile application with "ninja dfupkg"


##Version 0.3.0.65##

###Whats new:###
- BLE 4.2 support (Not all features enabled yet)
- OTA Firmware update using Nordic Semiconductor's DFU library/apps
- Ability to change to boot mode (using PUT /Device/System/Mode  value=12)

###Known Bugs:###
- Magnetometer has been disabled for now to avoid accelerometer large current drain of v0.2.0.
- Battery level service returns -1
- iPhone BLE connection fails at device discovery

###Notes:###
- REQUIRED: Update toolchain to **GNU Toolchain for ARM v. 5.4 2016q3** 
- To create DFU package for OTA update, compile application with "ninja dfupkg"



##Version 0.2.0.42##

###Whats new:###
- Initial version of Movesense device core library

###Known Bugs:###
- Starting accelerometer causes large current drain.

###Notes:###
- Requires **GNU Toolchain for ARM v. 5.4 2016q2** to compile