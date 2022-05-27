/**
 * @file at103_wwdg.c
 * @brief This file provides all the WWDG firmware functions.
 * @author pantianwen (pantianwen@163.com)
 * @version 1.0
 * @date 2022-03-17
 * @copyright Copyright (c) 2022 Icore, Inc
 */
#include "at103_wwdg.h"
#ifdef WWDG_MODULE_ENABLED

#define IS_WWDG_MODE(Mode) ((((Mode) == RESET_MODE) || \
                             ((Mode) == INTERRUPT_MODE)))
#define IS_WWDG_CYCLES(CYCLES)       (((CYCLES) == Counter_Cycles_64)||\
                                     ((CYCLES)  == Counter_Cycles_32)||\
                                     ((CYCLES)  == Counter_Cycles_16)||\
                                     ((CYCLES)  == Counter_Cycles_8)||\
                                     ((CYCLES)  == Counter_Cycles_4)||\
                                     ((CYCLES)  == Counter_Cycles_2)||\
                                     ((CYCLES)  == Counter_Cycles_1)))
#define IS_WWDG_HIGHER_PRESCALER(PRESCALER) (((PRESCALER) == WWDG_Higher_Prescaler_2) || \
                                             ((PRESCALER) == WWDG_Higher_Prescaler_4) || \
                                             ((PRESCALER) == WWDG_Higher_Prescaler_8) || \
                                             ((PRESCALER) == WWDG_Higher_Prescaler_16) || \
                                             ((PRESCALER) == WWDG_Higher_Prescaler_32) || \
                                             ((PRESCALER) == WWDG_Higher_Prescaler_64) || \
                                             ((PRESCALER) == WWDG_Higher_Prescaler_128) || \
                                             ((PRESCALER) == WWDG_Higher_Prescaler_256) || \
                                             ((PRESCALER) == WWDG_Higher_Prescaler_512) || \
                                             ((PRESCALER) == WWDG_Higher_Prescaler_1024) || \
                                             ((PRESCALER) == WWDG_Higher_Prescaler_2048) || \
                                             ((PRESCALER) == WWDG_Higher_Prescaler_4096) || \
                                             ((PRESCALER) == WWDG_Higher_Prescaler_8192) || \
                                             ((PRESCALER) == WWDG_Higher_Prescaler_16384))

#define IS_WWDG_LOWER_PRESCALER(PRESCALER) (((PRESCALER) == WWDG_Lower_Prescaler_1) || \
                                            ((PRESCALER) == WWDG_Lower_Prescaler_2) || \
                                            ((PRESCALER) == WWDG_Lower_Prescaler_4) || \
                                            ((PRESCALER) == WWDG_Lower_Prescaler_8) || \
                                            ((PRESCALER) == WWDG_Lower_Prescaler_16) || \
                                            ((PRESCALER) == WWDG_Lower_Prescaler_32) || \
                                            ((PRESCALER) == WWDG_Lower_Prescaler_64) || \
                                            ((PRESCALER) == WWDG_Lower_Prescaler_128) || \
                                            ((PRESCALER) == WWDG_Lower_Prescaler_256) || \
                                            ((PRESCALER) == WWDG_Lower_Prescaler_512) || \
                                            ((PRESCALER) == WWDG_Lower_Prescaler_1024) || \
                                            ((PRESCALER) == WWDG_Lower_Prescaler_2048) || \
                                            ((PRESCALER) == WWDG_Lower_Prescaler_4096) || \
                                            ((PRESCALER) == WWDG_Lower_Prescaler_8192) || \
                                            ((PRESCALER) == WWDG_Lower_Prescaler_16384))

#define IS_WWDG_COUNTER(COUNTER) (COUNTER <= 0xFFFFFFFF)
#define IS_WWDG_LENGTH(LENGTH)   ((LENGTH <= 0XFFFF) && (LENGTH >= 2))
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