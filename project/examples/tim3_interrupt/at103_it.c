/**
 * @file at103_it.c
 * @brief This file tim one pulse mode.
 * @author liyouchuang (lycdemeng@163.com)
 * @version 1.0
 * @date 2022-03-29
 * @copyright Copyright (c) 2022 Icore, Inc
 */

// #include "at103_tim.h"

#include "at103_it.h"

void TIM3_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) {
        /* Clear the interrupt flag bit */
        TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
        debug("[IRQ] Tim interrupt success!!!\r\n");
        GPIO_SetBits(GPIOE, GPIO_Pin_3);
    }
}