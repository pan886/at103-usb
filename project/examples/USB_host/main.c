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
uint8_t cnt     = 0;
uint8_t flg     = 0;
uint8_t sb[400] = {1};
void    delay()
{
    volatile uint16_t i, j = 0;
    for (i = 0; i < 0x1ff; i++)
        for (j = 0; j < 0xff; j++)
            ;
}
void putc(void *p, char c)
{
    if (c == '\n')
        uart_putc(UART_DEBUG_PORT, '\r');
    uart_putc(UART_DEBUG_PORT, c);
}

NVIC_InitTypeDef        NVIC_InitStructure;
NVIC_InitTypeDef        NVIC_InitStructure1;
GPIO_InitTypeDef        GPIOA_struct;
USB_HOST_RXTYPE_TypeDef Usb_Host_Rxtype;
USB_HOST_RXTYPE_TypeDef Usb_Host_Txtype;

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
    Usb_Set_Host_Mode();

    Usb_Host_Rxtype.SPEED    = USB_SPEED_FULL;
    Usb_Host_Rxtype.PROTOCOL = INTERRUPT;
    Usb_Host_Rxtype.EP_NUM   = EP1;

    Usb_Host_Txtype.SPEED    = USB_SPEED_FULL;
    Usb_Host_Txtype.PROTOCOL = INTERRUPT;
    Usb_Host_Txtype.EP_NUM   = EP7;
    Usb_Host_Wait();

    while (1) {

        printf("main progress!\n");
    }
}

void Usb_init()
{
    Usb_All_Interrupts(ENABLE);
}

void USB_MC_IRQHandler(void)
{
    uint8_t INTRUSB = 0;

    printf("enter int!!!!!!!!!!!!!!!\n");
    INTRUSB = REG8(USB_BASE + MUSB_INTRUSB);

    if (INTRUSB & MUSB_INTR_SESSREQ) {

        REG8(USB_BASE + MUSB_DEVCTL) = MUSB_DEVCTL_SESSION | MUSB_DEVCTL_HM;
        printf("Session Request signaling has been detected.\n");
    }

    if (INTRUSB & MUSB_INTR_CONNECT) {
        Usb_Host_Reset(ENABLE);
        delay();
        Usb_Host_Reset(DISABLE);

        printf("enter connect interrupt!\n");

        REG8(USB_BASE + MUSB_DEVCTL) = MUSB_DEVCTL_SESSION | MUSB_DEVCTL_HM | MUSB_INTR_CONNECT;
    }

    if (INTRUSB & MUSB_INTR_SOF) {
        printf("enter sof interrupt!\n");

        {
            // if (REG16(USB_BASE + REG_CSR0L) & MUSB_CSR0_H_ERROR) {
            //     REG16(USB_BASE + REG_CSR0L) &= ~MUSB_CSR0_H_ERROR;
            // }
            // if (REG16(USB_BASE + REG_CSR0L) & MUSB_CSR0_H_RXSTALL) {
            //     REG16(USB_BASE + REG_CSR0L) &= ~MUSB_CSR0_H_RXSTALL;
            // }
            // if (REG16(USB_BASE + REG_CSR0L) & MUSB_CSR0_H_NAKTIMEOUT) {
            //     REG16(USB_BASE + REG_CSR0L) &= ~MUSB_CSR0_H_NAKTIMEOUT;
            // }

            if (cnt < 21) {

                if ((REG16(CONFIG_EP(EP0) + MUSB_CSR0) & MUSB_CSR0_TXPKTRDY) == 0) {

                    Usb_Host_Ep0_Flush_Fifo();
                    cnt++;

                    if ((cnt % 21 == 0x1)) {
                        Usb_Write_Fifo(EP0, HOST_Discriptor_Device_SetUp, SETUP_SIZE);
                    }

                    if ((cnt % 21 == 0x2)) {
                        HOST_Set_Address_Setup[2] = 0x20;
                        Usb_Write_Fifo(EP0, HOST_Set_Address_Setup, SETUP_SIZE);
                    }

                    if ((cnt % 21 == 0x3)) {
                        Usb_Host_Set_Address(0x20);
                        HOST_Discriptor_Device_SetUp[6] = 0x12;
                        Usb_Write_Fifo(EP0, HOST_Discriptor_Device_SetUp, SETUP_SIZE);
                    }

                    if ((cnt % 21 == 0x5) || (cnt % 21 == 0xe)) {
                        Usb_Write_Fifo(EP0, HOST_Discriptor_Iserial_Number, SETUP_SIZE);
                    }

                    if ((cnt % 21 == 0x6) || (cnt % 21 == 0xf)) {

                        Usb_Write_Fifo(EP0, Host_Discriptor_LangID, SETUP_SIZE);
                    }

                    if ((cnt % 21 == 0x7) || (cnt % 21 == 0x10)) {

                        Usb_Write_Fifo(EP0, Host_Discriptor_Iproduct, SETUP_SIZE);
                    }
                    if ((cnt % 21 == 0x8) || (cnt % 21 == 0x11)) {
                        Usb_Write_Fifo(EP0, Host_Discriptor_configuration, SETUP_SIZE);
                    }
                    if ((cnt % 21 == 0x4) || (cnt % 21 == 0x9) || (cnt % 21 == 0x12)) {
                        //Host_Discriptor_configuration[6] = 0x22;
                        Usb_Write_Fifo(EP0, Host_Discriptor_configuration, SETUP_SIZE);
                    }

                    if ((cnt % 21 == 0xA)) {
                        Usb_Host_Set_Address(0x0);
                        HOST_Discriptor_Device_SetUp[6] = 0x40;
                        Usb_Write_Fifo(EP0, HOST_Discriptor_Device_SetUp, SETUP_SIZE);
                    }

                    if (cnt % 21 == 0xB) {
                        HOST_Set_Address_Setup[2] = 0x21;
                        Usb_Write_Fifo(EP0, HOST_Set_Address_Setup, SETUP_SIZE);
                    }

                    if (cnt % 21 == 0xC) {
                        Usb_Host_Set_Address(0x21);
                        HOST_Discriptor_Device_SetUp[6] = 0x12;
                        Usb_Write_Fifo(EP0, HOST_Discriptor_Device_SetUp, SETUP_SIZE);
                    }

                    if (cnt % 21 == 0xd) {
                        Host_Discriptor_configuration[6] = 0x22;
                        Usb_Write_Fifo(EP0, Host_Discriptor_configuration, SETUP_SIZE);
                    }
                    if (cnt % 21 == 0x13) {
                        Usb_Write_Fifo(EP0, Host_Set_Configuration, SETUP_SIZE);
                    }
                    if (cnt % 21 == 0x14) {
                        Usb_Write_Fifo(EP0, Host_Set_Idle, SETUP_SIZE);
                    }
                    if (cnt % 21 == 0) {
                        Usb_Write_Fifo(EP0, Host_Discriptor_report, SETUP_SIZE);
                    }
                    Usb_Host_Transmit_Setup(EP0);
                }

                while ((REG16(CONFIG_EP(EP0) + MUSB_CSR0) & MUSB_CSR0_TXPKTRDY))
                    ;

                {
                    Usb_Host_Req_IN_transaction(EP0);

                    delay();
                }

                Usb_Host_Receive_data(EP0, EPFIFO);

                if (cnt % 21 == 0) {
                    Usb_Host_Req_IN_transaction(EP0);
                    Usb_Host_Receive_data(EP0, EPFIFO);
                    flg = 1;
                }

                {
                    if ((cnt % 21 != 0x2) && (cnt % 21 != 0xb) && (cnt % 21 != 0x13) && (cnt % 21 != 0x14)) {

                        Usb_Host_Receive_data(EP0, EPFIFO);
                        Usb_Host_Ep0_Flush_Fifo();
                        Usb_Host_Out_Transaction(EP0);
                    }
                }

                {
                    if ((cnt % 21 == 0x1) || (cnt % 21 == 0x9) || (cnt % 21 == 0xA)) {
                        Usb_Host_Reset(ENABLE);
                        delay();
                        Usb_Host_Reset(DISABLE);
                    }
                }
            }
            if (flg == 0x1) {
                Usb_Host_Tx_RX_Type(&Usb_Host_Rxtype, HOST_RX);
                // REG8(USB_BASE + MUSB_INDEX)      = EP7;
                // REG16(USB_BASE + MUSB_TXFIFOADD) = 0x1;
                // REG8(USB_BASE + MUSB_TXFIFOSZ)   = 9;
                //  Usb_Write_Fifo(EP7, sb, 88);
                // Usb_Host_Tx_RX_Type(&Usb_Host_Txtype, HOST_TX);
                // REG8(CONFIG_EP(EP7) + MUSB_TXCSR)      = 1;
                //REG16(CONFIG_EP(EP7) + MUSB_TXCSR) |= 0x10 | MUSB_TXCSR_TXPKTRDY;
                REG16(CONFIG_EP(EP1) + MUSB_RXMAXP)    = 0x1ff;
                REG8(CONFIG_EP(EP1) + MUSB_RXINTERVAL) = 1;
                Usb_Host_Req_IN_transaction(EP1);
                printf("RqPktCount = %x\n", REG16(USB_BASE + 0x300 + 4));
                Usb_Host_Receive_data(EP1, EPFIFO);

                printf("MUSB_RXCSR = %x\n", REG16(CONFIG_EP(EP1) + MUSB_RXCSR));
            }
        }
    }
}
