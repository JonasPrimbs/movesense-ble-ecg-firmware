### How to identify which hardware variant the Movesense sensor is? ###

Under the battery on the PCB you can find a number with letter like "14451D". The last letter means the variant.

### Which HW variant is supported ###

Currently we support only the "G" variant.

### What's the difference between PowerOff and FullPowerOff modes (/System/Mode/) with regards to system clock (RTC) handling? ###

The difference is to keep the RTC running, but we do not support PowerOff mode yet.

### How to flash, i.e. update the software (firmware) on sensor? ###

There are two options:

1) With a mobile phone (Android, iPhone)
Generate the DFU (Device Firmware update) package by running the following command in the (sample) app directory:
> ninja dfupkg

Then send the resulting package ("movesense_dfu.zip") to your mobile phone via Bluetooth, for example. On Android, you can use our sample app to put the Movesense sensor into DFU mode and update its firmware. On iPhone, download Nordic Semiconductor's "nRF Connect" utility from App Store and use it to update the sensor firmware.

2)  With a flashing jig (available for purchase at movesense.com)
Once you have the jig properly set up, attach the sensor to it and run the following command in the (sample) app directory:
> ninja flash

### How to remove the manufacturing info data written to the sensor at factory? ###

This can only be done by connecting the sensor to the flashing jig (available for purchase at movesense.com)
Once you have the jig properly set up, attach the sensor to it and run the following command:
> ninja flash_all 

WARNING: This command will wipe all data on sensor, including settings.

### How to enable serial communication? ###

You can use the [Settings API](https://bitbucket.org/suunto/movesense-device-lib/src/master/MovesenseCoreLib/resources/movesense-api/system/settings.yaml) /System/Settings/UartOn

### Why does SERIAL_COMMUNICATION(true) not work? ###

The data from SERIAL_COMMUNICATION is saved to settings, but only if you use "flash_all" command.  
WARNING: This command will wipe all data on sensor, including settings.

### Why there are two ways of turning ON/OFF serial? ###

The main method for turning on serial is to use whiteboard command (Refer to _How to enable serial communication?_)
After you turn on serial this way your choise will be stored in non volatile Movesense settings.
```SERIAL_COMMUNICATION(true)``` is used for setup default value in application in case when manufacturing data on sensor settings are cleared by "flash_all" command.  
WARNING: Please refer to _Why does SERIAL_COMMUNICATION(true) not work?_ and _How to enable serial communication?_ Try to not use "flash_all" since it will wipe all manufacturing data without recovery option.

### How to setup environment? ###

The full instruction is in the [Readme.md](Readme.md) file and on our wiki.

### Which operating systems are supported? ###

Currently the best support is for Windows, but you can also use Linux and OSX.

### What is the difference Debug and Release variant? ###

Debug variant contains also RTT logs, but consumes more power and resurces.

### How to use see logs? ###

If you use Segger J-Link, you can download special application to read logs.

### How does the configuration of RTT Viewer look? ###

Connection: USB
Target: nRF52832_xxAA
Target Interface: SWD
Target speed: 1000kHz
RTT Control Block: Auto detection

### What is whiteboard? ###

Whiteboard is a special library created for the communication between devices.

### Why should I calculate on Movesense? ###

You should perform computations on Movesense because this allows to reduce amount of data transferred via Bluetooth, which is energy-consuming.
Here you can find a few examples of power consumption with BLE subscription and local one.
Subscription
|Name |	BT delta* |	Local delta*
|--|--|--
|ACC 13|	150uA|	15uA
|Gyro 13|	320uA|	188uA
|Magn 13|	290uA|	160uA

* - Delta means without const 100uA (Idle + BT connection)

### How to use DFU recovery mode? ###
   - Remove battery
   - Short circuit AFE pins and keep shorted (by fingers)
   - Plug battery
   - Wait since led start blinking (after 3 seconds of waiting)
   - Immediately release AFE pins to enter DFU mode (less than 1 second period)

### Some Movesense sample apps enable optional modules, what are they? ###

You can disable some parts of the Movesense to reduce memory and power consumption.

|Module |	Default status |	API
|--|--|--
|DataLogger| disabled| Mem/Dataloger.yaml
|Logbook| disabled| Mem/Logbook.yaml
|LedService| enabled| Component/led.yaml
|IndicationService| enabled| Ui/inf.yaml
|BleService| enabled| Comm/ble.yaml
|EepromService| disabled| Component/eeprom.yaml

### I'm experiencing data loss at high sampling frequencies, what's causing it? ###

Although Movesense sensor itself supports very high data sampling frequencies, Bluetooth transmission between the sensor and mobile device is often the bottleneck. 
In some cases this can even lead to data loss (lost Bluetooth packets), depending on the quality of Bluetooth stack and radio implementation on the mobile.
If such data loss occurs, the best strategy is to simply try to limit the amount of data transferred, either by lowering the data sampling rate or by implementing application level buffering mechanism.
You can notice that in some cases amount of sensor data copletely jams the sensor-moblie transfer especially during multi-subscription.  
You should take into consideration that the issue can happen when you are trying to send data from couple sensor subscriptions working on low frequecies or from one subscription for example on accelerometer but with very high frequency.  
Data amount limit depends also on mobile phone since mobiles uses different BT chipsets and stacks.

### Why battery level is higher after power off or reset? ###
### Why battery level is not changing very long time? ###

Movesense battery level estimation is based on measuring voltage of the battery which means that battery level values are reliable only when battery is almost discharged.  
(Please refer to CR2025 battery voltage to capacity characteristics). Battery level is not good for estimating working time or informing about battery capacity but it is usefull to notify that baterry is almost discharged and should be replaced in couple days. 

### Why LED is always turned on even after reset? ###

LED indicates that device is in DFU mode if stays turned on after boot (Device Firmware Upgrade mode)
There is possibility that firmware on device is broken due to lost BT connection during update or corrupted DFU package.
Use your phone with Movesense application to flash latest firmware to your Movesense sensor. There is also posibility that firmware version is not coresponding to bootloader version. Please check _MIGRATION.md_ file for more instructions.

### I cannot connect to Movesense sensor with my Android phone. What to do? ###

The issue is mainly caused by mobile phone Bluetooth stack cache.  
Some phones remembers BT specific settings and that leads to rejecting Movesens connection especialy after firmware update.  
You can try couple solutions for the problem:  

   - Turn off and on BT in your phone  
   - For some phones toggling BT is not enough - turn off and on phone  
   - If those two doesn't help - try to clear Bluetooth settings cache in application manageer in Android  

### I have turned off Bluetooth advertising in my own Movesense application. I cannot connect to the Movesense sensor any more. What to do? ###

There are two ways to recover from this situation.  

   - You can use jig and ST-Link to flash device with new application (Refer to _How to flash, i.e update the software (firmware) in sensor?_)  
   - You can use your phone and Movesense application to flash device with new firmware. Note that you can't connect to sensor using BT, and before you use phone you need to put sensor into DFU mode manually (Refer to _How to use DFU recovery mode?_)  

