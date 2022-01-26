## Version 2.1.2rc1.c9ef0545b42d82 ##

### Whats new:
- Bugfix: High current after DFU and poweroff in some custom firmwares
- Bugfix: CMake fails if source-only modules are used

### Known Bugs:
- /Comm/Ble/Scan only returns one result (since 2.0)

### Notes:
- This will be a combined medical and sport release. For medical clients, please download the 
default firmware and library from the [medical releases repository](https://bitbucket.org/suunto/movesense-medical-sw).

## Version 2.1.1.cd637625e009 ##

### Whats new:
- Bugfix: No advertising after disconnect of bonded connection

### Known Bugs:
- /Comm/Ble/Scan only returns one result (since 2.0)

### Notes:
- Medical release was postponed to 2.1.2

## Version 2.1.0.acccfdce5ef4d9 ##

### Whats new:
- Feature: Improved Logbook GET speed
- Feature: Support for flash mem HW
- Feature: Support HW-variants with no EEPROMs
- Feature: Set size property to Logbook/Entries
- Feature: ECG/HR LeadsOff detection
- Feature: Per-product & Per-HW bootloaders
- Feature: Binary releases of 3rd party modules
- Feature: Filter settings to ECG API
####
- Bugfix: Random Bad Logbook Entry timestamp
- Bugfix: Gyro doesnt accept 125 dps config
- Bugfix: DFU mode switch via ble returning bad gateway instead of accepted
- Bugfix: Timestamp jitter on ECG
- Bugfix: Slow Logbook/Entries with many logs
- Bugfix: DataLogger returns json log with just one value (Mobile-lib issue #86)
- Bugfix: Timestamps jump backward on ECG error case
####
- Technical: RTT log cleanup
- Technical: Updated fixed Nordic command line tools in build env docker (mergehex issue)


### Known Bugs:
- /Comm/Ble/Scan only returns one result (since 2.0)


### Notes:
- Medical release was postponed to 2.1.2

## Version 2.0.0.faefabe6891488 ##

### Whats new:
- Feature: BLE5 support (nRF SoftDevice v6.1.1)
- Feature: Docker based build system for all environments (Issue #82)
- Feature: Improved power-off logic of wakeup samples
- Feature: DataLogger flush
- Feature: 128bit UUID support for CustomGATTService
- Feature: Array and datatype configuration for DataLogger
- Feature: New sample gatt_sensordata_app for datastreams over GATT
- Feature: Watchdog support (protects from stuck sensors, reset after 30seconds)
- Feature: New nice HRS and NUS BLE modules
####
- Bugfix: App build, no ASSERT file path, only file name
- Bugfix: Datalogger does not log ECG data
- Bugfix: Faultcom reset reason sometimes gives wrong data
- Bugfix: BLE Advertising starts too early
- Bugfix: BLE service_changed called at wrong time
- Bugfix: Sensor stuck if /misc/gear/id read fails
- Bugfix: DataLogger writes invalid chunk
- Bugfix: BLE/Adv doesn't support Non-Connectable (Issue #72)
- Bugfix: OTA DFU fails for large apps (Issue #55)
####
- Technical: Change to 16bit Whiteboard ServiceUUID
- Technical: Enabled "warnings as errors" in app builds
- Technical: Much improved bootloader (blink during update, 6x speed, reset to app after timeout)

### Known Bugs:
- /Meas/IMU/Info gives invalid magnetometer scalings
- Slow /Logbook/Entries with many logs
- /Meas/ECG/info gives wrong current sample rate
- Simulator has wrong speed and sporadic connetion issues
- [DOC] Api default time when time is not explicitly set (Issue #45)


### Notes:

**This is a major update to Movesense core library. Practically everything has changed since 1.9.x**

 - BLE stack (SoftDevice)
 - Bootloader
 - Nordics SDK
 - Compiler environment and toolchains
 
As a result extra care should be taken to make sure that your application works correctly. **Please read
carefully the documentation in https://www.movesense.com/docs ** and test your sensor app thoroughly when moving to 2.0.

#### About DFU update:

Since the bootloader and softdevice have changed the DFU from older sensors **have to be performed** using the 
 *xxxxxxx_w_bootloader.zip* DFU package. If the sensor already has 2.0 firmware the smaller DFU package 
 can be used. 
 
 **Reverting to older version is not possible using OTA update.**

#### Flashing the sensor using JIG:

Flashing system was changed so that now the only file that needs/must to be flashed is the *Movesense_combined.hex*. 
Partial flashing will not work in most cases. For more read the documentation.



## Version 1.9.4.57e7d2e4fcbe

### Whats new:
- Bugfix: Sensor gets stuck randomly for 1:37h 

###Known Bugs:###
- [OSX] App build fails if path has space
- [OSX] GNU Toolchain for ARM 2017q4 on Mac has a bug that prevents builds

###Notes:###
- REQUIRED: Use **Windows** build environment only (due to above GNU OSX bug)
- REQUIRED: Use **GNU Toolchain for ARM v. 7.2.1 2017q4**

## Version 1.9.3.35a1deb37c0987

### Whats new:
- Bugfix: Too high current consumption on some sensors when not using Acc/IMU

### Known Bugs:
- [OSX] App build fails if path has space
- [OSX] GNU Toolchain for ARM 2017q4 on Mac has a bug that prevents builds

### Notes:
- REQUIRED: Use **Windows** build environment only (due to above GNU OSX bug)
- REQUIRED: Use **GNU Toolchain for ARM v. 7.2.1 2017q4**

## Version 1.9.2.64280c34992f07

### Whats new:
- Bugfix: /Meas/HR high current consumption

### Known Bugs:
- [OSX] App build fails if path has space
- [OSX] GNU Toolchain for ARM 2017q4 on Mac has a bug that prevents builds

### Notes:
- REQUIRED: Use **Windows** build environment only (due to above GNU OSX bug)
- REQUIRED: Use **GNU Toolchain for ARM v. 7.2.1 2017q4**

## Version 1.9.1.a1943b29485341

### Whats new:
- Bugfix: Writing to EEPROM sometimes fails under heavy load and concurrent reads
- Bugfix: /Meas/ECG sometimes stops sending data updates under heavy load


### Known Bugs:
- [OSX] App build fails if path has space
- [OSX] GNU Toolchain for ARM 2017q4 on Mac has a bug that prevents builds

### Notes:
- REQUIRED: Use **Windows** build environment only (due to above GNU OSX bug)
- REQUIRED: Use **GNU Toolchain for ARM v. 7.2.1 2017q4**

## Version 1.9.0.8e23b279009f7

### Whats new:
- Feature: Double tap parameters for LSM6DSx component
- Feature: Movement wakeup sample (movement_wakeup_app)
- Feature: 1-Wire standard speed support
- Feature: 1-Wire thermometer sample (1wire_temperature_app)
- Feature: /Mem/Logbook/IsFull implementation

- Bugfix: Movement wakeup doesn't work
- Bugfix: Bugfix: UART ON return value missing
- Bugfix: slow system shutdown on hardware
- Bugfix: Build creates too large hex files (Issue #53)
- Bugfix: Subscribe ECG / HR + Connector doesn't work (Issue #47)
- Bugfix: accelerometer timestamp jitter correction
- Bugfix: /System/Debug/Log API can't return entries over reset
- Bugfix: ble_std_app has wrong scan name
- Bugfix: SBEM generation dependency fix
- Bugfix: Logbook/../Data GET in infinite loop
- Bugfix: Logbook entry timestamps are wrong
- Bugfix: Logbook returns invalid SBEM

- Technical: Updated to FreeRTOS v10
- Technical: Enabled LTO option on builds
- Technical: Fix type check issues in response handling
- Technical: Support nrfutil >=4 
- Technical: Sample app fixes and clean up


### Known Bugs:
- [OSX] App build fails if path has space
- [OSX] GNU Toolchain for ARM 2017q4 on Mac has a bug that prevents builds

### Notes:
- REQUIRED: Use **GNU Toolchain for ARM v. 7.2.1 2017q4**

## Version 1.8.1.34a9fd4453016b

### Whats new:
- Bugfix: Fixed BLE issues with some phone models
- Bugfix: Fixed slow metadata fetch on first connection
- Bugfix: 1Wire command doesn't work from MDS
- Bugfix: samples ble_std_services and secure_ble_app wont start (Issue #50)

### Known Bugs:
- OTA Firmware update fails if app size is over 339968 bytes

### Notes:
- To create DFU package for OTA update, compile application with "ninja dfupkg"

## Version 1.8.0.1986.3e4686edc878fc  

### Whats new  
- Feature: BLE power consumption improvements 
- Feature: BLE speed improvements for BLE 4.2 and newer phones 
- Feature: Get /Algo/ECGRR when subscribing to /Meas/HR when rr_from_ecg module included in build
- Feature: 1-Wire communication API (only overdrive-speed supported for now)
- Feature: HR initial filtering
- Bugfix: In heavy BLE traffic accelerometer timestamps may have jitter 
- Bugfix: BLE deviceName does not match AdvName for customized ScanResp
- Bugfix: Gear ID is not correct
- Bugfix: Sensor goes sometimes to 5mA mode
- Bugfix: SBEM code not generated for modules (Issue #46)
- Bugfix: Connect after Disconnect fails sometimes
- Sample: New *accel_int16_app* that provides accelerometer data as int16

### Notes:  
- Subscribing higher ECG samplerates (> 200) can lead to loss of data due to small on chip FIFO on AFE chip (especially when heavy BLE traffic)

## Version 1.7.2.1932.31fa1076f86ebc  

### Whats new:  
- Bugfix: /System/States:Movement doesn't work without accelerometer subscription  
- Bugfix: /System/States:Connector doesn't work 100% (Issue #44)  

### Known Bugs:  
- In heavy BLE traffic accelerometer timestamps may have jitter  

## Version 1.7.1.1901.cd8cac83fb55d7

### Whats new:
- Bugfix: Samsung S7-S9 cannot find sensor in scan after disconnect

### Known Bugs:
- In heavy BLE traffic accelerometer timestamps may have jitter

### Notes:
- ECG over BLE datarate is close to max data bandwidth when subscribing samplerates 500 & 512. This can lead to loss of data due to small on chip FIFO on AFE chip.
- Build system has been changed quite a bit so please copy the new style CMakeLists.txt-file from one of the sample apps to your own project folder!

## Version 1.7.0.1820.f1520cad7c8107

### Whats new:

- Update: Whiteboard 3.16
- Feature: PC Simulator support (see: README.md)
- Feature: Framework for including 3rd party modules
- Feature: New /Time/Detailed resource for getting UTC and RelativeTime
- Bugfix: BLE Advertise name and GAP device name mismatch
- Bugfix: Shutdown fails sometimes
- Bugfix: Accel values becomes invalid after range change
- Bugfix: Fixed RR-description in hr.yaml API
- Bugfix: Some SystemStates issues

### Known Bugs:
- In heavy BLE traffic accelerometer timestamps may have jitter

### Notes:
- ECG over BLE datarate is close to max data bandwidth when subscribing samplerates 500 & 512. This can lead to loss of data due to small on chip FIFO on AFE chip.
- Build system has been changed quite a bit so please copy the new style CMakeLists.txt-file from one of the sample apps to your own project folder!

## Version 1.6.2.1664.34ea980127698d

### Whats new:
- Bugfix: ECG stuck in high sample-rates over BLE

### Known Bugs:

- Standard BLE battery profile does not send new values
- The first package of Acc/Magn/Gyro can be corrupted for high sample rates
- be sure to also check Bitbucket for any additional reported issues: [device library](https://bitbucket.org/suunto/movesense-device-lib/issues?status=new&status=open) and [mobile library](https://bitbucket.org/suunto/movesense-mobile-lib/issues?status=new&status=open)

### Notes:
- ECG over BLE datarate is close to max data bandwidth when subscribing samplerates 500 & 512. This can lead to loss of data due to small on chip FIFO on AFE chip. 


## Version 1.6.1.1655.ec1bba61bb8493 ##


### Whats new:

- Feature: Store RTC over reset (accuracy about +-1 second after reset)
- Feature: Added function faultcom_GetLastFaultStr() (See: *movesense.h*) to receive string to describe error/reason of last reset
- Bugfix: HR & ECG signal stops coming after a while
- Bugfix: consecutive ECG subscriptions fail with NOT_IMPLEMENTED
- Bugfix: Sometimes onUnsubscribe fails for /Meas/ECG
- Bugfix: accumulated RR-time drifts if ECG is subscribed on some samplerates 
- Bugfix: Issue in tickless implementation which lead to busy loop
- Bugfix: /Mem/Logbook/Entries only returns 4 entries
- Bugfix: /Mem/Logbook/Entries contain weird timestamps
- Deprecated: SERIAL_COMMUNICATION macro is no longer needed


### Known Bugs:

- Standard BLE battery profile does not send new values
- The first package of Acc/Magn/Gyro can be corrupted for high sample rates
- be sure to also check Bitbucket for any additional reported issues: [device library](https://bitbucket.org/suunto/movesense-device-lib/issues?status=new&status=open) and [mobile library](https://bitbucket.org/suunto/movesense-mobile-lib/issues?status=new&status=open)


## Version 1.6.0.1582.25d038d702bd99 ##
 
###Whats new:###
 
- Update: Whiteboard 3.15
- 20kB extra flash
- HR filtering
- Battery measurement improvements
- Timestamp added to ECG data
- Experimental: Custom GATT service
- Experimental: DebugService storage (EEPROM)
- Experimental: New system states (Free fall/Double tap/Tap)
- Bugfix: Leak of the GearID subscription
- BugFix: Battery and HRS BLE profile don�t pass Bluetooth SIG tests
- Bugfix: BLE scan does not return rssi
 
###Known Bugs:###

- HR/RR/ECG service sometimes stops sending data
- Standard BLE battery profile does not send new values
- The first package of Acc/Magn/Gyro can be corrupted for high sample rates
- be sure to also check Bitbucket for any additional reported issues: [device library](https://bitbucket.org/suunto/movesense-device-lib/issues?status=new&status=open) and [mobile library](https://bitbucket.org/suunto/movesense-mobile-lib/issues?status=new&status=open)
 
## Version 1.5.1.1516.59da9cbf7fca ##

###Whats new:###
- Bugfix: Increased power consumption after BLE disconnect
- Bugfix: GearID subscription asserts on disconnect
- Bugfix: Avoid AFE shutdown on Whiteboard disconnect when AFE is used as 1-Wire detector
- Bugfix: DS24L65 is active after GET
- Bugfix: Gear id cannot be read

###Known Bugs:###
- The first package of Acc/Magn/Gyro can be corrupted for high sample rates
- Battery measurement returns wrong value
- asyncSubscriptions with parameters get all data without filtration (Check migration file)
- be sure to also check Bitbucket for any additional reported issues: [device library](https://bitbucket.org/suunto/movesense-device-lib/issues?status=new&status=open) and [mobile library](https://bitbucket.org/suunto/movesense-mobile-lib/issues?status=new&status=open)

## Version 1.5.0.1422.0e9d9cde6126 ##

###Whats new:###
- Debug service optimizations
- States service optimizations
- DebugService message buffering
- Movesense logbook streams with offset param
- New solution for battery measurement.
- State movement based on the interrupt.
- Experimental: Secure BLE
- Bugfix: Second subscription for Meas/Hr not working
- Bugfix: Recovery mode is not working
- Bugfix: NRF port - fixed delayed and dropped BLE sends when multiple sends were attempted at the same time. 
- Bugfix: Belt detection is not working after disconnect
- Workaround: Samples: getResource/releaseResource for asyncSubscriptions with parameter

###Known Bugs:###
- The first package of Acc/Magn/Gyro can be corrupted for high sample rates
- asyncSubscriptions with parameters get all data without filtration (Check migration file)
- be sure to also check Bitbucket for any additional reported issues: [device library](https://bitbucket.org/suunto/movesense-device-lib/issues?status=new&status=open) and [mobile library](https://bitbucket.org/suunto/movesense-mobile-lib/issues?status=new&status=open)


## Version 1.4.1.1355.e503f45eed71 ##

###Whats new:###
- Fix: Hard fault handler failing due to wrong linker settings

###Known Bugs:###
- The first package of Acc/Magn/Gyro can be corrupted for high sample rates
- be sure to also check Bitbucket for any additional reported issues: [device library](https://bitbucket.org/suunto/movesense-device-lib/issues?status=new&status=open) and [mobile library](https://bitbucket.org/suunto/movesense-mobile-lib/issues?status=new&status=open)

## Version 1.4.0.1312.8e3f8c14e60a ##

###Whats new:###
- New: /System/Memory API provider
- New: /System/Debug API provider (experimental)
- New: BLE Peer and Scan API (experimental)
- Datalogger and logbook improvements
- Fix: Connectivity problems
- Fix: HR stops sending data after Acc/Magn/Gyro subscription
- Fix: DataLogger does not record */Meas/IMU* API's
- Fix: Clock inaccuracy
- Fix: Missing modules in /Info/App provider
- Fix: Battery level got stuck to lowest reported value

###Known Bugs:###
- The first package of Acc/Magn/Gyro can be corrupted for high sample rates
- be sure to also check Bitbucket for any additional reported issues: [device library](https://bitbucket.org/suunto/movesense-device-lib/issues?status=new&status=open) and [mobile library](https://bitbucket.org/suunto/movesense-mobile-lib/issues?status=new&status=open)

## Version 1.3.0.1216.3d263fcc962d ##

###Important
Starting from firmware release 1.2.0, each sensor sample app now generates two DFU files (with and without bootloader and SoftDevice). If the sensor firmware is updated using the package that contains new bootloader and SoftDevice ('movesense_dfu_w_bootloader.zip'), we recommend to either reset Bluetooth on mobile device or reboot the mobile device to avoid reconnection problems with the updated sensor. This applies to both iOS and Android.  

Additionally, if the Movesense sensor has been paired in mobile device's Bluetooth settings, pairing will have to be redone on mobile device side.  

###Whats new:###
- New: API System/States for connector (please, see the new sample app)
- New: Sample "connection scanner app". [See full description](samples/README.md)
- Update: Whiteboard 3.11
- Fix: Gear detection fixes

###Known Bugs:###
- The first package of Acc/Magn/Gyro can be corrupted for high sample rates
- HR stops sending data after Acc/Magn/Gyro subscription
- DataLogger does not record */Meas/IMU* -API's
- be sure to also check Bitbucket for any additional reported issues: [device library](https://bitbucket.org/suunto/movesense-device-lib/issues?status=new&status=open) and [mobile library](https://bitbucket.org/suunto/movesense-mobile-lib/issues?status=new&status=open)


## Version 1.2.0.1182.2689dce322ff ##

###Whats new:###
- IMPORTANT! SoftDevice upgraded to version 4.0.5 (Please review [MIGRATION.md](MIGRATION.md))
- Each sensor sample app now generates two DFU files (with and without bootloader and SoftDevice)  
    - Note: DFU package containing the bootloader ('movesense_dfu_w_bootloader.zip') is applied two times when firmware update is performed using Movesense Sample App on mobile, i.e. the update process is splitted into two stages  
    - Note: After the firmware update process is complete, we recommend to either reset Bluetooth on mobile or reboot the mobile device to avoid reconnection problems with the updated sensor. This applies to both iOS and Android    
- Update: Component/LED is using new API
- Update: Portable-python update
- Fix: Wrong SBEM version
- Fix: Logbook ID is always 1
- Fix: Potential overflow in timestamps
- Fix: BLEadv: get wrong data
- Fix: HR doesn't send RR value after long delay

###Known Bugs:###
- The first package of Acc/Magn/Gyro can be corrupted at high sample rates
- HR stops sending data after Acc/Magn/Gyro subscription
- be sure to also check Bitbucket for any additional reported issues: [device library](https://bitbucket.org/suunto/movesense-device-lib/issues?status=new&status=open) and [mobile library](https://bitbucket.org/suunto/movesense-mobile-lib/issues?status=new&status=open)

## Version 1.1.0.1058.32 ##

###Whats new:###
- Movesense memory optimizations
- ByPass service as additional module
- Battery provider improvements
- Low battery warning
- New Ble optional modules: BleStandardHRS and BleNordicUART
- Whiteboard update to version 3.9.0
- New samples: custom_bleadv_app and ble_std_services

###Known Bugs:###
- Timestamp sometimes is reseted after subscription Acc/Gyro/Magn
- The first package of Acc/Magn/Gyro can be corrupted for high sample rates


## Version 1.0.2.914.29 ##

###Whats new:###
- Fix: Random reset when BLE connection drops if data transfer is ongoing

###Known Bugs:###
- Please, check issues on bitbucket.
- Second subscription fails on _/Meas/ECG/{SampleRate}_ with _NOT_IMPLEMENTED_


## Version 1.0.1.894.28 ##

###Whats new:###
- Fix: DataLogger does not record */Meas/IMU* -API's
- Fix: Second EEPROM cannot write the data on 2 pages at the same time.
- Logbook uses 2 EEPROMs
- Logbook size can be changed "LOGBOOK_MEMORY_AREA(offset, size);"

###Known Bugs:###
- Please, check issues on bitbucket.
- GET on */Logbook/byId/{ID}/Data* ASSERT's if the log is empty


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
- Support for the second EEPROM available in Movesense sensor G1 hardware variant
- API: New /Meas/IMU provider
- API: New /System/States provider
- API: New /Info/App path in info.yaml
- API: /Meas/Ecg contains sample rate (CHECK MIGRATION DOC)
- Samples: Some updates in the build proces debug/release for samples.
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
DO NOT USE THIS ON THE C HARDWARE VARIANT!!!

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
- Fix: UART settings problem
- Fix: BLE Adv. too short
- Fix: Battery level adv. does not work
- Fix: Magnetometer does not work correctly

###Known Bugs:###
- Magnetometer value needs scaling and unit
- Detecting of the gear id is not working if is subscribed
- Power performance needs the additional work

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
