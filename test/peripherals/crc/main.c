/**
 * @file main.c
 * @brief CRC test case.
 * @author pantianwen (pantianwen@163.com)
 * @version 1.0
 * @date 2022-06-02
 * @copyright Copyright (c) 2022 Icore, Inc
 */
#include "at103.h"
#include "unity.h"

void test_func(void)
{
    uint8_t  seed      = 0;
    uint8_t  input_ctl = 0;
    uint8_t  cal       = 0;
    uint32_t buf[1]    = {0x12345678};
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_CRC, ENABLE);
    CRC_SeedConfig(0);
    seed = CRC->CRCSEED.SEED;
    TEST_ASSERT_MESSAGE(seed == 0, "seed test failed!");
    CRC_InputControl(CRC_Not_Reverse);
    input_ctl = CRC->CRC_CTL.INVOUT;
    TEST_ASSERT_MESSAGE(input_ctl == 0, "input control test failed!");
    TEST_ASSERT_MESSAGE(CRC_CalcCRC(0x12345678) == 0xb42c, "CRC failed!");
    TEST_ASSERT_MESSAGE(CRC_GetCRC() == 0xb42c, "CRC get value failed!");
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_CRC, ENABLE);
    CRC_SeedConfig(0);
    CRC_InputControl(CRC_Not_Reverse);
    TEST_ASSERT_MESSAGE(CRC_CalcBlockCRC(buf, 1) == 0xb42c, "CRC_Calc Block failed!");
}

void unity_test(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_func);
    UNITY_END();
}

void main(void)
{
    pll_init();
    sys_io_init();
    uart_init(UART_BOOT_PORT, UART_PARITY_NONE, UART_STOPBITS_1, UART_DATABITS_8, UART_BOOT_BD);
    debug("CRC test start!\r\n");
    unity_test();
    while (1) {
    }
}
