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


