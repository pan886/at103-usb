/**
 * @file main.c
 * @brief helloworld example
 * @author Dong Qin (dong.qin@timesintelli.com)
 * @version 1.0
 * @date 2022-01-07
 * @copyright Copyright (c) 2022 Timesintelli, Inc
 */
#define REG8(x)   (*((volatile uint8_t *)(x)))
#define REG16(x)  (*((volatile uint16_t *)(x)))
#define REG32(x)  (*((volatile uint32_t *)(x)))
#define USB_BAS   0x40028000
#define REG_POWER 0x1

#include "at103.h"
NVIC_InitTypeDef    NVIC_InitStructure;
GPIO_InitTypeDef    GPIOA_struct;
__NOT_IN_FLASH void main(void)
{
    pll_init();
    sys_io_init();
    uart_init(UART_BOOT_PORT, UART_PARITY_NONE, UART_STOPBITS_1, UART_DATABITS_8, UART_BOOT_BD);
    GPIOA_struct.GPIO_Mode = GPIO_Mode_AIN;
    GPIOA_struct.GPIO_Pin  = GPIO_Pin_12 | GPIO_Pin_11;
    GPIO_Init(GPIOA, &GPIOA_struct);

    RCC_OTGFSCLKConfig(0xB);
    RCC->USBPHY_CTRL.CLK_MODE_BIT   = 0x1;
    RCC->USBPHY_CTRL.PLL_EN_BIT     = 0x1;
    RCC->USBPHY_CTRL.IDDIG_BIT      = 0x1;
    RCC->USBPHY_CTRL.REFCLK_SEL_BIT = 0x1;
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_USB, ENABLE);

    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
    NVIC_InitStructure.NVIC_IRQChannel                   = USB_MC_IRQn;
    __enable_irq();

    debug("\r\n");
#if 1
    // *((uint32_t *)(0x40028001)) = 0x60;
    // USB->POWER.EN_SUSPENDM      = 0x1;

    RCC->USBPHY_CTRL.value = 0x77;
    USB->POWER.EN_SUSPENDM = 0x1;
    USB->POWER.HS_ENAB     = 0x1;

    /* interrupt enable bits for each of the interrupts in IntrUSB*/
    //USB->INTRUSBE = 0xff;

    REG8(USB_BASE + REG_POWER) = 0x40; /*soft connect*/
    if (REG32(USB_BASE + REG_POWER) == 0x40)
        debug("soft connect complete!\n");
    //USB->DEVCTL.
    REG16(USB_BAS + REG_INTRRXE) = 0xfe;
    REG16(USB_BAS + REG_INTRTXE) = 0xff;
    REG8(USB_BAS + REG_INTRUSBE) = 0xfe;
    //USB->INTRUSBE.value = 0xff;
    USB->FADDR.value = 0x0;
    // USB->INDEX.SEL_EP = 0x1;
    REG8(USB_BAS + REG_INDEX) = 0x0;
    REG8(USB_BAS + REG_CSR0H) |= 0x1;
    REG16(USB_BAS + REG_EP_TXMAXP) = 0x3;
    REG16(USB_BAS + REG_EP_RXCSRL) = 0x110;
    REG32(USB_BASE + REG_DMA_ADDR) = 0xfffffff;
    REG32(USB_BASE + REG_EP0_FIFO) = 0x12;

    // USB->INTRRXE.value  = 0xfe;
    debug("FIFOSIZE = %x\n", REG8(USB_BAS + REG_FIFOSIZE));
    debug("FADDR.value = %x\n", USB->FADDR.value);
    debug("power.value = %x\n", REG32(USB_BASE + REG_POWER));
    debug("INTRTX.value= %x\n", USB->INTRTX.value);
    debug("REG_EP_RXCSRL.value = %x\n", REG32(USB_BASE + REG_EP_RXCSRL));
    debug("REG_DMA_ADDR = %x\n", REG32(USB_BASE + REG_DMA_ADDR));
    if (USB->INTRTX.value == 0x1) /*active interrupts are cleared when this register is read.*/
    {
        debug("Endpoint 0 interrupt\n");
    }
    debug("REG_EP_TXMAXP = %x\n", REG16(USB_BAS + REG_EP_TXMAXP));
    debug("REG_EP0_COUNT = %x\n", REG8(USB_BAS + REG_EP0_COUNT));
    // USB->TESTMODE.value = 0x1 << 3;
#endif
    debug("REG_EPINFO = %x\n", REG8(USB_BAS + REG_EPINFO));
    debug("INTRRX.value = %x\n", USB->INTRRX.value);
    debug("INTRTXE.value = %x\n", USB->INTRTXE.value);
    debug("INTRRXE.value = %x\n", REG16(USB_BAS + REG_INTRRXE));
    debug("INTRUSB.value = %x\n", USB->INTRUSB.value);
    debug("INTRUSBE.value = %x\n", REG16(USB_BAS + 0xb));

    debug("REG_DEVCTL = %x\n", REG8(USB_BAS + REG_DEVCTL));
    debug("REG_INDEX = %x\n", REG16(USB_BAS + REG_INDEX));
    //debug("REG_CSR0H = %x\n", REG16(USB_BAS + REG_CSR0H));
    debug("REG_EP0_COUNT = %x\n", REG8(USB_BAS + REG_EP0_COUNT));
    debug("REG_EP0_FIFO = %x\n", REG32(USB_BASE + REG_EP0_FIFO));
    debug("REG_HWVERS = %x\n", REG32(USB_BASE + REG_HWVERS));
    debug("REG_CSR0L = %x\n", REG32(USB_BASE + REG_CSR0L));
    while (1) {
    }
}