/**
 * @file at103_wwdg.h
 * @brief This file contains all the functions prototypes for the WWDG firmware 
 *         library. \n
 *        How to use this driver? \n
 * (+) At first, use  WWDG_Enable() to enable WWDG. \n
 * (+) Then,use WWDG_SetMode(...)to set the mode of wwdg.the mode can be interrupt or reset. \n
 * (+) Use WWDG_SetPrescaler(...)to set the Prescaler value. \n
 * (+) Finally,use WWDG_ReloadCounter() to restart the WWDG counter. \n
 * (+) Note: \n
 * @author pantianwen (pantianwen@163.com)
 * @version 1.0
 * @date 2022-03-17
 * @copyright Copyright (c) 2022 Icore, Inc
 */
#include "at103.h"

#ifdef __cplusplus
extern "C" {
#endif

#define RESET_MODE     0x0 /**< reset mode */
#define INTERRUPT_MODE 0x1 /**< interrupt mode */

#define Counter_Cycles_64 ((uint8_t)0x0000) /**< 64K of WWDG Counter clock cycles */
#define Counter_Cycles_32 ((uint8_t)0x0001) /**< 32K of WWDF Counter clock cycles */
#define Counter_Cycles_16 ((uint8_t)0x0002) /**< 16K of WWDG Counter clock cycles */
#define Counter_Cycles_8  ((uint8_t)0x0003) /**< 8K of WWDG Counter clock cycles */
#define Counter_Cycles_4  ((uint8_t)0x0004) /**< 4K of WWDG Counter clock cycles */
#define Counter_Cycles_2  ((uint8_t)0x0005) /**< 2K of WWDG Counter clock cycles */
#define Counter_Cycles_1  ((uint8_t)0x0006) /**< 1K of WWDG Counter clock cycles */

#define WWDG_Higher_Prescaler_2     ((uint32_t)0x0000) /**< system clock Divide by 2 */
#define WWDG_Higher_Prescaler_4     ((uint32_t)0x0001) /**< system clock Divide by 4 */
#define WWDG_Higher_Prescaler_8     ((uint32_t)0x0002) /**< system clock Divide by 8 */
#define WWDG_Higher_Prescaler_16    ((uint32_t)0x0003) /**< system clock Divide by 16 */
#define WWDG_Higher_Prescaler_32    ((uint32_t)0x0004) /**< system clock Divide by 32 */
#define WWDG_Higher_Prescaler_64    ((uint32_t)0x0005) /**< system clock Divide by 64 */
#define WWDG_Higher_Prescaler_128   ((uint32_t)0x0006) /**< system clock Divide by 128 */
#define WWDG_Higher_Prescaler_256   ((uint32_t)0x0007) /**< system clock Divide by 256 */
#define WWDG_Higher_Prescaler_512   ((uint32_t)0x0008) /**< system clock Divide by 512 */
#define WWDG_Higher_Prescaler_1024  ((uint32_t)0x0009) /**< system clock Divide by 1024 */
#define WWDG_Higher_Prescaler_2048  ((uint32_t)0x000A) /**< system clock Divide by 2048 */
#define WWDG_Higher_Prescaler_4096  ((uint32_t)0x000B) /**< system clock Divide by 4096 */
#define WWDG_Higher_Prescaler_8192  ((uint32_t)0x000C) /**< system clock Divide by 8192 */
#define WWDG_Higher_Prescaler_16384 ((uint32_t)0x000D) /**< system clock Divide by 16384 */

#define WWDG_Lower_Prescaler_1     ((uint32_t)0x0000) /**< No divide */
#define WWDG_Lower_Prescaler_2     ((uint32_t)0x0001) /**< further Divide by 2 */
#define WWDG_Lower_Prescaler_4     ((uint32_t)0x0002) /**< further Divide by 4 */
#define WWDG_Lower_Prescaler_8     ((uint32_t)0x0003) /**< further Divide by 8 */
#define WWDG_Lower_Prescaler_16    ((uint32_t)0x0004) /**< further Divide by 16 */
#define WWDG_Lower_Prescaler_32    ((uint32_t)0x0005) /**< further Divide by 32 */
#define WWDG_Lower_Prescaler_64    ((uint32_t)0x0006) /**< further Divide by 64 */
#define WWDG_Lower_Prescaler_128   ((uint32_t)0x0007) /**< further Divide by 128 */
#define WWDG_Lower_Prescaler_256   ((uint32_t)0x0008) /**< further Divide by 256 */
#define WWDG_Lower_Prescaler_512   ((uint32_t)0x0009) /**< further Divide by 512 */
#define WWDG_Lower_Prescaler_1024  ((uint32_t)0x000A) /**< further Divide by 1024 */
#define WWDG_Lower_Prescaler_2048  ((uint32_t)0x000B) /**< further Divide by 2048 */
#define WWDG_Lower_Prescaler_4096  ((uint32_t)0x000C) /**< further Divide by 4096 */
#define WWDG_Lower_Prescaler_8192  ((uint32_t)0x000D) /**< further Divide by 8192 */
#define WWDG_Lower_Prescaler_16384 ((uint32_t)0x000E) /**< further Divide by 16384 */

#define KEY_Reload ((uint16_t)0x76) /**<  write “0x76” to WDT_CRR, then the LDR value can be load to Restart counter*/

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
 * @param[in] length The value of reset pulse length is the number of pclk cycles. At the least 2 pclk cycles
 */
void WWDG_Set_Reset_Pulselength(uint16_t length);

/**
 * @brief :Sets WWDG higher and lower Prescaler value
 * @param[in] WWDG_Higher_Prescaler specifies the WWDG higher presaclaer value
 * @param[in] WWDG_Lower_Prescaler specifies the WWDG lower presaclaer value
 */
void WWDG_SetPrescaler(uint32_t WWDG_Higher_Prescaler, uint32_t WWDG_Lower_Prescaler);

/**
 * @brief :set Time-out time,When a WDT Interrupt is generated, if it is not cleared before the Time-out time, then it generates a system reset.
 * @param[in] cycle WWDG Counter clock cycles. 
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
