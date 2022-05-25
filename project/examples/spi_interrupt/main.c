/**
 * @file main.c
 * @brief SPI 1Line Tx/Rx interrupt test
 * This example provides a basic communication between SPIy and SPIz using interrupt.
 * First, the SPIy,acts as master,sends SPIy_Buffer_Tx to SPIz, The SPIz ,acts as slaver,
 * reads the received data and store it into SPIz_Buffer_Rx.The received
 * data is then compared with the send ones and the result of this comparison is
 * stored in the "TransferStatus1" variable.
 *
 * Second, the SPIz,acts as master,sends SPIz_Buffer_Tx to SPIy, The SPIy, acts as slaver,
 * reads the received data and store it into SPIy_Buffer_Rx.The received
 * data is then compared with the send ones and the result of this comparison is
 * stored in the "TransferStatus2" .
 * Connect SPIy CSN pin (PA.04) to SPIz CSN pin (PB.12).
 * Connect SPIy SCK pin (PA.05) to SPIz SCK pin (PB.13).
 * Connect SPIy MOSI pin (PA.07) to SPIz MOSI pin (PB.15).
 * @author luoming (luoming@i-core.cn)
 * @version 1.0
 * @date 2022-04-09
 * @copyright Copyright (c) 2022 i-core, Inc
 */

#include "at103.h"

/* Private typedef -----------------------------------------------------------*/
typedef enum { FAILED = 0,
               PASSED = !FAILED } TestStatus;

/* Private define ------------------------------------------------------------*/
#define BufferSize 32

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
SPI_InitTypeDef  SPI_InitStructure;
GPIO_InitTypeDef GPIO_InitStructure;
uint8_t          itSPIy_Buffer_Tx[BufferSize] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
                                        0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E,
                                        0x0F, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15,
                                        0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C,
                                        0x1D, 0x1E, 0x1F, 0x20};
uint8_t          itSPIz_Buffer_Tx[BufferSize] = {0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57,
                                        0x58, 0x59, 0x5A, 0x5B, 0x5C, 0x5D, 0x5E,
                                        0x5F, 0x60, 0x61, 0x62, 0x63, 0x64, 0x65,
                                        0x66, 0x67, 0x68, 0x69, 0x6A, 0x6B, 0x6C,
                                        0x6D, 0x6E, 0x6F, 0x70};
uint8_t          itSPIy_Buffer_Rx[BufferSize], itSPIz_Buffer_Rx[BufferSize];
__IO uint8_t     spiyTxIdx       = 0;
__IO uint8_t     spiyRxIdx       = 0;
__IO uint8_t     spizTxIdx       = 0;
__IO uint8_t     spizRxIdx       = 0;
__IO uint8_t     k               = 0;
__IO TestStatus  TransferStatus1 = FAILED;
__IO TestStatus  TransferStatus2 = FAILED;

/* Private function prototypes -----------------------------------------------*/
void       RCC_Configuration(void);
void       GPIO_Configuration_SPIMOSI(void);
void       NVIC_Configuration(void);
TestStatus Buffercmp(uint8_t *pBuffer1, uint8_t *pBuffer2, uint16_t BufferLength);

/* Private functions ---------------------------------------------------------*/

void main(void)
{
    pll_init();
    sys_io_init();
    uart_init(UART_BOOT_PORT, UART_PARITY_NONE, UART_STOPBITS_1, UART_DATABITS_8, UART_BOOT_BD);
    RCC_Configuration();
    NVIC_Configuration();
    /**1st phase: SPIy as master TX,SPIz as slave RX. */
    /** Configure SPI_MASTER pins: CSN,SCK and MOSI. */
    SPI_DeInit(SPIy);
    SPI_DeInit(SPIz);
    GPIO_Configuration_SPIMOSI();

    SPI_InitStructure.SPI_Direction         = SPI_Direction_1Line_Tx;
    SPI_InitStructure.SPI_Mode              = SPI_Mode_Master;
    SPI_InitStructure.SPI_DataSize          = SPI_DataSize_8b;
    SPI_InitStructure.SPI_CPOL              = SPI_CPOL_High;
    SPI_InitStructure.SPI_CPHA              = SPI_CPHA_1Edge;
    SPI_InitStructure.SPI_NSS               = SPI_NSS_Hard;
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_64;
    SPI_InitStructure.SPI_FirstBit          = SPI_FirstBit_LSB;
    SPI_InitStructure.SPI_CRCPolynomial     = 7;
    /** Configure SPIy*/
    SPI_Init(SPIy, &SPI_InitStructure);
    /** Configure SPIz */
    SPI_InitStructure.SPI_Direction = SPI_Direction_1Line_Rx;
    SPI_InitStructure.SPI_Mode      = SPI_Mode_Slave;
    SPI_Init(SPIz, &SPI_InitStructure);
    spiyTxIdx = 0;
    spizRxIdx = 0;
    SPI_ClearITPendingBit(SPIy, SPI_IT_ALL);
    SPI_ClearITPendingBit(SPIz, SPI_IT_ALL);
    SPI_ITConfig(SPIy, SPI_IT_ALL, DISABLE);
    SPI_ITConfig(SPIz, SPI_IT_ALL, DISABLE);
    /** Enable SPIz */
    SPI_Cmd(SPIz, ENABLE);
    /** Enable SPIy */
    SPI_Cmd(SPIy, ENABLE);
    /** Enable SPIy CSN */
    SPI_SSOutputCmd(SPIy, ENABLE);
    /** Enable SPIy,SPIz Interrupt */
    SPI_ITConfig(SPIy, SPI_IT_TXE, ENABLE);
    SPI_ITConfig(SPIz, SPI_IT_RXNE, ENABLE);
    /** 1st phase: Transfer procedure */
    while (spizRxIdx < BufferSize) {
    }
    SPI_ITConfig(SPIy, SPI_IT_ALL, DISABLE);
    SPI_ITConfig(SPIz, SPI_IT_ALL, DISABLE);
    SPI_SSOutputCmd(SPIy, DISABLE);
    SPI_Cmd(SPIy, DISABLE);
    SPI_Cmd(SPIz, DISABLE);
    SPI_ClearITPendingBit(SPIy, SPI_IT_ALL);
    SPI_ClearITPendingBit(SPIz, SPI_IT_ALL);
    /* Check the correctness of written dada */
    TransferStatus1 = Buffercmp(itSPIz_Buffer_Rx, itSPIy_Buffer_Tx, BufferSize);
    if (TransferStatus1) {
        debug("masterSPIy->slaveSPIz 1Line TransferStatus = [PASSED]!!!\n");
    } else {
        debug("masterSPIy->slaveSPIz 1Line TransferStatus = [FAILED]!!!\n");
    }

    /**2nd phase: SPIy as slave RX,SPIz as master TX. */
    SPI_DeInit(SPIy);
    SPI_DeInit(SPIz);
    SPI_InitStructure.SPI_Direction         = SPI_Direction_1Line_Rx;
    SPI_InitStructure.SPI_Mode              = SPI_Mode_Slave;
    SPI_InitStructure.SPI_DataSize          = SPI_DataSize_8b;
    SPI_InitStructure.SPI_CPOL              = SPI_CPOL_High;
    SPI_InitStructure.SPI_CPHA              = SPI_CPHA_1Edge;
    SPI_InitStructure.SPI_NSS               = SPI_NSS_Hard;
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_32;
    SPI_InitStructure.SPI_FirstBit          = SPI_FirstBit_LSB;
    SPI_InitStructure.SPI_CRCPolynomial     = 7;
    /** Configure SPIy*/
    SPI_Init(SPIy, &SPI_InitStructure);
    /** Configure SPIz */
    SPI_InitStructure.SPI_Direction = SPI_Direction_1Line_Tx;
    SPI_InitStructure.SPI_Mode      = SPI_Mode_Master;
    SPI_Init(SPIz, &SPI_InitStructure);
    spiyRxIdx = 0;
    spizTxIdx = 0;
    SPI_ClearITPendingBit(SPIy, SPI_IT_ALL);
    SPI_ClearITPendingBit(SPIz, SPI_IT_ALL);
    SPI_ITConfig(SPIz, SPI_IT_ALL, DISABLE);
    SPI_ITConfig(SPIy, SPI_IT_ALL, DISABLE);
    /** Enable SPIz */
    SPI_Cmd(SPIz, ENABLE);
    /** Enable SPIy */
    SPI_Cmd(SPIy, ENABLE);
    /** Enable SPIZ CSN */
    SPI_SSOutputCmd(SPIz, ENABLE);
    /** Enable SPIy,SPIz Interrupt */
    SPI_ITConfig(SPIy, SPI_IT_RXNE, ENABLE);
    SPI_ITConfig(SPIz, SPI_IT_TXE, ENABLE);
    /** 2nd phase: Transfer procedure */
    while (spiyRxIdx < BufferSize) {
    }

    SPI_ITConfig(SPIy, SPI_IT_ALL, DISABLE);
    SPI_ITConfig(SPIz, SPI_IT_ALL, DISABLE);
    SPI_SSOutputCmd(SPIz, DISABLE);
    SPI_Cmd(SPIy, DISABLE);
    SPI_Cmd(SPIz, DISABLE);
    SPI_ClearITPendingBit(SPIy, SPI_IT_ALL);
    SPI_ClearITPendingBit(SPIz, SPI_IT_ALL);
    /* Check the correctness of written dada */
    TransferStatus2 = Buffercmp(itSPIy_Buffer_Rx, itSPIz_Buffer_Tx, BufferSize);
    if (TransferStatus2) {
        debug("masterSPIz->slaveSPIy 1Line TransferStatus = [PASSED]!!!\n");
    } else {
        debug("masterSPIz->slaveSPIy 1Line TransferStatus = [FAILED]!!!\n");
    }

    while (1) {
    }
}

/**
 * @brief  Configures the different system clocks.
 * @param  None
 * @retval None
 */
void RCC_Configuration(void)
{
    /* PCLK2 = HCLK/2 */
    RCC_PCLK2Config(RCC_HCLK_Div2);

    /**  Enable SPIy clock and GPIO clock for SPIy and SPIz */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOA | RCC_APB2Periph_SPI1, ENABLE);
    /**  Enable SPIz Periph clock */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
}

/**
 * @brief Config SPI1&SPI2GPIO AS MOSI
 */
void GPIO_Configuration_SPIMOSI(void)
{
    /** Remap SPI1 use PA4 PA5 PA6 PA7 */
    GPIO_PinRemapConfig(GPIO_Remap_SPI1, DISABLE);
    /** Remap SPI2 use PB12 PB13 PB14 PB15 */
    GPIO_PinRemapConfig(GPIO_Remap_SPI2, ENABLE);
    /* Configure SPIx as AF mode */
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Pin   = SPIy_PIN_CSN | SPIy_PIN_SCK | SPIy_PIN_MOSI;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
    GPIO_Init(SPIy_GPIO, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = SPIz_PIN_CSN | SPIz_PIN_SCK | SPIz_PIN_MOSI;
    GPIO_Init(SPIz_GPIO, &GPIO_InitStructure);
}

/**
 * @brief  Configures the SPI nested vectored interrupt controller.
 * @param  None
 * @retval None
 */
void NVIC_Configuration(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
    NVIC_InitStructure.NVIC_IRQChannel                   = SPIy_IRQn;
    NVIC_Init(&NVIC_InitStructure);

    NVIC_InitStructure.NVIC_IRQChannel = SPIz_IRQn;
    NVIC_Init(&NVIC_InitStructure);

    /* Enable Global Interrupt. */
    __enable_irq();
}

/**
 * @brief  Compares two buffers.
 * @param  pBuffer1, pBuffer2: buffers to be compared.
 * @param  BufferLength: buffer's length
 * @retval PASSED: pBuffer1 identical to pBuffer2
 *         FAILED: pBuffer1 differs from pBuffer2
 */
TestStatus Buffercmp(uint8_t *pBuffer1, uint8_t *pBuffer2, uint16_t BufferLength)
{
    while (BufferLength--) {
        if (*pBuffer1 != *pBuffer2) {
            return FAILED;
        }

        pBuffer1++;
        pBuffer2++;
    }

    return PASSED;
}

#ifdef USE_FULL_ASSERT

/**
 * @brief Reports the name of the source file and the source line number
 *        where the assert_param error has occurred.
 * @param[in] file: pointer to the source file name
 * @param[in] line: assert_param error line source number
 */
void assert_failed(uint8_t *file, uint32_t line)
{
    /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

    debug("Wrong parameters value: file %s on line %d\r\n", file, line);
    /* Infinite loop */
    while (1) {
    }
}

#endif
