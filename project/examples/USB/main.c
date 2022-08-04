/**
 * @file main.c
 * @brief helloworld example
 * @author Dong Qin (dong.qin@timesintelli.com)
 * @version 1.0
 * @date 2022-01-07
 * @copyright Copyright (c) 2022 Timesintelli, Inc
 */
#define REG16(x)  (*((volatile uint16_t *)(x)))
#define REG32(x)  (*((volatile uint32_t *)(x)))
#define USB_BAS   0x40028000
#define REG_POWER 0x1

#include "at103.h"

__NOT_IN_FLASH void main(void)
{
    pll_init();
    sys_io_init();
    uart_init(UART_BOOT_PORT, UART_PARITY_NONE, UART_STOPBITS_1, UART_DATABITS_8, UART_BOOT_BD);
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_USB, ENABLE);
    //debug("hello world!!!\r\n");
    REG32(USB_BAS + REG_POWER) = 0x60; /*soft connect*/
    // *((uint32_t *)(0x40028001)) = 0x60;
    // USB->POWER.EN_SUSPENDM      = 0x1;
    RCC->USBPHY_CTRL.value = 0x77;
    USB->POWER.EN_SUSPENDM = 0x1;
    USB->POWER.HS_ENAB     = 0x1;

    /* interrupt enable bits for each of the interrupts in IntrUSB*/
    USB->INTRUSBE.value = 0xff;

    USB->DEVCTL.

        debug("%x\n", USB->POWER.value);

    while (1) {
    }
}