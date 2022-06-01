/**
 * @file at103_adc.h
 * @brief This file contains all the functions prototypes for the ADC firmware library. \n
 *        How to use this driver? \n
 * (+) At first, use ADC_Init(...) with ADC initialization structure to initialize ADC. \n
 * (+) Then use ADC_Cmd(...) to enable ADC. \n
 * (+) Use ADC_ResetCalibration(...) to reset calibration register and use ADC_StartCalibration(...) to Start ADC1 calibration. \n
 * (+) Finally, use ADC_SoftwareStartConvCmd(...) to Start ADC Software Conversion. 
 * @author pantianwen (pantianwen@163.com)
 * @version 1.0
 * @date 2022-03-20
 * @copyright Copyright (c) 2022 Icore, Inc
 */

#ifndef AT103_ADC_H
#define AT103_ADC_H
#include "at103.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief ADC Init structure definition  
 */
typedef struct
{
    uint32_t ADC_Mode; /**< configures the ADC to operate in Dual mode. */

    FunctionalState ADC_ScanConvMode; /**< Specifies whether the conversion is performed in
                                         Scan (multichannels) or Single (one channel) mode. */

    FunctionalState ADC_ContinuousConvMode; /**< Specifies whether the conversion is performed in
                                               Continuous or Single mode. */

    uint32_t ADC_ExternalTrigConv; /**< Defines the external trigger used to start the analog
                                               to digital conversion of regular channels. */

    uint32_t ADC_DataAlign; /**< Specifies whether the ADC data alignment is left or right. */

    uint8_t ADC_NbrOfChannel; /**< Specifies the number of ADC channels that will be converted
                                   using the sequencer for regular channel group. */
    uint8_t resolution;       /**< Specifies the ADC resolution. */

} ADC_InitTypeDef;

#define ADC_Channel_0  ((uint8_t)0x00) /**< ADC channel 0. */
#define ADC_Channel_1  ((uint8_t)0x01) /**< ADC channel 1. */
#define ADC_Channel_2  ((uint8_t)0x02) /**< ADC channel 2. */
#define ADC_Channel_3  ((uint8_t)0x03) /**< ADC channel 3. */
#define ADC_Channel_4  ((uint8_t)0x04) /**< ADC channel 4. */
#define ADC_Channel_5  ((uint8_t)0x05) /**< ADC channel 5. */
#define ADC_Channel_6  ((uint8_t)0x06) /**< ADC channel 6. */
#define ADC_Channel_7  ((uint8_t)0x07) /**< ADC channel 7. */
#define ADC_Channel_8  ((uint8_t)0x08) /**< ADC channel 8. */
#define ADC_Channel_9  ((uint8_t)0x09) /**< ADC channel 9. */
#define ADC_Channel_10 ((uint8_t)0x0A) /**< ADC channel 10. */
#define ADC_Channel_11 ((uint8_t)0x0B) /**< ADC channel 11. */
#define ADC_Channel_12 ((uint8_t)0x0C) /**< ADC channel 12. */
#define ADC_Channel_13 ((uint8_t)0x0D) /**< ADC channel 13. */
#define ADC_Channel_14 ((uint8_t)0x0E) /**< ADC channel 14. */
#define ADC_Channel_15 ((uint8_t)0x0F) /**< ADC channel 15. */
#define ADC_Channel_16 ((uint8_t)0x10) /**< ADC channel 16. */
#define ADC_Channel_17 ((uint8_t)0x11) /**< ADC channel 17. */

#define ADC_Channel_TempSensor ((uint8_t)ADC_Channel_16) /**< ADC channel tempture sensor. */
#define ADC_Channel_Vrefint    ((uint8_t)ADC_Channel_17) /**< ADC channel vrefint. */

#define ADC_Mode_Independent             ((uint32_t)0x0000) /**< Independent mode. */
#define ADC_Mode_RegInjectSimult         ((uint32_t)0x0001) /**< Combined regular simultaneous + injected simultaneous mode.*/
#define ADC_Mode_RegSimult_AlterTrig     ((uint32_t)0x0002) /**< Combined regular simultaneous + alternate trigger mode.*/
#define ADC_Mode_InjecSimult_FastInterl  ((uint32_t)0x0003) /**< Combined injected simultaneous + fast interleaved mode.*/
#define ADC_Mode_InjectSimult_SlowInterl ((uint32_t)0x0004) /**< Combined injected simultaneous + slow Interleaved mode.*/
#define ADC_Mode_InjectSimult            ((uint32_t)0x0005) /**< Injected simultaneous mode only.*/
#define ADC_Mode_RegSimult               ((uint32_t)0x0006) /**< Regular simultaneous mode only.*/
#define ADC_Mode_FastInterl              ((uint32_t)0x0007) /**< Fast interleaved mode only.*/
#define ADC_Mode_SlowInterl              ((uint32_t)0x0008) /**< Slow interleaved mode only.*/
#define ADC_Mode_AlterTrig               ((uint32_t)0x0009) /**< Alternate trigger mode only.*/

#define ADC_InjectedChannel_1 ((uint8_t)0x14) /**< ADC injected channel1. */
#define ADC_InjectedChannel_2 ((uint8_t)0x18) /**< ADC injected channel2. */
#define ADC_InjectedChannel_3 ((uint8_t)0x1C) /**< ADC injected channel3. */
#define ADC_InjectedChannel_4 ((uint8_t)0x20) /**< ADC injected channel4.*/

#define ADC_ExternalTrigConv_T1_CC1             ((uint32_t)0x000000)   /**< External trigger sources for regular channels conversion,For ADC1 and ADC2. */
#define ADC_ExternalTrigConv_T1_CC2             ((uint32_t)0x020000)   /**< External trigger sources for regular channels conversion,For ADC1 and ADC2. */
#define ADC_ExternalTrigConv_T2_CC2             ((uint32_t)0x060000)   /**< External trigger sources for regular channels conversion, For ADC1 and ADC2. */
#define ADC_ExternalTrigConv_T3_TRGO            ((uint32_t)0x080000)   /**< External trigger sources for regular channels conversion, For ADC1 and ADC2. */
#define ADC_ExternalTrigConv_T4_CC4             ((uint32_t)0x0A0000)   /**< External trigger sources for regular channels conversion, For ADC1 and ADC2.*/
#define ADC_ExternalTrigConv_Ext_IT11_TIM8_TRGO ((uint32_t)0x0C0000)   /**< External trigger sources for regular channels conversion, For ADC1 and ADC2. */
#define ADC_ExternalTrigConv_T1_CC3             ((uint32_t)0x00040000) /**< External trigger sources for regular channels conversion,For ADC1 and ADC2. */
#define ADC_ExternalTrigConv_None               ((uint32_t)0x000E0000) /**< External trigger sources for regular channels conversion,For ADC1 and ADC2. */

#define ADC_SampleTime_1Cycles   ((uint8_t)0x00) /**< 1.5 cycles sample time.*/
#define ADC_SampleTime_7Cycles   ((uint8_t)0x01) /**< 7.5 cycles sample time.*/
#define ADC_SampleTime_13Cycles  ((uint8_t)0x02) /**< 13.5 cycles sample time.*/
#define ADC_SampleTime_28Cycles  ((uint8_t)0x03) /**< 28.5 cycles sample time.*/
#define ADC_SampleTime_41Cycles  ((uint8_t)0x04) /**< 41.5 cycles sample time.*/
#define ADC_SampleTime_55Cycles  ((uint8_t)0x05) /**< 55.5 cycles sample time.*/
#define ADC_SampleTime_71Cycles  ((uint8_t)0x06) /**< 71.5 cycles sample time. */
#define ADC_SampleTime_239Cycles ((uint8_t)0x07) /**< 239.5 cycles sample time. */

#define ADC_IT_EOC  ((uint16_t)0x0220) /**< End of conversion interrupt. */
#define ADC_IT_AWD  ((uint16_t)0x0140) /**< Analog watchdog interrupt. */
#define ADC_IT_JEOC ((uint16_t)0x0480) /**< The end of conversion interrupt for injected channels. */

#define ADC_FLAG_AWD   ((uint8_t)0x01) /**< Analog watchdog flag. */
#define ADC_FLAG_EOC   ((uint8_t)0x02) /**< End of conversion. */
#define ADC_FLAG_JEOC  ((uint8_t)0x04) /**< Injected channel end of conversion.*/
#define ADC_FLAG_JSTRT ((uint8_t)0x08) /**< Injected channel start flag. */
#define ADC_FLAG_STRT  ((uint8_t)0x10) /**< Regular channel start flag. */

#define ADC_DataAlign_Right ((uint32_t)0x00000000) /**< Right alignment. */
#define ADC_DataAlign_Left  ((uint32_t)0x00000800) /**< Left alignment. */

#define ADC_AnalogWatchdog_SingleRegEnable        ((uint32_t)0x00800200) /**< Analog watchdog single regular channel enable. */
#define ADC_AnalogWatchdog_SingleInjecEnable      ((uint32_t)0x00400200) /**< Analog watchdog single injected channel enable. */
#define ADC_AnalogWatchdog_SingleRegOrInjecEnable ((uint32_t)0x00C00200) /**< Analog watchdog single regular or injected channel enable. */
#define ADC_AnalogWatchdog_AllRegEnable           ((uint32_t)0x00800000) /**< Analog watchdog  all regular channel enable. */
#define ADC_AnalogWatchdog_AllInjecEnable         ((uint32_t)0x00400000) /**< Analog watchdog all injected channel enable. */
#define ADC_AnalogWatchdog_AllRegAllInjecEnable   ((uint32_t)0x00C00000) /**< Analog watchdog  all regular injected enable. */
#define ADC_AnalogWatchdog_None                   ((uint32_t)0x00000000) /**< Analog watchdog none. */

/**
  * @brief Deinitializes the ADCx peripheral registers to their default reset values.
  * @param[in] ADCx where x can be 1 or 2 to select the ADC peripheral.
  */
void ADC_DeInit(ADC_TypeDef *ADCx);

/**
  * @brief Initializes the ADCx peripheral according to the specified parameters in the ADC_InitStruct.
  * @param[in] ADCx  where x can be 1 or 2 to select the ADC peripheral.
  * @param[in] ADC_InitStruct pointer to an ADC_InitTypeDef structure that contains the configuration information for the specified ADC peripheral.
  */
void ADC_Init(ADC_TypeDef *ADCx, ADC_InitTypeDef *ADC_InitStruct);

/**
 * @brief Fills each ADC_InitStruct member with its default value.
 * @param[in] ADC_InitStruct pointer to an ADC_InitTypeDef structure which will be initialized.
 */
void ADC_StructInit(ADC_InitTypeDef *ADC_InitStruct);

/**
 * @brief Enables or disables the specified ADC peripheral.
 * @param[in] ADCx where x can be 1 or 2 to select the ADC peripheral.
 * @param[in] NewState new state of the ADCx peripheral.
 */
void ADC_Cmd(ADC_TypeDef *ADCx, FunctionalState NewState);

/**
  * @brief Enables or disables the specified ADC DMA request.
  * @param[in] ADCx  where x can be 1 or 2 to select the ADC peripheral.
  * @param[in] NewState new state of the selected ADC DMA transfer.
  */
void ADC_DMACmd(ADC_TypeDef *ADCx, FunctionalState NewState);

/**
 * @brief Enables or disables the specified ADC interrupts.
 * @param[in] ADCx  where x can be 1 or 2 to select the ADC peripheral. 
 * @param[in] ADC_IT specifies the ADC interrupt sources to be enabled or disabled.
 * @param[in] NewState new state of the specified ADC interrupts.
 */
void ADC_ITConfig(ADC_TypeDef *ADCx, uint16_t ADC_IT, FunctionalState NewState);

/**
  * @brief Resets the selected ADC calibration registers.
  * @param[in] ADCx where x can be 1 or 2 to select the ADC peripheral.
  */
void ADC_ResetCalibration(ADC_TypeDef *ADCx);

/**
  * @brief Gets the selected ADC reset calibration registers status.
  * @param[in] ADCx where x can be 1 or 2 to select the ADC peripheral.
  * @return FlagStatus The new state of ADC reset calibration registers (SET or RESET).
  */
FlagStatus ADC_GetResetCalibrationStatus(ADC_TypeDef *ADCx);

/**
 * @brief Starts the selected ADC calibration process.
 * @param[in] ADCx where x can be 1 or 2.
 */
void ADC_StartCalibration(ADC_TypeDef *ADCx);

/**
 * @brief Gets the selected ADC calibration status.
 * @param[in] ADCx where x can be 1 or 2.
 * @return FlagStatus  the new state of ADC calibration(SET or RESET).
 */
FlagStatus ADC_GetCalibrationStatus(ADC_TypeDef *ADCx);

/**
 * @brief Enables or disables the selected ADC software start conversion.
 * @param[in] ADCx where x can be 1 or 2.
 * @param[in] NewState new state of the selected ADC software start conversion.
 */
void ADC_SoftwareStartConvCmd(ADC_TypeDef *ADCx, FunctionalState NewState);

/**
 * @brief Gets the selected ADC software start conversion status.
 * @param[in] ADCx where x can be 1 or 2.
 * @return FlagStatus the new state of ADC software start conversion(SET or RESET).
 */
FlagStatus ADC_GetSoftwareStartConvStatus(ADC_TypeDef *ADCx);

/**
 * @brief Configures the discontinuous mode for the selected ADC regular
  *         group channel.
 * @param[in] ADCx where x can be 1 or 2 to select the ADC peripheral.
 * @param[in] Number specifies the discontinuous mode regular channel
  *         count value. This number must be between 1 and 8.
 */
void ADC_DiscModeChannelCountConfig(ADC_TypeDef *ADCx, uint8_t Number);

/**
 * @brief Enables or disables the discontinuous mode on regular group channel for the specified ADC.
 * @param[in] ADCx where x can be 1 or 2 to select the ADC peripheral.
 * @param[in] NewState new state of the selected ADC discontinuous mode on regular group channel.
 */
void ADC_DiscModeCmd(ADC_TypeDef *ADCx, FunctionalState NewState);

/**
  * @brief :Configures for the selected ADC regular channel its corresponding
  *         rank in the sequencer and its sample time.
  * @param[in] ADCx  where x can be 1 or 2 to select the ADC peripheral.
  * @param[in] ADC_Channel the ADC channel to configure. 
  * @param[in] Rank The rank in the regular group sequencer. This parameter must be between 1 to 16.
  * @param[in] ADC_SampleTime The sample time value to be set for the selected channel. 
  */
void ADC_RegularChannelConfig(ADC_TypeDef *ADCx, uint8_t ADC_Channel, uint8_t Rank, uint8_t ADC_SampleTime);

/**
 * @brief : Enables or disables the ADCx conversion through external trigger.
 * @param[in] ADCx where x can be 1 or 2 to select the ADC peripheral.
 * @param[in] NewState new state of the selected ADC external trigger start of conversion.
 */
void ADC_ExternalTrigConvCmd(ADC_TypeDef *ADCx, FunctionalState NewState);

/**
  * @brief Returns the last ADCx conversion result data for regular channel.
  * @param[in] ADCx where x can be 1 or 2 to select the ADC peripheral.
  * @return uint16_t The Data conversion value.
  */
uint16_t ADC_GetConversionValue(ADC_TypeDef *ADCx);

/**
  * @brief Returns the last ADC1 and ADC2 conversion result data in dual mode.
  * @return uint32_t  The Data conversion value.
  */
uint32_t ADC_GetDualModeConversionValue(void);

/**
  * @brief Enables or disables the selected ADC automatic injected group conversion after regular one.
  * @param[in] ADCx where x can be 1 or 2 to select the ADC peripheral.
  * @param[in] NewState new state of the selected ADC auto injected conversion.
  */
void ADC_AutoInjectedConvCmd(ADC_TypeDef *ADCx, FunctionalState NewState);

/**
  * @brief Enables or disables the discontinuous mode for injected group channel for the specified ADC.
  * @param[in] ADCx where x can be 1 or 2 to select the ADC peripheral.
  * @param[in] NewState new state of the selected ADC discontinuous mode on injected group channel.
  */
void ADC_InjectedDiscModeCmd(ADC_TypeDef *ADCx, FunctionalState NewState);

/**
  * @brief Configures the ADCx external trigger for injected channels conversion.
  * @param[in] ADCx where x can be 1 or 2 to select the ADC peripheral.
  * @param[in] ADC_ExternalTrigInjecConv specifies the ADC trigger to start injected conversion. 
  */
void ADC_ExternalTrigInjectedConvConfig(ADC_TypeDef *ADCx, uint32_t ADC_ExternalTrigInjecConv);

/**
  * @brief Enables or disables the ADCx injected channels conversion through external trigger.
  * @param[in] ADCx where x can be 1 or 2 to select the ADC peripheral.
  * @param[in] NewState new state of the selected ADC external trigger start of injected conversion.
  */
void ADC_ExternalTrigInjectedConvCmd(ADC_TypeDef *ADCx, FunctionalState NewState);

/**
  * @brief Enables or disables the selected ADC start of the injected channels conversion.
  * @param[in] ADCx where x can be 1 or 2 to select the ADC peripheral.
  * @param[in] NewState new state of the selected ADC software start injected conversion.
  */
void ADC_SoftwareStartInjectedConvCmd(ADC_TypeDef *ADCx, FunctionalState NewState);

/**
  * @brief Gets the selected ADC Software start injected conversion Status.
  * @param[in] ADCx where x can be 1 or 2 to select the ADC peripheral.
  * @return FlagStatus The new state of ADC software start injected conversion (SET or RESET).
  */
FlagStatus ADC_GetSoftwareStartInjectedConvCmdStatus(ADC_TypeDef *ADCx);

/**
  * @brief Configures for the selected ADC injected channel its corresponding rank in the sequencer and its sample time.
  * @param[in] ADCx where x can be 1 or 2 to select the ADC peripheral.
  * @param[in] ADC_Channel the ADC channel to configure. 
  * @param[in] Rank The rank in the injected group sequencer. This parameter must be between 1 and 4.
  * @param[in] ADC_SampleTime The sample time value to be set for the selected channel. 
  */
void ADC_InjectedChannelConfig(ADC_TypeDef *ADCx, uint8_t ADC_Channel, uint8_t Rank, uint8_t ADC_SampleTime);

/**
 * @brief Configures the sequencer length for injected channels
 * @param[in] ADCx  where x can be 1 or 2 to select the ADC peripheral.
 * @param[in] Length The sequencer length. This parameter must be a number between 1 to 4.
 */
void ADC_InjectedSequencerLengthConfig(ADC_TypeDef *ADCx, uint8_t Length);

/**
  * @brief Set the injected channels conversion value offset
  * @param[in] ADCx where x can be 1 or 2 to select the ADC peripheral.
  * @param[in] ADC_InjectedChannel the ADC injected channel to set its offset. 
  * @param[in] Offset the offset value for the selected ADC injected channel
  */
void ADC_SetInjectedOffset(ADC_TypeDef *ADCx, uint8_t ADC_InjectedChannel, uint16_t Offset);

/**
  * @brief Returns the ADC injected channel conversion result
  * @param[in] ADCx where x can be 1 or 2 to select the ADC peripheral.
  * @param[in] ADC_InjectedChannel the converted ADC injected channel.
  * @return uint16_t The Data conversion value.
  */
uint16_t ADC_GetInjectedConversionValue(ADC_TypeDef *ADCx, uint8_t ADC_InjectedChannel);

/**
  * @brief Enables or disables the analog watchdog on single/all regular or injected channels
  * @param[in] ADCx where x can be 1 or 2 to select the ADC peripheral.
  * @param[in] ADC_AnalogWatchdog the ADC analog watchdog configuration.
  */
void ADC_AnalogWatchdogCmd(ADC_TypeDef *ADCx, uint32_t ADC_AnalogWatchdog);

/**
  * @brief Configures the high and low thresholds of the analog watchdog.
  * @param[in] ADCx where x can be 1 or 2 to select the ADC peripheral.
  * @param[in] HighThreshold the ADC analog watchdog High threshold value.
  * @param[in] LowThreshold the ADC analog watchdog Low threshold value.
  */
void ADC_AnalogWatchdogThresholdsConfig(ADC_TypeDef *ADCx, uint16_t HighThreshold, uint16_t LowThreshold);

/**
  * @brief Configures the analog watchdog guarded single channel.
  * @param[in] ADCx where x can be 1 or 2 to select the ADC peripheral.
  * @param[in] ADC_Channel the ADC channel to configure for the analog watchdog. 
  */
void ADC_AnalogWatchdogSingleChannelConfig(ADC_TypeDef *ADCx, uint8_t ADC_Channel);

/**
  * @brief Enables or disables the temperature sensor and Vrefint channel.
  * @param[in] NewState new state of the temperature sensor.
  */
void ADC_TempSensorVrefintCmd(FunctionalState NewState);

/**
  * @brief Checks whether the specified ADC flag is set or not.
  * @param[in] ADCx where x can be 1 or 2 to select the ADC peripheral.
  * @param[in] ADC_FLAG specifies the flag to check. 
  * @return FlagStatus  
  */
FlagStatus ADC_GetFlagStatus(ADC_TypeDef *ADCx, uint8_t ADC_FLAG);

/**
  * @brief Clears the ADCx's pending flags.
  * @param[in] ADCx where x can be 1 or 2 to select the ADC peripheral.
  * @param[in] ADC_FLAG specifies the flag to clear. 
  */
void ADC_ClearFlag(ADC_TypeDef *ADCx, uint8_t ADC_FLAG);

/**
  * @brief Checks whether the specified ADC interrupt has occurred or not.
  * @param[in] ADCx where x can be 1 or 2 to select the ADC peripheral.
  * @param[in] ADC_IT specifies the ADC interrupt source to check. 
  * @return ITStatus  The new state of ADC_IT (SET or RESET).
  */
ITStatus ADC_GetITStatus(ADC_TypeDef *ADCx, uint16_t ADC_IT);

/**
  * @brief Clears the ADCx's interrupt pending bits.
  * @param[in] ADCx where x can be 1 or 2 to select the ADC peripheral.
  * @param[in] ADC_IT specifies the ADC interrupt pending bit to clear.
  */
void ADC_ClearITPendingBit(ADC_TypeDef *ADCx, uint16_t ADC_IT);

#ifdef __cplusplus
}
#endif

#endif
