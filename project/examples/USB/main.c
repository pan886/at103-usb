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
//#include "at103_usb.h"

#define CURSOR_STEP 5

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
    AFIO->GPIO[0].IC &= ~(0x3 << 27);
    GPIOA_struct.GPIO_Mode = GPIO_Mode_AIN;
    GPIOA_struct.GPIO_Pin  = GPIO_Pin_12 | GPIO_Pin_11;
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

    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
    NVIC_InitStructure.NVIC_IRQChannel                   = USB_MC_IRQn;
    NVIC_Init(&NVIC_InitStructure);

    NVIC_InitStructure1.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStructure1.NVIC_IRQChannel    = USB_DMA_IRQn;
    NVIC_Init(&NVIC_InitStructure1);
    __enable_irq();
}

void main(void)
{
    uint8_t val = 0;
    pll_init();
    sys_io_init();
    uart_init(UART_BOOT_PORT, UART_PARITY_NONE, UART_STOPBITS_1, UART_DATABITS_8, UART_BOOT_BD);
    Set_System();
    Cfg_IO();
    pProperty = &Device_Property;
    pProperty->Init();

    while (1) {

        debug("main progress!\n");
    }
}

void Usb_init()
{
    /*Detect VBUS_VALID*/
    while ((REG8(USB_BASE + MUSB_DEVCTL) & MUSB_DEVCTL_VBUS) != MUSB_DEVCTL_VBUS)
        ;
    Usb_All_Interrupts(DISABLE);
    Usb_ISO_Update(ENABLE);
    Usb_All_Interrupts(ENABLE);
    Usb_Start_Session(ENABLE);
    Usb_Soft_Connect(ENABLE);
}

void mouse_send()
{
    ep1_buf[1] += CURSOR_STEP;
    Usb_Write_Fifo(EP1, ep1_buf, 4);
    Usb_Device_Transmiit(EP1);
}

void USB_MC_IRQHandler(void)
{

    if (REG16(CONFIG_EP(EP0) + MUSB_CSR0) & MUSB_CSR0_RXPKTRDY) {

        Usb_Device_Clear_Setup();
        pInformation        = Usb_Read_Fifo(EP0, buf, 8);
        uint32_t Request_No = pInformation->bRequest;
        /*  clear the RxPktRdy bit. indicating that the command has read from the FIFO*/
        Usb_Device_Clear_RXPktRdy();
        Usb_Device_Flush_fifo(EP0);
        if ((Request_No == SET_ADDRESS)) {
            delay(0xf);
            Usb_Set_Address(pInformation->USBwValue0);
            REG16(CONFIG_EP(EP0) + MUSB_CSR0) = MUSB_CSR0_P_DATAEND;
        }
        if ((Request_No == GET_DESCRIPTOR)) {
            if ((pInformation->USBwValue1 == DEVICE_DESCRIPTOR)) {

                Usb_Write_Fifo(EP0, DeviceDescriptor, SIZ_DEVICE_DESC);
            }

            if ((pInformation->USBwValue1 == CONFIG_DESCRIPTOR)) {
                if (pInformation->USBwLength0 == 0xff) {

                    Usb_Write_Fifo(EP0, ConfigDescriptor, SIZ_CONFIG_DESC);
                }
                /*config*/
                if ((pInformation->USBwLength0 < 0xff)) {

                    Usb_Write_Fifo(EP0, ConfigDescriptor, pInformation->USBwLength0);
                }
            }

            if ((pInformation->USBwValue1 == STRING_DESCRIPTOR)) {
                /*serial number*/
                if ((pInformation->USBwValue0 == STRING3)) {

                    Usb_Write_Fifo(EP0, StringSerial, SIZ_STRING_SERIAL);
                }
                /* lang ID*/
                if ((pInformation->USBwIndex == STRING0)) {

                    Usb_Write_Fifo(EP0, StringLangID, SIZ_STRING_LANGID);
                }
                /*string iproduct*/
                if ((pInformation->USBwValue0 == STRING2)) {

                    Usb_Write_Fifo(EP0, StringProduct, SIZ_STRING_PRODUCT);
                }
            }

            /*device qualifier*/
            if (pInformation->USBwValue1 == QUALIFIER_DESCRIPTOR) {
                Usb_Device_Send_Stall(EP0);
            }
            Usb_Device_Data_End();
            Usb_Device_Transmiit(EP0);
        }

        /*set idle*/
        if ((Request_No == GET_INTERFACE)) {
            Usb_Device_Data_End();
            Usb_Device_Send_Stall(EP0);
        }

        if ((pInformation->USBwValue1 == REPORT_DESCRIPTOR)) {

            Usb_Write_Fifo(EP0, ReportDescriptor, SIZ_REPORT_DESC);

            Usb_Device_Transmiit(EP0);
            /*config ep1*/
            Usb_Device_Tx_RX_Type(EP1, TX);
        }
    }

    {

        if (REG16(CONFIG_EP(EP1) + MUSB_TXCSR) & MUSB_TXCSR_P_UNDERRUN) {
            REG16(CONFIG_EP(EP1) + MUSB_TXCSR) &= ~MUSB_TXCSR_P_UNDERRUN;
            Usb_Device_Flush_fifo(EP1);
            if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0) == Bit_RESET) {
                mouse_send();
            } else {
            }
        }
    }
}
