/**
 * @file main.c
 * @brief This demo shows how to write/read BKP register and use tamper. connect PC13 to GND, then disconnect GND,
 * The tamper dectector enter interrupt,the interrupt can be observed and printed through the serial port.and the BKP registers are clean.
 * @author pantianwen (pantianwen@163.com)
 * @version 1.0
 * @date 2022-04-12
 * @copyright Copyright (c) 2022 Icore, Inc
 */

#include "at103.h"

void main(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;

    pll_init();
    sys_io_init();
    uart_init(UART_BOOT_PORT, UART_PARITY_NONE, UART_STOPBITS_1, UART_DATABITS_8, UART_BOOT_BD);

    RTCCLK_Select(RTC_LSE);

    NVIC_InitStructure.NVIC_IRQChannel                   = TAMPER_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    __enable_irq();

    Tamper_Init();
    BKP_TamperPinLevelConfig(BKP_TamperPinLevel_High);
    BKP_TamperPinCmd(ENABLE);
    TAMPER_Irq_Enable();
    BKP_WriteBackupRegister(BKP_DR1, 0x1A2B);

    while (1) {
        debug("BKP1 = %x\n", BKP_ReadBackupRegister(BKP_DR1));
    }
}

void TAMPER_IRQHandler(void)
{
    debug("enter tamper interruot!\n");
    BKP_ClearIT_Tamper();
}