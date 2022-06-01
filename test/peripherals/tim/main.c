/**
 * @file main.c
 * @brief TIM interrupt and pwm output test case.
 * @author Liyouchuang (lycdemeng@163.com)
 * @version 1.0
 * @date 2022-05-18
 * @copyright Copyright (c) 2022 Icore, Inc
 *
 * test_func         : Using TIM3 timer interrupt to control LED light.
 * test_func2        : Using timer interrupt generated PWM to control LED brightness.
 * Board Version     : AiP32rv001_EVB_V01
 * Hardware setup    : test_func2
 * Connect GND to The negative of LED;
 * Connect PE9(TIM1_CH1) to The positive of LED;
 */

#include "at103.h"
#include "at103_it.h"
#include "unity.h"

/* Private variables ---------------------------------------------------------*/
USART_InitTypeDef       USART_InitStructure;
GPIO_InitTypeDef        GPIO_InitStructure;
TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
NVIC_InitTypeDef        NVIC_InitStructure;
TIM_OCInitTypeDef       TIM_OCInitStructure;
TIM_ICInitTypeDef       TIM_ICInitStructure;
TIM_BDTRInitTypeDef     TIM_BDTRInitStructure;

/**
 * @brief TIM3 interrupt test:Using TIM3 timer interrupt to control LED light.
 */
void test_func_1(void)
{
    uint16_t Prescaler            = 0;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    /*initialization pin PE3*/
    GPIO_Init(GPIOE, &GPIO_InitStructure);

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

    TIM_TimeBaseStructure.TIM_Period        = 4999;
    TIM_TimeBaseStructure.TIM_Prescaler     = 7199;
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode   = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

    /* Prescaler configuration */
    TIM_PrescalerConfig(TIM3, 7199, TIM_PSCReloadMode_Immediate);
    Prescaler = TIM_GetPrescaler(TIM3);
    debug("Prescaler = %x\r\n", Prescaler);

    TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);

    NVIC_InitStructure.NVIC_IRQChannel                   = TIM3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    __enable_irq();

    TIM_Cmd(TIM3, ENABLE);
    if ((NVIC_InitStructure.NVIC_IRQChannel != TIM3_IRQn) || (TIM3->TIM_DIER.value != 0x0001)) {

        TEST_ASSERT_MESSAGE(0, "LED not light Tim interrupt test fail");

    } else
        debug("LED light Tim interrupt success!!!\r\n");
}

/**
 * @brief TIM1 PWM Output test:Using timer interrupt generated PWM to control LED brightness.
 * Connect GND to The negative of LED;
 * Connect PE9(TIM1_CH1) to The positive of LED;
 */
void test_func_2(void)
{
    uint16_t pwmvalue;
    uint8_t  dir = 1;
    /* Internal clock to provide the clock source for TIM1 */
    TIM_InternalClockConfig(TIM1);

    /* TIM1 REMAP */
    GPIO_PinRemapConfig(GPIO_FullRemap_TIM1, ENABLE);
    /* GPIO configuration */
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOE, &GPIO_InitStructure);

    /* TIM1, GPIOE clock enable */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1 | RCC_APB2Periph_GPIOE, ENABLE);

    /* Time Base configuration */
    TIM_TimeBaseStructure.TIM_Prescaler     = 0;
    TIM_TimeBaseStructure.TIM_CounterMode   = TIM_CounterMode_Up;
    TIM_TimeBaseStructure.TIM_Period        = 899;
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);

    /* Channel 1 Configuration in PWM mode */
    TIM_OCInitStructure.TIM_OCMode      = TIM_OCMode_PWM2;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_OCPolarity  = TIM_OCPolarity_High;
    TIM_OC1Init(TIM1, &TIM_OCInitStructure);

    /* Enable the preloaded register of TIM1 on CCR1 */
    TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);

    /* Enable the ARR Preload */
    TIM_ARRPreloadConfig(TIM1, ENABLE);

    /* Master Mode selection */
    TIM_SelectOutputTrigger(TIM1, TIM_TRGOSource_Update);

    /* Select the Master Slave Mode */
    TIM_SelectMasterSlaveMode(TIM1, TIM_MasterSlaveMode_Enable);

    TIM_GenerateEvent(TIM1, TIM_EventSource_CC1);

    /* TIM1 counter enable */
    TIM_Cmd(TIM1, ENABLE);

    /* TIM1 PWM Output Enable */
    TIM_CtrlPWMOutputs(TIM1, ENABLE);

    for (pwmvalue = 899; pwmvalue > 0; pwmvalue--) {
        TIM_SetCompare1(TIM1, pwmvalue);
        debug("pwmvalue = %x\r\n", pwmvalue);
    }
    for (pwmvalue = 0; pwmvalue < 899; pwmvalue++) {
        TIM_SetCompare1(TIM1, pwmvalue);
        debug("pwmvalue = %x\r\n", pwmvalue);
    }

    if (TIM1->TIM_CCMR1.value != 0x78 || TIM1->TIM_CCER.value != 0x0001 ||
        TIM1->TIM_CR1.TIM_CR1_CEN != 1 || TIM1->TIM_BDTR.TIM_BDTR_MOE != 1) {

        TEST_ASSERT_MESSAGE(0, "TIM PWM function test fail");

    } else
        debug("LED Light TIM PWM function test pass !!!\r\n");
}

/*****************************************************************************/
void unity_test(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_func_1);
    RUN_TEST(test_func_2);
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