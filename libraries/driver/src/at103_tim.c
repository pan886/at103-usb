/**
 * @file at103_tim.c
 * @brief This file provides all the TIM firmware functions.
 * @author liyouchuang (lycdemeng@163.com)
 * @version 1.0
 * @date 2022-03-06
 * @copyright Copyright (c) 2022 Icore, Inc
 */

#include "at103_tim.h"
#ifdef TIM_MODULE_ENABLED
/* ---------------------- TIM registers bit mask ------------------------ */
#define SMCR_ETR_Mask ((uint16_t)0x00FF)
#define CCMR_Offset   ((uint16_t)0x0018)
#define CCER_CCE_Set  ((uint16_t)0x0001)
#define CCER_CCNE_Set ((uint16_t)0x0004)

#define IS_TIM_ALL_PERIPH(PERIPH) (((PERIPH) == TIM1) || \
                                   ((PERIPH) == TIM2) || \
                                   ((PERIPH) == TIM3) || \
                                   ((PERIPH) == TIM4))

#define IS_TIM_LIST1_PERIPH(PERIPH) ((PERIPH) == TIM1)

#define IS_TIM_OC_MODE(MODE) (((MODE) == TIM_OCMode_Timing) || \
                              ((MODE) == TIM_OCMode_Active) || \
                              ((MODE) == TIM_OCMode_Inactive) || \
                              ((MODE) == TIM_OCMode_Toggle) || \
                              ((MODE) == TIM_OCMode_PWM1) || \
                              ((MODE) == TIM_OCMode_PWM2))
#define IS_TIM_OCM(MODE) (((MODE) == TIM_OCMode_Timing) || \
                          ((MODE) == TIM_OCMode_Active) || \
                          ((MODE) == TIM_OCMode_Inactive) || \
                          ((MODE) == TIM_OCMode_Toggle) || \
                          ((MODE) == TIM_OCMode_PWM1) || \
                          ((MODE) == TIM_OCMode_PWM2) || \
                          ((MODE) == TIM_ForcedAction_Active) || \
                          ((MODE) == TIM_ForcedAction_InActive))

#define IS_TIM_OPM_MODE(MODE) (((MODE) == TIM_OPMode_Single) || \
                               ((MODE) == TIM_OPMode_Repetitive))

#define IS_TIM_CHANNEL(CHANNEL) (((CHANNEL) == TIM_Channel_1) || \
                                 ((CHANNEL) == TIM_Channel_2) || \
                                 ((CHANNEL) == TIM_Channel_3) || \
                                 ((CHANNEL) == TIM_Channel_4))
#define IS_TIM_PWMI_CHANNEL(CHANNEL) (((CHANNEL) == TIM_Channel_1) || \
                                      ((CHANNEL) == TIM_Channel_2))
#define IS_TIM_COMPLEMENTARY_CHANNEL(CHANNEL) (((CHANNEL) == TIM_Channel_1) || \
                                               ((CHANNEL) == TIM_Channel_2) || \
                                               ((CHANNEL) == TIM_Channel_3))

#define IS_TIM_CKD_DIV(DIV) (((DIV) == TIM_CKD_DIV1) || \
                             ((DIV) == TIM_CKD_DIV2) || \
                             ((DIV) == TIM_CKD_DIV4))

#define IS_TIM_COUNTER_MODE(MODE) (((MODE) == TIM_CounterMode_Up) || \
                                   ((MODE) == TIM_CounterMode_Down) || \
                                   ((MODE) == TIM_CounterMode_CenterAligned1) || \
                                   ((MODE) == TIM_CounterMode_CenterAligned2) || \
                                   ((MODE) == TIM_CounterMode_CenterAligned3))

#define IS_TIM_OC_POLARITY(POLARITY) (((POLARITY) == TIM_OCPolarity_High) || \
                                      ((POLARITY) == TIM_OCPolarity_Low))

#define IS_TIM_OCN_POLARITY(POLARITY) (((POLARITY) == TIM_OCNPolarity_High) || \
                                       ((POLARITY) == TIM_OCNPolarity_Low))

#define IS_TIM_OUTPUT_STATE(STATE) (((STATE) == TIM_OutputState_Disable) || \
                                    ((STATE) == TIM_OutputState_Enable))

#define IS_TIM_OUTPUTN_STATE(STATE) (((STATE) == TIM_OutputNState_Disable) || \
                                     ((STATE) == TIM_OutputNState_Enable))

#define IS_TIM_CCX(CCX) (((CCX) == TIM_CCx_Enable) || \
                         ((CCX) == TIM_CCx_Disable))

#define IS_TIM_CCXN(CCXN) (((CCXN) == TIM_CCxN_Enable) || \
                           ((CCXN) == TIM_CCxN_Disable))

#define IS_TIM_BREAK_STATE(STATE) (((STATE) == TIM_Break_Enable) || \
                                   ((STATE) == TIM_Break_Disable))

#define IS_TIM_BREAK_POLARITY(POLARITY) (((POLARITY) == TIM_BreakPolarity_Low) || \
                                         ((POLARITY) == TIM_BreakPolarity_High))

#define IS_TIM_AUTOMATIC_OUTPUT_STATE(STATE) (((STATE) == TIM_AutomaticOutput_Enable) || \
                                              ((STATE) == TIM_AutomaticOutput_Disable))

#define IS_TIM_LOCK_LEVEL(LEVEL) (((LEVEL) == TIM_LOCKLevel_OFF) || \
                                  ((LEVEL) == TIM_LOCKLevel_1) || \
                                  ((LEVEL) == TIM_LOCKLevel_2) || \
                                  ((LEVEL) == TIM_LOCKLevel_3))

#define IS_TIM_OSSI_STATE(STATE) (((STATE) == TIM_OSSIState_Enable) || \
                                  ((STATE) == TIM_OSSIState_Disable))

#define IS_TIM_OSSR_STATE(STATE) (((STATE) == TIM_OSSRState_Enable) || \
                                  ((STATE) == TIM_OSSRState_Disable))

#define IS_TIM_OCIDLE_STATE(STATE) (((STATE) == TIM_OCIdleState_Set) || \
                                    ((STATE) == TIM_OCIdleState_Reset))

#define IS_TIM_OCNIDLE_STATE(STATE) (((STATE) == TIM_OCNIdleState_Set) || \
                                     ((STATE) == TIM_OCNIdleState_Reset))

#define IS_TIM_IC_POLARITY(POLARITY) (((POLARITY) == TIM_ICPolarity_Rising) || \
                                      ((POLARITY) == TIM_ICPolarity_Falling))
#define IS_TIM_IC_POLARITY_LITE(POLARITY) (((POLARITY) == TIM_ICPolarity_Rising) || \
                                           ((POLARITY) == TIM_ICPolarity_Falling) || \
                                           ((POLARITY) == TIM_ICPolarity_BothEdge))

#define IS_TIM_IC_SELECTION(SELECTION) (((SELECTION) == TIM_ICSelection_DirectTI) || \
                                        ((SELECTION) == TIM_ICSelection_IndirectTI) || \
                                        ((SELECTION) == TIM_ICSelection_TRC))

#define IS_TIM_IC_PRESCALER(PRESCALER) (((PRESCALER) == TIM_ICPSC_DIV1) || \
                                        ((PRESCALER) == TIM_ICPSC_DIV2) || \
                                        ((PRESCALER) == TIM_ICPSC_DIV4) || \
                                        ((PRESCALER) == TIM_ICPSC_DIV8))

#define IS_TIM_IT(IT) ((((IT) & (uint16_t)0xFF00) == 0x0000) && ((IT) != 0x0000))

#define IS_TIM_GET_IT(IT) (((IT) == TIM_IT_Update) || \
                           ((IT) == TIM_IT_CC1) || \
                           ((IT) == TIM_IT_CC2) || \
                           ((IT) == TIM_IT_CC3) || \
                           ((IT) == TIM_IT_CC4) || \
                           ((IT) == TIM_IT_COM) || \
                           ((IT) == TIM_IT_Trigger) || \
                           ((IT) == TIM_IT_Break))

#define IS_TIM_DMA_BASE(BASE) (((BASE) == TIM_DMABase_CR1) || \
                               ((BASE) == TIM_DMABase_CR2) || \
                               ((BASE) == TIM_DMABase_SMCR) || \
                               ((BASE) == TIM_DMABase_DIER) || \
                               ((BASE) == TIM_DMABase_SR) || \
                               ((BASE) == TIM_DMABase_EGR) || \
                               ((BASE) == TIM_DMABase_CCMR1) || \
                               ((BASE) == TIM_DMABase_CCMR2) || \
                               ((BASE) == TIM_DMABase_CCER) || \
                               ((BASE) == TIM_DMABase_CNT) || \
                               ((BASE) == TIM_DMABase_PSC) || \
                               ((BASE) == TIM_DMABase_ARR) || \
                               ((BASE) == TIM_DMABase_RCR) || \
                               ((BASE) == TIM_DMABase_CCR1) || \
                               ((BASE) == TIM_DMABase_CCR2) || \
                               ((BASE) == TIM_DMABase_CCR3) || \
                               ((BASE) == TIM_DMABase_CCR4) || \
                               ((BASE) == TIM_DMABase_BDTR) || \
                               ((BASE) == TIM_DMABase_DCR))

#define IS_TIM_DMA_LENGTH(LENGTH) (((LENGTH) == TIM_DMABurstLength_1Transfer) || \
                                   ((LENGTH) == TIM_DMABurstLength_2Transfers) || \
                                   ((LENGTH) == TIM_DMABurstLength_3Transfers) || \
                                   ((LENGTH) == TIM_DMABurstLength_4Transfers) || \
                                   ((LENGTH) == TIM_DMABurstLength_5Transfers) || \
                                   ((LENGTH) == TIM_DMABurstLength_6Transfers) || \
                                   ((LENGTH) == TIM_DMABurstLength_7Transfers) || \
                                   ((LENGTH) == TIM_DMABurstLength_8Transfers) || \
                                   ((LENGTH) == TIM_DMABurstLength_9Transfers) || \
                                   ((LENGTH) == TIM_DMABurstLength_10Transfers) || \
                                   ((LENGTH) == TIM_DMABurstLength_11Transfers) || \
                                   ((LENGTH) == TIM_DMABurstLength_12Transfers) || \
                                   ((LENGTH) == TIM_DMABurstLength_13Transfers) || \
                                   ((LENGTH) == TIM_DMABurstLength_14Transfers) || \
                                   ((LENGTH) == TIM_DMABurstLength_15Transfers) || \
                                   ((LENGTH) == TIM_DMABurstLength_16Transfers) || \
                                   ((LENGTH) == TIM_DMABurstLength_17Transfers) || \
                                   ((LENGTH) == TIM_DMABurstLength_18Transfers))

#define IS_TIM_DMA_SOURCE(SOURCE) ((((SOURCE) & (uint16_t)0x80FF) == 0x0000) && ((SOURCE) != 0x0000))

#define IS_TIM_EXT_PRESCALER(PRESCALER) (((PRESCALER) == TIM_ExtTRGPSC_OFF) || \
                                         ((PRESCALER) == TIM_ExtTRGPSC_DIV2) || \
                                         ((PRESCALER) == TIM_ExtTRGPSC_DIV4) || \
                                         ((PRESCALER) == TIM_ExtTRGPSC_DIV8))

#define IS_TIM_TRIGGER_SELECTION(SELECTION) (((SELECTION) == TIM_TS_ITR0) || \
                                             ((SELECTION) == TIM_TS_ITR1) || \
                                             ((SELECTION) == TIM_TS_ITR2) || \
                                             ((SELECTION) == TIM_TS_ITR3) || \
                                             ((SELECTION) == TIM_TS_TI1F_ED) || \
                                             ((SELECTION) == TIM_TS_TI1FP1) || \
                                             ((SELECTION) == TIM_TS_TI2FP2) || \
                                             ((SELECTION) == TIM_TS_ETRF))
#define IS_TIM_INTERNAL_TRIGGER_SELECTION(SELECTION) (((SELECTION) == TIM_TS_ITR0) || \
                                                      ((SELECTION) == TIM_TS_ITR1) || \
                                                      ((SELECTION) == TIM_TS_ITR2) || \
                                                      ((SELECTION) == TIM_TS_ITR3))

#define IS_TIM_TIXCLK_SOURCE(SOURCE) (((SOURCE) == TIM_TIxExternalCLK1Source_TI1) || \
                                      ((SOURCE) == TIM_TIxExternalCLK1Source_TI2) || \
                                      ((SOURCE) == TIM_TIxExternalCLK1Source_TI1ED))

#define IS_TIM_EXT_POLARITY(POLARITY) (((POLARITY) == TIM_ExtTRGPolarity_Inverted) || \
                                       ((POLARITY) == TIM_ExtTRGPolarity_NonInverted))

#define IS_TIM_PRESCALER_RELOAD(RELOAD) (((RELOAD) == TIM_PSCReloadMode_Update) || \
                                         ((RELOAD) == TIM_PSCReloadMode_Immediate))

#define IS_TIM_FORCED_ACTION(ACTION) (((ACTION) == TIM_ForcedAction_Active) || \
                                      ((ACTION) == TIM_ForcedAction_InActive))

#define IS_TIM_ENCODER_MODE(MODE) (((MODE) == TIM_EncoderMode_TI1) || \
                                   ((MODE) == TIM_EncoderMode_TI2) || \
                                   ((MODE) == TIM_EncoderMode_TI12))

#define IS_TIM_EVENT_SOURCE(SOURCE) ((((SOURCE) & (uint16_t)0xFF00) == 0x0000) && ((SOURCE) != 0x0000))

#define IS_TIM_UPDATE_SOURCE(SOURCE) (((SOURCE) == TIM_UpdateSource_Global) || \
                                      ((SOURCE) == TIM_UpdateSource_Regular))

#define IS_TIM_OCPRELOAD_STATE(STATE) (((STATE) == TIM_OCPreload_Enable) || \
                                       ((STATE) == TIM_OCPreload_Disable))

#define IS_TIM_OCFAST_STATE(STATE) (((STATE) == TIM_OCFast_Enable) || \
                                    ((STATE) == TIM_OCFast_Disable))

#define IS_TIM_OCCLEAR_STATE(STATE) (((STATE) == TIM_OCClear_Enable) || \
                                     ((STATE) == TIM_OCClear_Disable))

#define IS_TIM_TRGO_SOURCE(SOURCE) (((SOURCE) == TIM_TRGOSource_Reset) || \
                                    ((SOURCE) == TIM_TRGOSource_Enable) || \
                                    ((SOURCE) == TIM_TRGOSource_Update) || \
                                    ((SOURCE) == TIM_TRGOSource_OC1) || \
                                    ((SOURCE) == TIM_TRGOSource_OC1Ref) || \
                                    ((SOURCE) == TIM_TRGOSource_OC2Ref) || \
                                    ((SOURCE) == TIM_TRGOSource_OC3Ref) || \
                                    ((SOURCE) == TIM_TRGOSource_OC4Ref))

#define IS_TIM_SLAVE_MODE(MODE) (((MODE) == TIM_SlaveMode_Reset) || \
                                 ((MODE) == TIM_SlaveMode_Gated) || \
                                 ((MODE) == TIM_SlaveMode_Trigger) || \
                                 ((MODE) == TIM_SlaveMode_External1))

#define IS_TIM_MSM_STATE(STATE) (((STATE) == TIM_MasterSlaveMode_Enable) || \
                                 ((STATE) == TIM_MasterSlaveMode_Disable))

#define IS_TIM_GET_FLAG(FLAG) (((FLAG) == TIM_FLAG_Update) || \
                               ((FLAG) == TIM_FLAG_CC1) || \
                               ((FLAG) == TIM_FLAG_CC2) || \
                               ((FLAG) == TIM_FLAG_CC3) || \
                               ((FLAG) == TIM_FLAG_CC4) || \
                               ((FLAG) == TIM_FLAG_COM) || \
                               ((FLAG) == TIM_FLAG_Trigger) || \
                               ((FLAG) == TIM_FLAG_Break) || \
                               ((FLAG) == TIM_FLAG_CC1OF) || \
                               ((FLAG) == TIM_FLAG_CC2OF) || \
                               ((FLAG) == TIM_FLAG_CC3OF) || \
                               ((FLAG) == TIM_FLAG_CC4OF))

#define IS_TIM_CLEAR_FLAG(TIM_FLAG) ((((TIM_FLAG) & (uint16_t)0xE100) == 0x0000) && ((TIM_FLAG) != 0x0000))

#define IS_TIM_IC_FILTER(ICFILTER) ((ICFILTER) <= 0xF)

#define IS_TIM_EXT_FILTER(EXTFILTER) ((EXTFILTER) <= 0xF)

/**
 * @brief TIM_Private_FunctionPrototypes
 */
static void TI1_Config(TIM_TypeDef *TIMx, uint16_t TIM_ICPolarity, uint16_t TIM_ICSelection,
                       uint16_t TIM_ICFilter);
static void TI2_Config(TIM_TypeDef *TIMx, uint16_t TIM_ICPolarity, uint16_t TIM_ICSelection,
                       uint16_t TIM_ICFilter);
static void TI3_Config(TIM_TypeDef *TIMx, uint16_t TIM_ICPolarity, uint16_t TIM_ICSelection,
                       uint16_t TIM_ICFilter);
static void TI4_Config(TIM_TypeDef *TIMx, uint16_t TIM_ICPolarity, uint16_t TIM_ICSelection,
                       uint16_t TIM_ICFilter);

void TIM_DeInit(TIM_TypeDef *TIMx)
{
    /* Check the parameters */
    assert_param(IS_TIM_ALL_PERIPH(TIMx));

    if (TIMx == TIM1) {
        RCC_APB2PeriphResetCmd(RCC_APB2Periph_TIM1, ENABLE);
        RCC_APB2PeriphResetCmd(RCC_APB2Periph_TIM1, DISABLE);
    } else if (TIMx == TIM2) {
        RCC_APB1PeriphResetCmd(RCC_APB1Periph_TIM2, ENABLE);
        RCC_APB1PeriphResetCmd(RCC_APB1Periph_TIM2, DISABLE);
    } else if (TIMx == TIM3) {
        RCC_APB1PeriphResetCmd(RCC_APB1Periph_TIM3, ENABLE);
        RCC_APB1PeriphResetCmd(RCC_APB1Periph_TIM3, DISABLE);
    } else {
        if (TIMx == TIM4) {
            RCC_APB1PeriphResetCmd(RCC_APB1Periph_TIM4, ENABLE);
            RCC_APB1PeriphResetCmd(RCC_APB1Periph_TIM4, DISABLE);
        }
    }
}

void TIM_TimeBaseInit(TIM_TypeDef *TIMx, TIM_TimeBaseInitTypeDef *TIM_TimeBaseInitStruct)
{
    uint16_t tmpcr1 = 0;

    /* Check the parameters */
    assert_param(IS_TIM_ALL_PERIPH(TIMx));
    assert_param(IS_TIM_COUNTER_MODE(TIM_TimeBaseInitStruct->TIM_CounterMode));
    assert_param(IS_TIM_CKD_DIV(TIM_TimeBaseInitStruct->TIM_ClockDivision));

    tmpcr1 = TIMx->TIM_CR1.value;
    /* Select the Counter Mode */
    tmpcr1 &= (uint16_t)(~((uint16_t)(TIM_CR1_TIM_CR1_DIR | TIM_CR1_TIM_CR1_CMS)));
    tmpcr1 |= (uint32_t)(TIM_TimeBaseInitStruct->TIM_CounterMode);
    /* Set the clock division */
    tmpcr1 &= (uint16_t)(~((uint16_t)TIM_CR1_TIM_CR1_CKD));
    tmpcr1 |= (uint32_t)(TIM_TimeBaseInitStruct->TIM_ClockDivision);

    TIMx->TIM_CR1.value = tmpcr1;

    /* Set the Autoreload value */
    TIMx->TIM_ARR.value = TIM_TimeBaseInitStruct->TIM_Period;

    /* Set the Prescaler value */
    TIMx->TIM_PSC.value = TIM_TimeBaseInitStruct->TIM_Prescaler;

    if ((TIMx == TIM1)) {
        /* Set the Repetition Counter value */
        TIMx->TIM_RCR.value = TIM_TimeBaseInitStruct->TIM_RepetitionCounter;
    }

    /* Generate an update event to reload the Prescaler and the Repetition counter
     values immediately */
    TIMx->TIM_EGR.value = TIM_PSCReloadMode_Immediate;
}

void TIM_OC1Init(TIM_TypeDef *TIMx, TIM_OCInitTypeDef *TIM_OCInitStruct)
{
    uint16_t tmpccmrx = 0;
    uint16_t tmpccer  = 0;
    uint16_t tmpcr2   = 0;

    /* Check the parameters */
    assert_param(IS_TIM_ALL_PERIPH(TIMx));
    assert_param(IS_TIM_OC_MODE(TIM_OCInitStruct->TIM_OCMode));
    assert_param(IS_TIM_OUTPUT_STATE(TIM_OCInitStruct->TIM_OutputState));
    assert_param(IS_TIM_OC_POLARITY(TIM_OCInitStruct->TIM_OCPolarity));

    /* Disable the Channel 1: Reset the CC1E Bit */
    TIMx->TIM_CCER.TIM_CCER_CC1E = 0;

    /* Get the TIMx CCER register value */
    tmpccer = TIMx->TIM_CCER.value;

    /* Get the TIMx CR2 register value */
    tmpcr2 = TIMx->TIM_CR2.value;

    /* Get the TIMx CCMR1 register value */
    tmpccmrx = TIMx->TIM_CCMR1.value;

    /* Reset the Output Compare Mode Bits */
    tmpccmrx &= (uint16_t)(~((uint16_t)TIM_CCMR1_TIM_CCMR1_OC1M));
    tmpccmrx &= (uint16_t)(~((uint16_t)TIM_CCMR1_TIM_CCMR1_CC1S));

    /* Select the Output Compare Mode */
    tmpccmrx |= TIM_OCInitStruct->TIM_OCMode;

    /* Reset the Output Polarity level */
    tmpccer &= (uint16_t)(~((uint16_t)TIM_CCER_TIM_CCER_CC1P));
    /* Set the Output Compare Polarity */
    tmpccer |= TIM_OCInitStruct->TIM_OCPolarity;

    /* Set the Output State */
    tmpccer |= TIM_OCInitStruct->TIM_OutputState;

    if (TIMx == TIM1) {
        assert_param(IS_TIM_OUTPUTN_STATE(TIM_OCInitStruct->TIM_OutputNState));
        assert_param(IS_TIM_OCN_POLARITY(TIM_OCInitStruct->TIM_OCNPolarity));
        assert_param(IS_TIM_OCNIDLE_STATE(TIM_OCInitStruct->TIM_OCNIdleState));
        assert_param(IS_TIM_OCIDLE_STATE(TIM_OCInitStruct->TIM_OCIdleState));

        /* Reset the Output N Polarity level */
        tmpccer &= (uint16_t)(~((uint16_t)TIM_CCER_TIM_CCER_CC1NP));
        /* Set the Output N Polarity */
        tmpccer |= TIM_OCInitStruct->TIM_OCNPolarity;

        /* Reset the Output N State */
        tmpccer &= (uint16_t)(~((uint16_t)TIM_CCER_TIM_CCER_CC1NE));
        /* Set the Output N State */
        tmpccer |= TIM_OCInitStruct->TIM_OutputNState;

        /* Reset the Output Compare and Output Compare N IDLE State */
        tmpcr2 &= (uint16_t)(~((uint16_t)TIM_CR2_TIM_CR2_OIS1));
        tmpcr2 &= (uint16_t)(~((uint16_t)TIM_CR2_TIM_CR2_OIS1N));

        /* Set the Output Idle state */
        tmpcr2 |= TIM_OCInitStruct->TIM_OCIdleState;
        /* Set the Output N Idle state */
        tmpcr2 |= TIM_OCInitStruct->TIM_OCNIdleState;
    }
    /* Write to TIMx CR2 */
    TIMx->TIM_CR2.value = tmpcr2;

    /* Write to TIMx CCMR1 */
    TIMx->TIM_CCMR1.value = tmpccmrx;

    /* Set the Capture Compare Register value */
    TIMx->TIM_CCR1.value = TIM_OCInitStruct->TIM_Pulse;

    /* Write to TIMx CCER */
    TIMx->TIM_CCER.value = tmpccer;
}

void TIM_OC2Init(TIM_TypeDef *TIMx, TIM_OCInitTypeDef *TIM_OCInitStruct)
{
    uint16_t tmpccmrx = 0;
    uint16_t tmpccer  = 0;
    uint16_t tmpcr2   = 0;

    /* Check the parameters */
    assert_param(IS_TIM_ALL_PERIPH(TIMx));
    assert_param(IS_TIM_OC_MODE(TIM_OCInitStruct->TIM_OCMode));
    assert_param(IS_TIM_OUTPUT_STATE(TIM_OCInitStruct->TIM_OutputState));
    assert_param(IS_TIM_OC_POLARITY(TIM_OCInitStruct->TIM_OCPolarity));
    /* Disable the Channel 2: Reset the CC2E Bit */
    TIMx->TIM_CCER.TIM_CCER_CC2E = 0;

    /* Get the TIMx CCER register value */
    tmpccer = TIMx->TIM_CCER.value;
    /* Get the TIMx CR2 register value */
    tmpcr2 = TIMx->TIM_CR2.value;

    /* Get the TIMx CCMR1 register value */
    tmpccmrx = TIMx->TIM_CCMR1.value;

    /* Reset the Output Compare mode and Capture/Compare selection Bits */
    tmpccmrx &= (uint16_t)(~((uint16_t)TIM_CCMR1_TIM_CCMR1_OC2M));
    tmpccmrx &= (uint16_t)(~((uint16_t)TIM_CCMR1_TIM_CCMR1_CC2S));

    /* Select the Output Compare Mode */
    tmpccmrx |= (uint16_t)(TIM_OCInitStruct->TIM_OCMode << 8);

    /* Reset the Output Polarity level */
    tmpccer &= (uint16_t)(~((uint16_t)TIM_CCER_TIM_CCER_CC2P));
    /* Set the Output Compare Polarity */
    tmpccer |= (uint16_t)(TIM_OCInitStruct->TIM_OCPolarity << 4);

    /* Set the Output State */
    tmpccer |= (uint16_t)(TIM_OCInitStruct->TIM_OutputState << 4);

    if (TIMx == TIM1) {
        assert_param(IS_TIM_OUTPUTN_STATE(TIM_OCInitStruct->TIM_OutputNState));
        assert_param(IS_TIM_OCN_POLARITY(TIM_OCInitStruct->TIM_OCNPolarity));
        assert_param(IS_TIM_OCNIDLE_STATE(TIM_OCInitStruct->TIM_OCNIdleState));
        assert_param(IS_TIM_OCIDLE_STATE(TIM_OCInitStruct->TIM_OCIdleState));

        /* Reset the Output N Polarity level */
        tmpccer &= (uint16_t)(~((uint16_t)TIM_CCER_TIM_CCER_CC2NP));
        /* Set the Output N Polarity */
        tmpccer |= (uint16_t)(TIM_OCInitStruct->TIM_OCNPolarity << 4);

        /* Reset the Output N State */
        tmpccer &= (uint16_t)(~((uint16_t)TIM_CCER_TIM_CCER_CC2NE));
        /* Set the Output N State */
        tmpccer |= (uint16_t)(TIM_OCInitStruct->TIM_OutputNState << 4);

        /* Reset the Output Compare and Output Compare N IDLE State */
        tmpcr2 &= (uint16_t)(~((uint16_t)TIM_CR2_TIM_CR2_OIS2));
        tmpcr2 &= (uint16_t)(~((uint16_t)TIM_CR2_TIM_CR2_OIS2N));

        /* Set the Output Idle state */
        tmpcr2 |= (uint16_t)(TIM_OCInitStruct->TIM_OCIdleState << 2);
        /* Set the Output N Idle state */
        tmpcr2 |= (uint16_t)(TIM_OCInitStruct->TIM_OCNIdleState << 2);
    }
    /* Write to TIMx CR2 */
    TIMx->TIM_CR2.value = tmpcr2;

    /* Write to TIMx CCMR1 */
    TIMx->TIM_CCMR1.value = tmpccmrx;

    /* Set the Capture Compare Register value */
    TIMx->TIM_CCR2.value = TIM_OCInitStruct->TIM_Pulse;

    /* Write to TIMx CCER */
    TIMx->TIM_CCER.value = tmpccer;
}

void TIM_OC3Init(TIM_TypeDef *TIMx, TIM_OCInitTypeDef *TIM_OCInitStruct)
{
    uint16_t tmpccmrx = 0;
    uint16_t tmpccer  = 0;
    uint16_t tmpcr2   = 0;

    /* Check the parameters */
    assert_param(IS_TIM_ALL_PERIPH(TIMx));
    assert_param(IS_TIM_OC_MODE(TIM_OCInitStruct->TIM_OCMode));
    assert_param(IS_TIM_OUTPUT_STATE(TIM_OCInitStruct->TIM_OutputState));
    assert_param(IS_TIM_OC_POLARITY(TIM_OCInitStruct->TIM_OCPolarity));
    /* Disable the Channel 2: Reset the CC2E Bit */
    TIMx->TIM_CCER.TIM_CCER_CC3E = 0;

    /* Get the TIMx CCER register value */
    tmpccer = TIMx->TIM_CCER.value;
    /* Get the TIMx CR2 register value */
    tmpcr2 = TIMx->TIM_CR2.value;

    /* Get the TIMx CCMR2 register value */
    tmpccmrx = TIMx->TIM_CCMR2.value;

    /* Reset the Output Compare mode and Capture/Compare selection Bits */
    tmpccmrx &= (uint16_t)(~((uint16_t)TIM_CCMR2_TIM_CCMR2_OC3M));
    tmpccmrx &= (uint16_t)(~((uint16_t)TIM_CCMR2_TIM_CCMR2_CC3S));
    /* Select the Output Compare Mode */
    tmpccmrx |= TIM_OCInitStruct->TIM_OCMode;

    /* Reset the Output Polarity level */
    tmpccer &= (uint16_t)(~((uint16_t)TIM_CCER_TIM_CCER_CC3P));
    /* Set the Output Compare Polarity */
    tmpccer |= (uint16_t)(TIM_OCInitStruct->TIM_OCPolarity << 8);

    /* Set the Output State */
    tmpccer |= (uint16_t)(TIM_OCInitStruct->TIM_OutputState << 8);

    if (TIMx == TIM1) {
        assert_param(IS_TIM_OUTPUTN_STATE(TIM_OCInitStruct->TIM_OutputNState));
        assert_param(IS_TIM_OCN_POLARITY(TIM_OCInitStruct->TIM_OCNPolarity));
        assert_param(IS_TIM_OCNIDLE_STATE(TIM_OCInitStruct->TIM_OCNIdleState));
        assert_param(IS_TIM_OCIDLE_STATE(TIM_OCInitStruct->TIM_OCIdleState));

        /* Reset the Output N Polarity level */
        tmpccer &= (uint16_t)(~((uint16_t)TIM_CCER_TIM_CCER_CC3NP));
        /* Set the Output N Polarity */
        tmpccer |= (uint16_t)(TIM_OCInitStruct->TIM_OCNPolarity << 8);
        /* Reset the Output N State */
        tmpccer &= (uint16_t)(~((uint16_t)TIM_CCER_TIM_CCER_CC3NE));

        /* Set the Output N State */
        tmpccer |= (uint16_t)(TIM_OCInitStruct->TIM_OutputNState << 8);
        /* Reset the Output Compare and Output Compare N IDLE State */
        tmpcr2 &= (uint16_t)(~((uint16_t)TIM_CR2_TIM_CR2_OIS3));
        tmpcr2 &= (uint16_t)(~((uint16_t)TIM_CR2_TIM_CR2_OIS3N));
        /* Set the Output Idle state */
        tmpcr2 |= (uint16_t)(TIM_OCInitStruct->TIM_OCIdleState << 4);
        /* Set the Output N Idle state */
        tmpcr2 |= (uint16_t)(TIM_OCInitStruct->TIM_OCNIdleState << 4);
    }
    /* Write to TIMx CR2 */
    TIMx->TIM_CR2.value = tmpcr2;

    /* Write to TIMx CCMR2 */
    TIMx->TIM_CCMR2.value = tmpccmrx;

    /* Set the Capture Compare Register value */
    TIMx->TIM_CCR3.value = TIM_OCInitStruct->TIM_Pulse;

    /* Write to TIMx CCER */
    TIMx->TIM_CCER.value = tmpccer;
}

void TIM_OC4Init(TIM_TypeDef *TIMx, TIM_OCInitTypeDef *TIM_OCInitStruct)
{
    uint16_t tmpccmrx = 0;
    uint16_t tmpccer  = 0;
    uint16_t tmpcr2   = 0;

    /* Check the parameters */
    assert_param(IS_TIM_ALL_PERIPH(TIMx));
    assert_param(IS_TIM_OC_MODE(TIM_OCInitStruct->TIM_OCMode));
    assert_param(IS_TIM_OUTPUT_STATE(TIM_OCInitStruct->TIM_OutputState));
    assert_param(IS_TIM_OC_POLARITY(TIM_OCInitStruct->TIM_OCPolarity));
    /* Disable the Channel 2: Reset the CC4E Bit */
    TIMx->TIM_CCER.TIM_CCER_CC4E = 0;

    /* Get the TIMx CCER register value */
    tmpccer = TIMx->TIM_CCER.value;
    /* Get the TIMx CR2 register value */
    tmpcr2 = TIMx->TIM_CR2.value;

    /* Get the TIMx CCMR2 register value */
    tmpccmrx = TIMx->TIM_CCMR2.value;

    /* Reset the Output Compare mode and Capture/Compare selection Bits */
    tmpccmrx &= (uint16_t)(~((uint16_t)TIM_CCMR2_TIM_CCMR2_OC4M));
    tmpccmrx &= (uint16_t)(~((uint16_t)TIM_CCMR2_TIM_CCMR2_CC4S));

    /* Select the Output Compare Mode */
    tmpccmrx |= (uint16_t)(TIM_OCInitStruct->TIM_OCMode << 8);

    /* Reset the Output Polarity level */
    tmpccer &= (uint16_t)(~((uint16_t)TIM_CCER_TIM_CCER_CC4P));
    /* Set the Output Compare Polarity */
    tmpccer |= (uint16_t)(TIM_OCInitStruct->TIM_OCPolarity << 12);

    /* Set the Output State */
    tmpccer |= (uint16_t)(TIM_OCInitStruct->TIM_OutputState << 12);

    if (TIMx == TIM1) {
        assert_param(IS_TIM_OCIDLE_STATE(TIM_OCInitStruct->TIM_OCIdleState));
        /* Reset the Output Compare IDLE State */
        tmpcr2 &= (uint16_t)(~((uint16_t)TIM_CR2_TIM_CR2_OIS4));
        /* Set the Output Idle state */
        tmpcr2 |= (uint16_t)(TIM_OCInitStruct->TIM_OCIdleState << 6);
    }
    /* Write to TIMx CR2 */
    TIMx->TIM_CR2.value = tmpcr2;

    /* Write to TIMx CCMR2 */
    TIMx->TIM_CCMR2.value = tmpccmrx;

    /* Set the Capture Compare Register value */
    TIMx->TIM_CCR4.value = TIM_OCInitStruct->TIM_Pulse;

    /* Write to TIMx CCER */
    TIMx->TIM_CCER.value = tmpccer;
}

void TIM_ICInit(TIM_TypeDef *TIMx, TIM_ICInitTypeDef *TIM_ICInitStruct)
{
    /* Check the parameters */
    assert_param(IS_TIM_CHANNEL(TIM_ICInitStruct->TIM_Channel));
    assert_param(IS_TIM_IC_SELECTION(TIM_ICInitStruct->TIM_ICSelection));
    assert_param(IS_TIM_IC_PRESCALER(TIM_ICInitStruct->TIM_ICPrescaler));
    assert_param(IS_TIM_IC_FILTER(TIM_ICInitStruct->TIM_ICFilter));

    if ((TIMx == TIM1) || (TIMx == TIM2) || (TIMx == TIM3) || (TIMx == TIM4)) {
        assert_param(IS_TIM_IC_POLARITY(TIM_ICInitStruct->TIM_ICPolarity));
    } else {
        assert_param(IS_TIM_IC_POLARITY_LITE(TIM_ICInitStruct->TIM_ICPolarity));
    }

    if (TIM_ICInitStruct->TIM_Channel == TIM_Channel_1) {
        assert_param(IS_TIM_ALL_PERIPH(TIMx));
        /* TI1 Configuration */
        TI1_Config(TIMx, TIM_ICInitStruct->TIM_ICPolarity,
                   TIM_ICInitStruct->TIM_ICSelection,
                   TIM_ICInitStruct->TIM_ICFilter);
        /* Set the Input Capture Prescaler value */
        TIM_SetIC1Prescaler(TIMx, TIM_ICInitStruct->TIM_ICPrescaler);
    } else if (TIM_ICInitStruct->TIM_Channel == TIM_Channel_2) {
        assert_param(IS_TIM_ALL_PERIPH(TIMx));
        /* TI2 Configuration */
        TI2_Config(TIMx, TIM_ICInitStruct->TIM_ICPolarity,
                   TIM_ICInitStruct->TIM_ICSelection,
                   TIM_ICInitStruct->TIM_ICFilter);
        /* Set the Input Capture Prescaler value */
        TIM_SetIC2Prescaler(TIMx, TIM_ICInitStruct->TIM_ICPrescaler);
    } else if (TIM_ICInitStruct->TIM_Channel == TIM_Channel_3) {
        assert_param(IS_TIM_ALL_PERIPH(TIMx));
        /* TI3 Configuration */
        TI3_Config(TIMx, TIM_ICInitStruct->TIM_ICPolarity,
                   TIM_ICInitStruct->TIM_ICSelection,
                   TIM_ICInitStruct->TIM_ICFilter);
        /* Set the Input Capture Prescaler value */
        TIM_SetIC3Prescaler(TIMx, TIM_ICInitStruct->TIM_ICPrescaler);
    } else {
        assert_param(IS_TIM_ALL_PERIPH(TIMx));
        /* TI4 Configuration */
        TI4_Config(TIMx, TIM_ICInitStruct->TIM_ICPolarity,
                   TIM_ICInitStruct->TIM_ICSelection,
                   TIM_ICInitStruct->TIM_ICFilter);
        /* Set the Input Capture Prescaler value */
        TIM_SetIC4Prescaler(TIMx, TIM_ICInitStruct->TIM_ICPrescaler);
    }
}

void TIM_PWMIConfig(TIM_TypeDef *TIMx, TIM_ICInitTypeDef *TIM_ICInitStruct)
{
    uint16_t icoppositepolarity  = TIM_ICPolarity_Rising;
    uint16_t icoppositeselection = TIM_ICSelection_DirectTI;

    /* Check the parameters */
    assert_param(IS_TIM_ALL_PERIPH(TIMx));
    /* Select the Opposite Input Polarity */
    if (TIM_ICInitStruct->TIM_ICPolarity == TIM_ICPolarity_Rising) {
        icoppositepolarity = TIM_ICPolarity_Falling;
    } else {
        icoppositepolarity = TIM_ICPolarity_Rising;
    }
    /* Select the Opposite Input */
    if (TIM_ICInitStruct->TIM_ICSelection == TIM_ICSelection_DirectTI) {
        icoppositeselection = TIM_ICSelection_IndirectTI;
    } else {
        icoppositeselection = TIM_ICSelection_DirectTI;
    }

    if (TIM_ICInitStruct->TIM_Channel == TIM_Channel_1) {
        /* TI1 Configuration */
        TI1_Config(TIMx, TIM_ICInitStruct->TIM_ICPolarity, TIM_ICInitStruct->TIM_ICSelection,
                   TIM_ICInitStruct->TIM_ICFilter);
        /* Set the Input Capture Prescaler value */
        TIM_SetIC1Prescaler(TIMx, TIM_ICInitStruct->TIM_ICPrescaler);
        /* TI2 Configuration */
        TI2_Config(TIMx, icoppositepolarity, icoppositeselection, TIM_ICInitStruct->TIM_ICFilter);
        /* Set the Input Capture Prescaler value */
        TIM_SetIC2Prescaler(TIMx, TIM_ICInitStruct->TIM_ICPrescaler);
    } else {
        /* TI2 Configuration */
        TI2_Config(TIMx, TIM_ICInitStruct->TIM_ICPolarity, TIM_ICInitStruct->TIM_ICSelection,
                   TIM_ICInitStruct->TIM_ICFilter);
        /* Set the Input Capture Prescaler value */
        TIM_SetIC2Prescaler(TIMx, TIM_ICInitStruct->TIM_ICPrescaler);
        /* TI1 Configuration */
        TI1_Config(TIMx, icoppositepolarity, icoppositeselection, TIM_ICInitStruct->TIM_ICFilter);
        /* Set the Input Capture Prescaler value */
        TIM_SetIC1Prescaler(TIMx, TIM_ICInitStruct->TIM_ICPrescaler);
    }
}

void TIM_BDTRConfig(TIM_TypeDef *TIMx, TIM_BDTRInitTypeDef *TIM_BDTRInitStruct)
{
    /* Check the parameters */
    assert_param(IS_TIM_LIST1_PERIPH(TIMx));
    assert_param(IS_TIM_OSSR_STATE(TIM_BDTRInitStruct->TIM_OSSRState));
    assert_param(IS_TIM_OSSI_STATE(TIM_BDTRInitStruct->TIM_OSSIState));
    assert_param(IS_TIM_LOCK_LEVEL(TIM_BDTRInitStruct->TIM_LOCKLevel));
    assert_param(IS_TIM_BREAK_STATE(TIM_BDTRInitStruct->TIM_Break));
    assert_param(IS_TIM_BREAK_POLARITY(TIM_BDTRInitStruct->TIM_BreakPolarity));
    assert_param(IS_TIM_AUTOMATIC_OUTPUT_STATE(TIM_BDTRInitStruct->TIM_AutomaticOutput));
    /* Set the Lock level, the Break enable Bit and the Ploarity, the OSSR State,
     the OSSI State, the dead time value and the Automatic Output Enable Bit */
    TIMx->TIM_BDTR.value = (uint32_t)(TIM_BDTRInitStruct->TIM_OSSRState | TIM_BDTRInitStruct->TIM_OSSIState |
                                      TIM_BDTRInitStruct->TIM_LOCKLevel | TIM_BDTRInitStruct->TIM_DeadTime |
                                      TIM_BDTRInitStruct->TIM_Break | TIM_BDTRInitStruct->TIM_BreakPolarity |
                                      TIM_BDTRInitStruct->TIM_AutomaticOutput);
}

void TIM_TimeBaseStructInit(TIM_TimeBaseInitTypeDef *TIM_TimeBaseInitStruct)
{
    /* Set the default configuration */
    TIM_TimeBaseInitStruct->TIM_Period            = 0xFFFF;
    TIM_TimeBaseInitStruct->TIM_Prescaler         = 0x0000;
    TIM_TimeBaseInitStruct->TIM_ClockDivision     = TIM_CKD_DIV1;
    TIM_TimeBaseInitStruct->TIM_CounterMode       = TIM_CounterMode_Up;
    TIM_TimeBaseInitStruct->TIM_RepetitionCounter = 0x0000;
}

void TIM_OCStructInit(TIM_OCInitTypeDef *TIM_OCInitStruct)
{
    /* Set the default configuration */
    TIM_OCInitStruct->TIM_OCMode       = TIM_OCMode_Timing;
    TIM_OCInitStruct->TIM_OutputState  = TIM_OutputState_Disable;
    TIM_OCInitStruct->TIM_OutputNState = TIM_OutputNState_Disable;
    TIM_OCInitStruct->TIM_Pulse        = 0x0000;
    TIM_OCInitStruct->TIM_OCPolarity   = TIM_OCPolarity_High;
    TIM_OCInitStruct->TIM_OCNPolarity  = TIM_OCPolarity_High;
    TIM_OCInitStruct->TIM_OCIdleState  = TIM_OCIdleState_Reset;
    TIM_OCInitStruct->TIM_OCNIdleState = TIM_OCNIdleState_Reset;
}

void TIM_ICStructInit(TIM_ICInitTypeDef *TIM_ICInitStruct)
{
    /* Set the default configuration */
    TIM_ICInitStruct->TIM_Channel     = TIM_Channel_1;
    TIM_ICInitStruct->TIM_ICPolarity  = TIM_ICPolarity_Rising;
    TIM_ICInitStruct->TIM_ICSelection = TIM_ICSelection_DirectTI;
    TIM_ICInitStruct->TIM_ICPrescaler = TIM_ICPSC_DIV1;
    TIM_ICInitStruct->TIM_ICFilter    = 0x00;
}

void TIM_BDTRStructInit(TIM_BDTRInitTypeDef *TIM_BDTRInitStruct)
{
    /* Set the default configuration */
    TIM_BDTRInitStruct->TIM_OSSRState       = TIM_OSSRState_Disable;
    TIM_BDTRInitStruct->TIM_OSSIState       = TIM_OSSIState_Disable;
    TIM_BDTRInitStruct->TIM_LOCKLevel       = TIM_LOCKLevel_OFF;
    TIM_BDTRInitStruct->TIM_DeadTime        = 0x00;
    TIM_BDTRInitStruct->TIM_Break           = TIM_Break_Disable;
    TIM_BDTRInitStruct->TIM_BreakPolarity   = TIM_BreakPolarity_Low;
    TIM_BDTRInitStruct->TIM_AutomaticOutput = TIM_AutomaticOutput_Disable;
}

void TIM_Cmd(TIM_TypeDef *TIMx, FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_TIM_ALL_PERIPH(TIMx));
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    if (NewState != DISABLE) {
        /* Enable the TIM Counter */
        TIMx->TIM_CR1.TIM_CR1_CEN = 1;
    } else {
        /* Disable the TIM Counter */
        TIMx->TIM_CR1.TIM_CR1_CEN = 0;
    }
}

void TIM_CtrlPWMOutputs(TIM_TypeDef *TIMx, FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_TIM_LIST1_PERIPH(TIMx));
    assert_param(IS_FUNCTIONAL_STATE(NewState));
    if (NewState != DISABLE) {
        /* Enable the TIM Main Output */
        TIMx->TIM_BDTR.TIM_BDTR_MOE = 1;
    } else {
        /* Disable the TIM Main Output */
        TIMx->TIM_BDTR.TIM_BDTR_MOE = 0;
    }
}

void TIM_ITConfig(TIM_TypeDef *TIMx, uint16_t TIM_IT, FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_TIM_ALL_PERIPH(TIMx));
    assert_param(IS_TIM_IT(TIM_IT));
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    if (NewState != DISABLE) {
        /* Enable the Interrupt sources */
        TIMx->TIM_DIER.value |= TIM_IT;
    } else {
        /* Disable the Interrupt sources */
        TIMx->TIM_DIER.value &= ((uint16_t)(~TIM_IT));
    }
}

void TIM_GenerateEvent(TIM_TypeDef *TIMx, uint16_t TIM_EventSource)
{
    /* Check the parameters */
    assert_param(IS_TIM_ALL_PERIPH(TIMx));
    assert_param(IS_TIM_EVENT_SOURCE(TIM_EventSource));

    /* Set the event sources */
    TIMx->TIM_EGR.value = TIM_EventSource;
}

void TIM_DMAConfig(TIM_TypeDef *TIMx, uint16_t TIM_DMABase, uint16_t TIM_DMABurstLength)
{
    /* Check the parameters */
    assert_param(IS_TIM_ALL_PERIPH(TIMx));
    assert_param(IS_TIM_DMA_BASE(TIM_DMABase));
    assert_param(IS_TIM_DMA_LENGTH(TIM_DMABurstLength));
    /* Set the DMA Base and the DMA Burst Length */
    TIMx->TIM_DCR.value = TIM_DMABase | TIM_DMABurstLength;
}

void TIM_DMACmd(TIM_TypeDef *TIMx, uint16_t TIM_DMASource, FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_TIM_ALL_PERIPH(TIMx));
    assert_param(IS_TIM_DMA_SOURCE(TIM_DMASource));
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    if (NewState != DISABLE) {
        /* Enable the DMA sources */
        TIMx->TIM_DIER.value |= TIM_DMASource;
    } else {
        /* Disable the DMA sources */
        TIMx->TIM_DIER.value &= ~TIM_DMASource;
    }
}

void TIM_InternalClockConfig(TIM_TypeDef *TIMx)
{
    /* Check the parameters */
    assert_param(IS_TIM_ALL_PERIPH(TIMx));
    /* Disable slave mode to clock the prescaler directly with the internal clock */
    TIMx->TIM_SMCR.TIM_SMCR_SMS = 0;
}

void TIM_ITRxExternalClockConfig(TIM_TypeDef *TIMx, uint16_t TIM_InputTriggerSource)
{
    /* Check the parameters */
    assert_param(IS_TIM_ALL_PERIPH(TIMx));
    assert_param(IS_TIM_INTERNAL_TRIGGER_SELECTION(TIM_InputTriggerSource));
    /* Select the Internal Trigger */
    TIM_SelectInputTrigger(TIMx, TIM_InputTriggerSource);
    /* Select the External clock mode1 */
    TIMx->TIM_SMCR.value |= TIM_SlaveMode_External1;
}

void TIM_TIxExternalClockConfig(TIM_TypeDef *TIMx, uint16_t TIM_TIxExternalCLKSource,
                                uint16_t TIM_ICPolarity, uint16_t ICFilter)
{
    /* Check the parameters */
    assert_param(IS_TIM_ALL_PERIPH(TIMx));
    assert_param(IS_TIM_TIXCLK_SOURCE(TIM_TIxExternalCLKSource));
    assert_param(IS_TIM_IC_POLARITY(TIM_ICPolarity));
    assert_param(IS_TIM_IC_FILTER(ICFilter));
    /* Configure the Timer Input Clock Source */
    if (TIM_TIxExternalCLKSource == TIM_TIxExternalCLK1Source_TI2) {
        TI2_Config(TIMx, TIM_ICPolarity, TIM_ICSelection_DirectTI, ICFilter);
    } else {
        TI1_Config(TIMx, TIM_ICPolarity, TIM_ICSelection_DirectTI, ICFilter);
    }
    /* Select the Trigger source */
    TIM_SelectInputTrigger(TIMx, TIM_TIxExternalCLKSource);
    /* Select the External clock mode1 */
    TIMx->TIM_SMCR.value |= TIM_SlaveMode_External1;
}

void TIM_ETRClockMode1Config(TIM_TypeDef *TIMx, uint16_t TIM_ExtTRGPrescaler, uint16_t TIM_ExtTRGPolarity,
                             uint16_t ExtTRGFilter)
{
    uint16_t tmpsmcr = 0;

    /* Check the parameters */
    assert_param(IS_TIM_ALL_PERIPH(TIMx));
    assert_param(IS_TIM_EXT_PRESCALER(TIM_ExtTRGPrescaler));
    assert_param(IS_TIM_EXT_POLARITY(TIM_ExtTRGPolarity));
    assert_param(IS_TIM_EXT_FILTER(ExtTRGFilter));
    /* Configure the ETR Clock source */
    TIM_ETRConfig(TIMx, TIM_ExtTRGPrescaler, TIM_ExtTRGPolarity, ExtTRGFilter);

    /* Get the TIMx SMCR register value */
    tmpsmcr = TIMx->TIM_SMCR.value;
    /* Reset the SMS Bits */
    tmpsmcr &= (uint16_t)(~((uint16_t)TIM_SMCR_TIM_SMCR_SMS));
    /* Select the External clock mode1 */
    tmpsmcr |= TIM_SlaveMode_External1;
    /* Select the Trigger selection : ETRF */
    tmpsmcr &= (uint16_t)(~((uint16_t)TIM_SMCR_TIM_SMCR_TS));
    tmpsmcr |= TIM_TS_ETRF;
    /* Write to TIMx SMCR */
    TIMx->TIM_SMCR.value = tmpsmcr;
}

void TIM_ETRClockMode2Config(TIM_TypeDef *TIMx, uint16_t TIM_ExtTRGPrescaler,
                             uint16_t TIM_ExtTRGPolarity, uint16_t ExtTRGFilter)
{
    /* Check the parameters */
    assert_param(IS_TIM_ALL_PERIPH(TIMx));
    assert_param(IS_TIM_EXT_PRESCALER(TIM_ExtTRGPrescaler));
    assert_param(IS_TIM_EXT_POLARITY(TIM_ExtTRGPolarity));
    assert_param(IS_TIM_EXT_FILTER(ExtTRGFilter));
    /* Configure the ETR Clock source */
    TIM_ETRConfig(TIMx, TIM_ExtTRGPrescaler, TIM_ExtTRGPolarity, ExtTRGFilter);
    /* Enable the External clock mode2 */
    TIMx->TIM_SMCR.TIM_SMCR_ECE = 1;
}

void TIM_ETRConfig(TIM_TypeDef *TIMx, uint16_t TIM_ExtTRGPrescaler, uint16_t TIM_ExtTRGPolarity,
                   uint16_t ExtTRGFilter)
{
    uint16_t tmpsmcr = 0;
    /* Check the parameters */
    assert_param(IS_TIM_ALL_PERIPH(TIMx));
    assert_param(IS_TIM_EXT_PRESCALER(TIM_ExtTRGPrescaler));
    assert_param(IS_TIM_EXT_POLARITY(TIM_ExtTRGPolarity));
    assert_param(IS_TIM_EXT_FILTER(ExtTRGFilter));
    tmpsmcr = TIMx->TIM_SMCR.value;
    /* Reset the ETR Bits */
    tmpsmcr &= SMCR_ETR_Mask;
    /* Set the Prescaler, the Filter value and the Polarity */
    tmpsmcr |= (uint16_t)(TIM_ExtTRGPrescaler | (uint16_t)(TIM_ExtTRGPolarity | (uint16_t)(ExtTRGFilter << (uint16_t)8)));
    /* Write to TIMx SMCR */
    TIMx->TIM_SMCR.value = tmpsmcr;
}

void TIM_PrescalerConfig(TIM_TypeDef *TIMx, uint16_t Prescaler, uint16_t TIM_PSCReloadMode)
{
    /* Check the parameters */
    assert_param(IS_TIM_ALL_PERIPH(TIMx));
    assert_param(IS_TIM_PRESCALER_RELOAD(TIM_PSCReloadMode));
    /* Set the Prescaler value */
    TIMx->TIM_PSC.value = Prescaler;
    /* Set or reset the UG Bit */
    TIMx->TIM_EGR.value = TIM_PSCReloadMode;
}

void TIM_CounterModeConfig(TIM_TypeDef *TIMx, uint16_t TIM_CounterMode)
{
    uint16_t tmpcr1 = 0;
    /* Check the parameters */
    assert_param(IS_TIM_ALL_PERIPH(TIMx));
    assert_param(IS_TIM_COUNTER_MODE(TIM_CounterMode));
    tmpcr1 = TIMx->TIM_CR1.value;
    /* Reset the CMS and DIR Bits */
    tmpcr1 &= (uint16_t)(~((uint16_t)(TIM_CR1_TIM_CR1_DIR | TIM_CR1_TIM_CR1_CMS)));
    /* Set the Counter Mode */
    tmpcr1 |= TIM_CounterMode;
    /* Write to TIMx CR1 register */
    TIMx->TIM_CR1.value = tmpcr1;
}

void TIM_SelectInputTrigger(TIM_TypeDef *TIMx, uint16_t TIM_InputTriggerSource)
{
    uint16_t tmpsmcr = 0;
    /* Check the parameters */
    assert_param(IS_TIM_ALL_PERIPH(TIMx));
    assert_param(IS_TIM_TRIGGER_SELECTION(TIM_InputTriggerSource));
    /* Get the TIMx SMCR register value */
    tmpsmcr = TIMx->TIM_SMCR.value;
    /* Reset the TS Bits */
    tmpsmcr &= (uint16_t)(~((uint16_t)TIM_SMCR_TIM_SMCR_TS));
    /* Set the Input Trigger source */
    tmpsmcr |= TIM_InputTriggerSource;
    /* Write to TIMx SMCR */
    TIMx->TIM_SMCR.value = tmpsmcr;
}

void TIM_EncoderInterfaceConfig(TIM_TypeDef *TIMx, uint16_t TIM_EncoderMode,
                                uint16_t TIM_IC1Polarity, uint16_t TIM_IC2Polarity)
{
    uint16_t tmpsmcr  = 0;
    uint16_t tmpccmr1 = 0;
    uint16_t tmpccer  = 0;
    /* Check the parameters */
    assert_param(IS_TIM_ALL_PERIPH(TIMx));
    assert_param(IS_TIM_ENCODER_MODE(TIM_EncoderMode));
    assert_param(IS_TIM_IC_POLARITY(TIM_IC1Polarity));
    assert_param(IS_TIM_IC_POLARITY(TIM_IC2Polarity));

    /* Get the TIMx SMCR register value */
    tmpsmcr = TIMx->TIM_SMCR.value;

    /* Get the TIMx CCMR1 register value */
    tmpccmr1 = TIMx->TIM_CCMR1.value;

    /* Get the TIMx CCER register value */
    tmpccer = TIMx->TIM_CCER.value;

    /* Set the encoder Mode */
    tmpsmcr &= (uint16_t)(~((uint16_t)TIM_SMCR_TIM_SMCR_SMS));
    tmpsmcr |= TIM_EncoderMode;

    /* Select the Capture Compare 1 and the Capture Compare 2 as input */
    tmpccmr1 &= (uint16_t)(((uint16_t) ~((uint16_t)TIM_CCMR1_TIM_CCMR1_CC1S2)) & (uint16_t)(~((uint16_t)TIM_CCMR1_TIM_CCMR1_CC2S2)));
    tmpccmr1 |= TIM_CCMR1_CC1S_0 | TIM_CCMR1_CC2S_0;

    /* Set the TI1 and the TI2 Polarities */
    tmpccer &= (uint16_t)(((uint16_t) ~((uint16_t)TIM_CCER_TIM_CCER_CC1P)) & ((uint16_t) ~((uint16_t)TIM_CCER_TIM_CCER_CC2P)));
    tmpccer |= (uint16_t)(TIM_IC1Polarity | (uint16_t)(TIM_IC2Polarity << (uint16_t)4));

    /* Write to TIMx SMCR */
    TIMx->TIM_SMCR.value = tmpsmcr;
    /* Write to TIMx CCMR1 */
    TIMx->TIM_CCMR1.value = tmpccmr1;
    /* Write to TIMx CCER */
    TIMx->TIM_CCER.value = tmpccer;
}

void TIM_ForcedOC1Config(TIM_TypeDef *TIMx, uint16_t TIM_ForcedAction)
{
    uint16_t tmpccmr1 = 0;
    /* Check the parameters */
    assert_param(IS_TIM_ALL_PERIPH(TIMx));
    assert_param(IS_TIM_FORCED_ACTION(TIM_ForcedAction));
    tmpccmr1 = TIMx->TIM_CCMR1.value;
    /* Reset the OC1M Bits */
    tmpccmr1 &= (uint16_t)(~(uint16_t)TIM_CCMR1_TIM_CCMR1_OC1M);
    /* Configure The Forced output Mode */
    tmpccmr1 |= TIM_ForcedAction;
    /* Write to TIMx CCMR1 register */
    TIMx->TIM_CCMR1.value = tmpccmr1;
}

void TIM_ForcedOC2Config(TIM_TypeDef *TIMx, uint16_t TIM_ForcedAction)
{
    uint16_t tmpccmr1 = 0;
    /* Check the parameters */
    assert_param(IS_TIM_ALL_PERIPH(TIMx));
    assert_param(IS_TIM_FORCED_ACTION(TIM_ForcedAction));
    tmpccmr1 = TIMx->TIM_CCMR1.value;
    /* Reset the OC2M Bits */
    tmpccmr1 &= (uint16_t)(~(uint16_t)TIM_CCMR1_TIM_CCMR1_OC2M);
    /* Configure The Forced output Mode */
    tmpccmr1 |= (uint16_t)(TIM_ForcedAction << 8);
    /* Write to TIMx CCMR1 register */
    TIMx->TIM_CCMR1.value = tmpccmr1;
}

void TIM_ForcedOC3Config(TIM_TypeDef *TIMx, uint16_t TIM_ForcedAction)
{
    uint16_t tmpccmr2 = 0;
    /* Check the parameters */
    assert_param(IS_TIM_ALL_PERIPH(TIMx));
    assert_param(IS_TIM_FORCED_ACTION(TIM_ForcedAction));
    tmpccmr2 = TIMx->TIM_CCMR2.value;
    /* Reset the OC1M Bits */
    tmpccmr2 &= (uint16_t)(~(uint16_t)TIM_CCMR2_TIM_CCMR2_OC3M);
    /* Configure The Forced output Mode */
    tmpccmr2 |= TIM_ForcedAction;
    /* Write to TIMx CCMR2 register */
    TIMx->TIM_CCMR2.value = tmpccmr2;
}

void TIM_ForcedOC4Config(TIM_TypeDef *TIMx, uint16_t TIM_ForcedAction)
{
    uint16_t tmpccmr2 = 0;
    /* Check the parameters */
    assert_param(IS_TIM_ALL_PERIPH(TIMx));
    assert_param(IS_TIM_FORCED_ACTION(TIM_ForcedAction));
    tmpccmr2 = TIMx->TIM_CCMR2.value;
    /* Reset the OC2M Bits */
    tmpccmr2 &= (uint16_t)(~(uint16_t)TIM_CCMR2_TIM_CCMR2_OC4M);
    /* Configure The Forced output Mode */
    tmpccmr2 |= (uint16_t)(TIM_ForcedAction << 8);
    /* Write to TIMx CCMR2 register */
    TIMx->TIM_CCMR2.value = tmpccmr2;
}

void TIM_ARRPreloadConfig(TIM_TypeDef *TIMx, FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_TIM_ALL_PERIPH(TIMx));
    assert_param(IS_FUNCTIONAL_STATE(NewState));
    if (NewState != DISABLE) {
        /* Set the ARR Preload Bit */
        TIMx->TIM_CR1.TIM_CR1_ARPE = 1;
    } else {
        /* Reset the ARR Preload Bit */
        TIMx->TIM_CR1.TIM_CR1_ARPE = 0;
    }
}

void TIM_SelectCOM(TIM_TypeDef *TIMx, FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_TIM_LIST1_PERIPH(TIMx));
    assert_param(IS_FUNCTIONAL_STATE(NewState));
    if (NewState != DISABLE) {
        /* Set the COM Bit */
        TIMx->TIM_CR2.TIM_CR2_CCUS = 1;
    } else {
        /* Reset the COM Bit */
        TIMx->TIM_CR2.TIM_CR2_CCUS = 0;
    }
}

void TIM_SelectCCDMA(TIM_TypeDef *TIMx, FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_TIM_ALL_PERIPH(TIMx));
    assert_param(IS_FUNCTIONAL_STATE(NewState));
    if (NewState != DISABLE) {
        /* Set the CCDS Bit */
        TIMx->TIM_CR2.TIM_CR2_CCDS = 1;
    } else {
        /* Reset the CCDS Bit */
        TIMx->TIM_CR2.TIM_CR2_CCDS = 0;
    }
}

void TIM_CCPreloadControl(TIM_TypeDef *TIMx, FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_TIM_LIST1_PERIPH(TIMx));
    assert_param(IS_FUNCTIONAL_STATE(NewState));
    if (NewState != DISABLE) {
        /* Set the CCPC Bit */
        TIMx->TIM_CR2.TIM_CR2_CCPC = 1;
    } else {
        /* Reset the CCPC Bit */
        TIMx->TIM_CR2.TIM_CR2_CCPC = 0;
    }
}

void TIM_OC1PreloadConfig(TIM_TypeDef *TIMx, uint16_t TIM_OCPreload)
{
    uint16_t tmpccmr1 = 0;
    /* Check the parameters */
    assert_param(IS_TIM_ALL_PERIPH(TIMx));
    assert_param(IS_TIM_OCPRELOAD_STATE(TIM_OCPreload));
    tmpccmr1 = TIMx->TIM_CCMR1.value;
    /* Reset the OC1PE Bit */
    tmpccmr1 &= (uint16_t)(~(uint16_t)TIM_CCMR1_TIM_CCMR1_OC1PE);
    /* Enable or Disable the Output Compare Preload feature */
    tmpccmr1 |= TIM_OCPreload;
    /* Write to TIMx CCMR1 register */
    TIMx->TIM_CCMR1.value = tmpccmr1;
}

void TIM_OC2PreloadConfig(TIM_TypeDef *TIMx, uint16_t TIM_OCPreload)
{
    uint16_t tmpccmr1 = 0;
    /* Check the parameters */
    assert_param(IS_TIM_ALL_PERIPH(TIMx));
    assert_param(IS_TIM_OCPRELOAD_STATE(TIM_OCPreload));
    tmpccmr1 = TIMx->TIM_CCMR1.value;
    /* Reset the OC2PE Bit */
    tmpccmr1 &= (uint16_t)(~(uint16_t)TIM_CCMR1_TIM_CCMR1_OC2PE);
    /* Enable or Disable the Output Compare Preload feature */
    tmpccmr1 |= (uint16_t)(TIM_OCPreload << 8);
    /* Write to TIMx CCMR1 register */
    TIMx->TIM_CCMR1.value = tmpccmr1;
}

void TIM_OC3PreloadConfig(TIM_TypeDef *TIMx, uint16_t TIM_OCPreload)
{
    uint16_t tmpccmr2 = 0;
    /* Check the parameters */
    assert_param(IS_TIM_ALL_PERIPH(TIMx));
    assert_param(IS_TIM_OCPRELOAD_STATE(TIM_OCPreload));
    tmpccmr2 = TIMx->TIM_CCMR2.value;
    /* Reset the OC3PE Bit */
    tmpccmr2 &= (uint16_t)(~(uint16_t)TIM_CCMR2_TIM_CCMR2_OC3PE);
    /* Enable or Disable the Output Compare Preload feature */
    tmpccmr2 |= TIM_OCPreload;
    /* Write to TIMx CCMR2 register */
    TIMx->TIM_CCMR2.value = tmpccmr2;
}

void TIM_OC4PreloadConfig(TIM_TypeDef *TIMx, uint16_t TIM_OCPreload)
{
    uint16_t tmpccmr2 = 0;
    /* Check the parameters */
    assert_param(IS_TIM_ALL_PERIPH(TIMx));
    assert_param(IS_TIM_OCPRELOAD_STATE(TIM_OCPreload));
    tmpccmr2 = TIMx->TIM_CCMR2.value;
    /* Reset the OC4PE Bit */
    tmpccmr2 &= (uint16_t)(~(uint16_t)TIM_CCMR2_TIM_CCMR2_OC4PE);
    /* Enable or Disable the Output Compare Preload feature */
    tmpccmr2 |= (uint16_t)(TIM_OCPreload << 8);
    /* Write to TIMx CCMR2 register */
    TIMx->TIM_CCMR2.value = tmpccmr2;
}

void TIM_OC1FastConfig(TIM_TypeDef *TIMx, uint16_t TIM_OCFast)
{
    uint16_t tmpccmr1 = 0;
    /* Check the parameters */
    assert_param(IS_TIM_ALL_PERIPH(TIMx));
    assert_param(IS_TIM_OCFAST_STATE(TIM_OCFast));
    /* Get the TIMx CCMR1 register value */
    tmpccmr1 = TIMx->TIM_CCMR1.value;
    /* Reset the OC1FE Bit */
    tmpccmr1 &= (uint16_t)(~(uint16_t)TIM_CCMR1_TIM_CCMR1_OC1FE);
    /* Enable or Disable the Output Compare Fast Bit */
    tmpccmr1 |= TIM_OCFast;
    /* Write to TIMx CCMR1 */
    TIMx->TIM_CCMR1.value = tmpccmr1;
}

void TIM_OC2FastConfig(TIM_TypeDef *TIMx, uint16_t TIM_OCFast)
{
    uint16_t tmpccmr1 = 0;
    /* Check the parameters */
    assert_param(IS_TIM_ALL_PERIPH(TIMx));
    assert_param(IS_TIM_OCFAST_STATE(TIM_OCFast));
    /* Get the TIMx CCMR1 register value */
    tmpccmr1 = TIMx->TIM_CCMR1.value;
    /* Reset the OC2FE Bit */
    tmpccmr1 &= (uint16_t)(~(uint16_t)TIM_CCMR1_TIM_CCMR1_OC2FE);
    /* Enable or Disable the Output Compare Fast Bit */
    tmpccmr1 |= (uint16_t)(TIM_OCFast << 8);
    /* Write to TIMx CCMR1 */
    TIMx->TIM_CCMR1.value = tmpccmr1;
}

void TIM_OC3FastConfig(TIM_TypeDef *TIMx, uint16_t TIM_OCFast)
{
    uint16_t tmpccmr2 = 0;
    /* Check the parameters */
    assert_param(IS_TIM_ALL_PERIPH(TIMx));
    assert_param(IS_TIM_OCFAST_STATE(TIM_OCFast));
    /* Get the TIMx CCMR2 register value */
    tmpccmr2 = TIMx->TIM_CCMR2.value;
    /* Reset the OC3FE Bit */
    tmpccmr2 &= (uint16_t)(~(uint16_t)TIM_CCMR2_TIM_CCMR2_OC3FE);
    /* Enable or Disable the Output Compare Fast Bit */
    tmpccmr2 |= TIM_OCFast;
    /* Write to TIMx CCMR2 */
    TIMx->TIM_CCMR2.value = tmpccmr2;
}

void TIM_OC4FastConfig(TIM_TypeDef *TIMx, uint16_t TIM_OCFast)
{
    uint16_t tmpccmr2 = 0;
    /* Check the parameters */
    assert_param(IS_TIM_ALL_PERIPH(TIMx));
    assert_param(IS_TIM_OCFAST_STATE(TIM_OCFast));
    /* Get the TIMx CCMR2 register value */
    tmpccmr2 = TIMx->TIM_CCMR2.value;
    /* Reset the OC4FE Bit */
    tmpccmr2 &= (uint16_t)(~(uint16_t)TIM_CCMR2_TIM_CCMR2_OC4FE);
    /* Enable or Disable the Output Compare Fast Bit */
    tmpccmr2 |= (uint16_t)(TIM_OCFast << 8);
    /* Write to TIMx CCMR2 */
    TIMx->TIM_CCMR2.value = tmpccmr2;
}

void TIM_ClearOC1Ref(TIM_TypeDef *TIMx, uint16_t TIM_OCClear)
{
    uint16_t tmpccmr1 = 0;
    /* Check the parameters */
    assert_param(IS_TIM_ALL_PERIPH(TIMx));
    assert_param(IS_TIM_OCCLEAR_STATE(TIM_OCClear));

    tmpccmr1 = TIMx->TIM_CCMR1.value;

    /* Reset the OC1CE Bit */
    tmpccmr1 &= (uint16_t)(~(uint16_t)TIM_CCMR1_TIM_CCMR1_OC1CE);
    /* Enable or Disable the Output Compare Clear Bit */
    tmpccmr1 |= TIM_OCClear;
    /* Write to TIMx CCMR1 register */
    TIMx->TIM_CCMR1.value = tmpccmr1;
}

void TIM_ClearOC2Ref(TIM_TypeDef *TIMx, uint16_t TIM_OCClear)
{
    uint16_t tmpccmr1 = 0;
    /* Check the parameters */
    assert_param(IS_TIM_ALL_PERIPH(TIMx));
    assert_param(IS_TIM_OCCLEAR_STATE(TIM_OCClear));
    tmpccmr1 = TIMx->TIM_CCMR1.value;
    /* Reset the OC2CE Bit */
    tmpccmr1 &= (uint16_t)(~(uint16_t)TIM_CCMR1_TIM_CCMR1_OC2CE);
    /* Enable or Disable the Output Compare Clear Bit */
    tmpccmr1 |= (uint16_t)(TIM_OCClear << 8);
    /* Write to TIMx CCMR1 register */
    TIMx->TIM_CCMR1.value = tmpccmr1;
}

void TIM_ClearOC3Ref(TIM_TypeDef *TIMx, uint16_t TIM_OCClear)
{
    uint16_t tmpccmr2 = 0;
    /* Check the parameters */
    assert_param(IS_TIM_ALL_PERIPH(TIMx));
    assert_param(IS_TIM_OCCLEAR_STATE(TIM_OCClear));
    tmpccmr2 = TIMx->TIM_CCMR2.value;
    /* Reset the OC3CE Bit */
    tmpccmr2 &= (uint16_t)(~(uint16_t)TIM_CCMR2_TIM_CCMR2_OC3CE);
    /* Enable or Disable the Output Compare Clear Bit */
    tmpccmr2 |= TIM_OCClear;
    /* Write to TIMx CCMR2 register */
    TIMx->TIM_CCMR2.value = tmpccmr2;
}

void TIM_ClearOC4Ref(TIM_TypeDef *TIMx, uint16_t TIM_OCClear)
{
    uint16_t tmpccmr2 = 0;
    /* Check the parameters */
    assert_param(IS_TIM_ALL_PERIPH(TIMx));
    assert_param(IS_TIM_OCCLEAR_STATE(TIM_OCClear));
    tmpccmr2 = TIMx->TIM_CCMR2.value;
    /* Reset the OC4CE Bit */
    tmpccmr2 &= (uint16_t)(~(uint16_t)TIM_CCMR2_TIM_CCMR2_OC4CE);
    /* Enable or Disable the Output Compare Clear Bit */
    tmpccmr2 |= (uint16_t)(TIM_OCClear << 8);
    /* Write to TIMx CCMR2 register */
    TIMx->TIM_CCMR2.value = tmpccmr2;
}

void TIM_OC1PolarityConfig(TIM_TypeDef *TIMx, uint16_t TIM_OCPolarity)
{
    uint16_t tmpccer = 0;
    /* Check the parameters */
    assert_param(IS_TIM_ALL_PERIPH(TIMx));
    assert_param(IS_TIM_OC_POLARITY(TIM_OCPolarity));
    tmpccer = TIMx->TIM_CCER.value;
    /* Set or Reset the CC1P Bit */
    tmpccer &= (uint16_t)(~(uint16_t)TIM_CCER_TIM_CCER_CC1P);
    tmpccer |= TIM_OCPolarity;
    /* Write to TIMx CCER register */
    TIMx->TIM_CCER.value = tmpccer;
}

void TIM_OC1NPolarityConfig(TIM_TypeDef *TIMx, uint16_t TIM_OCNPolarity)
{
    uint16_t tmpccer = 0;
    /* Check the parameters */
    assert_param(IS_TIM_LIST1_PERIPH(TIMx));
    assert_param(IS_TIM_OCN_POLARITY(TIM_OCNPolarity));

    tmpccer = TIMx->TIM_CCER.value;
    /* Set or Reset the CC1NP Bit */
    tmpccer &= (uint16_t)(~(uint16_t)TIM_CCER_TIM_CCER_CC1NP);
    tmpccer |= TIM_OCNPolarity;
    /* Write to TIMx CCER register */
    TIMx->TIM_CCER.value = tmpccer;
}

void TIM_OC2PolarityConfig(TIM_TypeDef *TIMx, uint16_t TIM_OCPolarity)
{
    uint16_t tmpccer = 0;
    /* Check the parameters */
    assert_param(IS_TIM_ALL_PERIPH(TIMx));
    assert_param(IS_TIM_OC_POLARITY(TIM_OCPolarity));
    tmpccer = TIMx->TIM_CCER.value;
    /* Set or Reset the CC2P Bit */
    tmpccer &= (uint16_t)(~(uint16_t)TIM_CCER_TIM_CCER_CC2P);
    tmpccer |= (uint16_t)(TIM_OCPolarity << 4);
    /* Write to TIMx CCER register */
    TIMx->TIM_CCER.value = tmpccer;
}

void TIM_OC2NPolarityConfig(TIM_TypeDef *TIMx, uint16_t TIM_OCNPolarity)
{
    uint16_t tmpccer = 0;
    /* Check the parameters */
    assert_param(IS_TIM_LIST1_PERIPH(TIMx));
    assert_param(IS_TIM_OCN_POLARITY(TIM_OCNPolarity));

    tmpccer = TIMx->TIM_CCER.value;
    /* Set or Reset the CC2NP Bit */
    tmpccer &= (uint16_t)(~(uint16_t)TIM_CCER_TIM_CCER_CC2NP);
    tmpccer |= (uint16_t)(TIM_OCNPolarity << 4);

    /* Write to TIMx CCER register */
    TIMx->TIM_CCER.value = tmpccer;
}

void TIM_OC3PolarityConfig(TIM_TypeDef *TIMx, uint16_t TIM_OCPolarity)
{
    uint16_t tmpccer = 0;
    /* Check the parameters */
    assert_param(IS_TIM_ALL_PERIPH(TIMx));
    assert_param(IS_TIM_OC_POLARITY(TIM_OCPolarity));
    tmpccer = TIMx->TIM_CCER.value;
    /* Set or Reset the CC3P Bit */
    tmpccer &= (uint16_t)(~(uint16_t)TIM_CCER_TIM_CCER_CC3P);
    tmpccer |= (uint16_t)(TIM_OCPolarity << 8);
    /* Write to TIMx CCER register */
    TIMx->TIM_CCER.value = tmpccer;
}

void TIM_OC3NPolarityConfig(TIM_TypeDef *TIMx, uint16_t TIM_OCNPolarity)
{
    uint16_t tmpccer = 0;

    /* Check the parameters */
    assert_param(IS_TIM_LIST1_PERIPH(TIMx));
    assert_param(IS_TIM_OCN_POLARITY(TIM_OCNPolarity));

    tmpccer = TIMx->TIM_CCER.value;
    /* Set or Reset the CC3NP Bit */
    tmpccer &= (uint16_t)(~(uint16_t)TIM_CCER_TIM_CCER_CC3NP);
    tmpccer |= (uint16_t)(TIM_OCNPolarity << 8);
    /* Write to TIMx CCER register */
    TIMx->TIM_CCER.value = tmpccer;
}

void TIM_OC4PolarityConfig(TIM_TypeDef *TIMx, uint16_t TIM_OCPolarity)
{
    uint16_t tmpccer = 0;
    /* Check the parameters */
    assert_param(IS_TIM_ALL_PERIPH(TIMx));
    assert_param(IS_TIM_OC_POLARITY(TIM_OCPolarity));
    tmpccer = TIMx->TIM_CCER.value;
    /* Set or Reset the CC4P Bit */
    tmpccer &= (uint16_t)(~(uint16_t)TIM_CCER_TIM_CCER_CC4P);
    tmpccer |= (uint16_t)(TIM_OCPolarity << 12);
    /* Write to TIMx CCER register */
    TIMx->TIM_CCER.value = tmpccer;
}

void TIM_CCxCmd(TIM_TypeDef *TIMx, uint16_t TIM_Channel, uint16_t TIM_CCx)
{
    uint16_t tmp = 0;

    /* Check the parameters */
    assert_param(IS_TIM_ALL_PERIPH(TIMx));
    assert_param(IS_TIM_CHANNEL(TIM_Channel));
    assert_param(IS_TIM_CCX(TIM_CCx));

    tmp = CCER_CCE_Set << TIM_Channel;

    /* Reset the CCxE Bit */
    TIMx->TIM_CCER.value &= (uint16_t)(~tmp);

    /* Set or reset the CCxE Bit */
    TIMx->TIM_CCER.value |= (uint16_t)(TIM_CCx << TIM_Channel);
}

void TIM_CCxNCmd(TIM_TypeDef *TIMx, uint16_t TIM_Channel, uint16_t TIM_CCxN)
{
    uint16_t tmp = 0;

    /* Check the parameters */
    assert_param(IS_TIM_LIST1_PERIPH(TIMx));
    assert_param(IS_TIM_COMPLEMENTARY_CHANNEL(TIM_Channel));
    assert_param(IS_TIM_CCXN(TIM_CCxN));

    tmp = CCER_CCNE_Set << TIM_Channel;

    /* Reset the CCxNE Bit */
    TIMx->TIM_CCER.value &= (uint16_t)(~tmp);

    /* Set or reset the CCxNE Bit */
    TIMx->TIM_CCER.value |= (uint16_t)(TIM_CCxN << TIM_Channel);
}

void TIM_SelectOCxM(TIM_TypeDef *TIMx, uint16_t TIM_Channel, uint16_t TIM_OCMode)
{
    uint32_t tmp  = 0;
    uint16_t tmp1 = 0;

    /* Check the parameters */
    assert_param(IS_TIM_ALL_PERIPH(TIMx));
    assert_param(IS_TIM_CHANNEL(TIM_Channel));
    assert_param(IS_TIM_OCM(TIM_OCMode));

    tmp = (uint32_t)(TIMx);
    tmp += CCMR_Offset;

    tmp1 = CCER_CCE_Set << (uint16_t)(TIM_Channel);

    /* Disable the Channel: Reset the CCxE Bit */
    TIMx->TIM_CCER.value &= (uint16_t)(~tmp1);

    if ((TIM_Channel == TIM_Channel_1) || (TIM_Channel == TIM_Channel_3)) {
        tmp += (TIM_Channel >> 1);

        /* Reset the OCxM bits in the CCMRx register */
        *(__IO uint32_t *)tmp &= ~((uint32_t)TIM_CCMR1_TIM_CCMR1_OC1M);

        /* Configure the OCxM bits in the CCMRx register */
        *(__IO uint32_t *)tmp |= TIM_OCMode;
    } else {
        tmp += (uint16_t)(TIM_Channel - (uint16_t)4) >> (uint16_t)1;

        /* Reset the OCxM bits in the CCMRx register */
        *(__IO uint32_t *)tmp &= ~((uint32_t)TIM_CCMR1_TIM_CCMR1_OC2M);

        /* Configure the OCxM bits in the CCMRx register */
        *(__IO uint32_t *)tmp |= (uint16_t)(TIM_OCMode << 8);
    }
}

void TIM_UpdateDisableConfig(TIM_TypeDef *TIMx, FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_TIM_ALL_PERIPH(TIMx));
    assert_param(IS_FUNCTIONAL_STATE(NewState));
    if (NewState != DISABLE) {
        /* Set the Update Disable Bit */
        TIMx->TIM_CR1.TIM_CR1_UDIS = 1;
    } else {
        /* Reset the Update Disable Bit */
        TIMx->TIM_CR1.TIM_CR1_UDIS = 0;
    }
}

void TIM_UpdateRequestConfig(TIM_TypeDef *TIMx, uint16_t TIM_UpdateSource)
{
    /* Check the parameters */
    assert_param(IS_TIM_ALL_PERIPH(TIMx));
    assert_param(IS_TIM_UPDATE_SOURCE(TIM_UpdateSource));
    if (TIM_UpdateSource != TIM_UpdateSource_Global) {
        /* Set the URS Bit */
        TIMx->TIM_CR1.TIM_CR1_URS = 1;
    } else {
        /* Reset the URS Bit */
        TIMx->TIM_CR1.TIM_CR1_URS = 0;
    }
}

void TIM_SelectHallSensor(TIM_TypeDef *TIMx, FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_TIM_ALL_PERIPH(TIMx));
    assert_param(IS_FUNCTIONAL_STATE(NewState));
    if (NewState != DISABLE) {
        /* Set the TI1S Bit */
        TIMx->TIM_CR2.TIM_CR2_TI1S = 1;
    } else {
        /* Reset the TI1S Bit */
        TIMx->TIM_CR2.TIM_CR2_TI1S = 0;
    }
}

void TIM_SelectOnePulseMode(TIM_TypeDef *TIMx, uint16_t TIM_OPMode)
{
    /* Check the parameters */
    assert_param(IS_TIM_ALL_PERIPH(TIMx));
    assert_param(IS_TIM_OPM_MODE(TIM_OPMode));
    /* Reset the OPM Bit */
    TIMx->TIM_CR1.TIM_CR1_OPM = 0;
    /* Configure the OPM Mode */
    TIMx->TIM_CR1.value |= TIM_OPMode;
}

void TIM_SelectOutputTrigger(TIM_TypeDef *TIMx, uint16_t TIM_TRGOSource)
{
    /* Check the parameters */
    assert_param(IS_TIM_ALL_PERIPH(TIMx));
    assert_param(IS_TIM_TRGO_SOURCE(TIM_TRGOSource));
    /* Reset the MMS Bits */
    TIMx->TIM_CR2.TIM_CR2_MMS = 0;
    /* Select the TRGO source */
    TIMx->TIM_CR2.value |= TIM_TRGOSource;
}

void TIM_SelectSlaveMode(TIM_TypeDef *TIMx, uint16_t TIM_SlaveMode)
{
    /* Check the parameters */
    assert_param(IS_TIM_ALL_PERIPH(TIMx));
    assert_param(IS_TIM_SLAVE_MODE(TIM_SlaveMode));
    /* Reset the SMS Bits */
    TIMx->TIM_SMCR.TIM_SMCR_SMS = 0;
    /* Select the Slave Mode */
    TIMx->TIM_SMCR.value |= TIM_SlaveMode;
}

void TIM_SelectMasterSlaveMode(TIM_TypeDef *TIMx, uint16_t TIM_MasterSlaveMode)
{
    /* Check the parameters */
    assert_param(IS_TIM_ALL_PERIPH(TIMx));
    assert_param(IS_TIM_MSM_STATE(TIM_MasterSlaveMode));
    /* Reset the MSM Bit */
    TIMx->TIM_SMCR.TIM_SMCR_MSM = 0;

    /* Set or Reset the MSM Bit */
    TIMx->TIM_SMCR.value |= TIM_MasterSlaveMode;
}

void TIM_SetCounter(TIM_TypeDef *TIMx, uint16_t Counter)
{
    /* Check the parameters */
    assert_param(IS_TIM_ALL_PERIPH(TIMx));
    /* Set the Counter Register value */
    TIMx->TIM_CNT.value = Counter;
}

void TIM_SetAutoreload(TIM_TypeDef *TIMx, uint16_t Autoreload)
{
    /* Check the parameters */
    assert_param(IS_TIM_ALL_PERIPH(TIMx));
    /* Set the Autoreload Register value */
    TIMx->TIM_ARR.value = Autoreload;
}

void TIM_SetCompare1(TIM_TypeDef *TIMx, uint16_t Compare1)
{
    /* Check the parameters */
    assert_param(IS_TIM_ALL_PERIPH(TIMx));
    /* Set the Capture Compare1 Register value */
    TIMx->TIM_CCR1.value = Compare1;
}

void TIM_SetCompare2(TIM_TypeDef *TIMx, uint16_t Compare2)
{
    /* Check the parameters */
    assert_param(IS_TIM_ALL_PERIPH(TIMx));
    /* Set the Capture Compare2 Register value */
    TIMx->TIM_CCR2.value = Compare2;
}

void TIM_SetCompare3(TIM_TypeDef *TIMx, uint16_t Compare3)
{
    /* Check the parameters */
    assert_param(IS_TIM_ALL_PERIPH(TIMx));
    /* Set the Capture Compare3 Register value */
    TIMx->TIM_CCR3.value = Compare3;
}

void TIM_SetCompare4(TIM_TypeDef *TIMx, uint16_t Compare4)
{
    /* Check the parameters */
    assert_param(IS_TIM_ALL_PERIPH(TIMx));
    /* Set the Capture Compare4 Register value */
    TIMx->TIM_CCR4.value = Compare4;
}

void TIM_SetIC1Prescaler(TIM_TypeDef *TIMx, uint16_t TIM_ICPSC)
{
    /* Check the parameters */
    assert_param(IS_TIM_ALL_PERIPH(TIMx));
    assert_param(IS_TIM_IC_PRESCALER(TIM_ICPSC));
    /* Reset the IC1PSC Bits */
    TIMx->TIM_CCMR1.TIM_CCMR1_IC1PSC = 0;
    /* Set the IC1PSC value */
    TIMx->TIM_CCMR1.value |= TIM_ICPSC;
}

void TIM_SetIC2Prescaler(TIM_TypeDef *TIMx, uint16_t TIM_ICPSC)
{
    /* Check the parameters */
    assert_param(IS_TIM_ALL_PERIPH(TIMx));
    assert_param(IS_TIM_IC_PRESCALER(TIM_ICPSC));
    /* Reset the IC2PSC Bits */
    TIMx->TIM_CCMR1.TIM_CCMR1_IC2PSC = 0;
    /* Set the IC2PSC value */
    TIMx->TIM_CCMR1.value |= (uint16_t)(TIM_ICPSC << 8);
}

void TIM_SetIC3Prescaler(TIM_TypeDef *TIMx, uint16_t TIM_ICPSC)
{
    /* Check the parameters */
    assert_param(IS_TIM_ALL_PERIPH(TIMx));
    assert_param(IS_TIM_IC_PRESCALER(TIM_ICPSC));
    /* Reset the IC3PSC Bits */
    TIMx->TIM_CCMR2.TIM_CCMR2_IC3PSC = 0;
    /* Set the IC3PSC value */
    TIMx->TIM_CCMR2.value |= TIM_ICPSC;
}

void TIM_SetIC4Prescaler(TIM_TypeDef *TIMx, uint16_t TIM_ICPSC)
{
    /* Check the parameters */
    assert_param(IS_TIM_ALL_PERIPH(TIMx));
    assert_param(IS_TIM_IC_PRESCALER(TIM_ICPSC));
    /* Reset the IC4PSC Bits */
    TIMx->TIM_CCMR2.TIM_CCMR2_IC4PSC = 0;
    /* Set the IC4PSC value */
    TIMx->TIM_CCMR2.value |= (uint16_t)(TIM_ICPSC << 8);
}

void TIM_SetClockDivision(TIM_TypeDef *TIMx, uint16_t TIM_CKD)
{
    /* Check the parameters */
    assert_param(IS_TIM_ALL_PERIPH(TIMx));
    assert_param(IS_TIM_CKD_DIV(TIM_CKD));
    /* Reset the CKD Bits */
    TIMx->TIM_CR1.TIM_CR1_CKD = 0;
    /* Set the CKD value */
    TIMx->TIM_CR1.value |= TIM_CKD;
}

uint16_t TIM_GetCapture1(TIM_TypeDef *TIMx)
{
    /* Check the parameters */
    assert_param(IS_TIM_ALL_PERIPH(TIMx));
    /* Get the Capture 1 Register value */
    return TIMx->TIM_CCR1.value;
}

uint16_t TIM_GetCapture2(TIM_TypeDef *TIMx)
{
    /* Check the parameters */
    assert_param(IS_TIM_ALL_PERIPH(TIMx));
    /* Get the Capture 2 Register value */
    return TIMx->TIM_CCR2.value;
}

uint16_t TIM_GetCapture3(TIM_TypeDef *TIMx)
{
    /* Check the parameters */
    assert_param(IS_TIM_ALL_PERIPH(TIMx));
    /* Get the Capture 3 Register value */
    return TIMx->TIM_CCR3.value;
}

uint16_t TIM_GetCapture4(TIM_TypeDef *TIMx)
{
    /* Check the parameters */
    assert_param(IS_TIM_ALL_PERIPH(TIMx));
    /* Get the Capture 4 Register value */
    return TIMx->TIM_CCR4.value;
}

uint16_t TIM_GetCounter(TIM_TypeDef *TIMx)
{
    /* Check the parameters */
    assert_param(IS_TIM_ALL_PERIPH(TIMx));
    /* Get the Counter Register value */
    return TIMx->TIM_CNT.value;
}

uint16_t TIM_GetPrescaler(TIM_TypeDef *TIMx)
{
    /* Check the parameters */
    assert_param(IS_TIM_ALL_PERIPH(TIMx));
    /* Get the Prescaler Register value */
    return TIMx->TIM_PSC.value;
}

FlagStatus TIM_GetFlagStatus(TIM_TypeDef *TIMx, uint16_t TIM_FLAG)
{
    ITStatus bitstatus = RESET;
    /* Check the parameters */
    assert_param(IS_TIM_ALL_PERIPH(TIMx));
    assert_param(IS_TIM_GET_FLAG(TIM_FLAG));

    if ((TIMx->TIM_SR.value & TIM_FLAG) != RESET) {
        bitstatus = SET;
    } else {
        bitstatus = RESET;
    }
    return bitstatus;
}

void TIM_ClearFlag(TIM_TypeDef *TIMx, uint16_t TIM_FLAG)
{
    /* Check the parameters */
    assert_param(IS_TIM_ALL_PERIPH(TIMx));
    assert_param(IS_TIM_CLEAR_FLAG(TIM_FLAG));

    /* Clear the flags */
    TIMx->TIM_SR.value = ~TIM_FLAG;
}

ITStatus TIM_GetITStatus(TIM_TypeDef *TIMx, uint16_t TIM_IT)
{
    ITStatus bitstatus = RESET;
    uint16_t itstatus = 0x0, itenable = 0x0;
    /* Check the parameters */
    assert_param(IS_TIM_ALL_PERIPH(TIMx));
    assert_param(IS_TIM_GET_IT(TIM_IT));

    itstatus = TIMx->TIM_SR.value & TIM_IT;

    itenable = TIMx->TIM_DIER.value & TIM_IT;
    if ((itstatus != (uint16_t)RESET) && (itenable != (uint16_t)RESET)) {
        bitstatus = SET;
    } else {
        bitstatus = RESET;
    }
    return bitstatus;
}

void TIM_ClearITPendingBit(TIM_TypeDef *TIMx, uint16_t TIM_IT)
{
    /* Check the parameters */
    assert_param(IS_TIM_ALL_PERIPH(TIMx));
    assert_param(IS_TIM_IT(TIM_IT));
    /* Clear the IT pending Bit */
    TIMx->TIM_SR.value = ~TIM_IT;
}

/**
 * @brief  Configure the TI1 as Input.
 * @param[in] TIMx: where x can be 1 to 4 to select the TIMx peripheral
 * @param[in] TIM_ICPolarity: The Input Polarity.
 * @param[in] TIM_ICSelection: specifies the input to be used.
 * @param[in] TIM_ICFilter: Specifies the Input Capture Filter.
 */
static void TI1_Config(TIM_TypeDef *TIMx, uint16_t TIM_ICPolarity, uint16_t TIM_ICSelection,
                       uint16_t TIM_ICFilter)
{
    uint16_t tmpccmr1 = 0, tmpccer = 0;

    /* Disable the Channel 1: Reset the CC1E Bit */
    TIMx->TIM_CCER.TIM_CCER_CC1E = 0;

    tmpccmr1 = TIMx->TIM_CCMR1.value;
    tmpccer  = TIMx->TIM_CCER.value;
    /* Select the Input and set the filter */
    tmpccmr1 &= (uint16_t)(((uint16_t) ~((uint16_t)TIM_CCMR1_TIM_CCMR1_CC1S2)) & ((uint16_t) ~((uint16_t)TIM_CCMR1_TIM_CCMR1_IC1F)));
    tmpccmr1 |= (uint16_t)(TIM_ICSelection | (uint16_t)(TIM_ICFilter << (uint16_t)4));

    /* Select the Polarity and set the CC1E Bit */
    tmpccer &= (uint16_t) ~((uint16_t)TIM_CCER_TIM_CCER_CC1P);
    tmpccer |= (uint16_t)(TIM_ICPolarity | TIM_CCER_TIM_CCER_CC1E);

    /* Write to TIMx CCMR1 and CCER registers */
    TIMx->TIM_CCMR1.value = tmpccmr1;
    TIMx->TIM_CCER.value  = tmpccer;
}

/**
 * @brief Configure the TI2 as Input.
 * @param[in] TIMx: where x can be 1 to 4 to select the TIMx peripheral
 * @param[in] TIM_ICPolarity: The Input Polarity.
 * @param[in] TIM_ICSelection: specifies the input to be used.
 * @param[in] TIM_ICFilter: Specifies the Input Capture Filter.
 */
static void TI2_Config(TIM_TypeDef *TIMx, uint16_t TIM_ICPolarity, uint16_t TIM_ICSelection,
                       uint16_t TIM_ICFilter)
{
    uint16_t tmpccmr1 = 0, tmpccer = 0, tmp = 0;

    /* Disable the Channel 2: Reset the CC2E Bit */
    TIMx->TIM_CCER.TIM_CCER_CC2E = 0;

    tmpccmr1 = TIMx->TIM_CCMR1.value;
    tmpccer  = TIMx->TIM_CCER.value;
    tmp      = (uint16_t)(TIM_ICPolarity << 4);
    /* Select the Input and set the filter */
    tmpccmr1 &= (uint16_t)(((uint16_t) ~((uint16_t)TIM_CCMR1_TIM_CCMR1_CC2S2)) & ((uint16_t) ~((uint16_t)TIM_CCMR1_TIM_CCMR1_IC2F)));
    tmpccmr1 |= (uint16_t)(TIM_ICFilter << 12);
    tmpccmr1 |= (uint16_t)(TIM_ICSelection << 8);

    /* Select the Polarity and set the CC2E Bit */
    tmpccer &= (uint16_t) ~((uint16_t)TIM_CCER_TIM_CCER_CC2P);
    tmpccer |= (uint16_t)(tmp | TIM_CCER_TIM_CCER_CC2E);

    /* Write to TIMx CCMR1 and CCER registers */
    TIMx->TIM_CCMR1.value = tmpccmr1;
    TIMx->TIM_CCER.value  = tmpccer;
}

/**
 * @brief Configure the TI3 as Input.
 * @param[in] TIMx: where x can be 1 to 4 to select the TIMx peripheral
 * @param[in] TIM_ICPolarity: The Input Polarity.
 * @param[in] TIM_ICSelection: specifies the input to be used.
 * @param[in] TIM_ICFilter: Specifies the Input Capture Filter.
 */
static void TI3_Config(TIM_TypeDef *TIMx, uint16_t TIM_ICPolarity, uint16_t TIM_ICSelection,
                       uint16_t TIM_ICFilter)
{
    uint16_t tmpccmr2 = 0, tmpccer = 0, tmp = 0;
    /* Disable the Channel 3: Reset the CC3E Bit */
    TIMx->TIM_CCER.TIM_CCER_CC3E = 0;

    tmpccmr2 = TIMx->TIM_CCMR2.value;
    tmpccer  = TIMx->TIM_CCER.value;
    tmp      = (uint16_t)(TIM_ICPolarity << 8);
    /* Select the Input and set the filter */
    tmpccmr2 &= (uint16_t)((uint16_t)(~((uint16_t)TIM_CCMR2_TIM_CCMR2_CC3S2)) & ((uint16_t) ~((uint16_t)TIM_CCMR2_TIM_CCMR2_IC3F)));
    tmpccmr2 |= (uint16_t)(TIM_ICSelection | (uint16_t)(TIM_ICFilter << (uint16_t)4));

    /* Select the Polarity and set the CC3E Bit */
    tmpccer &= ~((uint16_t)TIM_CCER_TIM_CCER_CC3P);
    tmpccer |= (uint16_t)(tmp | TIM_CCER_TIM_CCER_CC3E);

    /* Write to TIMx CCMR2 and CCER registers */
    TIMx->TIM_CCMR2.value = tmpccmr2;
    TIMx->TIM_CCER.value  = tmpccer;
}

/**
 * @brief Configure the TI4 as Input.
 * @param[in] TIMx: where x can be 1 to 4 to select the TIMx peripheral
 * @param[in] TIM_ICPolarity: The Input Polarity.
 * @param[in] TIM_ICSelection: specifies the input to be used.
 * @param[in] TIM_ICFilter: Specifies the Input Capture Filter.
 */
static void TI4_Config(TIM_TypeDef *TIMx, uint16_t TIM_ICPolarity, uint16_t TIM_ICSelection,
                       uint16_t TIM_ICFilter)
{
    uint16_t tmpccmr2 = 0, tmpccer = 0, tmp = 0;

    /* Disable the Channel 4: Reset the CC4E Bit */
    TIMx->TIM_CCER.TIM_CCER_CC4E = 0;

    tmpccmr2 = TIMx->TIM_CCMR2.value;
    tmpccer  = TIMx->TIM_CCER.value;
    tmp      = (uint16_t)(TIM_ICPolarity << 12);
    /* Select the Input and set the filter */
    tmpccmr2 &= (uint16_t)((uint16_t)(~(uint16_t)TIM_CCMR2_TIM_CCMR2_CC4S2) & ((uint16_t) ~((uint16_t)TIM_CCMR2_TIM_CCMR2_IC4F)));
    tmpccmr2 |= (uint16_t)(TIM_ICSelection << 8);
    tmpccmr2 |= (uint16_t)(TIM_ICFilter << 12);

    /* Select the Polarity and set the CC4E Bit */
    tmpccer &= ~((uint16_t)TIM_CCER_TIM_CCER_CC4P);
    tmpccer |= (uint16_t)(tmp | TIM_CCER_TIM_CCER_CC4E);

    /* Write to TIMx CCMR2 and CCER registers */
    TIMx->TIM_CCMR2.value = tmpccmr2;
    TIMx->TIM_CCER.value  = tmpccer;
}
#endif