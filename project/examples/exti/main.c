/**
 * @file main.c
 * @brief Exti example.
 * @author Dong Qin (dong.qin@timesintelli.com)
 * @version 1.0
 * @date 2022-03-14
 * @copyright Copyright (c) 2022 Timesintelli, Inc
 * The rising edge of PA1 triggers the PA0 interrupt.
 * Please connect PA0 to PA1.
 */

#include "at103.h"

EXTI_InitTypeDef EXTI_InitStructure;
GPIO_InitTypeDef GPIO_InitStructure;
NVIC_InitTypeDef NVIC_InitStructure;

void EXTI0_Config(void);
void PA1_Config(void);

void main(void)
{
    pll_init();
    sys_io_init();
    uart_init(UART_BOOT_PORT, UART_PARITY_NONE, UART_STOPBITS_1, UART_DATABITS_8, UART_BOOT_BD);
    __enable_irq();

    debug("EXTI example !!!\r\n");
    debug("Please connect PA0 to PA1 !!!\r\n");
    /* Configure PA.01 in output mode */
    PA1_Config();
    GPIO_ResetBits(GPIOA, GPIO_Pin_1);

    /* Configure PA.00 in interrupt mode */
    EXTI0_Config();

    /* PA1 0->1 : Rising edge triggers interrupt */
    GPIO_SetBits(GPIOA, GPIO_Pin_1);

    NopDelay(1000);
    /* PA1 1->0 : Falling edges don't trigger interrupt */
    GPIO_ResetBits(GPIOA, GPIO_Pin_1);

    while (1) {
    }
}

void PA1_Config(void)
{
    /* Enable GPIOA clock */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    /* Configure PA.01 pin as output PP */
    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
}

void EXTI0_Config(void)
{
    /* Enable GPIOA clock */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    /* Configure PA.00 pin as input floating */
    GPIO_StructInit(&GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    /* Enable AFIO clock */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);

    /* Connect EXTI0 Line to PA.00 pin */
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource0);

    /* Configure EXTI0 line */
    EXTI_StructInit(&EXTI_InitStructure);
    EXTI_InitStructure.EXTI_Line    = EXTI_Line0;
    EXTI_InitStructure.EXTI_Mode    = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

    /* Enable and set EXTI0 Interrupt to the lowest priority */
    NVIC_InitStructure.NVIC_IRQChannel                   = EXTI0_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}