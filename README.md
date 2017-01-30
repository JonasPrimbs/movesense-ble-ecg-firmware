## Example

To build the example project, you will need to clone the repo (or download both Movesense Core library and Example App), 
and run the following commands from a build directory of your choosing.

Generate build file:
```
cmake -G Ninja -DMOVESENSE_CORE_LIBRARY=<path_to_movesense_library> -DCMAKE_TOOLCHAIN_FILE=<path_to_movesense_library>/toolchain/gcc-nrf52.cmake <path_to_example_app>
```

and build the example app:
```
ninja
```

if you are using "Movesense programming jig" you can flash the resulting app and needed nRF SoftDevice with:
```
ninja flash
```

## Installation

"Movesense Core library" for movesense device is available from git repository or as a zip download. Just unpack the zip anywhere.

To compile the applications you also need the following tools and programs:
- [GNU Toolchain for ARM Embedded](https://launchpad.net/gcc-arm-embedded)
- [Ninja build tool](https://ninja-build.org/)
- [cmake >=3.6](https://cmake.org/download/)

and if you want to flash the device firmware using the programming jig you need also:
- Nordic Semiconductor's from [nRF5x-Command-Line-Tools-Win32](https://www.nordicsemi.com/eng/Products/Bluetooth-low-energy/nRF52-DK)

## Frequently Asked Questions

FAQ can be found via http://stackoverflow.com/questions/tagged/movesense. If your problem is not addressed there, please [post](http://stackoverflow.com/questions/ask) a new question, and tag it with 'movesense' so that it will added to the FAQ collection.

## Bug Reports

Please report bugs by [raising](https://bitbucket.org/suunto/movesense-device-lib/issues/new) an Issue via Bitbucket.

## Contributions
Your input is appreciated and we encourage you to post your contributions as pull requests to this repository.

## License

See the LICENSE file for more info.