/**
 * @file main.c
 * @brief helloworld example
 * @author Dong Qin (dong.qin@timesintelli.com)
 * @version 1.0
 * @date 2022-01-07
 * @copyright Copyright (c) 2022 Timesintelli, Inc
 */

#include "at103.h"

void main(void)
{
    pll_init();
    sys_io_init();
    uart_init(UART_BOOT_PORT, UART_PARITY_NONE, UART_STOPBITS_1, UART_DATABITS_8, UART_BOOT_BD);

    for (uint32_t i = 0; i < 10; i++) {
        debug("hello world!!!\r\n");
    }
    while (1) {
    }
}