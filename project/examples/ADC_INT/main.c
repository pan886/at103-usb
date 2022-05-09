/**
 * @file main.c
 * @brief The example shows how to use channel 10 of ADC1 to acquire analog data in the interrupt mode.
 * We can observe the data of adc from the serial port.
 * @author pantianwen (pantianwen@163.com)
 * @version 1.0
 * @date 2022-04-18
 * @copyright Copyright (c) 2022 Icore, Inc
 */

#include "at103.h"
uint16_t      Get_Adc(uint8_t ch);
uint16_t      adc_data = 0;
__IO uint32_t interrupt_flag;

RCC_ClocksTypeDef adccc;
void              main(void)
{

    ADC_InitTypeDef  ADC_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    pll_init();
    sys_io_init();
    uart_init(UART_BOOT_PORT, UART_PARITY_NONE, UART_STOPBITS_1, UART_DATABITS_8, UART_BOOT_BD);
    __enable_irq();
    /* interrupt function test */
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
    interrupt_flag                                       = ADC1_2_IRQn;

    NVIC_InitStructure.NVIC_IRQChannel = interrupt_flag;
    NVIC_Init(&NVIC_InitStructure);
    ADC_DeInit(ADC1);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_ADC1, ENABLE);
    RCC_PCLK2Config(0xf);
    RCC_ADCCLKConfig(RCC_HCLK_Div16);

    /* Configure PC.0 (ADC Channel10) as analog input */
    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    /* ADC1 configuration */
    ADC_InitStructure.ADC_Mode               = ADC_Mode_Independent;
    ADC_InitStructure.ADC_ScanConvMode       = DISABLE;
    ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
    ADC_InitStructure.ADC_ExternalTrigConv   = ADC_ExternalTrigConv_None;
    ADC_InitStructure.ADC_DataAlign          = ADC_DataAlign_Right;
    ADC_InitStructure.ADC_NbrOfChannel       = 1;

    ADC_Init(ADC1, &ADC_InitStructure);
    /* Enable EOC interrupt */
    ADC_ITConfig(ADC1, ADC_IT_EOC, ENABLE);
    /* Enable ADC1 */
    ADC_Cmd(ADC1, ENABLE);

    /* Enable ADC1 reset calibration register */
    ADC_ResetCalibration(ADC1);
    /* Check the end of ADC1 reset calibration register */
    while (ADC_GetResetCalibrationStatus(ADC1))
        ;

    /* Start ADC1 calibration */
    ADC_StartCalibration(ADC1);
    /* Check the end of ADC1 calibration */
    while (ADC_GetCalibrationStatus(ADC1))
        ;

    /* Start ADC1 Software Conversion */
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);

    while (1) {
    }
}

uint16_t Get_Adc(uint8_t ch)
{
    /* ADC1 regular channel10 configuration */
    ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_239Cycles);
    /* Start ADC1 Software Conversion */
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);
    while (!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC))
        ;
    return ADC_GetConversionValue(ADC1);
}

void ADC1_2_IRQHandler(void)
{
    debug("enter interrupt!\n");
    if (ADC_GetITStatus(ADC1, ADC_IT_EOC) == SET) {
        adc_data = Get_Adc(10);
        debug("adc_data =%d\n", adc_data);
    }
    ADC_ClearITPendingBit(ADC1, ADC_IT_EOC);
}
