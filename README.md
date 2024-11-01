# Movesense BLE Firmware

A custom firmware for [Movesense Sensor devices](https://www.movesense.com/) which implements a Bluetooth Low Energy (BLE) GATT electrocardiograph (ECG) voltage and movement measurements.

This Repository is forked from **version 2.2.1** of the [official Movesense Device Library](https://bitbucket.org/movesense/movesense-device-lib/).

## Citation

This software was originally created for the SSTeP-KiZ Project. You may cite the software like this:

Primbs, J.; et al. The SSTeP-KiZ System—Secure Real-Time Communication Based on Open Web Standards for Multimodal Sensor-Assisted Tele-Psychotherapy. Sensors 2022, 22, 9589. [https://doi.org/10.3390/s22249589](https://doi.org/10.3390/s22249589)

## Documentation

### Provided GATT Services and Characteristics

Our custom firmware provides the following GATT Services:

- **Heart Rate Service** with 16-bit UUID `0x180D`: Provides access to R-R intervals in milliseconds. See the [Bluetooth SIG specification](https://www.bluetooth.com/de/specifications/specs/heart-rate-service-1-0/) for more details.
- **Activity Data Service** with 16-bit UUID `0x1859`: Provides access to the ECG voltage and movement measurements.
  - **ECG Voltage Characteristic** with 16-bit UUID `0x2BDD`: Provides *NOTIFY* access to a relative *timestamp* and a series of *n* *voltages*. Each voltage is encoded as a 16-bit integer where -32768 is interpreted as an invalid value.
  - **ECG Measurement Interval Characteristic** with 16-bit UUID `0x2BE3`: Provides *READ* and *WRITE* access to the ECG measurement interval in milliseconds. Allowed values: 2 (500 Hz), 4 (250 Hz), 8 (125 Hz), and 10 (100 Hz).
  - **Movement Characteristic** with 16-bit UUID `0x2BE2`: Provides *NOTIFY* access to a relative *timestamp* and a series of *n* *3-tuples of 3D vectors of acceleration, gyroscope, and magnetic field values*. Each value is encoded as a 16-bit integer where -32768 is interpreted as an invalid value.
  - **Movement Measurement Interval Characteristic** with 16-bit UUID `0x2BE4`: Provides *READ* and *WRITE* access to the movement measurement interval in milliseconds. Allowed values:  5 (208 Hz), 10 (104 Hz), 20 (52 Hz), 40 (26 Hz).

### Lifecycle

The sensor powers on when either the battery is inserted or the ECG measures activity.

Then the sensor is on and announces Bluetooth connectivity with an interval of 100 milliseconds.
While the sensor is on, the red LED blinks with an interval of 5 seconds.
After 30 seconds without any Bluetooth connection to a device, the red LED lights up for 3 seconds before it shuts down.
When the sensor disconnects from a connected Bluetooth device, this sequence is repeated and the sensor shuts down after 30 + 3 seconds without any connection.

The shutdown can be only prevented within the time window of 30 seconds.
As soon as the sensor enters the 3 second shutdown sequence with a lighted LED, the shutdown cannot be prevented anymore.

## Getting Started

To use the Movesense ECG devices with the BLE GATT ECG Voltage service, you must flash the firmware on your Movesense sensor with the firmware whose source code is provided in this repository.
To get this custom firmware, you have two choices:

- [Build it from source](#build-from-source) or
- [Download it from releases](#download-from-releases)

### Build from Source

To build the firmware from source, first pull the official Docker image:

```bash
docker pull movesense/sensor-build-env:2.2
```

Then clone this repository:

```bash
git clone https://github.com/JonasPrimbs/movesense-ble-ecg-firmware.git
```

Then run the Docker image and mount the directory to the image:

Linux or MacOS:
```bash
docker run -it --rm -v /path/to/repo/movesense-ble-ecg-firmware:/movesense:delegated movesense/sensor-build-env:2.2
```

Windows:
```powershell
docker run -it --rm -v C:\path\to\repo\movesense-ble-ecg-firmware:/movesense:delegated movesense/sensor-build-env:2.2
```

When the Docker container starts, create the directory `/movesense/build` and navigate to it:

```bash
mkdir /movesense/build
cd /movesense/build
```

To prepare for building in **debug** mode, prompt:

```bash
# For HR+/HR2/MD sensor:
cmake -G Ninja -DMOVESENSE_CORE_LIBRARY=../MovesenseCoreLib/ -DCMAKE_TOOLCHAIN_FILE=../MovesenseCoreLib/toolchain/gcc-nrf52.cmake -DHWCONFIG=SS2 ../ble-ecg

# For Flash sensor:
cmake -G Ninja -DMOVESENSE_CORE_LIBRARY=../MovesenseCoreLib/ -DCMAKE_TOOLCHAIN_FILE=../MovesenseCoreLib/toolchain/gcc-nrf52.cmake -DHWCONFIG=SS2_NAND ../ble-ecg
```

To prepare for building in **production** mode, prompt:

```bash
# For HR+/HR2/MD sensor:
cmake -G Ninja -DMOVESENSE_CORE_LIBRARY=../MovesenseCoreLib/ -DCMAKE_TOOLCHAIN_FILE=../MovesenseCoreLib/toolchain/gcc-nrf52.cmake -DCMAKE_BUILD_TYPE=Release ../ble-ecg

# For Flash sensor:
# DO NOT USE PRODUCTION MODE FOR FLASH SENSOR AS THIS WILL BRICK THE SENSOR!
# See https://www.movesense.com/docs/system/hw_variants/
```

To build in prepared mode, prompt:

```bash
ninja pkgs
```

This will build the firmware to `/movesense/build`.
The important files to deploy the firmware are:

- `Movesense_dfu_w_bootloader.zip`: The bootloader required to flash Movesense sensors for the first time.
- `Movesense_dfu.zip`: The actual firmware.

With these files, you can go on with section [Install Firmware](#install-firmware).

### Download from Releases

Go to [Releases](https://github.com/JonasPrimbs/movesense-ble-ecg-firmware/releases) and download the latest version.

If you want to flash the Movesense HR+/HR2/MD sensor, download these files:

- `Movesense_dfu_w_bootloader.zip`
- `Movesense_dfu.zip`

If you want to flash the Movesense Flash sensor, download these files:

- `Movesense_dfu_w_bootloader_NAND.zip`
- `Movesense_dfu_NAND.zip`

Now go on with section [Install Firmware](#install-firmware).

### Install Firmware

To flash the firmware, install the Movesense Showcase App for [Android](https://bitbucket.org/movesense/movesense-mobile-lib/downloads/) or [iOS](https://apps.apple.com/de/app/movesense-showcase/id1439876677) and use its DFU tool as described [here](https://www.movesense.com/docs/esw/dfu_update/).

**Warning: We are using Firmware version 2.2.1 as basis!**
**This requires also a Bootloader of version 2.x.**
**Flashing the sensor with this Bootloader requires an updated Showcase App (Android: from 1.9.8, iOS: from 1.0.5)!**
Otherwise you will consider a file corrupted error.

**Hint: If you haven't flashed the firmware since buying your Movesense sensor, you must first flash it with the `Movesense_dfu_w_bootloader.zip` (or `Movesense_dfu_w_bootloader_NAND.zip` for Movesense Flash sensor) file.**
After that, you can directly flash the firmware with `Movesense_dfu.zip` (or `Movesense_dfu_NAND.zip` for Movesense Flash sensor).
