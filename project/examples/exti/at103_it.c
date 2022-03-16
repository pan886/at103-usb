#include "at103_it.h"

void EXTI0_IRQHandler(void)
{
    if (EXTI_GetITStatus(EXTI_Line0) != RESET) {
        debug("[IRQ]EXTI Line0 !!!\r\n");
        /* Clear the  EXTI line 0 pending bit */
        EXTI_ClearITPendingBit(EXTI_Line0);
    }
}