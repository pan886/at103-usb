/**
 * @file main.c
 * @brief CRC example.
 * 
 * @author pantianwen (pantianwen@163.com)
 * @version 1.0
 * @date 2022-05-15
 * @copyright Copyright (c) 2022 Icore, Inc
 */
#include "at103.h"
void delay()
{
    for (uint16_t i = 0; i < 0xffff; i++)
        ;
}
void main(void)
{
    pll_init();
    sys_io_init();
    uart_init(UART_BOOT_PORT, UART_PARITY_NONE, UART_STOPBITS_1, UART_DATABITS_8, UART_BOOT_BD);
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_CRC, ENABLE);
    //

    CRC->CRCSEED.SEED   = 0;
    CRC->CRC_CTL.CLEAR  = 1;
    CRC->CRC_CTL.INVOUT = 0;
    CRC->CRC_IN         = 0x1;
    //
    delay();
    //RCC_AHBPeriphResetCmd(RCC_AHBPeriph_CRC, ENABLE);
    debug("seed = %x\n", *((uint32_t *)(CRC_BASE + 0xFFFC)));

    debug("seed2= %x\n", CRC->CRCSEED.SEED);

    debug("out2 = %x\n", (*((uint32_t *)(CRC_BASE + 0xFFF4)) & 0xffff));
    debug("out = %x\n", CRC->CRC_OUT.COUT);

    // *((uint32_t *)(CRC_BASE + 0xFFFC)) = 0x5A;
    //  *((uint32_t *)(CRC_BASE))          = 0x86;
    // *((uint32_t *)(CRC_BASE + 0xFFF8)) = 0x1;
    while (1) {
    }
}