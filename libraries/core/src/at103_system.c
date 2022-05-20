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

    /*init flash controller*/
    while (FLASH->STS0.INIT_STATUS == 0) {
    }
    FLASH->LOCK      = 0;
    FLASH->CTL.LVCTL = 0;
    FLASH->SEC_P0    = 0;
    FLASH->SEC_P1    = 0;
    FLASH->SEC_P2    = 0;
    FLASH->SEC_P3    = 0;
    FLASH->SEC_P4    = 0;
    FLASH->SEC_P5    = 0;
    FLASH->SEC_P6    = 0;
    FLASH->SEC_P7    = 0;
    FLASH->LOCK      = 0xF;

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
