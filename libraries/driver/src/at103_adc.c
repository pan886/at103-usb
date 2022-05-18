/**
 * @file at103_adc.c
 * @brief This file provides all the ADC firmware functions.
 * @author pantianwen (pantianwen@163.com)
 * @version 1.0
 * @date 2022-03-20
 * @copyright Copyright (c) 2022 Icore, Inc
 */
#include "at103_adc.h"
#include "at103_rcc.h"
#ifdef ADC_MODULE_ENABLED
void ADC_DeInit(ADC_TypeDef *ADCx)
{
    /* Check the parameters */
    assert_param(IS_ADC_ALL_PERIPH(ADCx));

    if (ADCx == ADC1) {
        /* Enable ADC1 reset state */
        RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1, ENABLE);
        /* Release ADC1 from reset state */
        RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1, DISABLE);
    } else if (ADCx == ADC2) {
        /* Enable ADC2 reset state */
        RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC2, ENABLE);
        /* Release ADC2 from reset state */
        RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC2, DISABLE);
    }
    ADCx->ANA.value &= ~(1 << 2);
    ADCx->ANA.value |= (1 << 2);
}

void ADC_Init(ADC_TypeDef *ADCx, ADC_InitTypeDef *ADC_InitStruct)
{
    uint32_t tmpreg1 = 0;
    uint8_t  tmpreg2 = 0;
    /* Check the parameters */
    assert_param(IS_ADC_ALL_PERIPH(ADCx));
    assert_param(IS_ADC_MODE(ADC_InitStruct->ADC_Mode));
    assert_param(IS_FUNCTIONAL_STATE(ADC_InitStruct->ADC_ScanConvMode));
    assert_param(IS_FUNCTIONAL_STATE(ADC_InitStruct->ADC_ContinuousConvMode));
    assert_param(IS_ADC_EXT_TRIG(ADC_InitStruct->ADC_ExternalTrigConv));
    assert_param(IS_ADC_DATA_ALIGN(ADC_InitStruct->ADC_DataAlign));
    assert_param(IS_ADC_REGULAR_LENGTH(ADC_InitStruct->ADC_NbrOfChannel));

    /*---------------------------- ADCx CR1 Configuration -----------------*/
    /* Get the ADCx CR1 value */
    tmpreg1 = ADCx->CR1.value;
    /* Clear DUALMOD and SCAN bits */
    tmpreg1 &= CR1_CLEAR_Mask;
    /* Configure ADCx: Dual mode and scan conversion mode */
    /* Set DUALMOD bits according to ADC_Mode value */
    /* Set SCAN bit according to ADC_ScanConvMode value */
    tmpreg1 |= (uint32_t)(ADC_InitStruct->ADC_Mode | ((uint32_t)ADC_InitStruct->ADC_ScanConvMode << 8));
    /* Write to ADCx CR1 */
    ADCx->CR1.value = tmpreg1;

    /*---------------------------- ADCx CR2 Configuration -----------------*/
    /* Get the ADCx CR2 value */
    tmpreg1 = ADCx->CR2.value;
    /* Clear CONT, ALIGN and EXTSEL bits */
    tmpreg1 &= CR2_CLEAR_Mask;
    /* Configure ADCx: external trigger event and continuous conversion mode */
    /* Set ALIGN bit according to ADC_DataAlign value */
    /* Set EXTSEL bits according to ADC_ExternalTrigConv value */
    /* Set CONT bit according to ADC_ContinuousConvMode value */
    tmpreg1 |= (uint32_t)(ADC_InitStruct->ADC_DataAlign | ADC_InitStruct->ADC_ExternalTrigConv |
                          ((uint32_t)ADC_InitStruct->ADC_ContinuousConvMode << 1));
    /* Write to ADCx CR2 */
    ADCx->CR2.value = tmpreg1;

    /*---------------------------- ADCx SQR1 Configuration -----------------*/
    /* Get the ADCx SQR1 value */
    tmpreg1 = ADCx->SQR1.value;
    /* Clear L bits */
    tmpreg1 &= SQR1_CLEAR_Mask;
    /* Configure ADCx: regular channel sequence length */
    /* Set L bits according to ADC_NbrOfChannel value */
    tmpreg2 |= (uint8_t)(ADC_InitStruct->ADC_NbrOfChannel - (uint8_t)1);
    tmpreg1 |= (uint32_t)tmpreg2 << 20;
    /* Write to ADCx SQR1 */
    ADCx->SQR1.value = tmpreg1;

    ADCx->ANA.value |= ADC_InitStruct->resolution << 11;
    ADCx->ANA.value |= 0x6;
}

void ADC_Cmd(ADC_TypeDef *ADCx, FunctionalState NewState)
{

    assert_param(IS_ADC_ALL_PERIPH(ADCx));
    if (NewState != DISABLE) {

        ADCx->CR2.ADON |= 0x1;
    } else {

        ADCx->CR2.ADON &= 0x0;
    }
}

void ADC_StructInit(ADC_InitTypeDef *ADC_InitStruct)
{
    ADC_InitStruct->ADC_Mode               = ADC_Mode_Independent;
    ADC_InitStruct->ADC_ScanConvMode       = DISABLE;
    ADC_InitStruct->ADC_ContinuousConvMode = DISABLE;
    ADC_InitStruct->ADC_ExternalTrigConv   = ADC_ExternalTrigConv_T1_CC1;
}

void ADC_ITConfig(ADC_TypeDef *ADCx, uint16_t ADC_IT, FunctionalState NewState)
{
    uint8_t itmask = 0;
    assert_param(IS_ADC_ALL_PERIPH(ADCx));

    itmask = (uint8_t)ADC_IT;
    if (NewState != DISABLE) {
        ADCx->CR1.value |= itmask;
    } else {

        ADCx->CR1.value &= (~(uint32_t)itmask);
    }
}

void ADC_StartCalibration(ADC_TypeDef *ADCx)
{
    assert_param(IS_ADC_ALL_PERIPH(ADCx));
    ADCx->CR2.value |= 0x4;
}

FlagStatus ADC_GetCalibrationStatus(ADC_TypeDef *ADCx)
{
    FlagStatus bitstatus = RESET;
    assert_param(IS_ADC_ALL_PERIPH(ADCx));
    if ((ADCx->CR2.value & 0x4) != (uint32_t)RESET) {
        bitstatus = SET;
    } else {
        bitstatus = RESET;
    }
    return bitstatus;
}

void ADC_SoftwareStartConvCmd(ADC_TypeDef *ADCx, FunctionalState NewState)
{
    assert_param(IS_ADC_ALL_PERIPH(ADCx));
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    if (NewState != DISABLE) {
        ADCx->CR2.value |= 0x500000;
    } else {
        ADCx->CR2.value &= 0xAFFFFF;
    }
}

FlagStatus ADC_GetSoftwareStartConvStatus(ADC_TypeDef *ADCx)
{
    FlagStatus bitstatus = RESET;
    assert_param(IS_ADC_ALL_PERIPH(ADCx));
    if ((ADCx->CR2.value & 0x400000) != (uint32_t)RESET) {
        bitstatus = SET;
    } else {
        bitstatus = RESET;
    }
    return bitstatus;
}

void ADC_DiscModeChannelCountConfig(ADC_TypeDef *ADCx, uint8_t Number)
{
    assert_param(IS_ADC_ALL_PERIPH(ADCx));
    uint32_t tmpreg1 = 0;
    uint32_t tmpreg2 = 0;
    /* Get the old register value */
    tmpreg1 = ADCx->CR1.value;
    /* Clear the old discontinuous mode channel count */
    tmpreg1 &= CR1_DISCNUM_Reset;
    /* Set the discontinuous mode channel count */
    tmpreg2 = Number - 1;
    tmpreg1 |= tmpreg2 << 13;
    /* Store the new register value */
    ADCx->CR1.value = tmpreg1;
}

void ADC_ResetCalibration(ADC_TypeDef *ADCx)
{
    /* Check the parameters */
    assert_param(IS_ADC_ALL_PERIPH(ADCx));
    /* Resets the selected ADC calibration registers */
    ADCx->CR2.value |= CR2_RSTCAL_Set;
}

FlagStatus ADC_GetResetCalibrationStatus(ADC_TypeDef *ADCx)
{
    FlagStatus bitstatus = RESET;
    /* Check the parameters */
    assert_param(IS_ADC_ALL_PERIPH(ADCx));
    /* Check the status of RSTCAL bit */
    if ((ADCx->CR2.value & CR2_RSTCAL_Set) != (uint32_t)RESET) {
        /* RSTCAL bit is set */
        bitstatus = SET;
    } else {
        /* RSTCAL bit is reset */
        bitstatus = RESET;
    }
    /* Return the RSTCAL bit status */
    return bitstatus;
}

uint16_t ADC_GetConversionValue(ADC_TypeDef *ADCx)
{
    /* Check the parameters */
    assert_param(IS_ADC_ALL_PERIPH(ADCx));
    /* Return the selected ADC conversion value */
    return (uint16_t)ADCx->DR.value;
}

uint32_t ADC_GetDualModeConversionValue(void)
{
    /* Return the dual mode conversion value */
    return (*(__IO uint32_t *)DR_ADDRESS);
}

void ADC_AutoInjectedConvCmd(ADC_TypeDef *ADCx, FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_ADC_ALL_PERIPH(ADCx));
    assert_param(IS_FUNCTIONAL_STATE(NewState));
    if (NewState != DISABLE) {
        /* Enable the selected ADC automatic injected group conversion */
        ADCx->CR1.value |= CR1_JAUTO_Set;
    } else {
        /* Disable the selected ADC automatic injected group conversion */
        ADCx->CR1.value &= CR1_JAUTO_Reset;
    }
}

void ADC_InjectedDiscModeCmd(ADC_TypeDef *ADCx, FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_ADC_ALL_PERIPH(ADCx));
    assert_param(IS_FUNCTIONAL_STATE(NewState));
    if (NewState != DISABLE) {
        /* Enable the selected ADC injected discontinuous mode */
        ADCx->CR1.value |= CR1_JDISCEN_Set;
    } else {
        /* Disable the selected ADC injected discontinuous mode */
        ADCx->CR1.value &= CR1_JDISCEN_Reset;
    }
}

void ADC_ExternalTrigInjectedConvConfig(ADC_TypeDef *ADCx, uint32_t ADC_ExternalTrigInjecConv)
{
    uint32_t tmpreg = 0;
    /* Check the parameters */
    assert_param(IS_ADC_ALL_PERIPH(ADCx));
    assert_param(IS_ADC_EXT_INJEC_TRIG(ADC_ExternalTrigInjecConv));
    /* Get the old register value */
    tmpreg = ADCx->CR2.value;
    /* Clear the old external event selection for injected group */
    tmpreg &= CR2_JEXTSEL_Reset;
    /* Set the external event selection for injected group */
    tmpreg |= ADC_ExternalTrigInjecConv;
    /* Store the new register value */
    ADCx->CR2.value = tmpreg;
}

void ADC_ExternalTrigInjectedConvCmd(ADC_TypeDef *ADCx, FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_ADC_ALL_PERIPH(ADCx));
    assert_param(IS_FUNCTIONAL_STATE(NewState));
    if (NewState != DISABLE) {
        /* Enable the selected ADC external event selection for injected group */
        ADCx->CR2.value |= CR2_JEXTTRIG_Set;
    } else {
        /* Disable the selected ADC external event selection for injected group */
        ADCx->CR2.value &= CR2_JEXTTRIG_Reset;
    }
}

void ADC_SoftwareStartInjectedConvCmd(ADC_TypeDef *ADCx, FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_ADC_ALL_PERIPH(ADCx));
    assert_param(IS_FUNCTIONAL_STATE(NewState));
    if (NewState != DISABLE) {
        /* Enable the selected ADC conversion for injected group on external event and start the selected
       ADC injected conversion */
        ADCx->CR2.value |= CR2_JEXTTRIG_JSWSTART_Set;
    } else {
        /* Disable the selected ADC conversion on external event for injected group and stop the selected
       ADC injected conversion */
        ADCx->CR2.value &= CR2_JEXTTRIG_JSWSTART_Reset;
    }
}

FlagStatus ADC_GetSoftwareStartInjectedConvCmdStatus(ADC_TypeDef *ADCx)
{
    FlagStatus bitstatus = RESET;
    /* Check the parameters */
    assert_param(IS_ADC_ALL_PERIPH(ADCx));
    /* Check the status of JSWSTART bit */
    if ((ADCx->CR2.value & CR2_JSWSTART_Set) != (uint32_t)RESET) {
        /* JSWSTART bit is set */
        bitstatus = SET;
    } else {
        /* JSWSTART bit is reset */
        bitstatus = RESET;
    }
    /* Return the JSWSTART bit status */
    return bitstatus;
}

void ADC_InjectedChannelConfig(ADC_TypeDef *ADCx, uint8_t ADC_Channel, uint8_t Rank, uint8_t ADC_SampleTime)
{
    uint32_t tmpreg1 = 0, tmpreg2 = 0, tmpreg3 = 0;
    /* Check the parameters */
    assert_param(IS_ADC_ALL_PERIPH(ADCx));
    assert_param(IS_ADC_CHANNEL(ADC_Channel));
    assert_param(IS_ADC_INJECTED_RANK(Rank));
    assert_param(IS_ADC_SAMPLE_TIME(ADC_SampleTime));
    /* if ADC_Channel_10 ... ADC_Channel_17 is selected */
    if (ADC_Channel > ADC_Channel_9) {
        /* Get the old register value */
        tmpreg1 = ADCx->SMPR1.value;
        /* Calculate the mask to clear */
        tmpreg2 = SMPR1_SMP_Set << (3 * (ADC_Channel - 10));
        /* Clear the old channel sample time */
        tmpreg1 &= ~tmpreg2;
        /* Calculate the mask to set */
        tmpreg2 = (uint32_t)ADC_SampleTime << (3 * (ADC_Channel - 10));
        /* Set the new channel sample time */
        tmpreg1 |= tmpreg2;
        /* Store the new register value */
        ADCx->SMPR1.value = tmpreg1;
    } else /* ADC_Channel include in ADC_Channel_[0..9] */
    {
        /* Get the old register value */
        tmpreg1 = ADCx->SMPR2.value;
        /* Calculate the mask to clear */
        tmpreg2 = SMPR2_SMP_Set << (3 * ADC_Channel);
        /* Clear the old channel sample time */
        tmpreg1 &= ~tmpreg2;
        /* Calculate the mask to set */
        tmpreg2 = (uint32_t)ADC_SampleTime << (3 * ADC_Channel);
        /* Set the new channel sample time */
        tmpreg1 |= tmpreg2;
        /* Store the new register value */
        ADCx->SMPR2.value = tmpreg1;
    }
    /* Rank configuration */
    /* Get the old register value */
    tmpreg1 = ADCx->JSQR.value;
    /* Get JL value: Number = JL+1 */
    tmpreg3 = (tmpreg1 & JSQR_JL_Set) >> 20;
    /* Calculate the mask to clear: ((Rank-1)+(4-JL-1)) */
    tmpreg2 = JSQR_JSQ_Set << (5 * (uint8_t)((Rank + 3) - (tmpreg3 + 1)));
    /* Clear the old JSQx bits for the selected rank */
    tmpreg1 &= ~tmpreg2;
    /* Calculate the mask to set: ((Rank-1)+(4-JL-1)) */
    tmpreg2 = (uint32_t)ADC_Channel << (5 * (uint8_t)((Rank + 3) - (tmpreg3 + 1)));
    /* Set the JSQx bits for the selected rank */
    tmpreg1 |= tmpreg2;
    /* Store the new register value */
    ADCx->JSQR.value = tmpreg1;
}

void ADC_InjectedSequencerLengthConfig(ADC_TypeDef *ADCx, uint8_t Length)
{
    uint32_t tmpreg1 = 0;
    uint32_t tmpreg2 = 0;
    /* Check the parameters */
    assert_param(IS_ADC_ALL_PERIPH(ADCx));
    assert_param(IS_ADC_INJECTED_LENGTH(Length));

    /* Get the old register value */
    tmpreg1 = ADCx->JSQR.value;
    /* Clear the old injected sequnence lenght JL bits */
    tmpreg1 &= JSQR_JL_Reset;
    /* Set the injected sequnence lenght JL bits */
    tmpreg2 = Length - 1;
    tmpreg1 |= tmpreg2 << 20;
    /* Store the new register value */
    ADCx->JSQR.value = tmpreg1;
}

void ADC_SetInjectedOffset(ADC_TypeDef *ADCx, uint8_t ADC_InjectedChannel, uint16_t Offset)
{
    __IO uint32_t tmp = 0;

    /* Check the parameters */
    assert_param(IS_ADC_ALL_PERIPH(ADCx));
    assert_param(IS_ADC_INJECTED_CHANNEL(ADC_InjectedChannel));
    assert_param(IS_ADC_OFFSET(Offset));

    tmp = (uint32_t)ADCx;
    tmp += ADC_InjectedChannel;

    /* Set the selected injected channel data offset */
    *(__IO uint32_t *)tmp = (uint32_t)Offset;
}

uint16_t ADC_GetInjectedConversionValue(ADC_TypeDef *ADCx, uint8_t ADC_InjectedChannel)
{
    __IO uint32_t tmp = 0;

    /* Check the parameters */
    assert_param(IS_ADC_ALL_PERIPH(ADCx));
    assert_param(IS_ADC_INJECTED_CHANNEL(ADC_InjectedChannel));

    tmp = (uint32_t)ADCx;
    tmp += ADC_InjectedChannel + JDR_Offset;

    /* Returns the selected injected channel conversion data value */
    return (uint16_t)(*(__IO uint32_t *)tmp);
}

void ADC_AnalogWatchdogCmd(ADC_TypeDef *ADCx, uint32_t ADC_AnalogWatchdog)
{
    uint32_t tmpreg = 0;
    /* Check the parameters */
    assert_param(IS_ADC_ALL_PERIPH(ADCx));
    assert_param(IS_ADC_ANALOG_WATCHDOG(ADC_AnalogWatchdog));
    /* Get the old register value */
    tmpreg = ADCx->CR1.value;
    /* Clear AWDEN, AWDENJ and AWDSGL bits */
    tmpreg &= CR1_AWDMode_Reset;
    /* Set the analog watchdog enable mode */
    tmpreg |= ADC_AnalogWatchdog;
    /* Store the new register value */
    ADCx->CR1.value = tmpreg;
}

void ADC_AnalogWatchdogThresholdsConfig(ADC_TypeDef *ADCx, uint16_t HighThreshold,
                                        uint16_t LowThreshold)
{
    /* Check the parameters */
    assert_param(IS_ADC_ALL_PERIPH(ADCx));
    assert_param(IS_ADC_THRESHOLD(HighThreshold));
    assert_param(IS_ADC_THRESHOLD(LowThreshold));
    /* Set the ADCx high threshold */
    ADCx->HTR.value = HighThreshold;
    /* Set the ADCx low threshold */
    ADCx->LTR.value = LowThreshold;
}

void ADC_AnalogWatchdogSingleChannelConfig(ADC_TypeDef *ADCx, uint8_t ADC_Channel)
{
    uint32_t tmpreg = 0;
    /* Check the parameters */
    assert_param(IS_ADC_ALL_PERIPH(ADCx));
    assert_param(IS_ADC_CHANNEL(ADC_Channel));
    /* Get the old register value */
    tmpreg = ADCx->CR1.value;
    /* Clear the Analog watchdog channel select bits */
    tmpreg &= CR1_AWDCH_Reset;
    /* Set the Analog watchdog channel */
    tmpreg |= ADC_Channel;
    /* Store the new register value */
    ADCx->CR1.value = tmpreg;
}

void ADC_TempSensorVrefintCmd(FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_FUNCTIONAL_STATE(NewState));
    if (NewState != DISABLE) {
        /* Enable the temperature sensor and Vrefint channel*/
        ADC1->CR2.value |= CR2_TSVREFE_Set;
    } else {
        /* Disable the temperature sensor and Vrefint channel*/
        ADC1->CR2.value &= CR2_TSVREFE_Reset;
    }
}

void ADC_ClearFlag(ADC_TypeDef *ADCx, uint8_t ADC_FLAG)
{
    /* Check the parameters */
    assert_param(IS_ADC_ALL_PERIPH(ADCx));
    assert_param(IS_ADC_CLEAR_FLAG(ADC_FLAG));
    /* Clear the selected ADC flags */
    ADCx->SR.value = ~(uint32_t)ADC_FLAG;
}

ITStatus ADC_GetITStatus(ADC_TypeDef *ADCx, uint16_t ADC_IT)
{
    ITStatus bitstatus = RESET;
    uint32_t itmask = 0, enablestatus = 0;
    /* Check the parameters */
    assert_param(IS_ADC_ALL_PERIPH(ADCx));
    assert_param(IS_ADC_GET_IT(ADC_IT));
    /* Get the ADC IT index */
    itmask = ADC_IT >> 8;
    /* Get the ADC_IT enable bit status */
    enablestatus = (ADCx->CR1.value & (uint8_t)ADC_IT);
    /* Check the status of the specified ADC interrupt */
    if (((ADCx->SR.value & itmask) != (uint32_t)RESET) && enablestatus) {
        /* ADC_IT is set */
        bitstatus = SET;
    } else {
        /* ADC_IT is reset */
        bitstatus = RESET;
    }
    /* Return the ADC_IT status */
    return bitstatus;
}

void ADC_ClearITPendingBit(ADC_TypeDef *ADCx, uint16_t ADC_IT)
{
    uint8_t itmask = 0;
    /* Check the parameters */
    assert_param(IS_ADC_ALL_PERIPH(ADCx));
    assert_param(IS_ADC_IT(ADC_IT));
    /* Get the ADC IT index */
    itmask = (uint8_t)(ADC_IT >> 8);
    /* Clear the selected ADC interrupt pending bits */
    ADCx->SR.value = ~(uint32_t)itmask;
}

FlagStatus ADC_GetFlagStatus(ADC_TypeDef *ADCx, uint8_t ADC_FLAG)
{
    FlagStatus bitstatus = RESET;
    /* Check the parameters */
    assert_param(IS_ADC_ALL_PERIPH(ADCx));
    assert_param(IS_ADC_GET_FLAG(ADC_FLAG));
    /* Check the status of the specified ADC flag */
    if ((ADCx->SR.value & ADC_FLAG) != (uint8_t)RESET) {
        /* ADC_FLAG is set */
        bitstatus = SET;
    } else {
        /* ADC_FLAG is reset */
        bitstatus = RESET;
    }
    /* Return the ADC_FLAG status */
    return bitstatus;
}

void ADC_RegularChannelConfig(ADC_TypeDef *ADCx, uint8_t ADC_Channel, uint8_t Rank, uint8_t ADC_SampleTime)
{
    uint32_t tmpreg1 = 0, tmpreg2 = 0;
    /* Check the parameters */
    assert_param(IS_ADC_ALL_PERIPH(ADCx));
    assert_param(IS_ADC_CHANNEL(ADC_Channel));
    assert_param(IS_ADC_REGULAR_RANK(Rank));
    assert_param(IS_ADC_SAMPLE_TIME(ADC_SampleTime));
    /* if ADC_Channel_10 ... ADC_Channel_17 is selected */
    if (ADC_Channel > ADC_Channel_9) {
        /* Get the old register value */
        tmpreg1 = ADCx->SMPR1.value;
        /* Calculate the mask to clear */
        tmpreg2 = SMPR1_SMP_Set << (3 * (ADC_Channel - 10));
        /* Clear the old channel sample time */
        tmpreg1 &= ~tmpreg2;
        /* Calculate the mask to set */
        tmpreg2 = (uint32_t)ADC_SampleTime << (3 * (ADC_Channel - 10));
        /* Set the new channel sample time */
        tmpreg1 |= tmpreg2;
        /* Store the new register value */
        ADCx->SMPR1.value = tmpreg1;
    } else /* ADC_Channel include in ADC_Channel_[0..9] */
    {
        /* Get the old register value */
        tmpreg1 = ADCx->SMPR2.value;
        /* Calculate the mask to clear */
        tmpreg2 = SMPR2_SMP_Set << (3 * ADC_Channel);
        /* Clear the old channel sample time */
        tmpreg1 &= ~tmpreg2;
        /* Calculate the mask to set */
        tmpreg2 = (uint32_t)ADC_SampleTime << (3 * ADC_Channel);
        /* Set the new channel sample time */
        tmpreg1 |= tmpreg2;
        /* Store the new register value */
        ADCx->SMPR2.value = tmpreg1;
    }
    /* For Rank 1 to 6 */
    if (Rank < 7) {
        /* Get the old register value */
        tmpreg1 = ADCx->SQR3.value;
        /* Calculate the mask to clear */
        tmpreg2 = SQR3_SQ_Set << (5 * (Rank - 1));
        /* Clear the old SQx bits for the selected rank */
        tmpreg1 &= ~tmpreg2;
        /* Calculate the mask to set */
        tmpreg2 = (uint32_t)ADC_Channel << (5 * (Rank - 1));
        /* Set the SQx bits for the selected rank */
        tmpreg1 |= tmpreg2;
        /* Store the new register value */
        ADCx->SQR3.value = tmpreg1;
    }
    /* For Rank 7 to 12 */
    else if (Rank < 13) {
        /* Get the old register value */
        tmpreg1 = ADCx->SQR2.value;
        /* Calculate the mask to clear */
        tmpreg2 = SQR2_SQ_Set << (5 * (Rank - 7));
        /* Clear the old SQx bits for the selected rank */
        tmpreg1 &= ~tmpreg2;
        /* Calculate the mask to set */
        tmpreg2 = (uint32_t)ADC_Channel << (5 * (Rank - 7));
        /* Set the SQx bits for the selected rank */
        tmpreg1 |= tmpreg2;
        /* Store the new register value */
        ADCx->SQR2.value = tmpreg1;
    }
    /* For Rank 13 to 16 */
    else {
        /* Get the old register value */
        tmpreg1 = ADCx->SQR1.value;
        /* Calculate the mask to clear */
        tmpreg2 = SQR1_SQ_Set << (5 * (Rank - 13));
        /* Clear the old SQx bits for the selected rank */
        tmpreg1 &= ~tmpreg2;
        /* Calculate the mask to set */
        tmpreg2 = (uint32_t)ADC_Channel << (5 * (Rank - 13));
        /* Set the SQx bits for the selected rank */
        tmpreg1 |= tmpreg2;
        /* Store the new register value */
        ADCx->SQR1.value = tmpreg1;
    }
}

void ADC_DiscModeCmd(ADC_TypeDef *ADCx, FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_ADC_ALL_PERIPH(ADCx));
    assert_param(IS_FUNCTIONAL_STATE(NewState));
    if (NewState != DISABLE) {
        /* Enable the selected ADC regular discontinuous mode */
        ADCx->CR1.value |= CR1_DISCEN_Set;
    } else {
        /* Disable the selected ADC regular discontinuous mode */
        ADCx->CR1.value &= CR1_DISCEN_Reset;
    }
}

void ADC_ExternalTrigConvCmd(ADC_TypeDef *ADCx, FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_ADC_ALL_PERIPH(ADCx));
    assert_param(IS_FUNCTIONAL_STATE(NewState));
    if (NewState != DISABLE) {
        /* Enable the selected ADC conversion on external event */
        ADCx->CR2.value |= CR2_EXTTRIG_Set;
    } else {
        /* Disable the selected ADC conversion on external event */
        ADCx->CR2.value &= CR2_EXTTRIG_Reset;
    }
}

void ADC_DMACmd(ADC_TypeDef *ADCx, FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_ADC_DMA_PERIPH(ADCx));
    assert_param(IS_FUNCTIONAL_STATE(NewState));
    if (NewState != DISABLE) {
        /* Enable the selected ADC DMA request */
        ADCx->CR2.value |= CR2_DMA_Set;
    } else {
        /* Disable the selected ADC DMA request */
        ADCx->CR2.value &= CR2_DMA_Reset;
    }
}
#endif