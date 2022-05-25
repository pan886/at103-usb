/**
 * @file main.c
 * @brief SPI FullDuplex poll test
 *This example provides a basic communication between SPIy and SPIz using flags.
 * First, the SPIy,acts as master,sends SPIy_Buffer_Tx to SPIz,receives data from
 * SPIz and store it into SPIy_Buffer_Rx. The SPIz ,acts as slaver,reads the received
 * data and store it into SPIz_Buffer_Rx,sends SPIz_Buffer_Tx to SPIy.The received
 * data is then compared with the send ones and the result of this comparison is
 * stored in the "TransferStatus1" "TransferStatus2" variable.
 *
 * Second, the SPIz,acts as master,sends SPIz_Buffer_Tx to SPIy,receives data from
 * SPIy and store it into SPIz_Buffer_Rx. The SPIy, acts as slaver,reads the received
 * data and store it into SPIy_Buffer_Rx,sends SPIy_Buffer_Tx to SPIz.The received
 * data is then compared with the send ones and the result of this comparison is
 * stored in the "TransferStatus3" "TransferStatus4" variable.
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
#define BufferSize 32

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
SPI_InitTypeDef SPI_InitStructure;
uint8_t         SPIy_Buffer_Tx[BufferSize] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
                                      0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E,
                                      0x0F, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15,
                                      0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C,
                                      0x1D, 0x1E, 0x1F, 0x20};
uint8_t         SPIz_Buffer_Tx[BufferSize] = {0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57,
                                      0x58, 0x59, 0x5A, 0x5B, 0x5C, 0x5D, 0x5E,
                                      0x5F, 0x60, 0x61, 0x62, 0x63, 0x64, 0x65,
                                      0x66, 0x67, 0x68, 0x69, 0x6A, 0x6B, 0x6C,
                                      0x6D, 0x6E, 0x6F, 0x70};
uint8_t         SPIy_Buffer_Rx[BufferSize], SPIz_Buffer_Rx[BufferSize];
__IO uint8_t    TxIdx = 0, RxIdx = 0, k = 0;
__IO TestStatus TransferStatus1 = FAILED;
__IO TestStatus TransferStatus2 = FAILED;
__IO TestStatus TransferStatus3 = FAILED;
__IO TestStatus TransferStatus4 = FAILED;

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
    /**1st phase: SPIy as master,SPIz as slave. */
    SPI_DeInit(SPIy);
    SPI_DeInit(SPIz);
    SPI_InitStructure.SPI_Direction         = SPI_Direction_2Lines_FullDuplex;
    SPI_InitStructure.SPI_Mode              = SPI_Mode_Master;
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
    SPI_InitStructure.SPI_Mode = SPI_Mode_Slave;
    SPI_Init(SPIz, &SPI_InitStructure);
    /** Enable SPIy CSN */
    SPI_SSOutputCmd(SPIy, ENABLE);
    /** Enable SPIy */
    SPI_Cmd(SPIy, ENABLE);
    /** Enable SPIz */
    SPI_Cmd(SPIz, ENABLE);
    /** 1st phase: Transfer procedure */
    TxIdx = 0;
    RxIdx = 0;
    while (RxIdx < BufferSize) {
        while (SPI_GetFlagStatus(SPIy, SPI_FLAG_TXE) == RESET)
            ;
        SPI_SendData(SPIz, SPIz_Buffer_Tx[TxIdx]);
        SPI_SendData(SPIy, SPIy_Buffer_Tx[TxIdx++]);
        while (SPI_GetFlagStatus(SPIz, SPI_FLAG_RXNE) == RESET)
            ;
        SPIz_Buffer_Rx[RxIdx] = SPI_ReceiveData(SPIz);
        while (SPI_GetFlagStatus(SPIy, SPI_FLAG_RXNE) == RESET)
            ;
        SPIy_Buffer_Rx[RxIdx++] = SPI_ReceiveData(SPIy);
    }
    SPI_Cmd(SPIy, DISABLE);
    SPI_Cmd(SPIz, DISABLE);
    SPI_SSOutputCmd(SPIy, DISABLE);
    /**1st phase Check the correctness of written dada */
    TransferStatus1 = Buffercmp(SPIz_Buffer_Rx, SPIy_Buffer_Tx, BufferSize);
    TransferStatus2 = Buffercmp(SPIy_Buffer_Rx, SPIz_Buffer_Tx, BufferSize);
    if (TransferStatus1) {
        debug("masterSPIy->slaveSPIz TransferStatus = [PASSED]!!!\n");
    } else {
        debug("masterSPIy->slaveSPIz TransferStatus = [FAILED]!!!\n");
    }
    if (TransferStatus2) {
        debug("slaveSPIz->masterSPIy  TransferStatus = [PASSED]!!!\n");
    } else {
        debug("slaveSPIz->masterSPIy TransferStatus = [FAILED]!!!\n");
    }

    /** 2nd phase: SPIy Slave and SPIz Master */
    SPI_InitStructure.SPI_Direction         = SPI_Direction_2Lines_FullDuplex;
    SPI_InitStructure.SPI_Mode              = SPI_Mode_Master;
    SPI_InitStructure.SPI_DataSize          = SPI_DataSize_8b;
    SPI_InitStructure.SPI_CPOL              = SPI_CPOL_High;
    SPI_InitStructure.SPI_CPHA              = SPI_CPHA_2Edge;
    SPI_InitStructure.SPI_NSS               = SPI_NSS_Hard;
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_128;
    SPI_InitStructure.SPI_FirstBit          = SPI_FirstBit_LSB;
    SPI_InitStructure.SPI_CRCPolynomial     = 7;
    /** Configure SPIz*/
    SPI_Init(SPIz, &SPI_InitStructure);
    /** Configure SPIy */
    SPI_InitStructure.SPI_Mode = SPI_Mode_Slave;
    SPI_Init(SPIy, &SPI_InitStructure);
    /** 2nd phase: Transfer procedure */
    TxIdx = 0;
    RxIdx = 0;
    for (k = 0; k < BufferSize; k++) {
        SPIz_Buffer_Rx[k] = 0;
    }
    for (k = 0; k < BufferSize; k++) {
        SPIy_Buffer_Rx[k] = 0;
    }
    /** Enable SPIz CSN */
    SPI_SSOutputCmd(SPIz, ENABLE);
    /** Enable SPIy */
    SPI_Cmd(SPIy, ENABLE);
    /** Enable SPIz */
    SPI_Cmd(SPIz, ENABLE);
    while (RxIdx < BufferSize) {
        while (SPI_GetFlagStatus(SPIz, SPI_FLAG_TXE) == RESET)
            ;
        SPI_SendData(SPIy, SPIy_Buffer_Tx[TxIdx]);
        SPI_SendData(SPIz, SPIz_Buffer_Tx[TxIdx++]);
        while (SPI_GetFlagStatus(SPIy, SPI_FLAG_RXNE) == RESET)
            ;
        SPIy_Buffer_Rx[RxIdx] = SPI_ReceiveData(SPIy);
        while (SPI_GetFlagStatus(SPIz, SPI_FLAG_RXNE) == RESET)
            ;
        SPIz_Buffer_Rx[RxIdx++] = SPI_ReceiveData(SPIz);
    }
    SPI_Cmd(SPIy, DISABLE);
    SPI_Cmd(SPIz, DISABLE);
    SPI_SSOutputCmd(SPIz, DISABLE);
    /** 2nd phase Check the correctness of written dada */
    TransferStatus3 = Buffercmp(SPIy_Buffer_Rx, SPIz_Buffer_Tx, BufferSize);
    TransferStatus4 = Buffercmp(SPIz_Buffer_Rx, SPIy_Buffer_Tx, BufferSize);

    if (TransferStatus3) {
        debug("masterSPIz->slaveSPIy TransferStatus = [PASSED]!!!\n");
    } else {
        debug("masterSPIz->slaveSPIy TransferStatus = [FAILED]!!!\n");
    }
    if (TransferStatus4) {
        debug("slaveSPIy->masterSPIz TransferStatus = [PASSED]!!!\n");
    } else {
        debug("slaveSPIy->masterSPIz TransferStatus = [FAILED]!!!\n");
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
    GPIO_InitStructure.GPIO_Pin   = SPIy_PIN_CSN | SPIy_PIN_SCK | SPIy_PIN_MOSI | SPIy_PIN_MISO;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
    GPIO_Init(SPIy_GPIO, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = SPIz_PIN_CSN | SPIz_PIN_SCK | SPIz_PIN_MOSI | SPIz_PIN_MISO;
    GPIO_Init(SPIz_GPIO, &GPIO_InitStructure);
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