/**
 * @file at103_it.c
 * @brief This file provides template for all exceptions handler
 *        and peripherals interrupt service routine.
 * @author liyouchuang (lycdemeng@163.com)
 * @version 1.0
 * @date 2022-05-18
 * @copyright Copyright (c) 2022 Icore, Inc
 */

#include "at103_it.h"

uint16_t TIM4CH3_CAPTURE_STA = 0;
uint32_t TIM4CH3_CAPTURE_VAL;

void TIM4_IRQHandler(void)
{
    if ((TIM4CH3_CAPTURE_STA & 0x80) == 0) {
        if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET) {
            if (TIM4CH3_CAPTURE_STA & 0x40) {
                if ((TIM4CH3_CAPTURE_STA & 0x3F) == 0x3F) {
                    TIM4CH3_CAPTURE_STA |= 0x80;
                    TIM4CH3_CAPTURE_VAL = 0xFFFF;
                } else
                    TIM4CH3_CAPTURE_STA++;
            }
        }
        if (TIM_GetITStatus(TIM4, TIM_IT_CC3) != RESET) {
            if (TIM4CH3_CAPTURE_STA & 0x40) {
                TIM4CH3_CAPTURE_STA |= 0x80; //capture success
                TIM4CH3_CAPTURE_VAL = TIM_GetCapture3(TIM4);
                TIM_OC3PolarityConfig(TIM4, TIM_ICPolarity_Rising);
                // debug("InputCapture1 = %x\r\n", TIM4CH3_CAPTURE_STA);
            } else {                     //captured
                TIM4CH3_CAPTURE_STA = 0; //clear
                TIM4CH3_CAPTURE_VAL = 0;
                TIM_SetCounter(TIM4, 0);
                TIM4CH3_CAPTURE_STA |= 0x40; //get high level
                TIM_OC3PolarityConfig(TIM4, TIM_ICPolarity_Falling);
            }
        }
        // debug("InputCapture1 = %x\r\n", TIM4CH3_CAPTURE_STA);
    }
    TIM_ClearITPendingBit(TIM4, TIM_IT_CC3 | TIM_IT_Update);
}