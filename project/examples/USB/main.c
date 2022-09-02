/**
 * @file main.c
 * @brief helloworld example
 * @author Dong Qin (dong.qin@timesintelli.com)
 * @version 1.0
 * @date 2022-01-07
 * @copyright Copyright (c) 2022 Timesintelli, Inc
 */
#include "at103.h"
static const uint8_t musb_test_packet[53] = {
    /* implicit SYNC then DATA0 to start */

    /* JKJKJKJK x9 */
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    /* JJKKJJKK x8 */
    0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa,
    /* JJJJKKKK x8 */
    0xee, 0xee, 0xee, 0xee, 0xee, 0xee, 0xee, 0xee,
    /* JJJJJJJKKKKKKK x8 */
    0xfe, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    /* JJJJJJJK x8 */
    0x7f, 0xbf, 0xdf, 0xef, 0xf7, 0xfb, 0xfd,
    /* JKKKKKKK x10, JK */
    0xfc, 0x7e, 0xbf, 0xdf, 0xef, 0xf7, 0xfb, 0xfd, 0x7e

    /* implicit CRC16 then EOP to end */
};

#define REG8(x)   (*((volatile uint8_t *)(x)))
#define REG16(x)  (*((volatile uint16_t *)(x)))
#define REG32(x)  (*((volatile uint32_t *)(x)))
#define USB_BAS   0x40028000
#define REG_POWER 0x1
volatile unsigned int cnt;
volatile unsigned int start_addr = 0x14800;
volatile unsigned int next_addr;
volatile unsigned int usbAddr   = 0x57;
volatile unsigned int i0        = 0;
volatile unsigned int i1        = 0;
volatile unsigned int i2        = 0;
volatile unsigned int i3        = 0;
volatile unsigned int i4        = 0;
volatile unsigned int fail_flag = 0;

#ifndef RX_EP
volatile unsigned int rxEp = 1;
#else
volatile unsigned int rxEp = RX_EP;
#endif

volatile unsigned int rxMaxP = 0x40;

NVIC_InitTypeDef    NVIC_InitStructure;
GPIO_InitTypeDef    GPIOA_struct;
__NOT_IN_FLASH void main(void)
{
    uint8_t val = 0;
    pll_init();
    sys_io_init();
    uart_init(UART_BOOT_PORT, UART_PARITY_NONE, UART_STOPBITS_1, UART_DATABITS_8, UART_BOOT_BD);
    AFIO->GPIO[0].IC &= ~(0x3 << 27);
    GPIOA_struct.GPIO_Mode = GPIO_Mode_AIN;
    GPIOA_struct.GPIO_Pin  = GPIO_Pin_12 | GPIO_Pin_11;
    GPIO_Init(GPIOA, &GPIOA_struct);

    // RCC_OTGFSCLKConfig(0xB);
    RCC->USBPHY_CTRL.CLK_MODE_BIT   = 0x1;
    RCC->USBPHY_CTRL.PLL_EN_BIT     = 0x1;
    RCC->USBPHY_CTRL.IDDIG_BIT      = 0x1;
    RCC->USBPHY_CTRL.REFCLK_SEL_BIT = 0x1;
    /*USB AHB clk enable*/
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_USB, ENABLE);
    debug("xclk will be 60M after a few time!\n");
    RCC_AHBPeriphResetCmd(RCC_AHBPeriph_USB, ENABLE);
    debug("USB Dev wait xclk catch the rst.\n");
    RCC_AHBPeriphResetCmd(RCC_AHBPeriph_USB, DISABLE);
    debug("USB Dev wait rst finish.\n");

    RCC_OTGFSCLKConfig(0xB); //usb PHY ref clk(12M) div(TBD)
                             //6:iddig; 4:vbus sessend; 3:avalid; 2:vbus valid; 1:PHY PLL enable; 0:sel ref clk
    RCC->USBPHY_CTRL.value                               = (0x1 << 6) | (0x1 << 1) | (0x1 << 0) | (0x1 << 4);
    RCC->USBPHY_CTRL.value                               = (0x1 << 6) | (0x1 << 1) | (0x1 << 0) | (0x0 << 4) | (0x1 << 3) | (0x1 << 2);
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
    NVIC_InitStructure.NVIC_IRQChannel                   = USB_MC_IRQn;
    __enable_irq();

    cnt        = 0;
    start_addr = 0x14800;
    usbAddr    = 0x57;
    rxEp       = 1;
    rxMaxP     = 0x40;
    next_addr  = start_addr;
    i0         = 0;
    i1         = 0;
    i2         = 0;
    i3         = 0;
    i4         = 0;
    fail_flag  = 0;
    debug("\r\n");
    usb_dev_cfg_before_u_rst();
    REG8(USB_BAS + REG_DEVCTL) |= 0x1;
    while (1) {
        val = REG8(USB_BAS + REG_DEVCTL);

        if ((val & 0x10) == 0x10) {
            debug("Detect VBUS_VALID, USB_DEVCTL=%x\n", val);
            break;
        }
    }

    // REG8(USB_BAS + REG_TXCSRL) = 0x8;

    //REG8(USB_BAS + REG_TXCSRL) |= 0x1;
#if 1
    // *((uint32_t *)(0x40028001)) = 0x60;
    // USB->POWER.EN_SUSPENDM      = 0x1;

    //RCC->USBPHY_CTRL.value = 0x77;
    //USB->POWER.EN_SUSPENDM = 0x1;
    //USB->POWER.HS_ENAB     = 0x1;
    // REG8(USB_BAS + REG_POWER) |= 0x4;
    /* interrupt enable bits for each of the interrupts in IntrUSB*/
    //USB->INTRUSBE = 0xff;

    // REG8(USB_BASE + REG_POWER) = 0x40; /*soft connect*/
    // if (REG32(USB_BASE + REG_POWER) == 0x40)
    //     debug("soft connect complete!\n");

    //USB->INTRUSBE.value = 0xff;
    // USB->FADDR.value = 0x0;
    // USB->INDEX.SEL_EP = 0x1;

    // REG8(USB_BAS + REG_CSR0H) |= 0x1;
    // REG16(USB_BAS + REG_EP_TXMAXP) = 0x3;
    // REG16(USB_BAS + REG_EP_RXCSRL) = 0x110;
    // REG32(USB_BASE + REG_DMA_ADDR) = 0xfffffff;
    REG8(USB_BASE + REG_INDEX)     = 0xf;  //0x1;
    REG8(USB_BASE + REG_EP0_FIFO)  = 0x55; //0x12;
    REG32(USB_BASE + REG_EP1_FIFO) = 0x34;
    REG32(USB_BASE + REG_EP2_FIFO) = 0x56;
    REG8(USB_BASE + REG_CSR0L) |= 0x2;
    // USB->INTRRXE.value  = 0xfe;
    debug("REG_EP0_FIFO= %x\n", REG8(USB_BASE + REG_EP0_FIFO));
    debug("REG_EP1_FIFO= %x\n", REG8(USB_BASE + REG_EP1_FIFO));
    debug("REG_EP2_FIFO= %x\n", REG8(USB_BASE + REG_EP2_FIFO));
    debug("REG_EP3_FIFO= %x\n", REG8(USB_BASE + REG_EP3_FIFO));
    debug("REG_EP4_FIFO= %x\n", REG8(USB_BASE + REG_EP4_FIFO));
    debug("REG_EP5_FIFO= %x\n", REG8(USB_BASE + REG_EP5_FIFO));
    debug("REG_Raminfo= %x\n", REG8(USB_BASE + REG_Raminfo));
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
    debug("REG_EP0_FIFO = %x\n", REG8(USB_BASE + REG_EP0_FIFO));
    debug("REG_HWVERS = %x\n", REG32(USB_BASE + REG_HWVERS));
    //debug("REG_CSR0L = %x\n", REG32(USB_BASE + REG_CSR0L));
    while (1) {
        //  debug("REG_DEVCTL = %x\n", REG8(USB_BAS + REG_DEVCTL));
    }
}

usb_dev_cfg_before_u_rst()
{
    //REG16(USB_BAS + REG_C_T_UCH)  = 0x1100;
    REG16(USB_BAS + REG_INTRTXE) = 0x0;
    REG16(USB_BAS + REG_INTRRXE) = 0x0;
    REG8(USB_BAS + REG_INTRUSBE) = 0x0;
    REG8(USB_BAS + REG_TESTMODE) = 0x0;

    // REG16(USB_BAS + REG_LinkInfo) = 0x51;
    // REG8(USB_BAS + REG_INDEX)     = 0x0;
    // REG16(USB_BAS + REG_TXCSRL)   = 0x0100;
    // REG8(USB_BAS + REG_Raminfo);
    // REG8(USB_BAS + REG_INTRUSBE) = 0xfe;
    REG8(USB_BAS + REG_POWER) |= 0x4;
}

void USB_MC_IRQHandler(void)
{

    debug("enter interrupt!");
}
