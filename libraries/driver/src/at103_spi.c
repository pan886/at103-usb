/**
 * @file      at103_spi.c
 * @brief     This file provides all the SPI firmware functions.
 * @author    luoming (luoming@i-core.cn)
 * @version   1.0
 * @date      2022-03-29
 * @copyright Copyright (c) 2022 i-core, Inc
 */

/** Includes ------------------------------------------------------------------*/
#include "at103_spi.h"

#ifdef SPI_MODULE_ENABLED

/** SPI SPE mask */
#define GCTL_SPE_Set   ((uint32_t)0x00000001)
#define GCTL_SPE_Reset ((uint32_t)0xFFFFFFFE)

/** SPI INT mask */
#define GCTL_INT_Set   ((uint32_t)0x00000002)
#define GCTL_INT_Reset ((uint32_t)0xFFFFFFFD)

/** SPI DMA mask */
#define GCTL_DMA_txReset ((uint32_t)0xFFFFFC7F)
#define GCTL_DMA_rxReset ((uint32_t)0xFFFFFD9F)

/* SPI SSOE mask */
#define CSN_SSOE_Set   ((uint32_t)0xFFFFFFFE)
#define CSN_SSOE_Reset ((uint32_t)0x00000001)

void SPI_DeInit(SPI_TypeDef *SPIx)
{
    /** Check the parameters */
    assert_param(IS_SPI_ALL_PERIPH(SPIx));

    if (SPIx == SPI1) {
        /** Enable SPI1 reset state */
        RCC_APB2PeriphResetCmd(RCC_APB2Periph_SPI1, ENABLE);
        /** Release SPI1 from reset state */
        RCC_APB2PeriphResetCmd(RCC_APB2Periph_SPI1, DISABLE);
    } else if (SPIx == SPI2) {
        /** Enable SPI2 reset state */
        RCC_APB1PeriphResetCmd(RCC_APB1Periph_SPI2, ENABLE);
        /** Release SPI2 from reset state */
        RCC_APB1PeriphResetCmd(RCC_APB1Periph_SPI2, DISABLE);
    }
}

void SPI_Init(SPI_TypeDef *SPIx, SPI_InitTypeDef *SPI_InitStruct)
{
    uint32_t tmpreg = 0;

    /** check the parameters */
    assert_param(IS_SPI_ALL_PERIPH(SPIx));

    /** Check the SPI parameters */
    assert_param(IS_SPI_DIRECTION_MODE(SPI_InitStruct->SPI_Direction));
    assert_param(IS_SPI_MODE(SPI_InitStruct->SPI_Mode));
    assert_param(IS_SPI_DATASIZE(SPI_InitStruct->SPI_DataSize));
    assert_param(IS_SPI_CPOL(SPI_InitStruct->SPI_CPOL));
    assert_param(IS_SPI_CPHA(SPI_InitStruct->SPI_CPHA));
    assert_param(IS_SPI_NSS(SPI_InitStruct->SPI_NSS));
    assert_param(IS_SPI_BAUDRATE_PRESCALER(SPI_InitStruct->SPI_BaudRatePrescaler));
    assert_param(IS_SPI_FIRST_BIT(SPI_InitStruct->SPI_FirstBit));

    /**---------------------------- SPIx GCTL Configuration ------------------------*/
    tmpreg = 0x0;

    tmpreg |= SPI_InitStruct->SPI_Direction |
              SPI_InitStruct->SPI_Mode |
              SPI_InitStruct->SPI_NSS;

    SPIx->GCTL.value = tmpreg;
    /**---------------------------- SPIx CCTL Configuration ------------------------*/
    tmpreg = 0x0;

    tmpreg |= SPI_InitStruct->SPI_CPHA |
              SPI_InitStruct->SPI_CPOL |
              SPI_InitStruct->SPI_FirstBit |
              SPI_InitStruct->SPI_DataSize;

    SPIx->CCTL.value = tmpreg;
    /**---------------------------- SPIx SPBRG Configuration ------------------------*/
    SPIx->SPBRG.BAUDRATE = SPI_InitStruct->SPI_BaudRatePrescaler;
}

void SPI_StructInit(SPI_InitTypeDef *SPI_InitStruct)
{
    /** Initialize the SPI_Direction member */
    SPI_InitStruct->SPI_Direction = SPI_Direction_2Lines_FullDuplex;
    /** initialize the SPI_Mode member */
    SPI_InitStruct->SPI_Mode = SPI_Mode_Slave;
    /** initialize the SPI_DataSize member */
    SPI_InitStruct->SPI_DataSize = SPI_DataSize_8b;
    /** Initialize the SPI_CPOL member */
    SPI_InitStruct->SPI_CPOL = SPI_CPOL_Low;
    /** Initialize the SPI_CPHA member */
    SPI_InitStruct->SPI_CPHA = SPI_CPHA_1Edge;
    /** Initialize the SPI_NSS member */
    SPI_InitStruct->SPI_NSS = SPI_NSS_Hard;
    /** Initialize the SPI_BaudRatePrescaler member */
    SPI_InitStruct->SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;
    /** Initialize the SPI_FirstBit member */
    SPI_InitStruct->SPI_FirstBit = SPI_FirstBit_MSB;
    /** Initialize the SPI_CRCPolynomial member
        Don't support.                     */
    SPI_InitStruct->SPI_CRCPolynomial = 0;
}

void SPI_Cmd(SPI_TypeDef *SPIx, FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_SPI_ALL_PERIPH(SPIx));
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    if (NewState != DISABLE) {

        SPIx->GCTL.value |= GCTL_SPE_Set;

    } else {

        SPIx->GCTL.value &= GCTL_SPE_Reset;
    }
}

void SPI_ITConfig(SPI_TypeDef *SPIx, uint8_t SPI_IT, FunctionalState NewState)
{
    uint32_t itmask = 0;
    /** Check the parameters */
    assert_param(IS_SPI_ALL_PERIPH(SPIx));
    assert_param(IS_FUNCTIONAL_STATE(NewState));
    assert_param(IS_SPI_CONFIG_IT(SPI_IT));

    /** Set the IT mask */
    itmask = 0;
    if (SPI_IT == SPI_IT_TXE) {
        itmask |= (uint32_t)SPI_IT_TXE;
    } else if (SPI_IT == SPI_IT_RXNE) {
        itmask |= (uint32_t)SPI_IT_RXNE;
    } else if (SPI_IT == SPI_IT_ERR) {
        itmask |= (uint32_t)SPI_IT_ERR;
    } else {
        itmask |= (uint32_t)SPI_IT_ALL;
    }

    if (NewState != DISABLE) {
        SPIx->INTENA.value |= itmask;
        SPIx->GCTL.value |= GCTL_INT_Set;
    } else {
        SPIx->INTENA.value &= (uint32_t)~itmask;
        SPIx->GCTL.value &= GCTL_INT_Reset;
    }
}

void SPI_DMACmd(SPI_TypeDef *SPIx, uint16_t SPI_DMAReq, FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_SPI_ALL_PERIPH(SPIx));
    assert_param(IS_FUNCTIONAL_STATE(NewState));
    assert_param(IS_SPI_DMAREQ(SPI_DMAReq));
    if (NewState != DISABLE) {
        SPIx->GCTL.value |= SPI_DMAReq;
    } else {
        if (SPI_DMAReq == SPI_DMAReq_Tx) {
            SPIx->GCTL.value &= GCTL_DMA_txReset;
        } else {
            SPIx->GCTL.value &= GCTL_DMA_rxReset;
        }
    }
}

void SPI_SendData(SPI_TypeDef *SPIx, uint16_t Data)
{
    assert_param(IS_SPI_ALL_PERIPH(SPIx));

    /** Write in the DR register the data to be sent */
    SPIx->TXREG = (uint32_t)Data;
}

uint16_t SPI_ReceiveData(SPI_TypeDef *SPIx)
{
    assert_param(IS_SPI_ALL_PERIPH(SPIx));
    return (uint16_t)SPIx->RXREG;
}

void SPI_SSOutputCmd(SPI_TypeDef *SPIx, FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_SPI_ALL_PERIPH(SPIx));
    assert_param(IS_FUNCTIONAL_STATE(NewState));
    if (NewState != DISABLE) {
        SPIx->SCSR.value &= CSN_SSOE_Set;
    } else {
        SPIx->SCSR.value |= CSN_SSOE_Reset;
    }
}

void SPI_DataSizeConfig(SPI_TypeDef *SPIx, uint16_t SPI_DataSize)
{
    assert_param(IS_SPI_ALL_PERIPH(SPIx));
    assert_param(IS_SPI_DATASIZE(SPI_DataSize));
    SPIx->CCTL.value &= SPI_DataSize_MASK;
    SPIx->CCTL.value |= SPI_DataSize;
}

void SPI_BiDirectionalLineConfig(SPI_TypeDef *SPIx, uint16_t SPI_Direction)
{
    /* Check the parameters */
    assert_param(IS_SPI_ALL_PERIPH(SPIx));
    assert_param(IS_SPI_DIRECTION(SPI_Direction));
    SPIx->GCTL.value &= SPI_Direction_MASK;
    if (SPI_Direction == SPI_Direction_Tx) {
        SPIx->GCTL.value |= SPI_Direction_Tx;
    } else {
        SPIx->GCTL.value |= SPI_Direction_Rx;
    }
}

FlagStatus SPI_GetFlagStatus(SPI_TypeDef *SPIx, uint16_t SPI_FLAG)
{
    FlagStatus bitstatus = RESET;
    assert_param(IS_SPI_ALL_PERIPH(SPIx));
    assert_param(IS_SPI_GET_FLAG(SPI_FLAG));
    if ((SPIx->CSTAT.value & (uint32_t)SPI_FLAG) != (uint32_t)RESET) {
        bitstatus = SET;
    } else {
        bitstatus = RESET;
    }
    return bitstatus;
}

ITStatus SPI_GetITStatus(SPI_TypeDef *SPIx, uint8_t SPI_IT)
{
    ITStatus bitstatus = RESET;
    uint32_t itmask = 0, itpos = 0;
    bitstatus = 0;

    /* Check the parameters */
    assert_param(IS_SPI_ALL_PERIPH(SPIx));
    assert_param(IS_SPI_GET_IT(SPI_IT));

    itpos  = (uint32_t)SPI_IT;
    itmask = (uint32_t)SPI_IT;

    if ((SPIx->INTSTAT.value & itpos) == itmask) {
        bitstatus = SET;
    } else {
        bitstatus = RESET;
    }

    return bitstatus;
}

void SPI_ClearITPendingBit(SPI_TypeDef *SPIx, uint8_t SPI_IT)
{
    uint32_t itpos = 0;
    /* Check the parameters */
    assert_param(IS_SPI_ALL_PERIPH(SPIx));
    assert_param(IS_SPI_CLEAR_IT(SPI_IT));

    itpos = (uint32_t)SPI_IT;

    SPIx->INTCLR.value |= itpos;
}

#endif