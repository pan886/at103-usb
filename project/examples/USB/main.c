/**
 * @file main.c
 * @brief helloworld example
 * @author Dong Qin (dong.qin@timesintelli.com)
 * @version 1.0
 * @date 2022-01-07
 * @copyright Copyright (c) 2022 Timesintelli, Inc
 */
#include "at103.h"
uint16_t abc = 0;
uint16_t i, j = 0;
#define REG8(x)   (*((volatile uint8_t *)(x)))
#define REG16(x)  (*((volatile uint16_t *)(x)))
#define REG32(x)  (*((volatile uint32_t *)(x)))
#define USB_BAS   0x40028000
#define REG_POWER 0x1
void tfp_printf(char *fmt, ...);

#define printf tfp_printf
uint8_t          fifo_0    = 0;
uint8_t          fifo_1    = 0;
uint8_t          fifo_2    = 0;
uint8_t          fifo_3    = 0;
uint8_t          fifo_4    = 0;
uint8_t          fifo_5    = 0;
uint8_t          fifo_6    = 0;
uint8_t          fifo_7    = 0;
uint8_t          fifo_8    = 0;
uint8_t          fifo_9    = 0;
uint8_t          fifo_10   = 0;
uint8_t          fifo_11   = 0;
uint8_t          fifo_12   = 0;
uint8_t          fifo_13   = 0;
uint8_t          fifo_14   = 0;
uint8_t          fifo_15   = 0;
uint8_t          stop_flag = 0;
NVIC_InitTypeDef NVIC_InitStructure;
GPIO_InitTypeDef GPIOA_struct;

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
#if 1
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

    /* interrupt enable bits for each of the interrupts in IntrUSB*/
    //USB->INTRUSBE = 0xff;

    // REG8(USB_BASE + REG_POWER) = 0x40; /*soft connect*/
    // if (REG32(USB_BASE + REG_POWER) == 0x40)
    //     debug("soft connect complete!\n");

    //USB->INTRUSBE.value = 0xff;

    REG8(USB_BAS + REG_EP_RXFIFOS) |= 0x9;
    REG8(USB_BAS + REG_EP_RXFIFOA) = 0;

    REG16(USB_BAS + REG_INTRTXE) = 0x3; //0x1;
    REG8(USB_BASE + REG_INDEX)   = 0x1;

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

#endif
    debug("REG_EPINFO = %x\n", REG8(USB_BAS + REG_EPINFO));
    debug("INTRRX.value = %x\n", USB->INTRRX.value);
    debug("INTRTXE.value = %x\n", USB->INTRTXE.value);
    debug("INTRRXE.value = %x\n", REG16(USB_BAS + REG_INTRRXE));
    debug("INTRUSB.value = %x\n", USB->INTRUSB.value);
    debug("INTRUSBE.value = %x\n", REG16(USB_BAS + 0xb));

    debug("REG_DEVCTL = %x\n", REG8(USB_BAS + REG_DEVCTL));
    debug("REG_INDEX = %x\n", REG16(USB_BAS + REG_INDEX));

    debug("REG_EP0_COUNT = %x\n", REG8(USB_BAS + REG_EP0_COUNT));
    debug("REG_EP0_FIFO = %x\n", REG8(USB_BASE + REG_EP0_FIFO));

    debug("REG_HWVERS = %x\n", REG32(USB_BASE + REG_HWVERS));
#endif
    // printf("abc");

    while (1) {
        // REG8(USB_BASE + REG_EP1_FIFO) = 0x0;
        // REG8(USB_BASE + REG_EP1_FIFO) = 0x14;
        // REG8(USB_BASE + REG_EP1_FIFO) = 0x0;
        // REG8(USB_BASE + REG_EP1_FIFO) = 0x0;
        //debug("jump!\n");
        printf("sb!\n");
    }
}

usb_dev_cfg_before_u_rst()
{
    //REG16(USB_BAS + REG_C_T_UCH)  = 0x1100;
    REG16(USB_BAS + REG_INTRTXE) = 0x0;
    REG16(USB_BAS + REG_INTRRXE) = 0x0;
    REG8(USB_BAS + REG_INTRUSBE) = 0x0;
    REG8(USB_BAS + REG_TESTMODE) = 0x0;

    REG8(USB_BAS + REG_POWER) |= 0x4 | 0x80;
}

#if 1
void USB_MC_IRQHandler(void)
{

    debug("enter interrupt!\n");
    printf("REG_CSR0L = %x\n", REG8(USB_BAS + REG_CSR0L));
    if (REG8(USB_BAS + REG_CSR0L) & 0x1) {
        /*  clear the RxPktRdy bit. */
        //  REG8(USB_BASE + REG_CSR0L) |= 0x1 << 6;
        fifo_0 = REG8(USB_BASE + REG_EP0_FIFO);
        fifo_1 = REG8(USB_BASE + REG_EP0_FIFO);
        fifo_2 = REG8(USB_BASE + REG_EP0_FIFO);
        fifo_3 = REG8(USB_BASE + REG_EP0_FIFO);
        fifo_4 = REG8(USB_BASE + REG_EP0_FIFO);
        fifo_5 = REG8(USB_BASE + REG_EP0_FIFO);
        fifo_6 = REG8(USB_BASE + REG_EP0_FIFO);
        fifo_7 = REG8(USB_BASE + REG_EP0_FIFO);
        /*  clear the RxPktRdy bit. */
        REG8(USB_BASE + REG_CSR0L) |= 0x1 << 6;
        // REG8(USB_BAS + REG_CSR0L) |= 0x1 << 7;
        REG8(USB_BASE + REG_CSR0L) = 0x0;
        REG8(USB_BASE + REG_CSR0H) = 0x1;

    } else {
        REG8(USB_BAS + REG_CSR0L) |= 0x1 << 3;
        /*  clear the RxPktRdy bit. */
        REG8(USB_BASE + REG_CSR0L) |= 0x1 << 6;
    }

    //REG8(USB_BASE + REG_CSR0L) |= 0x1 << 3;
    // if (stop_flag == 0x1) {
    //     REG8(USB_BASE + REG_CSR0L) |= 0x1 << 3;
    // }

    // REG8(USB_BASE + REG_EP0_FIFO);

    if ((fifo_0 == 0x80) && (fifo_1 == 0x6) && (fifo_2 == 0) && (fifo_3 == 0x1) && (fifo_4 == 0) && (fifo_5 == 0) && (fifo_6 == 0x40)) {
        REG8(USB_BASE + REG_CSR0H)    = 0x1;
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
        //REG8(USB_BASE + REG_CSR0L) |= 0x1 << 3;
        //  REG8(USB_BASE + REG_CSR0L) |= 0x1 << 6;
        //    REG16(USB_BASE + REG_CSR0H) |= 0x1;
    }

    if ((fifo_0 == 0) && (fifo_1 == 0x5)) {
        // REG16(USB_BASE + REG_CSR0H) |= 0x1;
        //debug("REG_EP0_FIF1 = %x\n");
        // debug("REG_EP0_FIFO = %x\n", REG8(USB_BASE + REG_EP0_FIFO));
        // debug("REG_EP0_FIF1 = %x\n", REG8(USB_BASE + REG_EP0_FIFO));

        //REG8(USB_BASE + REG_EP0_FIFO);
        // while (REG8(USB_BASE + REG_FADDR) != fifo_2)
        //     ;
        // debug("REG_EP0_FIF3 = %x\n", REG8(USB_BASE + REG_EP0_FIFO));
        // debug("REG_EP0_FIF4 = %x\n", REG8(USB_BASE + REG_EP0_FIFO));
        // debug("REG_EP0_FIF5 = %x\n", REG8(USB_BASE + REG_EP0_FIFO));
        // debug("REG_EP0_FIF6 = %x\n", REG8(USB_BASE + REG_EP0_FIFO));
        // debug("REG_EP0_FIF7 = %x\n", REG8(USB_BASE + REG_EP0_FIFO));
        //REG8(USB_BASE + REG_CSR0L) |= 0x72;
        // REG8(USB_BASE + REG_CSR0L) |= 0x10;
        //REG8(USB_BASE + REG_CSR0H) |= 0x1;
        debug("fifo_0 = %x\n", fifo_0);
        debug("fifo_1 = %x\n", fifo_1);
        debug("fifo_2 = %x\n", fifo_2);
        debug("fifo_3 = %x\n", fifo_3);
        debug("fifo_4 = %x\n", fifo_4);
        debug("fifo_5 = %x\n", fifo_5);
        debug("fifo_6 = %x\n", fifo_6);
        debug("fifo_7 = %x\n", fifo_7);
        REG8(USB_BASE + REG_FADDR) = fifo_2;
        debug("faddr = %x\n", REG8(USB_BASE + REG_FADDR));
    }

    if ((fifo_1 == 0x6) && (fifo_3 == 0x1)) {
        REG16(USB_BASE + REG_CSR0H) |= 0x1;

        REG8(USB_BASE + REG_EP0_FIFO) = 0x12;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x1;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x0;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x2;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x0;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x0;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x2;
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
        //REG8(USB_BASE + REG_CSR0L) |= 0x1 << 3;
    }
    /*config*/
    if ((fifo_2 == 0) && (fifo_3 == 0x2) && (fifo_4 == 0) && (fifo_5 == 0)) {
        REG16(USB_BASE + REG_CSR0H) |= 0x1;
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
        // REG8(USB_BASE + REG_CSR0L) |= 0x1 << 3;
        //REG8(USB_BASE + REG_CSR0L) |= 0x1 << 6;
    }
    // /*serial number*/
    // if ((fifo_1 == 0x6) && (fifo_2 == 0x3) && (fifo_3 == 0x3) && (fifo_4 == 0x9) && (fifo_5 == 0x4)) {
    //     REG16(USB_BASE + REG_CSR0H) |= 0x1;
    //     REG8(USB_BASE + REG_EP0_FIFO) = 0x1A;
    //     REG8(USB_BASE + REG_EP0_FIFO) = 0x3;
    //     REG8(USB_BASE + REG_EP0_FIFO) = 0x41;
    //     REG8(USB_BASE + REG_EP0_FIFO) = 0x0;
    //     REG8(USB_BASE + REG_EP0_FIFO) = 0x69;
    //     REG8(USB_BASE + REG_EP0_FIFO) = 0x0;
    //     REG8(USB_BASE + REG_EP0_FIFO) = 0x50;
    //     REG8(USB_BASE + REG_EP0_FIFO) = 0x0;
    //     REG8(USB_BASE + REG_EP0_FIFO) = 0x33;
    //     REG8(USB_BASE + REG_EP0_FIFO) = 0x0;
    //     REG8(USB_BASE + REG_EP0_FIFO) = 0x32;
    //     REG8(USB_BASE + REG_EP0_FIFO) = 0x0;
    //     REG8(USB_BASE + REG_EP0_FIFO) = 0x52;
    //     REG8(USB_BASE + REG_EP0_FIFO) = 0x0;
    //     REG8(USB_BASE + REG_EP0_FIFO) = 0x56;
    //     REG8(USB_BASE + REG_EP0_FIFO) = 0x0;
    //     REG8(USB_BASE + REG_EP0_FIFO) = 0x31;
    //     REG8(USB_BASE + REG_EP0_FIFO) = 0x0;
    //     REG8(USB_BASE + REG_EP0_FIFO) = 0x35;
    //     REG8(USB_BASE + REG_EP0_FIFO) = 0x0;
    //     REG8(USB_BASE + REG_EP0_FIFO) = 0x58;
    //     REG8(USB_BASE + REG_EP0_FIFO) = 0x0;
    //     REG8(USB_BASE + REG_EP0_FIFO) = 0x0;
    //     REG8(USB_BASE + REG_EP0_FIFO) = 0x0;
    //     REG8(USB_BASE + REG_EP0_FIFO) = 0x0;
    //     REG8(USB_BASE + REG_EP0_FIFO) = 0x0;
    //     REG8(USB_BASE + REG_CSR0L) |= 0x2;
    // }
    /* lang ID*/
    if ((fifo_0 == 0x80) && (fifo_1 == 0x6) && (fifo_3 == 0x3) && (fifo_6 == 0xff)) {
        REG8(USB_BASE + REG_CSR0H) |= 0x1;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x4;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x3;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x9;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x4;

        REG8(USB_BASE + REG_CSR0L) |= 0x2;
        //REG8(USB_BASE + REG_CSR0L) |= 0x1 << 3;
    }
    /*iproduct*/
    if ((fifo_2 == 0x2) && (fifo_3 == 0x3) && (fifo_4 == 0x9) && (fifo_5 == 0x4)) {
        REG16(USB_BASE + REG_CSR0H) |= 0x1;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x26;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x3;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x41;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x0;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x69;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x0;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x70;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x0;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x33;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x0;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x32;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x0;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x52;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x0;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x56;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x0;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x31;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x0;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x35;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x0;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x58;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x0;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x0;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x0;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x0;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x0;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x0;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x0;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x0;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x0;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x0;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x0;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x0;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x0;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x0;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x0;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x0;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x0;

        REG8(USB_BASE + REG_CSR0L) |= 0x2;
        // REG8(USB_BASE + REG_CSR0L) |= 0x1 << 3;
    }

    if ((fifo_0 == 0x80) && (fifo_1 == 0x6) && (fifo_2 == 0x3) && (fifo_3 == 0x3) && (fifo_4 == 0x9) && (fifo_5 == 0x4)) {
        REG8(USB_BASE + REG_CSR0H) |= 0x1;

        REG8(USB_BASE + REG_EP0_FIFO) = 0x1A;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x3;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x41;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x0;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x69;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x0;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x50;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x0;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x33;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x0;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x32;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x0;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x52;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x0;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x56;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x0;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x31;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x0;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x35;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x0;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x58;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x0;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x0;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x0;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x0;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x0;

        REG8(USB_BASE + REG_CSR0L) |= 0x2;
        //REG8(USB_BASE + REG_CSR0L) |= 0x1 << 3;
    }

    /*configuration*/
    if ((fifo_3 == 0x2) && (fifo_6 == 0x9)) {
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
        //REG8(USB_BASE + REG_CSR0L) |= 0x1 << 3;
    }

    if ((fifo_0 == 0x21) && (fifo_1 == 0xA)) {
        //   REG8(USB_BASE + REG_CSR0H) |= 0x1;
        REG8(USB_BASE + REG_CSR0L) |= 0x1 << 2;
        REG8(USB_BASE + REG_CSR0L) |= 0x1 << 5;

        REG8(USB_BASE + REG_CSR0L) |= (0x1 << 6);
    }

    if ((fifo_0 == 0x80) && (fifo_1 == 0x6) && (fifo_3 == 0x2) && (fifo_6 == 0x22)) {
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
        //REG8(USB_BASE + REG_CSR0L) |= 0x1 << 3;
    }

    if (fifo_6 == 0xa) {

        REG8(USB_BASE + REG_CSR0L) |= (0x1 << 5);
        REG8(USB_BASE + REG_CSR0L) |= (0x1 << 6);
    }

    if ((fifo_0 == 0x81) && (fifo_1 == 0x6)) {
        // stop_flag = 0x1;
        // while (REG8(USB_BASE + REG_CSR0L) & 0x1 != 0)
        //     ;
        REG8(USB_BASE + REG_CSR0H) |= 0x1;
        debug("fifo_0 = %x\n", fifo_0);
        debug("fifo_1 = %x\n", fifo_1);
        debug("fifo_2 = %x\n", fifo_2);
        debug("fifo_3 = %x\n", fifo_3);
        debug("fifo_4 = %x\n", fifo_4);
        debug("fifo_5 = %x\n", fifo_5);
        debug("fifo_6 = %x\n", fifo_6);
        debug("fifo_7 = %x\n", fifo_7);
        REG8(USB_BASE + REG_EP0_FIFO) = 0x5;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x1;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x9;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x2;
        REG8(USB_BASE + REG_EP0_FIFO) = 0xA1;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x1;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x9;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x1;
        REG8(USB_BASE + REG_EP0_FIFO) = 0xA1;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x0;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x5;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x9;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x19;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x1;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x29;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x3;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x15;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x0;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x25;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x1;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x95;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x3;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x75;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x1;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x81;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x2;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x95;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x1;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x75;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x5;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x81;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x1;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x5;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x1;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x9;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x30;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x9;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x31;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x9;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x38;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x15;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x81;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x25;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x7f;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x75;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x8;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x95;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x3;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x81;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x6;
        REG8(USB_BASE + REG_EP0_FIFO) = 0xC0;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x9;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x3C;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x5;
        REG8(USB_BASE + REG_EP0_FIFO) = 0xFF;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x9;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x1;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x15;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x0;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x25;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x1;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x75;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x1;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x95;
        //REG8(USB_BASE + REG_CSR0L) |= 0x1 << 3;
        REG8(USB_BASE + REG_CSR0L) |= 0x2;

        while (REG8(USB_BASE + REG_CSR0L) & 0x2)
            ;

        {
            //REG8(USB_BASE + REG_EP_TXCSR) |= 0x1 << 3 | 0x1;
            REG8(USB_BASE + REG_CSR0H) |= 0x1;
            REG8(USB_BASE + REG_EP0_FIFO) = 0x2;
            REG8(USB_BASE + REG_EP0_FIFO) = 0xb1;
            REG8(USB_BASE + REG_EP0_FIFO) = 0x22;
            REG8(USB_BASE + REG_EP0_FIFO) = 0x75;
            REG8(USB_BASE + REG_EP0_FIFO) = 0x6;
            REG8(USB_BASE + REG_EP0_FIFO) = 0x95;
            REG8(USB_BASE + REG_EP0_FIFO) = 0x1;
            REG8(USB_BASE + REG_EP0_FIFO) = 0xb1;
            REG8(USB_BASE + REG_EP0_FIFO) = 0x1;
            REG8(USB_BASE + REG_EP0_FIFO) = 0xC0;
            REG8(USB_BASE + REG_CSR0L) |= 0x1 << 3;
            REG8(USB_BASE + REG_CSR0L) |= 0x2;
            //  REG8(USB_BASE + REG_CSR0L) |= 0x1 << 3;
            while (REG8(USB_BASE + REG_CSR0L) & 0x2)
                ;
            //   REG8(USB_BASE + REG_CSR0L) |= 0x1 << 3;
            // REG8(USB_BASE + REG_CSR0H) |= 0x1;
            // while (REG8(USB_BASE + REG_CSR0H) & 0x1 == 0)
            //     ;
            // REG8(USB_BASE + REG_CSR0L) |= 0x1 << 3;

            // while (REG8(USB_BASE + REG_CSR0L) & 0x2)
            //     ;

            // REG8(USB_BASE + REG_CSR0L) |= 0x1 << 4;
        }
    }
    if ((fifo_1 == 0x9) && (fifo_2 == 0x1)) {

        REG8(USB_BASE + REG_CSR0L) |= 0x1 << 6;
        REG8(USB_BASE + REG_CSR0L) |= 0x1 << 3;
    }
}
#endif