/**
 * @file at103_icache.h
 * @brief This file contains all the functions prototypes for the ICACHE firmware library. 
 * @author Dong Qin (dong.qin@timesintelli.com)
 * @version 1.0
 * @date 2022-01-07
 * @copyright Copyright (c) 2022 Timesintelli, Inc
 */

#ifndef __AT103_ICACHE_H
#define __AT103_ICACHE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "at103.h"

/**
 * @brief Instruction cache enable.
 */
static inline void __icache_enable()
{
    ICACHE->ENABLE.EN_BIT = 1;
}

/**
 * @brief Instruction cache disable.
 */
static inline void __icache_disable()
{
    ICACHE->ENABLE.EN_BIT = 0;
}

/**
 * @brief Instruction cache flush.
 */
static inline void __icache_flush()
{
    ICACHE->FLUSH.FLUSH_BIT = 1;
}

#ifdef __cplusplus
}
#endif

#endif