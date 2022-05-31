/**
 * @file main.c
 * @brief This demo shows how to use RTC module.The current time can be printed out through the serial port.
 * Alarms can be set to generate interrupt.
 * @author pantianwen (pantianwen@163.com)
 * @version 1.0
 * @date 2022-04-12
 * @copyright Copyright (c) 2022 Icore, Inc
 */

#include "at103.h"
uint32_t        bkp = 0;
uint32_t        min, sec, hour, year = 0;
RTC_TimeTypeDef timeset;
RTC_TimeTypeDef time_alarm1;

void main(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;

    pll_init();
    sys_io_init();
    uart_init(UART_BOOT_PORT, UART_PARITY_NONE, UART_STOPBITS_1, UART_DATABITS_8, UART_BOOT_BD);

    RTC_SelClk(RTC_LSE);
    NVIC_InitStructure.NVIC_IRQChannel                   = RTC_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    __enable_irq();

    time_alarm1.sec  = 0x20;
    time_alarm1.min  = 0x5;
    time_alarm1.hour = 0x6;
    /*set the alarm1*/
    RTC_SetAlarm(time_alarm1);
    /*set the interrupt output every 1/2 sec */
    RTC_SetSecINT(RTC_PERIOD_1_sec_2);

    timeset.sec  = 0x3;
    timeset.min  = 0x6;
    timeset.hour = 0x9;
    /*set RTC */
    RTC_SetTimeDate(timeset);

    RTC->RTC_RDSTA_A7.RDSTA = 0x1;
    min                     = RTC->MINUTE.MIN;
    sec                     = RTC->SECOND.SEC;
    hour                    = RTC->HOUR.HOUR19;

    RTC->RTC_RDSTP_A7.RDSTP = 0x1;

    debug("sec = %x\n", sec);
    debug("min = %x\n", min);
    debug("hour = %x\n", hour);
    while (1) {
    }
}

void RTC_IRQHandler(void)
{
    debug("enter RTC intrrupt!\n");
    RTC_ClearITPendingBit(RTC_IT_ALR1);
    RTC_ClearITPendingBit(RTC_IT_ALR2);
}
