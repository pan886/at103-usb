/**
 * @file at103_system.h
 * @brief AT103 Device Peripheral Access Layer System Header File.
 * @author Dong Qin (dong.qin@timesintelli.com)
 * @version 1.0
 * @date 2022-01-07
 * @copyright Copyright (c) 2022 Timesintelli, Inc
 */

#ifndef __AT103_SYSTEM_H
#define __AT103_SYSTEM_H

#ifdef __cplusplus
extern "C" {
#endif

#include "at103.h"

/**
 * @brief early system initialization.
 */
extern void _SysInit(void);

/**
 * @brief nop delay.
 * @param[in] time delay time.
 */
extern void NopDelay(uint32_t time);

#ifdef __cplusplus
}
#endif

#endif