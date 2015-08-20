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

TX and RX pin definitions are target specific - the code and the physical board must both be using the same pins:

1. To know which pins the code is looking for, see the ``serial_asynch.cpp`` file for this sample. The pins are defined as ``TEST_SERIAL_ONE_TX_PIN`` and ``TEST_SERIAL_TWO_RX_PIN``.

2. To know where on your board those pins are, please look at its [pinout picture](http://developer.mbed.org/platforms/FRDM-K64F/#overview).

### Getting Started

1. Connect the FRDM-K64F to the computer with the micro-USB cable, being careful to use the micro-USB port labeled "OpenSDA".

2. Connect TEST_SERIAL_ONE_TX_PIN to TEST_SERIAL_TWO_RX_PIN. See the previous section for an explanation if you do not know how to locate these.

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
