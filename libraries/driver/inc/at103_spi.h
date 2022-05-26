/**
 * @file      at103_spi.h
 * @brief     This file contains all the functions prototypes for the SPI firmware
 *           library.
 * @author    luoming (luoming@i-core.cn)
 * @version   1.0
 * @date      2022-03-30
 * @copyright Copyright (c) 2022 i-core, Inc
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __AT103_SPI_H
#define __AT103_SPI_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "at103.h"

/**
 * @brief SPI Init Structure definition
 */
typedef struct
{
    uint16_t SPI_Direction; /**< Specifies the SPI unidirectional or bidirectional data mode. */

    uint16_t SPI_Mode; /**< Specifies the SPI operating mode. */

    uint16_t SPI_DataSize; /**< Specifies the SPI data size.  */

    uint16_t SPI_CPOL; /**< Specifies the serial clock steady state. */

    uint16_t SPI_CPHA; /**< Specifies the clock active edge for the bit capture.  */

    uint16_t SPI_NSS; /**< Specifies whether the NSS signal is managed by
                            hardware (NSS pin) or by software using the SSI bit.*/

    uint16_t SPI_BaudRatePrescaler; /**< Specifies the Baud Rate prescaler value which will be
                                          used to configure the transmit and receive SCK clock.
                                          @note The communication clock is derived from the master
                                                clock. The slave clock does not need to be set. */

    uint16_t SPI_FirstBit; /** Specifies whether data transfers start from MSB or LSB bit.*/

    uint16_t SPI_CRCPolynomial; /**< Specifies the polynomial used for the CRC calculation. */
} SPI_InitTypeDef;

/**
 * @brief SPI_Exported_Types
 */
#define IS_SPI_ALL_PERIPH(PERIPH) (((PERIPH) == SPI1) || \
                                   ((PERIPH) == SPI2))

/**
 * @brief SPI_Direction_MODE
 */
#define SPI_Direction_2Lines_FullDuplex ((uint16_t)0x0018)
#define SPI_Direction_2Lines_RxOnly     ((uint16_t)0x0010)
#define SPI_Direction_1Line_Rx          ((uint16_t)0x0010)
#define SPI_Direction_1Line_Tx          ((uint16_t)0x0008)
#define IS_SPI_DIRECTION_MODE(MODE)     (((MODE) == SPI_Direction_2Lines_FullDuplex) || \
                                     ((MODE) == SPI_Direction_2Lines_RxOnly) || \
                                     (SPI_Direction_1Line_Rx) || \
                                     (SPI_Direction_1Line_Tx))
/**
 * @brief SPI_Direction
 */
#define SPI_Direction_MASK          ((uint32_t)0xffffffe7)
#define SPI_Direction_Rx            ((uint16_t)0x0010)
#define SPI_Direction_Tx            ((uint16_t)0x0008)
#define IS_SPI_DIRECTION(DIRECTION) (((DIRECTION) == SPI_Direction_Rx) || \
                                     ((DIRECTION) == SPI_Direction_Tx))
/**
 * @brief SPI_Mode
 */
#define SPI_Mode_Master   ((uint16_t)0x0004)
#define SPI_Mode_Slave    ((uint16_t)0x0000)
#define IS_SPI_MODE(MODE) (((MODE) == SPI_Mode_Master) || \
                           ((MODE) == SPI_Mode_Slave))
/**
 * @brief SPI_DataSize
 */
#define SPI_DataSize_MASK         ((uint32_t)0xffffe0ff)
#define SPI_DataSize_16b          ((uint16_t)0x0f00)
#define SPI_DataSize_8b           ((uint16_t)0x0700)
#define IS_SPI_DATASIZE(DATASIZE) (((DATASIZE) == SPI_DataSize_16b) || \
                                   ((DATASIZE) == SPI_DataSize_8b))
/**
 * @brief SPI_CPOL
 */
#define SPI_CPOL_Low      ((uint16_t)0x0000)
#define SPI_CPOL_High     ((uint16_t)0x0002)
#define IS_SPI_CPOL(CPOL) (((CPOL) == SPI_CPOL_Low) || \
                           ((CPOL) == SPI_CPOL_High))
/**
 * @brief SPI_CPHA
 */
#define SPI_CPHA_1Edge    ((uint16_t)0x0000)
#define SPI_CPHA_2Edge    ((uint16_t)0x0001)
#define IS_SPI_CPHA(CPHA) (((CPHA) == SPI_CPHA_1Edge) || \
                           ((CPHA) == SPI_CPHA_2Edge))

/**
 * @brief SPI_NSS always 0
 */
#define SPI_NSS_Hard    ((uint16_t)0x0000)
#define IS_SPI_NSS(NSS) ((NSS) == SPI_NSS_Hard)

/**
 * @brief SPI_BaudRatePrescaler
 */
#define SPI_BaudRatePrescaler_2              ((uint16_t)0x0002)
#define SPI_BaudRatePrescaler_4              ((uint16_t)0x0004)
#define SPI_BaudRatePrescaler_8              ((uint16_t)0x0008)
#define SPI_BaudRatePrescaler_16             ((uint16_t)0x0010)
#define SPI_BaudRatePrescaler_32             ((uint16_t)0x0020)
#define SPI_BaudRatePrescaler_64             ((uint16_t)0x0040)
#define SPI_BaudRatePrescaler_128            ((uint16_t)0x0080)
#define SPI_BaudRatePrescaler_256            ((uint16_t)0x0100)
#define IS_SPI_BAUDRATE_PRESCALER(PRESCALER) (((PRESCALER) == SPI_BaudRatePrescaler_2) || \
                                              ((PRESCALER) == SPI_BaudRatePrescaler_4) || \
                                              ((PRESCALER) == SPI_BaudRatePrescaler_8) || \
                                              ((PRESCALER) == SPI_BaudRatePrescaler_16) || \
                                              ((PRESCALER) == SPI_BaudRatePrescaler_32) || \
                                              ((PRESCALER) == SPI_BaudRatePrescaler_64) || \
                                              ((PRESCALER) == SPI_BaudRatePrescaler_128) || \
                                              ((PRESCALER) == SPI_BaudRatePrescaler_256))

/**
 * @brief SPI_FirstBit
 */
#define SPI_FirstBit_MSB      ((uint16_t)0x0000)
#define SPI_FirstBit_LSB      ((uint16_t)0x0004)
#define IS_SPI_FIRST_BIT(BIT) (((BIT) == SPI_FirstBit_MSB) || \
                               ((BIT) == SPI_FirstBit_LSB))

/**
 * @brief SPI_interrupts_definition
 */
#define SPI_IT_TXE  ((uint8_t)0x01)
#define SPI_IT_RXNE ((uint8_t)0x02)
#define SPI_IT_ERR  ((uint8_t)0x04)

#define SPI_IT_TX            ((uint8_t)0x40)
#define SPI_IT_OVR           ((uint8_t)0x08)
#define SPI_IT_UDR           ((uint8_t)0x04)
#define SPI_IT_RXFULL        ((uint8_t)0x20)
#define SPI_IT_ALL           ((uint8_t)0xFF)
#define IS_SPI_CONFIG_IT(IT) (((IT) == SPI_IT_TXE) || \
                              ((IT) == SPI_IT_RXNE) || \
                              ((IT) == SPI_IT_ERR) || \
                              ((IT) == SPI_IT_ALL))
#define IS_SPI_GET_IT(IT) (((IT) == SPI_IT_RXNE) || ((IT) == SPI_IT_TXE) || \
                           ((IT) == SPI_IT_TX) || ((IT) == SPI_IT_OVR) || \
                           ((IT) == SPI_IT_ALL) || ((IT) == SPI_IT_ERR) || \
                           ((IT) == SPI_IT_UDR) || ((IT) == SPI_IT_RXFULL))

#define IS_SPI_CLEAR_IT(IT) (((IT) == SPI_IT_RXNE) || ((IT) == SPI_IT_TXE) || \
                             ((IT) == SPI_IT_TX) || ((IT) == SPI_IT_OVR) || \
                             ((IT) == SPI_IT_ALL) || ((IT) == SPI_IT_ERR) || \
                             ((IT) == SPI_IT_UDR) || ((IT) == SPI_IT_RXFULL))

/**
 * @brief SPI_DMA_transfer_requests
 * Only DMA access TX FIFO and RX FIFO
 */
#define SPI_DMAReq_Tx         ((uint16_t)0x0300)
#define SPI_DMAReq_Rx         ((uint16_t)0x0240)
#define IS_SPI_DMAREQ(DMAREQ) (((DMAREQ) == SPI_DMAReq_Tx) || ((DMAREQ) == SPI_DMAReq_Rx))

/**
 * @brief SPI_flags_definition
 */
#define SPI_FLAG_TXE          ((uint16_t)0x0001)
#define SPI_FLAG_RXNE         ((uint16_t)0x0002)
#define SPI_FLAG_TXFULL       ((uint16_t)0x0004)
#define SPI_FLAG_RXAVL_4      ((uint16_t)0x0008)
#define IS_SPI_GET_FLAG(FLAG) (((FLAG) == SPI_FLAG_TXFULL) || ((FLAG) == SPI_FLAG_RXAVL_4) || \
                               ((FLAG) == SPI_FLAG_TXE) || ((FLAG) == SPI_FLAG_RXNE))

/**
 * @brief Deinitializes the SPIx peripheral registers to their default
 *         reset values.
 * @param[in] SPIx : where x can be 1 or 2 to select the SPI peripheral.
 */
void SPI_DeInit(SPI_TypeDef *SPIx);

/**
 * @brief Initializes the SPIx peripheral according to the specified
 *         parameters in the SPI_InitStruct.
 * @param[in] SPIx: where x can be 1 or 2 to select the SPI peripheral.
 * @param[in] SPI_InitStruct: pointer to a SPI_InitTypeDef structure that
 *         contains the configuration information for the specified SPI peripheral.
 */
void SPI_Init(SPI_TypeDef *SPIx, SPI_InitTypeDef *SPI_InitStruct);

/**
 * @brief Fills each SPI_InitStruct member with its default value.
 * @param[in] SPI_InitStruct: pointer to a SPI_InitTypeDef structure which will be initialized.
 */
void SPI_StructInit(SPI_InitTypeDef *SPI_InitStruct);

/**
 * @brief Enables or disables the specified SPI peripheral.
 * @param[in] SPIx: where x can be 1 or 2 to select the SPI peripheral.
 * @param[in] NewState: new state of the SPIx peripheral.
 *   This parameter can be: ENABLE or DISABLE.
 */
void SPI_Cmd(SPI_TypeDef *SPIx, FunctionalState NewState);

/**
 * @brief Enables or disables the specified SPI interrupts.
 * @param[in] SPIx: where x can be 1 or 2.
 * @param[in] SPI_IT:specifies the SPI interrupt source to be enabled or disabled.
 *   This parameter can be one of the following values:
 *     @arg SPI_IT_TXE: Tx buffer empty interrupt mask
 *     @arg SPI_IT_RXNE: Rx buffer not empty interrupt mask
 *     @arg SPI_IT_ERR: Error interrupt mask
 * @param[in] NewState: new state of the specified SPI interrupt.
 *   This parameter can be: ENABLE or DISABLE.
 */
void SPI_ITConfig(SPI_TypeDef *SPIx, uint8_t SPI_IT, FunctionalState NewState);

/**
 * @brief Enables or disables the SPIx DMA interface.
 * @param[in] SPIx: where x can be  1 or 2.
 * @param[in] SPI_DMAReq: specifies the SPI DMA transfer request to be enabled or disabled.
 *   This parameter can be any combination of the following values:
 *     @arg SPI_DMAReq_Tx: Tx buffer DMA transfer request
 *     @arg SPI_DMAReq_Rx: Rx buffer DMA transfer request
 * @param[in] NewState: new state of the selected SPI DMA transfer request.
 *   This parameter can be: ENABLE or DISABLE.
 */
void SPI_DMACmd(SPI_TypeDef *SPIx, uint16_t SPI_DMAReq, FunctionalState NewState);

/**
 * @brief Transmits a Data through the SPIx peripheral.
 * @param[in] SPIx: where x can be  1 or 2.
 * @param[in] Data: Data to be transmitted.
 */
void SPI_SendData(SPI_TypeDef *SPIx, uint16_t Data);

/**
 * @brief Returns the most recent received data by the SPIx peripheral.
 * @param[in] SPIx: where x can be  1 or 2.
 * @return uint16_t: The value of the received data.
 */
uint16_t SPI_ReceiveData(SPI_TypeDef *SPIx);

/**
 * @brief nables or disables the SS output for the selected SPI.
 * @param[in] SPIx: where x can be 1 or 2 to select the SPI peripheral.
 * @param[in] NewState: new state of the SPIx SS output.
 *   This parameter can be: ENABLE or DISABLE.
 */
void SPI_SSOutputCmd(SPI_TypeDef *SPIx, FunctionalState NewState);

/**
 * @brief Configures the data size for the selected SPI.
 * @param[in] SPIx: where x can be 1 or 2 to select the SPI peripheral.
 * @param[in] SPI_DataSize: specifies the SPI data size.
 *   This parameter can be one of the following values:
 *     @arg SPI_DataSize_16b: Set data frame format to 16bit
 *     @arg SPI_DataSize_8b: Set data frame format to 8bit
 */
void SPI_DataSizeConfig(SPI_TypeDef *SPIx, uint16_t SPI_DataSize);

/**
 * @brief Selects the data transfer direction in bi-directional mode for the specified SPI.
 * @param[in] SPIx: where x can be 1 or 2 to select the SPI peripheral.
 * @param[in] SPI_Direction: specifies the data transfer direction in bi-directional mode.
 *   This parameter can be one of the following values:
 *     @arg SPI_Direction_Tx: Selects Tx/Rxtransmission direction
 *     @arg SPI_Direction_Rx: Selects only Rx receive direction
 */
void SPI_BiDirectionalLineConfig(SPI_TypeDef *SPIx, uint16_t SPI_Direction);

/**
 * @brief  Checks whether the specified SPI flag is set or not.
 * @param[in] SPIx: where x can be 1 or 2 to select the SPI peripheral.
 * @param[in] SPI_FLAG: specifies the SPI flag to check.
 *   This parameter can be one of the following values:
 *     @arg SPI_FLAG_TXE:  Transmit buffer empty flag.
 *     @arg SPI_FLAG_RXNE: Receive buffer not empty flag.
 *     @arg SPI_FLAG_TXFULL: Transmit buffer full flag.
 *     @arg SPI_FLAG_RXAVL_4: Receive buffer available 4.
 * @return FlagStatus: The new state of SPI_FLAG (SET or RESET).
 */
FlagStatus SPI_GetFlagStatus(SPI_TypeDef *SPIx, uint16_t SPI_FLAG);

/**
 * @brief  Checks whether the specified SPI interrupt has occurred or not.
 * @param[in] SPIx: where x can be 1 or 2 to select the SPI peripheral.
 * @param[in] SPI_IT: specifies the SPI interrupt source to check.
 *   This parameter can be one of the following values:
 *     @arg SPI_IT_TXE: Transmit buffer empty interrupt.
 *     @arg SPI_IT_RXNE: Receive buffer not empty interrupt.
 *     @arg SPI_IT_OVR: Overrun interrupt.
 *     @arg SPI_IT_UDR: Underrun Fault interrupt.
 *     @arg SPI_IT_ERR: Underrun Fault interrupt.
 *     @arg SPI_IT_TX:  Transmit FIFO available interrupt.
 *     @arg SPI_IT_RXFULL: RX FIFO Full interrupt flag bit.
 * @return ITStatus: The new state of SPI_IT (SET or RESET).
 */
ITStatus SPI_GetITStatus(SPI_TypeDef *SPIx, uint8_t SPI_IT);

/**
 * @brief Clears the SPIx CRC Error (CRCERR) interrupt pending bit.
 * @param[in] SPIx: where x can be 1 or 2 to select the SPI peripheral.
 * @param[in] SPI_IT: specifies the SPI interrupt pending bit to clear.
 *   This parameter can be one of the following values:
 *     @arg SPI_IT_TXE: Transmit buffer empty interrupt.
 *     @arg SPI_IT_RXNE: Receive buffer not empty interrupt.
 *     @arg SPI_IT_OVR: Overrun interrupt.
 *     @arg SPI_IT_UDR: Underrun Fault interrupt.
 *     @arg SPI_IT_ERR: Underrun Fault interrupt.
 *     @arg SPI_IT_TX:  Transmit FIFO available interrupt.
 *     @arg SPI_IT_RXFULL: RX FIFO Full interrupt flag bit.
 */
void SPI_ClearITPendingBit(SPI_TypeDef *SPIx, uint8_t SPI_IT);

#ifdef __cplusplus
}
#endif
#endif