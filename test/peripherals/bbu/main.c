/**
 * @file main.c
 * @brief BBU test case.
 * @author pantianwen (pantianwen@163.com)
 * @version 1.0
 * @date 2022-05-11
 * @copyright Copyright (c) 2022 Icore, Inc
 * 
 * Test function    : Test BBU all function and api in turn.
 * Board Version    ：AiP32rv001_EVB_V01
 * Hardware setup   : Without.
 */

#include "at103.h"
#include "unity.h"

void test_func(void)
{
    uint16_t         bkp1             = 0;
    uint16_t         tamperpin_cfg    = 0;
    uint16_t         tamper_cmd       = 0;
    uint16_t         tamperirq_enable = 0;
    uint16_t         PC13_ctrl        = 0;
    NVIC_InitTypeDef NVIC_InitStructure;
    RTC_SelClk(RTC_LSI);

    NVIC_InitStructure.NVIC_IRQChannel                   = TAMPER_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    __enable_irq();

    BKP_TamperInit();
    PC13_ctrl = RTC->PC13_IO_CONTRL.value;
    TEST_ASSERT_MESSAGE(PC13_ctrl == 0x3c, "tamper init failed!");
    BKP_TamperPinLevelConfig(BKP_TamperPinLevel_High);
    tamperpin_cfg = RTC->TAMP_CONFIG.DET_MODE;
    TEST_ASSERT_MESSAGE(tamperpin_cfg == BKP_TamperPinLevel_High, "bkp tamperpin level config failed!");
    BKP_TamperPinCmd(ENABLE);
    tamper_cmd = RTC->TAMP_CTRL.ENA;
    TEST_ASSERT_MESSAGE(tamper_cmd == ENABLE, "bkp tamper pin cmd enable failed!");
    BKP_TamperIrqEnable();
    tamperirq_enable = RTC->TAMP_IRQ_EN.ENA;
    TEST_ASSERT_MESSAGE(tamperirq_enable == 0x1, "bkp tamper irq enable failed!");
    BKP_WriteBackupRegister(BKP_DR1, 0x1A2B);
    bkp1 = BKP_ReadBackupRegister(BKP_DR1);
    TEST_ASSERT_MESSAGE(bkp1 == 0x1A2B, "write and read bkp failed!");
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
    debug("BBU test start!\n");
    unity_test();
    while (1) {
    }
}

void TAMPER_IRQHandler(void)
{
    debug("enter tamper interruot!\n");
    BKP_TamperClearIT();
}