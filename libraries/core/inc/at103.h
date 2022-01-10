/**
 * @file at103.h
 * @brief Device Peripheral Access Layer Header File.
 *        This file contains all the peripheral register's definitions, bits definitions for AT103 
 *        The file is the unique include file that the application programmer is using in the C source code, usually in main.c. 
 * @author Dong Qin (dong.qin@timesintelli.com)
 * @version 1.0
 * @date 2022-01-07
 * @copyright Copyright (c) 2022 Timesintelli, Inc
 */

#ifndef __AT103_H
#define __AT103_H

#ifdef __cplusplus
extern "C" {
#endif

#include "at103_memmap.h"

/**
 * @brief Instruction Cache Peripheral Interface
 */
typedef struct
{
    union {
        struct {
            __OM uint32_t EN_BIT : 1;    /**< Instruction cache enable/disable bit */
            __OM          uint32_t : 31; /**< Reserved */
        };
    } ENABLE;
    union {
        struct {
            __OM uint32_t FLUSH_BIT : 1; /**< Instruction cache flush bit */
            __OM          uint32_t : 31; /**< Reserved */
        };
    } FLUSH;
    union {
        struct {
            __OM uint32_t EN_BIT : 1;    /**< Instruction cache performance counter enable bit */
            __OM          uint32_t : 31; /**< Reserved */
        };
    } PREF_CNT_EN;
    union {
        struct {
            __OM uint32_t CLR_BIT : 1;   /**< Instruction cache performance counter clear bit */
            __OM          uint32_t : 31; /**< Reserved */
        };
    } PERF_CNT_CLR;
    union {
        struct {
            __OM uint32_t REQ_EN_BIT : 1; /**< Instruction cache select flush request bit */
            __OM          uint32_t : 31;  /**< Reserved */
        };
    } SEL_FLUSH_REQ;
    __OM uint32_t SEL_FLUSH_ID; /**< ID of cachline to be flushed. */
    union {
        struct {
            __IM uint32_t IS_ENABLE_BIT : 1;     /**< Instruction cache enable/bypass status. */
            __IM uint32_t IS_FLUSH_BIT : 1;      /**< Instruction cache flush status. */
            __IM uint32_t IS_LINE_FLUSH_BIT : 1; /**< Selected cachline status. */
            __IM          uint32_t : 29;         /**< Reserved */
        };
        __IM uint32_t value;
    } STATUS;
    __IM uint32_t HIT_CNT;   /**< Number of instruction cache hit count. */
    __IM uint32_t TRANS_CNT; /**< Number of instruction request count. */
} ICACHE_TypeDef;

/** @addtogroup Peripheral declaration
  * @{
  */
#define ICACHE ((ICACHE_TypeDef *)ICACHE_BASE)

/** system level driver */
#include "at103_icache.h"

/** peripherals driver */
#include "at103_conf.h"

#ifdef USART_MODULE_ENABLED
#include "uart.h"
#endif /* USART_MODULE_ENABLED */

#ifdef __cplusplus
}
#endif

#endif