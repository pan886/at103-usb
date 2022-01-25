/**
 * @file at103_nvic.c
 * @brief This file provides all the NVIC firmware functions.
 * @author Dong Qin (dong.qin@timesintelli.com)
 * @version 1.0
 * @date 2022-01-17
 * @copyright Copyright (c) 2022 Timesintelli, Inc
 */

#include "at103_nvic.h"

#ifdef NVIC_MODULE_ENABLED

void NVIC_SetPriorityGrouping(uint32_t PriorityGroup)
{
    /* Check the parameters */
    assert_param(IS_NVIC_PRIORITY_GROUP(PriorityGroup));
}

uint32_t NVIC_GetPriorityGrouping(void)
{
    return NVIC_PriorityGroup_0;
}

void NVIC_EnableIRQ(IRQn_Type IRQn)
{
    /* Check the parameters */
    assert_param(IS_NVIC_SUB_IRQ_TYPE(IRQn));

    __IO uint32_t irq;
    irq = NVIC->IRQ_INTEN_L;
    irq |= 0x1 << IRQn;
    NVIC->IRQ_INTEN_L = irq;
}

void NVIC_DisableIRQ(IRQn_Type IRQn)
{
    /* Check the parameters */
    assert_param(IS_NVIC_SUB_IRQ_TYPE(IRQn));

    __IO uint32_t irq;
    irq = NVIC->IRQ_INTEN_L;
    irq &= ~(0x1 << IRQn);
    NVIC->IRQ_INTEN_L = irq;
}

uint32_t NVIC_GetActive(IRQn_Type IRQn)
{
    /* Check the parameters */
    assert_param(IS_NVIC_SUB_IRQ_TYPE(IRQn));

    __IO uint32_t macuse_value;
    macuse_value = read_csr(mcause);
    if (macuse_value == ((uint32_t)IRQn | 0x80000000))
        return 1;
    else
        return 0;
}

void NVIC_PriorityGroupConfig(uint32_t NVIC_PriorityGroup)
{
    /* Check the parameters */
    assert_param(IS_NVIC_PRIORITY_GROUP(NVIC_PriorityGroup));
}

void NVIC_Init(NVIC_InitTypeDef *NVIC_InitStruct)
{
    /* Check the parameters */
    assert_param(IS_FUNCTIONAL_STATE(NVIC_InitStruct->NVIC_IRQChannelCmd));
    assert_param(IS_NVIC_PREEMPTION_PRIORITY(NVIC_InitStruct->NVIC_IRQChannelPreemptionPriority));
    assert_param(IS_NVIC_SUB_PRIORITY(NVIC_InitStruct->NVIC_IRQChannelSubPriority));

    if (NVIC_InitStruct->NVIC_IRQChannelCmd != DISABLE) {
        /* Enable the Selected IRQ Channels --------------------------------------*/
        NVIC_EnableIRQ(NVIC_InitStruct->NVIC_IRQChannel);
    } else {
        /* Disable the Selected IRQ Channels -------------------------------------*/
        NVIC_DisableIRQ(NVIC_InitStruct->NVIC_IRQChannel);
    }
}

#endif
