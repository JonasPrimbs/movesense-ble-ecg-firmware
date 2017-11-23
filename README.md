## Announcements ##
**API changes**  
Movesense API has received several updates from its earlier versions. Please review the latest [API definition](https://bitbucket.org/suunto/movesense-device-lib/src/master/MovesenseCoreLib/resources/movesense-api/) and modify your existing code accordingly, if needed.

**Known issues**  
Please refer to [CHANGES.md](CHANGES.md) for a list of known issues.

**Testing coverage**  
Starting from release 1.0.0, Movesense device library will internally be tested only on the latest Movesense sensor hardware version ("G1" variant) -- please refer to the [FAQ section](FAQ.md) for instructions on how to identify which hardware version you have.  

If you're experiencing issues with any earlier hardware version and would like to upgrade to the newest sensor version available, please contact the Movesense team via [movesense.com](https://movesense.com).

## Overview ##

[Movesense sensor](https://www.movesense.com/developers/technology/#developers) is a programmable multi-purpose device comprising of accelerometer, gyroscope, magnetometer and thermometer together with optional heartrate/IBI (inter-beat interval) and intelligent gear ID APIs.  

Movesense APIs follow well-known REST principle and can be used both internally by the sensor app or externally via the iOS/Android libraries. The [API specification](https://bitbucket.org/suunto/movesense-device-lib/src/master/MovesenseCoreLib/resources/movesense-api/) is based on Swagger 2.0 syntax.

You can order Movesense sensors and Movesense Developer Kit on our online [shop](https://www.movesense.com/shop/).

Check also [Movesense mobile library](https://bitbucket.org/suunto/movesense-mobile-lib) for developing own Android and iOS applications to interact with the Movesense sensor. **Note:** Access to mobile libraries repository on Bitbucket is currently restricted. Please contact us at [movesense.com](https://movesense.com) for access.

## Documentation ##

Movesense developer documentation is divided in several documents:

[README.md](README.md): this document, with build environment setup instructions and other important information.  
[CHANGES.md](CHANGES.md): version history of Movesense releases.  
[MIGRATION.md](MIGRATION.md): instructions on how to upgrade from earlier versions of the Movesense device library.  
[FAQ.md](FAQ.md): list of frequently asked questions.  
[Movesense developer guides](https://bitbucket.org/suunto/movesense-docs/wiki/Home) in movesense-docs repository on Bitbucket.  

## Movesense APIs ##
Resource | Description|Implemented
---------|------------|--------------
/Comm/Ble|API for managing BLE.  
/Component/Eeprom|API for writing and reading the EEPROM memory/ies. 
/Info|API for accessing generic device information.
/Meas/Acc|API enabling subscribing linear acceleration data (based on accelerometer).
/Meas/ECG|API for the electrocardiography measurement.
/Meas/Gyro|API enabling subscribing angular velocity data (based on gyroscope).
/Meas/HR|API enabling subscribing heart rate data.
/Meas/Magn|API enabling subscribing magnetic field data (based on magnetometer).
/Meas/Temp|API enabling reading or subscribing temperature data (based on thermometer).
/Mem/DataLogger|Generic logger capable of logging max. 8 different resources.
/Mem/Logbook|Generic Logbook from where the logged data can be read.
/Misc/Gear| API to get the globally unique Movesense ID associated with the physical gear.
/System/Energy|API for reading the battery state.
/System/Mode|API for controlling the main system state (e.g. factory sleep).
/System/Settings| Settings API for a Movesense device.
/Time|API for accessing different time related services.
/Ui/Ind|API for controlling the LED.

## Setting up the development environment  

This section gives platform specific instructions on how to set up Movesense development environment. If you run into problems or need to install additional components
to your system to get the environment properly set up, please let us know by [submitting an issue](https://bitbucket.org/suunto/movesense-device-lib/issues/new).

There is an experimental installation procedure using Vagrant to set up an isolated environment for development ([check it out below](#markdown-header-automated-setup-on-all-oses-using-vagrant)), or you can follow the manual installation instructions below.

### Manual setup on Windows ###
Install the following tools: 

* [GNU Toolchain for ARM Embedded 6-2017-q2](https://developer.arm.com/open-source/gnu-toolchain/gnu-rm/downloads)
    * Remember to add it to your system path by ticking the checkbox during the last step of installation.
* [Ninja build tool](https://ninja-build.org/) 
    * Download the file 
    * place ninja.exe file to C:/bin
    * Add C:/bin to system settings (PATH variable)
* [cmake >=3.6](https://cmake.org/download/)
    * Download cmake-3.8.2-win64-x64.zip
        * Extract the file
        * Add the binary directory (containing cmake.exe) to the system settings (PATH variable)
* [nrfutil package & python 2.7](https://github.com/NordicSemiconductor/pc-nrfutil) for OTA firmware update package creation
    * Nordic provides pre-compiled Windows executable
* [Visual Studio Redistributable 2015](https://www.microsoft.com/en-us/download/details.aspx?id=48145)

Optionally: 

* For flashing with Movesense programming JIG: 
    * Nordic Semiconductor's [nRF5x-Command-Line-Tools-XXX](https://www.nordicsemi.com/eng/Products/Bluetooth-low-energy/nRF52-DK)
* In case you encounter `ImportError: No module named yaml`, try to install pyyaml package using pip.

### Manual setup on OSX ###
Install the following tools: 

* [GNU Toolchain for ARM Embedded 6-2017-q2](https://developer.arm.com/open-source/gnu-toolchain/gnu-rm/downloads)
* [Ninja build tool](https://ninja-build.org/) 
* [cmake >=3.6](https://cmake.org/download/)
    * Download Mac OSX libraries and follow installation instructions
* [pip](https://pip.pypa.io/en/stable/) for managing Python packages: `sudo easy_install pip` and then nrfutil: `sudo pip install nrfutil`. 
If the pip installation fails, try running command `sudo pip install nrfutil --ignore-installed six`.
* [nrfutil package & python 2.7](https://github.com/NordicSemiconductor/pc-nrfutil) for OTA firmware update package creation

Optionally:  

* For flashing with Movesense programming JIG:  
    * Nordic Semiconductor's [nRF5x-Command-Line-Tools-XXX](https://www.nordicsemi.com/eng/Products/Bluetooth-low-energy/nRF52-DK)  
    * Segger's [J-Link Software and Documentation pack](https://www.segger.com/downloads/jlink/) for Mac OSX
    * In case you encounter `ImportError: No module named yaml`, try to install pyyaml package using command `sudo pip install pyyaml`. 
 
Path configuration:  

* Remember to add ninja, CMake, GNU Toolchain for ARM, nrfjprog and mergehex (provided with nrfjprog) to system path by editing the PATH variable in .bash_profile.

### Manual setup on Linux ###
Installation steps for Ubuntu 17.10:

* Install dependencies

    >sudo apt install git cmake ninja-build python libc++1 python-pip

* Install the toolchain

    a) Installing from PPA
    
        >sudo add-apt-repository ppa:team-gcc-arm-embedded/ppa
        
        >sudo apt-get update
        
        >sudo apt-get install gcc-arm-embedded

   b) Installing from tar.gz
       TBD

* Install python dependencies

    > sudo pip install nrfutil pyyaml
    
* Clone repository

    > git clone https://bitbucket.org/suunto/movesense-device-lib.git

* Building

    > cd movesense-device-lib
    
    > mkdir buildDir
    
    > cd buildDir
    
    > cmake -G Ninja -DMOVESENSE_CORE_LIBRARY=../MovesenseCoreLib/ -DCMAKE_TOOLCHAIN_FILE=../MovesenseCoreLib/toolchain/gcc-nrf52.cmake
    
    > ../samples/hello_world_app
    
    > ninja dfupkg


Optional: JIG/Debugger support

* Download nRF5x-Command-Line Tools and JLink

http://www.nordicsemi.com/eng/nordic/Products/nRF51-DK/nRF5x-Command-Line-Tools-Linux64/51392

https://www.segger.com/downloads/jlink/JLink_Linux_V620h_x86_64.deb

* Install deb file

   > sudo dpkg -i ~/Downloads/JLink_Linux_V620h_x86_64.deb

* Extract command line tools

    > tar -xvf ~/Downloads/nRF5x-Command-Line-Tools_9_7_1_Linux-x86_64.tar
    
    > mkdir ~/tools
    
    > sudo mv nrfjprog /opt/SEGGER/JLink/
    
    > mv mergehex/ ~/tools/

* Add Path

    a) Permanently
    
    TBD
    
    b) Temporarily
    
    > export PATH=$PATH:~/tools/mergehex:/opt/SEGGER/JLink/nrfjprog

* Flash

    >ninja flash

### Automated setup on all operating systems using Vagrant ###
[Vagrant](https://www.vagrantup.com/) provides an easy, reproducable environment
for setting up development environments. It provides an isolated environment with
all the dependencies set up without any version clashes or corner case bugs.

**Note: this method doesn't automatically install support for Movesense programming JIG.** Refer to [Linux installation steps](#markdown-header-manual-setup-on-linux) to enable JIG support.

To get up and running

1. Get [Vagrant for your platform](https://www.vagrantup.com/downloads.html)
2. Get [Virtualbox](https://www.virtualbox.org/wiki/Downloads) (easiest way to run and manage VMs)
3. Clone this repository and run `vagrant up` - this will pull the ubuntu image and set up the environment necessary to develop Movesense software. This should take around 3 minutes.
4. Once the box is up, run `vagrant ssh` in the directory. You will be taken to the fully set up environment and ready to start developing. A great place to go next is *Example application build flow* below


## Example application build flow ##

```
#!python
> git clone git@bitbucket.org:suunto/movesense-device-lib.git
> cd movesense-device-lib
> mkdir myBuild
> cd myBuild

To build a debug version of a selected sample application:  

> cmake -G Ninja -DMOVESENSE_CORE_LIBRARY=../MovesenseCoreLib/ -DCMAKE_TOOLCHAIN_FILE=../MovesenseCoreLib/toolchain/gcc-nrf52.cmake ../samples/hello_world_app  

To build a release version:  

> cmake -G Ninja -DMOVESENSE_CORE_LIBRARY=../MovesenseCoreLib/ -DCMAKE_TOOLCHAIN_FILE=../MovesenseCoreLib/toolchain/gcc-nrf52.cmake -DCMAKE_BUILD_TYPE=Release ../samples/hello_world_app  

> ninja
```

**Note:** By default, the serial interface is enabled on Movesense sensor (see App.cpp file). Unless you need serial support (for programming JIG, for example), remember to keep it disabled to avoid excessive power consumption ([link to instructions](https://bitbucket.org/suunto/movesense-device-lib/src/master/MovesenseCoreLib/documentation/PowerOptimization.md?at=master&fileviewer=file-view-default)).

### Updating sensor "over the air" ###

If you want to create OTA firmware update package, run command
```
ninja dfupkg
```

which places the OTA package named *movesense_dfu.zip* in the build folder. You can then use our [Android Sample App](https://bitbucket.org/suunto/movesense-mobile-lib) to update the firmware on your Movesense sensor over Bluetooth.

### Updating sensor with programming JIG ###

if you are using a *Movesense programming JIG* you can flash the resulting .hex-file (and the nRF52 SoftDevice) with:
```
ninja flash
```
or you can use any other nRF52 compatible programming software such as *nRFGo Studio*. To acquire Movesense programming JIG, please contact us via [movesense.com](https://movesense.com).

### Removing manufacturing/settings data from the sensor ###
Command below updates the sensor firmware and additionally erases all settings in its memory. You should use it only if you want to replace default manufacturing/calibration data. Please don't use the command unless you know what you're doing!
```
ninja flash_all
```

## Bug reports and other feedback ##

Always start by checking if our [FAQ.md](FAQ.md) already contains the answer you're looking for.   

Please report all bugs by [raising an Issue](https://bitbucket.org/suunto/movesense-device-lib/issues/new) on Bitbucket. 

You can also discuss Movesense related topics on [stack overflow](http://stackoverflow.com/search?q=movesense). If your topic is not already addressed, 
please [post](http://stackoverflow.com/questions/ask) a new one, tagging it with 'movesense' (i.e. include [tag:movesense] in the question body).  

All feedback is welcome and helps to improve our offering!  

## Contributions to the source code ##

Your input is appreciated and we encourage you to post your contributions as pull requests to this repository.

## License ##

See [LICENSE.pdf](LICENSE.pdf) for details on Movesense license.