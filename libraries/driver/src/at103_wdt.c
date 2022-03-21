/**
 * @file at103_wdt.c
 * @brief This file provides all the WDT firmware functions.
 * @author pantianwen (pantianwen@163.com)
 * @version 1.0
 * @date 2022-03-17
 * @copyright Copyright (c) 2022 Icore, Inc
 */
#include "at103_wdt.h"
#ifdef WDT_MODULE_ENABLED

void WWDG_Enable()
{
    WWDG->TCR.ENANBLE = 0x1;
}

void WWDG_Disable()
{
    WWDG->TCR.ENANBLE = 0x0;
}

void WWDG_SetReload(uint32_t Reload)
{
    assert_param(IS_WWDG_COUNTER(Reload));
    WWDG->LDR = Reload;
}

void WWDG_SetMode(uint8_t Mode)
{
    assert_param(IS_WWDG_MODE(Mode));
    WWDG->TCR.ACTION = Mode;
}

void WWDG_ReloadCounter(void)
{
    WWDG->CRR.value = KEY_Reload;
}

void WWDG_ClearFlag(void)
{
    WWDG->ISR.INTSTAT = 0x1;
}

void WWDG_Set_Reset_Pulselength(uint16_t length)
{
    assert_param(IS_WWDG_LENGTH(length));
    WWDG->PLR.value = length;
}

void WWDG_SetPrescaler(uint32_t WWDG_Higher_Prescaler, uint32_t WWDG_Lower_Prescaler)
{
    assert_param(IS_WWDG_HIGHER_PRESCALER(WWDG_Higher_Prescaler));
    assert_param(IS_WWDG_LOWER_PRESCALER(WWDG_Lower_Prescaler));
    WWDG->TCR.PSC_DIV_HIGH = WWDG_Higher_Prescaler;
    WWDG->TCR.PSC_DIV_LOW  = WWDG_Lower_Prescaler;
}

void WWDG_Set_Timeout_range(uint8_t cycle)
{
    assert_param(IS_WWDG_CYCLES(cycle));
    WWDG->TCR.TIMEOUT_RANGE = cycle;
}

#endif