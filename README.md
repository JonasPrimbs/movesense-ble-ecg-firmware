# Movesense BLE Firmware

A custom firmware for [Movesense Sensor devices](https://www.movesense.com/) which implements a Bluetooth Low Energy (BLE) GATT electrocardiograph (ECG) voltage and movement measurements.

This Repository is forked from **version 2.3.2** of the [official Movesense Device Library](https://bitbucket.org/movesense/movesense-device-lib/).

## Citation

This software was originally created for the SSTeP-KiZ Project. You may cite the software like this:

Primbs, J.; et al. The SSTeP-KiZ System—Secure Real-Time Communication Based on Open Web Standards for Multimodal Sensor-Assisted Tele-Psychotherapy. Sensors 2022, 22, 9589. [https://doi.org/10.3390/s22249589](https://doi.org/10.3390/s22249589)

## Documentation

### Provided GATT Services and Characteristics

#### Version 0.9.0 and above
This version adds to services provided by 0.8.0 the ability to automatically transition to offline storage mode when the device disconnects, if ECG or IMU data streams are enabled. This happens on sudden disconnects but **also** if one disconnects from the sensor, without **properly unsubscribing** from ECG voltage or IMU characteristics.

The sensor will blink rapidly 5 times when it enters this "emergency logging mode". It will then continue to record the previously active datastreams at the configured samplerate. When re-connected, the sensor will stop the offline logging as soon as the client re-subscribes to all previously subscribed characteristics, before the connection terminated (e.g. when the sensor was sending ECG data before, the client has to re-subscribe to ECG-data in order to stop the logging or if it was sending ECG and IMU data the client has to re-subscribe to both ECG and IMU Data to stop the sensor-local recording. The logs can then be transferred to the client with the same procedure as "manual logs" (i.e. recordings on the sensor that were directly initiated by the client and not a connection loss), described under version 0.8.0.

#### Version 0.8.0 and above
Our custom firmware provides the following GATT Services:
- **Heart Rate Service** with 16-bit UUID `0x180D`: Provides access to R-R intervals in milliseconds. See the [Bluetooth SIG specification](https://www.bluetooth.com/de/specifications/specs/heart-rate-service-1-0/) for more details.
- **Activity Data and Offline Recording Service** with 16-bit UUID `0x1859`: Provides Access to ECG and IMU measurements, control of sampling rates and functions to control the offline recording to the local sensor storage
  - **ECG Voltage Characteristic** with 16-bit UUID `0x2BF1`: Provides *NOTIFY* access to a relative *timestamp* and a series of *n* *voltages*. Each voltage is encoded as a 16-bit integer where -32768 is interpreted as an invalid value.
  - **IMU Characteristic** with 16-bit UUID `0x2BF2`: Provides *NOTIFY* access to a relative *timestamp* and a series of *n* *3-tuples of 3D vectors of acceleration, gyroscope, and magnetic field values*. Each value is encoded as a 16-bit integer where -32768 is interpreted as an invalid value.
  - **Configuration Characteristic** with 16-bit UUID `0x2BF3`: provides *READ* and *WRITE* access to configure the data-recording as well as starting and stopping offline-recording, triggering data-transfer and delete operations and configuring a 64-bit UNIX-timestamp. The field uses 128-Bytes in total, where every Field has 1-Byte width except the 8-Byte timestamp:
    <table>
      <tr>
        <td>ECG-SR</td>
        <td>IMU-SR</td>
        <td>ECG-RC</td>
        <td>IMU-RC</td>
        <td>Rec-OP</td>
        <td>Trans-OP</td>
        <td>Del-OP</td>
        <td>R</td>
      </tr>
      <tr>
        <td colspan="8">UNIX-timestamp in µs-precision</td>
      </tr>
    </table>
    
    - ECG-SR: Sample-Rate setting for ECG-measurement. Can be: 2 (500 Hz), 4 (250 Hz), 8 (125 Hz), 10 (100 Hz).
    - IMU-SR: Sample-Rate setting for IMU-measurement. Can be: 5 (208 Hz), 10 (104 Hz), 20 (52 Hz), 40 (26 Hz).
    - ECG-RC: ECG Recording-Configuration. Include ECG-measurement in Offline recordings. Can be: 0 (no) or 1 (yes).
    - IMU-RC: IMU Recording-Configuration. Include IMU-measurement in Offline recordings. Can be: 0 (no) or 1 (yes).
    - Rec-OP: Triggering and Offline recording. Can be: 0 (no offline recording), 1 (offline recording now until set to 0 again or storage full). When Rec-OP is set to 1, the sensor will disable the 30s auto-shutdown, so recording can continue even without a BLE-connection.
    - Trans-OP: Triggering a transfer of all the locally recorded data: Can be: 0 (no data transfer), 1 (data transfer now, auto-resets to 0 when complete).
    - Del-OP: Triggering a Deletion of all the locally recorded data: Can be: 0 (no data deletion), 1 (data deletion now, auto-resets to 0 when complete).
    - R: unused field
    - UNIX-timestamp: 64-bit UNIX-timestamp of the current time on the client in microsecond-precision to be used in measurement data.

  - **Data Transfer Characteristic** with 16-bit UUID `0x2BF4`: provides *NOTIFY* access to data transferred from the local sensor storage. Triggered by the transfer-op bit from the configuration-characteristic. When a Data-Transfer is triggered, the notifications on this property should be activated. A file in [SBEM](https://www.movesense.com/docs/esw/data_storage/)-Format will be transferred to the client in sequential notifications. End of the transfer is signaled by sending an individual 0 byte, which is not part of the file. Chunks containing ECG data are of length 10 and have chunk ID 104. Chunks containing IMU Data are of length 38 and have chunk ID 105.

--- 

#### Version 0.7.0 and below
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
docker pull movesense/sensor-build-env:2.3
```

Then clone this repository:

```bash
git clone https://github.com/JonasPrimbs/movesense-ble-ecg-firmware.git
```

Then run the Docker image and mount the directory to the image:

Linux or MacOS:
```bash
docker run -it --rm -v /path/to/repo/movesense-ble-ecg-firmware:/movesense:delegated movesense/sensor-build-env:2.3
```

Windows:
```powershell
docker run -it --rm -v C:\path\to\repo\movesense-ble-ecg-firmware:/movesense:delegated movesense/sensor-build-env:2.3
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

**Warning: We are using Firmware version 2.3.1 as basis!**
**This requires also a Bootloader of version 2.x.**
**Flashing the sensor with this Bootloader requires an updated Showcase App (Android: from 1.9.8, iOS: from 1.0.5)!**
Otherwise you will consider a file corrupted error.

**Hint: If you haven't flashed the firmware since buying your Movesense sensor, you must first flash it with the `Movesense_dfu_w_bootloader.zip` (or `Movesense_dfu_w_bootloader_NAND.zip` for Movesense Flash sensor) file.**
After that, you can directly flash the firmware with `Movesense_dfu.zip` (or `Movesense_dfu_NAND.zip` for Movesense Flash sensor).
