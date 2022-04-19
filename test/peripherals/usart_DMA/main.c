/**
 * @file main.c
 * @brief USART dma polling memory to Peripheral transfer example.
 * @author liyouchuang (lycdemeng@163.com)
 * @version 1.0
 * @date 2022-03-24
 * @copyright Copyright (c) 2022 Icore, Inc 
 * Board Version    ：AiP32rv001_EVB_V01
 * Hardware setup   : Connect USART1 Tx pin (PA.09) to USART3 Rx pin (PD.9).
 *                    Connect USART1 Rx pin (PA.10) to USART3 Tx pin (PD.8).
 */

#include "at103.h"
#include "at103_uart.h"
#include "unity.h"

/* Private typedef -----------------------------------------------------------*/
typedef enum { FAILED = 0,
               PASSED = !FAILED } TestStatus;

/* Private define ------------------------------------------------------------*/
#define TxBufferSize1 (countof(TxBuffer1) - 1)
#define TxBufferSize2 (countof(TxBuffer2) - 1)

/* RBR Register */
#define USART1_DR_Address ((uint32_t)0x4001B000)
#define USART3_DR_Address ((uint32_t)0x40008000)
/* Private macro -------------------------------------------------------------*/
#define countof(a) (sizeof(a) / sizeof(*(a)))

/* Private variables ---------------------------------------------------------*/
USART_InitTypeDef USART_InitStructure;
DMA_InitTypeDef   DMA_InitStructure;
uint8_t           TxBuffer1[] = "USART DMA Polling: USARTy -> USARTz using DMA";
uint8_t           TxBuffer2[] = "USART DMA Polling: USARTz -> USARTy using DMA";
uint8_t           RxBuffer1[TxBufferSize2];
uint8_t           RxBuffer2[TxBufferSize1];

volatile TestStatus TransferStatus1 = FAILED;
volatile TestStatus TransferStatus2 = FAILED;

/* Private function prototypes -----------------------------------------------*/
void       GPIO_Configuration(void);
void       NVIC_Configuration(void);
void       DMA_Configuration(void);
TestStatus Buffercmp(uint8_t *pBuffer1, uint8_t *pBuffer2, uint16_t BufferLength);

/**
 * @brief uart dma polling memory to Peripheral transfer example.
 */
void test_func(void)
{
    /* Configure the GPIO ports */
    GPIO_Configuration();
    /* Configure NVIC */
    NVIC_Configuration();

    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA, ENABLE);

    /* USARTy_Tx_DMA_Channel (triggered by USARTy Tx event) Config */
    DMA_DeInit(DMA_Channel1);
    DMA_StructInit(&DMA_InitStructure);
    DMA_InitStructure.DMA_PeripheralBaseAddr  = USART1_DR_Address;
    DMA_InitStructure.DMA_MemoryBaseAddr      = (uint32_t)TxBuffer1;
    DMA_InitStructure.DMA_DIR                 = DMA_DIR_PeripheralDST;
    DMA_InitStructure.DMA_BufferSize          = TxBufferSize1;
    DMA_InitStructure.DMA_PeripheralInc       = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc           = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize  = DMA_PeripheralDataSize_Byte;
    DMA_InitStructure.DMA_MemoryDataSize      = DMA_MemoryDataSize_Byte;
    DMA_InitStructure.DMA_Mode                = DMA_Mode_Normal;
    DMA_InitStructure.DMA_Priority            = DMA_Priority_High;
    DMA_InitStructure.DMA_M2M                 = DMA_M2M_Disable;
    DMA_InitStructure.DMA_PeripheralHandshake = DMA_PeripheralHandshake_USART1_TX;
    DMA_Init(DMA_Channel1, &DMA_InitStructure);

    /* USARTy RX DMA1 Channel (triggered by USARTy Rx event) Config */
    DMA_DeInit(DMA_Channel2);
    DMA_InitStructure.DMA_PeripheralBaseAddr  = USART1_DR_Address;
    DMA_InitStructure.DMA_MemoryBaseAddr      = (uint32_t)RxBuffer1;
    DMA_InitStructure.DMA_DIR                 = DMA_DIR_PeripheralSRC;
    DMA_InitStructure.DMA_BufferSize          = TxBufferSize2;
    DMA_InitStructure.DMA_PeripheralHandshake = DMA_PeripheralHandshake_USART1_RX;
    DMA_Init(DMA_Channel2, &DMA_InitStructure);

    /* USARTz_Tx_DMA_Channel (triggered by USARTz Tx event) Config */
    DMA_DeInit(DMA_Channel3);
    DMA_InitStructure.DMA_PeripheralBaseAddr  = USART3_DR_Address;
    DMA_InitStructure.DMA_MemoryBaseAddr      = (uint32_t)TxBuffer2;
    DMA_InitStructure.DMA_DIR                 = DMA_DIR_PeripheralDST;
    DMA_InitStructure.DMA_BufferSize          = TxBufferSize2;
    DMA_InitStructure.DMA_PeripheralHandshake = DMA_PeripheralHandshake_USART3_TX;
    DMA_Init(DMA_Channel3, &DMA_InitStructure);

    /* USARTz RX DMA1 Channel (triggered by USARTz Rx event) Config */
    DMA_DeInit(DMA_Channel4);
    DMA_InitStructure.DMA_PeripheralBaseAddr  = USART3_DR_Address;
    DMA_InitStructure.DMA_MemoryBaseAddr      = (uint32_t)RxBuffer2;
    DMA_InitStructure.DMA_DIR                 = DMA_DIR_PeripheralSRC;
    DMA_InitStructure.DMA_BufferSize          = TxBufferSize1;
    DMA_InitStructure.DMA_PeripheralHandshake = DMA_PeripheralHandshake_USART3_RX;
    DMA_Init(DMA_Channel4, &DMA_InitStructure);

    USART_InitStructure.USART_BaudRate            = 115200;
    USART_InitStructure.USART_WordLength          = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits            = USART_StopBits_1;
    USART_InitStructure.USART_Parity              = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;

    /* Configure USART1 */
    USART_Init(USART1, &USART_InitStructure);
    // /* Configure USARTz */
    USART_Init(USART3, &USART_InitStructure);

    /* Enable USART1 DMA TX request */
    USART_DMACmd(USART1, USART_DMAReq_Tx, ENABLE);
    /* Enable USARTz DMA TX request */
    USART_DMACmd(USART3, USART_DMAReq_Tx, ENABLE);

    /* Enable DMA Channel1 transfer */
    DMA_Cmd(DMA_Channel1, ENABLE);
    /* Enable DMA Channel2 transfer */
    DMA_Cmd(DMA_Channel2, ENABLE);
    /* Enable DMA Channel1 transfer */
    DMA_Cmd(DMA_Channel3, ENABLE);
    /* Enable DMA Channel2 transfer */
    DMA_Cmd(DMA_Channel4, ENABLE);

    /* Wait until DMA Channel1  Transfer Complete */
    while (DMA_GetFlagStatus(DMA_FLAG_TC1) == RESET) {
    }
    /* Wait until DMA Channel2  Transfer Complete */
    while (DMA_GetFlagStatus(DMA_FLAG_TC2) == RESET) {
    }
    /* Wait until DMA Channel3  Transfer Complete */
    while (DMA_GetFlagStatus(DMA_FLAG_TC3) == RESET) {
    }
    /* Wait until DMA Channel4  Transfer Complete */
    while (DMA_GetFlagStatus(DMA_FLAG_TC4) == RESET) {
    }

    /* Check the received data with the send ones */
    TransferStatus1 = Buffercmp(TxBuffer2, RxBuffer1, TxBufferSize2);
    /* TransferStatus1 = PASSED, if the data transmitted from memory and
     received by USART1 are the same */
    /* TransferStatus1 = FAILED, if the data transmitted from memory and
     received by USART1 are different */
    TransferStatus2 = Buffercmp(TxBuffer1, RxBuffer2, TxBufferSize1);

    if (TransferStatus1 == PASSED || TransferStatus2 == PASSED) {
        debug("Transfer USART DMA [PASSED]!!!\n");
    } else {
        TEST_ASSERT_MESSAGE(0, "USART DMA controller function test fail !!!");
    }
}

/**
 * @brief Config USART1&USART3 GPIO
 */
void GPIO_Configuration(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    /* Remap USART1 use PA9 PA10 */
    GPIO_PinRemapConfig(GPIO_FullRemap_TIM1, ENABLE);
    GPIO_PinRemapConfig(GPIO_Remap_USART1, DISABLE);

    /* Enable the USART3 Pins Software Remapping */
    GPIO_PinRemapConfig(GPIO_FullRemap_USART3, ENABLE);

    /* Configure USART as AF mode */
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
    GPIO_InitStructure.GPIO_Pin   = USARTy_RxPin | USARTy_TxPin;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
    GPIO_Init(USARTy_GPIO, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = USARTz_RxPin | USARTz_TxPin;
    GPIO_Init(USARTz_GPIO, &GPIO_InitStructure);
}

/**
 * @brief Configures the nested vectored interrupt controller.
 */
void NVIC_Configuration(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel                   = DMA_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
    NVIC_InitStructure.NVIC_IRQChannel                   = USART1_IRQn;
    NVIC_Init(&NVIC_InitStructure);

    NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
    NVIC_Init(&NVIC_InitStructure);
    /* Enable Global Interrupt. */
    __enable_irq();
}

/*****************************************************************************/
void unity_test(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_func);
    UNITY_END();
}

void main(void)
{
    pll_init();
    sys_io_init();
    uart_init(UART_BOOT_PORT, UART_PARITY_NONE, UART_STOPBITS_1, UART_DATABITS_8, UART_BOOT_BD);
    debug("Usart controller test start!\r\n");
    unity_test();
    while (1) {
        /* test end */
    }
}

/**
 * @brief Compares two buffers.
 * @param[in] pBuffer1 buffers to be compared.
 * @param[in] pBuffer2 buffers to be compared.
 * @param[in] BufferLength buffer's length
 * @return TestStatus PASSED: pBuffer1 identical to pBuffer2 
 *                    FAILED: pBuffer1 differs from pBuffer2
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