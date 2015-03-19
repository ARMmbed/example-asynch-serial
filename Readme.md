# Asynchronous Serial

## The demo application

This repository is an example for asynchronous Serial, a test unit with defined test cases for asynchronous Serial methods, using two Serial modules.

Please look at the target's pin definitions in the serial_asynch.cpp file (TEST_SERIAL_ONE_TX_PIN and TEST_SERIAL_TWO_RX_PIN). The Tx pin from one serial module should be connected to RX pin on another serial module.

Currently only the following configuration is supported:
- Freescale FRDM-K64F board
- GCC ARM Embedded Toolchain

### Installation
This is a yotta executable. You need the latest [yotta](https://github.com/ARMmbed/yotta) release version (latest tested: 0.0.36) and all of its dependencies. See also the [yotta documentation](http://armmbed.github.io/yotta/) for further details about the installation and the build process.

Assuming you have cloned this repository, move to its folder. Select target, install and build:
```bash
cd /path/to/mbed-example-asynch-serial
yotta target frdm-k64f-gcc
yotta install
yotta build
```

The resulting binary file will be located in `build/frdm-k64f-gcc/source/`. After flashing, you can observe the UART output on the USB serial port.

The output on the console should be:
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


## API Overview
The current mbed Serial API is byte-oriented, providing blocking methods (putc() and getc()) and non-blocking interrupt method to enable/disable TX/RX IRQ (attach()).

This documents describes the non-blocking methods which are array-oriented.
The write and read methods initiate the transfer and return to the caller. The completition is reported via callbacks which are registered to specific events.

## Asynch methods

The Serial API was expanded by 6 new methods which are defined in the SerialBase class.

### TX methods
The write method is used to start transferring data (TX transfer). There are 2 write methods, with different buffer width - 8-bit or 16-bit.
The write method starts the transfer and return to the caller. The completition invokes registered TX event callback, only if any was defined.
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
The read method is used to start reading data (RX transfer). The completition invokes registred RX event callback, only if any was defined.
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
