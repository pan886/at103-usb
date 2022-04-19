/**
 * @file at103_it.h
 * @brief This file contains the headers of the interrupt handlers.
 *
 * @author liyouchuang (lycdemeng@163.com)
 * @version 1.0
 * @date 2022-02-13
 * @copyright Copyright (c) 2022 Icore, Inc
 */

#ifndef __AT103_IT_H
#define __AT103_IT_H

#ifdef __cplusplus
extern "C" {
#endif

#include "at103.h"

void WWDG_IRQHandler(void);
void RCC_IRQHandler(void);
void TAMPER_IRQHandler(void);
void RTC_IRQHandler(void);
void EXTI0_IRQHandler(void);
void EXTI1_IRQHandler(void);
void EXTI2_IRQHandler(void);
void EXTI3_IRQHandler(void);
void EXTI4_IRQHandler(void);
void DMA_IRQHandler(void);
void ADC1_2_IRQHandler(void);
void CAN_IRQHandler(void);
void EXTI9_5_IRQHandler(void);
void TIM1_BRK_IRQHandler(void);
void TIM1_UP_IRQHandler(void);
void TIM1_TRG_COM_IRQHandler(void);
void TIM1_CC_IRQHandler(void);
void TIM2_IRQHandler(void);
void TIM3_IRQHandler(void);
void TIM4_IRQHandler(void);
void I2C1_IRQHandler(void);
void I2C2_IRQHandler(void);
void SPI1_IRQHandler(void);
void SPI2_IRQHandler(void);
void USART1_IRQHandler(void);
void USART2_IRQHandler(void);
void USART3_IRQHandler(void);
void EXTI15_10_IRQHandler(void);
void USB_MC_IRQHandler(void);
void USB_DMA_IRQHandler(void);
void IllegalInstruction_Handler(void);
void ECALL_InstructionExecuted_Handler(void);

#ifdef __cplusplus
}
#endif

#endif /* __AT103_IT_H */