/**
 * @file at103_gpio.h
 * @brief This file contains all the functions prototypes for the GPIO firmware library.
 * @author Dong Qin (dong.qin@timesintelli.com)
 * @version 1.0
 * @date 2022-03-14
 * @copyright Copyright (c) 2022 Timesintelli, Inc
 */

#ifndef __AT103_GPIO_H
#define __AT103_GPIO_H
#include "at103.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief GPIO_Exported_Types.
 */
#define IS_GPIO_ALL_PERIPH(PERIPH) (((PERIPH) == GPIOA) || \
                                    ((PERIPH) == GPIOB) || \
                                    ((PERIPH) == GPIOC) || \
                                    ((PERIPH) == GPIOD) || \
                                    ((PERIPH) == GPIOE))

/**
 * @brief Output Maximum frequency selection.
 */
typedef enum {
    GPIO_Speed_2MHz  = 0x1,
    GPIO_Speed_10MHz = 0x2,
    GPIO_Speed_50MHz = 0x3,
} GPIOSpeed_TypeDef;

#define IS_GPIO_SPEED(SPEED) (((SPEED) == GPIO_Speed_10MHz) || ((SPEED) == GPIO_Speed_2MHz) || \
                              ((SPEED) == GPIO_Speed_50MHz))

/**
 * @brief Configuration Mode enumeration.
 */
typedef enum {
    GPIO_Mode_AIN         = 0x30,
    GPIO_Mode_IN_FLOATING = 0x00,
    GPIO_Mode_IPD         = 0x08,
    GPIO_Mode_IPU         = 0x0C,
    GPIO_Mode_Out_OD      = 0x03,
    GPIO_Mode_Out_PP      = 0x01,
    GPIO_Mode_AF          = 0x10,
} GPIOMode_TypeDef;

#define IS_GPIO_MODE(MODE) (((MODE) == GPIO_Mode_AIN) || ((MODE) == GPIO_Mode_IN_FLOATING) || \
                            ((MODE) == GPIO_Mode_IPD) || ((MODE) == GPIO_Mode_IPU) || \
                            ((MODE) == GPIO_Mode_Out_OD) || ((MODE) == GPIO_Mode_Out_PP) || \
                            ((MODE) == GPIO_Mode_AF))

/**
 * @brief GPIO Init structure definition.
 */
typedef struct
{
    uint16_t          GPIO_Pin;   /**< Specifies the GPIO pins to be configured. */
    GPIOSpeed_TypeDef GPIO_Speed; /**< Specifies the speed for the selected pins. */
    GPIOMode_TypeDef  GPIO_Mode;  /**< Specifies the operating mode for the selected pins. */
} GPIO_InitTypeDef;

/**
 * @brief  Bit_SET and Bit_RESET enumeration.
 */
typedef enum {
    Bit_RESET = 0,
    Bit_SET
} BitAction;

#define IS_GPIO_BIT_ACTION(ACTION) (((ACTION) == Bit_RESET) || ((ACTION) == Bit_SET))

/**
 * @brief GPIO pins define
 */
#define GPIO_Pin_0   ((uint16_t)0x0001) /**< Pin 0 selected */
#define GPIO_Pin_1   ((uint16_t)0x0002) /**< Pin 1 selected */
#define GPIO_Pin_2   ((uint16_t)0x0004) /**< Pin 2 selected */
#define GPIO_Pin_3   ((uint16_t)0x0008) /**< Pin 3 selected */
#define GPIO_Pin_4   ((uint16_t)0x0010) /**< Pin 4 selected */
#define GPIO_Pin_5   ((uint16_t)0x0020) /**< Pin 5 selected */
#define GPIO_Pin_6   ((uint16_t)0x0040) /**< Pin 6 selected */
#define GPIO_Pin_7   ((uint16_t)0x0080) /**< Pin 7 selected */
#define GPIO_Pin_8   ((uint16_t)0x0100) /**< Pin 8 selected */
#define GPIO_Pin_9   ((uint16_t)0x0200) /**< Pin 9 selected */
#define GPIO_Pin_10  ((uint16_t)0x0400) /**< Pin 10 selected */
#define GPIO_Pin_11  ((uint16_t)0x0800) /**< Pin 11 selected */
#define GPIO_Pin_12  ((uint16_t)0x1000) /**< Pin 12 selected */
#define GPIO_Pin_13  ((uint16_t)0x2000) /**< Pin 13 selected */
#define GPIO_Pin_14  ((uint16_t)0x4000) /**< Pin 14 selected */
#define GPIO_Pin_15  ((uint16_t)0x8000) /**< Pin 15 selected */
#define GPIO_Pin_All ((uint16_t)0xFFFF) /**< All pins selected */

#define IS_GPIO_PIN(PIN) ((((PIN) & (uint16_t)0x00) == 0x00) && ((PIN) != (uint16_t)0x00))

#define IS_GET_GPIO_PIN(PIN) (((PIN) == GPIO_Pin_0) || \
                              ((PIN) == GPIO_Pin_1) || \
                              ((PIN) == GPIO_Pin_2) || \
                              ((PIN) == GPIO_Pin_3) || \
                              ((PIN) == GPIO_Pin_4) || \
                              ((PIN) == GPIO_Pin_5) || \
                              ((PIN) == GPIO_Pin_6) || \
                              ((PIN) == GPIO_Pin_7) || \
                              ((PIN) == GPIO_Pin_8) || \
                              ((PIN) == GPIO_Pin_9) || \
                              ((PIN) == GPIO_Pin_10) || \
                              ((PIN) == GPIO_Pin_11) || \
                              ((PIN) == GPIO_Pin_12) || \
                              ((PIN) == GPIO_Pin_13) || \
                              ((PIN) == GPIO_Pin_14) || \
                              ((PIN) == GPIO_Pin_15))

/**
 * @brief GPIO remap define
 */
#define GPIO_Remap_SPI1          ((uint32_t)0x00000001) /**< SPI1 Alternate Function mapping */
#define GPIO_Remap_I2C1          ((uint32_t)0x00000002) /**< I2C1 Alternate Function mapping */
#define GPIO_Remap_USART1        ((uint32_t)0x00000004) /**< USART1 Alternate Function mapping */
#define GPIO_Remap_USART2        ((uint32_t)0x00000008) /**< USART2 Alternate Function mapping */
#define GPIO_PartialRemap_USART3 ((uint32_t)0x00000010) /**< USART3 Partial Alternate Function mapping */
#define GPIO_FullRemap_USART3    ((uint32_t)0x00000030) /**< USART3 Full Alternate Function mapping */
#define GPIO_PartialRemap_TIM1   ((uint32_t)0x00000040) /**< TIM1 Partial Alternate Function mapping */
#define GPIO_FullRemap_TIM1      ((uint32_t)0x000000C0) /**< TIM1 Full Alternate Function mapping */
#define GPIO_PartialRemap1_TIM2  ((uint32_t)0x00000100) /**< TIM2 Partial1 Alternate Function mapping */
#define GPIO_PartialRemap2_TIM2  ((uint32_t)0x00000200) /**< TIM2 Partial2 Alternate Function mapping */
#define GPIO_FullRemap_TIM2      ((uint32_t)0x00000300) /**< TIM2 Full Alternate Function mapping */
#define GPIO_PartialRemap_TIM3   ((uint32_t)0x00000800) /**< TIM3 Partial Alternate Function mapping */
#define GPIO_FullRemap_TIM3      ((uint32_t)0x00000C00) /**< TIM3 Full Alternate Function mapping */
#define GPIO_Remap_TIM4          ((uint32_t)0x00001000) /**< TIM4 Alternate Function mapping */
#define GPIO_Remap1_CAN          ((uint32_t)0x00004000) /**< CAN Alternate Function mapping */
#define GPIO_Remap2_CAN          ((uint32_t)0x00006000) /**< CAN Alternate Function mapping */
#define GPIO_Remap_PD01          ((uint32_t)0x00008000) /**< PD01 Alternate Function mapping */
#define GPIO_Remap_SPI2          ((uint32_t)0x00010000) /**< SPI2 Alternate Function mapping */

#define IS_GPIO_REMAP(REMAP) (((REMAP) == GPIO_Remap_SPI1) || ((REMAP) == GPIO_Remap_I2C1) || \
                              ((REMAP) == GPIO_Remap_USART1) || ((REMAP) == GPIO_Remap_USART2) || \
                              ((REMAP) == GPIO_PartialRemap_USART3) || ((REMAP) == GPIO_FullRemap_USART3) || \
                              ((REMAP) == GPIO_PartialRemap_TIM1) || ((REMAP) == GPIO_FullRemap_TIM1) || \
                              ((REMAP) == GPIO_PartialRemap1_TIM2) || ((REMAP) == GPIO_PartialRemap2_TIM2) || \
                              ((REMAP) == GPIO_FullRemap_TIM2) || ((REMAP) == GPIO_PartialRemap_TIM3) || \
                              ((REMAP) == GPIO_FullRemap_TIM3) || ((REMAP) == GPIO_Remap_TIM4) || \
                              ((REMAP) == GPIO_Remap1_CAN) || ((REMAP) == GPIO_Remap2_CAN) || \
                              ((REMAP) == GPIO_Remap_PD01) || ((REMAP) == GPIO_Remap_SPI2))

/**
 * @brief GPIO_Port_Sources
 */
#define GPIO_PortSourceGPIOA ((uint8_t)0x00)
#define GPIO_PortSourceGPIOB ((uint8_t)0x01)
#define GPIO_PortSourceGPIOC ((uint8_t)0x02)
#define GPIO_PortSourceGPIOD ((uint8_t)0x03)
#define GPIO_PortSourceGPIOE ((uint8_t)0x04)

#define IS_GPIO_EVENTOUT_PORT_SOURCE(PORTSOURCE) (((PORTSOURCE) == GPIO_PortSourceGPIOA) || \
                                                  ((PORTSOURCE) == GPIO_PortSourceGPIOB) || \
                                                  ((PORTSOURCE) == GPIO_PortSourceGPIOC) || \
                                                  ((PORTSOURCE) == GPIO_PortSourceGPIOD) || \
                                                  ((PORTSOURCE) == GPIO_PortSourceGPIOE))

#define IS_GPIO_EXTI_PORT_SOURCE(PORTSOURCE) (((PORTSOURCE) == GPIO_PortSourceGPIOA) || \
                                              ((PORTSOURCE) == GPIO_PortSourceGPIOB) || \
                                              ((PORTSOURCE) == GPIO_PortSourceGPIOC) || \
                                              ((PORTSOURCE) == GPIO_PortSourceGPIOD) || \
                                              ((PORTSOURCE) == GPIO_PortSourceGPIOE))

/**
 * @brief GPIO_Pin_sources
 */
#define GPIO_PinSource0  ((uint8_t)0x00)
#define GPIO_PinSource1  ((uint8_t)0x01)
#define GPIO_PinSource2  ((uint8_t)0x02)
#define GPIO_PinSource3  ((uint8_t)0x03)
#define GPIO_PinSource4  ((uint8_t)0x04)
#define GPIO_PinSource5  ((uint8_t)0x05)
#define GPIO_PinSource6  ((uint8_t)0x06)
#define GPIO_PinSource7  ((uint8_t)0x07)
#define GPIO_PinSource8  ((uint8_t)0x08)
#define GPIO_PinSource9  ((uint8_t)0x09)
#define GPIO_PinSource10 ((uint8_t)0x0A)
#define GPIO_PinSource11 ((uint8_t)0x0B)
#define GPIO_PinSource12 ((uint8_t)0x0C)
#define GPIO_PinSource13 ((uint8_t)0x0D)
#define GPIO_PinSource14 ((uint8_t)0x0E)
#define GPIO_PinSource15 ((uint8_t)0x0F)

#define IS_GPIO_PIN_SOURCE(PINSOURCE) (((PINSOURCE) == GPIO_PinSource0) || \
                                       ((PINSOURCE) == GPIO_PinSource1) || \
                                       ((PINSOURCE) == GPIO_PinSource2) || \
                                       ((PINSOURCE) == GPIO_PinSource3) || \
                                       ((PINSOURCE) == GPIO_PinSource4) || \
                                       ((PINSOURCE) == GPIO_PinSource5) || \
                                       ((PINSOURCE) == GPIO_PinSource6) || \
                                       ((PINSOURCE) == GPIO_PinSource7) || \
                                       ((PINSOURCE) == GPIO_PinSource8) || \
                                       ((PINSOURCE) == GPIO_PinSource9) || \
                                       ((PINSOURCE) == GPIO_PinSource10) || \
                                       ((PINSOURCE) == GPIO_PinSource11) || \
                                       ((PINSOURCE) == GPIO_PinSource12) || \
                                       ((PINSOURCE) == GPIO_PinSource13) || \
                                       ((PINSOURCE) == GPIO_PinSource14) || \
                                       ((PINSOURCE) == GPIO_PinSource15))

/**
 * @brief Deinitializes the GPIOx peripheral registers to their default reset values.
 * @param[in] GPIOx where x can be (A..E) to select the GPIO peripheral.
 */
void GPIO_DeInit(GPIO_TypeDef *GPIOx);

/**
 * @brief Deinitializes the Alternate Functions (remap, event control and EXTI configuration)
 *        registers to their default reset values.
 */
void GPIO_AFIODeInit(void);

/**
 * @brief  Initializes the GPIOx peripheral according to the specified parameters in the GPIO_InitStruct.
 * @param[in] GPIOx: where x can be (A..E) to select the GPIO peripheral.
 * @param[in] GPIO_InitStruct: pointer to a GPIO_InitTypeDef structure that contains the configuration
 *            information for the specified GPIO peripheral.
 */
void GPIO_Init(GPIO_TypeDef *GPIOx, GPIO_InitTypeDef *GPIO_InitStruct);

/**
 * @brief Fills each GPIO_InitStruct member with its default value.
 * @param[in] GPIO_InitStruct pointer to a GPIO_InitTypeDef structure which will be initialized.
 */
void GPIO_StructInit(GPIO_InitTypeDef *GPIO_InitStruct);

/**
 * @brief Reads the specified input port pin.
 * @param[in] GPIOx where x can be (A..E) to select the GPIO peripheral.
 * @param[in] GPIO_Pin pecifies the port bits to be written.
 *            This parameter can be any combination of GPIO_Pin_x where x can be (0..15).
 * @return uint8_t The input port pin value.
 */
uint8_t GPIO_ReadInputDataBit(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);

/**
 * @brief Reads the specified GPIO input data port.
 * @param[in] GPIOx where x can be (A..E) to select the GPIO peripheral.
 * @return uint16_t GPIO input data port value.
 */
uint16_t GPIO_ReadInputData(GPIO_TypeDef *GPIOx);

/**
 * @brief Reads the specified output data port bit.
 * @param[in] GPIOx where x can be (A..E) to select the GPIO peripheral.
 * @param[in] GPIO_Pin pecifies the port bits to be written.
 *            This parameter can be any combination of GPIO_Pin_x where x can be (0..15).
 * @return uint8_t The output port pin value.
 */
uint8_t GPIO_ReadOutputDataBit(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);

/**
 * @brief Reads the specified GPIO output data port.
 * @param[in] GPIOx where x can be (A..E) to select the GPIO peripheral.
 * @return uint16_t GPIO output data port value.
 */
uint16_t GPIO_ReadOutputData(GPIO_TypeDef *GPIOx);

/**
 * @brief Sets the selected data port bits.
 * @param[in] GPIOx where x can be (A..E) to select the GPIO peripheral.
 * @param[in] GPIO_Pin pecifies the port bits to be written.
 *            This parameter can be any combination of GPIO_Pin_x where x can be (0..15).
 */
void GPIO_SetBits(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);

/**
 * @brief Clears the selected data port bits.
 * @param[in] GPIOx where x can be (A..E) to select the GPIO peripheral.
 * @param[in] GPIO_Pin pecifies the port bits to be written.
 *            This parameter can be any combination of GPIO_Pin_x where x can be (0..15).
 */
void GPIO_ResetBits(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);

/**
 * @brief Sets or clears the selected data port bit.
 * @param[in] GPIOx where x can be (A..E) to select the GPIO peripheral.
 * @param[in] GPIO_Pin pecifies the port bits to be written.
 *            This parameter can be any combination of GPIO_Pin_x where x can be (0..15).
 * @param[in] BitVal specifies the value to be written to the selected bit.
 */
void GPIO_WriteBit(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin, BitAction BitVal);

/**
 * @brief Writes data to the specified GPIO data port.
 * @param[in] GPIOx where x can be (A..E) to select the GPIO peripheral.
 * @param[in] PortVal specifies the value to be written to the port output data register.
 */
void GPIO_Write(GPIO_TypeDef *GPIOx, uint16_t PortVal);

/**
 * @brief Changes the mapping of the specified pin.
 * @param[in] GPIO_Remap selects the pin to remap.
 * @param[in] NewState new state of the port pin remapping.
 *            This parameter can be: ENABLE or DISABLE.
 */
void GPIO_PinRemapConfig(uint32_t GPIO_Remap, FunctionalState NewState);

/**
 * @brief Selects the GPIO pin used as EXTI Line.
 * @param[in] GPIO_PortSource selects the GPIO port to be used as source for EXTI lines.
 *            This parameter can be GPIO_PortSourceGPIOx where x can be (A..E).
 * @param[in] GPIO_PinSource specifies the EXTI line to be configured.
 *            This parameter can be GPIO_PinSourcex where x can be (0..15).
 */
void GPIO_EXTILineConfig(uint8_t GPIO_PortSource, uint8_t GPIO_PinSource);

/**
 * @brief Enable debounce for the specified output data port bit.
 * @param[in] GPIOx where x can be (A..E) to select the GPIO peripheral.
 * @param[in] GPIO_Pin pecifies the port bits to be written.
 *            This parameter can be any combination of GPIO_Pin_x where x can be (0..15).
 */
void GPIO_Debounce_Enable(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);

/**
 * @brief Disable debounce for the specified output data port bit.
 * @param[in] GPIOx where x can be (A..E) to select the GPIO peripheral.
 * @param[in] GPIO_Pin pecifies the port bits to be written.
 *            This parameter can be any combination of GPIO_Pin_x where x can be (0..15).
 */
void GPIO_Debounce_Disable(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);

#ifdef __cplusplus
}
#endif

#endif