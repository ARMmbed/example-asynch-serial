# Asynchronous Serial

## The demo application

This repository is an example for asynchronous Serial, a test unit with defined test cases for asynchronous Serial methods, using two Serial peripheral modules (for instance TX from USART0 and RX from USART1).

Please look at the target's pin definitions in the serial_asynch.cpp file (TEST_SERIAL_ONE_TX_PIN and TEST_SERIAL_TWO_RX_PIN).
The pin definitions are target specific, look at the [mbed platform page](https://developer.mbed.org/platforms), where each platform has pinout picture.

Currently only the following configuration is supported:
- Freescale FRDM-K64F board
- GCC ARM Embedded Toolchain

### Pre-requisites

To build and run this example the requirements below are necessary:
* A computer with the following software installed
  * CMake
  * yotta
  * python
  * ARM-GCC toolchain
  * a serial terminal emulator (e.g. screen, pyserial, cu)
* A frdm-k64f development board
* A micro-USB cable
* A wire to connect TX and RX pins

### Getting started
This is a yotta executable. You need the latest [yotta](https://github.com/ARMmbed/yotta) release version (latest tested: 0.2.2) and all of its dependencies. See also the [yotta documentation](http://armmbed.github.io/yotta/) for further details about the installation and the build process.

1. Connect the frdm-k64f to the computer with the micro-USB cable, being careful to use the micro-usb port labeled "OpenSDA"

2. Connect TEST_SERIAL_ONE_TX_PIN to TEST_SERIAL_TWO_RX_PIN. These pins are target specific, please look at the source code file where are defined for your target.

3. Assuming you have cloned this repository, move to its folder. Select target, update, install and build:
    ```
    cd /path/to/mbed-example-asynch-serial
    yotta target frdm-k64f-gcc
    yotta build
    ```

4. Copy 'build/frdm-k64f-gcc/source/mbed-example-asynch-serial.bin` to your mbed board and wait until the LED next to the USB port stops blinking.

5. Start the serial terminal emulator and connect to the virtual serial port presented by frdm-k64f. For settings, use 9600 baud, 8N1, no flow control.

6. Press the reset button on the board.

7. The output in the terminal should look like:
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

8. LED should be slowly blinking (toggling about 1 Hz).

## API Overview
The current mbed Serial API is byte-oriented, providing blocking methods (putc() and getc()) and non-blocking interrupt method to enable/disable TX/RX IRQ (attach()).

This documents describes the non-blocking methods which are array-oriented.
The write and read methods initiate the transfer and return to the caller. The completion is reported via callbacks which are registered to specific events.

## Asynch methods

The Serial API was expanded by 6 new methods which are defined in the SerialBase class.

### TX methods
The write method is used to start transferring data (TX transfer). There are 2 write methods, with different buffer width - 8-bit or 16-bit.
The write method starts the transfer and return to the caller. The completion invokes registered TX event callback, only if any was defined.
```
int write(uint8_t *buffer, int length, void (*callback)(int), int event = SERIAL_EVENT_TX_COMPLETE);

int write(uint16_t *buffer, int length, void (*callback)(int), int event = SERIAL_EVENT_TX_COMPLETE);

```

To abort the on-going TX transfer, use abort_write() which stops the current TX transfer.
```
void abort_write();
```

### RX methods
There are 2 read methods, with different buffer width - 8-bit or 16-bit.
The read method is used to start reading data (RX transfer). The completion invokes registred RX event callback, only if any was defined.
```
int read(uint8_t *buffer, int length, void (*callback)(int), int event = SERIAL_EVENT_RX_COMPLETE, unsigned char char_match = SERIAL_RESERVED_CHAR_MATCH);

int read(uint16_t *buffer, int length, void (*callback)(int), int event = SERIAL_EVENT_RX_COMPLETE, unsigned char char_match = SERIAL_RESERVED_CHAR_MATCH);

```

To abort the on-going RX transfer, use abort_read() which stops the current RX transfer. One use might be to set a time out to receive data, and use abort_read() if data has not arrived within the specified time.
```
void abort_read();
```

## Events

### TX events

The following TX events are available:

 - SERIAL_EVENT_TX_COMPLETE - The data from TX buffer have been transmitted

### RX events

The following RX events are available:

- SERIAL_EVENT_RX_COMPLETE - All data from RX buffer have been received
- SERIAL_EVENT_RX_OVERRUN_ERROR - The overrun error
- SERIAL_EVENT_RX_FRAMING_ERROR - The framing error
- SERIAL_EVENT_RX_PARITY_ERROR - The parity error
- SERIAL_EVENT_RX_OVERFLOW - The RX overflow
- SERIAL_EVENT_RX_CHARACTER_MATCH - The defined character has been received
- SERIAL_EVENT_RX_ALL - Logical OR of all available RX events
