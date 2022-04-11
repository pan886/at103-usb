/**
 * @file at103_it.c
 * @brief This file provides template for all exceptions handler
 *        and peripherals interrupt service routine.
 * @author Dong Qin (dong.qin@timesintelli.com)
 * @version 1.0
 * @date 2022-01-17
 * @copyright Copyright (c) 2022 Timesintelli, Inc
 */

#include "at103_it.h"

extern __IO uint32_t DMA_Channel1_TransferDoneFlag;

void DMA_Channel1_IRQHandler(void)
{
    /* clear interrupt force bit */
    if (DMA_GetITStatus(DMA_IT_TC1) != RESET) {
        debug("[IRQ] DMA channel1 transfer complete\r\n");
        DMA_ClearITPendingBit(DMA_IT_TC1);
        DMA_Channel1_TransferDoneFlag = 1;
    }
    if (DMA_GetITStatus(DMA_IT_TE1) != RESET) {
        debug("[IRQ] DMA channel1 transfer error\r\n");
        DMA_ClearITPendingBit(DMA_IT_TE1);
    }
}
