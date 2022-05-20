/**
 * @file main.c
 * @brief FLASH erase and program example
 * @author Dong Qin (dong.qin@timesintelli.com)
 * @version 1.0
 * @date 2022-05-16
 * @copyright Copyright (c) 2022 Timesintelli, Inc
 */

#include "at103.h"

void main(void)
{
    pll_init();
    sys_io_init();
    uart_init(UART_BOOT_PORT, UART_PARITY_NONE, UART_STOPBITS_1, UART_DATABITS_8, UART_BOOT_BD);

    /* FLASH Unlock */
    debug("FLASH Unlock \r\n");
    FLASH_Unlock();

    /* FLASH Update Latency, if AHB frequency is changed, that it must be called to ensure correct Flash access settings */
    debug("FLASH Update Latency \r\n");
    FLASH_UpdateLatency();

    /* FLASH Enable Prefetch Buffer */
    debug("FLASH Enable Prefetch Buffer \r\n");
    FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);
    debug("FLASH Prefetch Buffer Status = %d\r\n", FLASH_GetPrefetchBufferStatus());

    debug("FLASH_GetStatus = %d\r\n", FLASH_GetStatus());

    /* FLASH Erase All Pages */
    debug("FLASH Erase All Pages Start\r\n");
    FLASH_EraseAllPages();
    debug("FLASH Erase All Pages Completed\r\n");

    /* FLASH Program All Pages */
    debug("FLASH Program All Pages Start\r\n");
    for (uint32_t i = 0; i < 128 * 257; i++) {
        FLASH_ProgramDoubleWord(FLASH_BASE + i * 8, (uint64_t)i);
    }
    debug("FLASH Program All Pages Completed\r\n");

    /* FLASH Read All Pages */
    debug("FLASH Read All Pages Start\r\n");
    for (uint32_t i = 0; i < 128 * 257; i++) {
        if (*(uint64_t *)(FLASH_BASE + i * 8) != i) {
            debug("FLASH test error, address = 0x%x = 0x%x\r\n", FLASH_BASE + i * 8, i);
            while (1) {
                /* code */
            }
        }
    }
    debug("FLASH Read All Pages Completed\r\n");

    /* FLASH Lock */
    debug("FLASH Lock \r\n");
    FLASH_Lock();

    while (1) {
    }
}