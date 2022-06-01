/**
 * @file at103_tim.h
 * @brief This file contains all the functions prototypes for the TIM firmware library. \n
 *        How to use this driver? \n
 * (+) At first, use RCC_APB1(2)PeriphClockCmd(...) to enable TIMx Clock. \n
 * (+) Then, use TIM_TimeBaseInit(...) initialize tim parameters, set automatic reloading value, frequency division coefficient, counting mode, etc. \n
 *     For details on the TIM Time Base Init Structure definition, see "TIM_TimeBaseInitTypeDef". \n
 * (+) Use TIM_ITConfig(...) to enable TIM_interrupt sources, and you need to implement the corresponding TIMx service function: TIMx_IRQHandler. \n
 *     For details,you can see TIM_DIER register. \n
 * (+) Finally, use TIM_Cmd(...) to enable TIMx,start TIMx. \n
 * (+) In additional, use TIM_OCxInit(...) to initialize output compare parameters, for details on the TIM Output Compare Init structure definition, see "TIM_OCInitTypeDef". \n
 *     use TIM_ICInit(...) to initialize input capture parameters, for details on the TIM Input Capture Init structure definition, see "TIM_ICInitTypeDef". \n
 *     use TIM_BDTRConfig(...) to initialize BDTR parameters,for details on the BDTR structure definition, see "TIM_BDTRInitTypeDef", this structure is used only with TIM1. \n
 * (+) Of course, you can use TIM_GetFlagStatus(...) to get the state of the TIMx at any time, this is helpful in determining whether the specified TIM flag is set or not.
 * @author liyouchuang (lycdemeng@163.com)
 * @version 1.0
 * @date 2022-03-02
 * @copyright Copyright (c) 2022 Icore, Inc
 */

#ifndef __AT103_TIM_H
#define __AT103_TIM_H

#ifdef __cplusplus
extern "C" {
#endif
#include "at103.h"
/**
 * @brief TIM Time Base Init Structure definition.
 */
typedef struct
{
    uint16_t TIM_Prescaler;         /**< Specifies the prescaler value used to divide the TIM clock. */
    uint16_t TIM_CounterMode;       /**< Specifies the counter mode. */
    uint16_t TIM_Period;            /**< Specifies the period value to be loaded into the active
                                         Auto-Reload Register at the next update event. */
    uint16_t TIM_ClockDivision;     /**< Specifies the clock division. */
    uint8_t  TIM_RepetitionCounter; /**< Specifies the repetition counter value. 
                                         This parameter is valid only for TIM1. */
} TIM_TimeBaseInitTypeDef;

/**
 * @brief  TIM Output Compare Init structure definition.
 */
typedef struct
{
    uint16_t TIM_OCMode;       /**< Specifies the TIM mode. */
    uint16_t TIM_OutputState;  /**< Specifies the TIM Output Compare state. */
    uint16_t TIM_OutputNState; /**< Specifies the TIM complementary Output Compare state.
                                    This parameter is valid only for TIM1. */
    uint16_t TIM_Pulse;        /**< Specifies the pulse value to be loaded into the Capture Compare Register. */
    uint16_t TIM_OCPolarity;   /**< Specifies the output polarity. */
    uint16_t TIM_OCNPolarity;  /**< Specifies the complementary output polarity.
                                    This parameter is valid only for TIM1. */
    uint16_t TIM_OCIdleState;  /**< Specifies the TIM Output Compare pin state during Idle state.
                                    This parameter is valid only for TIM1. */
    uint16_t TIM_OCNIdleState; /**< Specifies the TIM Output Compare pin state during Idle state.
                                    This parameter is valid only for TIM1. */
} TIM_OCInitTypeDef;

/**
 * @brief TIM Input Capture Init structure definition.
 */
typedef struct
{
    uint16_t TIM_Channel;     /**< Specifies the TIM channel. */
    uint16_t TIM_ICPolarity;  /**< Specifies the active edge of the input signal. */
    uint16_t TIM_ICSelection; /**< Specifies the input. */
    uint16_t TIM_ICPrescaler; /**< Specifies the Input Capture Prescaler. */
    uint16_t TIM_ICFilter;    /**< Specifies the input capture filter. */
} TIM_ICInitTypeDef;

/**
 * @brief BDTR structure definition, this structure is used only with TIM1.
 */
typedef struct
{
    uint16_t TIM_OSSRState;       /**< Specifies the Off-State selection used in Run mode. */
    uint16_t TIM_OSSIState;       /**< Specifies the Off-State used in Idle state. */
    uint16_t TIM_LOCKLevel;       /**< Specifies the LOCK level parameters. */
    uint16_t TIM_DeadTime;        /**< Specifies the delay time between the switching-off and the switching-on of the outputs. */
    uint16_t TIM_Break;           /**< Specifies whether the TIM Break input is enabled or not. */
    uint16_t TIM_BreakPolarity;   /**< Specifies the TIM Break Input pin polarity. */
    uint16_t TIM_AutomaticOutput; /**< Specifies whether the TIM Automatic Output feature is enabled or not. */
} TIM_BDTRInitTypeDef;

#define TIM_CR1_TIM_CR1_DIR       ((uint16_t)0x0010) /**< TIM_CR1 register, Direction. */
#define TIM_CR1_TIM_CR1_CMS       ((uint16_t)0x0060) /**< TIM_CR1 register, CMS bits (Center-aligned mode selection). */
#define TIM_CR1_TIM_CR1_CKD       ((uint16_t)0x0300) /**< TIM_CR1 register, CKD bits (clock division). */
#define TIM_CCMR1_TIM_CCMR1_OC1M  ((uint16_t)0x0070) /**< TIM_CCMR1 register, OC1M bits (Output Compare 1 Mode). */
#define TIM_CCMR1_TIM_CCMR1_CC1S  ((uint16_t)0x0003) /**< TIM_CCMR1 register, CC1S bits (Compare 1 Selection). */
#define TIM_CCER_TIM_CCER_CC1E    ((uint16_t)0x0001) /**< TIM_CCER register, Capture/Compare 1 output enable. */
#define TIM_CCER_TIM_CCER_CC1P    ((uint16_t)0x0002) /**< TIM_CCER register, Capture/Compare 1 output Polarity. */
#define TIM_CCER_TIM_CCER_CC1NP   ((uint16_t)0x0008) /**< TIM_CCER register, Capture/Compare 1 Complementary output Polarity. */
#define TIM_CCER_TIM_CCER_CC1NE   ((uint16_t)0x0004) /**< TIM_CCER register, Capture/Compare 1 Complementary output enable. */
#define TIM_CR2_TIM_CR2_OIS1      ((uint16_t)0X0100) /**< TIM_CR2 register, Output Idle state 1 (OC1 output). */
#define TIM_CR2_TIM_CR2_OIS1N     ((uint16_t)0X0200) /**< TIM_CR2 register, Output Idle state 1 (OC1N output). */
#define TIM_CCMR1_TIM_CCMR1_OC2M  ((uint16_t)0x7000) /**< TIM_CCMR1 register, OC2M bits (Output Compare 2 Mode). */
#define TIM_CCMR1_TIM_CCMR1_CC2S  ((uint16_t)0x0300) /**< TIM_CCMR1 register, CC2S bits (Capture/Compare 2 Selection). */
#define TIM_CCER_TIM_CCER_CC2E    ((uint16_t)0x0010) /**< TIM_CCER register, Capture/Compare 2 output enable. */
#define TIM_CCER_TIM_CCER_CC2P    ((uint16_t)0x0020) /**< TIM_CCER register, Capture/Compare 2 output Polarity. */
#define TIM_CCER_TIM_CCER_CC2NP   ((uint16_t)0x0080) /**< TIM_CCER register, Capture/Compare 2 Complementary output Polarity. */
#define TIM_CCER_TIM_CCER_CC2NE   ((uint16_t)0x0040) /**< TIM_CCER register, Capture/Compare 2 Complementary output enable. */
#define TIM_CR2_TIM_CR2_OIS2      ((uint16_t)0X0400) /**< TIM_CR2 register, Output Idle state 2 (OC2 output). */
#define TIM_CR2_TIM_CR2_OIS2N     ((uint16_t)0X0800) /**< TIM_CR2 register, Output Idle state 2 (OC2N output). */
#define TIM_CCMR2_TIM_CCMR2_OC3M  ((uint16_t)0x0070) /**< TIM_CCMR2 register, OC3M bits (Output Compare 3 Mode). */
#define TIM_CCMR2_TIM_CCMR2_CC3S  ((uint16_t)0x0003) /**< TIM_CCMR2 register, CC3S bits (Capture/Compare 3 Selection). */
#define TIM_CCER_TIM_CCER_CC3E    ((uint16_t)0x0100) /**< TIM_CCER register, Capture/Compare 3 output enable. */
#define TIM_CCER_TIM_CCER_CC3P    ((uint16_t)0x0200) /**< TIM_CCER register, Capture/Compare 3 output Polarity. */
#define TIM_CCER_TIM_CCER_CC3NP   ((uint16_t)0x0800) /**< TIM_CCER register, Capture/Compare 3 Complementary output Polarity. */
#define TIM_CCER_TIM_CCER_CC3NE   ((uint16_t)0x0400) /**< TIM_CCER register, Capture/Compare 3 Complementary output enable. */
#define TIM_CR2_TIM_CR2_OIS3      ((uint16_t)0X1000) /**< TIM_CR2 register, Output Idle state 3 (OC3 output). */
#define TIM_CR2_TIM_CR2_OIS3N     ((uint16_t)0X2000) /**< TIM_CR2 register, Output Idle state 3 (OC3N output). */
#define TIM_CCMR2_TIM_CCMR2_OC4M  ((uint16_t)0x7000) /**< TIM_CCMR2 register, OC4M bits (Output Compare 4 Mode). */
#define TIM_CCMR2_TIM_CCMR2_CC4S  ((uint16_t)0x0300) /**< TIM_CCMR2 register, CC4S bits (Capture/Compare 4 Selection). */
#define TIM_CCER_TIM_CCER_CC4E    ((uint16_t)0x1000) /**< TIM_CCER register, Capture/Compare 4 output enable. */
#define TIM_CCER_TIM_CCER_CC4P    ((uint16_t)0x2000) /**< TIM_CCER register, Capture/Compare 4 output Polarity. */
#define TIM_CR2_TIM_CR2_OIS4      ((uint16_t)0X4000) /**< TIM_CR2 register, Output Idle state 4 (OC4 output). */
#define TIM_SMCR_TIM_SMCR_SMS     ((uint16_t)0x0007) /**< TIM_SMCR register, SMS bits (Slave mode selection). */
#define TIM_SMCR_TIM_SMCR_TS      ((uint16_t)0x0070) /**< TIM_SMCR register, TS bits (Trigger selection). */
#define TIM_CCMR1_TIM_CCMR1_CC1S2 ((uint16_t)0x0003) /**< TIM_CCMR1 register, CC1S bits (Capture 1 Selection). */
#define TIM_CCMR1_CC1S_0          ((uint16_t)0x0001) /**< TIM_CCMR1 register, CC1S bit 0. */
#define TIM_CCMR1_CC2S_0          ((uint16_t)0x0100) /**< TIM_CCMR1 register, CC2S bit 0. */
#define TIM_CCMR1_TIM_CCMR1_CC2S2 ((uint16_t)0x0300) /**< TIM_CCMR1 register, CC2S bits (Capture 2 Selection). */
#define TIM_CCMR1_TIM_CCMR1_OC1PE ((uint16_t)0x0008) /**< TIM_CCMR1 register, Output Compare 1 Preload enable. */
#define TIM_CCMR1_TIM_CCMR1_OC2PE ((uint16_t)0x0800) /**< TIM_CCMR1 register, Output Compare 2 Preload enable. */
#define TIM_CCMR2_TIM_CCMR2_OC3PE ((uint16_t)0x0008) /**< TIM_CCMR2 register, Output Compare 3 Preload enable. */
#define TIM_CCMR2_TIM_CCMR2_OC4PE ((uint16_t)0x0800) /**< TIM_CCMR2 register, Output Compare 3 Preload enable. */
#define TIM_CCMR1_TIM_CCMR1_OC1FE ((uint16_t)0x0004) /**< TIM_CCMR1 register, Output Compare 1 Fast enable. */
#define TIM_CCMR1_TIM_CCMR1_OC2FE ((uint16_t)0x0400) /**< TIM_CCMR1 register, Output Compare 2 Fast enable. */
#define TIM_CCMR2_TIM_CCMR2_OC3FE ((uint16_t)0x0004) /**< TIM_CCMR2 register, Output Compare 3 Fast enable. */
#define TIM_CCMR2_TIM_CCMR2_OC4FE ((uint16_t)0x0400) /**< TIM_CCMR2 register, Output Compare 4 Fast enable. */
#define TIM_CCMR1_TIM_CCMR1_OC1CE ((uint16_t)0x0080) /**< TIM_CCMR1 register, Output Compare 1 Clear Enable. */
#define TIM_CCMR1_TIM_CCMR1_OC2CE ((uint16_t)0x8000) /**< TIM_CCMR1 register, Output Compare 2 Clear Enable. */
#define TIM_CCMR2_TIM_CCMR2_OC3CE ((uint16_t)0x0080) /**< TIM_CCMR2 register, Output Compare 3 Clear Enable. */
#define TIM_CCMR2_TIM_CCMR2_OC4CE ((uint16_t)0x8000) /**< TIM_CCMR2 register, Output Compare 4 Clear Enable. */
#define TIM_CCMR1_TIM_CCMR1_IC1F  ((uint16_t)0x00F0) /**< TIM_CCMR1 register, IC1F bits (Input Capture 1 Filter). */
#define TIM_CCMR1_TIM_CCMR1_IC2F  ((uint16_t)0xF000) /**< TIM_CCMR1 register, IC2F bits (Input Capture 2 Filter). */
#define TIM_CCMR2_TIM_CCMR2_IC3F  ((uint16_t)0x00F0) /**< TIM_CCMR2 register, IC3F bits (Input Capture 3 Filter). */
#define TIM_CCMR2_TIM_CCMR2_IC4F  ((uint16_t)0xF000) /**< TIM_CCMR2 register, IC4F bits (Input Capture 4 Filter). */
#define TIM_CCMR2_TIM_CCMR2_CC3S2 ((uint16_t)0x0003) /**< TIM_CCMR2 register, CC3S bits (Capture 3 Selection). */
#define TIM_CCMR2_TIM_CCMR2_CC4S2 ((uint16_t)0x0300) /**< TIM_CCMR2 register, CC4S bits (Capture 4 Selection). */

#define TIM_OCMode_Timing   ((uint16_t)0x0000) /**< Output Compare Mode: Frozen. */
#define TIM_OCMode_Active   ((uint16_t)0x0010) /**< Output Compare Mode: Set channel x to active level on match. */
#define TIM_OCMode_Inactive ((uint16_t)0x0020) /**< Output Compare Mode: Set channel x to inactive level on match. */
#define TIM_OCMode_Toggle   ((uint16_t)0x0030) /**< Output Compare Mode: Toggle. */
#define TIM_OCMode_PWM1     ((uint16_t)0x0060) /**< Output Compare Mode: PWM mode 1. */
#define TIM_OCMode_PWM2     ((uint16_t)0x0070) /**< Output Compare Mode: PWM mode 2. */

#define TIM_OPMode_Single     ((uint16_t)0x0008) /**< One pulse mode: Counter stops counting at the next update event. */
#define TIM_OPMode_Repetitive ((uint16_t)0x0000) /**< One pulse mode: Counter is not stopped at update event. */

#define TIM_Channel_1 ((uint16_t)0x0000) /**< TIMx Channel 1. */
#define TIM_Channel_2 ((uint16_t)0x0004) /**< TIMx Channel 2. */
#define TIM_Channel_3 ((uint16_t)0x0008) /**< TIMx Channel 3. */
#define TIM_Channel_4 ((uint16_t)0x000C) /**< TIMx Channel 4. */

#define TIM_CKD_DIV1 ((uint16_t)0x0000) /**< TIM Clock division: tDTS = tCK_INT. */
#define TIM_CKD_DIV2 ((uint16_t)0x0100) /**< TIM Clock division: tDTS = 2 × tCK_INT. */
#define TIM_CKD_DIV4 ((uint16_t)0x0200) /**< TIM Clock division: tDTS = 4 × tCK_INT. */

#define TIM_CounterMode_Up             ((uint16_t)0x0000) /**< Direction: Counter used as upcounter. */
#define TIM_CounterMode_Down           ((uint16_t)0x0010) /**< Direction: Counter used as downcounter. */
#define TIM_CounterMode_CenterAligned1 ((uint16_t)0x0020) /**< Center-aligned mode selection: Center-aligned mode 1. */
#define TIM_CounterMode_CenterAligned2 ((uint16_t)0x0040) /**< Center-aligned mode selection: Center-aligned mode 2. */
#define TIM_CounterMode_CenterAligned3 ((uint16_t)0x0060) /**< Center-aligned mode selection: Center-aligned mode 3. */

#define TIM_OCPolarity_High ((uint16_t)0x0000) /**< output Compare polarity: High. */
#define TIM_OCPolarity_Low  ((uint16_t)0x0002) /**< output Compare polarity: Low. */

#define TIM_OCNPolarity_High ((uint16_t)0x0000) /**< output Compare N polarity: High. */
#define TIM_OCNPolarity_Low  ((uint16_t)0x0008) /**< output Compare N polarity: Low. */

#define TIM_OutputState_Disable ((uint16_t)0x0000) /**< output Compare state: Disable. */
#define TIM_OutputState_Enable  ((uint16_t)0x0001) /**< output Compare state: Enable. */

#define TIM_OutputNState_Disable ((uint16_t)0x0000) /**< output Compare N state: Disable. */
#define TIM_OutputNState_Enable  ((uint16_t)0x0004) /**< output Compare N state: Enable. */

#define TIM_CCx_Enable  ((uint16_t)0x0001) /**< TIM Capture/Compare state: Enable. */
#define TIM_CCx_Disable ((uint16_t)0x0000) /**< TIM Capture/Compare state: Disable. */

#define TIM_CCxN_Enable  ((uint16_t)0x0004) /**< TIM Capture/Compare N state: Enable. */
#define TIM_CCxN_Disable ((uint16_t)0x0000) /**< TIM Capture/Compare N state: Disable. */

#define TIM_Break_Enable  ((uint16_t)0x1000) /**< TIM Break Input enable. */
#define TIM_Break_Disable ((uint16_t)0x0000) /**< TIM Break Input disable. */

#define TIM_BreakPolarity_Low  ((uint16_t)0x0000) /**< TIM Break polarity: Active low. */
#define TIM_BreakPolarity_High ((uint16_t)0x2000) /**< TIM Break polarity: Active high. */

#define TIM_AutomaticOutput_Enable  ((uint16_t)0x4000) /**< TIM Automatic output enable. */
#define TIM_AutomaticOutput_Disable ((uint16_t)0x0000) /**< TIM Automatic output disable. */

#define TIM_LOCKLevel_OFF ((uint16_t)0x0000) /**< Lock configuration: LOCK OFF. */
#define TIM_LOCKLevel_1   ((uint16_t)0x0100) /**< Lock configuration: LOCK Level 1. */
#define TIM_LOCKLevel_2   ((uint16_t)0x0200) /**< Lock configuration: LOCK Level 2. */
#define TIM_LOCKLevel_3   ((uint16_t)0x0300) /**< Lock configuration: LOCK Level 3. */

#define TIM_OSSIState_Enable  ((uint16_t)0x0400) /**< Off-state selection for Idle mode enable. */
#define TIM_OSSIState_Disable ((uint16_t)0x0000) /**< Off-state selection for Idle mode disable. */

#define TIM_OSSRState_Enable  ((uint16_t)0x0800) /**< Off-state selection for Run mode enable. */
#define TIM_OSSRState_Disable ((uint16_t)0x0000) /**< Off-state selection for Run mode disable. */

#define TIM_OCIdleState_Set   ((uint16_t)0x0100) /**< Set TIM Output Compare Idle State. */
#define TIM_OCIdleState_Reset ((uint16_t)0x0000) /**< Reset TIM Output Compare Idle State. */

#define TIM_OCNIdleState_Set   ((uint16_t)0x0200) /**< Set TIM Output Compare N Idle State. */
#define TIM_OCNIdleState_Reset ((uint16_t)0x0000) /**< Reset TIM Output Compare N Idle State. */

#define TIM_ICPolarity_Rising   ((uint16_t)0x0000) /**< TIM Input Capture Polarity: Capture is done on a rising edge. */
#define TIM_ICPolarity_Falling  ((uint16_t)0x0002) /**< TIM Input Capture Polarity: Capture is done on a falling edge. */
#define TIM_ICPolarity_BothEdge ((uint16_t)0x000A) /**< TIM Input Capture Polarity: Capture is done on a both edge. */

#define TIM_ICSelection_DirectTI   ((uint16_t)0x0001) /**< TIM Input Capture Selection: IC1 is mapped on TI1. */
#define TIM_ICSelection_IndirectTI ((uint16_t)0x0002) /**< TIM Input Capture Selection: IC1 is mapped on TI2. */
#define TIM_ICSelection_TRC        ((uint16_t)0x0003) /**< TIM Input Capture Selection: IC1 is mapped on TRC. */

#define TIM_ICPSC_DIV1 ((uint16_t)0x0000) /**< Capture performed each time an edge is detected on the capture input. */
#define TIM_ICPSC_DIV2 ((uint16_t)0x0004) /**< Capture performed once every 2 events. */
#define TIM_ICPSC_DIV4 ((uint16_t)0x0008) /**< Capture performed once every 4 events. */
#define TIM_ICPSC_DIV8 ((uint16_t)0x000C) /**< Capture performed once every 8 events. */

#define TIM_IT_Update  ((uint16_t)0x0001) /**< TIM interrupt sources: Update interrupt. */
#define TIM_IT_CC1     ((uint16_t)0x0002) /**< TIM interrupt sources: CC1 interrupt. */
#define TIM_IT_CC2     ((uint16_t)0x0004) /**< TIM interrupt sources: CC2 interrupt. */
#define TIM_IT_CC3     ((uint16_t)0x0008) /**< TIM interrupt sources: CC3 interrupt. */
#define TIM_IT_CC4     ((uint16_t)0x0010) /**< TIM interrupt sources: CC4 interrupt. */
#define TIM_IT_COM     ((uint16_t)0x0020) /**< TIM interrupt sources: COM interrupt. */
#define TIM_IT_Trigger ((uint16_t)0x0040) /**< TIM interrupt sources: Trigger interrupt. */
#define TIM_IT_Break   ((uint16_t)0x0080) /**< TIM interrupt sources: Break interrupt. */

#define TIM_DMABase_CR1   ((uint16_t)0x0000) /**< DMA base address offset of TIMx_CR1 register: TIMx_CR1. */
#define TIM_DMABase_CR2   ((uint16_t)0x0001) /**< DMA base address offset of TIMx_CR1 register: TIMx_CR2. */
#define TIM_DMABase_SMCR  ((uint16_t)0x0002) /**< DMA base address offset of TIMx_CR1 register: TIMx_SMCR. */
#define TIM_DMABase_DIER  ((uint16_t)0x0003) /**< DMA base address offset of TIMx_CR1 register: TIMx_DIER. */
#define TIM_DMABase_SR    ((uint16_t)0x0004) /**< DMA base address offset of TIMx_CR1 register: TIMx_SR. */
#define TIM_DMABase_EGR   ((uint16_t)0x0005) /**< DMA base address offset of TIMx_CR1 register: TIMx_EGR. */
#define TIM_DMABase_CCMR1 ((uint16_t)0x0006) /**< DMA base address offset of TIMx_CR1 register: TIMx_CCMR1. */
#define TIM_DMABase_CCMR2 ((uint16_t)0x0007) /**< DMA base address offset of TIMx_CR1 register: TIMx_CCMR2. */
#define TIM_DMABase_CCER  ((uint16_t)0x0008) /**< DMA base address offset of TIMx_CR1 register: TIMx_CCER. */
#define TIM_DMABase_CNT   ((uint16_t)0x0009) /**< DMA base address offset of TIMx_CR1 register: TIMx_CNT. */
#define TIM_DMABase_PSC   ((uint16_t)0x000A) /**< DMA base address offset of TIMx_CR1 register: TIMx_PSC. */
#define TIM_DMABase_ARR   ((uint16_t)0x000B) /**< DMA base address offset of TIMx_CR1 register: TIMx_ARR. */
#define TIM_DMABase_RCR   ((uint16_t)0x000C) /**< DMA base address offset of TIMx_CR1 register: TIMx_RCR. */
#define TIM_DMABase_CCR1  ((uint16_t)0x000D) /**< DMA base address offset of TIMx_CR1 register: TIMx_CCR1. */
#define TIM_DMABase_CCR2  ((uint16_t)0x000E) /**< DMA base address offset of TIMx_CR1 register: TIMx_CCR2. */
#define TIM_DMABase_CCR3  ((uint16_t)0x000F) /**< DMA base address offset of TIMx_CR1 register: TIMx_CCR3. */
#define TIM_DMABase_CCR4  ((uint16_t)0x0010) /**< DMA base address offset of TIMx_CR1 register: TIMx_CCR4. */
#define TIM_DMABase_BDTR  ((uint16_t)0x0011) /**< DMA base address offset of TIMx_CR1 register: TIMx_BDTR. */
#define TIM_DMABase_DCR   ((uint16_t)0x0012) /**< DMA base address offset of TIMx_CR1 register: TIMx_DCR. */

#define TIM_DMABurstLength_1Transfer   ((uint16_t)0x0000) /**< The number of TIM DMA Burst Length transfers: 1 transfer. */
#define TIM_DMABurstLength_2Transfers  ((uint16_t)0x0100) /**< The number of TIM DMA Burst Length transfers: 2 transfers. */
#define TIM_DMABurstLength_3Transfers  ((uint16_t)0x0200) /**< The number of TIM DMA Burst Length transfers: 3 transfers. */
#define TIM_DMABurstLength_4Transfers  ((uint16_t)0x0300) /**< The number of TIM DMA Burst Length transfers: 4 transfers. */
#define TIM_DMABurstLength_5Transfers  ((uint16_t)0x0400) /**< The number of TIM DMA Burst Length transfers: 5 transfers. */
#define TIM_DMABurstLength_6Transfers  ((uint16_t)0x0500) /**< The number of TIM DMA Burst Length transfers: 6 transfers. */
#define TIM_DMABurstLength_7Transfers  ((uint16_t)0x0600) /**< The number of TIM DMA Burst Length transfers: 7 transfers. */
#define TIM_DMABurstLength_8Transfers  ((uint16_t)0x0700) /**< The number of TIM DMA Burst Length transfers: 8 transfers. */
#define TIM_DMABurstLength_9Transfers  ((uint16_t)0x0800) /**< The number of TIM DMA Burst Length transfers: 9 transfers. */
#define TIM_DMABurstLength_10Transfers ((uint16_t)0x0900) /**< The number of TIM DMA Burst Length transfers: 10 transfers. */
#define TIM_DMABurstLength_11Transfers ((uint16_t)0x0A00) /**< The number of TIM DMA Burst Length transfers: 11 transfers. */
#define TIM_DMABurstLength_12Transfers ((uint16_t)0x0B00) /**< The number of TIM DMA Burst Length transfers: 12 transfers. */
#define TIM_DMABurstLength_13Transfers ((uint16_t)0x0C00) /**< The number of TIM DMA Burst Length transfers: 13 transfers. */
#define TIM_DMABurstLength_14Transfers ((uint16_t)0x0D00) /**< The number of TIM DMA Burst Length transfers: 14 transfers. */
#define TIM_DMABurstLength_15Transfers ((uint16_t)0x0E00) /**< The number of TIM DMA Burst Length transfers: 15 transfers. */
#define TIM_DMABurstLength_16Transfers ((uint16_t)0x0F00) /**< The number of TIM DMA Burst Length transfers: 16 transfers. */
#define TIM_DMABurstLength_17Transfers ((uint16_t)0x1000) /**< The number of TIM DMA Burst Length transfers: 17 transfers. */
#define TIM_DMABurstLength_18Transfers ((uint16_t)0x1100) /**< The number of TIM DMA Burst Length transfers: 18 transfers. */

#define TIM_DMA_Update  ((uint16_t)0x0100) /**< TIM DMA sources: Update DMA request. */
#define TIM_DMA_CC1     ((uint16_t)0x0200) /**< TIM DMA sources: Capture/Compare 1 DMA request. */
#define TIM_DMA_CC2     ((uint16_t)0x0400) /**< TIM DMA sources: Capture/Compare 2 DMA request. */
#define TIM_DMA_CC3     ((uint16_t)0x0800) /**< TIM DMA sources: Capture/Compare 3 DMA request. */
#define TIM_DMA_CC4     ((uint16_t)0x1000) /**< TIM DMA sources: Capture/Compare 4 DMA request. */
#define TIM_DMA_COM     ((uint16_t)0x2000) /**< TIM DMA sources: COM DMA request. */
#define TIM_DMA_Trigger ((uint16_t)0x4000) /**< TIM DMA sources: Trigger DMA request. */

#define TIM_ExtTRGPSC_OFF  ((uint16_t)0x0000) /**< External trigger prescaler: Prescaler OFF. */
#define TIM_ExtTRGPSC_DIV2 ((uint16_t)0x1000) /**< External trigger prescaler: ETRP frequency divided by 2. */
#define TIM_ExtTRGPSC_DIV4 ((uint16_t)0x2000) /**< External trigger prescaler: ETRP frequency divided by 4. */
#define TIM_ExtTRGPSC_DIV8 ((uint16_t)0x3000) /**< External trigger prescaler: ETRP frequency divided by 8. */

#define TIM_TS_ITR0    ((uint16_t)0x0000) /**< TIM Internal Trigger selection: Internal Trigger 0 (ITR0). */
#define TIM_TS_ITR1    ((uint16_t)0x0010) /**< TIM Internal Trigger selection: Internal Trigger 1 (ITR1). */
#define TIM_TS_ITR2    ((uint16_t)0x0020) /**< TIM Internal Trigger selection: Internal Trigger 2 (ITR2). */
#define TIM_TS_ITR3    ((uint16_t)0x0030) /**< TIM Internal Trigger selection: Internal Trigger 3 (ITR3). */
#define TIM_TS_TI1F_ED ((uint16_t)0x0040) /**< TIM Internal Trigger selection: TI1 Edge Detector (TI1F_ED). */
#define TIM_TS_TI1FP1  ((uint16_t)0x0050) /**< TIM Internal Trigger selection: Filtered Timer Input 1 (TI1FP1). */
#define TIM_TS_TI2FP2  ((uint16_t)0x0060) /**< TIM Internal Trigger selection: Filtered Timer Input 2 (TI2FP2). */
#define TIM_TS_ETRF    ((uint16_t)0x0070) /**< TIM Internal Trigger selection: External Trigger input (ETRF). */

#define TIM_TIxExternalCLK1Source_TI1   ((uint16_t)0x0050) /**< TIM TIx External Clock Source: Timer Input 1. */
#define TIM_TIxExternalCLK1Source_TI2   ((uint16_t)0x0060) /**< TIM TIx External Clock Source: Timer Input 2. */
#define TIM_TIxExternalCLK1Source_TI1ED ((uint16_t)0x0040) /**< TIM TIx External Clock Source: TI1 Edge Detector. */

#define TIM_ExtTRGPolarity_Inverted    ((uint16_t)0x8000) /**< External trigger polarity: ETR is inverted, active at low level or falling edge. */
#define TIM_ExtTRGPolarity_NonInverted ((uint16_t)0x0000) /**< External trigger polarity: ETR is non-inverted, active at high level or rising edge. */

#define TIM_PSCReloadMode_Update    ((uint16_t)0x0000) /**< TIM Prescaler Reload Mode Update generation: No action. */
#define TIM_PSCReloadMode_Immediate ((uint16_t)0x0001) /**< Reinitialize the counter and generates an update of the registers. */

#define TIM_ForcedAction_Active   ((uint16_t)0x0050) /**< Force active level - OC1REF is forced high. */
#define TIM_ForcedAction_InActive ((uint16_t)0x0040) /**< Force inactive level - OC1REF is forced low. */

#define TIM_EncoderMode_TI1  ((uint16_t)0x0001) /**< Encoder mode 1 - Counter counts up/down on TI2FP2 edge depending on TI1FP1 level. */
#define TIM_EncoderMode_TI2  ((uint16_t)0x0002) /**< Encoder mode 2 - Counter counts up/down on TI1FP1 edge depending on TI2FP2 level. */
#define TIM_EncoderMode_TI12 ((uint16_t)0x0003) /**< Encoder mode 3 - Counter counts up/down on both TI1FP1 and TI2FP2 edges \
                                                     depending on the level of the other input. */

#define TIM_EventSource_Update  ((uint16_t)0x0001) /**< TIM Event Source: Update generation. */
#define TIM_EventSource_CC1     ((uint16_t)0x0002) /**< TIM Event Source: Capture/Compare 1 generation. */
#define TIM_EventSource_CC2     ((uint16_t)0x0004) /**< TIM Event Source: Capture/Compare 2 generation. */
#define TIM_EventSource_CC3     ((uint16_t)0x0008) /**< TIM Event Source: Capture/Compare 3 generation. */
#define TIM_EventSource_CC4     ((uint16_t)0x0010) /**< TIM Event Source: Capture/Compare 4 generation. */
#define TIM_EventSource_COM     ((uint16_t)0x0020) /**< TIM Event Source: Capture/Compare control update generation. */
#define TIM_EventSource_Trigger ((uint16_t)0x0040) /**< TIM Event Source: Trigger generation. */
#define TIM_EventSource_Break   ((uint16_t)0x0080) /**< TIM Event Source: Break generation. */

#define TIM_UpdateSource_Global  ((uint16_t)0x0000) /**< Source of update is the counter overflow/underflow or the setting of UG bit, or an update generation through the slave mode controller. */
#define TIM_UpdateSource_Regular ((uint16_t)0x0001) /**< Source of update is counter overflow/underflow. */

#define TIM_OCPreload_Enable  ((uint16_t)0x0008) /**< TIM Output Compare Preload State: Enable. */
#define TIM_OCPreload_Disable ((uint16_t)0x0000) /**< TIM Output Compare Preload State: Disable. */

#define TIM_OCFast_Enable  ((uint16_t)0x0004) /**< TIM Output Compare Fast State: Enable. */
#define TIM_OCFast_Disable ((uint16_t)0x0000) /**< TIM Output Compare Fast State: Disable. */

#define TIM_OCClear_Enable  ((uint16_t)0x0080) /**< TIM Output Compare Clear State: Enable. */
#define TIM_OCClear_Disable ((uint16_t)0x0000) /**< TIM Output Compare Clear State: Disable. */

#define TIM_TRGOSource_Reset  ((uint16_t)0x0000) /**< Master mode selection TIM Trigger Output Source: Reset. */
#define TIM_TRGOSource_Enable ((uint16_t)0x0010) /**< Master mode selection TIM Trigger Output Source: Enable. */
#define TIM_TRGOSource_Update ((uint16_t)0x0020) /**< Master mode selection TIM Trigger Output Source: Update. */
#define TIM_TRGOSource_OC1    ((uint16_t)0x0030) /**< Master mode selection TIM Trigger Output Source: Compare Pulse. */
#define TIM_TRGOSource_OC1Ref ((uint16_t)0x0040) /**< Master mode selection TIM Trigger Output Source: OC1REF signal is used as trigger output. */
#define TIM_TRGOSource_OC2Ref ((uint16_t)0x0050) /**< Master mode selection TIM Trigger Output Source: OC2REF signal is used as trigger output. */
#define TIM_TRGOSource_OC3Ref ((uint16_t)0x0060) /**< Master mode selection TIM Trigger Output Source: OC3REF signal is used as trigger output. */
#define TIM_TRGOSource_OC4Ref ((uint16_t)0x0070) /**< Master mode selection TIM Trigger Output Source: OC4REF signal is used as trigger output. */

#define TIM_SlaveMode_Reset     ((uint16_t)0x0004) /**< Slave mode selection: Reset Mode. */
#define TIM_SlaveMode_Gated     ((uint16_t)0x0005) /**< Slave mode selection: Gated Mode. */
#define TIM_SlaveMode_Trigger   ((uint16_t)0x0006) /**< Slave mode selection: Trigger Mode. */
#define TIM_SlaveMode_External1 ((uint16_t)0x0007) /**< Slave mode selection: External Clock Mode 1. */

#define TIM_MasterSlaveMode_Enable  ((uint16_t)0x0080) /**< TIM Master Slave Mode: Enable. */
#define TIM_MasterSlaveMode_Disable ((uint16_t)0x0000) /**< TIM Master Slave Mode: Disable. */

#define TIM_FLAG_Update  ((uint16_t)0x0001) /**< TIM Update flag. */
#define TIM_FLAG_CC1     ((uint16_t)0x0002) /**< TIM Capture/Compare 1 flag. */
#define TIM_FLAG_CC2     ((uint16_t)0x0004) /**< TIM Capture/Compare 2 flag. */
#define TIM_FLAG_CC3     ((uint16_t)0x0008) /**< TIM Capture/Compare 3 flag. */
#define TIM_FLAG_CC4     ((uint16_t)0x0010) /**< TIM Capture/Compare 4 flag. */
#define TIM_FLAG_COM     ((uint16_t)0x0020) /**< TIM COM flag. */
#define TIM_FLAG_Trigger ((uint16_t)0x0040) /**< TIM Trigger flag. */
#define TIM_FLAG_Break   ((uint16_t)0x0080) /**< TIM Break flag. */
#define TIM_FLAG_CC1OF   ((uint16_t)0x0200) /**< TIM Capture/Compare 1 overcapture flag. */
#define TIM_FLAG_CC2OF   ((uint16_t)0x0400) /**< TIM Capture/Compare 2 overcapture flag. */
#define TIM_FLAG_CC3OF   ((uint16_t)0x0800) /**< TIM Capture/Compare 3 overcapture flag. */
#define TIM_FLAG_CC4OF   ((uint16_t)0x1000) /**< TIM Capture/Compare 4 overcapture flag. */

#define TIM_DMABurstLength_1Byte   TIM_DMABurstLength_1Transfer   /**< The number of TIM DMA Burst Length transfers: 1 Byte. */
#define TIM_DMABurstLength_2Bytes  TIM_DMABurstLength_2Transfers  /**< The number of TIM DMA Burst Length transfers: 2 Bytes. */
#define TIM_DMABurstLength_3Bytes  TIM_DMABurstLength_3Transfers  /**< The number of TIM DMA Burst Length transfers: 3 Bytes. */
#define TIM_DMABurstLength_4Bytes  TIM_DMABurstLength_4Transfers  /**< The number of TIM DMA Burst Length transfers: 4 Bytes. */
#define TIM_DMABurstLength_5Bytes  TIM_DMABurstLength_5Transfers  /**< The number of TIM DMA Burst Length transfers: 5 Bytes. */
#define TIM_DMABurstLength_6Bytes  TIM_DMABurstLength_6Transfers  /**< The number of TIM DMA Burst Length transfers: 6 Bytes. */
#define TIM_DMABurstLength_7Bytes  TIM_DMABurstLength_7Transfers  /**< The number of TIM DMA Burst Length transfers: 7 Bytes. */
#define TIM_DMABurstLength_8Bytes  TIM_DMABurstLength_8Transfers  /**< The number of TIM DMA Burst Length transfers: 8 Bytes. */
#define TIM_DMABurstLength_9Bytes  TIM_DMABurstLength_9Transfers  /**< The number of TIM DMA Burst Length transfers: 9 Bytes. */
#define TIM_DMABurstLength_10Bytes TIM_DMABurstLength_10Transfers /**< The number of TIM DMA Burst Length transfers: 10 Bytes. */
#define TIM_DMABurstLength_11Bytes TIM_DMABurstLength_11Transfers /**< The number of TIM DMA Burst Length transfers: 11 Bytes. */
#define TIM_DMABurstLength_12Bytes TIM_DMABurstLength_12Transfers /**< The number of TIM DMA Burst Length transfers: 12 Bytes. */
#define TIM_DMABurstLength_13Bytes TIM_DMABurstLength_13Transfers /**< The number of TIM DMA Burst Length transfers: 13 Bytes. */
#define TIM_DMABurstLength_14Bytes TIM_DMABurstLength_14Transfers /**< The number of TIM DMA Burst Length transfers: 14 Bytes. */
#define TIM_DMABurstLength_15Bytes TIM_DMABurstLength_15Transfers /**< The number of TIM DMA Burst Length transfers: 15 Bytes. */
#define TIM_DMABurstLength_16Bytes TIM_DMABurstLength_16Transfers /**< The number of TIM DMA Burst Length transfers: 16 Bytes. */
#define TIM_DMABurstLength_17Bytes TIM_DMABurstLength_17Transfers /**< The number of TIM DMA Burst Length transfers: 17 Bytes. */
#define TIM_DMABurstLength_18Bytes TIM_DMABurstLength_18Transfers /**< The number of TIM DMA Burst Length transfers: 18 Bytes. */

/**
 * @brief Deinitializes the TIMx peripheral registers to their default reset values.
 * @param[in] TIMx where x can be 1 to 4 to select the TIM peripheral.
 */
void TIM_DeInit(TIM_TypeDef *TIMx);

/**
 * @brief Initializes the TIMx Time Base Unit peripheral according to 
          the specified parameters in the TIM_TimeBaseInitStruct.
 * @param[in] TIMx where x can be 1 to 4 to select the TIM peripheral.
 * @param[in] TIM_TimeBaseInitStruct pointer to a "TIM_TimeBaseInitTypeDef"
 *            structure that contains the configuration information for the specified TIM peripheral.
 */
void TIM_TimeBaseInit(TIM_TypeDef *TIMx, TIM_TimeBaseInitTypeDef *TIM_TimeBaseInitStruct);

/**
 * @brief Initializes the TIMx Channel1 according to the specified parameters in the TIM_OCInitStruct.
 * @param[in] TIMx where x can be 1 to 4 to select the TIM peripheral.
 * @param[in] TIM_OCInitStruct pointer to a "TIM_OCInitTypeDef" structure
 *            that contains the configuration information for the specified TIM peripheral.
 */
void TIM_OC1Init(TIM_TypeDef *TIMx, TIM_OCInitTypeDef *TIM_OCInitStruct);

/**
 * @brief Initializes the TIMx Channel2 according to the specified parameters in the TIM_OCInitStruct.
 * @param[in] TIMx where x can be 1 to 4 to select the TIM peripheral.
 * @param[in] TIM_OCInitStruct pointer to a "TIM_OCInitTypeDef" structure
 *            that contains the configuration information for the specified TIM peripheral.
 */
void TIM_OC2Init(TIM_TypeDef *TIMx, TIM_OCInitTypeDef *TIM_OCInitStruct);

/**
 * @brief Initializes the TIMx Channel3 according to the specified parameters in the TIM_OCInitStruct.
 * @param[in] TIMx where x can be 1 to 4 to select the TIM peripheral.
 * @param[in] TIM_OCInitStruct pointer to a "TIM_OCInitTypeDef" structure
 *            that contains the configuration information for the specified TIM peripheral.
 */
void TIM_OC3Init(TIM_TypeDef *TIMx, TIM_OCInitTypeDef *TIM_OCInitStruct);

/**
 * @brief Initializes the TIMx Channel4 according to the specified parameters in the TIM_OCInitStruct.
 * @param[in] TIMx where x can be 1 to 4 to select the TIM peripheral.
 * @param[in] TIM_OCInitStruct pointer to a "TIM_OCInitTypeDef" structure
 *            that contains the configuration information for the specified TIM peripheral.
 */
void TIM_OC4Init(TIM_TypeDef *TIMx, TIM_OCInitTypeDef *TIM_OCInitStruct);

/**
 * @brief Initializes the TIM peripheral according to the specified parameters in the TIM_ICInitStruct.
 * @param[in] TIMx where x can be 1 to 4 to select the TIM peripheral.
 * @param[in] TIM_ICInitStruct pointer to a "TIM_ICInitTypeDef" structure 
 *            that contains the configuration information for the specified TIM peripheral.
 */
void TIM_ICInit(TIM_TypeDef *TIMx, TIM_ICInitTypeDef *TIM_ICInitStruct);

/**
 * @brief Configures the TIM peripheral according to the specified
 *        parameters in the TIM_ICInitStruct to measure an external PWM signal.
 * @param[in] TIMx where x can be 1 to 4 to select the TIM peripheral.
 * @param[in] TIM_ICInitStruct pointer to a "TIM_ICInitTypeDef" structure
 *            that contains the configuration information for the specified TIM peripheral.
 */
void TIM_PWMIConfig(TIM_TypeDef *TIMx, TIM_ICInitTypeDef *TIM_ICInitStruct);

/**
 * @brief  Configures the: Break feature, dead time, Lock level, the OSSI,
 *         the OSSR State and the AOE(automatic output enable).
 * @param[in] TIMx where x can be 1 to 4 to select the TIM peripheral.
 * @param[in] TIM_BDTRInitStruct pointer to a "TIM_BDTRInitTypeDef" structure that
 *            contains the BDTR Register configuration  information for the TIM peripheral.
 */
void TIM_BDTRConfig(TIM_TypeDef *TIMx, TIM_BDTRInitTypeDef *TIM_BDTRInitStruct);

/**
 * @brief Fills each TIM_TimeBaseInitStruct member with its default value.
 * @param[in] TIM_TimeBaseInitStruct pointer to a "TIM_TimeBaseInitTypeDef" structure which will be initialized.
 */
void TIM_TimeBaseStructInit(TIM_TimeBaseInitTypeDef *TIM_TimeBaseInitStruct);

/**
 * @brief Fills each TIM_OCInitStruct member with its default value.
 * @param[in] TIM_OCInitStruct pointer to a "TIM_OCInitTypeDef" structure which will be initialized.
 */
void TIM_OCStructInit(TIM_OCInitTypeDef *TIM_OCInitStruct);

/**
 * @brief Fills each TIM_ICInitStruct member with its default value.
 * @param[in] TIM_ICInitStruct TIM_ICInitStruct: pointer to a "TIM_ICInitTypeDef" structure which will
 *            be initialized.
 */
void TIM_ICStructInit(TIM_ICInitTypeDef *TIM_ICInitStruct);

/**
 * @brief Fills each TIM_BDTRInitStruct member with its default value.
 * @param[in] TIM_BDTRInitStruct TIM_BDTRInitStruct: pointer to a "TIM_BDTRInitTypeDef" structure which
 *            will be initialized.
 */
void TIM_BDTRStructInit(TIM_BDTRInitTypeDef *TIM_BDTRInitStruct);

/**
 * @brief Enables or disables the specified TIM peripheral.
 * @param[in] TIMx where x can be 1 to 4 to select the TIMx peripheral.
 * @param[in] NewState new state of the TIMx peripheral.
 *            This parameter can be: ENABLE or DISABLE.
 */
void TIM_Cmd(TIM_TypeDef *TIMx, FunctionalState NewState);

/**
 * @brief Enables or disables the TIM peripheral Main Outputs.
 * @param[in] TIMx where x can be 1 to select the TIMx peripheral
 * @param[in] NewState new state of the TIM peripheral Main Outputs.
 *            This parameter can be: ENABLE or DISABLE.
 */
void TIM_CtrlPWMOutputs(TIM_TypeDef *TIMx, FunctionalState NewState);

/**
 * @brief Enables or disables the specified TIM interrupts.
 * @param[in] TIMx where x can be 1 to 4 to select the TIMx peripheral.
 * @param[in] TIM_IT specifies the TIM interrupts sources to be enabled or disabled.
 *            This parameter can be any combination of the following values:
 *            TIM_IT_Update: TIM update Interrupt source;
 *            TIM_IT_CC1: TIM Capture Compare 1 Interrupt source;
 *            TIM_IT_CC2: TIM Capture Compare 2 Interrupt source;
 *            TIM_IT_CC3: TIM Capture Compare 3 Interrupt source;
 *            TIM_IT_CC4: TIM Capture Compare 4 Interrupt source;
 *            TIM_IT_COM: TIM Commutation Interrupt source;
 *            TIM_IT_Trigger: TIM Trigger Interrupt source;
 *            TIM_IT_Break: TIM Break Interrupt source;
 *            TIM_IT_Break and TIM_IT_COM is used only with TIM1.
 * @param[in] NewState new state of the TIM interrupts.
 *            This parameter can be: ENABLE or DISABLE.
 */
void TIM_ITConfig(TIM_TypeDef *TIMx, uint16_t TIM_IT, FunctionalState NewState);

/**
 * @brief Configures the TIMx event to be generate by software.
 * @param[in] TIMx where x can be 1 to 4 to select the TIMx peripheral.
 * @param[in] TIM_EventSource specifies the event source.
 *            This parameter can be one or more of the following values:
 *            TIM_EventSource_Update: Timer update Event source;
 *            TIM_EventSource_CC1: Timer Capture Compare 1 Event source;
 *            TIM_EventSource_CC2: Timer Capture Compare 2 Event source;
 *            TIM_EventSource_CC3: Timer Capture Compare 3 Event source;
 *            TIM_EventSource_CC4: Timer Capture Compare 4 Event source;
 *            TIM_EventSource_COM: Timer COM event source;
 *            TIM_EventSource_Trigger: Timer Trigger Event source;
 *            TIM_EventSource_Break: Timer Break event source.
 *            TIM_EventSource_COM and TIM_EventSource_Break are used only with TIM1.
 */
void TIM_GenerateEvent(TIM_TypeDef *TIMx, uint16_t TIM_EventSource);

/**
 * @brief Configures the TIMx's DMA interface.
 * @param[in] TIMx where x can be 1 to 4 to select the TIMx peripheral.
 * @param[in] TIM_DMABase DMA Base address.
 * @param[in] TIM_DMABurstLength DMA Burst length.
 *            This parameter can be one value between: TIM_DMABurstLength_1Transfer and TIM_DMABurstLength_18Transfers.
 */
void TIM_DMAConfig(TIM_TypeDef *TIMx, uint16_t TIM_DMABase, uint16_t TIM_DMABurstLength);

/**
 * @brief Enables or disables the TIMx's DMA Requests.
 * @param[in] TIMx where x can be 1 to 4 to select the TIMx peripheral.
 * @param[in] TIM_DMASource specifies the DMA Request sources.
 *            This parameter can be any combination of the following values:
 *            TIM_DMA_Update: TIM update Interrupt source;
 *            TIM_DMA_CC1: TIM Capture Compare 1 DMA source;
 *            TIM_DMA_CC2: TIM Capture Compare 2 DMA source;
 *            TIM_DMA_CC3: TIM Capture Compare 3 DMA source;
 *            TIM_DMA_CC4: TIM Capture Compare 4 DMA source;
 *            TIM_DMA_COM: TIM Commutation DMA source;
 *            TIM_DMA_Trigger: TIM Trigger DMA source.
 * @param[in] NewState new state of the DMA Request sources.
 *            This parameter can be: ENABLE or DISABLE.
 */
void TIM_DMACmd(TIM_TypeDef *TIMx, uint16_t TIM_DMASource, FunctionalState NewState);

/**
 * @brief Configures the TIMx internal Clock.
 * @param[in] TIMx where x can be 1 to 4 to select the TIMx peripheral.
 */
void TIM_InternalClockConfig(TIM_TypeDef *TIMx);

/**
 * @brief Configures the TIMx Internal Trigger as External Clock.
 * @param[in] TIMx where x can be 1 to 4 to select the TIMx peripheral.
 * @param[in] TIM_InputTriggerSource Trigger source.
 *            This parameter can be one of the following values:
 *            TIM_TS_ITR0: Internal Trigger 0;
 *            TIM_TS_ITR1: Internal Trigger 1;
 *            TIM_TS_ITR2: Internal Trigger 2;
 *            TIM_TS_ITR3: Internal Trigger 3.
 */
void TIM_ITRxExternalClockConfig(TIM_TypeDef *TIMx, uint16_t TIM_InputTriggerSource);

/**
 * @brief Configures the TIMx Trigger as External Clock.
 * @param[in] TIMx where x can be  1, 2, 3, 4 to select the TIM peripheral.
 * @param[in] TIM_TIxExternalCLKSource Trigger source.
 *            This parameter can be one of the following values:
 *            TIM_TIxExternalCLK1Source_TI1ED: TI1 Edge Detector;
 *            TIM_TIxExternalCLK1Source_TI1: Filtered Timer Input 1;
 *            TIM_TIxExternalCLK1Source_TI2: Filtered Timer Input 2.
 * @param[in] TIM_ICPolarity specifies the TIx Polarity.
 *            This parameter can be one of the following values:
 *            TIM_ICPolarity_Rising;
 *            TIM_ICPolarity_Falling.
 * @param[in] ICFilter specifies the filter value.
 *            This parameter must be a value between 0x0 and 0xF.
 */
void TIM_TIxExternalClockConfig(TIM_TypeDef *TIMx, uint16_t TIM_TIxExternalCLKSource,
                                uint16_t TIM_ICPolarity, uint16_t ICFilter);

/**
 * @brief Configures the External clock Mode1.
 * @param[in] TIMx where x can be  1, 2, 3, 4 to select the TIM peripheral.
 * @param[in] TIM_ExtTRGPrescaler The external Trigger Prescaler.
 *            This parameter can be one of the following values:
 *            TIM_ExtTRGPSC_OFF: ETRP Prescaler OFF;
 *            TIM_ExtTRGPSC_DIV2: ETRP frequency divided by 2;
 *            TIM_ExtTRGPSC_DIV4: ETRP frequency divided by 4;
 *            TIM_ExtTRGPSC_DIV8: ETRP frequency divided by 8.
 * @param[in] TIM_ExtTRGPolarity The external Trigger Polarity.
 *            This parameter can be one of the following values:
 *            TIM_ExtTRGPolarity_Inverted: active low or falling edge active;
 *            TIM_ExtTRGPolarity_NonInverted: active high or rising edge active.
 * @param[in] ExtTRGFilter External Trigger Filter.
 *            This parameter must be a value between 0x00 and 0x0F.
 */
void TIM_ETRClockMode1Config(TIM_TypeDef *TIMx, uint16_t TIM_ExtTRGPrescaler, uint16_t TIM_ExtTRGPolarity,
                             uint16_t ExtTRGFilter);

/**
 * @brief Configures the External clock Mode2.
 * @param[in] TIMx where x can be  1, 2, 3, 4 to select the TIM peripheral.
 * @param[in] TIM_ExtTRGPrescaler The external Trigger Prescaler.
 *            This parameter can be one of the following values:
 *            TIM_ExtTRGPSC_OFF: ETRP Prescaler OFF;
 *            TIM_ExtTRGPSC_DIV2: ETRP frequency divided by 2;
 *            TIM_ExtTRGPSC_DIV4: ETRP frequency divided by 4;
 *            TIM_ExtTRGPSC_DIV8: ETRP frequency divided by 8.
 * @param[in] TIM_ExtTRGPolarity The external Trigger Polarity.
 *            This parameter can be one of the following values:
 *            TIM_ExtTRGPolarity_Inverted: active low or falling edge active;
 *            TIM_ExtTRGPolarity_NonInverted: active high or rising edge active.
 * @param[in] ExtTRGFilter External Trigger Filter.
 *            This parameter must be a value between 0x00 and 0x0F.
 */
void TIM_ETRClockMode2Config(TIM_TypeDef *TIMx, uint16_t TIM_ExtTRGPrescaler,
                             uint16_t TIM_ExtTRGPolarity, uint16_t ExtTRGFilter);

/**
 * @brief Configures the TIMx External Trigger (ETR).
 * @param[in] TIMx where x can be  1, 2, 3, 4 to select the TIM peripheral.
 * @param[in] TIM_ExtTRGPrescaler The external Trigger Prescaler.
 *            This parameter can be one of the following values:
 *            TIM_ExtTRGPSC_OFF: ETRP Prescaler OFF;
 *            TIM_ExtTRGPSC_DIV2: ETRP frequency divided by 2;
 *            TIM_ExtTRGPSC_DIV4: ETRP frequency divided by 4;
 *            TIM_ExtTRGPSC_DIV8: ETRP frequency divided by 8.
 * @param[in] TIM_ExtTRGPolarity The external Trigger Polarity.
 *            This parameter can be one of the following values:
 *            TIM_ExtTRGPolarity_Inverted: active low or falling edge active;
 *            TIM_ExtTRGPolarity_NonInverted: active high or rising edge active.
 * @param[in] ExtTRGFilter External Trigger Filter.
 *            This parameter must be a value between 0x00 and 0x0F.
 */
void TIM_ETRConfig(TIM_TypeDef *TIMx, uint16_t TIM_ExtTRGPrescaler, uint16_t TIM_ExtTRGPolarity,
                   uint16_t ExtTRGFilter);

/**
 * @brief Configures the TIMx Prescaler.
 * @param[in] TIMx where x can be 1 to 4 to select the TIM peripheral.
 * @param[in] Prescaler specifies the Prescaler Register value.
 * @param[in] TIM_PSCReloadMode specifies the TIM Prescaler Reload mode.
 *            This parameter can be one of the following values:
 *            TIM_PSCReloadMode_Update: The Prescaler is loaded at the update event;
 *            TIM_PSCReloadMode_Immediate: The Prescaler is loaded immediately.
 */
void TIM_PrescalerConfig(TIM_TypeDef *TIMx, uint16_t Prescaler, uint16_t TIM_PSCReloadMode);

/**
 * @brief Specifies the TIMx Counter Mode to be used.
 * @param[in] TIMx where x can be 1 to 4 to select the TIM peripheral.
 * @param[in] TIM_CounterMode specifies the Counter Mode to be used.
 *            This parameter can be one of the following values:
 *            TIM_CounterMode_Up: TIM Up Counting Mode;
 *            TIM_CounterMode_Down: TIM Down Counting Mode;
 *            TIM_CounterMode_CenterAligned1: TIM Center Aligned Mode1;
 *            TIM_CounterMode_CenterAligned2: TIM Center Aligned Mode2;
 *            TIM_CounterMode_CenterAligned3: TIM Center Aligned Mode3.
 */
void TIM_CounterModeConfig(TIM_TypeDef *TIMx, uint16_t TIM_CounterMode);

/**
 * @brief Selects the Input Trigger source.
 * @param[in] TIMx where x can be 1 to 4 to select the TIM peripheral.
 * @param[in] TIM_InputTriggerSource The Input Trigger source.
 *            This parameter can be one of the following values:
 *            TIM_TS_ITR0: Internal Trigger 0;
 *            TIM_TS_ITR1: Internal Trigger 1;
 *            TIM_TS_ITR2: Internal Trigger 2;
 *            TIM_TS_ITR3: Internal Trigger 3;
 *            TIM_TS_TI1F_ED: TI1 Edge Detector;
 *            TIM_TS_TI1FP1: Filtered Timer Input 1;
 *            TIM_TS_TI2FP2: Filtered Timer Input 2;
 *            TIM_TS_ETRF: External Trigger input.
 */
void TIM_SelectInputTrigger(TIM_TypeDef *TIMx, uint16_t TIM_InputTriggerSource);

/**
 * @brief Configures the TIMx Encoder Interface.
 * @param[in] TIMx where x can be 1 to 4 to select the TIM peripheral.
 * @param[in] TIM_EncoderMode specifies the TIMx Encoder Mode.
 *            This parameter can be one of the following values:
 *            TIM_EncoderMode_TI1: Counter counts on TI1FP1 edge depending on TI2FP2 level;
 *            TIM_EncoderMode_TI2: Counter counts on TI2FP2 edge depending on TI1FP1 level;
 *            TIM_EncoderMode_TI12: Counter counts on both TI1FP1 and TI2FP2 edges depending
 *                                  on the level of the other input.
 * @param[in] TIM_IC1Polarity specifies the IC1 Polarity.
 *            This parameter can be one of the following values:
 *            TIM_ICPolarity_Falling: IC Falling edge;
 *            TIM_ICPolarity_Rising: IC Rising edge.
 * @param[in] TIM_IC2Polarity specifies the IC2 Polarity.
 *            This parameter can be one of the following values:
 *            TIM_ICPolarity_Falling: IC Falling edge;
 *            TIM_ICPolarity_Rising: IC Rising edge.
 */
void TIM_EncoderInterfaceConfig(TIM_TypeDef *TIMx, uint16_t TIM_EncoderMode,
                                uint16_t TIM_IC1Polarity, uint16_t TIM_IC2Polarity);

/**
 * @brief Forces the TIMx output 1 waveform to active or inactive level.
 * @param[in] TIMx where x can be 1 to 4 to select the TIM peripheral.
 * @param[in] TIM_ForcedAction specifies the forced Action to be set to the output waveform.
 *            This parameter can be one of the following values:
 *            TIM_ForcedAction_Active: Force active level on OC1REF;
 *            TIM_ForcedAction_InActive: Force inactive level on OC1REF.
 */
void TIM_ForcedOC1Config(TIM_TypeDef *TIMx, uint16_t TIM_ForcedAction);

/**
 * @brief Forces the TIMx output 2 waveform to active or inactive level.
 * @param[in] TIMx where x can be 1 to 4 to select the TIM peripheral.
 * @param[in] TIM_ForcedAction specifies the forced Action to be set to the output waveform.
 *            This parameter can be one of the following values:
 *            TIM_ForcedAction_Active: Force active level on OC2REF;
 *            TIM_ForcedAction_InActive: Force inactive level on OC2REF.
 */
void TIM_ForcedOC2Config(TIM_TypeDef *TIMx, uint16_t TIM_ForcedAction);

/**
 * @brief Forces the TIMx output 3 waveform to active or inactive level.
 * @param[in] TIMx where x can be 1 to 4 to select the TIM peripheral.
 * @param[in] TIM_ForcedAction specifies the forced Action to be set to the output waveform.
 *            This parameter can be one of the following values:
 *            TIM_ForcedAction_Active: Force active level on OC3REF;
 *            TIM_ForcedAction_InActive: Force inactive level on OC3REF.
 */
void TIM_ForcedOC3Config(TIM_TypeDef *TIMx, uint16_t TIM_ForcedAction);

/**
 * @brief Forces the TIMx output 3 waveform to active or inactive level.
 * @param[in] TIMx where x can be 1 to 4 to select the TIM peripheral.
 * @param[in] TIM_ForcedAction specifies the forced Action to be set to the output waveform.
 *            This parameter can be one of the following values:
 *            TIM_ForcedAction_Active: Force active level on OC4REF;
 *            TIM_ForcedAction_InActive: Force inactive level on OC4REF.
 */
void TIM_ForcedOC4Config(TIM_TypeDef *TIMx, uint16_t TIM_ForcedAction);

/**
 * @brief Enables or disables TIMx peripheral Preload register on ARR.
 * @param[in] TIMx where x can be 1 to 4 to select the TIM peripheral.
 * @param[in] NewState new state of the TIMx peripheral Preload register
 *            This parameter can be: ENABLE or DISABLE.
 */
void TIM_ARRPreloadConfig(TIM_TypeDef *TIMx, FunctionalState NewState);

/**
 * @brief Selects the TIM peripheral Commutation event.
 * @param[in] TIMx where x can be 1 to 4 to select the TIM peripheral.
 * @param[in] NewState new state of the Commutation event.
 *            This parameter can be: ENABLE or DISABLE.
 */
void TIM_SelectCOM(TIM_TypeDef *TIMx, FunctionalState NewState);

/**
 * @brief Selects the TIMx peripheral Capture Compare DMA source.
 * @param[in] TIMx where x can be 1 to 4 to select the TIM peripheral.
 * @param[in] NewState new state of the Capture Compare DMA source.
 *            This parameter can be: ENABLE or DISABLE.
 */
void TIM_SelectCCDMA(TIM_TypeDef *TIMx, FunctionalState NewState);

/**
 * @brief Sets or Resets the TIM peripheral Capture Compare Preload Control bit.
 * @param[in] TIMx where x can be 1 to 4 to select the TIM peripheral. 
 * @param[in] NewState new state of the Capture Compare Preload Control bit
 *            This parameter can be: ENABLE or DISABLE.
 */
void TIM_CCPreloadControl(TIM_TypeDef *TIMx, FunctionalState NewState);

/**
 * @brief Enables or disables the TIMx peripheral Preload register on CCR1.
 * @param[in] TIMx where x can be 1 to 4 to select the TIM peripheral.
 * @param[in] TIM_OCPreload new state of the TIMx peripheral Preload register.
 *            This parameter can be one of the following values:
 *            TIM_OCPreload_Enable or TIM_OCPreload_Disable.
 */
void TIM_OC1PreloadConfig(TIM_TypeDef *TIMx, uint16_t TIM_OCPreload);

/**
 * @brief Enables or disables the TIMx peripheral Preload register on CCR2.
 * @param[in] TIMx where x can be 1 to 4 to select the TIM peripheral.
 * @param[in] TIM_OCPreload new state of the TIMx peripheral Preload register.
 *            This parameter can be one of the following values:
 *            TIM_OCPreload_Enable or TIM_OCPreload_Disable.
 */
void TIM_OC2PreloadConfig(TIM_TypeDef *TIMx, uint16_t TIM_OCPreload);

/**
 * @brief Enables or disables the TIMx peripheral Preload register on CCR3.
 * @param[in] TIMx where x can be 1 to 4 to select the TIM peripheral.
 * @param[in] TIM_OCPreload new state of the TIMx peripheral Preload register.
 *            This parameter can be one of the following values:
 *            TIM_OCPreload_Enable or TIM_OCPreload_Disable.
 */
void TIM_OC3PreloadConfig(TIM_TypeDef *TIMx, uint16_t TIM_OCPreload);

/**
 * @brief Enables or disables the TIMx peripheral Preload register on CCR4.
 * @param[in] TIMx where x can be 1 to 4 to select the TIM peripheral.
 * @param[in] TIM_OCPreload new state of the TIMx peripheral Preload register.
 *            This parameter can be one of the following values:
 *            TIM_OCPreload_Enable or TIM_OCPreload_Disable.
 */
void TIM_OC4PreloadConfig(TIM_TypeDef *TIMx, uint16_t TIM_OCPreload);

/**
 * @brief Configures the TIMx Output Compare 1 Fast feature.
 * @param[in] TIMx where x can be 1 to 4 to select the TIM peripheral.
 * @param[in] TIM_OCFast new state of the Output Compare Fast Enable Bit.
 *            This parameter can be one of the following values:
 *            TIM_OCFast_Enable or TIM_OCFast_Disable.
 */
void TIM_OC1FastConfig(TIM_TypeDef *TIMx, uint16_t TIM_OCFast);

/**
 * @brief Configures the TIMx Output Compare 2 Fast feature.
 * @param[in] TIMx where x can be 1 to 4 to select the TIM peripheral.
 * @param[in] TIM_OCFast new state of the Output Compare Fast Enable Bit.
 *            This parameter can be one of the following values:
 *            TIM_OCFast_Enable or TIM_OCFast_Disable.
 */
void TIM_OC2FastConfig(TIM_TypeDef *TIMx, uint16_t TIM_OCFast);

/**
 * @brief Configures the TIMx Output Compare 3 Fast feature.
 * @param[in] TIMx where x can be 1 to 4 to select the TIM peripheral.
 * @param[in] TIM_OCFast new state of the Output Compare Fast Enable Bit.
 *            This parameter can be one of the following values:
 *            TIM_OCFast_Enable or TIM_OCFast_Disable.
 */
void TIM_OC3FastConfig(TIM_TypeDef *TIMx, uint16_t TIM_OCFast);

/**
 * @brief Configures the TIMx Output Compare 4 Fast feature.
 * @param[in] TIMx where x can be 1 to 4 to select the TIM peripheral.
 * @param[in] TIM_OCFast new state of the Output Compare Fast Enable Bit.
 *            This parameter can be one of the following values:
 *            TIM_OCFast_Enable or TIM_OCFast_Disable.
 */
void TIM_OC4FastConfig(TIM_TypeDef *TIMx, uint16_t TIM_OCFast);

/**
 * @brief Clears or safeguards the OCREF1 signal on an external event.
 * @param[in] TIMx where x can be 1 to 4 to select the TIM peripheral.
 * @param[in] TIM_OCClear new state of the Output Compare Clear Enable Bit.
 *            This parameter can be one of the following values:
 *            TIM_OCClear_Enable or TIM_OCClear_Disable.
 */
void TIM_ClearOC1Ref(TIM_TypeDef *TIMx, uint16_t TIM_OCClear);

/**
 * @brief Clears or safeguards the OCREF2 signal on an external event.
 * @param[in] TIMx where x can be 1 to 4 to select the TIM peripheral.
 * @param[in] TIM_OCClear new state of the Output Compare Clear Enable Bit.
 *            This parameter can be one of the following values:
 *            TIM_OCClear_Enable or TIM_OCClear_Disable.
 */
void TIM_ClearOC2Ref(TIM_TypeDef *TIMx, uint16_t TIM_OCClear);

/**
 * @brief Clears or safeguards the OCREF3 signal on an external event.
 * @param[in] TIMx where x can be 1 to 4 to select the TIM peripheral.
 * @param[in] TIM_OCClear new state of the Output Compare Clear Enable Bit.
 *            This parameter can be one of the following values:
 *            TIM_OCClear_Enable or TIM_OCClear_Disable.
 */
void TIM_ClearOC3Ref(TIM_TypeDef *TIMx, uint16_t TIM_OCClear);

/**
 * @brief Clears or safeguards the OCREF4 signal on an external event.
 * @param[in] TIMx where x can be 1 to 4 to select the TIM peripheral.
 * @param[in] TIM_OCClear new state of the Output Compare Clear Enable Bit.
 *            This parameter can be one of the following values:
 *            TIM_OCClear_Enable or TIM_OCClear_Disable.
 */
void TIM_ClearOC4Ref(TIM_TypeDef *TIMx, uint16_t TIM_OCClear);

/**
 * @brief Configures the TIMx channel 1 polarity.
 * @param[in] TIMx where x can be 1 to 4 to select the TIM peripheral.
 * @param[in] TIM_OCPolarity specifies the OC1 Polarity.
 *            This parameter can be one of the following values:
 *            TIM_OCPolarity_High or TIM_OCPolarity_Low.
 */
void TIM_OC1PolarityConfig(TIM_TypeDef *TIMx, uint16_t TIM_OCPolarity);

/**
 * @brief Configures the TIMx channel 1N polarity.
 * @param[in] TIMx where x can be 1 to select the TIM peripheral.
 * @param[in] TIM_OCNPolarity specifies the OC1N Polarity.
 *            This parameter can be one of the following values:
 *            TIM_OCPolarity_High or TIM_OCPolarity_Low.
 */
void TIM_OC1NPolarityConfig(TIM_TypeDef *TIMx, uint16_t TIM_OCNPolarity);

/**
 * @brief Configures the TIMx channel 2 polarity.
 * @param[in] TIMx where x can be 1 to 4 to select the TIM peripheral.
 * @param[in] TIM_OCPolarity specifies the OC2 Polarity.
 *            This parameter can be one of the following values:
 *            TIM_OCPolarity_High or TIM_OCPolarity_Low.
 */
void TIM_OC2PolarityConfig(TIM_TypeDef *TIMx, uint16_t TIM_OCPolarity);

/**
 * @brief Configures the TIMx Channel 2N polarity.
 * @param[in] TIMx where x can be 1 to select the TIM peripheral.
 * @param[in] TIM_OCNPolarity specifies the OC2N Polarity.
 *            This parameter can be one of the following values:
 *            TIM_OCPolarity_High or TIM_OCPolarity_Low.
 */
void TIM_OC2NPolarityConfig(TIM_TypeDef *TIMx, uint16_t TIM_OCNPolarity);

/**
 * @brief Configures the TIMx Channel 3 polarity.
 * @param[in] TIMx where x can be 1 to 4 to select the TIM peripheral. 
 * @param[in] TIM_OCPolarity specifies the OC3 Polarity.
 *            This parameter can be one of the following values:
 *            TIM_OCPolarity_High or TIM_OCPolarity_Low.
 */
void TIM_OC3PolarityConfig(TIM_TypeDef *TIMx, uint16_t TIM_OCPolarity);

/**
 * @brief Configures the TIMx Channel 3N polarity.
 * @param[in] TIMx where x can be 1 to select the TIM peripheral. 
 * @param[in] TIM_OCNPolarity specifies the OC3N Polarity.
 *            This parameter can be one of the following values:
 *            TIM_OCPolarity_High or TIM_OCPolarity_Low.
 */
void TIM_OC3NPolarityConfig(TIM_TypeDef *TIMx, uint16_t TIM_OCNPolarity);

/**
 * @brief Configures the TIMx Channel 4 polarity.
 * @param[in] TIMx where x can be 1 to 4 to select the TIM peripheral. 
 * @param[in] TIM_OCPolarity specifies the OC4 Polarity.
 *            This parameter can be one of the following values:
 *            TIM_OCPolarity_High or TIM_OCPolarity_Low.
 */
void TIM_OC4PolarityConfig(TIM_TypeDef *TIMx, uint16_t TIM_OCPolarity);

/**
 * @brief Enables or disables the TIM Capture Compare Channel x.
 * @param[in] TIMx where x can be 1 to 4 to select the TIM peripheral.
 * @param[in] TIM_Channel specifies the TIM Channel.
 *            This parameter can be one of the following values:
 *            TIM_Channel_1: TIM Channel 1;
 *            TIM_Channel_2: TIM Channel 2;
 *            TIM_Channel_3: TIM Channel 3;
 *            TIM_Channel_4: TIM Channel 4.
 * @param[in] TIM_CCx specifies the TIM Channel CCxE bit new state.
 *            This parameter can be: TIM_CCx_Enable or TIM_CCx_Disable.
 */
void TIM_CCxCmd(TIM_TypeDef *TIMx, uint16_t TIM_Channel, uint16_t TIM_CCx);

/**
 * @brief Enables or disables the TIM Capture Compare Channel xN.
 * @param[in] TIMx where x can be 1 to select the TIM peripheral.
 * @param[in] TIM_Channel specifies the TIM Channel.
 *            This parameter can be one of the following values:
 *            TIM_Channel_1: TIM Channel 1;
 *            TIM_Channel_2: TIM Channel 2;
 *            TIM_Channel_3: TIM Channel 3.
 * @param[in] TIM_CCxN specifies the TIM Channel CCxNE bit new state.
 *            This parameter can be: TIM_CCxN_Enable or TIM_CCxN_Disable.
 */
void TIM_CCxNCmd(TIM_TypeDef *TIMx, uint16_t TIM_Channel, uint16_t TIM_CCxN);

/**
 * @brief Selects the TIM Output Compare Mode.
 *        This function disables the selected channel before changing the Output Compare Mode.
 *        User has to enable this channel using TIM_CCxCmd and TIM_CCxNCmd functions.
 * @param[in] TIMx where x can be 1 to 4 to select the TIM peripheral.
 * @param[in] TIM_Channel specifies the TIM Channel.
 *            This parameter can be one of the following values:
 *            TIM_Channel_1: TIM Channel 1;
 *            TIM_Channel_2: TIM Channel 2;
 *            TIM_Channel_3: TIM Channel 3;
 *            TIM_Channel_4: TIM Channel 4.
 * @param[in] TIM_OCMode specifies the TIM Output Compare Mode.
 *            This parameter can be one of the following values:
 *            TIM_OCMode_Timing;
 *            TIM_OCMode_Active;
 *            TIM_OCMode_Toggle;
 *            TIM_OCMode_PWM1;
 *            TIM_OCMode_PWM2;
 *            TIM_ForcedAction_Active;
 *            TIM_ForcedAction_InActive.
 */
void TIM_SelectOCxM(TIM_TypeDef *TIMx, uint16_t TIM_Channel, uint16_t TIM_OCMode);

/**
 * @brief Enables or Disables the TIMx Update event.
 * @param[in] TIMx where x can be 1 to 4 to select the TIM peripheral.  
 * @param[in] NewState new state of the TIMx UDIS bit.
 *            This parameter can be: ENABLE or DISABLE.
 */
void TIM_UpdateDisableConfig(TIM_TypeDef *TIMx, FunctionalState NewState);

/**
 * @brief Configures the TIMx Update Request Interrupt source.
 * @param[in] TIMx where x can be 1 to 4 to select the TIM peripheral.
 * @param[in] TIM_UpdateSource specifies the Update source.
 *            This parameter can be one of the following values:
 *            TIM_UpdateSource_Regular: Source of update is the counter overflow/underflow
 *                                      or the setting of UG bit, or an update generation
 *                                      through the slave mode controller;
 *            TIM_UpdateSource_Global: Source of update is counter overflow/underflow.
 */
void TIM_UpdateRequestConfig(TIM_TypeDef *TIMx, uint16_t TIM_UpdateSource);

/**
 * @brief Enables or disables the TIMx's Hall sensor interface.
 * @param[in] TIMx where x can be 1 to 4 to select the TIM peripheral.  
 * @param[in] NewState new state of the TIMx Hall sensor interface.
 *            This parameter can be: ENABLE or DISABLE.
 */
void TIM_SelectHallSensor(TIM_TypeDef *TIMx, FunctionalState NewState);

/**
 * @brief Selects the TIMx's One Pulse Mode.
 * @param[in] TIMx where x can be 1 to 4 to select the TIM peripheral. 
 * @param[in] TIM_OPMode specifies the OPM Mode to be used.
 *            This parameter can be one of the following values:
 *            TIM_OPMode_Single or TIM_OPMode_Repetitive.
 */
void TIM_SelectOnePulseMode(TIM_TypeDef *TIMx, uint16_t TIM_OPMode);

/**
 * @brief Selects the TIMx Trigger Output Mode.
 * @param[in] TIMx where x can be 1 to 4 to select the TIM peripheral.
 * @param[in] TIM_TRGOSource specifies the Trigger Output source.
 *            This paramter can be one of the following values:
 *            TIM_TRGOSource_OC1: The trigger output sends a positive pulse when the CC1IF flag
 *                                is to be set, as soon as a capture or compare match occurs (TRGO);
 *            TIM_TRGOSource_OC1Ref: OC1REF signal is used as the trigger output (TRGO);
 *            TIM_TRGOSource_OC2Ref: OC2REF signal is used as the trigger output (TRGO);
 *            TIM_TRGOSource_OC3Ref: OC3REF signal is used as the trigger output (TRGO);
 *            TIM_TRGOSource_OC4Ref: OC4REF signal is used as the trigger output (TRGO).
 */
void TIM_SelectOutputTrigger(TIM_TypeDef *TIMx, uint16_t TIM_TRGOSource);

/**
 * @brief Selects the TIMx Slave Mode.
 * @param[in] TIMx where x can be 1 to 4 to select the TIM peripheral.
 * @param[in] TIM_SlaveMode specifies the Timer Slave Mode.
 *            This parameter can be one of the following values:
 *            TIM_SlaveMode_Reset: Rising edge of the selected trigger signal (TRGI) re-initializes
 *                                 the counter and triggers an update of the registers;
 *            TIM_SlaveMode_Gated: The counter clock is enabled when the trigger signal (TRGI) is high;
 *            TIM_SlaveMode_Trigger: The counter starts at a rising edge of the trigger TRGI;
 *            TIM_SlaveMode_External1: Rising edges of the selected trigger (TRGI) clock the counter.
 */
void TIM_SelectSlaveMode(TIM_TypeDef *TIMx, uint16_t TIM_SlaveMode);

/**
 * @brief Sets or Resets the TIMx Master/Slave Mode.
 * @param[in] TIMx where x can be 1 to 4 to select the TIM peripheral.
 * @param[in] TIM_MasterSlaveMode specifies the Timer Master Slave Mode.
 *            This parameter can be one of the following values:
 *            TIM_MasterSlaveMode_Enable: synchronization between the current timer
 *                                        and its slaves (through TRGO);
 *            TIM_MasterSlaveMode_Disable: No action.
 */
void TIM_SelectMasterSlaveMode(TIM_TypeDef *TIMx, uint16_t TIM_MasterSlaveMode);

/**
 * @brief Sets the TIMx Counter Register value.
 * @param[in] TIMx where x can be 1 to 4 to select the TIM peripheral. 
 * @param[in] Counter specifies the Counter register new value.
 */
void TIM_SetCounter(TIM_TypeDef *TIMx, uint16_t Counter);

/**
 * @brief Sets the TIMx Autoreload Register value.
 * @param[in] TIMx where x can be 1 to 4 to select the TIM peripheral.  
 * @param[in] Autoreload specifies the Autoreload register new value.
 */
void TIM_SetAutoreload(TIM_TypeDef *TIMx, uint16_t Autoreload);

/**
 * @brief Sets the TIMx Capture Compare1 Register value.
 * @param[in] TIMx where x can be 1 to 4 to select the TIM peripheral. 
 * @param[in] Compare1 specifies the Capture Compare1 register new value.
 */
void TIM_SetCompare1(TIM_TypeDef *TIMx, uint16_t Compare1);

/**
 * @brief Sets the TIMx Capture Compare2 Register value.
 * @param[in] TIMx where x can be 1 to 4 to select the TIM peripheral.  
 * @param[in] Compare2 specifies the Capture Compare2 register new value.
 */
void TIM_SetCompare2(TIM_TypeDef *TIMx, uint16_t Compare2);

/**
 * @brief Sets the TIMx Capture Compare3 Register value.
 * @param[in] TIMx where x can be 1 to 4 to select the TIM peripheral.  
 * @param[in] Compare3 specifies the Capture Compare3 register new value.
 */
void TIM_SetCompare3(TIM_TypeDef *TIMx, uint16_t Compare3);

/**
 * @brief Sets the TIMx Capture Compare4 Register value.
 * @param[in] TIMx where x can be 1 to 4 to select the TIM peripheral.  
 * @param[in] Compare4 specifies the Capture Compare4 register new value.
 */
void TIM_SetCompare4(TIM_TypeDef *TIMx, uint16_t Compare4);

/**
 * @brief Sets the TIMx Input Capture 1 prescaler.
 * @param[in] TIMx where x can be 1 to 4 to select the TIM peripheral.
 * @param[in] TIM_ICPSC specifies the Input Capture1 prescaler new value.
 *            This parameter can be one of the following values:
 *            TIM_ICPSC_DIV1: no prescaler;
 *            TIM_ICPSC_DIV2: capture is done once every 2 events;
 *            TIM_ICPSC_DIV4: capture is done once every 4 events;
 *            TIM_ICPSC_DIV8: capture is done once every 8 events.
 */
void TIM_SetIC1Prescaler(TIM_TypeDef *TIMx, uint16_t TIM_ICPSC);

/**
 * @brief Sets the TIMx Input Capture 2 prescaler.
 * @param[in] TIMx where x can be 1 to 4 to select the TIM peripheral.
 * @param[in] TIM_ICPSC specifies the Input Capture1 prescaler new value.
 *            This parameter can be one of the following values:
 *            TIM_ICPSC_DIV1: no prescaler;
 *            TIM_ICPSC_DIV2: capture is done once every 2 events;
 *            TIM_ICPSC_DIV4: capture is done once every 4 events;
 *            TIM_ICPSC_DIV8: capture is done once every 8 events.
 */
void TIM_SetIC2Prescaler(TIM_TypeDef *TIMx, uint16_t TIM_ICPSC);

/**
 * @brief Sets the TIMx Input Capture 3 prescaler.
 * @param[in] TIMx where x can be 1 to 4 to select the TIM peripheral.
 * @param[in] TIM_ICPSC specifies the Input Capture1 prescaler new value.
 *            This parameter can be one of the following values:
 *            TIM_ICPSC_DIV1: no prescaler;
 *            TIM_ICPSC_DIV2: capture is done once every 2 events;
 *            TIM_ICPSC_DIV4: capture is done once every 4 events;
 *            TIM_ICPSC_DIV8: capture is done once every 8 events.
 */
void TIM_SetIC3Prescaler(TIM_TypeDef *TIMx, uint16_t TIM_ICPSC);

/**
 * @brief Sets the TIMx Input Capture 4 prescaler.
 * @param[in] TIMx where x can be 1 to 4 to select the TIM peripheral.
 * @param[in] TIM_ICPSC specifies the Input Capture1 prescaler new value.
 *            This parameter can be one of the following values:
 *            TIM_ICPSC_DIV1: no prescaler;
 *            TIM_ICPSC_DIV2: capture is done once every 2 events;
 *            TIM_ICPSC_DIV4: capture is done once every 4 events;
 *            TIM_ICPSC_DIV8: capture is done once every 8 events.
 */
void TIM_SetIC4Prescaler(TIM_TypeDef *TIMx, uint16_t TIM_ICPSC);

/**
 * @brief Sets the TIMx Clock Division value.
 * @param[in] TIMx where x can be 1 to 4 to select the TIM peripheral.
 * @param[in] TIM_CKD specifies the clock division value.
 *            This parameter can be one of the following value:
 *            TIM_CKD_DIV1: TDTS = Tck_tim;
 *            TIM_CKD_DIV2: TDTS = 2*Tck_tim;
 *            TIM_CKD_DIV4: TDTS = 4*Tck_tim.
 */
void TIM_SetClockDivision(TIM_TypeDef *TIMx, uint16_t TIM_CKD);

/**
 * @brief Gets the TIMx Input Capture 1 value.
 * @param[in] TIMx where x can be 1 to 4 to select the TIM peripheral. 
 * @return uint16_t  
 */
uint16_t TIM_GetCapture1(TIM_TypeDef *TIMx);

/**
 * @brief Gets the TIMx Input Capture 2 value.
 * @param[in] TIMx where x can be 1 to 4 to select the TIM peripheral. 
 * @return uint16_t  
 */
uint16_t TIM_GetCapture2(TIM_TypeDef *TIMx);

/**
 * @brief Gets the TIMx Input Capture 3 value. 
 * @param[in] TIMx where x can be 1 to 4 to select the TIM peripheral. 
 * @return uint16_t  
 */
uint16_t TIM_GetCapture3(TIM_TypeDef *TIMx);

/**
 * @brief Gets the TIMx Input Capture 4 value. 
 * @param[in] TIMx where x can be 1 to 4 to select the TIM peripheral.  
 * @return uint16_t  
 */
uint16_t TIM_GetCapture4(TIM_TypeDef *TIMx);

/**
 * @brief Gets the TIMx Counter value.
 * @param[in] TIMx where x can be 1 to 4 to select the TIM peripheral. 
 * @return uint16_t  
 */
uint16_t TIM_GetCounter(TIM_TypeDef *TIMx);

/**
 * @brief Gets the TIMx Prescaler value.
 * @param[in] TIMx where x can be 1 to 4 to select the TIM peripheral. 
 * @return uint16_t  
 */
uint16_t TIM_GetPrescaler(TIM_TypeDef *TIMx);

/**
 * @brief Checks whether the specified TIM flag is set or not.
 * @param[in] TIMx where x can be 1 to 4 to select the TIM peripheral.
 * @param[in] TIM_FLAG specifies the flag to check.
 *            This parameter can be one of the following values:
 *            TIM_FLAG_Update: TIM update Flag;
 *            TIM_FLAG_CC1: TIM Capture Compare 1 Flag;
 *            TIM_FLAG_CC2: TIM Capture Compare 2 Flag;
 *            TIM_FLAG_CC3: TIM Capture Compare 3 Flag;
 *            TIM_FLAG_CC4: TIM Capture Compare 4 Flag;
 *            TIM_FLAG_COM: TIM Commutation Flag;
 *            TIM_FLAG_Trigger: TIM Trigger Flag;
 *            TIM_FLAG_Break: TIM Break Flag;
 *            TIM_FLAG_CC1OF: TIM Capture Compare 1 overcapture Flag;
 *            TIM_FLAG_CC2OF: TIM Capture Compare 2 overcapture Flag;
 *            TIM_FLAG_CC3OF: TIM Capture Compare 3 overcapture Flag;
 *            TIM_FLAG_CC4OF: TIM Capture Compare 4 overcapture Flag;
 *            TIM_FLAG_Break and TIM_FLAG_COM is used only with TIM1.
 * @return FlagStatus
 */
FlagStatus TIM_GetFlagStatus(TIM_TypeDef *TIMx, uint16_t TIM_FLAG);

/**
 * @brief Clears the TIMx's pending flags.
 * @param[in] TIMx where x can be 1 to 4 to select the TIM peripheral.
 * @param[in] TIM_FLAG specifies the flag bit to clear.
 *            This parameter can be any combination of the following values:
 *            TIM_FLAG_Update: TIM update Flag;
 *            TIM_FLAG_CC1: TIM Capture Compare 1 Flag;
 *            TIM_FLAG_CC2: TIM Capture Compare 2 Flag;
 *            TIM_FLAG_CC3: TIM Capture Compare 3 Flag;
 *            TIM_FLAG_CC4: TIM Capture Compare 4 Flag;
 *            TIM_FLAG_COM: TIM Commutation Flag;
 *            TIM_FLAG_Trigger: TIM Trigger Flag;
 *            TIM_FLAG_Break: TIM Break Flag;
 *            TIM_FLAG_CC1OF: TIM Capture Compare 1 overcapture Flag;
 *            TIM_FLAG_CC2OF: TIM Capture Compare 2 overcapture Flag;
 *            TIM_FLAG_CC3OF: TIM Capture Compare 3 overcapture Flag;
 *            TIM_FLAG_CC4OF: TIM Capture Compare 4 overcapture Flag.
 *            TIM_FLAG_Break and TIM_FLAG_COM is used only with TIM1.
 */
void TIM_ClearFlag(TIM_TypeDef *TIMx, uint16_t TIM_FLAG);

/**
 * @brief Checks whether the TIM interrupt has occurred or not.
 * @param[in] TIMx where x can be 1 to 4 to select the TIM peripheral.
 * @param[in] TIM_IT specifies the TIM interrupt source to check.
 *            This parameter can be one of the following values:
 *            TIM_IT_Update: TIM update Interrupt source;
 *            TIM_IT_CC1: TIM Capture Compare 1 Interrupt source;
 *            TIM_IT_CC2: TIM Capture Compare 2 Interrupt source;
 *            TIM_IT_CC3: TIM Capture Compare 3 Interrupt source;
 *            TIM_IT_CC4: TIM Capture Compare 4 Interrupt source;
 *            TIM_IT_COM: TIM Commutation Interrupt source;
 *            TIM_IT_Trigger: TIM Trigger Interrupt source;
 *            TIM_IT_Break: TIM Break Interrupt source.
 *            TIM_IT_Break and TIM_IT_COM is used only with TIM1.
 * @return ITStatus
 */
ITStatus TIM_GetITStatus(TIM_TypeDef *TIMx, uint16_t TIM_IT);

/**
 * @brief Clears the TIMx's interrupt pending bits.
 * @param[in] TIMx where x can be 1 to 4 to select the TIM peripheral.
 * @param[in] TIM_IT specifies the pending bit to clear.
 *            This parameter can be any combination of the following values:
 *            TIM_IT_Update: TIM1 update Interrupt source;
 *            TIM_IT_CC1: TIM Capture Compare 1 Interrupt source;
 *            TIM_IT_CC2: TIM Capture Compare 2 Interrupt source;
 *            TIM_IT_CC3: TIM Capture Compare 3 Interrupt source;
 *            TIM_IT_CC4: TIM Capture Compare 4 Interrupt source;
 *            TIM_IT_COM: TIM Commutation Interrupt source;
 *            TIM_IT_Trigger: TIM Trigger Interrupt source;
 *            TIM_IT_Break: TIM Break Interrupt source.
 *            TIM_IT_Break and TIM_IT_COM is used only with TIM1.
 */
void TIM_ClearITPendingBit(TIM_TypeDef *TIMx, uint16_t TIM_IT);

#ifdef __cplusplus
}
#endif

#endif