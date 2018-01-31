Movesense Sample applications README.
===================

![Movesense](http://ultramabouls.com/wp-content/uploads/2017/01/Movesense.jpg)

This README file explains how to use each of the sample applications.
----------

- [Accelerometer](#markdown-header-accelerometer-application)
- [BLE Conformance Test](#markdown-header-ble-conformance-test-application)
- [Blinky](#markdown-header-blinky-application)
- [Connection Scanner](#markdown-header-connection-scanner-application)
- [Hello World](#markdown-header-hello-world-application)
- [Jumpmeter](#markdown-header-jumpmeter-application)
- [Powermodes](#markdown-header-powermodes-application)
- [SensorOutputService](#markdown-header-sensoroutputservice-application)


# Connection Scanner application

Application description in general
-------------
The provided applications purpose is to detect what kind of device is connected to the Movesense.

Output uses blinks to show the result:
- GearID device connected: 2 blinks
- HR belt 4 blinks
- Unknown device (ex. short circuit with fingers) 5 blinks

In some scenarios you can wait more than 5 seconds on the result. See ConnectionScanner.h file to see how the sample works.


# SensorOutputService application

Application description in general
-------------
The provided applications purpose is to subscribe each sensor available in the Movesense device and verify its output.
The verification includes:

- Checking the range of sample timestamp
- Count the amount of received samples
- Search for gaps in the verified timestamps



Building and running
-------------
To properly build the provided application, the application needs to be specified while invoking the Cmake builder.
The name of the provided application used to configure the compilation is as follows:

- sens_output_test_app

So the full example command when using Cmake will be:

- cmake -G "Ninja" -DMOVESENSE_CORE_LIBRARY=../MovesenseCoreLib/ -DCMAKE_TOOLCHAIN_FILE=../MovesenseCoreLib/toolchain/gcc-nrf52.cmake ./sens_output_test_app

Of course the full description is available in the [README.md file](../README.md)



Application details and usage
-------------
The application comes with three use cases.

- Subcribe Request
- Put Request
- Get Request

#### Put ####
The first is to run the configuration, where the desired settings can be changed. This allows to set the sensor, sample rate and period of every verification.

#### Subscribe ####
The second use case is the subscription mode, where after each period the results will be displayed containing mentioned values: probesGood, probesBad, gaps
(which stands for verifying and correct samples, samples containing errors, gaps spotted in the verified timestamps).

#### Get ####
The third use case lets the user see current settings mentioned in the Put and Subscribe request.
Full description can be viewed in the proper .yaml file: [SensorsOutputSample](sens_output_test_app/wbresources/SensorsOutputSample.yaml)
