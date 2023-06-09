/**
 * @file main.c
 * @brief ADC TEST.
 * @author pantianwen (pantianwen@163.com)
 * @version 1.0
 * @date 2022-05-18
 * @copyright Copyright (c) 2022 Icore, Inc
 * 
 * Test function    :TEST ADC API.
 * Board Version    ：AiP32rv001_EVB_V01
 * Hardware setup   : Without.
 */
#include "at103.h"
#include "unity.h"

void test_func(void)
{
    uint32_t        cmd                         = 0;
    uint32_t        Rst_Cal                     = 0;
    uint32_t        start_cal                   = 0;
    uint32_t        start_conv                  = 0;
    uint32_t        Analog_Wdg                  = 0;
    uint32_t        HighThreshold               = 0;
    uint32_t        LowThreshold                = 0;
    uint32_t        TempVrefintCmd              = 0;
    uint32_t        clear_flag                  = 0;
    uint32_t        DiscModeCmd                 = 0;
    uint32_t        DMACmd                      = 0;
    uint32_t        InjectedSequencerLength     = 0;
    uint32_t        InjectedDiscModeCmd         = 0;
    uint32_t        AutoInjectedConvCmd         = 0;
    uint32_t        ExternalTrigInjectedConvCmd = 0;
    uint32_t        DiscModeChannelCountConfig  = 0;
    uint32_t        ADC_Mode                    = 0;
    uint32_t        ScanConvMode                = 0;
    uint32_t        ExternalTrigConv            = 0;
    uint32_t        DataAlign                   = 0;
    uint32_t        AnaWdg_SingleChannel_Cfg    = 0;
    uint32_t        ClearITPendingBit           = 0;
    ADC_InitTypeDef ADC_InitStructure;
    /* ADC1 configuration */
    ADC_InitStructure.ADC_Mode               = ADC_Mode_Independent;
    ADC_InitStructure.ADC_ScanConvMode       = DISABLE;
    ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
    ADC_InitStructure.ADC_ExternalTrigConv   = ADC_ExternalTrigConv_None;
    ADC_InitStructure.ADC_DataAlign          = ADC_DataAlign_Right;
    ADC_InitStructure.ADC_NbrOfChannel       = 1;

    ADC_Init(ADC1, &ADC_InitStructure);
    ADC_Mode         = ADC1->CR1.DUALMOD;
    ScanConvMode     = ADC1->CR1.SCAN;
    ExternalTrigConv = ADC1->CR2.EXTSEL;
    DataAlign        = ADC1->CR2.ALIGN;
    TEST_ASSERT_MESSAGE(ADC_Mode == 0x0, "ADC mode test failed!");
    TEST_ASSERT_MESSAGE(ScanConvMode == 0x0, "Scan mode test failed!");
    TEST_ASSERT_MESSAGE(ExternalTrigConv == 0b111, "External event select test failed!");
    TEST_ASSERT_MESSAGE(DataAlign == 0x0, "data align test failed!");
    /* Enable ADC1 */
    ADC_Cmd(ADC1, ENABLE);
    cmd = ADC1->CR2.ADON;
    TEST_ASSERT_MESSAGE(cmd == 0x1, "ADC cmd test failed!");

    /* Enable ADC1 reset calibration register */
    ADC_ResetCalibration(ADC1);
    Rst_Cal = ADC1->CR2.RSTCAL;
    TEST_ASSERT_MESSAGE(Rst_Cal == 0x1, "ADC reset calibration test failed!");

    /* Start ADC1 calibration */
    ADC_StartCalibration(ADC1);
    start_cal = ADC1->CR2.CAL;
    TEST_ASSERT_MESSAGE(start_cal == 0x1, "start calibration test failed!");

    /* Start ADC1 Software Conversion */
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);
    start_conv = ADC1->CR2.SWSTART && ADC1->CR2.EXTTRIG;
    TEST_ASSERT_MESSAGE(start_conv == 0x1, "start convert test failed!");

    ADC_AnalogWatchdogCmd(ADC1, ADC_AnalogWatchdog_SingleRegEnable);
    Analog_Wdg = ADC1->CR1.AWDSGL && ADC1->CR1.AWDEN;
    TEST_ASSERT_MESSAGE(Analog_Wdg == 0x1, "analog watchdog cmd test failed!");

    ADC_AnalogWatchdogThresholdsConfig(ADC1, 0xff, 0xaa);
    HighThreshold = ADC1->HTR.HT;
    LowThreshold  = ADC1->LTR.LT;
    TEST_ASSERT_MESSAGE(HighThreshold == 0xff && LowThreshold == 0xaa, "analog threshold config test failed!");

    ADC_AnalogWatchdogSingleChannelConfig(ADC1, ADC_Channel_0);
    AnaWdg_SingleChannel_Cfg = ADC1->CR1.AWDCH;
    TEST_ASSERT_MESSAGE(AnaWdg_SingleChannel_Cfg == 0x0, "analog watchdog single channel config failed!");

    ADC_TempSensorVrefintCmd(ENABLE);
    TempVrefintCmd = ADC1->CR2.TSVREFE;
    TEST_ASSERT_MESSAGE(TempVrefintCmd == 0x1, "Temperature sensor and VREFINT channel enabled failed!");

    ADC_ClearFlag(ADC1, ADC_FLAG_AWD);
    clear_flag = ADC1->SR.AWD;
    TEST_ASSERT_MESSAGE(clear_flag == 0x0, "Clear Analog watchdog flag failed!");

    ADC_ClearITPendingBit(ADC1, ADC_IT_EOC);
    ClearITPendingBit = ADC1->SR.EOC;
    TEST_ASSERT_MESSAGE(ClearITPendingBit == 0x0, "clear IT pednding bit test failed!");

    ADC_DiscModeCmd(ADC1, ENABLE);
    DiscModeCmd = ADC1->CR1.DISCEN;
    TEST_ASSERT_MESSAGE(DiscModeCmd == 0x1, " Discontinuous mode on regular channels enabled failed!");

    ADC_DMACmd(ADC1, ENABLE);
    DMACmd = ADC1->CR2.value & 0x100;
    TEST_ASSERT_MESSAGE(DMACmd == 0x100, " ADC DMA mode enabled failed!");

    ADC_InjectedSequencerLengthConfig(ADC1, 0x1);
    InjectedSequencerLength = ADC1->JSQR.L;
    TEST_ASSERT_MESSAGE(InjectedSequencerLength == 0x0, " Injected Sequencer Length config failed!");

    ADC_InjectedDiscModeCmd(ADC1, ENABLE);
    InjectedDiscModeCmd = ADC1->CR1.DISCEN;
    TEST_ASSERT_MESSAGE(InjectedDiscModeCmd == 0x1, " Enables the discontinuous mode for injected group channel failed!");

    ADC_AutoInjectedConvCmd(ADC1, ENABLE);
    AutoInjectedConvCmd = ADC1->CR1.JAUTO;
    TEST_ASSERT_MESSAGE(AutoInjectedConvCmd == 0x1, "Enables the selected ADC automatic injected group conversion after regular one failed!");

    ADC_ExternalTrigInjectedConvCmd(ADC1, ENABLE);
    ExternalTrigInjectedConvCmd = ADC1->CR2.JEXTTRIG;
    TEST_ASSERT_MESSAGE(ExternalTrigInjectedConvCmd == 0x1, "Conversion on external event enabled failed!");

    ADC_DiscModeChannelCountConfig(ADC1, 1);
    DiscModeChannelCountConfig = ADC1->CR1.DISCNUM;
    TEST_ASSERT_MESSAGE(DiscModeChannelCountConfig == 0x0, "discontinuous mode for the selected ADC regular group channel failed!");
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
    ADC_DeInit(ADC1);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_ADC1, ENABLE);
    RCC_ADCCLKConfig(RCC_HCLK_Div16);
    unity_test();
    while (1) {
    }
}
