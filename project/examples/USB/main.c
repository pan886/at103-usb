/**
 * @file main.c
 * @brief helloworld example
 * @author Dong Qin (dong.qin@timesintelli.com)
 * @version 1.0
 * @date 2022-01-07
 * @copyright Copyright (c) 2022 Timesintelli, Inc
 */
#include "at103.h"
uint16_t             abc                  = 0;
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
    NVIC_Init(&NVIC_InitStructure);
    __enable_irq();

    debug("\r\n");
    usb_dev_cfg_before_u_rst();
    //USB->FADDR.value = 0x123;
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
    //
    // USB->INDEX.SEL_EP = 0x1;
    //REG8(USB_BAS + REG_FADDR) = 0x1;
    // REG8(USB_BAS + REG_CSR0H) |= 0x1;
    // REG16(USB_BAS + REG_EP_TXMAXP) = 0x3;
    // REG16(USB_BAS + REG_EP_RXCSRL) = 0x110;
    // REG32(USB_BASE + REG_DMA_ADDR) = 0xfffffff;
    REG8(USB_BAS + REG_EP_RXFIFOS) |= 0x9;
    REG8(USB_BAS + REG_EP_RXFIFOA) = 0;

    REG16(USB_BAS + REG_INTRTXE) = 0x1;
    REG8(USB_BASE + REG_INDEX)   = 0x3; //0x1;

    // REG8(USB_BASE + REG_EP2_FIFO) = 0x56;
    // REG8(USB_BASE + REG_CSR0L) |= 0x3;
    REG8(USB_BAS + REG_INTRUSBE) = 0x0;
    REG8(USB_BAS + REG_POWER) &= ~0x4;
    REG8(USB_BAS + REG_POWER) &= ~0x20;
    // USB->INTRRXE.value  = 0xfe;

    // REG8(USB_BASE + REG_CSR0L) |= 0x81;
    REG8(USB_BASE + REG_CSR0H) |= 0xA0;
    //REG8(USB_BASE + REG_INDEX) |= 0xF;
    REG16(USB_BASE + REG_EP_TXMAXP) = 0x7ff;
    REG16(USB_BASE + REG_EP_RXMAXP) = 0x7ff;
    //REG8(USB_BASE + REG_EP1_FIFO)   = 0x34;

    REG8(USB_BASE + REG_EP_TXFIFOA) = 0x1FFF;
    //REG8(USB_BASE + REG_CSR0L) |= 0x3;
    // debug("REG_EP0_FIFO= %x\n", REG8(USB_BASE + REG_EP0_FIFO));
    // debug("REG_EP1_FIFO= %x\n", REG8(USB_BASE + REG_EP1_FIFO));
    // debug("REG_EP2_FIFO= %x\n", REG8(USB_BASE + REG_EP2_FIFO));
    // debug("REG_EP3_FIFO= %x\n", REG8(USB_BASE + REG_EP3_FIFO));
    // debug("REG_EP4_FIFO= %x\n", REG8(USB_BASE + REG_EP4_FIFO));
    // debug("REG_EP5_FIFO= %x\n", REG8(USB_BASE + REG_EP5_FIFO));
    debug("REG_Raminfo= %x\n", REG8(USB_BASE + REG_Raminfo));
    debug("FIFOSIZE = %x\n", REG8(USB_BAS + REG_FIFOSIZE));
    debug("FADDR.value = %x\n", USB->FADDR.value);
    debug("power.value = %x\n", REG8(USB_BASE + REG_POWER));
    debug("INTRTX.value= %x\n", USB->INTRTX.value);
    debug("REG_EP_RXCSRL.value = %x\n", REG32(USB_BASE + REG_EP_RXCSRL));
    debug("REG_DMA_ADDR = %x\n", REG32(USB_BASE + REG_DMA_ADDR));
    debug("REG_CSR0L = %x\n", REG8(USB_BASE + REG_CSR0L));
    debug("REG_INTRTX = %x\n", REG8(USB_BASE + REG_INTRTX));
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
    REG8(USB_BAS + REG_POWER) |= 0x4 | 0x80;
}

void USB_MC_IRQHandler(void)
{

    debug("enter interrupt!\n");
    //REG8(USB_BASE + REG_EP1_FIFO) = 0x56;
    // debug("REG_INTRUSB = %x\n", REG8(USB_BAS + REG_INTRUSB));
    //debug("REG_INTRUSBE= %x\n", REG8(USB_BAS + REG_INTRUSBE));
    //REG8(USB_BAS + REG_INTRUSBE) = 0x0;
    // REG8(USB_BASE + REG_CSR0L) |= 0x1;
    //REG8(USB_BAS + REG_CSR0H) |= 0x1;
    //  debug("REG_CSR0L = %x\n", REG16(USB_BASE + REG_CSR0L));
    debug("REG_EP0_FIFO = %x\n", REG8(USB_BASE + REG_EP0_FIFO));
    debug("REG_EP0_FIF1 = %x\n", REG8(USB_BASE + REG_EP0_FIFO));
    debug("REG_EP0_FIF2 = %x\n", REG8(USB_BASE + REG_EP0_FIFO));
    debug("REG_EP0_FIF3 = %x\n", REG8(USB_BASE + REG_EP0_FIFO));
    debug("REG_EP0_FIF4 = %x\n", REG8(USB_BASE + REG_EP0_FIFO));
    debug("REG_EP0_FIF5 = %x\n", REG8(USB_BASE + REG_EP0_FIFO));
    debug("REG_EP0_FIF6 = %x\n", REG8(USB_BASE + REG_EP0_FIFO));
    debug("REG_EP0_FIF7 = %x\n", REG8(USB_BASE + REG_EP0_FIFO));
    REG8(USB_BASE + REG_CSR0L) = 0x0;
    REG8(USB_BASE + REG_CSR0H) = 0x1;
    if (abc == 0) {

        // debug("REG_EP0_FIF8 = %x\n", REG8(USB_BASE + REG_EP0_FIFO));
        // debug("REG_EP0_FIF9 = %x\n", REG8(USB_BASE + REG_EP0_FIFO));
        // debug("REG_EP0_FIF10 = %x\n", REG8(USB_BASE + REG_EP0_FIFO));
        // debug("REG_EP0_FIF11 = %x\n", REG8(USB_BASE + REG_EP0_FIFO));
        // debug("REG_EP0_FIF12 = %x\n", REG8(USB_BASE + REG_EP0_FIFO));
        // debug("REG_EP0_FIF13 = %x\n", REG8(USB_BASE + REG_EP0_FIFO));
        // debug("REG_EP0_FIF14 = %x\n", REG8(USB_BASE + REG_EP0_FIFO));
        // debug("REG_EP0_FIF15 = %x\n", REG8(USB_BASE + REG_EP0_FIFO));
        // debug("REG_EP0_FIF16 = %x\n", REG8(USB_BASE + REG_EP0_FIFO));
        // debug("REG_EP0_FIF17 = %x\n", REG8(USB_BASE + REG_EP0_FIFO));
        // debug("REG_EP0_FIF18 = %x\n", REG8(USB_BASE + REG_EP0_FIFO));
        // debug("REG_EP0_FIF19 = %x\n", REG8(USB_BASE + REG_EP0_FIFO));
        // debug("REG_EP0_FIF20 = %x\n", REG8(USB_BASE + REG_EP0_FIFO));
        // debug("REG_EP0_FIF21 = %x\n", REG8(USB_BASE + REG_EP0_FIFO));
        // debug("REG_EP0_FIF22 = %x\n", REG8(USB_BASE + REG_EP0_FIFO));
        // debug("REG_EP0_FIF23 = %x\n", REG8(USB_BASE + REG_EP0_FIFO));
        // debug("REG_EP0_FIF24 = %x\n", REG8(USB_BASE + REG_EP0_FIFO));
        // debug("REG_EP0_FIF25 = %x\n", REG8(USB_BASE + REG_EP0_FIFO));
        // debug("REG_EP0_FIF26 = %x\n", REG8(USB_BASE + REG_EP0_FIFO));
        // debug("REG_EP0_FIF27 = %x\n", REG8(USB_BASE + REG_EP0_FIFO));
        // debug("REG_EP0_FIF28 = %x\n", REG8(USB_BASE + REG_EP0_FIFO));
        // debug("REG_EP0_FIF29 = %x\n", REG8(USB_BASE + REG_EP0_FIFO));
        // debug("REG_EP0_FIF30 = %x\n", REG8(USB_BASE + REG_EP0_FIFO));
        // debug("REG_EP0_FIF31 = %x\n", REG8(USB_BASE + REG_EP0_FIFO));
        // debug("REG_EP0_FIF32 = %x\n", REG8(USB_BASE + REG_EP0_FIFO));
        REG16(USB_BASE + REG_CSR0L) = 0x19;
        //REG8(USB_BASE + REG_CSR0L) |= 0x2;
        // REG8(USB_BASE + REG_CSR0H) |= 0x1;
        //debug("REG_CSR0L = %x\n", REG8(USB_BASE + REG_CSR0L));
        // REG8(USB_BASE + REG_EP_RXCSRL) |= 0x11;
        //REG8(USB_BASE + REG_CSR0H) |= 0x1;c
        REG8(USB_BASE + REG_EP0_FIFO) = 0x12;
        //REG8(USB_BASE + REG_CSR0L) |= 0x2;
        // debug("REG_EP0_FIF0 = %x\n", REG8(USB_BASE + REG_EP0_FIFO));
        REG8(USB_BASE + REG_EP0_FIFO) = 0x1;
        //REG8(USB_BASE + REG_CSR0L) |= 0x2;
        // debug("REG_EP0_FIF1 = %x\n", REG8(USB_BASE + REG_EP0_FIFO));
        REG8(USB_BASE + REG_EP0_FIFO) = 0x0;
        //REG8(USB_BASE + REG_CSR0L) |= 0x2;mk
        // debug("REG_EP0_FIF2 = %x\n", REG8(USB_BASE + REG_EP0_FIFO));
        REG8(USB_BASE + REG_EP0_FIFO) = 0x2;

        //   debug("REG_EP0_FIF3 = %x\n", REG8(USB_BASE + REG_EP0_FIFO));
        // REG8(USB_BASE + REG_CSR0L) |= 0x2;
        // REG8(USB_BASE + REG_CSR0H) |= 0x1;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x0;
        //debug("REG_EP0_FIF4 = %x\n", REG8(USB_BASE + REG_EP0_FIFO));
        REG8(USB_BASE + REG_EP0_FIFO) = 0x0;
        // debug("REG_EP0_FIF5 = %x\n", REG8(USB_BASE + REG_EP0_FIFO));
        REG8(USB_BASE + REG_EP0_FIFO) = 0x0;
        //   debug("REG_EP0_FIF6 = %x\n", REG8(USB_BASE + REG_EP0_FIFO));
        REG8(USB_BASE + REG_EP0_FIFO) = 0x40;
        //   debug("REG_EP0_FIF7 = %x\n", REG8(USB_BASE + REG_EP0_FIFO));
        REG8(USB_BASE + REG_EP0_FIFO) = 0xFF;
        //  debug("REG_EP0_FIF8 = %x\n", REG8(USB_BASE + REG_EP0_FIFO));
        REG8(USB_BASE + REG_EP0_FIFO) = 0xFF;
        //   debug("REG_EP0_FIF9 = %x\n", REG8(USB_BASE + REG_EP0_FIFO));
        REG8(USB_BASE + REG_EP0_FIFO) = 0x10;
        //  debug("REG_EP0_FIF10 = %x\n", REG8(USB_BASE + REG_EP0_FIFO));
        REG8(USB_BASE + REG_EP0_FIFO) = 0x57;
        // debug("REG_EP0_FIF11 = %x\n", REG8(USB_BASE + REG_EP0_FIFO));
        REG8(USB_BASE + REG_EP0_FIFO) = 0x0;
        //   debug("REG_EP0_FIF12 = %x\n", REG8(USB_BASE + REG_EP0_FIFO));
        REG8(USB_BASE + REG_EP0_FIFO) = 0x2;
        //  debug("REG_EP0_FIF13 = %x\n", REG8(USB_BASE + REG_EP0_FIFO));
        REG8(USB_BASE + REG_EP0_FIFO) = 0x1;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x2;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x3;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x1;
        REG8(USB_BASE + REG_CSR0L) |= 0x2;
        REG16(USB_BASE + REG_CSR0H) |= 0x1;

        // REG8(USB_BASE + REG_CSR0L) |= 0x10;
        // debug("REG_EP0_FIFO = %x\n", REG8(USB_BASE + REG_EP0_FIFO));
        // debug("REG_EP0_FIF1 = %x\n", REG8(USB_BASE + REG_EP0_FIFO));
        // debug("REG_EP0_FIF2 = %x\n", REG8(USB_BASE + REG_EP0_FIFO));
        // debug("REG_EP0_FIF3 = %x\n", REG8(USB_BASE + REG_EP0_FIFO));
        // debug("REG_EP0_FIF4 = %x\n", REG8(USB_BASE + REG_EP0_FIFO));
        // debug("REG_EP0_FIF5 = %x\n", REG8(USB_BASE + REG_EP0_FIFO));
        // debug("REG_EP0_FIF6 = %x\n", REG8(USB_BASE + REG_EP0_FIFO));
        // debug("REG_EP0_FIF7 = %x\n", REG8(USB_BASE + REG_EP0_FIFO));
        // debug("REG_EP0_FIF8 = %x\n", REG8(USB_BASE + REG_EP0_FIFO));
        // debug("REG_EP0_FIF9 = %x\n", REG8(USB_BASE + REG_EP0_FIFO));
        // debug("REG_EP0_FIF10 = %x\n", REG8(USB_BASE + REG_EP0_FIFO));
        // debug("REG_EP0_FIF11 = %x\n", REG8(USB_BASE + REG_EP0_FIFO));
        // debug("REG_EP0_FIF12 = %x\n", REG8(USB_BASE + REG_EP0_FIFO));
        // debug("REG_EP0_FIF13 = %x\n", REG8(USB_BASE + REG_EP0_FIFO));
        // debug("REG_EP0_FIF14 = %x\n", REG8(USB_BASE + REG_EP0_FIFO));
        // debug("REG_EP0_FIF15 = %x\n", REG8(USB_BASE + REG_EP0_FIFO));
        // debug("REG_EP0_FIF16 = %x\n", REG8(USB_BASE + REG_EP0_FIFO));
        // debug("REG_EP0_FIF17 = %x\n", REG8(USB_BASE + REG_EP0_FIFO));
        // debug("REG_EP0_FIF18 = %x\n", REG8(USB_BASE + REG_EP0_FIFO));
    }
    if ((abc >= 1) && (abc <= 1)) {
        debug("REG_EP0_FIFO = %x\n", REG8(USB_BASE + REG_EP0_FIFO));
        debug("REG_EP0_FIF1 = %x\n", REG8(USB_BASE + REG_EP0_FIFO));
        // debug("REG_EP0_FIF2 = %x\n", REG8(USB_BASE + REG_EP0_FIFO));
        REG8(USB_BASE + REG_FADDR) = REG8(USB_BASE + REG_EP0_FIFO);

        debug("REG_EP0_FIF3 = %x\n", REG8(USB_BASE + REG_EP0_FIFO));
        debug("REG_EP0_FIF4 = %x\n", REG8(USB_BASE + REG_EP0_FIFO));
        debug("REG_EP0_FIF5 = %x\n", REG8(USB_BASE + REG_EP0_FIFO));
        debug("REG_EP0_FIF6 = %x\n", REG8(USB_BASE + REG_EP0_FIFO));
        debug("REG_EP0_FIF7 = %x\n", REG8(USB_BASE + REG_EP0_FIFO));
        //REG8(USB_BASE + REG_CSR0L) |= 0x72;
        // REG8(USB_BASE + REG_CSR0L) |= 0x10;
        //REG8(USB_BASE + REG_CSR0H) |= 0x1;
        debug("faddr = %x\n", REG8(USB_BASE + REG_FADDR));
    }
    if (abc == 2) {
        // debug("REG_EP0_FIFO = %x\n", REG8(USB_BASE + REG_EP0_FIFO));
        // debug("REG_EP0_FIF1 = %x\n", REG8(USB_BASE + REG_EP0_FIFO));
        // debug("REG_EP0_FIF2 = %x\n", REG8(USB_BASE + REG_EP0_FIFO));
        // debug("REG_EP0_FIF3 = %x\n", REG8(USB_BASE + REG_EP0_FIFO));
        // debug("REG_EP0_FIF4 = %x\n", REG8(USB_BASE + REG_EP0_FIFO));
        // debug("REG_EP0_FIF5 = %x\n", REG8(USB_BASE + REG_EP0_FIFO));
        // debug("REG_EP0_FIF6 = %x\n", REG8(USB_BASE + REG_EP0_FIFO));
        // debug("REG_EP0_FIF7 = %x\n", REG8(USB_BASE + REG_EP0_FIFO));
        REG16(USB_BASE + REG_CSR0L)   = 0x19;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x12;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x1;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x0;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x2;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x0;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x0;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x0;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x40;
        REG8(USB_BASE + REG_EP0_FIFO) = 0xFF;
        REG8(USB_BASE + REG_EP0_FIFO) = 0xFF;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x10;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x57;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x0;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x2;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x1;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x2;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x3;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x1;
        REG8(USB_BASE + REG_CSR0L) |= 0x2;
        //REG16(USB_BASE + REG_CSR0H) |= 0x1;

        //  REG8(USB_BASE + REG_CSR0L) |= 0x10;
    }
    if (abc == 3) {
        REG8(USB_BASE + REG_EP0_FIFO) = 0x09;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x2;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x22;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x00;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x01;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x01;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x0;
        REG8(USB_BASE + REG_EP0_FIFO) = 0xe0;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x32;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x9;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x4;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x0;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x0;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x1;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x3;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x1;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x2;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x0;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x9;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x21;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x0;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x1;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x0;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x1;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x22;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x4A;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x0;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x7;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x5;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x81;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x3;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x4;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x0;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x20;

        REG8(USB_BASE + REG_CSR0L) |= 0x2;
    }

    if (abc == 4) {

        REG8(USB_BASE + REG_EP0_FIFO) = 0x1A;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x3;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x43;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x0;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x4D;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x0;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x33;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x0;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x32;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x0;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x4D;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x0;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x31;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x0;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x30;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x0;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x31;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x0;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x41;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x0;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x0;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x0;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x0;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x0;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x0;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x0;
        REG8(USB_BASE + REG_CSR0L) |= 0x2;
    }

    if ((abc == 5)) {
        REG8(USB_BASE + REG_CSR0H) |= 0x1;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x4;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x3;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x9;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x4;
        //REG8(USB_BASE + REG_EP0_FIFO) = 0x20;

        REG8(USB_BASE + REG_CSR0L) |= 0x2;
    }

    // if (abc == 6) {
    //     REG8(USB_BASE + REG_CSR0H) |= 0x1;
    //     REG8(USB_BASE + REG_EP0_FIFO) = 0x4;
    //     REG8(USB_BASE + REG_EP0_FIFO) = 0x3;
    //     REG8(USB_BASE + REG_EP0_FIFO) = 0x9;
    //     REG8(USB_BASE + REG_EP0_FIFO) = 0x3;

    //     REG8(USB_BASE + REG_CSR0L) |= 0x2;
    // }

    if (abc == 6) {

        REG8(USB_BASE + REG_EP0_FIFO) = 0x26;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x3;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x64;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x27;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x4d;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x0;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x33;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x0;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x32;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x0;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x4d;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x0;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x31;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x0;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x30;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x0;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x31;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x0;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x41;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x0;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x20;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x0;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x4A;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x0;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x6f;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x0;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x79;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x0;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x73;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x0;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x74;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x0;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x69;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x0;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x63;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x0;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x6B;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x0;
        REG8(USB_BASE + REG_CSR0L) |= 0x2;
    }
    if (abc == 7) {
        REG8(USB_BASE + REG_EP0_FIFO);
        REG8(USB_BASE + REG_EP0_FIFO);
        REG8(USB_BASE + REG_EP0_FIFO);
        REG8(USB_BASE + REG_EP0_FIFO);
        REG8(USB_BASE + REG_EP0_FIFO);
        REG8(USB_BASE + REG_EP0_FIFO);
        REG8(USB_BASE + REG_EP0_FIFO);
        REG8(USB_BASE + REG_EP0_FIFO);
    }

    if (abc == 8) {
        REG8(USB_BASE + REG_CSR0H) |= 0x1;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x4;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x3;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x9;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x3;

        REG8(USB_BASE + REG_CSR0L) |= 0x2;
    }
    if ((abc >= 9) && (abc <= 11)) {
        REG8(USB_BASE + REG_CSR0H) |= 0x1;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x4;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x3;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x9;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x4;

        REG8(USB_BASE + REG_CSR0L) |= 0x2;
    }
    if ((abc == 12)) {
        REG8(USB_BASE + REG_CSR0H) |= 0x1;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x26;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x3;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x43;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x00;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x4D;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x00;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x49;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x00;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x4F;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x00;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x54;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x00;

        REG8(USB_BASE + REG_EP0_FIFO) = 0x00;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x00;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x00;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x00;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x00;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x00;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x00;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x00;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x00;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x00;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x00;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x00;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x00;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x00;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x00;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x00;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x00;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x00;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x00;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x00;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x00;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x00;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x00;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x00;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x00;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x00;
        REG8(USB_BASE + REG_CSR0L) |= 0x2;
    }
    if ((abc == 14)) {
        REG8(USB_BASE + REG_EP0_FIFO) = 0x26;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x3;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x43;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x0;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x4d;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x0;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x33;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x0;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x32;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x0;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x4d;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x0;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x31;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x0;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x30;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x0;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x31;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x0;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x41;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x0;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x20;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x0;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x4A;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x0;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x6f;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x0;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x79;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x0;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x73;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x0;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x74;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x0;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x69;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x0;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x63;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x0;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x6B;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x0;
        REG8(USB_BASE + REG_CSR0L) |= 0x2;
    }
    if ((abc >= 15) && (abc < 16)) {
        REG8(USB_BASE + REG_CSR0H) |= 0x1;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x9;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x2;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x22;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x0;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x1;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x1;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x0;
        REG8(USB_BASE + REG_EP0_FIFO) = 0xe0;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x32;

        REG8(USB_BASE + REG_CSR0L) |= 0x2;
    }

    if ((abc == 17)) {
        REG8(USB_BASE + REG_CSR0H) |= 0x1;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x26;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x3;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x43;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x00;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x4D;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x00;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x49;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x00;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x4F;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x00;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x54;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x00;

        REG8(USB_BASE + REG_EP0_FIFO) = 0x00;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x00;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x00;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x00;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x00;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x00;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x00;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x00;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x00;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x00;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x00;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x00;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x00;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x00;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x00;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x00;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x00;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x00;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x00;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x00;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x00;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x00;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x00;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x00;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x00;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x00;
        REG8(USB_BASE + REG_CSR0L) |= 0x2;
    }

#if 0
    if (abc == 8) {
        REG8(USB_BASE + REG_CSR0H) |= 0x1;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x4;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x3;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x9;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x4;

        REG8(USB_BASE + REG_CSR0L) |= 0x2;
    }

    if ((abc == 9) || (abc == 10)) {
        REG8(USB_BASE + REG_CSR0H) |= 0x1;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x4;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x3;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x9;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x4;

        REG8(USB_BASE + REG_CSR0L) |= 0x2;
    }
    // if ((abc == 11)) {
    //     REG8(USB_BASE + REG_CSR0H) |= 0x1;
    //     REG8(USB_BASE + REG_EP0_FIFO) = 0x9;
    //     REG8(USB_BASE + REG_EP0_FIFO) = 0x2;
    //     REG8(USB_BASE + REG_EP0_FIFO) = 0x22;
    //     REG8(USB_BASE + REG_EP0_FIFO) = 0x0;
    //     REG8(USB_BASE + REG_EP0_FIFO) = 0x1;
    //     REG8(USB_BASE + REG_EP0_FIFO) = 0x1;
    //     REG8(USB_BASE + REG_EP0_FIFO) = 0x0;
    //     REG8(USB_BASE + REG_EP0_FIFO) = 0xe0;
    //     REG8(USB_BASE + REG_EP0_FIFO) = 0x32;

    //     REG8(USB_BASE + REG_CSR0L) |= 0x2;
    // }
    //     if (abc == 11) {
    // REG8(USB_BASE + REG_EP0_FIFO) = 0x

    //     }

    if ((abc == 11)) {
        REG8(USB_BASE + REG_CSR0H) |= 0x1;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x26;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x3;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x43;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x00;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x4D;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x00;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x49;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x00;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x4F;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x00;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x54;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x00;

        REG8(USB_BASE + REG_EP0_FIFO) = 0x00;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x00;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x00;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x00;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x00;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x00;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x00;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x00;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x00;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x00;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x00;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x00;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x00;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x00;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x00;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x00;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x00;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x00;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x00;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x00;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x00;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x00;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x00;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x00;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x00;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x00;
        REG8(USB_BASE + REG_CSR0L) |= 0x2;
    }

    if ((abc == 13)) {
        REG8(USB_BASE + REG_CSR0H) |= 0x1;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x9;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x2;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x22;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x0;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x1;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x1;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x0;
        REG8(USB_BASE + REG_EP0_FIFO) = 0xe0;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x32;

        REG8(USB_BASE + REG_CSR0L) |= 0x2;
    }
    if ((abc == 14)) {
        REG8(USB_BASE + REG_CSR0H) |= 0x1;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x09;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x2;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x22;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x00;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x01;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x01;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x0;
        REG8(USB_BASE + REG_EP0_FIFO) = 0xe0;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x32;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x9;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x4;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x0;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x0;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x1;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x3;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x1;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x2;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x0;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x9;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x21;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x0;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x1;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x0;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x1;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x22;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x4A;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x0;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x7;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x5;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x81;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x3;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x4;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x0;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x20;

        REG8(USB_BASE + REG_CSR0L) |= 0x2;
    }
#endif
    debug("REG_EP_RXCSRL = %x\n", REG8(USB_BASE + REG_EP_RXCSRL));
    if (abc <= 0xff)
        abc++;
}
