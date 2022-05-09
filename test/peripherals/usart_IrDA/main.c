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

typedef enum {
    JOY_NONE  = 0x00,
    JOY_UP    = 0x01,
    JOY_DOWN  = 0x02,
    JOY_LEFT  = 0x04,
    JOY_RIGHT = 0x06,
    JOY_SEL   = 0x07,
} JOYState_TypeDef;

/* Private variables ---------------------------------------------------------*/
USART_InitTypeDef USART_InitStructure;
JOYState_TypeDef  ReceivedData = JOY_NONE;
/* Private function prototypes -----------------------------------------------*/
void GPIO_Configuration(void);
/**
 * @brief uart dma polling memory to Peripheral transfer example.
 */
void test_func(void)
{
    /* Configure the GPIO ports */
    GPIO_Configuration();

    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA, ENABLE);

    USART_InitStructure.USART_BaudRate            = 115200;
    USART_InitStructure.USART_WordLength          = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits            = USART_StopBits_1;
    USART_InitStructure.USART_Parity              = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;

    /* Configure USART1 */
    USART_Init(USART1, &USART_InitStructure);
    /* Configure USARTz */
    USART_Init(USART3, &USART_InitStructure);

    /* Configure the USARTy IrDA mode */
    USART_IrDAConfig(USART1, USART_IrDAMode_Normal);

    /* Enable the USARTy IrDA mode */
    USART_IrDACmd(USART1, ENABLE);
    if (GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_8))
        // USART_SendData(USART1, JOY_NONE);
        ReceivedData = JOY_NONE;
    if (GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_9))
        // USART_SendData(USART1, JOY_UP);
        ReceivedData = JOY_UP;
    if (GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_10))
        // USART_SendData(USART1, JOY_DOWN);
        ReceivedData = JOY_DOWN;
    if (GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_11))
        // USART_SendData(USART1, JOY_LEFT);
        ReceivedData = JOY_LEFT;
    if (GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_12))
        // USART_SendData(USART1, JOY_RIGHT);
        ReceivedData = JOY_RIGHT;
    if (GPIO_ReadOutputDataBit(GPIOE, GPIO_Pin_13) == Bit_SET)
        USART_SendData(USART1, JOY_SEL);
    // ReceivedData = JOY_SEL;

    while (1) {
        /* Wait until a byte is received */
        // while (USART_GetLineFlagStatus(USART3, USART_FLAG_LSR_DR) == RESET) {
        // }
        /* Read the received byte */
        USART_SendData(USART1, JOY_SEL);
        ReceivedData = USART_ReceiveData(USART1);
        debug("ReceivedData= %d\r\n", ReceivedData);
        switch (ReceivedData) {
        case JOY_UP:
            GPIO_ResetBits(GPIOE, GPIO_Pin_2);
            GPIO_ResetBits(GPIOE, GPIO_Pin_3);
            GPIO_ResetBits(GPIOE, GPIO_Pin_4);
            GPIO_ResetBits(GPIOE, GPIO_Pin_5);
            break;
        case JOY_DOWN:
            GPIO_SetBits(GPIOE, GPIO_Pin_3);
            GPIO_ResetBits(GPIOE, GPIO_Pin_2);
            GPIO_ResetBits(GPIOE, GPIO_Pin_4);
            GPIO_ResetBits(GPIOE, GPIO_Pin_5);
            break;
        case JOY_LEFT:
            GPIO_SetBits(GPIOE, GPIO_Pin_4);
            GPIO_ResetBits(GPIOE, GPIO_Pin_2);
            GPIO_ResetBits(GPIOE, GPIO_Pin_3);
            GPIO_ResetBits(GPIOE, GPIO_Pin_5);
            break;
        case JOY_RIGHT:
            GPIO_SetBits(GPIOE, GPIO_Pin_5);
            GPIO_ResetBits(GPIOE, GPIO_Pin_2);
            GPIO_ResetBits(GPIOE, GPIO_Pin_3);
            GPIO_ResetBits(GPIOE, GPIO_Pin_4);
            break;
        case JOY_SEL:
            GPIO_SetBits(GPIOE, GPIO_Pin_2);
            GPIO_SetBits(GPIOE, GPIO_Pin_3);
            GPIO_SetBits(GPIOE, GPIO_Pin_4);
            GPIO_SetBits(GPIOE, GPIO_Pin_5);
            break;
        case JOY_NONE:
            break;
        default:
            break;
        }
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

    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOE, &GPIO_InitStructure);
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