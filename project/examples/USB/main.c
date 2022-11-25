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
uint16_t abc = 0;
uint16_t i, j = 0;

#define USB_BAS   0x40028000
#define REG_POWER 0x1
void tfp_printf(char *fmt, ...);

#define printf tfp_printf
extern const uint8_t DeviceDescriptor[SIZ_DEVICE_DESC];
const uint8_t        Joystick_ConfigDescriptor[SIZ_CONFIG_DESC];
uint8_t              Joystick_StringSerial[SIZ_STRING_SERIAL];
const uint8_t        Joystick_StringProduct[SIZ_STRING_PRODUCT];
const uint8_t        Joystick_StringLangID[SIZ_STRING_LANGID];
const uint8_t        Joystick_ConfigDescriptor[SIZ_CONFIG_DESC];
const uint8_t        Joystick_ReportDescriptor[SIZ_REPORT_DESC];

const uint8_t musb_test_packet[53] = {

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

};

void musb_write_fifo(const uint8_t abc[], uint8_t length)
{
    for (i = 0; i < length; i++) {
        REG8(USB_BASE + REG_EP0_FIFO) = abc[i];
    }
}

uint8_t *musb_read_fifo(uint8_t index, uint8_t buffer[], uint8_t length)
{

    for (i = 0; i < length; i++) {
        buffer[i] = REG8(USB_BASE + REG_EP0_FIFO);
        //printf("%x\n", buffer[i]);
    }
    return buffer;
}
void musb_load_testpacket(uint8_t a)
{

    musb_write_fifo(musb_test_packet, 53);
    REG8(USB_BASE + REG_TESTMODE) = 0x1 << 3 | 0x1 << 6 | 0x1 << 5;
    REG8(USB_BASE + REG_CSR0L)    = 0x2;
}

void delay(uint16_t xms)
{
    volatile uint16_t i, j, k = 0;
    for (i = 0; i < 0xff; i++)
        for (k = 0; k <= xms; k++)
            ;
}

uint8_t          buf[8]    = {0};
uint8_t          stop_flag = 0;
NVIC_InitTypeDef NVIC_InitStructure;
NVIC_InitTypeDef NVIC_InitStructure1;
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

    GPIOA_struct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIOA_struct.GPIO_Pin  = GPIO_Pin_0;
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
    NVIC_InitStructure1.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStructure1.NVIC_IRQChannel    = USB_DMA_IRQn;
    NVIC_Init(&NVIC_InitStructure1);
    __enable_irq();
    musb_enable_interrupts();
    REG8(USB_BASE + REG_INDEX) = 0x0;
    // debug("\r\n");
    // debug("INTRRXE = %x\n", REG16(USB_BAS + REG_INTRRXE));
    // debug("REG_INTRTXE = %x\n", REG16(USB_BAS + REG_INTRTXE));
    usb_dev_cfg_before_u_rst();

    REG8(USB_BAS + REG_DEVCTL) |= 0x1;
    while (1) {
        val = REG8(USB_BAS + REG_DEVCTL);

        if ((val & 0x10) == 0x10) {
            debug("Detect VBUS_VALID, USB_DEVCTL=%x\n", val);
            break;
        }
    }
    pProperty = &Device_Property;
    /* interrupt enable bits for each of the interrupts in IntrUSB*/

    REG8(USB_BASE + REG_POWER) = 0x40; /*soft connect*/
    if (REG32(USB_BASE + REG_POWER) == 0x40)
        debug("soft connect complete!\n");

    REG16(USB_BAS + REG_INTRTXE) = 0xffff;
    REG16(USB_BAS + REG_INTRRXE) = 0xfffe;

    REG8(USB_BAS + REG_INTRUSBE) = 0xff;
    REG8(USB_BAS + REG_POWER) &= ~0x4;
    REG8(USB_BAS + REG_POWER) &= ~0x20;

    debug("FIFOSIZE = %x\n", REG8(USB_BAS + MUSB_FIFOSIZE));

    debug("REG_EP_TXMAXP = %x\n", REG16(USB_BAS + REG_EP_TXMAXP));
    debug("REG_EP_RXMAXP = %x\n", REG16(USB_BAS + REG_EP_RXMAXP));

    while (1) {

        debug("main progress!\n");
    }
}

usb_dev_cfg_before_u_rst()
{
    REG16(USB_BAS + REG_INTRTXE) = 0x0;
    REG16(USB_BAS + REG_INTRRXE) = 0x0;
    REG8(USB_BAS + REG_INTRUSBE) = 0x0;
    REG8(USB_BAS + REG_TESTMODE) = 0x0;

    REG8(USB_BAS + REG_POWER) |= 0x4 | 0x80;
}

void Data_Setup0(void)
{
}

void USB_MC_IRQHandler(void)
{

    debug("enter interrupt!\n");

    if (REG8(USB_BAS + REG_CSR0L) & 0x1) {

        REG8(USB_BASE + REG_CSR0L) |= 0x1 << 7;
        pInformation        = musb_read_fifo(0, buf, 8);
        uint32_t Request_No = pInformation->bRequest;
        /*  clear the RxPktRdy bit. indicating that the command has read from the FIFO*/
        REG8(USB_BASE + REG_CSR0L) |= 0x1 << 6;

        if ((Request_No == SET_ADDRESS)) {
            delay(0xf);
            REG8(USB_BASE + REG_FADDR) = pInformation->USBwValue0;
            REG8(USB_BASE + REG_CSR0L) |= 0x1 << 3;
            debug("faddr = %x\n", REG8(USB_BASE + REG_FADDR));
        }
        if ((Request_No == GET_DESCRIPTOR)) {
            if ((pInformation->USBwValue1 == DEVICE_DESCRIPTOR)) {
                REG16(USB_BASE + REG_CSR0H) |= 0x1;
                musb_write_fifo(DeviceDescriptor, SIZ_DEVICE_DESC);
            }

            if ((pInformation->USBwValue1 == CONFIG_DESCRIPTOR)) {
                if (pInformation->USBwLength0 == 0xff) {
                    REG16(USB_BASE + REG_CSR0H) |= 0x1;
                    musb_write_fifo(ConfigDescriptor, SIZ_CONFIG_DESC);
                }
                /*config*/
                if ((pInformation->USBwLength0 < 0xff)) {
                    REG16(USB_BASE + REG_CSR0H) |= 0x1;
                    musb_write_fifo(ConfigDescriptor, pInformation->USBwLength0);
                }
            }

            if ((pInformation->USBwValue1 == STRING_DESCRIPTOR)) {
                // /*serial number*/
                if ((pInformation->USBwValue0 == STRING3)) {
                    REG16(USB_BASE + REG_CSR0H) |= 0x1;
                    musb_write_fifo(StringSerial, SIZ_STRING_SERIAL);
                }
                /* lang ID*/
                if ((pInformation->USBwIndex == STRING0)) {
                    REG8(USB_BASE + REG_CSR0H) |= 0x1;
                    musb_write_fifo(StringLangID, SIZ_STRING_LANGID);
                }
                /*string iproduct*/
                if ((pInformation->USBwValue0 == STRING2)) {
                    REG16(USB_BASE + REG_CSR0H) |= 0x1;
                    musb_write_fifo(StringProduct, SIZ_STRING_PRODUCT);
                }
            }

            /*device qualifier*/
            if (pInformation->USBwValue1 == QUALIFIER_DESCRIPTOR) {

                REG8(USB_BASE + REG_CSR0L) |= (0x1 << 5);
                REG8(USB_BASE + REG_CSR0L) |= (0x1 << 6);
                REG8(USB_BASE + REG_CSR0L) |= (0x1 << 3);
            }
            REG8(USB_BASE + REG_CSR0L) |= 0x2;
            REG8(USB_BASE + REG_CSR0L) |= 0x1 << 3;
        }

        /*set idle*/
        if ((Request_No == GET_INTERFACE)) {
            REG8(USB_BASE + REG_CSR0H) |= 0x1;
            REG8(USB_BASE + REG_CSR0L) |= 0x1 << 3;
            REG8(USB_BASE + REG_CSR0L) |= 0x1 << 5;
        }

        if ((pInformation->USBwValue1 == REPORT_DESCRIPTOR)) {

            musb_write_fifo(ReportDescriptor, SIZ_REPORT_DESC);
            REG8(USB_BASE + REG_CSR0L) |= 0x2;

            /*config ep1*/
            REG8(USB_BASE + REG_INDEX) = 0x1;

            //REG8(USB_BASE + 0x110 + MUSB_TXCSR) |= 0x40;

            REG16(USB_BASE + 0x110 + MUSB_TXMAXP) = 0x100f;
            REG16(USB_BASE + 0x110 + MUSB_RXMAXP) = 0x100f;
            //  REG16(USB_BASE + 0x110 + MUSB_RXCSR) |= 0x1 << 12;
            REG16(USB_BASE + 0x110 + MUSB_TXCSR) &= ~((0x1 << 14) | (0x1 << 12) | (0x1 << 11));
            REG16(USB_BASE + 0x110 + MUSB_TXCSR) |= (0x1 << 13) | (0x1 << 15) | (0x1 << 3);
            // REG16(USB_BASE + 0x110 + MUSB_TXCSR) |= 0x1 << 6;
        }
    }

    {

        if (REG16(USB_BASE + 0x110 + MUSB_TXCSR) & (0x1 << 2)) {
            REG16(USB_BASE + 0x110 + MUSB_TXCSR) &= ~(0x1 << 2);
            REG16(USB_BASE + 0x110 + MUSB_TXCSR) |= 0x1 << 3;
            if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0) == 0) {
                REG8(USB_BASE + REG_EP1_FIFO) = 0x2;
                REG8(USB_BASE + REG_EP1_FIFO) = 0x0;
                REG8(USB_BASE + REG_EP1_FIFO) = 0x0;
                REG8(USB_BASE + REG_EP1_FIFO) = 0x0;
            } else {
                REG8(USB_BASE + REG_EP1_FIFO) = 0x0;
                REG8(USB_BASE + REG_EP1_FIFO) = 0x0;
                REG8(USB_BASE + REG_EP1_FIFO) = 0x0;
                REG8(USB_BASE + REG_EP1_FIFO) = 0x0;
            }

            REG16(USB_BASE + 0x110 + MUSB_TXCSR) |= 0x1;
            REG16(USB_BASE + 0x110 + MUSB_TXCSR) |= 0x1 << 3;
            REG16(USB_BASE + 0x110 + MUSB_TXCSR) &= ~(0x1 << 5);
            REG16(USB_BASE + 0x110 + MUSB_TXCSR) &= ~(0x1 << 4);
        }
    }
}
