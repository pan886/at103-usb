/**
 * @file at103_conf.h
 * @brief
 *
 * @author zhangsheng (zhangsheng@zhangsheng.ic@gmail.com)
 * @version 1.0
 * @date 2022-01-17
 * @copyright Copyright (c) 2022 Icore, Inc
 */

#ifndef __AT103_CONFIG_H
#define __AT103_CONFIG_H

#ifdef __cplusplus
extern "C" {
#endif

#define USART_MODULE_ENABLED
#define GPIO_MODULE_ENABLED
#define RCC_MODULE_ENABLED
#define NVIC_MODULE_ENABLED
#define SPI_MODULE_ENABLED
#define USE_FULL_ASSERT

/** board specific*/
#define SPIy            SPI1
#define SPIy_CLK        RCC_APB2Periph_SPI1
#define SPIy_GPIO       GPIOA
#define SPIy_GPIO_CLK   RCC_APB2Periph_GPIOA
#define SPIy_PIN_CSN    GPIO_Pin_4
#define SPIy_PIN_SCK    GPIO_Pin_5
#define SPIy_PIN_MISO   GPIO_Pin_6
#define SPIy_PIN_MOSI   GPIO_Pin_7
#define SPIy_IRQn       SPI1_IRQn
#define SPIy_IRQHandler SPI1_IRQHandler

#define SPIz            SPI2
#define SPIz_CLK        RCC_APB1Periph_SPI2
#define SPIz_GPIO       GPIOB
#define SPIz_GPIO_CLK   RCC_APB2Periph_GPIOB
#define SPIz_PIN_CSN    GPIO_Pin_12
#define SPIz_PIN_SCK    GPIO_Pin_13
#define SPIz_PIN_MISO   GPIO_Pin_14
#define SPIz_PIN_MOSI   GPIO_Pin_15
#define SPIz_IRQn       SPI2_IRQn
#define SPIz_IRQHandler SPI2_IRQHandler

#ifdef __cplusplus
}
#endif

#endif /* __AT103_CONFIG_H */