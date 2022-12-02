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
#define CURSOR_STEP 5
void tfp_printf(char *fmt, ...);

#define printf tfp_printf

void delay(uint16_t xms)
{
    volatile uint16_t i, j = 0;
    for (i = 0; i < 0xff; i++)
        for (j = 0; j <= xms; j++)
            ;
}

uint8_t buf[8]     = {0};
uint8_t ep1_buf[4] = {0, 0, 0, 0};

NVIC_InitTypeDef NVIC_InitStructure;
NVIC_InitTypeDef NVIC_InitStructure1;
GPIO_InitTypeDef GPIOA_struct;

void Cfg_IO(void)
{
    GPIOA_struct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIOA_struct.GPIO_Pin  = GPIO_Pin_0;
    GPIO_Init(GPIOA, &GPIOA_struct);
}

void Set_System(void)
{
    RCC->USBPHY_CTRL.CLK_MODE_BIT   = 0x1;
    RCC->USBPHY_CTRL.PLL_EN_BIT     = 0x1;
    RCC->USBPHY_CTRL.IDDIG_BIT      = 0x1;
    RCC->USBPHY_CTRL.REFCLK_SEL_BIT = 0x1;
    /*USB AHB clk enable*/
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_USB, ENABLE);
    RCC_AHBPeriphResetCmd(RCC_AHBPeriph_USB, ENABLE);
    RCC_AHBPeriphResetCmd(RCC_AHBPeriph_USB, DISABLE);

    RCC_OTGFSCLKConfig(0xB); //usb PHY ref clk(12M) div(TBD)

    RCC->USBPHY_CTRL.value        = 0x0;
    RCC->USBPHY_CTRL.IDDIG_BIT    = 0x1;
    RCC->USBPHY_CTRL.CLK_MODE_BIT = 0x1;
    RCC->USBPHY_CTRL.AVALID_BIT   = 0x1;
}
void main(void)
{
    uint8_t val = 0;
    pll_init();
    sys_io_init();
    uart_init(UART_BOOT_PORT, UART_PARITY_NONE, UART_STOPBITS_1, UART_DATABITS_8, UART_BOOT_BD);

    AFIO->GPIO[0].IC &= ~(0x3 << 27);
    GPIOA_struct.GPIO_Mode = GPIO_Mode_AIN;
    GPIOA_struct.GPIO_Pin  = GPIO_Pin_12 | GPIO_Pin_11;
    GPIO_Init(GPIOA, &GPIOA_struct);

    Set_System();
    Cfg_IO();

    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
    NVIC_InitStructure.NVIC_IRQChannel                   = USB_MC_IRQn;
    NVIC_Init(&NVIC_InitStructure);

    NVIC_InitStructure1.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStructure1.NVIC_IRQChannel    = USB_DMA_IRQn;
    NVIC_Init(&NVIC_InitStructure1);
    __enable_irq();
    pProperty = &Device_Property;

    pProperty->Init();

    // REG8(USB_BASE + REG_POWER) &= ~0x4;
    // REG8(USB_BASE + REG_POWER) &= ~0x20;

    while (1) {

        debug("main progress!\n");
    }
}

void Usb_init()
{
    /*Detect VBUS_VALID*/
    while ((REG8(USB_BASE + REG_DEVCTL) & MUSB_DEVCTL_VBUS) != MUSB_DEVCTL_VBUS)
        ;
    REG16(USB_BASE + REG_INTRTXE) = 0x0;
    REG16(USB_BASE + REG_INTRRXE) = 0x0;
    REG8(USB_BASE + REG_INTRUSBE) = 0x0;
    REG8(USB_BASE + REG_TESTMODE) = 0x0;

    REG8(USB_BASE + REG_POWER) |= MUSB_POWER_RESUME | MUSB_POWER_ISOUPDATE;
    musb_enable_interrupts();
    REG8(USB_BASE + REG_INDEX) = EP0;

    REG8(USB_BASE + REG_DEVCTL) |= MUSB_DEVCTL_SESSION;
    REG8(USB_BASE + REG_POWER) = MUSB_POWER_SOFTCONN; /*soft connect*/
}

void Data_Setup0(void)
{
}

void USB_MC_IRQHandler(void)
{

    if (REG8(USB_BASE + REG_CSR0L) & MUSB_CSR0_RXPKTRDY) {

        REG8(USB_BASE + REG_CSR0L) |= MUSB_CSR0_P_SVDSETUPEND;
        pInformation        = musb_read_fifo(0, buf, 8);
        uint32_t Request_No = pInformation->bRequest;
        /*  clear the RxPktRdy bit. indicating that the command has read from the FIFO*/
        REG8(USB_BASE + REG_CSR0L) |= MUSB_CSR0_P_SVDRXPKTRDY;
        REG16(USB_BASE + REG_CSR0L) |= MUSB_CSR0_FLUSHFIFO;
        if ((Request_No == SET_ADDRESS)) {
            delay(0xf);
            REG8(USB_BASE + REG_FADDR) = pInformation->USBwValue0;
            REG8(USB_BASE + REG_CSR0L) |= MUSB_CSR0_P_DATAEND;
        }
        if ((Request_No == GET_DESCRIPTOR)) {
            if ((pInformation->USBwValue1 == DEVICE_DESCRIPTOR)) {

                musb_write_fifo(EP0, DeviceDescriptor, SIZ_DEVICE_DESC);
            }

            if ((pInformation->USBwValue1 == CONFIG_DESCRIPTOR)) {
                if (pInformation->USBwLength0 == 0xff) {

                    musb_write_fifo(EP0, ConfigDescriptor, SIZ_CONFIG_DESC);
                }
                /*config*/
                if ((pInformation->USBwLength0 < 0xff)) {

                    musb_write_fifo(EP0, ConfigDescriptor, pInformation->USBwLength0);
                }
            }

            if ((pInformation->USBwValue1 == STRING_DESCRIPTOR)) {
                /*serial number*/
                if ((pInformation->USBwValue0 == STRING3)) {

                    musb_write_fifo(EP0, StringSerial, SIZ_STRING_SERIAL);
                }
                /* lang ID*/
                if ((pInformation->USBwIndex == STRING0)) {

                    musb_write_fifo(EP0, StringLangID, SIZ_STRING_LANGID);
                }
                /*string iproduct*/
                if ((pInformation->USBwValue0 == STRING2)) {

                    musb_write_fifo(EP0, StringProduct, SIZ_STRING_PRODUCT);
                }
            }

            /*device qualifier*/
            if (pInformation->USBwValue1 == QUALIFIER_DESCRIPTOR) {

                REG8(USB_BASE + REG_CSR0L) |= MUSB_CSR0_P_SENDSTALL;
                REG8(USB_BASE + REG_CSR0L) |= MUSB_CSR0_P_SVDRXPKTRDY;
            }
            REG8(USB_BASE + REG_CSR0L) |= MUSB_CSR0_TXPKTRDY;
            REG8(USB_BASE + REG_CSR0L) |= MUSB_CSR0_P_DATAEND;
        }

        /*set idle*/
        if ((Request_No == GET_INTERFACE)) {
            REG8(USB_BASE + REG_CSR0L) |= MUSB_CSR0_P_DATAEND;
            REG8(USB_BASE + REG_CSR0L) |= MUSB_CSR0_P_SENDSTALL;
        }

        if ((pInformation->USBwValue1 == REPORT_DESCRIPTOR)) {

            musb_write_fifo(EP0, ReportDescriptor, SIZ_REPORT_DESC);
            REG8(USB_BASE + REG_CSR0L) |= MUSB_CSR0_TXPKTRDY;

            /*config ep1*/
            REG8(USB_BASE + REG_INDEX) = EP1;

            //REG8(USB_BASE + 0x110 + MUSB_TXCSR) |= 0x40;

            REG16(CONFIG_EP(EP1) + MUSB_TXMAXP) = 0x100f;
            REG16(CONFIG_EP(EP1) + MUSB_RXMAXP) = 0x100f;

            REG16(CONFIG_EP(EP1) + MUSB_TXCSR) &= ~((MUSB_TXCSR_P_ISO) | MUSB_TXCSR_P_MODE | MUSB_TXCSR_P_FrcDataTog);
            REG16(CONFIG_EP(EP1) + MUSB_TXCSR) |= MUSB_TXCSR_P_MODE | MUSB_TXCSR_P_AUTOSET | MUSB_TXCSR_P_FLUSHFIFO;
            // REG16(USB_BASE + 0x110 + MUSB_TXCSR) |= 0x1 << 6;
        }
    }

    {

        if (REG16(CONFIG_EP(EP1) + MUSB_TXCSR) & MUSB_TXCSR_P_UNDERRUN) {
            REG16(CONFIG_EP(EP1) + MUSB_TXCSR) &= ~MUSB_TXCSR_P_UNDERRUN;
            REG16(CONFIG_EP(EP1) + MUSB_TXCSR) |= MUSB_TXCSR_FLUSHFIFO;
            if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0) == Bit_RESET) {
                mouse_send();
                REG16(CONFIG_EP(EP1) + MUSB_TXCSR) |= MUSB_TXCSR_TXPKTRDY;
                REG16(CONFIG_EP(EP1) + MUSB_TXCSR) |= MUSB_TXCSR_FLUSHFIFO;
                REG16(CONFIG_EP(EP1) + MUSB_TXCSR) &= ~MUSB_TXCSR_SENTSTALL;
            } else {
            }
        }
    }
}

void mouse_send()
{
    ep1_buf[1] += CURSOR_STEP;
    musb_write_fifo(EP1, ep1_buf, 4);
}