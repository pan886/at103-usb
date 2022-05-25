/**
 * @file at103_exti.c
 * @brief This file provides all the EXTI firmware functions.
 * @author Dong Qin (dong.qin@timesintelli.com)
 * @version 1.0
 * @date 2022-03-11
 * @copyright Copyright (c) 2022 Timesintelli, Inc
 */
#include "at103_exti.h"

#ifdef EXTI_MODULE_ENABLED

#define EXTI_LINENONE ((uint32_t)0x00000) /* No interrupt selected */

#define IS_EXTI_MODE(MODE) (((MODE) == EXTI_Mode_Interrupt) || ((MODE) == EXTI_Mode_Event))

#define IS_EXTI_TRIGGER(TRIGGER) (((TRIGGER) == EXTI_Trigger_Rising) || \
                                  ((TRIGGER) == EXTI_Trigger_Falling) || \
                                  ((TRIGGER) == EXTI_Trigger_Rising_Falling))

#define IS_EXTI_LINE(LINE)     ((((LINE) & (uint32_t)0xFFFF0000) == 0x0) && (LINE) != (uint16_t)0x0)
#define IS_GET_EXTI_Line(LINE) ((LINE = EXTI_Line0) || (LINE = EXTI_Line1) || \
                                (LINE = EXTI_Line2) || (LINE = EXTI_Line3) || \
                                (LINE = EXTI_Line4) || (LINE = EXTI_Line5) || \
                                (LINE = EXTI_Line6) || (LINE = EXTI_Line7) || \
                                (LINE = EXTI_Line8) || (LINE = EXTI_Line9) || \
                                (LINE = EXTI_Line10) || (LINE = EXTI_Line11) || \
                                (LINE = EXTI_Line12) || (LINE = EXTI_Line13) || \
                                (LINE = EXTI_Line14) || (LINE = EXTI_Line15))

void EXTI_DeInit(void)
{
    EXTI->IMR = 0x0;
    EXTI->EMR = 0x0;
    EXTI->PR  = 0xFFFF;
}

void EXTI_Init(EXTI_InitTypeDef *EXTI_InitStruct)
{
    /* Check the parameters */
    assert_param(IS_EXTI_MODE(EXTI_InitStruct->EXTI_Mode));
    assert_param(IS_EXTI_TRIGGER(EXTI_InitStruct->EXTI_Trigger));
    assert_param(IS_EXTI_LINE(EXTI_InitStruct->EXTI_Line));
    assert_param(IS_FUNCTIONAL_STATE(EXTI_InitStruct->EXTI_LineCmd));

    if (EXTI_InitStruct->EXTI_LineCmd != DISABLE) {
        /* Clear EXTI line configuration */
        EXTI->IMR &= ~EXTI_InitStruct->EXTI_Line;
        EXTI->EMR &= ~EXTI_InitStruct->EXTI_Line;

        GPIOA->IS  = 0x0;
        GPIOB->IS  = 0x0;
        GPIOC->IS  = 0x0;
        GPIOD->IS  = 0x0;
        GPIOE->IS  = 0x0;
        GPIOA->IEN = 0xFFFF;
        GPIOB->IEN = 0xFFFF;
        GPIOC->IEN = 0xFFFF;
        GPIOD->IEN = 0xFFFF;
        GPIOE->IEN = 0xFFFF;

        /* Clear Rising Falling edge configuration */
        GPIOA->IBE &= ~EXTI_InitStruct->EXTI_Line;
        GPIOB->IBE &= ~EXTI_InitStruct->EXTI_Line;
        GPIOC->IBE &= ~EXTI_InitStruct->EXTI_Line;
        GPIOD->IBE &= ~EXTI_InitStruct->EXTI_Line;
        GPIOE->IBE &= ~EXTI_InitStruct->EXTI_Line;
        GPIOA->IEV &= ~EXTI_InitStruct->EXTI_Line;
        GPIOB->IEV &= ~EXTI_InitStruct->EXTI_Line;
        GPIOC->IEV &= ~EXTI_InitStruct->EXTI_Line;
        GPIOD->IEV &= ~EXTI_InitStruct->EXTI_Line;
        GPIOE->IEV &= ~EXTI_InitStruct->EXTI_Line;

        if (EXTI_InitStruct->EXTI_Trigger == EXTI_Trigger_Rising_Falling) {
            /* Rising Falling edge */
            GPIOA->IBE |= EXTI_InitStruct->EXTI_Line;
            GPIOB->IBE |= EXTI_InitStruct->EXTI_Line;
            GPIOC->IBE |= EXTI_InitStruct->EXTI_Line;
            GPIOD->IBE |= EXTI_InitStruct->EXTI_Line;
            GPIOE->IBE |= EXTI_InitStruct->EXTI_Line;
        } else if (EXTI_InitStruct->EXTI_Trigger == EXTI_Trigger_Rising) {
            /* Rising edge */
            GPIOA->IEV |= EXTI_InitStruct->EXTI_Line;
            GPIOB->IEV |= EXTI_InitStruct->EXTI_Line;
            GPIOC->IEV |= EXTI_InitStruct->EXTI_Line;
            GPIOD->IEV |= EXTI_InitStruct->EXTI_Line;
            GPIOE->IEV |= EXTI_InitStruct->EXTI_Line;
        }
        GPIOA->IC = EXTI_InitStruct->EXTI_Line;
        GPIOB->IC = EXTI_InitStruct->EXTI_Line;
        GPIOC->IC = EXTI_InitStruct->EXTI_Line;
        GPIOD->IC = EXTI_InitStruct->EXTI_Line;
        GPIOE->IC = EXTI_InitStruct->EXTI_Line;
        EXTI->PR  = EXTI_InitStruct->EXTI_Line;

        if (EXTI_InitStruct->EXTI_Mode == EXTI_Mode_Event) {
            EXTI->EMR |= EXTI_InitStruct->EXTI_Line;
        } else if (EXTI_InitStruct->EXTI_Mode == EXTI_Mode_Interrupt) {
            EXTI->IMR |= EXTI_InitStruct->EXTI_Line;
        }
    } else {
        EXTI->IMR &= ~EXTI_InitStruct->EXTI_Line;
        EXTI->EMR &= ~EXTI_InitStruct->EXTI_Line;
    }
}

void EXTI_StructInit(EXTI_InitTypeDef *EXTI_InitStruct)
{
    EXTI_InitStruct->EXTI_Line    = EXTI_LINENONE;
    EXTI_InitStruct->EXTI_Mode    = EXTI_Mode_Interrupt;
    EXTI_InitStruct->EXTI_Trigger = EXTI_Trigger_Falling;
    EXTI_InitStruct->EXTI_LineCmd = DISABLE;
}

FlagStatus EXTI_GetFlagStatus(uint32_t EXTI_Line)
{
    /* Check the parameters */
    assert_param(IS_GET_EXTI_Line(EXTI_Line));

    FlagStatus bitstatus = RESET;
    if ((EXTI->PR & EXTI_Line) != RESET) {
        bitstatus = SET;
    } else {
        bitstatus = RESET;
    }
    return bitstatus;
}

void EXTI_ClearFlag(uint32_t EXTI_Line)
{
    /* Check the parameters */
    assert_param(IS_GET_EXTI_Line(EXTI_Line));

    GPIOA->IC = EXTI_Line;
    GPIOB->IC = EXTI_Line;
    GPIOC->IC = EXTI_Line;
    GPIOD->IC = EXTI_Line;
    GPIOE->IC = EXTI_Line;
    EXTI->PR  = EXTI_Line;
}

ITStatus EXTI_GetITStatus(uint32_t EXTI_Line)
{
    ITStatus bitstatus    = RESET;
    uint32_t enablestatus = 0;

    /* Check the parameters */
    assert_param(IS_GET_EXTI_Line(EXTI_Line));

    enablestatus = EXTI->IMR & EXTI_Line;
    if (((EXTI->PR & EXTI_Line) != (uint32_t)RESET) && (enablestatus != (uint32_t)RESET)) {
        bitstatus = SET;
    } else {
        bitstatus = RESET;
    }
    return bitstatus;
}

void EXTI_ClearITPendingBit(uint32_t EXTI_Line)
{
    /* Check the parameters */
    assert_param(IS_GET_EXTI_Line(EXTI_Line));

    GPIOA->IC = EXTI_Line;
    GPIOB->IC = EXTI_Line;
    GPIOC->IC = EXTI_Line;
    GPIOD->IC = EXTI_Line;
    GPIOE->IC = EXTI_Line;
    EXTI->PR  = EXTI_Line;
}

#endif