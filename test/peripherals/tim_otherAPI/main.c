/**
 * @file main.c
 * @brief Tim others API test case.
 * @author liyouchuang (lycdemeng@163.com)
 * @version 1.0
 * @date 2022-05-18
 * @copyright Copyright (c) 2022 Icore, Inc
 *
 * Test function-Test function3    : Test the value of register.
 * Test function4                  : One pulse test.
 * Board Version                   : AiP32rv001_EVB_V01
 * Hardware setup                  : Without.
 */

#include "at103.h"
#include "unity.h"

/* Private variables ---------------------------------------------------------*/
GPIO_InitTypeDef        GPIO_InitStructure;
TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
TIM_OCInitTypeDef       TIM_OCInitStructure;
TIM_ICInitTypeDef       TIM_ICInitStructure;

/**
 * @brief others function
 * 
 */
void test_func_1(void)
{
    TIM_ITRxExternalClockConfig(TIM4, TIM_TS_ITR1);
    debug("ITRxExternal = %x\r\n", TIM4->TIM_SMCR.value);

    TIM_TIxExternalClockConfig(TIM2, TIM_TIxExternalCLK1Source_TI1, TIM_ICPolarity_Rising, 0xF);
    debug("TIxExternal = %x\r\n", TIM2->TIM_SMCR.value);

    /* Configure the ETR Clock source */
    TIM_ETRConfig(TIM3, TIM_ExtTRGPSC_DIV2, TIM_ExtTRGPolarity_Inverted, 0xF);
    TIM_ETRClockMode1Config(TIM3, TIM_ExtTRGPSC_DIV2, TIM_ExtTRGPolarity_Inverted, 0xF);
    debug("ETRClock = %x\r\n", TIM3->TIM_SMCR.value);

    TIM_EncoderInterfaceConfig(TIM1, TIM_EncoderMode_TI1, TIM_ICPolarity_Rising, TIM_ICPolarity_Falling);
    debug("EncoderInterface1 = %x\r\n", TIM1->TIM_SMCR.value);
    debug("EncoderInterface2 = %x\r\n", TIM1->TIM_CCMR1.value);
    debug("EncoderInterface3 = %x\r\n", TIM1->TIM_CCER.value);

    TIM_ForcedOC2Config(TIM4, TIM_ForcedAction_Active);
    debug("ForcedOC2 = %x\r\n", TIM4->TIM_CCMR1.value);

    TIM_OC1FastConfig(TIM4, TIM_OCFast_Enable);
    debug("OC1Fast = %x\r\n", TIM4->TIM_CCMR1.value);

    TIM_ClearOC1Ref(TIM4, TIM_OCClear_Enable);
    debug("ClearOC1 = %x\r\n", TIM4->TIM_CCMR1.value);

    TIM_OC2PolarityConfig(TIM3, TIM_OCPolarity_Low);
    debug("OC2Polarity = %x\r\n", TIM3->TIM_CCER.value);

    TIM_OC2NPolarityConfig(TIM1, TIM_OCNPolarity_Low);
    debug("OC2NPolarity = %x\r\n", TIM1->TIM_CCER.value);

    TIM_SelectOCxM(TIM3, TIM_Channel_2, TIM_OCMode_Active);
    debug("SelectOCxM = %x\r\n", TIM3->TIM_CCER.value);

    if (TIM4->TIM_SMCR.value != 0x17 || TIM2->TIM_SMCR.value != 0x57 || TIM3->TIM_SMCR.value != 0x9f77 ||
        TIM1->TIM_SMCR.value != 0x0001 || TIM1->TIM_CCMR1.value != 0x0101 || TIM4->TIM_CCMR1.value != 0x51f4 ||
        TIM1->TIM_CCER.value != 0x00a0 || TIM3->TIM_CCER.value != 0x0020) {

        TEST_ASSERT_MESSAGE(0, "Tim test_func_1 test fail");

    } else
        debug("Tim test_func_1 test success!!!\r\n");
}

/**
 * @brief TIM_CR1 test
 */
void test_func_2(void)
{
    uint16_t count = 0;
    TIM_UpdateDisableConfig(TIM4, ENABLE);
    debug("UpdateDisable = %x\r\n", TIM4->TIM_CR1.TIM_CR1_UDIS);

    TIM_UpdateRequestConfig(TIM4, TIM_UpdateSource_Regular);
    debug("UpdateRequest = %x\r\n", TIM4->TIM_CR1.TIM_CR1_URS);

    TIM_SetClockDivision(TIM3, TIM_CKD_DIV2);
    debug("ClockDivision = %x\r\n", TIM3->TIM_CR1.value);

    TIM_CounterModeConfig(TIM2, TIM_CounterMode_Down);
    debug("CounterMode = %x\r\n", TIM2->TIM_CR1.value);

    TIM_SetCounter(TIM2, 899);
    count = TIM_GetCounter(TIM2);
    debug("Counter = %x\r\n", count);

    TIM_SetAutoreload(TIM2, 0x55);
    debug("Autoreload = %x\r\n", TIM2->TIM_ARR.value);

    if (TIM4->TIM_CR1.TIM_CR1_UDIS != 0x0001 || TIM4->TIM_CR1.TIM_CR1_URS != 0x0001 || TIM3->TIM_CR1.value != 0x0100 ||
        TIM2->TIM_CR1.value != 0x0010 || count != 0x0383 || TIM2->TIM_ARR.value != 0x0055) {

        TEST_ASSERT_MESSAGE(0, "Tim test_func_2 test fail");

    } else
        debug("Tim test_func_2 test success!!!\r\n");
}

/**
 * @brief TIM_CR2 test
 */
void test_func_3(void)
{
    TIM_SelectCOM(TIM1, ENABLE);
    debug("SelectCOM = %x\r\n", TIM1->TIM_CR2.TIM_CR2_CCUS);

    TIM_SelectCCDMA(TIM1, ENABLE);
    debug("SelectCCDMA = %x\r\n", TIM1->TIM_CR2.TIM_CR2_CCDS);

    TIM_SelectHallSensor(TIM4, TIM_UpdateSource_Regular);
    debug("HallSensor = %x\r\n", TIM4->TIM_CR2.TIM_CR2_TI1S);

    TIM_CCPreloadControl(TIM1, ENABLE);
    debug("CCPreload = %x\r\n", TIM1->TIM_CR2.TIM_CR2_CCPC);

    if (TIM_GetFlagStatus(TIM3, TIM_FLAG_Update) != RESET) {
        TIM_ClearFlag(TIM3, TIM_FLAG_Update);
        debug("Tim GetFlagStatus success!!!\r\n");
    } else {
        TEST_ASSERT_MESSAGE(TIM_GetFlagStatus(TIM3, TIM_FLAG_Update) == RESET, "Tim test_func_3 test fail");
    }

    if (TIM1->TIM_CR2.TIM_CR2_CCUS != 0x0001 || TIM1->TIM_CR2.TIM_CR2_CCDS != 0x0001 ||
        TIM4->TIM_CR2.TIM_CR2_TI1S != 0x0001 || TIM1->TIM_CR2.TIM_CR2_CCPC != 0x0001) {

        TEST_ASSERT_MESSAGE(0, "Tim test_func_3 test fail");

    } else
        debug("Tim test_func_3 test success!!!\r\n");
}

/**
 * @brief One pulse test.
 */
void test_func_4(void)
{
    uint16_t PrescalerValue = 0;
    uint16_t Timer_Period   = 65535;
    uint16_t Timer_pulse    = 16383;
    /* TIM4 clock enable */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);

    /* GPIOB clock enable */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);

    /* TIM4_CH1\CH2 pin configuration */
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_12 | GPIO_Pin_13;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOD, &GPIO_InitStructure);

    PrescalerValue = (uint16_t)(72000000 / 24000000) - 1;
    /* Time base configuration */
    TIM_TimeBaseStructure.TIM_Period        = Timer_Period;
    TIM_TimeBaseStructure.TIM_Prescaler     = PrescalerValue;
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode   = TIM_CounterMode_Up;

    TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);

    /* TIM4 PWM2 Mode configuration: Channel1 */
    TIM_OCInitStructure.TIM_OCMode      = TIM_OCMode_PWM2;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse       = Timer_pulse;
    TIM_OCInitStructure.TIM_OCPolarity  = TIM_OCPolarity_High;

    TIM_OC1Init(TIM4, &TIM_OCInitStructure);

    /* TIM4 configuration in Input Capture Mode */

    TIM_ICStructInit(&TIM_ICInitStructure);

    TIM_ICInitStructure.TIM_Channel     = TIM_Channel_2;
    TIM_ICInitStructure.TIM_ICPolarity  = TIM_ICPolarity_Rising;
    TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
    TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
    TIM_ICInitStructure.TIM_ICFilter    = 0;

    TIM_ICInit(TIM4, &TIM_ICInitStructure);

    /* One Pulse Mode selection */
    TIM_SelectOnePulseMode(TIM4, TIM_OPMode_Single);

    /* Input Trigger selection */
    TIM_SelectInputTrigger(TIM4, TIM_TS_TI2FP2);

    /* Slave Mode selection: Trigger Mode */
    TIM_SelectSlaveMode(TIM4, TIM_SlaveMode_Trigger);
    debug("one pulse value:%d us\r\n", (Timer_Period - Timer_pulse) / 24);

    if (TIM4->TIM_CCMR1.value != 0x01f4 || TIM4->TIM_CCER.value != 0x0011 || TIM4->TIM_CCR1.value != 0x3fff) {

        TEST_ASSERT_MESSAGE(0, "Tim test_func_4 test fail");

    } else
        debug("Tim test_func_4 test success!!!\r\n");
}

/*****************************************************************************/
void unity_test(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_func_1);
    RUN_TEST(test_func_2);
    RUN_TEST(test_func_3);
    RUN_TEST(test_func_4);
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