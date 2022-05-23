/**
 * @file at103_dma.c
 * @brief This file provides all the DMA firmware functions.
 * @author Dong Qin (dong.qin@timesintelli.com)
 * @version 1.0
 * @date 2022-03-16
 * @copyright Copyright (c) 2022 Timesintelli, Inc
 */
#include "at103_dma.h"

#ifdef DMA_MODULE_ENABLED

#define IS_DMA_ALL_PERIPH(PERIPH) (((PERIPH) == DMA_Channel1) || \
                                   ((PERIPH) == DMA_Channel2) || \
                                   ((PERIPH) == DMA_Channel3) || \
                                   ((PERIPH) == DMA_Channel4) || \
                                   ((PERIPH) == DMA_Channel5) || \
                                   ((PERIPH) == DMA_Channel6) || \
                                   ((PERIPH) == DMA_Channel7) || \
                                   ((PERIPH) == DMA_Channel8))

#define IS_DMA_DIR(DIR) (((DIR) == DMA_DIR_PeripheralDST) || \
                         ((DIR) == DMA_DIR_PeripheralSRC))

#define IS_DMA_PERIPHERAL_INC_STATE(STATE) (((STATE) == DMA_PeripheralInc_Enable) || \
                                            ((STATE) == DMA_PeripheralInc_Disable))

#define IS_DMA_MEMORY_INC_STATE(STATE) (((STATE) == DMA_MemoryInc_Enable) || \
                                        ((STATE) == DMA_MemoryInc_Disable))

#define IS_DMA_PERIPHERAL_DATA_SIZE(SIZE) (((SIZE) == DMA_PeripheralDataSize_Byte) || \
                                           ((SIZE) == DMA_PeripheralDataSize_HalfWord) || \
                                           ((SIZE) == DMA_PeripheralDataSize_Word))

#define IS_DMA_MEMORY_DATA_SIZE(SIZE) (((SIZE) == DMA_MemoryDataSize_Byte) || \
                                       ((SIZE) == DMA_MemoryDataSize_HalfWord) || \
                                       ((SIZE) == DMA_MemoryDataSize_Word))

#define IS_DMA_MODE(MODE) (((MODE) == DMA_Mode_Circular) || ((MODE) == DMA_Mode_Normal))

#define IS_DMA_PRIORITY(PRIORITY) (((PRIORITY) == DMA_Priority_VeryHigh) || \
                                   ((PRIORITY) == DMA_Priority_High) || \
                                   ((PRIORITY) == DMA_Priority_Medium) || \
                                   ((PRIORITY) == DMA_Priority_Low))

#define IS_DMA_M2M_STATE(STATE) (((STATE) == DMA_M2M_Enable) || ((STATE) == DMA_M2M_Disable))

#define IS_DMA_CONFIG_IT(IT) ((((IT)&0xFFFFFFFC) == 0x00) && ((IT) != 0x00))

#define IS_DMA_CLEAR_IT(IT) ((((IT)&0xFFFF0000) == 0x00) && ((IT) != 0x00))

#define IS_DMA_GET_IT(IT) (((IT) == DMA_IT_GL1) || ((IT) == DMA_IT_TC1) || ((IT) == DMA_IT_TE1) || \
                           ((IT) == DMA_IT_GL2) || ((IT) == DMA_IT_TC2) || ((IT) == DMA_IT_TE2) || \
                           ((IT) == DMA_IT_GL3) || ((IT) == DMA_IT_TC3) || ((IT) == DMA_IT_TE3) || \
                           ((IT) == DMA_IT_GL4) || ((IT) == DMA_IT_TC4) || ((IT) == DMA_IT_TE4) || \
                           ((IT) == DMA_IT_GL5) || ((IT) == DMA_IT_TC5) || ((IT) == DMA_IT_TE5) || \
                           ((IT) == DMA_IT_GL6) || ((IT) == DMA_IT_TC6) || ((IT) == DMA_IT_TE6) || \
                           ((IT) == DMA_IT_GL7) || ((IT) == DMA_IT_TC7) || ((IT) == DMA_IT_TE7) || \
                           ((IT) == DMA_IT_GL8) || ((IT) == DMA_IT_TC8) || ((IT) == DMA_IT_TE8))

#define IS_DMA_CLEAR_FLAG(FLAG) ((((FLAG)&0xFFFF0000) == 0x00) && ((FLAG) != 0x00))

#define IS_DMA_GET_FLAG(FLAG) (((FLAG) == DMA_FLAG_GL1) || ((FLAG) == DMA_FLAG_TC1) || ((FLAG) == DMA_FLAG_TE1) || \
                               ((FLAG) == DMA_FLAG_GL2) || ((FLAG) == DMA_FLAG_TC2) || ((FLAG) == DMA_FLAG_TE2) || \
                               ((FLAG) == DMA_FLAG_GL3) || ((FLAG) == DMA_FLAG_TC3) || ((FLAG) == DMA_FLAG_TE3) || \
                               ((FLAG) == DMA_FLAG_GL4) || ((FLAG) == DMA_FLAG_TC4) || ((FLAG) == DMA_FLAG_TE4) || \
                               ((FLAG) == DMA_FLAG_GL5) || ((FLAG) == DMA_FLAG_TC5) || ((FLAG) == DMA_FLAG_TE5) || \
                               ((FLAG) == DMA_FLAG_GL6) || ((FLAG) == DMA_FLAG_TC6) || ((FLAG) == DMA_FLAG_TE6) || \
                               ((FLAG) == DMA_FLAG_GL7) || ((FLAG) == DMA_FLAG_TC7) || ((FLAG) == DMA_FLAG_TE7) || \
                               ((FLAG) == DMA_FLAG_GL8) || ((FLAG) == DMA_FLAG_TC8) || ((FLAG) == DMA_FLAG_TE8))

#define IS_DMA_PERIPHERAL_HANDSHAKE(HANDSHAKE) (((HANDSHAKE) == DMA_PeripheralHandshake_ADC1) || \
                                                ((HANDSHAKE) == DMA_PeripheralHandshake_SPI1_TX) || ((HANDSHAKE) == DMA_PeripheralHandshake_SPI1_RX) || \
                                                ((HANDSHAKE) == DMA_PeripheralHandshake_SPI2_TX) || ((HANDSHAKE) == DMA_PeripheralHandshake_SPI2_RX) || \
                                                ((HANDSHAKE) == DMA_PeripheralHandshake_USART1_TX) || ((HANDSHAKE) == DMA_PeripheralHandshake_USART1_RX) || \
                                                ((HANDSHAKE) == DMA_PeripheralHandshake_USART2_TX) || ((HANDSHAKE) == DMA_PeripheralHandshake_USART2_RX) || \
                                                ((HANDSHAKE) == DMA_PeripheralHandshake_USART3_TX) || ((HANDSHAKE) == DMA_PeripheralHandshake_USART3_RX) || \
                                                ((HANDSHAKE) == DMA_PeripheralHandshake_I2C1_TX) || ((HANDSHAKE) == DMA_PeripheralHandshake_I2C1_RX) || \
                                                ((HANDSHAKE) == DMA_PeripheralHandshake_I2C2_TX) || ((HANDSHAKE) == DMA_PeripheralHandshake_I2C2_RX) || \
                                                ((HANDSHAKE) == DMA_PeripheralHandshake_TIM1_CH1) || ((HANDSHAKE) == DMA_PeripheralHandshake_TIM1_UP) || \
                                                ((HANDSHAKE) == DMA_PeripheralHandshake_TIM1_CH2) || ((HANDSHAKE) == DMA_PeripheralHandshake_TIM1_TRIG) || \
                                                ((HANDSHAKE) == DMA_PeripheralHandshake_TIM1_CH3) || ((HANDSHAKE) == DMA_PeripheralHandshake_TIM1_COM) || \
                                                ((HANDSHAKE) == DMA_PeripheralHandshake_TIM1_CH4) || ((HANDSHAKE) == DMA_PeripheralHandshake_TIM2_UP) || \
                                                ((HANDSHAKE) == DMA_PeripheralHandshake_TIM2_CH1) || ((HANDSHAKE) == DMA_PeripheralHandshake_TIM2_CH2) || \
                                                ((HANDSHAKE) == DMA_PeripheralHandshake_TIM2_CH3) || ((HANDSHAKE) == DMA_PeripheralHandshake_TIM2_CH4) || \
                                                ((HANDSHAKE) == DMA_PeripheralHandshake_TIM3_UP) || ((HANDSHAKE) == DMA_PeripheralHandshake_TIM3_TRIG) || \
                                                ((HANDSHAKE) == DMA_PeripheralHandshake_TIM3_CH1) || ((HANDSHAKE) == DMA_PeripheralHandshake_TIM3_CH3) || \
                                                ((HANDSHAKE) == DMA_PeripheralHandshake_TIM3_CH4) || ((HANDSHAKE) == DMA_PeripheralHandshake_TIM4_UP) || \
                                                ((HANDSHAKE) == DMA_PeripheralHandshake_TIM4_CH1) || ((HANDSHAKE) == DMA_PeripheralHandshake_TIM4_CH2) || \
                                                ((HANDSHAKE) == DMA_PeripheralHandshake_TIM4_CH3))

#define IS_DMA_BUFFER_SIZE(SIZE) (((SIZE) >= 0x1) && ((SIZE) < 0x1000))

void DMA_DeInit(DMA_Channel_TypeDef *DMA_Channelx)
{
    uint32_t ch = 0;

    /* Check the parameters */
    assert_param(IS_DMA_ALL_PERIPH(DMA_Channelx));

    if (DMA_Channelx == DMA_Channel1) {
        ch = 0;
    } else if (DMA_Channelx == DMA_Channel2) {
        ch = 1;
    } else if (DMA_Channelx == DMA_Channel3) {
        ch = 2;
    } else if (DMA_Channelx == DMA_Channel4) {
        ch = 3;
    } else if (DMA_Channelx == DMA_Channel5) {
        ch = 4;
    } else if (DMA_Channelx == DMA_Channel6) {
        ch = 5;
    } else if (DMA_Channelx == DMA_Channel7) {
        ch = 6;
    } else if (DMA_Channelx == DMA_Channel8) {
        ch = 7;
    }

    /* Reset DMA Channelx control register */
    DMA_Channelx->CTL.value[0] = 0;
    DMA_Channelx->CTL.value[1] = 0;
    DMA_Channelx->CFG.value[0] = 0;
    DMA_Channelx->CFG.value[1] = 0;

    /* Reset DMA Channelx address register */
    DMA_Channelx->SAR.ADDR = 0;
    DMA_Channelx->DAR.ADDR = 0;
    DMA_Channelx->LLP.LOC  = 0;

    DMA->CH_EN_REG.value[0]      = (1 << (0 + 8));
    DMA->CLEAR_BLOCK.value[0]    = 1 << ch;
    DMA->CLEAR_TFR.value[0]      = 1 << ch;
    DMA->CLEAR_ERR.value[0]      = 1 << ch;
    DMA->CLEAR_DST_TRAN.value[0] = 1 << ch;
    DMA->CLEAR_SRC_TRAN.value[0] = 1 << ch;
    DMA->MASK_TFR.value[0]       = (1 << (ch + 8));
    DMA->MASK_BLOCK.value[0]     = (1 << (ch + 8));
    DMA->MASK_ERR.value[0]       = (1 << (ch + 8));
    DMA->MASK_DST_TRAN.value[0]  = (1 << (ch + 8));
    DMA->MASK_SRC_TRAN.value[0]  = (1 << (ch + 8));
}

void DMA_Init(DMA_Channel_TypeDef *DMA_Channelx, DMA_InitTypeDef *DMA_InitStruct)
{
    /* Check the parameters */
    assert_param(IS_DMA_ALL_PERIPH(DMA_Channelx));
    assert_param(IS_DMA_DIR(DMA_InitStruct->DMA_DIR));
    assert_param(IS_DMA_BUFFER_SIZE(DMA_InitStruct->DMA_BufferSize));
    assert_param(IS_DMA_PERIPHERAL_INC_STATE(DMA_InitStruct->DMA_PeripheralInc));
    assert_param(IS_DMA_MEMORY_INC_STATE(DMA_InitStruct->DMA_MemoryInc));
    assert_param(IS_DMA_PERIPHERAL_DATA_SIZE(DMA_InitStruct->DMA_PeripheralDataSize));
    assert_param(IS_DMA_MEMORY_DATA_SIZE(DMA_InitStruct->DMA_MemoryDataSize));
    assert_param(IS_DMA_MODE(DMA_InitStruct->DMA_Mode));
    assert_param(IS_DMA_PRIORITY(DMA_InitStruct->DMA_Priority));
    assert_param(IS_DMA_M2M_STATE(DMA_InitStruct->DMA_M2M));
    assert_param(IS_DMA_PERIPHERAL_HANDSHAKE(DMA_InitStruct->DMA_PeripheralHandshake));

    DMA->DMA_CFG_REG.DMA_EN = 1;
    if (DMA_InitStruct->DMA_M2M != DMA_M2M_Enable) {
        DMA_Channelx->CTL.TT_FC = DMA_InitStruct->DMA_DIR;
        RCC->DMA_HS_SEL.value &= ~(0x03 << ((DMA_InitStruct->DMA_PeripheralHandshake & 0xF) << 1));
        RCC->DMA_HS_SEL.value |= ((DMA_InitStruct->DMA_PeripheralHandshake >> 0x4) & 0x3) << ((DMA_InitStruct->DMA_PeripheralHandshake & 0xF) << 1);
        if (DMA_InitStruct->DMA_DIR == DMA_DIR_PeripheralSRC) {
            DMA_Channelx->CFG.HS_SEL_SRC = 0;
            DMA_Channelx->CFG.HS_SEL_DST = 1;
            DMA_Channelx->CFG.SRC_PER    = DMA_InitStruct->DMA_PeripheralHandshake & 0xF;
        } else {
            DMA_Channelx->CFG.HS_SEL_SRC = 1;
            DMA_Channelx->CFG.HS_SEL_DST = 0;
            DMA_Channelx->CFG.DST_PER    = DMA_InitStruct->DMA_PeripheralHandshake & 0xF;
        }
    } else {
        DMA_Channelx->CTL.TT_FC      = 0;
        DMA_Channelx->CFG.HS_SEL_SRC = 1;
        DMA_Channelx->CFG.HS_SEL_DST = 1;
    }

    if (DMA_InitStruct->DMA_DIR == DMA_DIR_PeripheralSRC) {
        DMA_Channelx->CTL.SRC_MSIZE    = (DMA_InitStruct->DMA_PeripheralHandshake >> 8) & 0x7;
        DMA_Channelx->SAR.ADDR         = DMA_InitStruct->DMA_PeripheralBaseAddr;
        DMA_Channelx->DAR.ADDR         = DMA_InitStruct->DMA_MemoryBaseAddr;
        DMA_Channelx->CTL.SINC         = DMA_InitStruct->DMA_PeripheralInc;
        DMA_Channelx->CTL.DINC         = DMA_InitStruct->DMA_MemoryInc;
        DMA_Channelx->CTL.SRC_TR_WIDTH = DMA_InitStruct->DMA_PeripheralDataSize;
        DMA_Channelx->CTL.DST_TR_WIDTH = DMA_InitStruct->DMA_MemoryDataSize;
    } else {
        DMA_Channelx->CTL.DST_MSIZE    = (DMA_InitStruct->DMA_PeripheralHandshake >> 8) & 0x7;
        DMA_Channelx->SAR.ADDR         = DMA_InitStruct->DMA_MemoryBaseAddr;
        DMA_Channelx->DAR.ADDR         = DMA_InitStruct->DMA_PeripheralBaseAddr;
        DMA_Channelx->CTL.SINC         = DMA_InitStruct->DMA_MemoryInc;
        DMA_Channelx->CTL.DINC         = DMA_InitStruct->DMA_PeripheralInc;
        DMA_Channelx->CTL.SRC_TR_WIDTH = DMA_InitStruct->DMA_MemoryDataSize;
        DMA_Channelx->CTL.DST_TR_WIDTH = DMA_InitStruct->DMA_PeripheralDataSize;
    }

    DMA_Channelx->CTL.LLP_DST_EN = 0;
    DMA_Channelx->CTL.LLP_SRC_EN = 0;
    DMA_Channelx->CTL.BLOCK_TS   = DMA_InitStruct->DMA_BufferSize;

    DMA_Channelx->CFG.CH_PRIOR   = DMA_InitStruct->DMA_Priority;
    DMA_Channelx->CFG.RELOAD     = DMA_InitStruct->DMA_Mode;
    DMA_Channelx->CFG.FIFO_MODE  = 1;
    DMA_Channelx->CFG.FCMODE     = 1;
    DMA_Channelx->CFG.MAX_ABRST  = 0;
    DMA_Channelx->CFG.SRC_HS_POL = 0;
    DMA_Channelx->CFG.DST_HS_POL = 0;
}

void DMA_StructInit(DMA_InitTypeDef *DMA_InitStruct)
{
    /*-------------- Reset DMA init structure parameters values ------------------*/
    /* Initialize the DMA_PeripheralBaseAddr member */
    DMA_InitStruct->DMA_PeripheralBaseAddr = 0;
    /* Initialize the DMA_MemoryBaseAddr member */
    DMA_InitStruct->DMA_MemoryBaseAddr = 0;
    /* Initialize the DMA_DIR member */
    DMA_InitStruct->DMA_DIR = DMA_DIR_PeripheralSRC;
    /* Initialize the DMA_BufferSize member */
    DMA_InitStruct->DMA_BufferSize = 0;
    /* Initialize the DMA_PeripheralInc member */
    DMA_InitStruct->DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    /* Initialize the DMA_MemoryInc member */
    DMA_InitStruct->DMA_MemoryInc = DMA_MemoryInc_Disable;
    /* Initialize the DMA_PeripheralDataSize member */
    DMA_InitStruct->DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    /* Initialize the DMA_MemoryDataSize member */
    DMA_InitStruct->DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    /* Initialize the DMA_Mode member */
    DMA_InitStruct->DMA_Mode = DMA_Mode_Normal;
    /* Initialize the DMA_Priority member */
    DMA_InitStruct->DMA_Priority = DMA_Priority_Low;
    /* Initialize the DMA_M2M member */
    DMA_InitStruct->DMA_M2M = DMA_M2M_Disable;
    /* Initialize the DMA_PeripheralHandshake member */
    DMA_InitStruct->DMA_PeripheralHandshake = DMA_PeripheralHandshake_ADC1;
}

void DMA_Cmd(DMA_Channel_TypeDef *DMA_Channelx, FunctionalState NewState)
{
    uint32_t ch = 0;

    /* Check the parameters */
    assert_param(IS_DMA_ALL_PERIPH(DMA_Channelx));
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    if (DMA_Channelx == DMA_Channel1) {
        ch = 0;
    } else if (DMA_Channelx == DMA_Channel2) {
        ch = 1;
    } else if (DMA_Channelx == DMA_Channel3) {
        ch = 2;
    } else if (DMA_Channelx == DMA_Channel4) {
        ch = 3;
    } else if (DMA_Channelx == DMA_Channel5) {
        ch = 4;
    } else if (DMA_Channelx == DMA_Channel6) {
        ch = 5;
    } else if (DMA_Channelx == DMA_Channel7) {
        ch = 6;
    } else if (DMA_Channelx == DMA_Channel8) {
        ch = 7;
    }

    if (NewState != DISABLE) {
        DMA->CLEAR_BLOCK.value[0] = 1 << ch;
        DMA->CLEAR_TFR.value[0]   = 1 << ch;
        DMA->CLEAR_ERR.value[0]   = 1 << ch;
        /* Enable the selected DMA Channelx */
        DMA->CH_EN_REG.value[0] = (1 << (ch + 8)) | (1 << ch);
    } else {
        /* Disable the selected DMA Channelx */
        DMA->CH_EN_REG.value[0] = (1 << (ch + 8));
    }
}

void DMA_ITConfig(DMA_Channel_TypeDef *DMA_Channelx, uint32_t DMA_IT, FunctionalState NewState)
{
    uint32_t ch = 0;

    /* Check the parameters */
    assert_param(IS_DMA_ALL_PERIPH(DMA_Channelx));
    assert_param(IS_DMA_CONFIG_IT(DMA_IT));
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    if (DMA_Channelx == DMA_Channel1) {
        ch = 0;
    } else if (DMA_Channelx == DMA_Channel2) {
        ch = 1;
    } else if (DMA_Channelx == DMA_Channel3) {
        ch = 2;
    } else if (DMA_Channelx == DMA_Channel4) {
        ch = 3;
    } else if (DMA_Channelx == DMA_Channel5) {
        ch = 4;
    } else if (DMA_Channelx == DMA_Channel6) {
        ch = 5;
    } else if (DMA_Channelx == DMA_Channel7) {
        ch = 6;
    } else if (DMA_Channelx == DMA_Channel8) {
        ch = 7;
    }

    DMA->CLEAR_BLOCK.value[0]    = 1 << ch;
    DMA->CLEAR_TFR.value[0]      = 1 << ch;
    DMA->CLEAR_ERR.value[0]      = 1 << ch;
    DMA->CLEAR_DST_TRAN.value[0] = 1 << ch;
    DMA->CLEAR_SRC_TRAN.value[0] = 1 << ch;
    DMA->MASK_TFR.value[0]       = (1 << (ch + 8));
    DMA->MASK_DST_TRAN.value[0]  = (1 << (ch + 8));
    DMA->MASK_SRC_TRAN.value[0]  = (1 << (ch + 8));

    if (NewState != DISABLE) {
        if ((DMA_IT & DMA_IT_TC) != 0) {
            DMA->MASK_BLOCK.value[0] = (1 << (ch + 8)) | (1 << ch);
        } else if ((DMA_IT & DMA_IT_TE) != 0) {
            DMA->MASK_ERR.value[0] = (1 << (ch + 8)) | (1 << ch);
        }
        DMA_Channelx->CTL.INT_EN = 1;
    } else {
        DMA_Channelx->CTL.INT_EN = 0;
        if ((DMA_IT & DMA_IT_TC) != 0) {
            DMA->MASK_BLOCK.value[0] = (1 << (ch + 8));
        } else if ((DMA_IT & DMA_IT_TE) != 0) {
            DMA->MASK_ERR.value[0] = (1 << (ch + 8));
        }
    }
}

void DMA_SetCurrDataCounter(DMA_Channel_TypeDef *DMA_Channelx, uint16_t DataNumber)
{
    /* Check the parameters */
    assert_param(IS_DMA_ALL_PERIPH(DMA_Channelx));
    assert_param(IS_DMA_BUFFER_SIZE(DataNumber));

    DMA_Channelx->CTL.BLOCK_TS = (uint32_t)DataNumber;
}

uint16_t DMA_GetCurrDataCounter(DMA_Channel_TypeDef *DMA_Channelx)
{
    /* Check the parameters */
    assert_param(IS_DMA_ALL_PERIPH(DMA_Channelx));

    return ((uint16_t)(DMA_Channelx->CTL.BLOCK_TS));
}

FlagStatus DMA_GetFlagStatus(uint32_t DMA_FLAG)
{
    FlagStatus bitstatus = RESET;
    uint32_t   tmpreg    = 0;

    /* Check the parameters */
    assert_param(IS_DMA_GET_FLAG(DMA_FLAG));

    tmpreg = (uint32_t)(DMA->RAW_BLOCK.value) | ((uint32_t)(DMA->RAW_ERR.value) << 8);

    /* Check the status of the specified DMA flag */
    if ((tmpreg & DMA_FLAG) != (uint32_t)RESET) {
        /* DMA_FLAG is set */
        bitstatus = SET;
    } else {
        /* DMA_FLAG is reset */
        bitstatus = RESET;
    }

    /* Return the DMA_FLAG status */
    return bitstatus;
}

void DMA_ClearFlag(uint32_t DMA_FLAG)
{
    /* Check the parameters */
    assert_param(IS_DMA_CLEAR_FLAG(DMA_FLAG));

    /* Calculate the used DMA */
    DMA->CLEAR_BLOCK.value[0] = DMA_FLAG & 0xFF;
    DMA->CLEAR_ERR.value[0]   = (DMA_FLAG >> 8) & 0xFF;
}

ITStatus DMA_GetITStatus(uint32_t DMA_IT)
{
    ITStatus bitstatus = RESET;
    uint32_t tmpreg    = 0;

    /* Check the parameters */
    assert_param(IS_DMA_GET_IT(DMA_IT));

    tmpreg = (uint32_t)(DMA->STATUSES_BLOCK.value) | ((uint32_t)(DMA->STATUSES_ERR.value) << 8);

    /* Check the status of the specified DMA interrupt */
    if ((tmpreg & DMA_IT) != (uint32_t)RESET) {
        /* DMA_IT is set */
        bitstatus = SET;
    } else {
        /* DMA_IT is reset */
        bitstatus = RESET;
    }
    /* Return the DMA_IT status */
    return bitstatus;
}

void DMA_ClearITPendingBit(uint32_t DMA_IT)
{
    /* Check the parameters */
    assert_param(IS_DMA_CLEAR_IT(DMA_IT));

    /* Calculate the used DMA */
    /* Calculate the used DMA */
    DMA->CLEAR_BLOCK.value[0] = DMA_IT & 0xFF;
    DMA->CLEAR_ERR.value[0]   = (DMA_IT >> 8) & 0xFF;
}

static void DMA_Channelx_IRQHandler(void)
{
    /* wake function prototype */
    while (1) {
    }
}

void DMA_Channel1_IRQHandler(void) __attribute__((weak, alias("DMA_Channelx_IRQHandler")));
void DMA_Channel2_IRQHandler(void) __attribute__((weak, alias("DMA_Channelx_IRQHandler")));
void DMA_Channel3_IRQHandler(void) __attribute__((weak, alias("DMA_Channelx_IRQHandler")));
void DMA_Channel4_IRQHandler(void) __attribute__((weak, alias("DMA_Channelx_IRQHandler")));
void DMA_Channel5_IRQHandler(void) __attribute__((weak, alias("DMA_Channelx_IRQHandler")));
void DMA_Channel6_IRQHandler(void) __attribute__((weak, alias("DMA_Channelx_IRQHandler")));
void DMA_Channel7_IRQHandler(void) __attribute__((weak, alias("DMA_Channelx_IRQHandler")));
void DMA_Channel8_IRQHandler(void) __attribute__((weak, alias("DMA_Channelx_IRQHandler")));

/**
 * @brief DMA interrupt unified handler
 */
void DMA_IRQHandler(void)
{
    uint32_t tmpreg = 0;

    tmpreg = (uint32_t)(DMA->STATUSES_BLOCK.value) | ((uint32_t)(DMA->STATUSES_ERR.value) << 8);

    if ((tmpreg & DMA_IT_GL1) != 0) {
        DMA_Channel1_IRQHandler();
    }
    if ((tmpreg & DMA_IT_GL2) != 0) {
        DMA_Channel2_IRQHandler();
    }
    if ((tmpreg & DMA_IT_GL3) != 0) {
        DMA_Channel3_IRQHandler();
    }
    if ((tmpreg & DMA_IT_GL4) != 0) {
        DMA_Channel4_IRQHandler();
    }
    if ((tmpreg & DMA_IT_GL5) != 0) {
        DMA_Channel5_IRQHandler();
    }
    if ((tmpreg & DMA_IT_GL6) != 0) {
        DMA_Channel6_IRQHandler();
    }
    if ((tmpreg & DMA_IT_GL7) != 0) {
        DMA_Channel7_IRQHandler();
    }
    if ((tmpreg & DMA_IT_GL8) != 0) {
        DMA_Channel8_IRQHandler();
    }
}

#endif
