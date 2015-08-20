
/* mbed Microcontroller Library
 * Copyright (c) 2015 ARM Limited
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include <stdio.h>

#include "mbed.h"
#include "minar/minar.h"
#include "Event.h"

#if DEVICE_SERIAL_ASYNCH

// Device config
#if defined(TARGET_K64F)
#define TEST_SERIAL_ONE_TX_PIN PTC17 // uart3
#define TEST_SERIAL_TWO_RX_PIN PTD2  // uart2

#else

#error Target not supported
#endif

// Test config
#define SHORT_XFR 3
#define LONG_XFR 16
#define TEST_BYTE_TX_BASE 0x5555
#define TEST_BYTE_RX_BASE 0x5A5A

using namespace minar;

class SerialTest {

public:
    SerialTest(): serial_tx(TEST_SERIAL_ONE_TX_PIN, NC), serial_rx(NC, TEST_SERIAL_TWO_RX_PIN) {
        for (uint32_t i = 0; i < sizeof(tx_buf); i++) {
            tx_buf[i] = i + TEST_BYTE_TX_BASE;
        }

    }

    void start() {
        printf("Starting short transfer test\r\n");
        init_rx_buffer();
        printf("Res is %d\r\n", serial_rx.read(rx_buf, SHORT_XFR, Serial::event_callback_t(this, &SerialTest::short_transfer_complete_cb), SERIAL_EVENT_RX_COMPLETE));
        printf("Res is %d\r\n", serial_tx.write(tx_buf, SHORT_XFR, NULL, 0));
    }

private:
    void init_rx_buffer() {
        for (uint32_t i = 0; i < sizeof(rx_buf); i++) {
            tx_buf[i] = 0;
        }
    }

    void compare_buffers(uint32_t len) {
         for (uint32_t i = 0; i < len; i ++) {
            if (tx_buf[i] != rx_buf[i]) {
                printf("MISMATCH at position %lu: expected %d, got %d\r\n", i, (int)tx_buf[i], (int)rx_buf[i]);
            }
        }
    }

    void short_transfer_complete_cb(Buffer buf, int narg) {
        (void)buf;
        printf("Short transfer DONE, event is %d\r\n", narg);
        compare_buffers(SHORT_XFR);
        printf("Starting long transfer test\r\n");
        init_rx_buffer();
        printf("Res is %d\r\n", serial_rx.read(rx_buf, LONG_XFR, Serial::event_callback_t(this, &SerialTest::long_transfer_complete_cb), SERIAL_EVENT_RX_COMPLETE));
        printf("Res is %d\r\n", serial_tx.write(tx_buf, LONG_XFR, NULL, 0));
    }

    void long_transfer_complete_cb(Buffer buf, int narg) {
        (void)buf;
        printf("Long transfer DONE, event is %d\r\n", narg);
        compare_buffers(LONG_XFR);
        printf("**** Test done ****\r\n");
    }

private:
    Serial serial_tx;
    Serial serial_rx;
    uint8_t tx_buf[LONG_XFR];
    uint8_t rx_buf[LONG_XFR];
};

void app_start(int, char*[]) {
    static SerialTest test;
    // set 115200 baud rate for stdout
    static Serial pc(USBTX, USBRX);
    pc.baud(115200);
    Scheduler::postCallback(FunctionPointer0<void>(&test, &SerialTest::start).bind());
}

#else
void app_start(int, char*[]) {
    // set 115200 baud rate for stdout
    static Serial pc(USBTX, USBRX);
    pc.baud(115200);
    printf("The target does not support Serial asynch API.\r\n");
}
#endif

