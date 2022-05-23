/**
 * @file at103_config.h
 * @brief Library configuration file.
 * @author liyouchuang (lycdemeng@163.com)
 * @version 1.0
 * @date 2022-05-23
 * @copyright Copyright (c) 2022 Icore, Inc
 */

#ifndef __AT103_CONFIG_H
#define __AT103_CONFIG_H

#ifdef __cplusplus
extern "C" {
#endif

#define USART_MODULE_ENABLED
#define NVIC_MODULE_ENABLED
#define RCC_MODULE_ENABLED
#define GPIO_MODULE_ENABLED
#define EXTI_MODULE_ENABLED
#define DMA_MODULE_ENABLED
#define TIM_MODULE_ENABLED

#define USE_FULL_ASSERT

/* board specific*/
#define USARTy            USART1
#define USARTy_GPIO       GPIOA
#define USARTy_RxPin      GPIO_Pin_10
#define USARTy_TxPin      GPIO_Pin_9
#define USARTy_IRQn       USART1_IRQn
#define USARTy_IRQHandler USART1_IRQHandler

#define USARTz            USART3
#define USARTz_GPIO       GPIOD
#define USARTz_RxPin      GPIO_Pin_9
#define USARTz_TxPin      GPIO_Pin_8
#define USARTz_IRQn       USART3_IRQn
#define USARTz_IRQHandler USART3_IRQHandler

#ifdef __cplusplus
}
#endif

#endif /* __AT103_CONFIG_H */