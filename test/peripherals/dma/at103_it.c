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
#include "unity.h"

extern __IO uint32_t DMA_TransferDoneFlag;

void DMA_Channel1_IRQHandler(void)
{
    /* clear interrupt force bit */
    if (DMA_GetITStatus(DMA_IT_TC1) != RESET) {
        // debug("[IRQ] DMA channel1 transfer complete\r\n");
        DMA_ClearITPendingBit(DMA_IT_TC1);
        DMA_TransferDoneFlag++;
    }
    TEST_ASSERT_MESSAGE(DMA_GetITStatus(DMA_IT_TE1) == RESET, "[IRQ] DMA channel1 transfer error");
}

void DMA_Channel2_IRQHandler(void)
{
    /* clear interrupt force bit */
    if (DMA_GetITStatus(DMA_IT_TC2) != RESET) {
        // debug("[IRQ] DMA channel2 transfer complete\r\n");
        DMA_ClearITPendingBit(DMA_IT_TC2);
        DMA_TransferDoneFlag++;
    }
    TEST_ASSERT_MESSAGE(DMA_GetITStatus(DMA_IT_TE2) == RESET, "[IRQ] DMA channel2 transfer error");
}

void DMA_Channel3_IRQHandler(void)
{
    /* clear interrupt force bit */
    if (DMA_GetITStatus(DMA_IT_TC3) != RESET) {
        // debug("[IRQ] DMA channel3 transfer complete\r\n");
        DMA_ClearITPendingBit(DMA_IT_TC3);
        DMA_TransferDoneFlag++;
    }
    TEST_ASSERT_MESSAGE(DMA_GetITStatus(DMA_IT_TE3) == RESET, "[IRQ] DMA channel3 transfer error");
}

void DMA_Channel4_IRQHandler(void)
{
    /* clear interrupt force bit */
    if (DMA_GetITStatus(DMA_IT_TC4) != RESET) {
        // debug("[IRQ] DMA channel4 transfer complete\r\n");
        DMA_ClearITPendingBit(DMA_IT_TC4);
        DMA_TransferDoneFlag++;
    }
    TEST_ASSERT_MESSAGE(DMA_GetITStatus(DMA_IT_TE4) == RESET, "[IRQ] DMA channel4 transfer error");
}

void DMA_Channel5_IRQHandler(void)
{
    /* clear interrupt force bit */
    if (DMA_GetITStatus(DMA_IT_TC5) != RESET) {
        // debug("[IRQ] DMA channel5 transfer complete\r\n");
        DMA_ClearITPendingBit(DMA_IT_TC5);
        DMA_TransferDoneFlag++;
    }
    TEST_ASSERT_MESSAGE(DMA_GetITStatus(DMA_IT_TE5) == RESET, "[IRQ] DMA channel5 transfer error");
}

void DMA_Channel6_IRQHandler(void)
{
    /* clear interrupt force bit */
    if (DMA_GetITStatus(DMA_IT_TC6) != RESET) {
        // debug("[IRQ] DMA channel6 transfer complete\r\n");
        DMA_ClearITPendingBit(DMA_IT_TC6);
        DMA_TransferDoneFlag++;
    }
    TEST_ASSERT_MESSAGE(DMA_GetITStatus(DMA_IT_TE6) == RESET, "[IRQ] DMA channel6 transfer error");
}

void DMA_Channel7_IRQHandler(void)
{
    /* clear interrupt force bit */
    if (DMA_GetITStatus(DMA_IT_TC7) != RESET) {
        // debug("[IRQ] DMA channel7 transfer complete\r\n");
        DMA_ClearITPendingBit(DMA_IT_TC7);
        DMA_TransferDoneFlag++;
    }
    TEST_ASSERT_MESSAGE(DMA_GetITStatus(DMA_IT_TE7) == RESET, "[IRQ] DMA channel7 transfer error");
}

void DMA_Channel8_IRQHandler(void)
{
    /* clear interrupt force bit */
    if (DMA_GetITStatus(DMA_IT_TC8) != RESET) {
        // debug("[IRQ] DMA channel8 transfer complete\r\n");
        DMA_ClearITPendingBit(DMA_IT_TC8);
        DMA_TransferDoneFlag++;
    }
    TEST_ASSERT_MESSAGE(DMA_GetITStatus(DMA_IT_TE8) == RESET, "[IRQ] DMA channel8 transfer error");
}
