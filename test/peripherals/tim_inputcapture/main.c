/**
 * @file main.c
 * @brief TIM3 InputCapture test case.
 * @author Liyouchuang (lycdemeng@163.com)
 * @version 1.0
 * @date 2022-04-17
 * @copyright Copyright (c) 2022 Icore, Inc
 *
 * Test function    : Capture the high level of time.
 * Board Version    ：AiP32rv001_EVB_V01
 * Hardware setup   : Connect PB9 to PC6 to generate high level.
 */

#include "at103.h"
#include "at103_it.h"
#include "unity.h"

extern uint16_t TIM3CH1_CAPTURE_STA;
extern uint32_t TIM3CH1_CAPTURE_VAL;

void delay()
{
    uint16_t i, j;
    for (i = 0; i < 0xFF; i++)
        for (j = 0; j < 0xFFF; j++)
            ;
}

void TIM1_PWM_Init(uint16_t arr, uint16_t psc)
{
    GPIO_InitTypeDef        GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_OCInitTypeDef       TIM_OCInitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE | RCC_APB2Periph_AFIO, ENABLE);

    /* Internal clock to provide the clock source for TIM1 */
    TIM_InternalClockConfig(TIM1);
    /* TIM1 REMAP */
    GPIO_PinRemapConfig(GPIO_FullRemap_TIM1, ENABLE);

    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOE, &GPIO_InitStructure);

    TIM_TimeBaseStructure.TIM_Period        = arr;
    TIM_TimeBaseStructure.TIM_Prescaler     = psc;
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode   = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);

    TIM_OCInitStructure.TIM_OCMode      = TIM_OCMode_PWM2;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse       = 0;
    TIM_OCInitStructure.TIM_OCPolarity  = TIM_OCPolarity_High;
    TIM_OC2Init(TIM1, &TIM_OCInitStructure);

    TIM_OC2PreloadConfig(TIM1, TIM_OCPreload_Enable);

    TIM_ARRPreloadConfig(TIM1, ENABLE);

    TIM_CtrlPWMOutputs(TIM1, ENABLE);
    TIM_Cmd(TIM1, ENABLE);
}

TIM_ICInitTypeDef TIM_ICInitStructure;
NVIC_InitTypeDef  NVIC_InitStructure;

void TIM3_Cap_Init(uint16_t arr, uint16_t psc)
{
    GPIO_InitTypeDef        GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOB, ENABLE);

    /* TIM4 REMAP */
    GPIO_PinRemapConfig(GPIO_FullRemap_TIM3, ENABLE);

    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
    /* Used PB9 to simulate output high level */
    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    TIM_TimeBaseStructure.TIM_Period        = arr;
    TIM_TimeBaseStructure.TIM_Prescaler     = psc;
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode   = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

    TIM_ICInitStructure.TIM_Channel     = TIM_Channel_1;
    TIM_ICInitStructure.TIM_ICPolarity  = TIM_ICPolarity_Rising;    //CCER
    TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; //CCMR1
    TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;           //CCMR1
    TIM_ICInitStructure.TIM_ICFilter    = 0x0000;                   //CCMR1
    TIM_ICInit(TIM3, &TIM_ICInitStructure);

    NVIC_InitStructure.NVIC_IRQChannel                   = TIM3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    __enable_irq();

    TIM_ITConfig(TIM3, TIM_IT_Update | TIM_IT_CC1, ENABLE);

    TIM_Cmd(TIM3, ENABLE);
}

/**
 * @brief TIM3 InputCapture test:Capture the high level of time
 *
 */
void test_func(void)
{
    uint32_t temp = 0;
    TIM1_PWM_Init(899, 0);
    TIM3_Cap_Init(0xFFFF, 72 - 1);

    TIM_SetCompare2(TIM1, TIM_GetCapture2(TIM1) + 1);

    if (TIM_GetCapture2(TIM1) == 300)
        TIM_SetCompare2(TIM1, 0);

    /* output high and low level */
    GPIO_SetBits(GPIOB, GPIO_Pin_9);
    delay();
    GPIO_ResetBits(GPIOB, GPIO_Pin_9);
    delay();

    /* Capture high level  */
    if (TIM3CH1_CAPTURE_STA & 0x80) {
        temp = TIM3CH1_CAPTURE_STA & 0x3F;
        temp *= 0xFFFF;
        temp += TIM3CH1_CAPTURE_VAL;
        debug("HIGH:%d us\r\n", temp);
        TIM3CH1_CAPTURE_STA = 0;
    }

    if (NVIC_InitStructure.NVIC_IRQChannel != TIM3_IRQn || TIM_ICInitStructure.TIM_ICSelection != TIM_ICSelection_DirectTI ||
        TIM3->TIM_CCER.value != 0x0001 || TIM3->TIM_CR1.TIM_CR1_CEN != 1) {

        TEST_ASSERT_MESSAGE(0, "Tim inputcapture test fail");

    } else
        debug("Tim inputcapture test success!!!\r\n");
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
    debug("tim controller test start!\r\n");
    unity_test();
    while (1) {
        /* test end */
    }
}