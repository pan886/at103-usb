/**
 * @file main.c
 * @brief 
 * 
 * @author pantianwen (pantianwen@163.com)
 * @version 1.0
 * @date 2022-11-06
 * @copyright Copyright (c) 2022 Icore, Inc
 */
#include "at103.h"
#include "usb_desc.h"
#define MUSB_TXCSR_H_WZC_BITS \
    (MUSB_TXCSR_H_NAKTIMEOUT | MUSB_TXCSR_H_RXSTALL | MUSB_TXCSR_H_ERROR | \
     MUSB_TXCSR_FIFONOTEMPTY)
uint8_t cnt = 0;
uint8_t flg = 0;
void    delay()
{
    volatile uint16_t i, j = 0;
    for (i = 0; i < 0xff; i++)
        for (j = 0; j < 0xff; j++)
            ;
}
void putc(void *p, char c)
{
    if (c == '\n')
        uart_putc(UART_DEBUG_PORT, '\r');
    uart_putc(UART_DEBUG_PORT, c);
}

NVIC_InitTypeDef NVIC_InitStructure;
NVIC_InitTypeDef NVIC_InitStructure1;
GPIO_InitTypeDef GPIOA_struct;

void Cfg_IO(void)
{
    GPIOA_struct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIOA_struct.GPIO_Pin  = GPIO_Pin_0;
    GPIO_Init(GPIOA, &GPIOA_struct);
    AFIO->GPIO[0].IC &= ~(0x3 << 27);
    GPIOA_struct.GPIO_Mode = GPIO_Mode_AIN;
    GPIOA_struct.GPIO_Pin  = GPIO_Pin_12 | GPIO_Pin_11;
    GPIO_Init(GPIOA, &GPIOA_struct);
}

void Set_System(void)
{
    RCC->USBPHY_CTRL.CLK_MODE_BIT   = 0x1;
    RCC->USBPHY_CTRL.PLL_EN_BIT     = 0x1;
    RCC->USBPHY_CTRL.IDDIG_BIT      = 0; /*host*/
    RCC->USBPHY_CTRL.REFCLK_SEL_BIT = 0x0;
    /*USB AHB clk enable*/
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_USB, ENABLE);
    RCC_AHBPeriphResetCmd(RCC_AHBPeriph_USB, ENABLE);
    RCC_AHBPeriphResetCmd(RCC_AHBPeriph_USB, DISABLE);

    RCC_OTGFSCLKConfig(0xB); //usb PHY ref clk(12M) div(TBD)

    RCC->USBPHY_CTRL.IDDIG_BIT     = 0;
    RCC->USBPHY_CTRL.CLK_MODE_BIT  = 0x1;
    RCC->USBPHY_CTRL.AVALID_BIT    = 0x1;
    RCC->USBPHY_CTRL.VBUSVAILD_BIT = 0x1;

    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
    NVIC_InitStructure.NVIC_IRQChannel                   = USB_MC_IRQn;
    NVIC_Init(&NVIC_InitStructure);
    __enable_irq();
    // NVIC_InitStructure1.NVIC_IRQChannelCmd = ENABLE;
    // NVIC_InitStructure1.NVIC_IRQChannel    = USB_DMA_IRQn;
    // NVIC_Init(&NVIC_InitStructure1);
    // __enable_irq();
}

uint8_t EPFIFO[100] = {0};
void    main(void)
{
    uint8_t val = 0;
    pll_init();
    sys_io_init();
    uart_init(UART_BOOT_PORT, UART_PARITY_NONE, UART_STOPBITS_1, UART_DATABITS_8, UART_BOOT_BD);
    init_printf(NULL, putc);
    Set_System();
    Cfg_IO();

    Usb_init();
    REG8(USB_BASE + REG_INTRUSBE) = 0xff;

    REG8(USB_BASE + REG_POWER) |= MUSB_POWER_HSMODE; //| MUSB_POWER_SUSPENDM | MUSB_POWER_ENSUSPEND;
    // REG8(USB_BASE + REG_POWER) |= MUSB_POWER_RESUME;
    //  delay();
    //  REG8(USB_BASE + REG_POWER) &= ~MUSB_POWER_RESUME; /*In Host mode, the CPU should clear this bit after 20 ms*/

    REG8(USB_BASE + REG_INTRUSBE) |= MUSB_INTR_SOF | MUSB_INTR_CONNECT | MUSB_INTR_SESSREQ | MUSB_INTR_DISCONNECT;
    REG8(USB_BASE + REG_DEVCTL) = MUSB_DEVCTL_SESSION | MUSB_DEVCTL_HM | MUSB_INTR_CONNECT;

    REG8(USB_BASE + 0x10 + MUSB_NAKLIMIT0) = 0x0;

    REG8(USB_BASE + 0x10 + MUSB_TXINTERVAL) = 0xf;
    REG16(USB_BASE + MUSB_INTRTXE)          = 0xffff;
    REG16(USB_BASE + MUSB_INTRRXE)          = 0x2;
    REG8(USB_BASE + REG_INDEX)              = EP0;

    REG8(USB_BASE + REG_PHYTMVBUS) = 0xffff;
    REG8(USB_BASE + TYPE0)         = 0x80;
    REG8(USB_BASE + REG_PHYTMVBUS) = 0xff;
    REG8(USB_BASE + NAKLimit0)     = 0xf;
    REG8(USB_BASE + REG_TMFSEOF1)  = 0xff;
    musb_set_host();

    while (1) {

        printf("REG_INTRUSBE = %x\n", REG8(USB_BASE + REG_INTRUSBE));
        printf("REG_POWER = %x\n", REG8(USB_BASE + REG_POWER));
        printf("REG_DEVCTL =%x\n", REG8(USB_BASE + REG_DEVCTL));
        printf("USBPHY_CTRL=%x\n", RCC->USBPHY_CTRL.value);
        printf("main progress!\n");
    }
}

void Usb_init()
{
    /*Detect VBUS_VALID*/
    //  while ((REG8(USB_BASE + REG_DEVCTL) & MUSB_DEVCTL_VBUS) != MUSB_DEVCTL_VBUS)
    //      ;
    REG16(USB_BASE + REG_INTRTXE) = 0x0;
    REG16(USB_BASE + REG_INTRRXE) = 0x0;
    REG8(USB_BASE + REG_INTRUSBE) = 0x0;
    REG8(USB_BASE + REG_TESTMODE) = 0; //0x20;
    // REG8(USB_BASE + 0x10 + MUSB_RXCSR) = MUSB_CSR0_H_REQPKT;
    //  REG8(USB_BASE + REG_POWER) &= ~MUSB_POWER_RESUME;
    musb_enable_interrupts();
    //  REG8(USB_BASE + REG_POWER) |= MUSB_POWER_SOFTCONN; /*soft connect*/
    //  REG8(USB_BASE + REG_DEVCTL) |= MUSB_DEVCTL_SESSION;
}

void USB_MC_IRQHandler(void)
{
    uint8_t  INTRUSB = 0;
    uint8_t  INTRTX  = 0;
    uint8_t  INTRRX  = 0;
    uint8_t  i       = 0;
    uint8_t  length  = 0;
    uint16_t CSR     = 0;
    int      retries = 5;

    printf("enter int!!!!!!!!!!!!!!!!!\n");
    INTRUSB = REG8(USB_BASE + REG_INTRUSB);

    // printf("REG_FRAME =%x\n", REG16(USB_BASE + REG_FRAME));
    //printf("USBPHY_CTRL=%x\n", RCC->USBPHY_CTRL.value);
    // REG8(USB_BASE + REG_POWER) |= MUSB_POWER_RESET;
    // REG8(USB_BASE + REG_INTRUSBE) = 0xff;
    // REG8(USB_BASE + REG_INDEX)    = EP0;
    // REG8(USB_BASE + REG_POWER) |= MUSB_POWER_RESUME;

    //REG8(USB_BASE + REG_POWER) |= MUSB_POWER_RESET;

    // REG8(USB_BASE + REG_POWER) &= ~MUSB_POWER_RESUME; /*In Host mode, the CPU should clear this bit after 20 ms*/
    //

    //REG8(USB_BASE + REG_DEVCTL) &= ~MUSB_DEVCTL_SESSION;
    if (INTRUSB & 0x40) {

        delay();
        REG8(USB_BASE + REG_DEVCTL) = MUSB_DEVCTL_SESSION | MUSB_DEVCTL_HM;
        printf("Session Request signaling has been detected.\n");
    }

    // printf("REG_DEVCTL =%x\n", REG8(USB_BASE + REG_DEVCTL));
    // if (REG8(USB_BASE + REG_DEVCTL) == 0x5d) {
    //     printf("enter full speed!\n");
    // }
    //printf("REG_INTRUSB= %d\n", REG8(USB_BASE + REG_INTRUSB));

    if (INTRUSB & 0x10) {
        REG8(USB_BASE + REG_POWER) |= MUSB_POWER_RESET;
        delay();
        REG8(USB_BASE + REG_POWER) &= ~MUSB_POWER_RESET;
        //  REG8(USB_BASE + REG_INTRUSBE) = 0xff;
        printf("enter connect interrupt!\n");

        REG8(USB_BASE + REG_DEVCTL) = MUSB_DEVCTL_SESSION | MUSB_DEVCTL_HM | MUSB_INTR_CONNECT;
    }

    if (INTRUSB & 0x8) {
        printf("enter sof interrupt!\n");

        //if (cnt < 13)
        {

            if (REG16(USB_BASE + REG_CSR0L) & MUSB_CSR0_H_ERROR) {
                REG16(USB_BASE + REG_CSR0L) &= ~MUSB_CSR0_H_ERROR;
            }
            if (REG16(USB_BASE + REG_CSR0L) & MUSB_CSR0_H_RXSTALL) {
                REG16(USB_BASE + REG_CSR0L) &= ~MUSB_CSR0_H_RXSTALL;
            }
            if (REG16(USB_BASE + REG_CSR0L) & MUSB_CSR0_H_NAKTIMEOUT) {
                REG16(USB_BASE + REG_CSR0L) &= ~MUSB_CSR0_H_NAKTIMEOUT;
            }

            //  REG8(USB_BASE + REG_DEVCTL) &= ~MUSB_DEVCTL_SESSION;

            // REG8(USB_BASE + REG_CSR0L) |= MUSB_CSR0_H_REQPKT;
            //REG16(USB_BASE + REG_CSR0L) |= MUSB_CSR0_H_WR_DATATOGGLE;
            // while (REG8(USB_BASE + REG_CSR0L) & MUSB_CSR0_TXPKTRDY)
            //     ;
            if (cnt < 21) {

                if ((REG16(USB_BASE + REG_CSR0L) & MUSB_CSR0_TXPKTRDY) == 0) {
                    do {
                        if (!(REG16(USB_BASE + REG_CSR0L) & (MUSB_CSR0_TXPKTRDY | MUSB_CSR0_RXPKTRDY)))
                            break;
                        REG16(USB_BASE + REG_CSR0L) |= MUSB_CSR0_FLUSHFIFO; /* code */
                    } while (--retries);
                    cnt++;

                    if ((cnt % 21 == 0x1)) {
                        // REG8(USB_BASE + REG_INDEX)    = 0x1;
                        REG8(USB_BASE + REG_EP0_FIFO) = 0x80;
                        REG8(USB_BASE + REG_EP0_FIFO) = 0x6;
                        REG8(USB_BASE + REG_EP0_FIFO) = 0x0;
                        REG8(USB_BASE + REG_EP0_FIFO) = 0x1;
                        REG8(USB_BASE + REG_EP0_FIFO) = 0x0;
                        REG8(USB_BASE + REG_EP0_FIFO) = 0x0;
                        REG8(USB_BASE + REG_EP0_FIFO) = 0x40;
                        REG8(USB_BASE + REG_EP0_FIFO) = 0x0;
                        //REG16(USB_BASE + REG_CSR0L) |= MUSB_CSR0_H_SETUPPKT | MUSB_CSR0_TXPKTRDY;
                    }

                    if ((cnt % 21 == 0x2)) {

                        REG8(USB_BASE + REG_EP0_FIFO) = 0x0;
                        REG8(USB_BASE + REG_EP0_FIFO) = 0x5;
                        REG8(USB_BASE + REG_EP0_FIFO) = 0x20;
                        REG8(USB_BASE + REG_EP0_FIFO) = 0x0;
                        REG8(USB_BASE + REG_EP0_FIFO) = 0x0;
                        REG8(USB_BASE + REG_EP0_FIFO) = 0x0;
                        REG8(USB_BASE + REG_EP0_FIFO) = 0x0;
                        REG8(USB_BASE + REG_EP0_FIFO) = 0x0;

                        //  REG16(USB_BASE + REG_CSR0L) |= MUSB_CSR0_H_SETUPPKT | MUSB_CSR0_TXPKTRDY;
                    }

                    if ((cnt % 21 == 0x3)) {
                        REG8(USB_BASE + REG_FADDR)    = 0x20;
                        REG8(USB_BASE + REG_EP0_FIFO) = 0x80;
                        REG8(USB_BASE + REG_EP0_FIFO) = 0x6;
                        REG8(USB_BASE + REG_EP0_FIFO) = 0x0;
                        REG8(USB_BASE + REG_EP0_FIFO) = 0x1;
                        REG8(USB_BASE + REG_EP0_FIFO) = 0x0;
                        REG8(USB_BASE + REG_EP0_FIFO) = 0x0;
                        REG8(USB_BASE + REG_EP0_FIFO) = 0x12;
                        REG8(USB_BASE + REG_EP0_FIFO) = 0x0;
                        //   REG16(USB_BASE + REG_CSR0L) |= MUSB_CSR0_H_SETUPPKT | MUSB_CSR0_TXPKTRDY;
                    }

                    if ((cnt % 21 == 0x5) || (cnt % 21 == 0xe)) {
                        REG8(USB_BASE + REG_EP0_FIFO) = 0x80;
                        REG8(USB_BASE + REG_EP0_FIFO) = 0x6;
                        REG8(USB_BASE + REG_EP0_FIFO) = 0x3;
                        REG8(USB_BASE + REG_EP0_FIFO) = 0x3;
                        REG8(USB_BASE + REG_EP0_FIFO) = 0x9;
                        REG8(USB_BASE + REG_EP0_FIFO) = 0x4;
                        REG8(USB_BASE + REG_EP0_FIFO) = 0xff;
                        REG8(USB_BASE + REG_EP0_FIFO) = 0x0;
                        //  REG16(USB_BASE + REG_CSR0L) |= MUSB_CSR0_H_SETUPPKT | MUSB_CSR0_TXPKTRDY;
                    }

                    if ((cnt % 21 == 0x6) || (cnt % 21 == 0xf)) {
                        REG8(USB_BASE + REG_EP0_FIFO) = 0x80;
                        REG8(USB_BASE + REG_EP0_FIFO) = 0x6;
                        REG8(USB_BASE + REG_EP0_FIFO) = 0x0;
                        REG8(USB_BASE + REG_EP0_FIFO) = 0x3;
                        REG8(USB_BASE + REG_EP0_FIFO) = 0x0;
                        REG8(USB_BASE + REG_EP0_FIFO) = 0x0;
                        REG8(USB_BASE + REG_EP0_FIFO) = 0xff;
                        REG8(USB_BASE + REG_EP0_FIFO) = 0x0;
                        //  REG16(USB_BASE + REG_CSR0L) |= MUSB_CSR0_H_SETUPPKT | MUSB_CSR0_TXPKTRDY;
                    }

                    if ((cnt % 21 == 0x7) || (cnt % 21 == 0x10)) {
                        REG8(USB_BASE + REG_EP0_FIFO) = 0x80;
                        REG8(USB_BASE + REG_EP0_FIFO) = 0x6;
                        REG8(USB_BASE + REG_EP0_FIFO) = 0x2;
                        REG8(USB_BASE + REG_EP0_FIFO) = 0x3;
                        REG8(USB_BASE + REG_EP0_FIFO) = 0x9;
                        REG8(USB_BASE + REG_EP0_FIFO) = 0x4;
                        REG8(USB_BASE + REG_EP0_FIFO) = 0xff;
                        REG8(USB_BASE + REG_EP0_FIFO) = 0x0;
                        //  REG16(USB_BASE + REG_CSR0L) |= MUSB_CSR0_H_SETUPPKT | MUSB_CSR0_TXPKTRDY;
                    }
                    if ((cnt % 21 == 0x8) || (cnt % 21 == 0x11)) {

                        REG8(USB_BASE + REG_EP0_FIFO) = 0x80;
                        REG8(USB_BASE + REG_EP0_FIFO) = 0x6;
                        REG8(USB_BASE + REG_EP0_FIFO) = 0x0;
                        REG8(USB_BASE + REG_EP0_FIFO) = 0x2;
                        REG8(USB_BASE + REG_EP0_FIFO) = 0x0;
                        REG8(USB_BASE + REG_EP0_FIFO) = 0x0;
                        REG8(USB_BASE + REG_EP0_FIFO) = 0x9;
                        REG8(USB_BASE + REG_EP0_FIFO) = 0x0;
                        //  REG16(USB_BASE + REG_CSR0L) |= MUSB_CSR0_H_SETUPPKT | MUSB_CSR0_TXPKTRDY;
                    }
                    if ((cnt % 21 == 0x4) || (cnt % 21 == 0x9) || (cnt % 21 == 0x12)) {

                        REG8(USB_BASE + REG_EP0_FIFO) = 0x80;
                        REG8(USB_BASE + REG_EP0_FIFO) = 0x6;
                        REG8(USB_BASE + REG_EP0_FIFO) = 0x0;
                        REG8(USB_BASE + REG_EP0_FIFO) = 0x2;
                        REG8(USB_BASE + REG_EP0_FIFO) = 0x0;
                        REG8(USB_BASE + REG_EP0_FIFO) = 0x0;
                        REG8(USB_BASE + REG_EP0_FIFO) = 0x22;
                        REG8(USB_BASE + REG_EP0_FIFO) = 0x0;
                        //  REG16(USB_BASE + REG_CSR0L) |= MUSB_CSR0_H_SETUPPKT | MUSB_CSR0_TXPKTRDY;
                    }

                    if ((cnt % 21 == 0xA)) {
                        REG8(USB_BASE + REG_FADDR)    = 0x0;
                        REG8(USB_BASE + REG_EP0_FIFO) = 0x80;
                        REG8(USB_BASE + REG_EP0_FIFO) = 0x6;
                        REG8(USB_BASE + REG_EP0_FIFO) = 0x0;
                        REG8(USB_BASE + REG_EP0_FIFO) = 0x1;
                        REG8(USB_BASE + REG_EP0_FIFO) = 0x0;
                        REG8(USB_BASE + REG_EP0_FIFO) = 0x0;
                        REG8(USB_BASE + REG_EP0_FIFO) = 0x40;
                        REG8(USB_BASE + REG_EP0_FIFO) = 0x0;
                        //REG16(USB_BASE + REG_CSR0L) |= MUSB_CSR0_H_SETUPPKT | MUSB_CSR0_TXPKTRDY;
                    }

                    if (cnt % 21 == 0xB) {

                        REG8(USB_BASE + REG_EP0_FIFO) = 0x0;
                        REG8(USB_BASE + REG_EP0_FIFO) = 0x5;
                        REG8(USB_BASE + REG_EP0_FIFO) = 0x21;
                        REG8(USB_BASE + REG_EP0_FIFO) = 0x0;
                        REG8(USB_BASE + REG_EP0_FIFO) = 0x0;
                        REG8(USB_BASE + REG_EP0_FIFO) = 0x0;
                        REG8(USB_BASE + REG_EP0_FIFO) = 0x0;
                        REG8(USB_BASE + REG_EP0_FIFO) = 0x0;
                    }

                    if (cnt % 21 == 0xC) {

                        REG8(USB_BASE + REG_FADDR)    = 0x21;
                        REG8(USB_BASE + REG_EP0_FIFO) = 0x80;
                        REG8(USB_BASE + REG_EP0_FIFO) = 0x6;
                        REG8(USB_BASE + REG_EP0_FIFO) = 0x0;
                        REG8(USB_BASE + REG_EP0_FIFO) = 0x1;
                        REG8(USB_BASE + REG_EP0_FIFO) = 0x0;
                        REG8(USB_BASE + REG_EP0_FIFO) = 0x0;
                        REG8(USB_BASE + REG_EP0_FIFO) = 0x12;
                        REG8(USB_BASE + REG_EP0_FIFO) = 0x0;
                    }

                    if (cnt % 21 == 0xd) {

                        REG8(USB_BASE + REG_FADDR)    = 0x21;
                        REG8(USB_BASE + REG_EP0_FIFO) = 0x80;
                        REG8(USB_BASE + REG_EP0_FIFO) = 0x6;
                        REG8(USB_BASE + REG_EP0_FIFO) = 0x0;
                        REG8(USB_BASE + REG_EP0_FIFO) = 0x2;
                        REG8(USB_BASE + REG_EP0_FIFO) = 0x0;
                        REG8(USB_BASE + REG_EP0_FIFO) = 0x0;
                        REG8(USB_BASE + REG_EP0_FIFO) = 0x22;
                        REG8(USB_BASE + REG_EP0_FIFO) = 0x0;
                    }
                    if (cnt % 21 == 0x13) {
                        REG8(USB_BASE + REG_EP0_FIFO) = 0x0;
                        REG8(USB_BASE + REG_EP0_FIFO) = 0x9;
                        REG8(USB_BASE + REG_EP0_FIFO) = 0x1;
                        REG8(USB_BASE + REG_EP0_FIFO) = 0x0;
                        REG8(USB_BASE + REG_EP0_FIFO) = 0x0;
                        REG8(USB_BASE + REG_EP0_FIFO) = 0x0;
                        REG8(USB_BASE + REG_EP0_FIFO) = 0x0;
                        REG8(USB_BASE + REG_EP0_FIFO) = 0x0;
                    }
                    if (cnt % 21 == 0x14) {
                        REG8(USB_BASE + REG_EP0_FIFO) = 0x21;
                        REG8(USB_BASE + REG_EP0_FIFO) = 0xA;
                        REG8(USB_BASE + REG_EP0_FIFO) = 0x0;
                        REG8(USB_BASE + REG_EP0_FIFO) = 0x0;
                        REG8(USB_BASE + REG_EP0_FIFO) = 0x0;
                        REG8(USB_BASE + REG_EP0_FIFO) = 0x0;
                        REG8(USB_BASE + REG_EP0_FIFO) = 0x0;
                        REG8(USB_BASE + REG_EP0_FIFO) = 0x0;
                    }
                    if (cnt % 21 == 0) {
                        REG8(USB_BASE + REG_EP0_FIFO) = 0x81;
                        REG8(USB_BASE + REG_EP0_FIFO) = 0x6;
                        REG8(USB_BASE + REG_EP0_FIFO) = 0x0;
                        REG8(USB_BASE + REG_EP0_FIFO) = 0x22;
                        REG8(USB_BASE + REG_EP0_FIFO) = 0x0;
                        REG8(USB_BASE + REG_EP0_FIFO) = 0x0;
                        REG8(USB_BASE + REG_EP0_FIFO) = 0x8A;
                        REG8(USB_BASE + REG_EP0_FIFO) = 0x0;
                    }
                    REG16(USB_BASE + REG_CSR0L) = MUSB_CSR0_H_SETUPPKT | MUSB_CSR0_TXPKTRDY;
                    //REG16(USB_BASE + REG_CSR0L) |= 0x1 | 0x2000;
                }

                while ((REG16(USB_BASE + REG_CSR0L) & MUSB_CSR0_TXPKTRDY))
                    ;

                // if (cnt % 13 == 0xA) {
                //     REG8(USB_BASE + REG_INDEX)    = 0x1;
                //     REG8(USB_BASE + REG_EP1_FIFO) = 0x0;
                //     REG8(USB_BASE + REG_EP1_FIFO) = 0x5;
                //     REG8(USB_BASE + REG_EP1_FIFO) = 0xB;
                //     REG8(USB_BASE + REG_EP1_FIFO) = 0x0;
                //     REG8(USB_BASE + REG_EP1_FIFO) = 0x0;
                //     REG8(USB_BASE + REG_EP1_FIFO) = 0x0;
                //     REG8(USB_BASE + REG_EP1_FIFO) = 0x0;
                //     REG8(USB_BASE + REG_EP1_FIFO) = 0x0;
                //     REG16(USB_BASE + REG_CSR0L) |= 0x1 | 0x2000;
                // }

                {
                    REG16(USB_BASE + REG_CSR0L) |= MUSB_CSR0_H_REQPKT;
                    delay();
                }

                // printf("REG_CSR0L =%x\n", REG16(USB_BASE + REG_CSR0L));
                // if ((REG8(USB_BASE + REG_CSR0L) & 0x1) == 0x1) {
                //     for (i = 0; i < 18; i++)
                //         printf("REG_EP0_FIFO =%x\n", REG8(USB_BASE + REG_EP0_FIFO));
                // }
                // else {

                //     REG16(USB_BASE + REG_CSR0L) |= MUSB_CSR0_H_REQPKT;
                // }
                // REG16(USB_BASE + REG_CSR0L) |= MUSB_CSR0_H_REQPKT;
                // {
                if (REG16(USB_BASE + REG_CSR0L) & MUSB_CSR0_RXPKTRDY)

                {

                    length = REG8(USB_BASE + REG_EP0_COUNT);
                    printf("length = %x\n", length);
                    for (i = 0; i < length; i++) {
                        EPFIFO[i] = REG8(USB_BASE + REG_EP0_FIFO);
                        printf("EPFIFO = %x\n", EPFIFO[i]);
                    }

                    REG16(USB_BASE + REG_CSR0L) &= ~MUSB_CSR0_RXPKTRDY;
                    // REG16(USB_BASE + REG_CSR0L) &= ~MUSB_CSR0_H_REQPKT;
                }

                if (cnt % 21 == 0) {
                    REG16(USB_BASE + REG_CSR0L) |= MUSB_CSR0_H_REQPKT;

                    delay();
                    if (REG16(USB_BASE + REG_CSR0L) & MUSB_CSR0_RXPKTRDY)

                    {

                        length = REG8(USB_BASE + REG_EP0_COUNT);
                        printf("length = %x\n", length);
                        for (i = 0; i < length; i++) {
                            EPFIFO[i] = REG8(USB_BASE + REG_EP0_FIFO);
                            printf("EPFIFO = %x\n", EPFIFO[i]);
                        }

                        REG16(USB_BASE + REG_CSR0L) &= ~MUSB_CSR0_RXPKTRDY;
                        // REG16(USB_BASE + REG_CSR0L) &= ~MUSB_CSR0_H_REQPKT;
                    }
                    flg = 1;
                }
                //  }

                // if (REG8(USB_BASE + REG_INTRTX) & 0x1) {
                //     if (REG16(USB_BASE + REG_CSR0L) & MUSB_CSR0_H_ERROR) {
                //         REG16(USB_BASE + REG_CSR0L) &= ~MUSB_CSR0_H_ERROR;
                //     }
                //     if (REG16(USB_BASE + REG_CSR0L) & MUSB_CSR0_H_RXSTALL) {
                //         REG16(USB_BASE + REG_CSR0L) &= ~MUSB_CSR0_H_RXSTALL;
                //     }
                //     if (REG16(USB_BASE + REG_CSR0L) & MUSB_CSR0_H_NAKTIMEOUT) {
                //         REG16(USB_BASE + REG_CSR0L) &= ~MUSB_CSR0_H_NAKTIMEOUT;
                //     }
                // }

                // }

                // REG8(USB_BASE + REG_EP0_FIFO) = 0x80;
                // REG8(USB_BASE + REG_EP0_FIFO) = 0x6;
                // REG8(USB_BASE + REG_EP0_FIFO) = 0x0;
                // REG8(USB_BASE + REG_EP0_FIFO) = 0x2;
                // REG8(USB_BASE + REG_EP0_FIFO) = 0x0;
                // REG8(USB_BASE + REG_EP0_FIFO) = 0x0;
                // REG8(USB_BASE + REG_EP0_FIFO) = 0x22;
                // REG8(USB_BASE + REG_EP0_FIFO) = 0x0;

                //REG8(USB_BASE + REG_CSR0L) &= ~MUSB_CSR0_H_SETUPPKT;

                // if (REG8(USB_BASE + REG_INTRTX) & 0x1)
                // {

                // if (REG16(USB_BASE + REG_CSR0L) & MUSB_CSR0_H_ERROR) {
                //     REG16(USB_BASE + REG_CSR0L) &= ~MUSB_CSR0_H_ERROR;
                // }
                // if (REG16(USB_BASE + REG_CSR0L) & MUSB_CSR0_H_RXSTALL) {
                //     REG16(USB_BASE + REG_CSR0L) &= ~MUSB_CSR0_H_RXSTALL;
                // }
                // if (REG16(USB_BASE + REG_CSR0L) & MUSB_CSR0_H_NAKTIMEOUT) {
                //     REG16(USB_BASE + REG_CSR0L) &= ~MUSB_CSR0_H_NAKTIMEOUT;
                // }

                /*These bits need to be set together i.e. in the same write operation*/
                // REG8(USB_BASE + REG_CSR0L) |= MUSB_CSR0_H_REQPKT;

                // printf("sbsbsb\n");
                // printf("REG_CSR0L = %x\n", REG16(USB_BASE + REG_CSR0L));
                // if (REG16(USB_BASE + REG_CSR0L) & MUSB_CSR0_H_ERROR) {
                //     REG16(USB_BASE + REG_CSR0L) &= ~MUSB_CSR0_H_ERROR;
                // }

                // {
                //     do {
                //         if (!(REG16(USB_BASE + REG_CSR0L) & (MUSB_CSR0_TXPKTRDY | MUSB_CSR0_RXPKTRDY)))
                //             break;
                //         REG16(USB_BASE + REG_CSR0L) |= MUSB_CSR0_FLUSHFIFO; /* code */
                //     } while (--retries);

                {
                    if ((cnt % 21 != 0x2) && (cnt % 21 != 0xb) && (cnt % 21 != 0x13) && (cnt % 21 != 0x14)) {

                        do {
                            // printf("do csr= %x\n ", REG16(USB_BASE + REG_CSR0L));
                            if (!(REG16(USB_BASE + REG_CSR0L) & (MUSB_CSR0_TXPKTRDY | MUSB_CSR0_RXPKTRDY)))
                                break;
                            REG16(USB_BASE + REG_CSR0L) |= MUSB_CSR0_FLUSHFIFO; /* code */
                        } while (--retries);
                        REG16(USB_BASE + REG_CSR0L) |= MUSB_CSR0_FLUSHFIFO; /* code */
                        REG16(USB_BASE + REG_CSR0L) &= ~MUSB_CSR0_H_SETUPPKT;
                        REG16(USB_BASE + REG_CSR0L) |= MUSB_CSR0_TXPKTRDY; /* code */
                                                                           // REG16(USB_BASE + REG_CSR0L) &= ~MUSB_CSR0_H_REQPKT;
                    }
                }

                // }

                // while ((REG16(USB_BASE + REG_CSR0L) & MUSB_CSR0_TXPKTRDY))
                //     ;

                // if (cnt % 20 == 0x9) {
                //     // musb_host_bus_suspend();
                //     // musb_bus_resume();
                //     // // delay();
                //     // REG8(USB_BASE + REG_POWER) &= ~MUSB_POWER_RESUME;
                //     REG8(USB_BASE + REG_POWER) |= MUSB_POWER_RESET;
                //     delay();
                //     REG8(USB_BASE + REG_POWER) &= ~MUSB_POWER_RESET;
                // }

                {
                    if ((cnt % 21 == 0x1) || (cnt % 21 == 0x9) || (cnt % 21 == 0xA)) {
                        REG8(USB_BASE + REG_POWER) |= MUSB_POWER_RESET;
                        delay();
                        REG8(USB_BASE + REG_POWER) &= ~MUSB_POWER_RESET;
                    }
                }
            }
            if (flg == 0x1) {
                REG16(USB_BASE + MUSB_INTRRXE) = 0x2;
                REG8(USB_BASE + REG_INDEX)     = 0x1;

                // REG8(USB_BASE + REG_EP1_FIFO) = 0x80;
                // REG8(USB_BASE + REG_EP1_FIFO) = 0x6;
                // REG8(USB_BASE + REG_EP1_FIFO) = 0x0;
                // REG8(USB_BASE + REG_EP1_FIFO) = 0x2;
                // REG8(USB_BASE + REG_EP1_FIFO) = 0x0;
                // REG8(USB_BASE + REG_EP1_FIFO) = 0x0;
                // REG8(USB_BASE + REG_EP1_FIFO) = 0x22;
                // REG8(USB_BASE + REG_EP1_FIFO) = 0x0;
                //REG16(USB_BASE + REG_EP_TXCSR) =
                // REG8(USB_BASE + 0x10 + MUSB_TXMAXP) = 0x1ff;
                REG16(USB_BASE + 0x100 + 0x1C) = 0xb1;

                REG16(USB_BASE + 0x100 + 0x10 + MUSB_RXMAXP) = 0x1ff;
                // REG16(USB_BASE + 0x100 + REG_EP_TXCSR) |= MUSB_TXCSR_TXPKTRDY | 0x0010;
                // REG16(USB_BASE + 0x100 + REG_EP_TXCSR) &= ~0x20;
                // REG16(USB_BASE + 0x100 + REG_EP_TXCSR) |= 0x1;
                REG16(USB_BASE + 0x100 + REG_EP_RXCSRL) |= MUSB_RXCSR_H_REQPKT;
                //REG8(USB_BASE + 0x100 + REG_EP_RXCSRH) &= ~0x40;

                printf("REG_EP_TXCSR=%x\n", REG16(CONFIG_EP(EP1) + REG_EP_TXCSR));
            }
#if 0
                //
                //    REG8(USB_BASE + REG_EP0_FIFO) = 0x80;
                //REG8(USB_BASE + REG_CSR0L) |= MUSB_CSR0_TXPKTRDY;
                //  delay();
                // do {
                //     if (!(REG8(USB_BASE + REG_CSR0L) & (MUSB_CSR0_TXPKTRDY | MUSB_CSR0_RXPKTRDY)))
                //         break;
                //     REG16(USB_BASE + REG_CSR0L) = MUSB_CSR0_FLUSHFIFO; /* code */
                // } while (0);
                //  REG16(USB_BASE + REG_CSR0L) = MUSB_CSR0_FLUSHFIFO; /* code */
                //   REG8(USB_BASE + REG_CSR0L) &= ~MUSB_CSR0_H_SETUPPKT;

                // if ((CSR & MUSB_CSR0_H_RXSTALL)) {
                //     printf("STALLING ENDPOINT!\n");
                //     REG16(USB_BASE + REG_CSR0L) &= ~(MUSB_CSR0_H_RXSTALL);

                // } else if (CSR & MUSB_CSR0_H_ERROR) {

                //     printf("no response\n");
                // } else if (CSR & MUSB_CSR0_H_NAKTIMEOUT) {
                //     printf("control NAK timeout\n");
                // }

                // REG8(USB_BASE + REG_CSR0L) |= MUSB_CSR0_TXPKTRDY;
                // while ((REG8(USB_BASE + REG_CSR0L) & MUSB_CSR0_RXPKTRDY))
                //     ;

                //
                // REG8(USB_BASE + REG_CSR0L) |= MUSB_CSR0_FLUSHFIFO;

                //

                //printf("EPFIFO[0] = %x\n", EPFIFO[0]);
                //  delay();
                // REG8(USB_BASE + REG_CSR0L) &= ~MUSB_CSR0_H_SETUPPKT;
            }
        }

        // if (cnt % 13 == 0xc) {
        //     REG16(USB_BASE + REG_INDEX)         = EP1;
        //     REG16(CONFIG_EP(EP1) + MUSB_TXMAXP) = 0x100f;
        //     REG16(CONFIG_EP(EP1) + MUSB_RXMAXP) = 0x100f;
        //     REG16(USB_BASE + 0x10 + MUSB_RXCSR) |= MUSB_RXCSR_H_REQPKT;
        // }


        // {
        //  REG8(USB_BASE + REG_POWER) &= ~MUSB_POWER_RESET;
        do {
            if (!(REG8(USB_BASE + REG_CSR0L) & (MUSB_CSR0_TXPKTRDY | MUSB_CSR0_RXPKTRDY)))
                break;
            REG16(USB_BASE + REG_CSR0L) = MUSB_CSR0_FLUSHFIFO; /* code */
        } while (0);
        if ((REG16(USB_BASE + REG_CSR0L) & MUSB_CSR0_TXPKTRDY) == 0) {
            REG8(USB_BASE + REG_EP0_FIFO) = 0x0;
            REG8(USB_BASE + REG_EP0_FIFO) = 0x5;
            REG8(USB_BASE + REG_EP0_FIFO) = 0x17;
            REG8(USB_BASE + REG_EP0_FIFO) = 0x0;
            REG8(USB_BASE + REG_EP0_FIFO) = 0x0;
            REG8(USB_BASE + REG_EP0_FIFO) = 0x0;
            REG8(USB_BASE + REG_EP0_FIFO) = 0x0;
            REG8(USB_BASE + REG_EP0_FIFO) = 0x0;
            REG8(USB_BASE + REG_CSR0L) |= MUSB_CSR0_H_SETUPPKT | MUSB_CSR0_TXPKTRDY;
        }

        // // delay();
        // REG8(USB_BASE + REG_CSR0L) &= ~MUSB_CSR0_H_SETUPPKT;
        // while ((REG16(USB_BASE + REG_CSR0L) & MUSB_CSR0_TXPKTRDY))
        //     ;
        REG16(USB_BASE + REG_CSR0L) |= MUSB_CSR0_H_REQPKT;

        if (REG16(USB_BASE + REG_CSR0L) & MUSB_CSR0_RXPKTRDY) {
            length = REG8(USB_BASE + REG_EP0_COUNT);
            CSR    = REG16(USB_BASE + REG_CSR0L);
            for (i = 0; i < length; i++) {
                EPFIFO[i] = REG8(USB_BASE + REG_EP0_FIFO);
                printf("EPFIFO = %x\n", EPFIFO[i]);
            }

            REG16(USB_BASE + REG_CSR0L) &= ~MUSB_CSR0_RXPKTRDY;
        }
        // REG16(USB_BASE + REG_CSR0L) |= MUSB_CSR0_FLUSHFIFO;
        // REG8(USB_BASE + REG_CSR0L) &= ~MUSB_CSR0_RXPKTRDY;
        // delay();
        // }
        // {

        {

            if ((REG16(USB_BASE + REG_CSR0L) & (MUSB_CSR0_TXPKTRDY)) == 0) {
                REG8(USB_BASE + REG_EP0_FIFO) = 0x80;
                REG8(USB_BASE + REG_EP0_FIFO) = 0x6;
                REG8(USB_BASE + REG_EP0_FIFO) = 0x0;
                REG8(USB_BASE + REG_EP0_FIFO) = 0x2;
                REG8(USB_BASE + REG_EP0_FIFO) = 0x0;
                REG8(USB_BASE + REG_EP0_FIFO) = 0x0;
                REG8(USB_BASE + REG_EP0_FIFO) = 0x22;
                REG8(USB_BASE + REG_EP0_FIFO) = 0x0;
                REG8(USB_BASE + REG_CSR0L) |= MUSB_CSR0_H_SETUPPKT | MUSB_CSR0_TXPKTRDY;
            }
            // }

            while ((REG16(USB_BASE + REG_CSR0L) & MUSB_CSR0_TXPKTRDY))
                ;
            REG16(USB_BASE + REG_CSR0L) |= MUSB_CSR0_H_REQPKT;

            if (REG16(USB_BASE + REG_CSR0L) & MUSB_CSR0_RXPKTRDY) {
                length = REG8(USB_BASE + REG_EP0_COUNT);
                CSR    = REG16(USB_BASE + REG_CSR0L);
                for (i = 0; i < length; i++) {
                    EPFIFO[i] = REG8(USB_BASE + REG_EP0_FIFO);
                    printf("EPFIFO = %x\n", EPFIFO[i]);
                }

                REG16(USB_BASE + REG_CSR0L) &= ~MUSB_CSR0_RXPKTRDY;
            }

            // do {
            //     if (!(REG8(USB_BASE + REG_CSR0L) & (MUSB_CSR0_TXPKTRDY | MUSB_CSR0_RXPKTRDY)))
            //         break;
            //     REG16(USB_BASE + REG_CSR0L) = MUSB_CSR0_FLUSHFIFO; /* code */
            // } while (0 /* condition */);
            // REG8(USB_BASE + REG_CSR0L) &= ~MUSB_CSR0_H_SETUPPKT;
            // REG8(USB_BASE + REG_CSR0L) |= MUSB_CSR0_H_STATUSPKT;
            // if (REG8(USB_BASE + REG_CSR0L) & MUSB_CSR0_RXPKTRDY) {
            //     REG8(USB_BASE + REG_CSR0L) &= ~MUSB_CSR0_RXPKTRDY;
            // }

            // }
            // {
            //     REG16(USB_BASE + REG_CSR0L) |= MUSB_CSR0_FLUSHFIFO;
            //     REG8(USB_BASE + REG_EP0_FIFO) = 0x0;
            //     REG8(USB_BASE + REG_EP0_FIFO) = 0x5;
            //     REG8(USB_BASE + REG_EP0_FIFO) = 0x17;
            //     REG8(USB_BASE + REG_EP0_FIFO) = 0x0;
            //     REG8(USB_BASE + REG_EP0_FIFO) = 0x0;
            //     REG8(USB_BASE + REG_EP0_FIFO) = 0x0;
            //     REG8(USB_BASE + REG_EP0_FIFO) = 0x0;
            //     REG8(USB_BASE + REG_EP0_FIFO) = 0x0;
            //     REG8(USB_BASE + REG_CSR0L) |= MUSB_CSR0_H_SETUPPKT | MUSB_CSR0_TXPKTRDY;

            //     // REG8(USB_BASE + REG_CSR0L) &= ~MUSB_CSR0_H_SETUPPKT;
            // }
        }

        //REG16(USB_BASE + REG_CSR0L) = MUSB_CSR0_FLUSHFIFO;

        // REG8(USB_BASE + REG_CSR0L) |= MUSB_CSR0_TXPKTRDY;

        //
        // if (INTRUSB & 0x4) {

        //     printf("enter reset interrupt!\n");
        //     //   REG8(USB_BASE + REG_DEVCTL) &= ~MUSB_DEVCTL_SESSION;
        //     printf("REG_DEVCTL = %x\n", REG8(USB_BASE + REG_DEVCTL));
        //     delay();

        //     //  REG8(USB_BASE + REG_POWER) &= ~MUSB_POWER_RESET;
        //     printf("REG_DEVCTL = %x\n", REG8(USB_BASE + REG_DEVCTL));
        // }

        //REG8(USB_BASE + REG_CSR0L) = MUSB_CSR0_TXPKTRDY | MUSB_CSR0_H_REQPKT | MUSB_CSR0_H_SETUPPKT | MUSB_CSR0_RXPKTRDY;
        {
            // REG16(USB_BASE + REG_CSR0L)   = MUSB_CSR0_FLUSHFIFO;
            // REG8(USB_BASE + REG_EP0_FIFO) = 0x80;
            // REG8(USB_BASE + REG_EP0_FIFO) = 0x6;
            // REG8(USB_BASE + REG_EP0_FIFO) = 0x0;
            // REG8(USB_BASE + REG_EP0_FIFO) = 0x1;
            // REG8(USB_BASE + REG_EP0_FIFO) = 0x0;
            // REG8(USB_BASE + REG_EP0_FIFO) = 0x0;
            // REG8(USB_BASE + REG_EP0_FIFO) = 0x40;
            // REG8(USB_BASE + REG_EP0_FIFO) = 0x0;
            // REG8(USB_BASE + REG_CSR0L) |= MUSB_CSR0_H_SETUPPKT | MUSB_CSR0_TXPKTRDY | MUSB_CSR0_H_STATUSPKT;
        }

    do {
        if (!(REG8(USB_BASE + REG_CSR0L) & (MUSB_CSR0_TXPKTRDY | MUSB_CSR0_RXPKTRDY)))
            break;
        REG16(USB_BASE + REG_CSR0L) = MUSB_CSR0_FLUSHFIFO; /* code */
    } while (0 /* condition */);
    if ((REG16(USB_BASE + REG_CSR0L) & (MUSB_CSR0_TXPKTRDY | MUSB_CSR0_RXPKTRDY)) == 0) {
        REG8(USB_BASE + REG_EP0_FIFO) = 0x0;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x5;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x17;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x0;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x0;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x0;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x0;
        REG8(USB_BASE + REG_EP0_FIFO) = 0x0;
        REG8(USB_BASE + REG_CSR0L) |= MUSB_CSR0_H_SETUPPKT | MUSB_CSR0_TXPKTRDY | MUSB_CSR0_H_STATUSPKT;
    }

    if (INTRTX == 0x1) {
        /*These bits need to be set together i.e. in the same write operation*/
        REG8(USB_BASE + REG_CSR0L) |= MUSB_CSR0_H_REQPKT;
        CSR = REG8(USB_BASE + REG_CSR0L);
        printf("REG_CSR0L = %x\n", CSR);
        if ((CSR & MUSB_CSR0_H_RXSTALL)) {
            printf("STALLING ENDPOINT!\n");
            REG16(USB_BASE + REG_CSR0L) &= ~(MUSB_CSR0_H_RXSTALL);

        } else if (CSR & MUSB_CSR0_H_ERROR) {

            printf("no response\n");
        } else if (CSR & MUSB_CSR0_H_NAKTIMEOUT) {
            printf("control NAK timeout\n");
        }

        {

            length = REG8(USB_BASE + REG_EP0_COUNT);
            CSR    = REG16(USB_BASE + REG_CSR0L);
            if (CSR & MUSB_CSR0_RXPKTRDY) {

                for (i = 0; i < length; i++) {
                    EPFIFO[i] = REG8(USB_BASE + REG_EP0_FIFO);
                    printf("EPFIFO = %x\n", EPFIFO[i]);
                }
            }
        }
        // REG8(USB_BASE + REG_CSR0L) &= ~MUSB_CSR0_H_SETUPPKT;
        // REG8(USB_BASE + REG_CSR0L) |= MUSB_CSR0_TXPKTRDY;

        
    }
#endif

            printf("REG_INTRUSB = %x\n", REG8(USB_BASE + REG_INTRUSB));
        }
    }
}
