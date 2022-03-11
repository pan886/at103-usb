/**
 * @file at103_it.c
 * @brief This file provides template for all exceptions handler
 *        and peripherals interrupt service routine.
 * @author Dong Qin (dong.qin@timesintelli.com)
 * @version 1.0
 * @date 2022-01-17
 * @copyright Copyright (c) 2022 Timesintelli, Inc
 */

#include "at103_it.h"

extern __IO uint32_t interrupt_flag;

void WWDG_IRQHandler(void)
{
    /* clear interrupt force bit */
    __IO uint32_t arg;
    arg = NVIC->IRQ_INTFORCE_L;
    arg &= ~(0x01 << WWDG_IRQn);
    NVIC->IRQ_INTFORCE_L = arg;

    interrupt_flag++;
}

void RCC_IRQHandler(void)
{
    /* clear interrupt force bit */
    __IO uint32_t arg;
    arg = NVIC->IRQ_INTFORCE_L;
    arg &= ~(0x01 << RCC_IRQn);
    NVIC->IRQ_INTFORCE_L = arg;

    interrupt_flag++;
}

void TAMPER_IRQHandler(void)
{
    /* clear interrupt force bit */
    __IO uint32_t arg;
    arg = NVIC->IRQ_INTFORCE_L;
    arg &= ~(0x01 << TAMPER_IRQn);
    NVIC->IRQ_INTFORCE_L = arg;

    interrupt_flag++;
}

void RTC_IRQHandler(void)
{
    /* clear interrupt force bit */
    __IO uint32_t arg;
    arg = NVIC->IRQ_INTFORCE_L;
    arg &= ~(0x01 << RTC_IRQn);
    NVIC->IRQ_INTFORCE_L = arg;

    interrupt_flag++;
}

void EXTI0_IRQHandler(void)
{
    /* clear interrupt force bit */
    __IO uint32_t arg;
    arg = NVIC->IRQ_INTFORCE_L;
    arg &= ~(0x01 << EXTI0_IRQn);
    NVIC->IRQ_INTFORCE_L = arg;

    interrupt_flag++;
}

void EXTI1_IRQHandler(void)
{
    /* clear interrupt force bit */
    __IO uint32_t arg;
    arg = NVIC->IRQ_INTFORCE_L;
    arg &= ~(0x01 << EXTI1_IRQn);
    NVIC->IRQ_INTFORCE_L = arg;

    interrupt_flag++;
}

void EXTI2_IRQHandler(void)
{
    /* clear interrupt force bit */
    __IO uint32_t arg;
    arg = NVIC->IRQ_INTFORCE_L;
    arg &= ~(0x01 << EXTI2_IRQn);
    NVIC->IRQ_INTFORCE_L = arg;

    interrupt_flag++;
}

void EXTI3_IRQHandler(void)
{
    /* clear interrupt force bit */
    __IO uint32_t arg;
    arg = NVIC->IRQ_INTFORCE_L;
    arg &= ~(0x01 << EXTI3_IRQn);
    NVIC->IRQ_INTFORCE_L = arg;

    interrupt_flag++;
}

void EXTI4_IRQHandler(void)
{
    /* clear interrupt force bit */
    __IO uint32_t arg;
    arg = NVIC->IRQ_INTFORCE_L;
    arg &= ~(0x01 << EXTI4_IRQn);
    NVIC->IRQ_INTFORCE_L = arg;

    interrupt_flag++;
}

void DMA_IRQHandler(void)
{
    /* clear interrupt force bit */
    __IO uint32_t arg;
    arg = NVIC->IRQ_INTFORCE_L;
    arg &= ~(0x01 << DMA_IRQn);
    NVIC->IRQ_INTFORCE_L = arg;

    interrupt_flag++;
}

void ADC1_2_IRQHandler(void)
{
    /* clear interrupt force bit */
    __IO uint32_t arg;
    arg = NVIC->IRQ_INTFORCE_L;
    arg &= ~(0x01 << ADC1_2_IRQn);
    NVIC->IRQ_INTFORCE_L = arg;

    interrupt_flag++;
}

void CAN_IRQHandler(void)
{
    /* clear interrupt force bit */
    __IO uint32_t arg;
    arg = NVIC->IRQ_INTFORCE_L;
    arg &= ~(0x01 << CAN_IRQn);
    NVIC->IRQ_INTFORCE_L = arg;

    interrupt_flag++;
}

void EXTI9_5_IRQHandler(void)
{
    /* clear interrupt force bit */
    __IO uint32_t arg;
    arg = NVIC->IRQ_INTFORCE_L;
    arg &= ~(0x01 << EXTI9_5_IRQn);
    NVIC->IRQ_INTFORCE_L = arg;

    interrupt_flag++;
}

void TIM1_BRK_IRQHandler(void)
{
    /* clear interrupt force bit */
    __IO uint32_t arg;
    arg = NVIC->IRQ_INTFORCE_L;
    arg &= ~(0x01 << TIM1_BRK_IRQn);
    NVIC->IRQ_INTFORCE_L = arg;

    interrupt_flag++;
}

void TIM1_UP_IRQHandler(void)
{
    /* clear interrupt force bit */
    __IO uint32_t arg;
    arg = NVIC->IRQ_INTFORCE_L;
    arg &= ~(0x01 << TIM1_UP_IRQn);
    NVIC->IRQ_INTFORCE_L = arg;

    interrupt_flag++;
}

void TIM1_TRG_COM_IRQHandler(void)
{
    /* clear interrupt force bit */
    __IO uint32_t arg;
    arg = NVIC->IRQ_INTFORCE_L;
    arg &= ~(0x01 << TIM1_TRG_COM_IRQn);
    NVIC->IRQ_INTFORCE_L = arg;

    interrupt_flag++;
}

void TIM1_CC_IRQHandler(void)
{
    /* clear interrupt force bit */
    __IO uint32_t arg;
    arg = NVIC->IRQ_INTFORCE_L;
    arg &= ~(0x01 << TIM1_CC_IRQn);
    NVIC->IRQ_INTFORCE_L = arg;

    interrupt_flag++;
}

void TIM2_IRQHandler(void)
{
    /* clear interrupt force bit */
    __IO uint32_t arg;
    arg = NVIC->IRQ_INTFORCE_L;
    arg &= ~(0x01 << TIM2_IRQn);
    NVIC->IRQ_INTFORCE_L = arg;

    interrupt_flag++;
}

void TIM3_IRQHandler(void)
{
    /* clear interrupt force bit */
    __IO uint32_t arg;
    arg = NVIC->IRQ_INTFORCE_L;
    arg &= ~(0x01 << TIM3_IRQn);
    NVIC->IRQ_INTFORCE_L = arg;

    interrupt_flag++;
}

void TIM4_IRQHandler(void)
{
    /* clear interrupt force bit */
    __IO uint32_t arg;
    arg = NVIC->IRQ_INTFORCE_L;
    arg &= ~(0x01 << TIM4_IRQn);
    NVIC->IRQ_INTFORCE_L = arg;

    interrupt_flag++;
}

void I2C1_IRQHandler(void)
{
    /* clear interrupt force bit */
    __IO uint32_t arg;
    arg = NVIC->IRQ_INTFORCE_L;
    arg &= ~(0x01 << I2C1_IRQn);
    NVIC->IRQ_INTFORCE_L = arg;

    interrupt_flag++;
}

void I2C2_IRQHandler(void)
{
    /* clear interrupt force bit */
    __IO uint32_t arg;
    arg = NVIC->IRQ_INTFORCE_L;
    arg &= ~(0x01 << I2C2_IRQn);
    NVIC->IRQ_INTFORCE_L = arg;

    interrupt_flag++;
}

void SPI1_IRQHandler(void)
{
    /* clear interrupt force bit */
    __IO uint32_t arg;
    arg = NVIC->IRQ_INTFORCE_L;
    arg &= ~(0x01 << SPI1_IRQn);
    NVIC->IRQ_INTFORCE_L = arg;

    interrupt_flag++;
}

void SPI2_IRQHandler(void)
{
    /* clear interrupt force bit */
    __IO uint32_t arg;
    arg = NVIC->IRQ_INTFORCE_L;
    arg &= ~(0x01 << SPI2_IRQn);
    NVIC->IRQ_INTFORCE_L = arg;

    interrupt_flag++;
}

void USART1_IRQHandler(void)
{
    /* clear interrupt force bit */
    __IO uint32_t arg;
    arg = NVIC->IRQ_INTFORCE_L;
    arg &= ~(0x01 << USART1_IRQn);
    NVIC->IRQ_INTFORCE_L = arg;

    interrupt_flag++;
}

void USART2_IRQHandler(void)
{
    /* clear interrupt force bit */
    __IO uint32_t arg;
    arg = NVIC->IRQ_INTFORCE_L;
    arg &= ~(0x01 << USART2_IRQn);
    NVIC->IRQ_INTFORCE_L = arg;

    interrupt_flag++;
}

void USART3_IRQHandler(void)
{
    /* clear interrupt force bit */
    __IO uint32_t arg;
    arg = NVIC->IRQ_INTFORCE_L;
    arg &= ~(0x01 << USART3_IRQn);
    NVIC->IRQ_INTFORCE_L = arg;

    interrupt_flag++;
}

void EXTI15_10_IRQHandler(void)
{
    /* clear interrupt force bit */
    __IO uint32_t arg;
    arg = NVIC->IRQ_INTFORCE_L;
    arg &= ~(0x01 << EXTI15_10_IRQn);
    NVIC->IRQ_INTFORCE_L = arg;

    interrupt_flag++;
}

void USB_MC_IRQHandler(void)
{
    /* clear interrupt force bit */
    __IO uint32_t arg;
    arg = NVIC->IRQ_INTFORCE_L;
    arg &= ~(0x01 << USB_MC_IRQn);
    NVIC->IRQ_INTFORCE_L = arg;

    interrupt_flag++;
}

void USB_DMA_IRQHandler(void)
{
    /* clear interrupt force bit */
    __IO uint32_t arg;
    arg = NVIC->IRQ_INTFORCE_L;
    arg &= ~(0x01 << USB_DMA_IRQn);
    NVIC->IRQ_INTFORCE_L = arg;

    interrupt_flag++;
}

void IllegalInstruction_Handler(void)
{
    while (1) {
        /* error debug */
    }
}

void ECALL_InstructionExecuted_Handler(void)
{
    debug("Software Interrupt !!!\r\n");
    while (1) {
        /* swi */
    }
}
