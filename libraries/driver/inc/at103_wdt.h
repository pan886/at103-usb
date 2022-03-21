/**
 * @file at103_wdt.h
 * @brief This file contains all the functions prototypes for the WDT firmware 
 *         library.
 * @author pantianwen (pantianwen@163.com)
 * @version 1.0
 * @date 2022-03-17
 * @copyright Copyright (c) 2022 Icore, Inc
 */
#include "at103.h"

#define RESET_MODE         0x0
#define INTERRUPT_MODE     0x1
#define IS_WWDG_MODE(Mode) ((((Mode) == RESET_MODE) || \
                             ((Mode) == INTERRUPT_MODE)))
#define Counter_Cycles_64 ((uint8_t)0x0000)
#define Counter_Cycles_32 ((uint8_t)0x0001)
#define Counter_Cycles_16 ((uint8_t)0x0002)
#define Counter_Cycles_8  ((uint8_t)0x0003)
#define Counter_Cycles_4  ((uint8_t)0x0004)
#define Counter_Cycles_2  ((uint8_t)0x0005)
#define Counter_Cycles_1  ((uint8_t)0x0006)
#define IS_WWDG_CYCLES(CYCLES)       (((CYCLES) == Counter_Cycles_64)||\
                                     ((CYCLES)  == Counter_Cycles_32)||\
                                     ((CYCLES)  == Counter_Cycles_16)||\
                                     ((CYCLES)  == Counter_Cycles_8)||\
                                     ((CYCLES)  == Counter_Cycles_4)||\
                                     ((CYCLES)  == Counter_Cycles_2)||\
                                     ((CYCLES)  == Counter_Cycles_1)))

#define WWDG_Higher_Prescaler_2             ((uint32_t)0x0000)
#define WWDG_Higher_Prescaler_4             ((uint32_t)0x0001)
#define WWDG_Higher_Prescaler_8             ((uint32_t)0x0002)
#define WWDG_Higher_Prescaler_16            ((uint32_t)0x0003)
#define WWDG_Higher_Prescaler_32            ((uint32_t)0x0004)
#define WWDG_Higher_Prescaler_64            ((uint32_t)0x0005)
#define WWDG_Higher_Prescaler_128           ((uint32_t)0x0006)
#define WWDG_Higher_Prescaler_256           ((uint32_t)0x0007)
#define WWDG_Higher_Prescaler_512           ((uint32_t)0x0008)
#define WWDG_Higher_Prescaler_1024          ((uint32_t)0x0009)
#define WWDG_Higher_Prescaler_2048          ((uint32_t)0x000A)
#define WWDG_Higher_Prescaler_4096          ((uint32_t)0x000B)
#define WWDG_Higher_Prescaler_8192          ((uint32_t)0x000C)
#define WWDG_Higher_Prescaler_16384         ((uint32_t)0x000D)
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

#define WWDG_Lower_Prescaler_1             ((uint32_t)0x0000)
#define WWDG_Lower_Prescaler_2             ((uint32_t)0x0001)
#define WWDG_Lower_Prescaler_4             ((uint32_t)0x0002)
#define WWDG_Lower_Prescaler_8             ((uint32_t)0x0003)
#define WWDG_Lower_Prescaler_16            ((uint32_t)0x0004)
#define WWDG_Lower_Prescaler_32            ((uint32_t)0x0005)
#define WWDG_Lower_Prescaler_64            ((uint32_t)0x0006)
#define WWDG_Lower_Prescaler_128           ((uint32_t)0x0007)
#define WWDG_Lower_Prescaler_256           ((uint32_t)0x0008)
#define WWDG_Lower_Prescaler_512           ((uint32_t)0x0009)
#define WWDG_Lower_Prescaler_1024          ((uint32_t)0x000A)
#define WWDG_Lower_Prescaler_2048          ((uint32_t)0x000B)
#define WWDG_Lower_Prescaler_4096          ((uint32_t)0x000C)
#define WWDG_Lower_Prescaler_8192          ((uint32_t)0x000D)
#define WWDG_Lower_Prescaler_16384         ((uint32_t)0x000E)
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
#define KEY_Reload ((uint16_t)0x76)

#define IS_WWDG_COUNTER(COUNTER) (COUNTER <= 0xFFFFFFFF)
#define IS_WWDG_LENGTH(LENGTH)   ((LENGTH <= 0XFFFF) && (LENGTH >= 2))

/**
 * @brief clear interrupt flag
 */
void WWDG_ClearFlag(void);

/**
 * @brief Reloads WWDG counter with value defined in the reload register
 * @param[in] Reload :Watchdog Timer load value 
 */
void WWDG_SetReload(uint32_t Reload);

/**
 * @brief :Set the length of Watchdog Timer Reset Pulse
 * @param[in] length:The value of reset pulse length is the number of pclk cycles. At the least 2 pclk cycles
 */
void WWDG_Set_Reset_Pulselength(uint16_t length);

/**
 * @brief :Sets WWDG higher and lower Prescaler value
 * @param[in] WWDG_Higher_Prescaler:specifies the WWDG higher presaclaer value
 * @param[in] WWDG_Lower_Prescaler:specifies the WWDG lower presaclaer value
 */
void WWDG_SetPrescaler(uint32_t WWDG_Higher_Prescaler, uint32_t WWDG_Lower_Prescaler);

/**
 * @brief :set Time-out time,When a WDT Interrupt is generated, if it is not cleared before the Time-out time, then it generates a system reset.
 * @param[in] cycle:WWDG Counter clock cycles. 
 */
void WWDG_Set_Timeout_range(uint8_t cycle);

/**
 * @brief Enable WWDG 
 */
void WWDG_Enable();

/**
 * @brief Disable WWDG
 */
void WWDG_Disable();

/**
 * @brief :Set the mode of WWDG
 * @param[in] Mode :reset or interrupt
 */
void WWDG_SetMode(uint8_t Mode);

/**
 * @brief: Restart the WWDG counter.
 */
void WWDG_ReloadCounter(void);
