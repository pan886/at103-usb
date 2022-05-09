/**
 * @file at103_adc.h
 * @brief This file contains all the functions prototypes for the ADC firmware library.
 * 
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
typedef struct
{
    uint32_t ADC_Mode; /**< configures the ADC to operate in Dual mode */

    FunctionalState ADC_ScanConvMode; /**< Specifies whether the conversion is performed in
                                         Scan (multichannels) or Single (one channel) mode.*/

    FunctionalState ADC_ContinuousConvMode; /**< Specifies whether the conversion is performed in
                                               Continuous or Single mode. */

    uint32_t ADC_ExternalTrigConv; /**< Defines the external trigger used to start the analog
                                               to digital conversion of regular channels. */

    uint32_t ADC_DataAlign; /**< Specifies whether the ADC data alignment is left or right. */

    uint8_t ADC_NbrOfChannel; /**< Specifies the number of ADC channels that will be converted
                                   using the sequencer for regular channel group. */
    uint8_t resolution;       /**< Specifies the ADC resolution */

} ADC_InitTypeDef;

/**
 * @brief ADC_channels
 */
#define ADC_Channel_0  ((uint8_t)0x00)
#define ADC_Channel_1  ((uint8_t)0x01)
#define ADC_Channel_2  ((uint8_t)0x02)
#define ADC_Channel_3  ((uint8_t)0x03)
#define ADC_Channel_4  ((uint8_t)0x04)
#define ADC_Channel_5  ((uint8_t)0x05)
#define ADC_Channel_6  ((uint8_t)0x06)
#define ADC_Channel_7  ((uint8_t)0x07)
#define ADC_Channel_8  ((uint8_t)0x08)
#define ADC_Channel_9  ((uint8_t)0x09)
#define ADC_Channel_10 ((uint8_t)0x0A)
#define ADC_Channel_11 ((uint8_t)0x0B)
#define ADC_Channel_12 ((uint8_t)0x0C)
#define ADC_Channel_13 ((uint8_t)0x0D)
#define ADC_Channel_14 ((uint8_t)0x0E)
#define ADC_Channel_15 ((uint8_t)0x0F)
#define ADC_Channel_16 ((uint8_t)0x10)
#define ADC_Channel_17 ((uint8_t)0x11)

#define ADC_Channel_TempSensor ((uint8_t)ADC_Channel_16)
#define ADC_Channel_Vrefint    ((uint8_t)ADC_Channel_17)

#define IS_ADC_CHANNEL(CHANNEL) (((CHANNEL) == ADC_Channel_0) || ((CHANNEL) == ADC_Channel_1) || \
                                 ((CHANNEL) == ADC_Channel_2) || ((CHANNEL) == ADC_Channel_3) || \
                                 ((CHANNEL) == ADC_Channel_4) || ((CHANNEL) == ADC_Channel_5) || \
                                 ((CHANNEL) == ADC_Channel_6) || ((CHANNEL) == ADC_Channel_7) || \
                                 ((CHANNEL) == ADC_Channel_8) || ((CHANNEL) == ADC_Channel_9) || \
                                 ((CHANNEL) == ADC_Channel_10) || ((CHANNEL) == ADC_Channel_11) || \
                                 ((CHANNEL) == ADC_Channel_11) || ((CHANNEL) == ADC_Channel_12) || \
                                 ((CHANNEL) == ADC_Channel_13) || ((CHANNEL) == ADC_Channel_14) || \
                                 ((CHANNEL) == ADC_Channel_15) || ((CHANNEL) == ADC_Channel_16))

#define IS_ADC_ALL_PERIPH(PERIPH) (((PERIPH) == ADC1) || \
                                   ((PERIPH) == ADC2))

/**
 * @brief ADC_mode
 */
#define ADC_Mode_Independent             ((uint32_t)0x0000) /**< Independent mode */
#define ADC_Mode_RegInjectSimult         ((uint32_t)0x0001) /**< Combined regular simultaneous + injected simultaneous mode*/
#define ADC_Mode_RegSimult_AlterTrig     ((uint32_t)0x0002) /**< Combined regular simultaneous + alternate trigger mode*/
#define ADC_Mode_InjecSimult_FastInterl  ((uint32_t)0x0003) /**< Combined injected simultaneous + fast interleaved mode*/
#define ADC_Mode_InjectSimult_SlowInterl ((uint32_t)0x0004) /**< Combined injected simultaneous + slow Interleaved mode*/
#define ADC_Mode_InjectSimult            ((uint32_t)0x0005) /**< Injected simultaneous mode only*/
#define ADC_Mode_RegSimult               ((uint32_t)0x0006) /**< Regular simultaneous mode only*/
#define ADC_Mode_FastInterl              ((uint32_t)0x0007) /**< Fast interleaved mode only*/
#define ADC_Mode_SlowInterl              ((uint32_t)0x0008) /**< Slow interleaved mode only*/
#define ADC_Mode_AlterTrig               ((uint32_t)0x0009) /**< Alternate trigger mode only*/

#define IS_ADC_MODE(MODE) (((MODE) == ADC_Mode_Independent) || \
                           ((MODE) == ADC_Mode_RegInjectSimult) || \
                           ((MODE) == ADC_Mode_RegSimult_AlterTrig) || \
                           ((MODE) == ADC_Mode_InjecSimult_FastInterl) || \
                           ((MODE) == ADC_Mode_InjectSimult_SlowInterl) || \
                           ((MODE) == ADC_Mode_InjectSimult) || \
                           ((MODE) == ADC_Mode_RegSimult) || \
                           ((MODE) == ADC_Mode_FastInterl) || \
                           ((MODE) == ADC_Mode_SlowInterl) || \
                           ((MODE) == ADC_Mode_AlterTrig))

#define ADC_ExternalTrigConv_T1_CC1             ((uint32_t)0x000000)
#define ADC_ExternalTrigConv_T1_CC2             ((uint32_t)0x020000)
#define ADC_ExternalTrigConv_T2_CC2             ((uint32_t)0x060000)
#define ADC_ExternalTrigConv_T3_TRGO            ((uint32_t)0x080000)
#define ADC_ExternalTrigConv_T4_CC4             ((uint32_t)0x0A0000)
#define ADC_ExternalTrigConv_Ext_IT11_TIM8_TRGO ((uint32_t)0x0C0000)

#define ADC_ExternalTrigConv_T1_CC3 ((uint32_t)0x00040000)
#define ADC_ExternalTrigConv_None   ((uint32_t)0x000E0000)

#define IS_ADC_EXT_TRIG(REGTRIG) (((REGTRIG) == ADC_ExternalTrigConv_T1_CC1) || \
                                  ((REGTRIG) == ADC_ExternalTrigConv_T1_CC2) || \
                                  ((REGTRIG) == ADC_ExternalTrigConv_T1_CC3) || \
                                  ((REGTRIG) == ADC_ExternalTrigConv_T2_CC2) || \
                                  ((REGTRIG) == ADC_ExternalTrigConv_T3_TRGO) || \
                                  ((REGTRIG) == ADC_ExternalTrigConv_T4_CC4) || \
                                  ((REGTRIG) == ADC_ExternalTrigConv_Ext_IT11_TIM8_TRGO) || \
                                  ((REGTRIG) == ADC_ExternalTrigConv_None) || \
                                  ((REGTRIG) == ADC_ExternalTrigConv_T3_CC1) || \
                                  ((REGTRIG) == ADC_ExternalTrigConv_T2_CC3) || \
                                  ((REGTRIG) == ADC_ExternalTrigConv_T8_CC1) || \
                                  ((REGTRIG) == ADC_ExternalTrigConv_T8_TRGO) || \
                                  ((REGTRIG) == ADC_ExternalTrigConv_T5_CC1) || \
                                  ((REGTRIG) == ADC_ExternalTrigConv_T5_CC3))

/** @defgroup ADC_sampling_time 
  * @{
  */

#define ADC_SampleTime_1Cycles   ((uint8_t)0x00)
#define ADC_SampleTime_7Cycles   ((uint8_t)0x01)
#define ADC_SampleTime_13Cycles  ((uint8_t)0x02)
#define ADC_SampleTime_28Cycles  ((uint8_t)0x03)
#define ADC_SampleTime_41Cycles  ((uint8_t)0x04)
#define ADC_SampleTime_55Cycles  ((uint8_t)0x05)
#define ADC_SampleTime_71Cycles  ((uint8_t)0x06)
#define ADC_SampleTime_239Cycles ((uint8_t)0x07)
#define IS_ADC_SAMPLE_TIME(TIME) (((TIME) == ADC_SampleTime_1Cycles) || \
                                  ((TIME) == ADC_SampleTime_7Cycles) || \
                                  ((TIME) == ADC_SampleTime_13Cycles) || \
                                  ((TIME) == ADC_SampleTime_28Cycles) || \
                                  ((TIME) == ADC_SampleTime_41Cycles) || \
                                  ((TIME) == ADC_SampleTime_55Cycles) || \
                                  ((TIME) == ADC_SampleTime_71Cycles) || \
                                  ((TIME) == ADC_SampleTime_239Cycles))

/**
 * @brief ADC_interrupts_definition 
 */
#define ADC_IT_EOC  ((uint16_t)0x0220)
#define ADC_IT_AWD  ((uint16_t)0x0140)
#define ADC_IT_JEOC ((uint16_t)0x0480)

#define IS_ADC_IT(IT) ((((IT) & (uint16_t)0xF81F) == 0x00) && ((IT) != 0x00))

#define IS_ADC_GET_IT(IT) (((IT) == ADC_IT_EOC) || ((IT) == ADC_IT_AWD) || \
                           ((IT) == ADC_IT_JEOC))

/** @defgroup ADC_flags_definition 
  * @{
  */

#define ADC_FLAG_AWD            ((uint8_t)0x01)
#define ADC_FLAG_EOC            ((uint8_t)0x02)
#define ADC_FLAG_JEOC           ((uint8_t)0x04)
#define ADC_FLAG_JSTRT          ((uint8_t)0x08)
#define ADC_FLAG_STRT           ((uint8_t)0x10)
#define IS_ADC_CLEAR_FLAG(FLAG) ((((FLAG) & (uint8_t)0xE0) == 0x00) && ((FLAG) != 0x00))
#define IS_ADC_GET_FLAG(FLAG)   (((FLAG) == ADC_FLAG_AWD) || ((FLAG) == ADC_FLAG_EOC) || \
                               ((FLAG) == ADC_FLAG_JEOC) || ((FLAG) == ADC_FLAG_JSTRT) || \
                               ((FLAG) == ADC_FLAG_STRT))

/** @defgroup ADC_regular_length 
  * @{
  */

#define IS_ADC_REGULAR_LENGTH(LENGTH) (((LENGTH) >= 0x1) && ((LENGTH) <= 0x10))

/** @defgroup ADC_data_align 
  * @{
  */

#define ADC_DataAlign_Right      ((uint32_t)0x00000000)
#define ADC_DataAlign_Left       ((uint32_t)0x00000800)
#define IS_ADC_DATA_ALIGN(ALIGN) (((ALIGN) == ADC_DataAlign_Right) || \
                                  ((ALIGN) == ADC_DataAlign_Left))

/** @defgroup ADC_injected_rank 
  * @{
  */

#define IS_ADC_INJECTED_RANK(RANK) (((RANK) >= 0x1) && ((RANK) <= 0x4))
/**
 * @brief ADC sampling_time
 */
#define ADC_SampleTime_1Cycles   ((uint8_t)0x00)
#define ADC_SampleTime_7Cycles   ((uint8_t)0x01)
#define ADC_SampleTime_13Cycles  ((uint8_t)0x02)
#define ADC_SampleTime_28Cycles  ((uint8_t)0x03)
#define ADC_SampleTime_41Cycles  ((uint8_t)0x04)
#define ADC_SampleTime_55Cycles  ((uint8_t)0x05)
#define ADC_SampleTime_71Cycles  ((uint8_t)0x06)
#define ADC_SampleTime_239Cycles ((uint8_t)0x07)

#define IS_ADC_SAMPLE_TIME(TIME) (((TIME) == ADC_SampleTime_1Cycles) || \
                                  ((TIME) == ADC_SampleTime_7Cycles) || \
                                  ((TIME) == ADC_SampleTime_13Cycles) || \
                                  ((TIME) == ADC_SampleTime_28Cycles) || \
                                  ((TIME) == ADC_SampleTime_41Cycles) || \
                                  ((TIME) == ADC_SampleTime_55Cycles) || \
                                  ((TIME) == ADC_SampleTime_71Cycles) || \
                                  ((TIME) == ADC_SampleTime_239Cycles))

/**
 * @brief ADC_analog_watchdog_selection 
 */
#define ADC_AnalogWatchdog_SingleRegEnable        ((uint32_t)0x00800200)
#define ADC_AnalogWatchdog_SingleInjecEnable      ((uint32_t)0x00400200)
#define ADC_AnalogWatchdog_SingleRegOrInjecEnable ((uint32_t)0x00C00200)
#define ADC_AnalogWatchdog_AllRegEnable           ((uint32_t)0x00800000)
#define ADC_AnalogWatchdog_AllInjecEnable         ((uint32_t)0x00400000)
#define ADC_AnalogWatchdog_AllRegAllInjecEnable   ((uint32_t)0x00C00000)
#define ADC_AnalogWatchdog_None                   ((uint32_t)0x00000000)

#define IS_ADC_ANALOG_WATCHDOG(WATCHDOG) (((WATCHDOG) == ADC_AnalogWatchdog_SingleRegEnable) || \
                                          ((WATCHDOG) == ADC_AnalogWatchdog_SingleInjecEnable) || \
                                          ((WATCHDOG) == ADC_AnalogWatchdog_SingleRegOrInjecEnable) || \
                                          ((WATCHDOG) == ADC_AnalogWatchdog_AllRegEnable) || \
                                          ((WATCHDOG) == ADC_AnalogWatchdog_AllInjecEnable) || \
                                          ((WATCHDOG) == ADC_AnalogWatchdog_AllRegAllInjecEnable) || \
                                          ((WATCHDOG) == ADC_AnalogWatchdog_None))

/* ADC DISCNUM mask */
#define CR1_DISCNUM_Reset ((uint32_t)0xFFFF1FFF)
/* ADC DISCEN mask */
#define CR1_DISCEN_Set   ((uint32_t)0x00000800)
#define CR1_DISCEN_Reset ((uint32_t)0xFFFFF7FF)

/* ADC JAUTO mask */
#define CR1_JAUTO_Set   ((uint32_t)0x00000400)
#define CR1_JAUTO_Reset ((uint32_t)0xFFFFFBFF)

/* ADC JDISCEN mask */
#define CR1_JDISCEN_Set   ((uint32_t)0x00001000)
#define CR1_JDISCEN_Reset ((uint32_t)0xFFFFEFFF)

/* ADC AWDCH mask */
#define CR1_AWDCH_Reset ((uint32_t)0xFFFFFFE0)

/* ADC Analog watchdog enable mode mask */
#define CR1_AWDMode_Reset ((uint32_t)0xFF3FFDFF)

/* CR1 register Mask */
#define CR1_CLEAR_Mask ((uint32_t)0xFFF0FEFF)

/* ADC ADON mask */
#define CR2_ADON_Set   ((uint32_t)0x00000001)
#define CR2_ADON_Reset ((uint32_t)0xFFFFFFFE)

/* ADC DMA mask */
#define CR2_DMA_Set   ((uint32_t)0x00000100)
#define CR2_DMA_Reset ((uint32_t)0xFFFFFEFF)

/* ADC RSTCAL mask */
#define CR2_RSTCAL_Set ((uint32_t)0x00000008)

/* ADC CAL mask */
#define CR2_CAL_Set ((uint32_t)0x00000004)

/* ADC SWSTART mask */
#define CR2_SWSTART_Set ((uint32_t)0x00400000)

/* ADC EXTTRIG mask */
#define CR2_EXTTRIG_Set   ((uint32_t)0x00100000)
#define CR2_EXTTRIG_Reset ((uint32_t)0xFFEFFFFF)

/* ADC Software start mask */
#define CR2_EXTTRIG_SWSTART_Set   ((uint32_t)0x00500000)
#define CR2_EXTTRIG_SWSTART_Reset ((uint32_t)0xFFAFFFFF)

/* ADC JEXTSEL mask */
#define CR2_JEXTSEL_Reset ((uint32_t)0xFFFF8FFF)

/* ADC JEXTTRIG mask */
#define CR2_JEXTTRIG_Set   ((uint32_t)0x00008000)
#define CR2_JEXTTRIG_Reset ((uint32_t)0xFFFF7FFF)

/* ADC JSWSTART mask */
#define CR2_JSWSTART_Set ((uint32_t)0x00200000)

/* ADC injected software start mask */
#define CR2_JEXTTRIG_JSWSTART_Set   ((uint32_t)0x00208000)
#define CR2_JEXTTRIG_JSWSTART_Reset ((uint32_t)0xFFDF7FFF)

/* ADC TSPD mask */
#define CR2_TSVREFE_Set   ((uint32_t)0x00800000)
#define CR2_TSVREFE_Reset ((uint32_t)0xFF7FFFFF)

/* CR2 register Mask */
#define CR2_CLEAR_Mask ((uint32_t)0xFFF1F7FD)

/* ADC SQx mask */
#define SQR3_SQ_Set ((uint32_t)0x0000001F)
#define SQR2_SQ_Set ((uint32_t)0x0000001F)
#define SQR1_SQ_Set ((uint32_t)0x0000001F)

/* SQR1 register Mask */
#define SQR1_CLEAR_Mask ((uint32_t)0xFF0FFFFF)

/* ADC JSQx mask */
#define JSQR_JSQ_Set ((uint32_t)0x0000001F)

/* ADC JL mask */
#define JSQR_JL_Set   ((uint32_t)0x00300000)
#define JSQR_JL_Reset ((uint32_t)0xFFCFFFFF)

/* ADC SMPx mask */
#define SMPR1_SMP_Set ((uint32_t)0x00000007)
#define SMPR2_SMP_Set ((uint32_t)0x00000007)

/* ADC JDRx registers offset */
#define JDR_Offset ((uint8_t)0x28)

/* ADC1 DR register base address */
#define DR_ADDRESS ((uint32_t)0x4001244C)

/**
  * @brief Deinitializes the ADCx peripheral registers to their default reset values.
  * @param[in] ADCx : where x can be 1 or 2 to select the ADC peripheral.
  */
void ADC_DeInit(ADC_TypeDef *ADCx);

/**
  * @brief Initializes the ADCx peripheral according to the specified parameters in the ADC_InitStruct.
  * @param[in] ADCx : where x can be 1 or 2 to select the ADC peripheral.
  * @param[in] ADC_InitStruct: pointer to an ADC_InitTypeDef structure that contains the configuration information for the specified ADC peripheral.
  */
void ADC_Init(ADC_TypeDef *ADCx, ADC_InitTypeDef *ADC_InitStruct);

/**
 * @brief Fills each ADC_InitStruct member with its default value.
 * @param[in] ADC_InitStruct:pointer to an ADC_InitTypeDef structure which will be initialized.
 * @retval None.
 */
void ADC_StructInit(ADC_InitTypeDef *ADC_InitStruct);

/**
 * @brief Enables or disables the specified ADC peripheral.
 * @param[in] ADCx :where x can be 1 or 2 to select the ADC peripheral.
 * @param[in] NewState :new state of the ADCx peripheral.
 */
void ADC_Cmd(ADC_TypeDef *ADCx, FunctionalState NewState);

/**
  * @brief Enables or disables the specified ADC DMA request.
  * @param[in] ADCx : where x can be 1 or 2 to select the ADC peripheral.
  * @param[in] NewState: new state of the selected ADC DMA transfer.
  */
void ADC_DMACmd(ADC_TypeDef *ADCx, FunctionalState NewState);

/**
 * @brief Enables or disables the specified ADC interrupts.
 * @param[in] ADCx :where x can be 1 or 2 to select the ADC peripheral. 
 * @param[in] ADC_IT:specifies the ADC interrupt sources to be enabled or disabled.
 * @param[in] NewState:new state of the specified ADC interrupts.
 */
void ADC_ITConfig(ADC_TypeDef *ADCx, uint16_t ADC_IT, FunctionalState NewState);

/**
  * @brief Resets the selected ADC calibration registers.
  * @param[in] ADCx : where x can be 1 or 2 to select the ADC peripheral.
  */
void ADC_ResetCalibration(ADC_TypeDef *ADCx);

/**
  * @brief Gets the selected ADC reset calibration registers status.
  * @param[in] ADCx : where x can be 1 or 2 to select the ADC peripheral.
  * @return FlagStatus :The new state of ADC reset calibration registers (SET or RESET).
  */
FlagStatus ADC_GetResetCalibrationStatus(ADC_TypeDef *ADCx);

/**
 * @brief Starts the selected ADC calibration process.
 * @param[in] ADCx:where x can be 1 or 2.
 */
void ADC_StartCalibration(ADC_TypeDef *ADCx);

/**
 * @brief Gets the selected ADC calibration status.
 * @param[in] ADCx:where x can be 1 or 2.
 * @return FlagStatus :the new state of ADC calibration(SET or RESET).
 */
FlagStatus ADC_GetCalibrationStatus(ADC_TypeDef *ADCx);

/**
 * @brief Enables or disables the selected ADC software start conversion.
 * @param[in] ADCx :where x can be 1 or 2.
 * @param[in] NewState:new state of the selected ADC software start conversion.
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
 * @param[in] ADCx : where x can be 1 or 2 to select the ADC peripheral.
 * @param[in] Number: specifies the discontinuous mode regular channel
  *         count value. This number must be between 1 and 8.
 */
void ADC_DiscModeChannelCountConfig(ADC_TypeDef *ADCx, uint8_t Number);

/**
 * @brief Enables or disables the discontinuous mode on regular group channel for the specified ADC.
 * @param[in] ADCx : where x can be 1 or 2 to select the ADC peripheral.
 * @param[in] NewState : new state of the selected ADC discontinuous mode on regular group channel.
 */
void ADC_DiscModeCmd(ADC_TypeDef *ADCx, FunctionalState NewState);

/**
  * @brief :Configures for the selected ADC regular channel its corresponding
  *         rank in the sequencer and its sample time.
  * @param[in] ADCx : where x can be 1 or 2 to select the ADC peripheral.
  * @param[in] ADC_Channel: the ADC channel to configure. 
  * @param[in] Rank : The rank in the regular group sequencer. This parameter must be between 1 to 16.
  * @param[in] ADC_SampleTime: The sample time value to be set for the selected channel. 
  */
void ADC_RegularChannelConfig(ADC_TypeDef *ADCx, uint8_t ADC_Channel, uint8_t Rank, uint8_t ADC_SampleTime);

/**
 * @brief : Enables or disables the ADCx conversion through external trigger.
 * @param[in] ADCx : where x can be 1 or 2 to select the ADC peripheral.
 * @param[in] NewState: new state of the selected ADC external trigger start of conversion.
 */
void ADC_ExternalTrigConvCmd(ADC_TypeDef *ADCx, FunctionalState NewState);

/**
  * @brief Returns the last ADCx conversion result data for regular channel.
  * @param[in] ADCx : where x can be 1 or 2 to select the ADC peripheral.
  * @return uint16_t : The Data conversion value.
  */
uint16_t ADC_GetConversionValue(ADC_TypeDef *ADCx);

/**
  * @brief Returns the last ADC1 and ADC2 conversion result data in dual mode.
  * @return uint32_t  The Data conversion value.
  */
uint32_t ADC_GetDualModeConversionValue(void);

/**
  * @brief Enables or disables the selected ADC automatic injected group conversion after regular one.
  * @param[in] ADCx : where x can be 1 or 2 to select the ADC peripheral.
  * @param[in] NewState: new state of the selected ADC auto injected conversion.
  */
void ADC_AutoInjectedConvCmd(ADC_TypeDef *ADCx, FunctionalState NewState);

/**
  * @brief Enables or disables the discontinuous mode for injected group channel for the specified ADC.
  * @param[in] ADCx : where x can be 1 or 2 to select the ADC peripheral.
  * @param[in] NewState: new state of the selected ADC discontinuous mode on injected group channel.
  */
void ADC_InjectedDiscModeCmd(ADC_TypeDef *ADCx, FunctionalState NewState);

/**
  * @brief Configures the ADCx external trigger for injected channels conversion.
  * @param[in] ADCx : where x can be 1 or 2 to select the ADC peripheral.
  * @param[in] ADC_ExternalTrigInjecConv: specifies the ADC trigger to start injected conversion. 
  */
void ADC_ExternalTrigInjectedConvConfig(ADC_TypeDef *ADCx, uint32_t ADC_ExternalTrigInjecConv);

/**
  * @brief Enables or disables the ADCx injected channels conversion through external trigger.
  * @param[in] ADCx : where x can be 1 or 2 to select the ADC peripheral.
  * @param[in] NewState:new state of the selected ADC external trigger start of injected conversion.
  */
void ADC_ExternalTrigInjectedConvCmd(ADC_TypeDef *ADCx, FunctionalState NewState);

/**
  * @brief Enables or disables the selected ADC start of the injected channels conversion.
  * @param[in] ADCx : where x can be 1 or 2 to select the ADC peripheral.
  * @param[in] NewState: new state of the selected ADC software start injected conversion.
  */
void ADC_SoftwareStartInjectedConvCmd(ADC_TypeDef *ADCx, FunctionalState NewState);

/**
  * @brief Gets the selected ADC Software start injected conversion Status.
  * @param[in] ADCx : where x can be 1 or 2 to select the ADC peripheral.
  * @return FlagStatus :The new state of ADC software start injected conversion (SET or RESET).
  */
FlagStatus ADC_GetSoftwareStartInjectedConvCmdStatus(ADC_TypeDef *ADCx);

/**
  * @brief Configures for the selected ADC injected channel its corresponding rank in the sequencer and its sample time.
  * @param[in] ADCx : where x can be 1 or 2 to select the ADC peripheral.
  * @param[in] ADC_Channel: the ADC channel to configure. 
  * @param[in] Rank : The rank in the injected group sequencer. This parameter must be between 1 and 4.
  * @param[in] ADC_SampleTime: The sample time value to be set for the selected channel. 
  */
void ADC_InjectedChannelConfig(ADC_TypeDef *ADCx, uint8_t ADC_Channel, uint8_t Rank, uint8_t ADC_SampleTime);

/**
 * @brief Configures the sequencer length for injected channels
 * @param[in] ADCx : where x can be 1 or 2 to select the ADC peripheral.
 * @param[in] Length:The sequencer length. This parameter must be a number between 1 to 4.
 */
void ADC_InjectedSequencerLengthConfig(ADC_TypeDef *ADCx, uint8_t Length);

/**
  * @brief Set the injected channels conversion value offset
  * @param[in] ADCx : where x can be 1 or 2 to select the ADC peripheral.
  * @param[in] ADC_InjectedChannel: the ADC injected channel to set its offset. 
  * @param[in] Offset: the offset value for the selected ADC injected channel
  */
void ADC_SetInjectedOffset(ADC_TypeDef *ADCx, uint8_t ADC_InjectedChannel, uint16_t Offset);

/**
  * @brief Returns the ADC injected channel conversion result
  * @param[in] ADCx : where x can be 1 or 2 to select the ADC peripheral.
  * @param[in] ADC_InjectedChannel: the converted ADC injected channel.
  * @return uint16_t : The Data conversion value.
  */
uint16_t ADC_GetInjectedConversionValue(ADC_TypeDef *ADCx, uint8_t ADC_InjectedChannel);

/**
  * @brief Enables or disables the analog watchdog on single/all regular or injected channels
  * @param[in] ADCx : where x can be 1 or 2 to select the ADC peripheral.
  * @param[in] ADC_AnalogWatchdog: the ADC analog watchdog configuration.
  */
void ADC_AnalogWatchdogCmd(ADC_TypeDef *ADCx, uint32_t ADC_AnalogWatchdog);

/**
  * @brief Configures the high and low thresholds of the analog watchdog.
  * @param[in] ADCx : where x can be 1 or 2 to select the ADC peripheral.
  * @param[in] HighThreshold: the ADC analog watchdog High threshold value.
  * @param[in] LowThreshold: the ADC analog watchdog Low threshold value.
  */
void ADC_AnalogWatchdogThresholdsConfig(ADC_TypeDef *ADCx, uint16_t HighThreshold, uint16_t LowThreshold);

/**
  * @brief Configures the analog watchdog guarded single channel.
  * @param[in] ADCx : where x can be 1 or 2 to select the ADC peripheral.
  * @param[in] ADC_Channel : the ADC channel to configure for the analog watchdog. 
  */
void ADC_AnalogWatchdogSingleChannelConfig(ADC_TypeDef *ADCx, uint8_t ADC_Channel);

/**
  * @brief Enables or disables the temperature sensor and Vrefint channel.
  * @param[in] NewState: new state of the temperature sensor.
  */
void ADC_TempSensorVrefintCmd(FunctionalState NewState);

/**
  * @brief Checks whether the specified ADC flag is set or not.
  * @param[in] ADCx : where x can be 1 or 2 to select the ADC peripheral.
  * @param[in] ADC_FLAG: specifies the flag to check. 
  * @return FlagStatus  
  */
FlagStatus ADC_GetFlagStatus(ADC_TypeDef *ADCx, uint8_t ADC_FLAG);

/**
  * @brief Clears the ADCx's pending flags.
  * @param[in] ADCx : where x can be 1 or 2 to select the ADC peripheral.
  * @param[in] ADC_FLAG: specifies the flag to clear. 
  */
void ADC_ClearFlag(ADC_TypeDef *ADCx, uint8_t ADC_FLAG);

/**
  * @brief Checks whether the specified ADC interrupt has occurred or not.
  * @param[in] ADCx : where x can be 1 or 2 to select the ADC peripheral.
  * @param[in] ADC_IT: specifies the ADC interrupt source to check. 
  * @return ITStatus  :The new state of ADC_IT (SET or RESET).
  */
ITStatus ADC_GetITStatus(ADC_TypeDef *ADCx, uint16_t ADC_IT);

/**
  * @brief Clears the ADCx's interrupt pending bits.
  * @param[in] ADCx : where x can be 1 or 2 to select the ADC peripheral.
  * @param[in] ADC_IT: specifies the ADC interrupt pending bit to clear.
  */
void ADC_ClearITPendingBit(ADC_TypeDef *ADCx, uint16_t ADC_IT);

#endif
