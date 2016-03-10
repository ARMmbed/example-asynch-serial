# Asynchronous Serial Example

This repository is an example for asynchronous serial. It is a test unit with test cases for asynchronous serial methods, using two serial peripheral modules (for instance TX from USART0 and RX from USART1).

### Pre-Requisites

To build and run this example the following requirements are necessary:

* A computer with the following software installed:
	* [CMake](http://www.cmake.org/download/).
	* [yotta](https://github.com/ARMmbed/yotta). Please note that **yotta has its own set of dependencies**, listed in the [installation instructions](http://armmbed.github.io/yotta/#installing-on-windows).
	* [Python](https://www.python.org/downloads/).
	* [ARM GCC toolchain](https://launchpad.net/gcc-arm-embedded).
	* A serial terminal emulator (e.g. screen, pySerial, cu).
* An FRDM-K64F development board. Please look at the [pinout picture](http://developer.mbed.org/platforms/FRDM-K64F/#overview) to find the correct pins.
* A micro-USB cable.
* A wire to connect TX and RX pins.
* If your OS is Windows, please follow the installation instructions [for the serial port driver](https://developer.mbed.org/handbook/Windows-serial-configuration).

### Pins

TX and RX pin definitions are target-specific.  To function correctly, the code and the physical board must both be using the same pins.  Follow these steps to determine which pins the code is configured to use.

1. First, find the pin references in the source files.  In this example, view ``serial_asynch.cpp``. The pin references are ``YOTTA_CFG_HARDWARE_TEST_PINS_SERIAL_TX`` and ``YOTTA_CFG_HARDWARE_TEST_PINS_SERIAL_RX``.

2. Next, find these target-specific pin references in the configuration data by running the command:
 ```
 yotta config
 ```
 Each keyword in the pin reference represents a level in the target configuration structure.  To find what YOTTA_CFG_HARDWARE_TEST_PINS_SERIAL resolves to, look under config...hardware...test-pins...serial.

 In this example, the serial tx and rx pins are defined as:
 ```
 "serial": {
        "tx": "PTC17",
        "rx": "PTD2"
 }
 ```
3. Finally, locate the pins (i.e. PTC17, PTD2) on the board's [pinout picture](https://www.mbed.com/en/development/hardware/boards/nxp/frdm_k64f/#overview).

### Getting Started

1. Connect the FRDM-K64F to the computer with the micro-USB cable, being careful to use the micro-USB port labeled "OpenSDA".

2. Connect YOTTA_CFG_HARDWARE_TEST_PINS_SERIAL_TX to YOTTA_CFG_HARDWARE_TEST_PINS_SERIAL_RX. See the previous section for an explanation if you do not know how to locate these.

3. Assuming you have cloned this repository or receive it with a release tarball, open a terminal window and navigate to the repository's folder.

	```
    cd /path/to/example-asynch-serial
	```

4. Select the yotta target and build to it:

    ```
    yotta target frdm-k64f-gcc
    yotta build
    ```

4. Copy ``build/frdm-k64f-gcc/source/example-asynch-serial.bin`` to your mbed board and wait until the LED next to the USB port stops blinking.

5. Start the serial terminal emulator and connect to the virtual serial port presented by FRDM-K64F. For settings, use 115200 baud, 8N1, no flow control.

6. Press the reset button on the board.

7. The output in the terminal should look like this:

    ```
    Starting short transfer test
    Res is 0
    Res is 0
    Short transfer DONE, event is 256
    Starting long transfer test
    Res is 0
    Res is 0
    Long transfer DONE, event is 256
    **** Test done ****
```
