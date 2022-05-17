/**
 * @file main.c
 * @brief The ADC Test case.
 * @author pantianwen (pantianwen@163.com)
 * @version 1.0
 * @date 2022-05-18
 * @copyright Copyright (c) 2022 Icore, Inc
 */

#include "at103.h"
#include "unity.h"

void test_func()
{
    ADC_InitTypeDef ADC_InitStructure;
    uint32_t        adc_clock_cfg       = 0;
    uint32_t        adc_enable          = 0;
    uint32_t        ResetCalibration    = 0;
    uint32_t        start_cmd           = 0;
    uint32_t        StartCalibration    = 0;
    uint32_t        adc_init_value      = 0;
    uint32_t        InjectedDiscModeCmd = 0;
    uint32_t        Disc_Mode_Cmd       = 0;
    uint32_t        ExternalTrigConvCmd = 0;
    uint32_t        DMACmd              = 0;
    uint32_t        FLAG_AWD            = 0;
    uint32_t        IT_EOC              = 0;
    uint32_t        AnalogWatchdog      = 0;
    uint32_t        AnalogWatchdogCmd   = 0;
    ADC_DeInit(ADC1);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_ADC1, ENABLE);
    RCC_ADCCLKConfig(RCC_HCLK_Div16);
    adc_clock_cfg = RCC->CLK_DIV_CFG.ADC_CLK_DIV_BIT;
    TEST_ASSERT_MESSAGE(adc_clock_cfg == RCC_HCLK_Div16, "ADC clock config test fail!");

    /* ADC1 configuration */
    ADC_InitStructure.ADC_Mode               = ADC_Mode_Independent;
    ADC_InitStructure.ADC_ScanConvMode       = DISABLE;
    ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
    ADC_InitStructure.ADC_ExternalTrigConv   = ADC_ExternalTrigConv_None;
    ADC_InitStructure.ADC_DataAlign          = ADC_DataAlign_Right;
    ADC_InitStructure.ADC_NbrOfChannel       = 1;

    ADC_Init(ADC1, &ADC_InitStructure);
    adc_init_value = ADC1->CR2.value & (uint32_t)(ADC_DataAlign_Right | ADC_ExternalTrigConv_None |
                                                  ((uint32_t)ENABLE << 1));
    TEST_ASSERT_MESSAGE(adc_init_value == (ADC_DataAlign_Right | ADC_ExternalTrigConv_None |
                                           ENABLE << 1),
                        "ADC init test fail!");

    ADC_Cmd(ADC1, DISABLE);
    adc_enable = ADC1->CR2.ADON;
    TEST_ASSERT_MESSAGE(adc_enable == 0, "ADC disable test fail!");
    ADC_Cmd(ADC1, ENABLE);
    adc_enable = ADC1->CR2.ADON;
    TEST_ASSERT_MESSAGE(adc_enable == 1, "ADC enable test fail!");

    /* Enable ADC1 reset calibration register */
    ADC_ResetCalibration(ADC1);
    ResetCalibration = ADC1->CR2.RSTCAL;
    TEST_ASSERT_MESSAGE(ResetCalibration == 0x1, "ADC reset calibration test fail!");
    /* Check the end of ADC1 reset calibration register */
    ADC_GetResetCalibrationStatus(ADC1);

    /* Start ADC1 calibration */
    ADC_StartCalibration(ADC1);
    StartCalibration = ADC1->CR2.CAL;
    TEST_ASSERT_MESSAGE(StartCalibration == 0x1, "ADC start Calibration test fail!");
    debug("ADC function test pass !!!\r\n");

    ADC_InjectedDiscModeCmd(ADC1, ENABLE);
    InjectedDiscModeCmd = ADC1->CR1.JDISCEN;
    TEST_ASSERT_MESSAGE(InjectedDiscModeCmd == 0x1, "ADC Inject Enables the discontinuous mode  test fail!");
    ADC_InjectedDiscModeCmd(ADC1, DISABLE);
    InjectedDiscModeCmd = ADC1->CR1.JDISCEN;
    TEST_ASSERT_MESSAGE(InjectedDiscModeCmd == 0x0, "ADC Inject Disables the discontinuous mode  test fail!");

    ADC_DiscModeCmd(ADC1, ENABLE);
    Disc_Mode_Cmd = ADC1->CR1.DISCEN;
    TEST_ASSERT_MESSAGE(Disc_Mode_Cmd == 0x1, "ADC Enables the discontinuous mode on regular group channel test fail!");
    ADC_DiscModeCmd(ADC1, DISABLE);
    Disc_Mode_Cmd = ADC1->CR1.DISCEN;
    TEST_ASSERT_MESSAGE(Disc_Mode_Cmd == 0x0, "ADC Disables the discontinuous mode on regular group channel test fail!");

    ADC_ExternalTrigConvCmd(ADC1, ENABLE);
    ExternalTrigConvCmd = ADC1->CR2.EXTTRIG;
    TEST_ASSERT_MESSAGE(ExternalTrigConvCmd == 0x1, "Enables the ADC1 conversion through external trigger test fail!");
    ADC_ExternalTrigConvCmd(ADC1, DISABLE);
    ExternalTrigConvCmd = ADC1->CR2.EXTTRIG;
    TEST_ASSERT_MESSAGE(ExternalTrigConvCmd == 0x0, "Disables the ADC1 conversion through external trigger test fail!");
    /* Start ADC1 Software Conversion */
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);
    start_cmd = ADC1->CR2.SWSTART;
    TEST_ASSERT_MESSAGE(start_cmd == 0x1, "ADC start converdsion enable cmd test fail!");
    ADC_SoftwareStartConvCmd(ADC1, DISABLE);
    start_cmd = ADC1->CR2.SWSTART;
    TEST_ASSERT_MESSAGE(start_cmd == 0x0, "ADC start converdsion disable cmd test fail!");

    ADC_DMACmd(ADC1, ENABLE);
    DMACmd = ADC1->CR2.value & 0x00000100;
    TEST_ASSERT_MESSAGE(DMACmd == 0x00000100, "ADC DMA enable cmd test fail!");
    ADC_ClearFlag(ADC1, ADC_FLAG_AWD);
    FLAG_AWD = ADC1->SR.AWD;
    TEST_ASSERT_MESSAGE(FLAG_AWD == 0x0, "ADC clear flag cmd test fail!");

    ADC_AnalogWatchdogThresholdsConfig(ADC1, 0xff, 0x1);
    AnalogWatchdog = ADC1->HTR.HT << 16 | ADC1->LTR.LT;
    TEST_ASSERT_MESSAGE(AnalogWatchdog == 0xff0001, "ADC Analog Watchdog test fail!");
    ADC_AnalogWatchdogCmd(ADC1, ADC_AnalogWatchdog_SingleRegEnable);
    AnalogWatchdogCmd = ADC1->CR1.AWDSGL & ADC1->CR1.AWDEN;
    TEST_ASSERT_MESSAGE(AnalogWatchdogCmd == 0x1, "ADC Analog Watchdog cmd test fail!");
    
}

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
    debug("ADC test start!\r\n");
    unity_test();
    while (1) {
    }
}
