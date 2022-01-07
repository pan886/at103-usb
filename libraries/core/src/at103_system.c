/**
 * @file at103_system.c
 * @brief AT103 Device Peripheral Access Layer System Source File.
 * @author Dong Qin (dong.qin@timesintelli.com)
 * @version 1.0
 * @date 2022-01-07
 * @copyright Copyright (c) 2022 Timesintelli, Inc
 */

#ifdef __cplusplus
extern "C" {
#endif

#include "at103_system.h"

void _SysInit(void)
{
    __icache_enable();
}

#ifdef __cplusplus
}
#endif
