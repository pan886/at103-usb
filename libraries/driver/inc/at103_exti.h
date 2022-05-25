/**
 * @file at103_exti.h
 * @brief This file contains all the functions prototypes for the EXTI firmware library. \n
 *        How to use this driver? \n
 * (+) Use EXTI_Init(...) with EXTI initialization structure to initialize a EXTI line, this module must work with the GPIO module. \n
 *     For details on the EXTI initialization structure, see "EXTI_InitTypeDef". \n
 * @author Dong Qin (dong.qin@timesintelli.com)
 * @version 1.0
 * @date 2022-03-11
 * @copyright Copyright (c) 2022 Timesintelli, Inc
 */

#ifndef __AT103_EXTI_H
#define __AT103_EXTI_H
#include "at103.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief EXTI mode enumeration.
 */
typedef enum {
    EXTI_Mode_Interrupt = 0x00, /**< EXTI interrupt mode. */
    EXTI_Mode_Event             /**< EXTI event mode. */
} EXTIMode_TypeDef;

/**
 * @brief EXTI Trigger enumeration.
 */
typedef enum {
    EXTI_Trigger_Falling = 0x1, /**< EXTI falling edge trigger mode. */
    EXTI_Trigger_Rising,        /**< EXTI rising edge trigger mode. */
    EXTI_Trigger_Rising_Falling /**< EXTI rising and falling edge trigger mode. */
} EXTITrigger_TypeDef;

/**
 * @brief EXTI Init Structrure definition.
 */
typedef struct
{
    uint32_t            EXTI_Line;    /**< Specifies the EXTI lines to be enabled or disabled. */
    EXTIMode_TypeDef    EXTI_Mode;    /**< Specifies the mode for the EXTI lines. This parameter can be a value of EXTIMode_TypeDef. */
    EXTITrigger_TypeDef EXTI_Trigger; /**< Specifies the trigger signal active edge for the EXTI lines. */
    FunctionalState     EXTI_LineCmd; /**< Specifies the new state of the selected EXTI lines. */
} EXTI_InitTypeDef;

#define EXTI_Line0  ((uint32_t)0x0001) /**< EXTI line 0. */
#define EXTI_Line1  ((uint32_t)0x0002) /**< EXTI line 1. */
#define EXTI_Line2  ((uint32_t)0x0004) /**< EXTI line 2. */
#define EXTI_Line3  ((uint32_t)0x0008) /**< EXTI line 3. */
#define EXTI_Line4  ((uint32_t)0x0010) /**< EXTI line 4. */
#define EXTI_Line5  ((uint32_t)0x0020) /**< EXTI line 5. */
#define EXTI_Line6  ((uint32_t)0x0040) /**< EXTI line 6. */
#define EXTI_Line7  ((uint32_t)0x0080) /**< EXTI line 7. */
#define EXTI_Line8  ((uint32_t)0x0100) /**< EXTI line 8. */
#define EXTI_Line9  ((uint32_t)0x0200) /**< EXTI line 9. */
#define EXTI_Line10 ((uint32_t)0x0400) /**< EXTI line 10. */
#define EXTI_Line11 ((uint32_t)0x0800) /**< EXTI line 11. */
#define EXTI_Line12 ((uint32_t)0x1000) /**< EXTI line 12. */
#define EXTI_Line13 ((uint32_t)0x2000) /**< EXTI line 13. */
#define EXTI_Line14 ((uint32_t)0x4000) /**< EXTI line 14. */
#define EXTI_Line15 ((uint32_t)0x8000) /**< EXTI line 15. */

/**
 * @brief Deinitializes the EXTI peripheral registers to their default reset values.
 */
void EXTI_DeInit(void);

/**
 * @brief Initializes the EXTI peripheral according to the specified parameters in the EXTI_InitStruct.
 * @param[in] EXTI_InitStruct pointer to a EXTI_InitTypeDef structure that contains the configuration information for the EXTI peripheral.
 */
void EXTI_Init(EXTI_InitTypeDef *EXTI_InitStruct);

/**
 * @brief Fills each EXTI_InitStruct member with its reset value.
 * @param[in] EXTI_InitStruct pointer to a EXTI_InitTypeDef structure which will be initialized.
 */
void EXTI_StructInit(EXTI_InitTypeDef *EXTI_InitStruct);

/**
 * @brief Checks whether the specified EXTI line flag is set or not.
 * @param[in] EXTI_Line specifies the EXTI line flag to check.
 * @return FlagStatus The new state of EXTI_Line (SET or RESET).
 */
FlagStatus EXTI_GetFlagStatus(uint32_t EXTI_Line);

/**
 * @brief Clears the EXTI's line pending flags.
 * @param[in] EXTI_Line specifies the EXTI line flag to check.
 */
void EXTI_ClearFlag(uint32_t EXTI_Line);

/**
 * @brief Checks whether the specified EXTI line is asserted or not.
 * @param[in] EXTI_Line specifies the EXTI line flag to check.
 * @return ITStatus The new state of EXTI_Line (SET or RESET).
 */
ITStatus EXTI_GetITStatus(uint32_t EXTI_Line);

/**
 * @brief Clears the EXTI's line pending bits.
 * @param[in] EXTI_Line specifies the EXTI line flag to check.
 */
void EXTI_ClearITPendingBit(uint32_t EXTI_Line);

#ifdef __cplusplus
}
#endif

#endif