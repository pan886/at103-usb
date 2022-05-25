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
#define DMA_MODULE_ENABLED
#define USE_FULL_ASSERT

/** board specific*/
#define SPI_MASTER          SPI1
#define SPI_MASTER_CLK      RCC_APB2Periph_SPI1
#define SPI_MASTER_GPIO     GPIOA
#define SPI_MASTER_GPIO_CLK RCC_APB2Periph_GPIOA
#define SPI_MASTER_PIN_CSN  GPIO_Pin_4
#define SPI_MASTER_PIN_SCK  GPIO_Pin_5
#define SPI_MASTER_PIN_MISO GPIO_Pin_6
#define SPI_MASTER_PIN_MOSI GPIO_Pin_7
#define SPI_MASTER_DR_Base  (SPI1_BASE + 0x0004)
#define SPI_MASTER_DT_Base  SPI1_BASE

#define SPI_SLAVE          SPI2
#define SPI_SLAVE_CLK      RCC_APB1Periph_SPI2
#define SPI_SLAVE_GPIO     GPIOB
#define SPI_SLAVE_GPIO_CLK RCC_APB2Periph_GPIOB
#define SPI_SLAVE_PIN_CSN  GPIO_Pin_12
#define SPI_SLAVE_PIN_SCK  GPIO_Pin_13
#define SPI_SLAVE_PIN_MISO GPIO_Pin_14
#define SPI_SLAVE_PIN_MOSI GPIO_Pin_15
#define SPI_SLAVE_DR_Base  0x40006004 //(SPI2_BASE+0x0004)
#define SPI_SLAVE_DT_Base  SPI2_BASE

#ifdef __cplusplus
}
#endif

#endif /* __AT103_CONFIG_H */