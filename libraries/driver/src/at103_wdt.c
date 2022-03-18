/**
 * @file at103_wdt.c
 * @brief 
 * 
 * @author pantianwen (pantianwen@163.com)
 * @version 1.0
 * @date 2022-03-17
 * @copyright Copyright (c) 2022 Icore, Inc
 */
#include "at103_wdt.h"
#ifdef WDT_MODULE_ENABLED
/**
 * @brief Enable WWDG 
 * 
 */
void WWDG_Enable()
{
    WWDG->TCR.ENANBLE = 0x1;
}

/**
 * @brief Reloads WWDG counter with value defined in the reload register
 * @param Reload 
 * @retval None
 */
void WWDG_SetReload(uint32_t Reload)
{
    assert_param(IS_WWDG_COUNTER(Reload));
    WWDG->LDR = Reload;
}

/**
 * @brief Set the mode of WWDG
 * @param Mode :reset or interrupt
 * @retval None
 */
void WWDG_SetMode(uint8_t Mode)
{
    assert_param(IS_WWDG_MODE(Mode));
    WWDG->TCR.ACTION = Mode;
}
/**
 * @brief Restart the WWDG counter.
 * @param  None.
 * @retval None.
 */
void WWDG_ReloadCounter(void)
{
    WWDG->CRR.value = KEY_Reload;
}

/**
 * @brief clear interrupt flag
 * @param  None.
 * @retval None.
 */
void WWDG_ClearFlag(void)
{
    WWDG->ISR.INTSTAT = 0x1;
}

/**
 * @brief Set the length of Watchdog Timer Reset Pulse
 * @param length The value of reset pulse length is the number of pclk cycles. At the least 2 pclk cycles
 * @retval None.
 */
void WWDG_Set_Reset_Pulselength(uint16_t length)
{
    assert_param(IS_WWDG_LENGTH(length));
    WWDG->PLR.value = length;
}

/**
 * @brief Sets WWDG higher and lower Prescaler value
 * @param WWDG_Higher_Prescaler :specifies the WWDG higher presaclaer value
 * @param WWDG_Lower_Prescaler :specifies the WWDG lower presaclaer value
 * @retval None.
 */
void WWDG_SetPrescaler(uint32_t WWDG_Higher_Prescaler, uint32_t WWDG_Lower_Prescaler)
{
    assert_param(IS_WWDG_HIGHER_PRESCALER(WWDG_Higher_Prescaler));
    assert_param(IS_WWDG_LOWER_PRESCALER(WWDG_Lower_Prescaler));
    WWDG->TCR.PSC_DIV_HIGH = WWDG_Higher_Prescaler;
    WWDG->TCR.PSC_DIV_LOW  = WWDG_Lower_Prescaler;
}

/**
 * @brief set Time-out time,When a WDT Interrupt is generated, 
 *        if it is not cleared before the Time-out time, then it generates a system reset. 
 * @param cycle  WWDG Counter clock cycles 
 * @retval None.
 */
void WWDG_Set_Timeout_range(uint8_t cycle)
{
    assert_param(IS_WWDG_CYCLES(cycle));
    WWDG->TCR.TIMEOUT_RANGE = cycle;
}

#endif