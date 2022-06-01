/**
 * @file at103_it.c
 * @brief This file provides template for all exceptions handler
 *        and peripherals interrupt service routine.
 * @author liyouchuang (lycdemeng@163.com)
 * @version 1.0
 * @date 2022-04-17
 * @copyright Copyright (c) 2022 Icore, Inc
 */

#include "at103_it.h"
#include "unity.h"

extern __IO uint32_t DMA_Channel1_TransferDoneFlag;

void DMA_Channel1_IRQHandler(void)
{
    /* clear interrupt force bit */
    if (DMA_GetITStatus(DMA_IT_TC1) != RESET) {
        // debug("[IRQ] DMA channel1 transfer complete\r\n");
        DMA_ClearITPendingBit(DMA_IT_TC1);
        DMA_Channel1_TransferDoneFlag++;
    }
    TEST_ASSERT_MESSAGE(DMA_GetITStatus(DMA_IT_TE1) == RESET, "[IRQ] DMA channel1 transfer error");
}