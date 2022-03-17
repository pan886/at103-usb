/**
 * @file main.c
 * @brief uart module poll test.
 * This example provides a basic communication between USARTy and USARTz using flags.
 * First, the USARTy sends TxBuffer to USARTz. The USARTz reads the received data and
 * store it into RxBuffer.The received data is then compared with the send ones and
 * the result of this comparison is stored in the "TransferStatus" variable.
 *
 * Connect USART1 Tx pin (PA.09) to USART3 Rx pin (PD.9).
 * Connect USART1 Rx pin (PA.10) to USART3 Tx pin (PD.8).
 * PA.10 and PA.09 are on jumper J90
 *
 * @author zhangsheng (zhangsheng@zhangsheng.ic.com)
 * @version 1.0
 * @date 2022-02-08
 * @copyright Copyright (c) 2022 Icore, Inc
 */

#include "at103.h"

/* Private typedef -----------------------------------------------------------*/
typedef enum { FAILED = 0,
               PASSED = !FAILED } TestStatus;

/* Private define ------------------------------------------------------------*/
#define TxBufferSize (countof(TxBuffer))
/* Private macro -------------------------------------------------------------*/
#define countof(a) (sizeof(a) / sizeof(*(a)))

/* Private variables ---------------------------------------------------------*/
USART_InitTypeDef   USART_InitStructure;
uint8_t             TxBuffer[] = "Buffer Send from USARTy to USARTz using Flags";
uint8_t             RxBuffer[TxBufferSize];
uint8_t             TxCounter = 0, RxCounter = 0;
volatile TestStatus TransferStatus = FAILED;

/* Private function prototypes -----------------------------------------------*/
void       RCC_Configuration(void);
void       GPIO_Configuration(void);
TestStatus Buffercmp(uint8_t *pBuffer1, uint8_t *pBuffer2, uint16_t BufferLength);
uint8_t    index = 0;

/* Private functions ---------------------------------------------------------*/

void main(void)
{
    pll_init();
    sys_io_init();
    uart_init(UART_BOOT_PORT, UART_PARITY_NONE, UART_STOPBITS_1, UART_DATABITS_8, UART_BOOT_BD);

    TxCounter = 0;
    RxCounter = 0;
    GPIO_Configuration();
    USART_InitStructure.USART_BaudRate            = 115200;
    USART_InitStructure.USART_WordLength          = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits            = USART_StopBits_1;
    USART_InitStructure.USART_Parity              = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;

    /* Configure USARTy */
    USART_Init(USARTy, &USART_InitStructure);

    /* Configure USARTz */
    USART_Init(USARTz, &USART_InitStructure);

    while (TxCounter < TxBufferSize) {
        /* Send one byte from USARTy to USARTz */
        USART_SendData(USARTy, TxBuffer[TxCounter++]);

        /* Loop until USARTy DR register is empty */
        while (USART_GetLineFlagStatus(USARTy, USART_FLAG_LSR_THRE) == RESET) {
        }

        /* Loop until the USARTz Receive Data Register is not empty */
        while (USART_GetLineFlagStatus(USARTz, USART_FLAG_LSR_DR) == RESET) {
        }

        /* Store the received byte in RxBuffer */
        RxBuffer[RxCounter++] = (USART_ReceiveData(USARTz) & 0x7F);
    }
    /* Check the received data with the send ones */
    TransferStatus = Buffercmp(TxBuffer, RxBuffer, TxBufferSize);

    if (TransferStatus) {
        debug("TransferStatus = [PASSED]!!!\n");
    } else {
        debug("TransferStatus = [FAILED]!!!\n");
    }
    while (1) {
    }
}

/**
 * @brief Config USART1&USART2 GPIO
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