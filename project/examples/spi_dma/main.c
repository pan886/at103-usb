/**
 * @file main.c
 * @brief spi dma example.
 * @author luoming (luoming@i-core.cn)
 * @version 1.0
 * @date 2022-04-19
 * @copyright Copyright (c) 2022 i-core, Inc
 */
/**
 * @file main.c
 * @brief SPI FullDuplex DMA example
 * This example provides a basic communication between SPI_MASTER and SPI_SLAVE using DMA.
 * The SPI_MASTER,acts as spi master,communicates with the SPI_SLAVE,acts as spi slave,in SPI FullDuplex mode.
 * The SPI transaction length  is 64 bytes
 * The SPI_MASTER transfers the data to be sent to txfifo from dmaSPI_MASTER_Buffer_Tx memory via DMA channel1,
 * and transfers the rxfifo data to  dmaSPI_MASTER_Buffer_Rx memory via  DMA channel2;
 * The SPI_SLAVE transfers the data to be sent to txfifo from dmaSPI_SLAVE_Buffer_Tx
 * memory via DMA channel3,transfers the rxfifo data to  dmaSPI_SLAVE_Buffer_Rx memory via  DMA channel4.
 *
 * The example will check whether the DMA transactions are done. If already completed,the example will
 * compare the 64 bytes data in dmaSPI_MASTER_Buffer_Tx memory with the data in dmaSPI_SLAVE_Buffer_Rx memory,
 * the 64 bytes data in dmaSPI_SLAVE_Buffer_Tx memory with the data in dmaSPI_MASTER_Buffer_Rx memory, and prints
 * the result by UART.

 * Connect SPIy CSN pin (PA.04) to SPIz CSN pin (PB.12).
 * Connect SPIy SCK pin (PA.05) to SPIz SCK pin (PB.13).
 * Connect SPIy MISO pin (PA.06) to SPIz MISO pin (PB.14).
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
#define BufferSize 64

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
SPI_InitTypeDef SPI_InitStructure;
DMA_InitTypeDef DMA_InitStructure;

uint8_t         dmaSPI_MASTER_Buffer_Tx[BufferSize] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
                                               0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E,
                                               0x0F, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15,
                                               0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C,
                                               0x1D, 0x1E, 0x1F, 0x20, 0x21, 0x22, 0x23,
                                               0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2A,
                                               0x2B, 0x2C, 0x2D, 0x2E, 0x2F, 0x30, 0x31,
                                               0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38,
                                               0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F,
                                               0x40};
uint8_t         dmaSPI_SLAVE_Buffer_Tx[BufferSize]  = {0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57,
                                              0x58, 0x59, 0x5A, 0x5B, 0x5C, 0x5D, 0x5E,
                                              0x5F, 0x60, 0x61, 0x62, 0x63, 0x64, 0x65,
                                              0x66, 0x67, 0x68, 0x69, 0x6A, 0x6B, 0x6C,
                                              0x6D, 0x6E, 0x6F, 0x70, 0x71, 0x72, 0x73,
                                              0x74, 0x75, 0x76, 0x77, 0x78, 0x79, 0x7A,
                                              0x7B, 0x7C, 0x7D, 0x7E, 0x7F, 0x80, 0x81,
                                              0x82, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88,
                                              0x89, 0x8A, 0x8B, 0x8C, 0x8D, 0x8E, 0x8F,
                                              0x90};
uint8_t         dmaSPI_SLAVE_Buffer_Rx[BufferSize];
uint8_t         dmaSPI_MASTER_Buffer_Rx[BufferSize];
__IO uint8_t    dmaTxIdx        = 0;
__IO uint8_t    dmaRxIdx        = 0;
__IO uint8_t    k               = 0;
__IO TestStatus TransferStatus1 = FAILED;
__IO TestStatus TransferStatus2 = FAILED;

/* Private function prototypes -----------------------------------------------*/
void       RCC_Configuration(void);
void       GPIO_Configuration(void);
TestStatus Buffercmp(uint8_t *pBuffer1, uint8_t *pBuffer2, uint16_t BufferLength);

/* Private functions ---------------------------------------------------------*/

void main(void)
{
    pll_init();

    sys_io_init();

    uart_init(UART_BOOT_PORT, UART_PARITY_NONE, UART_STOPBITS_1, UART_DATABITS_8, UART_BOOT_BD);

    RCC_Configuration();
    GPIO_Configuration();

    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA, ENABLE);

    DMA_Cmd(DMA_Channel1, DISABLE);
    DMA_Cmd(DMA_Channel2, DISABLE);
    DMA_Cmd(DMA_Channel3, DISABLE);
    DMA_Cmd(DMA_Channel4, DISABLE);

    DMA_DeInit(DMA_Channel1);
    DMA_StructInit(&DMA_InitStructure);
    DMA_InitStructure.DMA_PeripheralBaseAddr  = (uint32_t)SPI_MASTER_DT_Base;
    DMA_InitStructure.DMA_MemoryBaseAddr      = (uint32_t)dmaSPI_MASTER_Buffer_Tx;
    DMA_InitStructure.DMA_DIR                 = DMA_DIR_PeripheralDST;
    DMA_InitStructure.DMA_BufferSize          = BufferSize;
    DMA_InitStructure.DMA_PeripheralInc       = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc           = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize  = DMA_PeripheralDataSize_Byte;
    DMA_InitStructure.DMA_MemoryDataSize      = DMA_PeripheralDataSize_Byte;
    DMA_InitStructure.DMA_Mode                = DMA_Mode_Normal;
    DMA_InitStructure.DMA_Priority            = DMA_Priority_High;
    DMA_InitStructure.DMA_M2M                 = DMA_M2M_Disable;
    DMA_InitStructure.DMA_PeripheralHandshake = DMA_PeripheralHandshake_SPI1_TX;
    DMA_Init(DMA_Channel1, &DMA_InitStructure);
    debug("DMA1 transfer data length: %d\r\n", DMA_GetCurrDataCounter(DMA_Channel1));

    DMA_DeInit(DMA_Channel2);
    DMA_StructInit(&DMA_InitStructure);
    DMA_InitStructure.DMA_PeripheralBaseAddr  = (uint32_t)SPI_MASTER_DR_Base;
    DMA_InitStructure.DMA_MemoryBaseAddr      = (uint32_t)dmaSPI_MASTER_Buffer_Rx;
    DMA_InitStructure.DMA_DIR                 = DMA_DIR_PeripheralSRC;
    DMA_InitStructure.DMA_BufferSize          = BufferSize;
    DMA_InitStructure.DMA_PeripheralInc       = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc           = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize  = DMA_PeripheralDataSize_Byte;
    DMA_InitStructure.DMA_MemoryDataSize      = DMA_PeripheralDataSize_Byte;
    DMA_InitStructure.DMA_Mode                = DMA_Mode_Normal;
    DMA_InitStructure.DMA_Priority            = DMA_Priority_High;
    DMA_InitStructure.DMA_M2M                 = DMA_M2M_Disable;
    DMA_InitStructure.DMA_PeripheralHandshake = DMA_PeripheralHandshake_SPI1_RX;
    DMA_Init(DMA_Channel2, &DMA_InitStructure);
    debug("DMA2 transfer data length: %d\r\n", DMA_GetCurrDataCounter(DMA_Channel2));

    DMA_DeInit(DMA_Channel3);
    DMA_StructInit(&DMA_InitStructure);
    DMA_InitStructure.DMA_PeripheralBaseAddr  = (uint32_t)SPI_SLAVE_DT_Base;
    DMA_InitStructure.DMA_MemoryBaseAddr      = (uint32_t)dmaSPI_SLAVE_Buffer_Tx;
    DMA_InitStructure.DMA_DIR                 = DMA_DIR_PeripheralDST;
    DMA_InitStructure.DMA_BufferSize          = BufferSize;
    DMA_InitStructure.DMA_PeripheralInc       = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc           = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize  = DMA_PeripheralDataSize_Byte;
    DMA_InitStructure.DMA_MemoryDataSize      = DMA_PeripheralDataSize_Byte;
    DMA_InitStructure.DMA_Mode                = DMA_Mode_Normal;
    DMA_InitStructure.DMA_Priority            = DMA_Priority_High;
    DMA_InitStructure.DMA_M2M                 = DMA_M2M_Disable;
    DMA_InitStructure.DMA_PeripheralHandshake = DMA_PeripheralHandshake_SPI2_TX;
    DMA_Init(DMA_Channel3, &DMA_InitStructure);
    debug("DMA3 transfer data length: %d\r\n", DMA_GetCurrDataCounter(DMA_Channel3));

    DMA_DeInit(DMA_Channel4);
    DMA_StructInit(&DMA_InitStructure);
    DMA_InitStructure.DMA_PeripheralBaseAddr  = (uint32_t)SPI_SLAVE_DR_Base;
    DMA_InitStructure.DMA_MemoryBaseAddr      = (uint32_t)dmaSPI_SLAVE_Buffer_Rx;
    DMA_InitStructure.DMA_DIR                 = DMA_DIR_PeripheralSRC;
    DMA_InitStructure.DMA_BufferSize          = BufferSize;
    DMA_InitStructure.DMA_PeripheralInc       = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc           = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize  = DMA_PeripheralDataSize_Byte;
    DMA_InitStructure.DMA_MemoryDataSize      = DMA_PeripheralDataSize_Byte;
    DMA_InitStructure.DMA_Mode                = DMA_Mode_Normal;
    DMA_InitStructure.DMA_Priority            = DMA_Priority_High;
    DMA_InitStructure.DMA_M2M                 = DMA_M2M_Disable;
    DMA_InitStructure.DMA_PeripheralHandshake = DMA_PeripheralHandshake_SPI2_RX;
    DMA_Init(DMA_Channel4, &DMA_InitStructure);
    debug("DMA4 transfer data length: %d\r\n", DMA_GetCurrDataCounter(DMA_Channel4));

    SPI_DeInit(SPI_MASTER);
    SPI_DeInit(SPI_SLAVE);

    SPI_InitStructure.SPI_Direction         = SPI_Direction_2Lines_FullDuplex;
    SPI_InitStructure.SPI_Mode              = SPI_Mode_Master;
    SPI_InitStructure.SPI_DataSize          = SPI_DataSize_8b;
    SPI_InitStructure.SPI_CPOL              = SPI_CPOL_Low;
    SPI_InitStructure.SPI_CPHA              = SPI_CPHA_1Edge;
    SPI_InitStructure.SPI_NSS               = SPI_NSS_Hard;
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_64;
    SPI_InitStructure.SPI_FirstBit          = SPI_FirstBit_LSB;
    SPI_InitStructure.SPI_CRCPolynomial     = 7;

    /** Configure SPI MASTER*/
    SPI_Init(SPI_MASTER, &SPI_InitStructure);

    /** Configure SPI SLAVE*/
    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
    SPI_InitStructure.SPI_Mode      = SPI_Mode_Slave;
    SPI_Init(SPI_SLAVE, &SPI_InitStructure);
    /* Enable SPI_MASTER DMA request */
    SPI_DMACmd(SPI_MASTER, SPI_DMAReq_Tx, ENABLE);
    SPI_DMACmd(SPI_MASTER, SPI_DMAReq_Rx, ENABLE);
    SPI_DMACmd(SPI_SLAVE, SPI_DMAReq_Tx, ENABLE);
    SPI_DMACmd(SPI_SLAVE, SPI_DMAReq_Rx, ENABLE);

    DMA_Cmd(DMA_Channel1, ENABLE);
    DMA_Cmd(DMA_Channel2, ENABLE);
    DMA_Cmd(DMA_Channel3, ENABLE);
    DMA_Cmd(DMA_Channel4, ENABLE);
    /** Enable SPI MASTER CSN */
    SPI_SSOutputCmd(SPI_MASTER, ENABLE);
    /** Enable SPI MASTER */
    SPI_Cmd(SPI_MASTER, ENABLE);
    /** Enable SPI SLAVE */
    SPI_Cmd(SPI_SLAVE, ENABLE);
    /* Wait for DMA transmission to complete */
    while (DMA_GetFlagStatus(DMA_FLAG_GL1) == RESET) {
    }
    if (DMA_GetFlagStatus(DMA_FLAG_TE1) == SET) {
        debug("DMA channel1 transfer error.\r\n");
    }
    if (DMA_GetFlagStatus(DMA_FLAG_TC1) == SET) {
        debug("DMA channel1 transfer complete.\r\n");
    }
    DMA_ClearFlag(DMA_FLAG_GL1);

    while (DMA_GetFlagStatus(DMA_FLAG_GL2) == RESET) {
    }
    if (DMA_GetFlagStatus(DMA_FLAG_TE2) == SET) {
        debug("DMA channel2 transfer error.\r\n");
    }
    if (DMA_GetFlagStatus(DMA_FLAG_TC2) == SET) {
        debug("DMA channel2 transfer complete.\r\n");
    }
    DMA_ClearFlag(DMA_FLAG_GL2);

    while (DMA_GetFlagStatus(DMA_FLAG_GL3) == RESET) {
    }

    if (DMA_GetFlagStatus(DMA_FLAG_TE3) == SET) {
        debug("DMA channel3 transfer error.\r\n");
    }
    if (DMA_GetFlagStatus(DMA_FLAG_TC3) == SET) {
        debug("DMA channel3 transfer complete.\r\n");
    }
    DMA_ClearFlag(DMA_FLAG_GL3);

    while (DMA_GetFlagStatus(DMA_FLAG_GL4) == RESET) {
    }
    if (DMA_GetFlagStatus(DMA_FLAG_TE4) == SET) {
        debug("DMA channel4 transfer error.\r\n");
    }
    if (DMA_GetFlagStatus(DMA_FLAG_TC4) == SET) {
        debug("DMA channel4 transfer complete.\r\n");
    }
    DMA_ClearFlag(DMA_FLAG_GL4);

    SPI_SSOutputCmd(SPI_MASTER, DISABLE);
    SPI_Cmd(SPI_MASTER, DISABLE);
    SPI_Cmd(SPI_SLAVE, DISABLE);
    SPI_DMACmd(SPI_MASTER, SPI_DMAReq_Tx, DISABLE);
    SPI_DMACmd(SPI_MASTER, SPI_DMAReq_Rx, DISABLE);
    SPI_DMACmd(SPI_SLAVE, SPI_DMAReq_Tx, DISABLE);
    SPI_DMACmd(SPI_SLAVE, SPI_DMAReq_Rx, DISABLE);

    /* Check the correctness of written data */
    TransferStatus1 = Buffercmp(dmaSPI_SLAVE_Buffer_Rx, dmaSPI_MASTER_Buffer_Tx, BufferSize);
    TransferStatus2 = Buffercmp(dmaSPI_MASTER_Buffer_Rx, dmaSPI_SLAVE_Buffer_Tx, BufferSize);

    if (TransferStatus1) {
        debug("masterSPI->slaveSPI;slaveRx->DMA TransferStatus = [PASSED]!!!\n");
    } else {
        debug("masterSPI->slaveSPI;slaveRx->DMA  TransferStatus = [FAILED]!!!\n");
    }

    if (TransferStatus2) {
        debug("slaveSPI->masterSPI;masterRx->DMA TransferStatus = [PASSED]!!!\n");
    } else {
        debug("slaveSPI->masterSPI;masterRx->DMA  TransferStatus = [FAILED]!!!\n");
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
    /**  Enable SPI1 clock and GPIO clock for SPI1 and SPI2 */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOA | RCC_APB2Periph_SPI1, ENABLE);
    /**  Enable SPI2 Periph clock */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
}

/**
 * @brief Config SPI1&SPI2GPIO
 */
void GPIO_Configuration(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    /** Remap SPI1 use PA4 PA5 PA6 PA7 */
    GPIO_PinRemapConfig(GPIO_Remap_SPI1, DISABLE);
    /** Remap SPI2 use PB12 PB13 PB14 PB15 */
    GPIO_PinRemapConfig(GPIO_Remap_SPI2, ENABLE);
    /* Configure SPIx as AF mode */
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Pin   = SPI_MASTER_PIN_CSN | SPI_MASTER_PIN_SCK | SPI_MASTER_PIN_MOSI | SPI_MASTER_PIN_MISO;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
    GPIO_Init(SPI_MASTER_GPIO, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = SPI_SLAVE_PIN_CSN | SPI_SLAVE_PIN_SCK | SPI_SLAVE_PIN_MOSI | SPI_SLAVE_PIN_MISO;
    GPIO_Init(SPI_SLAVE_GPIO, &GPIO_InitStructure);
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