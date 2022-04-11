/**
 * @file main.c
 * @brief uart module interrupt test
 * This example provides a basic communication between USARTy and USARTz using
 * interrupts.USARTz sends TxBuffer2 to USARTy which sends TxBuffer1 to USARTz.
 * The data received by USARTy and USARTz are stored respectively in RxBuffer1
 * and RxBuffer2. The data transfer is managed in USARTy_IRQHandler and
 * USARTz_IRQHandler in at103_it.c file.
 *
 * Connect USART1 Tx pin (PA.09) to USART3 Rx pin (PD.9)
 * Connect USART1 Rx pin (PA.10) to USART3 Tx pin (PD.8)
 * PA.10 and PA.09 are on jumper J90
 *
 * @author zhangsheng (zhangsheng@zhangsheng.ic.com)
 * @version 1.0
 * @date 2022-02-13
 * @copyright Copyright (c) 2022 Icore, Inc
 */

#include "at103.h"

/* Private typedef -----------------------------------------------------------*/
typedef enum { FAILED = 0,
               PASSED = !FAILED } TestStatus;

/* Private define ------------------------------------------------------------*/
#define TxBufferSize1 (countof(TxBuffer1) - 1)
#define TxBufferSize2 (countof(TxBuffer2) - 1)
#define RxBufferSize1 TxBufferSize2
#define RxBufferSize2 TxBufferSize1

/* Private macro -------------------------------------------------------------*/
#define countof(a) (sizeof(a) / sizeof(*(a)))

/* Private variables ---------------------------------------------------------*/
USART_InitTypeDef USART_InitStructure;
uint8_t           TxBuffer1[] = "USART Interrupt Example: USARTy -> USARTz using Interrupt";
uint8_t           TxBuffer2[] = "USART Interrupt Example: USARTz -> USARTy using Interrupt";
uint8_t           RxBuffer1[RxBufferSize1];
uint8_t           RxBuffer2[RxBufferSize2];
__IO uint8_t      TxCounter1           = 0x00;
__IO uint8_t      TxCounter2           = 0x00;
__IO uint8_t      RxCounter1           = 0x00;
__IO uint8_t      RxCounter2           = 0x00;
uint8_t           NbrOfDataToTransfer1 = TxBufferSize1;
uint8_t           NbrOfDataToTransfer2 = TxBufferSize2;
uint8_t           NbrOfDataToRead1     = RxBufferSize1;
uint8_t           NbrOfDataToRead2     = RxBufferSize2;
__IO TestStatus   TransferStatus1      = FAILED;
__IO TestStatus   TransferStatus2      = FAILED;
/* Private function prototypes -----------------------------------------------*/
void       RCC_Configuration(void);
void       GPIO_Configuration(void);
void       NVIC_Configuration(void);
TestStatus Buffercmp(uint8_t *pBuffer1, uint8_t *pBuffer2, uint16_t BufferLength);
uint8_t    index = 0;

/* Private functions ---------------------------------------------------------*/

void main(void)
{
    pll_init();
    sys_io_init();
    uart_init(UART_BOOT_PORT, UART_PARITY_NONE, UART_STOPBITS_1, UART_DATABITS_8, UART_BOOT_BD);

    TxCounter1 = 0x00;
    TxCounter2 = 0x00;
    RxCounter1 = 0x00;
    RxCounter2 = 0x00;

    GPIO_Configuration();

    NVIC_Configuration();

    USART_InitStructure.USART_BaudRate            = 115200;
    USART_InitStructure.USART_WordLength          = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits            = USART_StopBits_1;
    USART_InitStructure.USART_Parity              = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;

    /* Configure USARTy */
    USART_Init(USARTy, &USART_InitStructure);
    /* Configure USARTz */
    USART_Init(USARTz, &USART_InitStructure);

    USART_ITConfig(USARTy, USART_IT_IER_RDI, ENABLE);
    USART_ITConfig(USARTy, USART_IT_IER_THRI, ENABLE);

    USART_ITConfig(USARTz, USART_IT_IER_RDI, ENABLE);
    USART_ITConfig(USARTz, USART_IT_IER_THRI, ENABLE);

    /* Wait until end of transmission from USARTy to USARTz */
    while (RxCounter2 < RxBufferSize2) {
    }

    /* Wait until end of transmission from USARTz to USARTy */
    while (RxCounter1 < RxBufferSize1) {
    }

    /* Check the received data with the send ones */
    TransferStatus1 = Buffercmp(TxBuffer2, RxBuffer1, RxBufferSize1);
    /* TransferStatus1 = PASSED, if the data transmitted from USARTz and
     received by USARTy are the same */
    /* TransferStatus1 = FAILED, if the data transmitted from USARTz and
     received by USARTy are different */
    TransferStatus2 = Buffercmp(TxBuffer1, RxBuffer2, RxBufferSize2);
    /* TransferStatus2 = PASSED, if the data transmitted from USARTy and
     received by USARTz are the same */
    /* TransferStatus2 = FAILED, if the data transmitted from USARTy and
     received by USARTz are different */
    if (TransferStatus1 == PASSED || TransferStatus2 == PASSED) {
        debug("Transfer Interrupt [PASSED]!!!\n");
    } else {
        debug("Transfer Interrupt [FAILED]!!!\n");
    }

    while (1) {
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

    /* Enable the USART2 Pins Software Remapping */
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
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
    NVIC_InitStructure.NVIC_IRQChannel                   = USARTy_IRQn;
    NVIC_Init(&NVIC_InitStructure);

    NVIC_InitStructure.NVIC_IRQChannel = USARTz_IRQn;
    NVIC_Init(&NVIC_InitStructure);

    /* Enable Global Interrupt. */
    __enable_irq();
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