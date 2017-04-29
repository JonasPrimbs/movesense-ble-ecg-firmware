# Movesense Sensor Power Optimization #

This document describes the power saving features of the Movesense sensor. 

## Overview ##

The Movesense sensor core software architecture has been designed to be asynchronous and event based. That naturally results good results for power saving in normal operation. If there is no subscriptions for sensors for example the sensor is put to "power off" mode, and it will be re-enabled as soon as there is someone (either internally or over Whiteboard communication) who wants to get the sensor data.  While the basic structure is sound for power saving, there are a few things that must be taken into account to achieve good power consumption figures. 

_Note: as of writing this some of the measurements have not been completed yet, so some of the information is not available yet._

## Features and their power use ##

The movesense core library contains a lot of different features and their power consumption varies. Some don't use any extra power when IDLE, while others consume quite a bit. Here are some of the most important aspects and features and how they affect the power use (Note: All the given power figures are based in the current Movesense software and hardware and situation with them can change in the future)

### UART / Serial-communication ###

The Movesense sensor supports serial communication via the debug pins on the PCB. These can be used with the Movesense programming JIG and it shows as a normal USB serial port. However the UART is _very_ power hungry even when not in use (about 1.2mA when IDLE), so it makes sense to disable it as soon as you don't need it. 

To disable it just change 
```    SERIAL_COMMUNICATION(true)```
to
```    SERIAL_COMMUNICATION(false)```

in your applications _App.cpp_ file.


### RTT debug log messages ###


The Movesense software stack is configured to enable debug logging / tracing over RTT debug lines in the programming JIG. 

*TBD*


### BLE Advertising ###

*TBD*

## Sensors ##

Also different sensors use a different amount of power. Here're some ballpark figures on the power use:

- *Accelerometer:* (resource: _/Device/Measurement/LineaAcceleration_)  100uA
- *Gyroscope:* (resource: _/Device/Measurement/AngularVelocity_)  1 mA
- *Magnetometer:* (resource: _/Device/Measurement/MagneticField_)  200 uA
- *Thermometer:* (resource: _/Device/Measurement/Temperature_)  *TBD*
- *Heartrate sensor:* *TBD*

