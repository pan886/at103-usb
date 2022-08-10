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

    // *((uint32_t *)(0x40028001)) = 0x60;
    // USB->POWER.EN_SUSPENDM      = 0x1;
    RCC->USBPHY_CTRL.value = 0x77;
    USB->POWER.EN_SUSPENDM = 0x1;
    USB->POWER.HS_ENAB     = 0x1;

    /* interrupt enable bits for each of the interrupts in IntrUSB*/
    //USB->INTRUSBE = 0xff;
    //debug("INTRUSBE.value = %x\n", REG32(USB_BASE + 0xb));

    //REG32(USB_BASE + REG_POWER) = 0x60; /*soft connect*/
    USB->POWER.value = 0x60;
    //USB->DEVCTL.
    REG32(USB_BAS + REG_INTRUSBE) = 0xfe;
    //USB->INTRUSBE.value = 0xff;
    USB->FADDR.value = 0x15;
    // USB->INDEX.SEL_EP = 0x1;
    REG32(USB_BAS + REG_INDEX)     = 0x1;
    REG32(USB_BAS + REG_EP_TXMAXP) = 0x3;
    // USB->INTRRXE.value  = 0xfe;

    debug("FADDR.value = %x\n", USB->FADDR.value);
    debug("power.value = %x\n", USB->POWER.value);
    debug("INTRTX.value = %x\n", USB->INTRTX.value);
    debug("INTRRX.value = %x\n", USB->INTRRX.value);
    debug("INTRTXE.value = %x\n", USB->INTRTXE.value);
    debug("INTRRXE.value = %x\n", USB->INTRRXE.value);
    debug("INTRUSB.value = %x\n", USB->INTRUSB.value);
    debug("INTRUSBE.value = %x\n", USB->INTRUSBE);
    debug("INTRUSBE.value = %x\n", REG16(USB_BAS + 0xb));
    debug("REG_EP_FIFOBITS = %x\n", REG32(USB_BAS + REG_EP_FIFOBITS));
    debug("REG_DEVCTL = %x\n", REG32(USB_BAS + REG_DEVCTL));
    debug("REG_INDEX = %x\n", REG32(USB_BAS + REG_INDEX));

    debug("REG_EPINFO = %x\n", REG16(USB_BAS + REG_EPINFO));
    while (1) {
    }
}