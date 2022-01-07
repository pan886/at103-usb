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

/* base address for each module*/
#define ROM_BASE_ADDR  0x00008000
#define SRAM_BASE_ADDR 0x00010000 /* 20K */

/* APB1 slaves */
#define WDT_CTRL_BASE_ADDR  0x40004000 /* WDT  */
#define UART_CTRL_BASE_ADDR 0x40007000 /* UART2 */

/* APB2 slaves */
#define AFIO_BASE_ADDR        0x40010000 /* AFIO */
#define TIMER_CTRL_BASE_ADDR  0x40019000 /* timer 1*/
#define RCC_BASE_ADDR         0x4001D000 /* system clock*/
#define CRC_CTRL_BASE_ADDR    0x40040000 /* CRC */
#define EFLASH_BASE_ADDR      0x4001C000 /* EFlash */
#define DMA_CTRL_BASE_ADDR    0x40030000
#define ICACHE_CTRL_BASE_ADDR 0x4001F000

/**
 * @brief Peripheral memory map
 * 
 */
#define ICACHE_BASE ICACHE_CTRL_BASE_ADDR

#ifdef __cplusplus
}
#endif

#endif