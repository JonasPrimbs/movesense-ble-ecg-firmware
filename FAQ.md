### How to check HW variant? ###

Under the battery on the PCB you can find a number with letter like "14451D". The last letter means the variant.

### Which HW variant is supported ###

Currently we support only the "G" variant.

### What's the difference between PowerOff and FullPowerOff modes (/System/Mode/) with regards to system clock (RTC) handling? ###

Due to a bug in our implementation, currently there's no difference in clock behaviour: neither of these modes maintains the clock. Once a fix to this issue is released, PowerOff mode properly handles system clock count.

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

### Is UartOn, how it works? ###

You can enable/disable the serial communication.

### Why does SERIAL_COMMUNICATION(true) not work? ###

The data from SERIAL_COMMUNICATION is saved to settings, but only if you use "flash_all" command.

### How to setup environment? ###

You can check the Movesense wiki.

### Which operating systems are supported? ###

Currently the best support is for Windows, but you can also use Linux and OSX.

### What is the difference Debug and Release variant? ###

Debug variant contains also RTT logs, but consumes more power.

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
You can disable some parts of the Movesense to reduce memory consumption.
