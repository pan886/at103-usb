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
 * @brief ICACHE Peripheral Interface
 * 
 */
typedef struct
{
    union {
        struct {
            __OM uint32_t ENABLE;
            __OM          uint32_t : 32; /**< Reserved */
        };
    };
    union {
        struct {
            __OM uint32_t FLUSH;
            __OM          uint32_t : 32; /**< Reserved */
        };
    };
    union {
        struct {
            __OM uint32_t PREF_CNT_EN;
            __OM          uint32_t : 32; /**< Reserved */
        };
    };
    union {
        struct {
            __OM uint32_t PERF_CNT_CLR;
            __OM          uint32_t : 32; /**< Reserved */
        };
    };
    union {
        struct {
            __OM uint32_t SEL_FLUSH_REQ;
            __OM          uint32_t : 32; /**< Reserved */
        };
    };
    union {
        struct {
            __OM uint32_t SEL_FLUSH_ID;
            __OM          uint32_t : 32; /**< Reserved */
        };
    };
    union {
        struct {
            __IM uint32_t STATUS;
            __IM          uint32_t : 32; /**< Reserved */
        };
    };
    union {
        struct {
            __IM uint32_t HIT_CNT;
            __IM          uint32_t : 32; /**< Reserved */
        };
    };
    union {
        struct {
            __IM uint32_t TRANS_CNT;
            __IM          uint32_t : 32; /**< Reserved */
        };
    };
} ICACHE_TypeDef;

/** @addtogroup Peripheral declaration
  * @{
  */
#define ICACHE ((ICACHE_TypeDef *)ICACHE_BASE)

#include "at103_icache.h"
#include "uart.h"

#ifdef __cplusplus
}
#endif

#endif