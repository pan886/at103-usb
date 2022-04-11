/**
 * @file at103_memmap.h
 * @brief AT103 Device Peripheral Memory Mapping Layer Header File.
 * @author Dong Qin (dong.qin@timesintelli.com)
 * @version 1.0
 * @date 2022-01-07
 * @copyright Copyright (c) 2022 Timesintelli, Inc
 */

#ifndef __AT103_MEMMAP_H
#define __AT103_MEMMAP_H

#ifdef __cplusplus
extern "C" {
#endif

#include "core_riscv32.h"

/* AT103 memory map begin */
#define FLASH_BASE  ((uint32_t)0x40050000) /**< FLASH base address in the alias region */
#define SRAM_BASE   ((uint32_t)0x00010000) /**< SRAM base address in the alias region */
#define PERIPH_BASE ((uint32_t)0x40000000) /**< Peripheral base address in the alias region */

/* Peripheral memory map */
#define APB1PERIPH_BASE PERIPH_BASE
#define APB2PERIPH_BASE (PERIPH_BASE + 0x10000)
#define AHBPERIPH_BASE  (PERIPH_BASE + 0x28000)

/* APB1 slaves */
#define TIM2_BASE   (APB1PERIPH_BASE + 0x0000)
#define TIM3_BASE   (APB1PERIPH_BASE + 0x1000)
#define TIM4_BASE   (APB1PERIPH_BASE + 0x2000)
#define RTC_BASE    (APB1PERIPH_BASE + 0x3000)
#define WWDG_BASE   (APB1PERIPH_BASE + 0x4000)
#define SPI2_BASE   (APB1PERIPH_BASE + 0x6000)
#define USART2_BASE (APB1PERIPH_BASE + 0x7000)
#define USART3_BASE (APB1PERIPH_BASE + 0x8000)
#define I2C1_BASE   (APB1PERIPH_BASE + 0x9000)
#define I2C2_BASE   (APB1PERIPH_BASE + 0xA000)
#define CAN_BASE    (APB1PERIPH_BASE + 0xC000)
#define PWR_BASE    (APB1PERIPH_BASE + 0xE000)

/* APB2 slaves */
#define AFIO_BASE    (APB2PERIPH_BASE + 0x0000)
#define EXTI_BASE    (APB2PERIPH_BASE + 0x006C)
#define NVIC_BASE    (APB2PERIPH_BASE + 0x1000)
#define GPIOA_BASE   (APB2PERIPH_BASE + 0x2000)
#define GPIOB_BASE   (APB2PERIPH_BASE + 0x3000)
#define GPIOC_BASE   (APB2PERIPH_BASE + 0x4000)
#define GPIOD_BASE   (APB2PERIPH_BASE + 0x5000)
#define GPIOE_BASE   (APB2PERIPH_BASE + 0x6000)
#define ADC1_BASE    (APB2PERIPH_BASE + 0x7000)
#define ADC2_BASE    (APB2PERIPH_BASE + 0x8000)
#define TIM1_BASE    (APB2PERIPH_BASE + 0x9000)
#define SPI1_BASE    (APB2PERIPH_BASE + 0xA000)
#define USART1_BASE  (APB2PERIPH_BASE + 0xB000)
#define FLASH_R_BASE (APB2PERIPH_BASE + 0xC000)
#define RCC_BASE     (APB2PERIPH_BASE + 0xD000)
#define ICACHE_BASE  (APB2PERIPH_BASE + 0xF000)

/* AHB slaves */
#define USB_BASE          (AHBPERIPH_BASE + 0x00000)
#define DMA_Channel1_BASE (AHBPERIPH_BASE + 0x08000)
#define DMA_Channel2_BASE (AHBPERIPH_BASE + 0x08058)
#define DMA_Channel3_BASE (AHBPERIPH_BASE + 0x080B0)
#define DMA_Channel4_BASE (AHBPERIPH_BASE + 0x08108)
#define DMA_Channel5_BASE (AHBPERIPH_BASE + 0x08160)
#define DMA_Channel6_BASE (AHBPERIPH_BASE + 0x081B8)
#define DMA_Channel7_BASE (AHBPERIPH_BASE + 0x08210)
#define DMA_Channel8_BASE (AHBPERIPH_BASE + 0x08268)
#define DMA_BASE          (AHBPERIPH_BASE + 0x082C0)
#define CRC_BASE          (AHBPERIPH_BASE + 0x18000)

/* DEBUG module */
#define DBGMCU_BASE ((uint32_t)0x40020000)
/* AT103 memory map end */

#ifdef __cplusplus
}
#endif

#endif /* __AT103_MEMMAP_H */