# Asynchronous Serial API

The current mbed Serial API is byte-oriented, providing blocking methods (``putc()`` and ``getc()``) and a non-blocking interrupt method to enable and disable TX/RX IRQ (``attach()``). The new version of the API has six new array-oriented, non-blocking methods. These are defined in the SerialBase class.

Currently only the following configuration is supported:

- Freescale FRDM-K64F board
- GCC ARM Embedded Toolchain

## Asynch Methods

The Serial API has six new methods. We'll review the TX methods first, then the RX methods.

### TX Methods

The ``write()`` method starts the TX transfer and return to the caller. The completion invokes a registered TX event callback if one was defined.

There are two write methods, with different buffer widths: 8-bit and 16-bit.

```
int write(uint8_t *buffer, int length, void (*callback)(int), int event = SERIAL_EVENT_TX_COMPLETE);

int write(uint16_t *buffer, int length, void (*callback)(int), int event = SERIAL_EVENT_TX_COMPLETE);

```

To abort the on-going TX transfer, use ``abort_write()``:

```
void abort_write();
```

### RX Methods

The ``read()`` method is used to start reading data (RX transfer). The completion invokes a registered RX event callback if one was defined.

There are two read methods, with different buffer widths: 8-bit or 16-bit.

```
int read(uint8_t *buffer, int length, void (*callback)(int), int event = SERIAL_EVENT_RX_COMPLETE, unsigned char char_match = SERIAL_RESERVED_CHAR_MATCH);

int read(uint16_t *buffer, int length, void (*callback)(int), int event = SERIAL_EVENT_RX_COMPLETE, unsigned char char_match = SERIAL_RESERVED_CHAR_MATCH);

```

To abort the on-going RX transfer, use ``abort_read()``. One use might be to set a time-out to receive data, and use ``abort_read()`` if data has not arrived within the specified time.

```
void abort_read();
```

## Events

There are some predefined TX and RX events for event handling. 

### TX Events

The following TX events are available:

 - SERIAL_EVENT_TX_COMPLETE - All data from the TX buffer have been transmitted.

### RX Events

The following RX events are available:

- SERIAL_EVENT_RX_COMPLETE - All data from RX buffer have been received.
- SERIAL_EVENT_RX_OVERRUN_ERROR - Overrun error.
- SERIAL_EVENT_RX_FRAMING_ERROR - Framing error.
- SERIAL_EVENT_RX_PARITY_ERROR - Parity error.
- SERIAL_EVENT_RX_OVERFLOW - RX overflow.
- SERIAL_EVENT_RX_CHARACTER_MATCH - The defined character has been received.
- SERIAL_EVENT_RX_ALL - Logical OR of all available RX events.

## The Demo Application

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
    cd /path/to/mbed-example-asynch-serial
	```

4. Select the yotta target and build to it:

    ```
    yotta target frdm-k64f-gcc
    yotta build
    ```

4. Copy ``build/frdm-k64f-gcc/source/mbed-example-asynch-serial.bin`` to your mbed board and wait until the LED next to the USB port stops blinking.

5. Start the serial terminal emulator and connect to the virtual serial port presented by FRDM-K64F. For settings, use 9600 baud, 8N1, no flow control.

6. Press the reset button on the board.

7. The output in the terminal should look like this:

    ```
    TEST(Serial_Asynchronous, char_matching_with_complete) - 2 ms
    TEST(Serial_Asynchronous, char_matching_failed) - 2 ms
    TEST(Serial_Asynchronous, char_matching_success) - 2 ms
    TEST(Serial_Asynchronous, rx_framing_error) - 4 ms
    TEST(Serial_Asynchronous, rx_parity_error) - 2 ms
    TEST(Serial_Asynchronous, long_tx_long_rx) - 2 ms
    TEST(Serial_Asynchronous, short_tx_short_rx) - 1 ms
    TEST(Serial_Asynchronous, short_tx_0_rx) - 1 ms


    OK (8 tests, 8 ran, 28 checks, 0 ignored, 0 filtered out, 57 ms)

    {{success}}
    {{end}}
```

8. The LED should be slowly blinking (toggling about 1 Hz).