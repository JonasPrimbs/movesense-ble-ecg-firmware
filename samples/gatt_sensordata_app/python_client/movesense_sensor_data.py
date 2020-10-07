# -*- coding: utf-8 -*-
"""
Python gatt_sensordata_app client example using the Bleak GATT client.

This example is based on the examples in the Bleak repo: https://github.com/hbldh/bleak
"""

import logging
import asyncio
import platform
import signal
from bleak import BleakClient
from bleak import _logger as logger
from bleak import discover
from functools import reduce
import struct

WRITE_UUID = (
    "34800001-7185-4d5d-b431-630e7050e8f0"
)

CHARACTERISTIC_UUID = (
    "34800002-7185-4d5d-b431-630e7050e8f0"
)


# https://stackoverflow.com/a/56243296
class DataView:
    def __init__(self, array, bytes_per_element=1):
        """
        bytes_per_element is the size of each element in bytes.
        By default we are assume the array is one byte per element.
        """
        self.array = array
        self.bytes_per_element = 1

    def __get_binary(self, start_index, byte_count, signed=False):
        integers = [self.array[start_index + x] for x in range(byte_count)]
        bytes = [integer.to_bytes(
            self.bytes_per_element, byteorder='little', signed=signed) for integer in integers]
        return reduce(lambda a, b: a + b, bytes)

    def get_uint_16(self, start_index):
        bytes_to_read = 2
        return int.from_bytes(self.__get_binary(start_index, bytes_to_read), byteorder='little')

    def get_uint_8(self, start_index):
        bytes_to_read = 1
        return int.from_bytes(self.__get_binary(start_index, bytes_to_read), byteorder='little')

    def get_uint_32(self, start_index):
        bytes_to_read = 4
        binary = self.__get_binary(start_index, bytes_to_read)
        return struct.unpack('<I', binary)[0]  # <f for little endian

    def get_float_32(self, start_index):
        bytes_to_read = 4
        binary = self.__get_binary(start_index, bytes_to_read)
        return struct.unpack('<f', binary)[0]  # <f for little endian


def notification_handler(sender, data):
    """Simple notification handler which prints the data received."""
    d = DataView(data)
    print("Data:", d.get_uint_32(2), d.get_float_32(
        6), d.get_float_32(10), d.get_float_32(14))


async def run(address, loop, debug=False):
    if debug:
        import sys
        loop.set_debug(True)
        l = logging.getLogger("asyncio")
        l.setLevel(logging.DEBUG)
        h = logging.StreamHandler(sys.stdout)
        h.setLevel(logging.DEBUG)
        l.addHandler(h)
        logger.addHandler(h)

    # Check the device is available
    devices = await discover()
    found = False
    for d in devices:
        if d.address == address:
            print("device found")
            found = True
            break

    if found:
        async with BleakClient(address, loop=loop) as client:

            # This event is set if device disconnects or ctrl+c is pressed
            disconnected_event = asyncio.Event()

            def disconnect_callback(client, future):
                print("Disconnected callback called!")
                loop.call_soon_threadsafe(disconnected_event.set)

            # Connect the device
            x = await client.is_connected()
            logger.info("Connected: {0}".format(x))

            # Set disconnect callback
            if platform.system() != "Darwin":
                client.set_disconnected_callback(disconnect_callback)

            # Add signal handler for ctrl+c
            for signame in {'SIGINT', 'SIGTERM'}:
                loop.add_signal_handler(
                    getattr(signal, signame), lambda:
                    loop.call_soon_threadsafe(disconnected_event.set))

            # Start notifications and subscribe to acceleration
            await client.start_notify(CHARACTERISTIC_UUID, notification_handler)
            await client.write_gatt_char(WRITE_UUID, bytearray([1, 99])+bytearray("/Meas/Acc/13", "utf-8"))

            # Run until disconnect event is set
            await disconnected_event.wait()

            # Check the conection status to infer if the device disconnected or crtl+c was pressed
            status = await client.is_connected()
            logger.info("Connected: {0}".format(status))

            # If status is connected, unsubscribe and stop notifications
            if status:
                await client.write_gatt_char(WRITE_UUID, bytearray([2, 99]))
                await client.stop_notify(CHARACTERISTIC_UUID)
                stop = True
            await asyncio.sleep(1.0, loop=loop)
    else:
        print("Sensor", address, "not found!")


if __name__ == "__main__":
    import os

    os.environ["PYTHONASYNCIODEBUG"] = str(1)
    address = (
        # <--- Change to your device's address here if you are using Windows or Linux
        "0C:8C:DC:22:54:15"
        if platform.system() != "Darwin"
        # <--- Change to your device's address here if you are using macOS
        else "CCC22DB7-B9E4-4FD4-9E28-E1E97383EC4E"
    )

    loop = asyncio.get_event_loop()
    loop.run_until_complete(run(address, loop, True))
