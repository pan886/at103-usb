/**
 * @file main.c
 * @brief Usart controller test case.
 * @author liyouchuang (lycdemeng@163.com)
 * @version 1.0
 * @date 2022-03-14
 * @copyright Copyright (c) 2022 Icore, Inc 
 * Board Version    ：AiP32rv001_EVB_V01
 * Hardware setup   : Connect USART1 Tx pin (PA.09) to USART3 Rx pin (PD.9).
 *                    Connect USART1 Rx pin (PA.10) to USART3 Tx pin (PD.8).
 */

#include "at103.h"
#include "at103_it.h"
#include "unity.h"

/* Private typedef -----------------------------------------------------------*/
typedef enum { FAILED = 0,
               PASSED = !FAILED } TestStatus;

/* Private define ------------------------------------------------------------*/
#define TxBufferSize (countof(TxBuffer))

#define TxBufferSize1 (countof(TxBuffer1) - 1)
#define TxBufferSize2 (countof(TxBuffer2) - 1)
#define RxBufferSize1 TxBufferSize2
#define RxBufferSize2 TxBufferSize1

/* Private macro -------------------------------------------------------------*/
#define countof(a) (sizeof(a) / sizeof(*(a))) /*Count the number of members in the array*/

/* Private variables ---------------------------------------------------------*/
USART_InitTypeDef   USART_InitStructure;
uint8_t             TxBuffer[] = "Buffer Send from USARTy to USARTz using Flags";
uint8_t             RxBuffer[TxBufferSize];
uint8_t             TxCounter = 0, RxCounter = 0;
volatile TestStatus TransferStatus = FAILED;

uint8_t         TxBuffer1[] = "USART Interrupt Example: USARTy -> USARTz using Interrupt";
uint8_t         TxBuffer2[] = "USART Interrupt Example: USARTz -> USARTy using Interrupt";
uint8_t         RxBuffer1[RxBufferSize1];
uint8_t         RxBuffer2[RxBufferSize2];
__IO uint8_t    TxCounter1           = 0x00;
__IO uint8_t    TxCounter2           = 0x00;
__IO uint8_t    RxCounter1           = 0x00;
__IO uint8_t    RxCounter2           = 0x00;
uint8_t         NbrOfDataToTransfer1 = TxBufferSize1;
uint8_t         NbrOfDataToTransfer2 = TxBufferSize2;
uint8_t         NbrOfDataToRead1     = RxBufferSize1;
uint8_t         NbrOfDataToRead2     = RxBufferSize2;
__IO TestStatus TransferStatus1      = FAILED;
__IO TestStatus TransferStatus2      = FAILED;

/* Private function prototypes -----------------------------------------------*/
void       GPIO_Configuration(void);
void       NVIC_Configuration(void);
TestStatus Buffercmp(uint8_t *pBuffer1, uint8_t *pBuffer2, uint16_t BufferLength);

/*Configure  USART parameter*/
void USARTparameter(void)
{
    USART_InitStructure.USART_BaudRate            = 115200;
    USART_InitStructure.USART_WordLength          = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits            = USART_StopBits_1;
    USART_InitStructure.USART_Parity              = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
}

/**
 * @brief uart module poll test.
 * This example provides a basic communication between USARTy and USARTz using flags.
 * First, the USARTy sends TxBuffer to USARTz. The USARTz reads the received data and
 * store it into RxBuffer.The received data is then compared with the send ones and 
 * the result of this comparison is stored in the "TransferStatus" variable.
 */
void test_func(void)
{
    TxCounter = 0;
    RxCounter = 0;
    GPIO_Configuration();

    /*Configure  USART parameter*/
    USARTparameter();

    /* Configure USART1 */
    USART_Init(USART1, &USART_InitStructure);
    /* Configure USART2 */
    USART_Init(USART3, &USART_InitStructure);

    while (TxCounter < TxBufferSize) {
        /* Send one byte from USARTy to USARTz */
        USART_SendData(USART1, TxBuffer[TxCounter++]);

        /* Loop until USARTy DR register is empty */
        while (USART_GetLineFlagStatus(USART1, USART_FLAG_LSR_THRE) == RESET) {
        }

        /* Loop until the USARTz Receive Data Register is not empty */
        while (USART_GetLineFlagStatus(USART3, USART_FLAG_LSR_DR) == RESET) {
        }
        /* Store the received byte in RxBuffer */
        RxBuffer[RxCounter++] = (USART_ReceiveData(USART3) & 0x7F);
    }

    /* Check the received data with the send ones */
    TransferStatus = Buffercmp(TxBuffer, RxBuffer, TxBufferSize);

    if (TransferStatus == PASSED) {
        debug("USART1 To USART3 TransferStatus = [PASSED]!!!\n"); /* TransferStatus = PASSED, if the data transmitted from USART1 and  
                                                                     received by USART3 are the same */
    } else {
        TEST_ASSERT_MESSAGE(0, "USART1 To USART3 TransferStatus = [FAILED]!!!"); /* TransferStatus = FAILED, if the data transmitted from USART1 and 
                                                                     received by USART3 are different */
    }
}

/**
 * @brief uart module interrupt test
 * This example provides a basic communication between USARTy and USARTz using
 * interrupts.USARTz sends TxBuffer2 to USARTy which sends TxBuffer1 to USARTz.
 * The data received by USARTy and USARTz are stored respectively in RxBuffer1
 * and RxBuffer2. The data transfer is managed in USARTy_IRQHandler and
 * USARTz_IRQHandler in at103_it.c file.
 */
void test_func2(void)
{
    TxCounter1 = 0x00;
    TxCounter2 = 0x00;
    RxCounter1 = 0x00;
    RxCounter2 = 0x00;

    GPIO_Configuration();
    // RCC_Configuration();
    NVIC_Configuration();

    /*Configure  USART parameter*/
    USARTparameter();

    /* Configure USART1 */
    USART_Init(USART1, &USART_InitStructure);
    /* Configure USART2 */
    USART_Init(USART3, &USART_InitStructure);
    /*Configure  USART1 interrupts*/
    USART_ITConfig(USART1, USART_IT_IER_RDI, ENABLE);  /*Receiver data interrupt*/
    USART_ITConfig(USART1, USART_IT_IER_THRI, ENABLE); /*Transmitter holding register int.*/
    /*Configure  USART3 interrupts*/
    USART_ITConfig(USART3, USART_IT_IER_RDI, ENABLE);
    USART_ITConfig(USART3, USART_IT_IER_THRI, ENABLE);

    /* Wait until end of transmission from USART1 to USART3 */
    while (RxCounter2 < RxBufferSize2) {
    }
    /* Wait until end of transmission from USART3 to USART1 */
    while (RxCounter1 < RxBufferSize1) {
    }

    /* Check the received data with the send ones */
    TransferStatus1 = Buffercmp(TxBuffer2, RxBuffer1, RxBufferSize1);
    /* TransferStatus1 = PASSED, if the data transmitted from USART3 and
     received by USART1 are the same */
    /* TransferStatus1 = FAILED, if the data transmitted from USART3 and
     received by USART1 are different */
    TransferStatus2 = Buffercmp(TxBuffer1, RxBuffer2, RxBufferSize2);
    /* TransferStatus2 = PASSED, if the data transmitted from USART1 and
     received by USART3 are the same */
    /* TransferStatus2 = FAILED, if the data transmitted from USART1 and
     received by USART3 are different */

    if (TransferStatus1 == PASSED || TransferStatus2 == PASSED) {
        debug("Transfer Interrupt [PASSED]!!!\n");
    } else {
        TEST_ASSERT_MESSAGE(0, "Interrupt controller function test fail !!!");
    }
}

/**
 * @brief test USART_GetITStatus 
 */
void test_func3(void)
{
    /* receive IT */
    if (USART_GetITStatus(USART1, USART_IT_IER_RDI) != RESET) { /*receive IT*/
        debug("USART_GetITStatus success!!!\n");
    }
}

/**
 * @brief test other functions
 */
void test_func4(void)
{
    /* Sets the address of the USART1*/
    USART_SetAddress(USART1, 0x50);
    debug("USART_Address = %x\r\n", USART1->TAR.ADRESS);
    // USART_SendBreak(USART1); /*Set USART1 break control*/
    // debug("USART_SendBreak = %x\r\n", USART1->LCR.BREAK);

    /* DISABLE IrDA */
    USART_IrDACmd(USART1, DISABLE);
    debug("USART_IrDACmd = %x\r\n", USART1->MCR.SIRE);
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

/*****************************************************************************/
void unity_test(void)
{
    UNITY_BEGIN();

    RUN_TEST(test_func);
    RUN_TEST(test_func2);
    RUN_TEST(test_func3);
    RUN_TEST(test_func4);

    UNITY_END();
}

void main(void)
{
    // USART_DeInit(USART1);  /*default initialize*/
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