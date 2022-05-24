/**
 * @file main.c
 * @brief In this demo. It shows how to use CRC to calculate the CRC value.If the result is success,
 * We can observe "crc success!" through serial port.
 * @author pantianwen (pantianwen@163.com)
 * @version 1.0
 * @date 2022-05-15
 * @copyright Copyright (c) 2022 Icore, Inc
 */
#include "at103.h"
uint32_t adc[] = {0x2};
void     main(void)
{
    pll_init();
    sys_io_init();
    uart_init(UART_BOOT_PORT, UART_PARITY_NONE, UART_STOPBITS_1, UART_DATABITS_8, UART_BOOT_BD);
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_CRC, ENABLE);

    CRC_Seed_Config(0);
    CRC_Input_Control(CRC_Not_Reverse);
    debug("result = %x\n", CRC_CalcCRC(0x12345678));
    if (CRC_CalcCRC(0x12345678) == 0xb42c) {
        debug("CRC success!");
    }

    debug("%x\n", CRC_CalcBlockCRC(adc, 1));
}
