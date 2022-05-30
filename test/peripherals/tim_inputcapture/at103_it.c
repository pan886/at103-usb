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

uint16_t TIM3CH1_CAPTURE_STA = 0;
uint32_t TIM3CH1_CAPTURE_VAL;

void TIM3_IRQHandler(void)
{
    if ((TIM3CH1_CAPTURE_STA & 0x80) == 0) {
        if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) {
            if (TIM3CH1_CAPTURE_STA & 0x40) {
                if ((TIM3CH1_CAPTURE_STA & 0x3F) == 0x3F) {
                    TIM3CH1_CAPTURE_STA |= 0x80;
                    TIM3CH1_CAPTURE_VAL = 0xFFFF;
                } else
                    TIM3CH1_CAPTURE_STA++;
            }
        }
        if (TIM_GetITStatus(TIM3, TIM_IT_CC1) != RESET) {
            if (TIM3CH1_CAPTURE_STA & 0x40) {
                TIM3CH1_CAPTURE_STA |= 0x80; //capture success
                TIM3CH1_CAPTURE_VAL = TIM_GetCapture1(TIM3);
                TIM_OC1PolarityConfig(TIM3, TIM_ICPolarity_Rising);
                // debug("InputCapture1 = %x\r\n", (TIM3CH1_CAPTURE_STA & 0x40));
            } else {                     //captured
                TIM3CH1_CAPTURE_STA = 0; //clear
                TIM3CH1_CAPTURE_VAL = 0;
                TIM_SetCounter(TIM3, 0);
                TIM3CH1_CAPTURE_STA |= 0x40; //get high level
                TIM_OC1PolarityConfig(TIM3, TIM_ICPolarity_Falling);
            }
        }
        // debug("InputCapture1 = %x\r\n", TIM3CH1_CAPTURE_STA);
    }
    TIM_ClearITPendingBit(TIM3, TIM_IT_CC1 | TIM_IT_Update);
}