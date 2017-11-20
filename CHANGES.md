## Version 1.0.0.855.25 ##

###Whats new:###
- Fix: SBEM generator sometimes fails
- Fix: Full battery indicated as 60% in /System/Energy/Level
- Fix: Device doesn't start after firmware update (DFU)
- Fix: Gear ID does not work
- Fix: Magnetometer calibration and scaling (wrong units used)
- Fix: Gyroscope scaling (wrong units used)
- Fix: BLE advertising sometimes fails
- Fix: Long delay before /Meas/HR readings become available
- Power consumption improvements
- Memory consumption improvements
- DFU recovery mode (CHECK FAQ)
- Support for second EEPROM available in Movesense sensor G1 hardware variant
- API: New /Meas/IMU provider
- API: New /System/States provider
- API: New /Info/App path in info.yaml
- API: /Meas/Ecg contains sample rate (CHECK MIGRATION DOC)
- Samples: Some updates in build proces debug/release for samples.
- Samples: New BLE samples
- Workaround: Logbook/Datalogger break /Component/Eeprom data
      Disabled by default  (CHECK MIGRATION DOC)

###Known Bugs:###
- Please, check issues on bitbucket.
- Random reset when BLE connection drops if data transfer is ongoing
- Second EEPROM cannot write the data on 2 pages at the same time.
      To avoid the problem you should write data in one page (the page size is 256)
         addr / 256 == (addr + size) / 256
- DataLogger does not record */Meas/IMU* -API's
- GET on */Logbook/byId/{ID}/Data* ASSERT's if the log is empty

##Version 0.9.1.616 ##

###Whats new:###
- Fix: 15mA leak for the C version

###Known Bugs:###
- No changes from 0.9.0

###Notes:###
- REQUIRED: Use **GNU Toolchain for ARM v. 5.4 2016q3**
- To create DFU package for OTA update, compile application with "ninja dfupkg"

##Version 0.9.0.602 ##
DO NOT USE THIS ON THE C VERSION!!!

###Whats new:###
- Samples sens_output_test_app, hr_wakeup_app
- Cmake files for samples
- Release variant of the movesense library
- Whiteboard 3.4.1
- Blinky_app sample uses Ind API
- Power and memory optimalizations
- New HW support
- Eeprom provider: /Component/Eeprom
- Fix: Wakeup for max3000x
- Fix: Uart settings problem
- Fix: BLE Adv. too short
- Fix: Battery level adv. is not working
- Fix: Magnetometer's not working correctly

###Known Bugs:###
- Magnetometer value needs scaling and unit
- Detecting of the gear id is not working if is subscribed
- Power performance needs additional work

###Notes:###
- REQUIRED: Use **GNU Toolchain for ARM v. 5.4 2016q3**
- To create DFU package for OTA update, compile application with "ninja dfupkg"

##Version 0.6.0.426 ##

###Whats new:###
- Fixed: Leaks after disconnect without unsubscribe command
- ECG provider
- New whiteboard 3.3

###Known Bugs:###
- Maximum subscription rate is only 104Hz for Acc/Gyro/Magn (old HW)
- Lost some values for Acc and Gyro

###Notes:###
- REQUIRED: Use **GNU Toolchain for ARM v. 5.4 2016q3**
- To create DFU package for OTA update, compile application with "ninja dfupkg"

##Version 0.5.2.384 ##

###Whats new:###
- Fixed: Missing support for magnetometer - LIS2MDL (only new HW)

###Known Bugs:###
- Leaks after disconnect without unsubscribe command
- Maximum subscription rate is only 104Hz for Acc/Gyro/Magn (old HW)
- Lost some values for Acc and Gyro

###Notes:###
- REQUIRED: Use **GNU Toolchain for ARM v. 5.4 2016q3**
- To create DFU package for OTA update, compile application with "ninja dfupkg"

##Version 0.5.1.372 ##

###Whats new:###
- New API: /Ui/Ind
- New driver for LSM6DSL
- Fixed: Acc and Gyro subscribed in pair and with that order return wrong data.

###Known Bugs:###
- Leaks after disconnect without unsubscribe command
- Maximum subscription rate is only 104Hz for Acc/Gyro/Magn
- Missing support for magnetometer - LIS2MDL (only new HW)
- Lost some values for Acc and Gyro

###Notes:###
- REQUIRED: Use **GNU Toolchain for ARM v. 5.4 2016q3**
- To create DFU package for OTA update, compile application with "ninja dfupkg"

##Version 0.5.0.341##

###Whats new:###
- Changed API: /Info, /Meas/Temp, /Mem/DataLogger, /Mem/Logbook, /System/Energy, /System/Mode, /System/Settings, /Time
- New API: /Comm/Ble, /Meas/HR, /Misc/Gear
- Fixed: Logbook: DataLogger stores (or Logbook returns) wrong data if more than one path is specified in /DataLogger/Config
- Fixed: DataLogger only stores the first element of an array (e.g. for /Meas/Acc/xxx it stores the 1st measurement in the AccData-package)

###Known Bugs:###
- Leaks after disconnect without unsubscribe command
- Maximum subscription rate for Acc/Gyro/Magn is only 104Hz
- Acc and Gyro subscribed in pair and with that order return wrong data.

###Notes:###
- REQUIRED: Use **GNU Toolchain for ARM v. 5.4 2016q3**
- To create DFU package for OTA update, compile application with "ninja dfupkg"

##Version 0.4.1.184##

###Whats new:###
- Fixed: DataLogger for new Accelerometer, Gyroscope & Magnetometer APIs
- Fixed: Accelerometer/Gyroscope/Magnetometer lag (start with delay) when subscribing a second time
- Fixed: /Info returns wrong version number

###Known Bugs:###
- Leaks after disconnect without unsubscribe command
- DataLogger stores (or Logbook returns) wrong data if more than one path is specified in /DataLogger/Config
- DataLogger only stores the first element of an array (e.g. for /Meas/Acc/xxx it stores the 1st measurement in the AccData-package)

###Notes:###
- REQUIRED: Use **GNU Toolchain for ARM v. 5.4 2016q3** 
- To create DFU package for OTA update, compile application with "ninja dfupkg"

##Version 0.4.0.148##

###Whats new:###
- The new measurement provider for Acc/Gyro/Magn with sample rate and range support
- New samples JumpMeter and PowerMode
- Initial implementation of wakeup from poweroff with HR lead connect
- Power off functionality (PUT value 1 to /Device/System/Mode)

###Known Bugs:###
- The changes in Sensor API's mean that for now the DataLogger is unusable for logging sensor data. We will release fix soon that addresses this issue
- Accelerometer/Gyroscope/Magnetometer lag (start with delay) when subscribing a second time
- Leaks after disconnect without unsubscribe command

###Notes:###
- REQUIRED: Use **GNU Toolchain for ARM v. 5.4 2016q3** 
- To create DFU package for OTA update, compile application with "ninja dfupkg"

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