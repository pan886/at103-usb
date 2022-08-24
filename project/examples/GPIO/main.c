/**
 * @file main.c
 * @brief Light up the running lights and get the state for PA0. 
 * @author pantianwen (pantianwen@163.com)
 * @version 1.0
 * @date 2022-03-16
 * @copyright Copyright (c) 2022 Icore, Inc
 */

#include "at103.h"
GPIO_InitTypeDef        GPIOB_struct;
GPIO_InitTypeDef        GPIOA_struct;
TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
EXTI_InitTypeDef        EXTI_InitStructure;
NVIC_InitTypeDef        NVIC_InitStructure;
void                    GPIO_Toggle(GPIO_TypeDef *GPIOx, uint16_t PortVal);

void main(void)
{
    pll_init();
    sys_io_init();
    uart_init(UART_BOOT_PORT, UART_PARITY_NONE, UART_STOPBITS_1, UART_DATABITS_8, UART_BOOT_BD);

    /* NVIC_IRQ configuration */
    NVIC_InitStructure.NVIC_IRQChannel                   = TIM3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    __enable_irq();
    EXTI0_Config();
    /* TIM3 clock enable */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
    /* Time Base configuration */
    TIM_TimeBaseStructure.TIM_Period        = 999;
    TIM_TimeBaseStructure.TIM_Prescaler     = 7199;
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode   = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
    /* Enables TIM3 interrupt */
    TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);

    /* TIM3 counter enable */
    TIM_Cmd(TIM3, ENABLE);

    /*Push-pull output mode */
    GPIOB_struct.GPIO_Mode  = GPIO_Mode_Out_PP;
    GPIOB_struct.GPIO_Pin   = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
    GPIOB_struct.GPIO_Speed = GPIO_Speed_2MHz;

    GPIO_Init(GPIOB, &GPIOB_struct);

    while (1) {
        /* no code here */
    }
}

void GPIO_Toggle(GPIO_TypeDef *GPIOx, uint16_t PortVal)
{
    GPIOx->DATA ^= PortVal;
}

void TIM3_IRQHandler(void)
{
    static uint8_t i = 0;
    if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) {
        /* Clear the interrupt flag bit */
        TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
        i++;
        if (i % 2 == 0)
            GPIO_Toggle(GPIOB, GPIO_Pin_15);

        if (i % 2 == 1)
            GPIO_Toggle(GPIOB, GPIO_Pin_14);
    }
}

void EXTI0_Config(void)
{
    /* Enable GPIOA clock */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    /* Configure PA.00 pin as input floating */
    GPIO_StructInit(&GPIOA_struct);
    GPIOA_struct.GPIO_Pin  = GPIO_Pin_0;
    GPIOA_struct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIOA_struct);

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

void EXTI0_IRQHandler(void)
{
    if (EXTI_GetITStatus(EXTI_Line0) != RESET) {
        debug("[IRQ]EXTI Line0 !!!\r\n");
        GPIO_Toggle(GPIOB, GPIO_Pin_13);
        /* Clear the  EXTI line 0 pending bit */
        EXTI_ClearITPendingBit(EXTI_Line0);
    }
}