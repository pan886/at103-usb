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

    /*reset interrupt controller*/
    NVIC->IRQ_INTEN_L               = 0;
    NVIC->IRQ_INTEN_H               = 0;
    NVIC->IRQ_INTFORCE_L            = 0;
    NVIC->IRQ_INTFORCE_H            = 0;
    NVIC->IRQ_PLEVEL.IRQ_PLEVEL_BIT = 0;
    for (uint32_t i = 0; i < 64; i++) {
        NVIC->IRQ_PR[i].PROIORITY_BIT = 0;
    }
}

#ifdef __cplusplus
}
#endif
