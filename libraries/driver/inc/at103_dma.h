/**
 * @file at103_dma.h
 * @brief This file contains all the functions prototypes for the DMA firmware library. \n
 *        How to use this driver? \n
 * (+) At first, use ''DMA_DeInit(...)'' to reset requires DMA channel. \n
 * (+) Then, use ''DMA_Init(...)'' with DMA initialization structure to initialize a DMA channel. \n
 *     For details on the DMA initialization structure, see @ref DMA_InitTypeDef. \n
 * (+) Use ''DMA_ITConfig(...)'' to enable interrupts that require DMA channel, and you need to implement the corresponding channel service function: DMA_Channelx_IRQHandler. \n
 *     For details on the DMA interrupt type, see @ref DMA_interrupts_definition. \n
 * (+) Finally, use ''DMA_Cmd(...)'' to start a DMA channel transfer. \n
 * (+) Of course, you can use ''DMA_GetFlagStatus(...)'' to get the state of the DMA channel at any time, this is helpful in determining whether the DMA channel has completed its transfer. \n
 * (+) Note: \n
 *     DMA channels can transfer up to 4095 units at a time. \n
 * @author Dong Qin (dong.qin@timesintelli.com)
 * @version 1.0
 * @date 2022-03-16
 * @copyright Copyright (c) 2022 Timesintelli, Inc
 */
#ifndef __AT103_DMA_H
#define __AT103_DMA_H

#ifdef __cplusplus
extern "C" {
#endif

#include "at103.h"

/**
 * @brief DMA Init structure definition.
 */
typedef struct
{
    uint32_t DMA_PeripheralBaseAddr;  /**< Specifies the peripheral base address for DMA Channelx. */
    uint32_t DMA_MemoryBaseAddr;      /**< Specifies the memory base address for DMA Channelx. */
    uint32_t DMA_DIR;                 /**< Specifies if the peripheral is the source or destination.
                                           This parameter can be a value of @ref DMA_data_transfer_direction */
    uint32_t DMA_BufferSize;          /**< Specifies the buffer size, in data unit, of the specified Channel.
                                           The data unit is equal to the configuration set in DMA_PeripheralDataSize
                                           or DMA_MemoryDataSize members depending in the transfer direction. */
    uint32_t DMA_PeripheralInc;       /**< Specifies whether the Peripheral address register is incremented or not.
                                           This parameter can be a value of @ref DMA_peripheral_incremented_mode */
    uint32_t DMA_MemoryInc;           /**< Specifies whether the memory address register is incremented or not.
                                           This parameter can be a value of @ref DMA_memory_incremented_mode */
    uint32_t DMA_PeripheralDataSize;  /**< Specifies the Peripheral data width.
                                           This parameter can be a value of @ref DMA_peripheral_data_size */
    uint32_t DMA_MemoryDataSize;      /**< Specifies the Memory data width.
                                           This parameter can be a value of @ref DMA_memory_data_size */
    uint32_t DMA_Mode;                /**< Specifies the operation mode of the DMA Channelx.
                                           This parameter can be a value of @ref DMA_circular_normal_mode. */
    uint32_t DMA_Priority;            /**< Specifies the software priority for the DMA Channelx.
                                           This parameter can be a value of @ref DMA_priority_level */
    uint32_t DMA_M2M;                 /**< Specifies if the DMA Channelx will be used in memory-to-memory transfer.
                                           This parameter can be a value of @ref DMA_memory_to_memory */
    uint32_t DMA_PeripheralHandshake; /**< Specifies the hardware handshake between the peripheral and DMA Channelx.
                                           This parameter can be a value of @ref DMA_peripherals_handshake_definition */
} DMA_InitTypeDef;

#define IS_DMA_ALL_PERIPH(PERIPH) (((PERIPH) == DMA_Channel1) || \
                                   ((PERIPH) == DMA_Channel2) || \
                                   ((PERIPH) == DMA_Channel3) || \
                                   ((PERIPH) == DMA_Channel4) || \
                                   ((PERIPH) == DMA_Channel5) || \
                                   ((PERIPH) == DMA_Channel6) || \
                                   ((PERIPH) == DMA_Channel7) || \
                                   ((PERIPH) == DMA_Channel8))

/**
 * @brief DMA data transfer direction
 */
#define DMA_DIR_PeripheralDST ((uint32_t)0x00000001)
#define DMA_DIR_PeripheralSRC ((uint32_t)0x00000002)
#define IS_DMA_DIR(DIR)       (((DIR) == DMA_DIR_PeripheralDST) || \
                         ((DIR) == DMA_DIR_PeripheralSRC))

/**
 * @brief DMA peripheral incremented mode
 */
#define DMA_PeripheralInc_Enable           ((uint32_t)0x00000000)
#define DMA_PeripheralInc_Disable          ((uint32_t)0x00000002)
#define IS_DMA_PERIPHERAL_INC_STATE(STATE) (((STATE) == DMA_PeripheralInc_Enable) || \
                                            ((STATE) == DMA_PeripheralInc_Disable))

/**
 * @brief DMA memory incremented mode
 */
#define DMA_MemoryInc_Enable           ((uint32_t)0x00000000)
#define DMA_MemoryInc_Disable          ((uint32_t)0x00000002)
#define IS_DMA_MEMORY_INC_STATE(STATE) (((STATE) == DMA_MemoryInc_Enable) || \
                                        ((STATE) == DMA_MemoryInc_Disable))

/**
 * @brief DMA peripheral data size
 */
#define DMA_PeripheralDataSize_Byte       ((uint32_t)0x00000000)
#define DMA_PeripheralDataSize_HalfWord   ((uint32_t)0x00000001)
#define DMA_PeripheralDataSize_Word       ((uint32_t)0x00000002)
#define IS_DMA_PERIPHERAL_DATA_SIZE(SIZE) (((SIZE) == DMA_PeripheralDataSize_Byte) || \
                                           ((SIZE) == DMA_PeripheralDataSize_HalfWord) || \
                                           ((SIZE) == DMA_PeripheralDataSize_Word))

/**
 * @brief DMA memory data size
 */
#define DMA_MemoryDataSize_Byte       ((uint32_t)0x00000000)
#define DMA_MemoryDataSize_HalfWord   ((uint32_t)0x00000001)
#define DMA_MemoryDataSize_Word       ((uint32_t)0x00000002)
#define IS_DMA_MEMORY_DATA_SIZE(SIZE) (((SIZE) == DMA_MemoryDataSize_Byte) || \
                                       ((SIZE) == DMA_MemoryDataSize_HalfWord) || \
                                       ((SIZE) == DMA_MemoryDataSize_Word))

/**
 * @brief DMA circular normal mode
 */
#define DMA_Mode_Circular ((uint32_t)0x00000003)
#define DMA_Mode_Normal   ((uint32_t)0x00000000)
#define IS_DMA_MODE(MODE) (((MODE) == DMA_Mode_Circular) || ((MODE) == DMA_Mode_Normal))

/**
 * @brief DMA priority level
 */
#define DMA_Priority_VeryHigh     ((uint32_t)0x00000003)
#define DMA_Priority_High         ((uint32_t)0x00000002)
#define DMA_Priority_Medium       ((uint32_t)0x00000001)
#define DMA_Priority_Low          ((uint32_t)0x00000000)
#define IS_DMA_PRIORITY(PRIORITY) (((PRIORITY) == DMA_Priority_VeryHigh) || \
                                   ((PRIORITY) == DMA_Priority_High) || \
                                   ((PRIORITY) == DMA_Priority_Medium) || \
                                   ((PRIORITY) == DMA_Priority_Low))

/**
 * @brief DMA memory to memory
 */
#define DMA_M2M_Enable          ((uint32_t)0x00000001)
#define DMA_M2M_Disable         ((uint32_t)0x00000002)
#define IS_DMA_M2M_STATE(STATE) (((STATE) == DMA_M2M_Enable) || ((STATE) == DMA_M2M_Disable))

/**
 * @brief DMA interrupts definition
 */
#define DMA_IT_TC            ((uint32_t)0x00000001) /**< DMA  transfer complete interrupt */
#define DMA_IT_TE            ((uint32_t)0x00000002) /**< DMA  transfer error interrupt */
#define IS_DMA_CONFIG_IT(IT) ((((IT)&0xFFFFFFFC) == 0x00) && ((IT) != 0x00))

#define DMA_IT_GL1 ((uint32_t)0x00000101) /**< DMA Channel1 global interrupt */
#define DMA_IT_TC1 ((uint32_t)0x00000001) /**< DMA Channel1 transfer complete interrupt */
#define DMA_IT_TE1 ((uint32_t)0x00000100) /**< DMA Channel1 transfer error interrupt */
#define DMA_IT_GL2 ((uint32_t)0x00000202) /**< DMA Channel2 global interrupt */
#define DMA_IT_TC2 ((uint32_t)0x00000002) /**< DMA Channel2 transfer complete interrupt */
#define DMA_IT_TE2 ((uint32_t)0x00000200) /**< DMA Channel2 transfer error interrupt */
#define DMA_IT_GL3 ((uint32_t)0x00000404) /**< DMA Channel3 global interrupt */
#define DMA_IT_TC3 ((uint32_t)0x00000004) /**< DMA Channel3 transfer complete interrupt */
#define DMA_IT_TE3 ((uint32_t)0x00000400) /**< DMA Channel3 transfer error interrupt */
#define DMA_IT_GL4 ((uint32_t)0x00000808) /**< DMA Channel4 global interrupt */
#define DMA_IT_TC4 ((uint32_t)0x00000008) /**< DMA Channel4 transfer complete interrupt */
#define DMA_IT_TE4 ((uint32_t)0x00000800) /**< DMA Channel4 transfer error interrupt */
#define DMA_IT_GL5 ((uint32_t)0x00001010) /**< DMA Channel5 global interrupt */
#define DMA_IT_TC5 ((uint32_t)0x00000010) /**< DMA Channel5 transfer complete interrupt */
#define DMA_IT_TE5 ((uint32_t)0x00001000) /**< DMA Channel5 transfer error interrupt */
#define DMA_IT_GL6 ((uint32_t)0x00002020) /**< DMA Channel6 global interrupt */
#define DMA_IT_TC6 ((uint32_t)0x00000020) /**< DMA Channel6 transfer complete interrupt */
#define DMA_IT_TE6 ((uint32_t)0x00002000) /**< DMA Channel6 transfer error interrupt */
#define DMA_IT_GL7 ((uint32_t)0x00004040) /**< DMA Channel7 global interrupt */
#define DMA_IT_TC7 ((uint32_t)0x00000040) /**< DMA Channel7 transfer complete interrupt */
#define DMA_IT_TE7 ((uint32_t)0x00004000) /**< DMA Channel7 transfer error interrupt */
#define DMA_IT_GL8 ((uint32_t)0x00008080) /**< DMA Channel8 global interrupt */
#define DMA_IT_TC8 ((uint32_t)0x00000080) /**< DMA Channel8 transfer complete interrupt */
#define DMA_IT_TE8 ((uint32_t)0x00008000) /**< DMA Channel8 transfer error interrupt */

#define IS_DMA_CLEAR_IT(IT) ((((IT)&0xFFFF0000) == 0x00) && ((IT) != 0x00))

#define IS_DMA_GET_IT(IT) (((IT) == DMA_IT_GL1) || ((IT) == DMA_IT_TC1) || ((IT) == DMA_IT_TE1) || \
                           ((IT) == DMA_IT_GL2) || ((IT) == DMA_IT_TC2) || ((IT) == DMA_IT_TE2) || \
                           ((IT) == DMA_IT_GL3) || ((IT) == DMA_IT_TC3) || ((IT) == DMA_IT_TE3) || \
                           ((IT) == DMA_IT_GL4) || ((IT) == DMA_IT_TC4) || ((IT) == DMA_IT_TE4) || \
                           ((IT) == DMA_IT_GL5) || ((IT) == DMA_IT_TC5) || ((IT) == DMA_IT_TE5) || \
                           ((IT) == DMA_IT_GL6) || ((IT) == DMA_IT_TC6) || ((IT) == DMA_IT_TE6) || \
                           ((IT) == DMA_IT_GL7) || ((IT) == DMA_IT_TC7) || ((IT) == DMA_IT_TE7) || \
                           ((IT) == DMA_IT_GL8) || ((IT) == DMA_IT_TC8) || ((IT) == DMA_IT_TE8))

/**
 * @brief DMA flags definition
 */
#define DMA_FLAG_GL1 ((uint32_t)0x00000101) /**< DMA Channel1 global flag */
#define DMA_FLAG_TC1 ((uint32_t)0x00000001) /**< DMA Channel1 transfer complete flag */
#define DMA_FLAG_TE1 ((uint32_t)0x00000100) /**< DMA Channel1 transfer error flag */
#define DMA_FLAG_GL2 ((uint32_t)0x00000202) /**< DMA Channel2 global flag */
#define DMA_FLAG_TC2 ((uint32_t)0x00000002) /**< DMA Channel2 transfer complete flag */
#define DMA_FLAG_TE2 ((uint32_t)0x00000200) /**< DMA Channel2 transfer error flag */
#define DMA_FLAG_GL3 ((uint32_t)0x00000404) /**< DMA Channel3 global flag */
#define DMA_FLAG_TC3 ((uint32_t)0x00000004) /**< DMA Channel3 transfer complete flag */
#define DMA_FLAG_TE3 ((uint32_t)0x00000400) /**< DMA Channel3 transfer error flag */
#define DMA_FLAG_GL4 ((uint32_t)0x00000808) /**< DMA Channel4 global flag */
#define DMA_FLAG_TC4 ((uint32_t)0x00000008) /**< DMA Channel4 transfer complete flag */
#define DMA_FLAG_TE4 ((uint32_t)0x00000800) /**< DMA Channel4 transfer error flag */
#define DMA_FLAG_GL5 ((uint32_t)0x00001010) /**< DMA Channel5 global flag */
#define DMA_FLAG_TC5 ((uint32_t)0x00000010) /**< DMA Channel5 transfer complete flag */
#define DMA_FLAG_TE5 ((uint32_t)0x00001000) /**< DMA Channel5 transfer error flag */
#define DMA_FLAG_GL6 ((uint32_t)0x00002020) /**< DMA Channel6 global flag */
#define DMA_FLAG_TC6 ((uint32_t)0x00000020) /**< DMA Channel6 transfer complete flag */
#define DMA_FLAG_TE6 ((uint32_t)0x00002000) /**< DMA Channel6 transfer error flag */
#define DMA_FLAG_GL7 ((uint32_t)0x00004040) /**< DMA Channel7 global flag */
#define DMA_FLAG_TC7 ((uint32_t)0x00000040) /**< DMA Channel7 transfer complete flag */
#define DMA_FLAG_TE7 ((uint32_t)0x00004000) /**< DMA Channel7 transfer error flag */
#define DMA_FLAG_GL8 ((uint32_t)0x00008080) /**< DMA Channel8 global flag */
#define DMA_FLAG_TC8 ((uint32_t)0x00000080) /**< DMA Channel8 transfer complete flag */
#define DMA_FLAG_TE8 ((uint32_t)0x00008000) /**< DMA Channel8 transfer error flag */

#define IS_DMA_CLEAR_FLAG(FLAG) ((((FLAG)&0xFFFF0000) == 0x00) && ((FLAG) != 0x00))

#define IS_DMA_GET_FLAG(FLAG) (((FLAG) == DMA_FLAG_GL1) || ((FLAG) == DMA_FLAG_TC1) || ((FLAG) == DMA_FLAG_TE1) || \
                               ((FLAG) == DMA_FLAG_GL2) || ((FLAG) == DMA_FLAG_TC2) || ((FLAG) == DMA_FLAG_TE2) || \
                               ((FLAG) == DMA_FLAG_GL3) || ((FLAG) == DMA_FLAG_TC3) || ((FLAG) == DMA_FLAG_TE3) || \
                               ((FLAG) == DMA_FLAG_GL4) || ((FLAG) == DMA_FLAG_TC4) || ((FLAG) == DMA_FLAG_TE4) || \
                               ((FLAG) == DMA_FLAG_GL5) || ((FLAG) == DMA_FLAG_TC5) || ((FLAG) == DMA_FLAG_TE5) || \
                               ((FLAG) == DMA_FLAG_GL6) || ((FLAG) == DMA_FLAG_TC6) || ((FLAG) == DMA_FLAG_TE6) || \
                               ((FLAG) == DMA_FLAG_GL7) || ((FLAG) == DMA_FLAG_TC7) || ((FLAG) == DMA_FLAG_TE7) || \
                               ((FLAG) == DMA_FLAG_GL8) || ((FLAG) == DMA_FLAG_TC8) || ((FLAG) == DMA_FLAG_TE8))

/**
 * @brief DMA peripherals handshake definition
 *        [7 :0] handshake value;
 *        [10:8] burst size value;
 */
#define DMA_PeripheralHandshake_ADC1      ((uint32_t)0x00000000)
#define DMA_PeripheralHandshake_SPI1_TX   ((uint32_t)0x00000002)
#define DMA_PeripheralHandshake_SPI1_RX   ((uint32_t)0x00000001)
#define DMA_PeripheralHandshake_SPI2_TX   ((uint32_t)0x00000004)
#define DMA_PeripheralHandshake_SPI2_RX   ((uint32_t)0x00000003)
#define DMA_PeripheralHandshake_USART1_TX ((uint32_t)0x0000000A)
#define DMA_PeripheralHandshake_USART1_RX ((uint32_t)0x0000000B)
#define DMA_PeripheralHandshake_USART2_TX ((uint32_t)0x00000008)
#define DMA_PeripheralHandshake_USART2_RX ((uint32_t)0x00000009)
#define DMA_PeripheralHandshake_USART3_TX ((uint32_t)0x00000006)
#define DMA_PeripheralHandshake_USART3_RX ((uint32_t)0x00000007)
#define DMA_PeripheralHandshake_I2C1_TX   ((uint32_t)0x0000000C)
#define DMA_PeripheralHandshake_I2C1_RX   ((uint32_t)0x0000000D)
#define DMA_PeripheralHandshake_I2C2_TX   ((uint32_t)0x0000000E)
#define DMA_PeripheralHandshake_I2C2_RX   ((uint32_t)0x0000000F)
#define DMA_PeripheralHandshake_TIM1_UP   ((uint32_t)0x00000014)
#define DMA_PeripheralHandshake_TIM1_TRIG ((uint32_t)0x0000001A)
#define DMA_PeripheralHandshake_TIM1_COM  ((uint32_t)0x00000023)
#define DMA_PeripheralHandshake_TIM1_CH1  ((uint32_t)0x00000011)
#define DMA_PeripheralHandshake_TIM1_CH2  ((uint32_t)0x00000012)
#define DMA_PeripheralHandshake_TIM1_CH3  ((uint32_t)0x00000015)
#define DMA_PeripheralHandshake_TIM1_CH4  ((uint32_t)0x00000013)
#define DMA_PeripheralHandshake_TIM2_UP   ((uint32_t)0x00000018)
#define DMA_PeripheralHandshake_TIM2_CH1  ((uint32_t)0x0000001B)
#define DMA_PeripheralHandshake_TIM2_CH2  ((uint32_t)0x00000016)
#define DMA_PeripheralHandshake_TIM2_CH3  ((uint32_t)0x00000010)
#define DMA_PeripheralHandshake_TIM2_CH4  ((uint32_t)0x0000001D)
#define DMA_PeripheralHandshake_TIM3_UP   ((uint32_t)0x00000022)
#define DMA_PeripheralHandshake_TIM3_TRIG ((uint32_t)0x00000025)
#define DMA_PeripheralHandshake_TIM3_CH1  ((uint32_t)0x0000001C)
#define DMA_PeripheralHandshake_TIM3_CH3  ((uint32_t)0x00000021)
#define DMA_PeripheralHandshake_TIM3_CH4  ((uint32_t)0x00000019)
#define DMA_PeripheralHandshake_TIM4_UP   ((uint32_t)0x00000026)
#define DMA_PeripheralHandshake_TIM4_CH1  ((uint32_t)0x00000017)
#define DMA_PeripheralHandshake_TIM4_CH2  ((uint32_t)0x00000027)
#define DMA_PeripheralHandshake_TIM4_CH3  ((uint32_t)0x00000024)

#define IS_DMA_PERIPHERAL_HANDSHAKE(HANDSHAKE) (((HANDSHAKE) == DMA_PeripheralHandshake_ADC1) || \
                                                ((HANDSHAKE) == DMA_PeripheralHandshake_SPI1_TX) || ((HANDSHAKE) == DMA_PeripheralHandshake_SPI1_RX) || \
                                                ((HANDSHAKE) == DMA_PeripheralHandshake_SPI2_TX) || ((HANDSHAKE) == DMA_PeripheralHandshake_SPI2_RX) || \
                                                ((HANDSHAKE) == DMA_PeripheralHandshake_USART1_TX) || ((HANDSHAKE) == DMA_PeripheralHandshake_USART1_RX) || \
                                                ((HANDSHAKE) == DMA_PeripheralHandshake_USART2_TX) || ((HANDSHAKE) == DMA_PeripheralHandshake_USART2_RX) || \
                                                ((HANDSHAKE) == DMA_PeripheralHandshake_USART3_TX) || ((HANDSHAKE) == DMA_PeripheralHandshake_USART3_RX) || \
                                                ((HANDSHAKE) == DMA_PeripheralHandshake_I2C1_TX) || ((HANDSHAKE) == DMA_PeripheralHandshake_I2C1_RX) || \
                                                ((HANDSHAKE) == DMA_PeripheralHandshake_I2C2_TX) || ((HANDSHAKE) == DMA_PeripheralHandshake_I2C2_RX) || \
                                                ((HANDSHAKE) == DMA_PeripheralHandshake_TIM1_CH1) || ((HANDSHAKE) == DMA_PeripheralHandshake_TIM1_UP) || \
                                                ((HANDSHAKE) == DMA_PeripheralHandshake_TIM1_CH2) || ((HANDSHAKE) == DMA_PeripheralHandshake_TIM1_TRIG) || \
                                                ((HANDSHAKE) == DMA_PeripheralHandshake_TIM1_CH3) || ((HANDSHAKE) == DMA_PeripheralHandshake_TIM1_COM) || \
                                                ((HANDSHAKE) == DMA_PeripheralHandshake_TIM1_CH4) || ((HANDSHAKE) == DMA_PeripheralHandshake_TIM2_UP) || \
                                                ((HANDSHAKE) == DMA_PeripheralHandshake_TIM2_CH1) || ((HANDSHAKE) == DMA_PeripheralHandshake_TIM2_CH2) || \
                                                ((HANDSHAKE) == DMA_PeripheralHandshake_TIM2_CH3) || ((HANDSHAKE) == DMA_PeripheralHandshake_TIM2_CH4) || \
                                                ((HANDSHAKE) == DMA_PeripheralHandshake_TIM3_UP) || ((HANDSHAKE) == DMA_PeripheralHandshake_TIM3_TRIG) || \
                                                ((HANDSHAKE) == DMA_PeripheralHandshake_TIM3_CH1) || ((HANDSHAKE) == DMA_PeripheralHandshake_TIM3_CH3) || \
                                                ((HANDSHAKE) == DMA_PeripheralHandshake_TIM3_CH4) || ((HANDSHAKE) == DMA_PeripheralHandshake_TIM4_UP) || \
                                                ((HANDSHAKE) == DMA_PeripheralHandshake_TIM4_CH1) || ((HANDSHAKE) == DMA_PeripheralHandshake_TIM4_CH2) || \
                                                ((HANDSHAKE) == DMA_PeripheralHandshake_TIM4_CH3))

/**
 * @brief DMA buffer size
 */
#define IS_DMA_BUFFER_SIZE(SIZE) (((SIZE) >= 0x1) && ((SIZE) < 0x1000))

/**
 * @brief Deinitializes the DMA Channelx registers to their default reset values.
 * @param[in] DMA_Channelx Where x can be 1 to 8 to select the DMA channel.
 */
void DMA_DeInit(DMA_Channel_TypeDef *DMA_Channelx);

/**
 * @brief Initializes the DMA Channelx according to the specified parameters in the DMA_InitStruct.
 * @param[in] DMA_Channelx Where x can be 1 to 8 to select the DMA channel.
 * @param[in] DMA_InitStruct pointer to a DMA_InitTypeDef structure that contains
 *            the configuration information for the specified DMA Channel.
 */
void DMA_Init(DMA_Channel_TypeDef *DMA_Channelx, DMA_InitTypeDef *DMA_InitStruct);

/**
 * @brief Fills each DMA_InitStruct member with its default value.
 * @param[in] DMA_InitStruct pointer to a DMA_InitTypeDef structure which will be initialized.
 */
void DMA_StructInit(DMA_InitTypeDef *DMA_InitStruct);

/**
 * @brief Enables or disables the specified DMA Channelx.
 * @param[in] DMA_Channelx Where x can be 1 to 8 to select the DMA channel.
 * @param[in] NewState new state of the DMA Channelx.
 *            This parameter can be: ENABLE or DISABLE.
 */
void DMA_Cmd(DMA_Channel_TypeDef *DMA_Channelx, FunctionalState NewState);

/**
 * @brief Enables or disables the specified DMA Channelx interrupts.
 * @param[in] DMA_Channelx Where x can be 1 to 8 to select the DMA channel.
 * @param[in] DMA_IT specifies the DMA interrupts sources to be enabled or disabled.
 *            This parameter can be any combination of the following values:
 *              DMA_IT_TC:  Transfer complete interrupt mask
 *              DMA_IT_TE:  Transfer error interrupt mask
 * @param[in] NewState new state of the specified DMA interrupts.
 *            This parameter can be: ENABLE or DISABLE.
 */
void DMA_ITConfig(DMA_Channel_TypeDef *DMA_Channelx, uint32_t DMA_IT, FunctionalState NewState);

/**
 * @brief Sets the number of data units in the current DMA Channelx transfer.
 *        This function can only be used when the DMA_Channelx is disabled.
 * @param[in] DMA_Channelx Where x can be 1 to 8 to select the DMA channel.
 * @param[in] DataNumber The number of data units in the current DMA Channelx transfer.
 */
void DMA_SetCurrDataCounter(DMA_Channel_TypeDef *DMA_Channelx, uint16_t DataNumber);

/**
 * @brief After the transfer has started, returns the number of completing data units in the current DMA Channelx transfer.
 *        Before the transmission starts or after the transmission ends, the return values are the values set before the transfer starts.
 * @param[in] DMA_Channelx Where x can be 1 to 8 to select the DMA channel.
 * @return uint16_t The number of data units.
 */
uint16_t DMA_GetCurrDataCounter(DMA_Channel_TypeDef *DMA_Channelx);

/**
 * @brief Checks whether the specified DMA Channelx flag is set or not.
 * @param[in] DMA_FLAG specifies the flag to check.
 * @return FlagStatus The new state of DMA_FLAG (SET or RESET).
 */
FlagStatus DMA_GetFlagStatus(uint32_t DMA_FLAG);

/**
 * @brief Clears the DMA Channelx's pending flags.
 * @param[in] DMA_FLAG specifies the flag to clear.
 */
void DMA_ClearFlag(uint32_t DMA_FLAG);

/**
 * @brief Checks whether the specified DMA Channelx interrupt has occurred or not.
 * @param[in] DMA_IT specifies the DMA interrupt source to check.
 * @return ITStatus The new state of DMA_IT (SET or RESET).
 */
ITStatus DMA_GetITStatus(uint32_t DMA_IT);

/**
 * @brief Clears the DMA Channelx's interrupt pending bits.
 * @param[in] DMA_IT specifies the DMA interrupt source to check.
 */
void DMA_ClearITPendingBit(uint32_t DMA_IT);

#ifdef __cplusplus
}
#endif

#endif