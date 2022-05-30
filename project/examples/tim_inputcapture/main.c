/**
 * @file main.c
 * @brief TIM InputCapture example:Capture the high level pulse width of the time in PD14.
 * Connect PB9 to PD14,;
 * @author Liyouchuang (lycdemeng@163.com)
 * @version 1.0
 * @date 2022-05-18
 * @copyright Copyright (c) 2022 Icore, Inc
 */

#include "at103.h"
#include "at103_it.h"

extern uint16_t TIM4CH3_CAPTURE_STA;
extern uint32_t TIM4CH3_CAPTURE_VAL;

void TIM3_PWM_Init(uint16_t arr, uint16_t psc)
{
    GPIO_InitTypeDef        GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_OCInitTypeDef       TIM_OCInitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_AFIO, ENABLE);

    /* Internal clock to provide the clock source for TIM3 */
    TIM_InternalClockConfig(TIM3);
    /* TIM3 REMAP */
    GPIO_PinRemapConfig(GPIO_FullRemap_TIM3, ENABLE);

    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    TIM_TimeBaseStructure.TIM_Period        = arr;
    TIM_TimeBaseStructure.TIM_Prescaler     = psc;
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode   = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

    TIM_OCInitStructure.TIM_OCMode      = TIM_OCMode_PWM2;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse       = 0;
    TIM_OCInitStructure.TIM_OCPolarity  = TIM_OCPolarity_High;
    TIM_OC2Init(TIM3, &TIM_OCInitStructure);

    TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);

    TIM_ARRPreloadConfig(TIM3, ENABLE);

    TIM_Cmd(TIM3, ENABLE);
}
void delay()
{
    uint16_t i, j;
    for (i = 0; i < 0xFF; i++)
        for (j = 0; j < 0xFFF; j++)
            ;
}
TIM_ICInitTypeDef TIM_ICInitStructure;

void TIM4_Cap_Init(uint16_t arr, uint16_t psc)
{
    GPIO_InitTypeDef        GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    NVIC_InitTypeDef        NVIC_InitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOB, ENABLE);

    /* TIM4 REMAP */
    GPIO_PinRemapConfig(GPIO_Remap_TIM4, ENABLE);

    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_14;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOD, &GPIO_InitStructure);

    TIM_TimeBaseStructure.TIM_Period        = arr;
    TIM_TimeBaseStructure.TIM_Prescaler     = psc;
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode   = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);

    TIM_ICInitStructure.TIM_Channel     = TIM_Channel_3;
    TIM_ICInitStructure.TIM_ICPolarity  = TIM_ICPolarity_Rising;    //CCER
    TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; //CCMR1
    TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;           //CCMR1
    TIM_ICInitStructure.TIM_ICFilter    = 0x0000;                   //CCMR1
    TIM_ICInit(TIM4, &TIM_ICInitStructure);

    NVIC_InitStructure.NVIC_IRQChannel                   = TIM4_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    __enable_irq();

    TIM_ITConfig(TIM4, TIM_IT_Update | TIM_IT_CC3, ENABLE);

    TIM_Cmd(TIM4, ENABLE);
}

void main(void)
{
    pll_init();
    sys_io_init();
    uart_init(UART_BOOT_PORT, UART_PARITY_NONE, UART_STOPBITS_1, UART_DATABITS_8, UART_BOOT_BD);
    uint32_t temp = 0;
    TIM3_PWM_Init(899, 0);
    TIM4_Cap_Init(0xFFFF, 72 - 1);

    while (1) {
        TIM_SetCompare2(TIM3, TIM_GetCapture2(TIM3) + 1);

        if (TIM_GetCapture2(TIM3) == 300)
            TIM_SetCompare2(TIM3, 0);

        GPIO_SetBits(GPIOB, GPIO_Pin_9);
        delay();
        GPIO_ResetBits(GPIOB, GPIO_Pin_9);
        delay();

        if (TIM4CH3_CAPTURE_STA & 0x80) {
            temp = TIM4CH3_CAPTURE_STA & 0x3F;
            temp *= 0xFFFF;
            temp += TIM4CH3_CAPTURE_VAL;
            debug("HIGH:%d us\r\n", temp);
            TIM4CH3_CAPTURE_STA = 0;
        }
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