/**
 * @file main.c
 * @brief TIM1 Complementary Signals test.
 * @author Liyouchuang (lycdemeng@163.com)
 * @version 1.0
 * @date 2022-05-18
 * @copyright Copyright (c) 2022 Icore, Inc
 *
 * Test function    : This test shows how to configure the TIM1 peripheral to generate three complementary TIM1 signals.
 * Board Version    : AiP32rv001_EVB_V01.
 * Hardware setup   : Without.
 */

#include "at103.h"
#include "unity.h"

/* Private variables ---------------------------------------------------------*/
TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
TIM_OCInitTypeDef       TIM_OCInitStructure;
USART_InitTypeDef       USART_InitStructure;
GPIO_InitTypeDef        GPIO_InitStructure;
TIM_BDTRInitTypeDef     TIM_BDTRInitStructure;
uint16_t                TimerPeriod   = 0;
uint16_t                Channel1Pulse = 0, Channel2Pulse = 0, Channel3Pulse = 0;

/**
 * @brief TIM1 Complementary Signals test.
 *
 */
void test_func(void)
{
    /* Internal clock to provide the clock source for TIM1 */
    TIM_InternalClockConfig(TIM1);

    /* TIM1 REMAP */
    GPIO_PinRemapConfig(GPIO_FullRemap_TIM1, ENABLE);
    /* GPIO configuration CH1\CH1N\CH2\CH2N\CH3\CH3N*/
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_9 | GPIO_Pin_8 | GPIO_Pin_11 | GPIO_Pin_10 | GPIO_Pin_13 | GPIO_Pin_12;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOE, &GPIO_InitStructure);
    /* GPIO configuration BKIN*/
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN_FLOATING;
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_15;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOE, &GPIO_InitStructure);

    /* TIM1, GPIOE clock enable */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1 | RCC_APB2Periph_GPIOE | RCC_APB2Periph_AFIO, ENABLE);

    /* Compute the value to be set in ARR register to generate signal frequency at 17.57 Khz */
    TimerPeriod = (72000000 / 17570) - 1;
    /* Compute CCR1 value to generate a duty cycle at 50% for channel 1 */
    Channel1Pulse = (uint16_t)(((uint32_t)5 * (TimerPeriod - 1)) / 10);
    /* Compute CCR2 value to generate a duty cycle at 25%  for channel 2 */
    Channel2Pulse = (uint16_t)(((uint32_t)25 * (TimerPeriod - 1)) / 100);
    /* Compute CCR3 value to generate a duty cycle at 12.5%  for channel 3 */
    Channel3Pulse = (uint16_t)(((uint32_t)125 * (TimerPeriod - 1)) / 1000);

    /* Time Base configuration */
    TIM_TimeBaseStructure.TIM_Prescaler         = 0;
    TIM_TimeBaseStructure.TIM_CounterMode       = TIM_CounterMode_Up;
    TIM_TimeBaseStructure.TIM_Period            = TimerPeriod;
    TIM_TimeBaseStructure.TIM_ClockDivision     = 0;
    TIM_TimeBaseStructure.TIM_RepetitionCounter = 0x00;
    TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);

    /* Channel 1 Configuration in PWM mode */
    TIM_OCInitStructure.TIM_OCMode       = TIM_OCMode_PWM2;
    TIM_OCInitStructure.TIM_Pulse        = Channel1Pulse;
    TIM_OCInitStructure.TIM_OCPolarity   = TIM_OCPolarity_High;
    TIM_OCInitStructure.TIM_OCNPolarity  = TIM_OCPolarity_High;
    TIM_OCInitStructure.TIM_OutputState  = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
    TIM_OCInitStructure.TIM_OCIdleState  = TIM_OCIdleState_Reset;
    TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Reset;
    TIM_OC1Init(TIM1, &TIM_OCInitStructure);

    /* CH2  */
    TIM_OCInitStructure.TIM_Pulse = Channel2Pulse;
    TIM_OC2Init(TIM1, &TIM_OCInitStructure);

    /* CH3  */
    TIM_OCInitStructure.TIM_Pulse = Channel3Pulse;
    TIM_OC3Init(TIM1, &TIM_OCInitStructure);

    /* BDTR configuration */
    TIM_BDTRInitStructure.TIM_OSSRState       = TIM_OSSRState_Enable;
    TIM_BDTRInitStructure.TIM_OSSIState       = TIM_OSSIState_Enable;
    TIM_BDTRInitStructure.TIM_LOCKLevel       = TIM_LOCKLevel_1;
    TIM_BDTRInitStructure.TIM_DeadTime        = 11;
    TIM_BDTRInitStructure.TIM_Break           = TIM_Break_Enable;
    TIM_BDTRInitStructure.TIM_BreakPolarity   = TIM_BreakPolarity_High;
    TIM_BDTRInitStructure.TIM_AutomaticOutput = TIM_AutomaticOutput_Enable;
    TIM_BDTRConfig(TIM1, &TIM_BDTRInitStructure);

    /* Enable the preloaded register of TIM1 on CCR1\CCR2\CCR3 */
    TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);
    TIM_OC2PreloadConfig(TIM1, TIM_OCPreload_Enable);
    TIM_OC3PreloadConfig(TIM1, TIM_OCPreload_Enable);
    /* Enable the ARR Preload */
    TIM_ARRPreloadConfig(TIM1, ENABLE);

    /* TIM1 counter enable */
    TIM_Cmd(TIM1, ENABLE);

    /* TIM1 PWM Output Enable */
    TIM_CtrlPWMOutputs(TIM1, ENABLE);

    debug("TIM_Period  = %x\r\n", TimerPeriod);
    debug("Channel1Pulse  = %x\r\n", Channel1Pulse);
    debug("Channel2Pulse  = %x\r\n", Channel2Pulse);
    debug("Channel3Pulse  = %x\r\n", Channel3Pulse);

    debug("22233 = %x\r\n", TIM1->TIM_CCER.value);
    if (TIM1->TIM_CCMR1.value != 0x7878 || TIM1->TIM_CCER.value != 0x0555 ||
        TIM1->TIM_CR1.TIM_CR1_CEN != 1 || TIM1->TIM_BDTR.TIM_BDTR_MOE != 1) {

        TEST_ASSERT_MESSAGE(0, "TIM complementary function test fail");

    } else
        debug("TIM complementary function test pass !!!\r\n");
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