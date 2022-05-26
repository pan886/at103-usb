/**
 * @file at103_nvic.h
 * @brief This file contains all the functions prototypes for the NVIC firmware library. \n
 *        How to use this driver? \n
 * (+) Use NVIC_Init(...) with NVIC initialization structure to initialize a interrupt channel.
 *        For details on the NVIC initialization structure, see "NVIC_InitTypeDef". \n
 * (+) Use NVIC_EnableIRQ(...) or NVIC_DisableIRQ(...) to enable or disable a channel interrupt, must use __enable_irq() to turn on global interrupts before doing so.
 * @author Dong Qin (dong.qin@timesintelli.com)
 * @version 1.0
 * @date 2022-01-17
 * @copyright Copyright (c) 2022 Timesintelli, Inc
 */

#ifndef __AT103_NVIC_H
#define __AT103_NVIC_H

#ifdef __cplusplus
extern "C" {
#endif

#include "at103.h"

/**
 * @brief Interrupt Number Definition.
 */
typedef enum IRQn {
    WWDG_IRQn         = 0,  /**< Window WatchDog Interrupt. */
    RCC_IRQn          = 1,  /**< RCC global Interrupt. */
    TAMPER_IRQn       = 2,  /**< Tamper Interrupt. */
    RTC_IRQn          = 3,  /**< RTC global Interrupt. */
    EXTI0_IRQn        = 4,  /**< EXTI Line0 Interrupt. */
    EXTI1_IRQn        = 5,  /**< EXTI Line1 Interrupt. */
    EXTI2_IRQn        = 6,  /**< EXTI Line2 Interrupt. */
    EXTI3_IRQn        = 7,  /**< EXTI Line3 Interrupt. */
    EXTI4_IRQn        = 8,  /**< EXTI Line4 Interrupt. */
    DMA_IRQn          = 9,  /**< DMA global Interrupt. */
    ADC1_2_IRQn       = 10, /**< ADC1 and ADC2 global Interrupt. */
    CAN_IRQn          = 11, /**< CAN Interrupt. */
    EXTI9_5_IRQn      = 12, /**< External Line[9:5] Interrupts. */
    TIM1_BRK_IRQn     = 13, /**< TIM1 Break Interrupts. */
    TIM1_UP_IRQn      = 14, /**< TIM1 Update Interrupts. */
    TIM1_TRG_COM_IRQn = 15, /**< TIM1 Trigger and Commutation Interrupt */
    TIM1_CC_IRQn      = 16, /**< TIM1 Capture Compare Interrupt. */
    TIM2_IRQn         = 17, /**< TIM2 global Interrupt. */
    TIM3_IRQn         = 18, /**< TIM3 global Interrupt. */
    TIM4_IRQn         = 19, /**< TIM4 global Interrupt. */
    I2C1_IRQn         = 20, /**< I2C1 Interrupt. */
    I2C2_IRQn         = 21, /**< I2C2 Interrupt. */
    SPI1_IRQn         = 22, /**< SPI1 global Interrupt. */
    SPI2_IRQn         = 23, /**< SPI2 global Interrupt. */
    USART1_IRQn       = 24, /**< USART1 global Interrupt. */
    USART2_IRQn       = 25, /**< USART2 global Interrupt. */
    USART3_IRQn       = 26, /**< USART3 global Interrupt. */
    EXTI15_10_IRQn    = 27, /**< External Line[15:10] Interrupts. */
    USB_MC_IRQn       = 28, /**< USB Interrupts. */
    USB_DMA_IRQn      = 29, /**< USB DMAInterrupts. */
} IRQn_Type;

/**
 * @brief NVIC Init Structure definition.
 */
typedef struct
{
    uint8_t         NVIC_IRQChannel;                   /**< IRQ channel. This parameter can be a value of IRQn_Type. */
    uint8_t         NVIC_IRQChannelPreemptionPriority; /**< Must be zero, software priority setting is not supported, Reserved. */
    uint8_t         NVIC_IRQChannelSubPriority;        /**< Must be zero, software priority setting is not supported, Reserved. */
    FunctionalState NVIC_IRQChannelCmd;                /**< This parameter can be set either to ENABLE or DISABLE. */
} NVIC_InitTypeDef;

#define NVIC_PriorityGroup_0 0 /**< 0 bits for pre-emption priority 4 bits for subpriority. */

/**
 * @brief Set the Priority Grouping, it won't work on the AT103.
 * @param[in] PriorityGroup is priority grouping field, Only support NVIC_PriorityGroup_0.
 */
void NVIC_SetPriorityGrouping(uint32_t PriorityGroup);

/**
 * @brief Get the Priority Grouping, it won't work on the AT103.
 * @return uint32_t priority grouping field, always return NVIC_PriorityGroup_0.
 */
uint32_t NVIC_GetPriorityGrouping(void);

/**
 * @brief Enable Interrupt in Interrupt Controller.
 * @param[in] IRQn The positive number of the external interrupt to enable.
 */
void NVIC_EnableIRQ(IRQn_Type IRQn);

/**
 * @brief Disable the interrupt line for external interrupt specified.
 * @param[in] IRQn The positive number of the external interrupt to enable.
 */
void NVIC_DisableIRQ(IRQn_Type IRQn);

/**
 * @brief Read the active bit for an external interrupt.
 * @param[in] IRQn The number of the interrupt for read active bit.
 * @return uint32_t return 1 = interrupt active, return 0 = interrupt not active.
 */
uint32_t NVIC_GetActive(IRQn_Type IRQn);

/**
 * @brief Configures the priority grouping: pre-emption priority and subpriority, it won't work on the AT103.
 * @param[in] NVIC_PriorityGroup Only support NVIC_PriorityGroup_0.
 */
void NVIC_PriorityGroupConfig(uint32_t NVIC_PriorityGroup);

/**
 * @brief Initializes the NVIC peripheral according to the specified
 *        parameters in the NVIC_InitStruct.
 * @param[in] NVIC_InitStruct pointer to a NVIC_InitTypeDef structure that contains
 *            the configuration information for the specified NVIC peripheral.
 */
void NVIC_Init(NVIC_InitTypeDef *NVIC_InitStruct);

#ifdef __cplusplus
}
#endif

#endif