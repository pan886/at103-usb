/**
 * @file at103_spi.h
 * @brief This file contains all the functions prototypes for the SPI firmware library. \n
 *        How to use this driver? \n
 * (+) At first, use SPI_DeInit(...) to reset the required SPI. \n
 * (+) Then, use SPI_Init(...) with SPI initialization structure to initialize a SPI. \n
 *     For details on the SPI initialization structure, see "SPI_InitTypeDef". \n
 * (+) You can use SPI_ITConfig(...) to enable the required SPI interrupts and you need to implement the corresponding  service function: SPIx_IRQHandler. \n
 *     For details on the SPI interrupt type, see SPI interrupts type definition. \n
 * (+) You can use SPI_DMACmd(...) to enable SPI DMA transfer. \n
 *     For details on the SPI DMA type, see the SPI DMA requst type definition. \n
 * (+) Finally, use SPI_Cmd(...) to start a SPI communication. \n
 * (+) Of course, you can use SPI_GetFlagStatus(...) to get the state of the SPI at any time, this is helpful in handling the SPI communication. \n
 * (+) Note: \n
 *     SPI can support fullduplex communication with configurable Baudrate and clock phase.
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
    uint16_t SPI_Direction;         /**< Specifies the SPI unidirectional or bidirectional data mode. */
    uint16_t SPI_Mode;              /**< Specifies the SPI operating mode. */
    uint16_t SPI_DataSize;          /**< Specifies the SPI data size.  */
    uint16_t SPI_CPOL;              /**< Specifies the serial clock steady state. */
    uint16_t SPI_CPHA;              /**< Specifies the clock active edge for the bit capture.  */
    uint16_t SPI_NSS;               /**< Specifies whether the NSS signal is managed by hardware (NSS pin) or by software using the SSI bit.*/
    uint16_t SPI_BaudRatePrescaler; /**< Specifies the Baud Rate prescaler value which will be used to configure the transmit and receive SCK clock.
                                          The communication clock is derived from the master clock.
                                          The slave clock does not need to be set. */
    uint16_t SPI_FirstBit;          /**< Specifies whether data transfers start from MSB or LSB bit.*/
    uint16_t SPI_CRCPolynomial;     /**< Specifies the polynomial used for the CRC calculation. */
} SPI_InitTypeDef;

#define SPI_Direction_2Lines_FullDuplex ((uint16_t)0x0018) /**< SPI Recevive and Tramsmit Enable. */
#define SPI_Direction_2Lines_RxOnly     ((uint16_t)0x0010) /**< only SPI Recevive Enable. */
#define SPI_Direction_1Line_Rx          ((uint16_t)0x0010) /**< only SPI Recevive Enable. */
#define SPI_Direction_1Line_Tx          ((uint16_t)0x0008) /**< only SPI Transmit Enable. */

#define SPI_Direction_Rx ((uint16_t)0x0010) /**< only SPI Recevive Enable. */
#define SPI_Direction_Tx ((uint16_t)0x0008) /**< only SPI Transmit Enable. */

#define SPI_Mode_Master ((uint16_t)0x0004) /**< SPI Master mode. */
#define SPI_Mode_Slave  ((uint16_t)0x0000) /**< SPI Slave mode. */

#define SPI_DataSize_16b ((uint16_t)0x0f00) /**< SPI Data length is 16. */
#define SPI_DataSize_8b  ((uint16_t)0x0700) /**< SPI Data length is 8. */

#define SPI_CPOL_Low  ((uint16_t)0x0000) /**< Active-high SPI clocks selected. In idle state SCLK is low. */
#define SPI_CPOL_High ((uint16_t)0x0002) /**< Active-low SPI clocks selected. In idle state SCLK is high. */

#define SPI_CPHA_1Edge ((uint16_t)0x0000) /**< The second clock transition is the first data capture edge. */
#define SPI_CPHA_2Edge ((uint16_t)0x0001) /**< The first clock transition is the first data capture edge. */

#define SPI_NSS_Hard ((uint16_t)0x0000) /**< Only support software method. */

#define SPI_BaudRatePrescaler_2   ((uint16_t)0x0002) /**< Baud Rate is fpclk / 2. */
#define SPI_BaudRatePrescaler_4   ((uint16_t)0x0004) /**< Baud Rate is fpclk / 4. */
#define SPI_BaudRatePrescaler_8   ((uint16_t)0x0008) /**< Baud Rate is fpclk / 8. */
#define SPI_BaudRatePrescaler_16  ((uint16_t)0x0010) /**< Baud Rate is fpclk / 16. */
#define SPI_BaudRatePrescaler_32  ((uint16_t)0x0020) /**< Baud Rate is fpclk / 32. */
#define SPI_BaudRatePrescaler_64  ((uint16_t)0x0040) /**< Baud Rate is fpclk / 64. */
#define SPI_BaudRatePrescaler_128 ((uint16_t)0x0080) /**< Baud Rate is fpclk / 128. */
#define SPI_BaudRatePrescaler_256 ((uint16_t)0x0100) /**< Baud Rate is fpclk / 256. */

#define SPI_FirstBit_MSB ((uint16_t)0x0000) /**< Data is transferred or received most significant bit (MSB) first. */
#define SPI_FirstBit_LSB ((uint16_t)0x0004) /**< Data is transferred or received least significant bit (LSB) first. */

#define SPI_IT_TXE  ((uint8_t)0x01) /**< Transmit FIFO available interrupt. */
#define SPI_IT_RXNE ((uint8_t)0x02) /**< Receiver data available interrupt. */
#define SPI_IT_ERR  ((uint8_t)0x0C) /**< SPI slave transmitter underrun or Receiver Overrun Error interrupt . */

#define SPI_IT_TX     ((uint8_t)0x40) /**< Transmitter empty interrupt . */
#define SPI_IT_OVR    ((uint8_t)0x08) /**< Receiver Overrun Error interrupt . */
#define SPI_IT_UDR    ((uint8_t)0x04) /**< SPI slave transmitter underrun . */
#define SPI_IT_RXFULL ((uint8_t)0x20) /**< Transmitter empty interrupt . */
#define SPI_IT_ALL    ((uint8_t)0xFF) /**< All SPI interrupt . */

#define SPI_DMAReq_Tx ((uint16_t)0x0300) /**< enable DMA TX access,TX FIFO trigger level is 8 or more valid space in TX FIFO. */
#define SPI_DMAReq_Rx ((uint16_t)0x0240) /**< enable DMA RX access,RX FIFO trigger level is more then 8  valid data in RX FIFO. */

/**
 * @brief SPI_flags_definition
 */
#define SPI_FLAG_TXE     ((uint16_t)0x0001) /**< Transmitter empty flag. */
#define SPI_FLAG_RXNE    ((uint16_t)0x0002) /**< Receive available data flag. */
#define SPI_FLAG_TXFULL  ((uint16_t)0x0004) /**< Transmitter FIFO Full Status flag. */
#define SPI_FLAG_RXAVL_4 ((uint16_t)0x0008) /**< Receive available 4 word data flag. */

/**
 * @brief Deinitializes the SPIx peripheral registers to their default reset values.
 * @param[in] SPIx: where x can be 1 or 2 to select the SPI peripheral.
 */
void SPI_DeInit(SPI_TypeDef *SPIx);

/**
 * @brief Initializes the SPIx peripheral according to the specified parameters in the SPI_InitStruct.
 * @param[in] SPIx: where x can be 1 or 2 to select the SPI peripheral.
 * @param[in] SPI_InitStruct: pointer to a SPI_InitTypeDef structure that
 *            contains the configuration information for the specified SPI peripheral.
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
 *            This parameter can be: ENABLE or DISABLE.
 */
void SPI_Cmd(SPI_TypeDef *SPIx, FunctionalState NewState);

/**
 * @brief Enables or disables the specified SPI interrupts.
 * @param[in] SPIx: where x can be 1 or 2.
 * @param[in] SPI_IT: the SPI interrupt source to be enabled or disabled.
 *            This parameter can be one of the following values: \n
 *              SPI_IT_TXE: Tx buffer empty interrupt mask; \n
 *              SPI_IT_RXNE: Rx buffer not empty interrupt mask; \n
 *              SPI_IT_ERR: Error interrupt mask.
 * @param[in] NewState: new state of the specified SPI interrupt.
 *            This parameter can be: ENABLE or DISABLE.
 */
void SPI_ITConfig(SPI_TypeDef *SPIx, uint8_t SPI_IT, FunctionalState NewState);

/**
 * @brief Enables or disables the SPIx DMA interface.
 * @param[in] SPIx: where x can be  1 or 2.
 * @param[in] SPI_DMAReq: specifies the SPI DMA transfer request to be enabled or disabled.
 *              This parameter can be any combination of the following values: \n
 *              SPI_DMAReq_Tx: Tx buffer DMA transfer request; \n
 *              SPI_DMAReq_Rx: Rx buffer DMA transfer request.
 * @param[in] NewState: new state of the selected SPI DMA transfer request.
 *            This parameter can be: ENABLE or DISABLE.
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
 *            This parameter can be: ENABLE or DISABLE.
 */
void SPI_SSOutputCmd(SPI_TypeDef *SPIx, FunctionalState NewState);

/**
 * @brief Configures the data size for the selected SPI.
 * @param[in] SPIx: where x can be 1 or 2 to select the SPI peripheral.
 * @param[in] SPI_DataSize: specifies the SPI data size.
 *            This parameter can be one of the following values: \n
 *              SPI_DataSize_16b: Set data frame format to 16bit; \n
 *              SPI_DataSize_8b: Set data frame format to 8bit.
 */
void SPI_DataSizeConfig(SPI_TypeDef *SPIx, uint16_t SPI_DataSize);

/**
 * @brief Selects the data transfer direction in bi-directional mode for the specified SPI.
 * @param[in] SPIx: where x can be 1 or 2 to select the SPI peripheral.
 * @param[in] SPI_Direction: specifies the data transfer direction in bi-directional mode.
 *            This parameter can be one of the following values: \n
 *              SPI_Direction_Tx: Selects Tx/Rxtransmission direction; \n
 *              SPI_Direction_Rx: Selects only Rx receive direction.
 */
void SPI_BiDirectionalLineConfig(SPI_TypeDef *SPIx, uint16_t SPI_Direction);

/**
 * @brief  Checks whether the specified SPI flag is set or not.
 * @param[in] SPIx: where x can be 1 or 2 to select the SPI peripheral.
 * @param[in] SPI_FLAG: specifies the SPI flag to check.
 *            This parameter can be one of the following values: \n
 *              SPI_FLAG_TXE:  Transmit buffer empty flag; \n
 *              SPI_FLAG_RXNE: Receive buffer not empty flag; \n
 *              SPI_FLAG_TXFULL: Transmit buffer full flag; \n
 *              SPI_FLAG_RXAVL_4: Receive buffer available 4.
 * @return FlagStatus: The new state of SPI_FLAG (SET or RESET).
 */
FlagStatus SPI_GetFlagStatus(SPI_TypeDef *SPIx, uint16_t SPI_FLAG);

/**
 * @brief  Checks whether the specified SPI interrupt has occurred or not.
 * @param[in] SPIx: where x can be 1 or 2 to select the SPI peripheral.
 * @param[in] SPI_IT: specifies the SPI interrupt source to check.
 *            This parameter can be one of the following values: \n
 *              SPI_IT_TXE: Transmit buffer empty interrupt; \n
 *              SPI_IT_RXNE: Receive buffer not empty interrupt; \n
 *              SPI_IT_OVR: Overrun interrupt; \n
 *              SPI_IT_UDR: Underrun Fault interrupt; \n
 *              SPI_IT_ERR: Underrun Fault interrupt; \n
 *              SPI_IT_TX:  Transmit FIFO available interrupt; \n
 *              SPI_IT_RXFULL: RX FIFO Full interrupt flag bit.
 * @return ITStatus: The new state of SPI_IT (SET or RESET).
 */
ITStatus SPI_GetITStatus(SPI_TypeDef *SPIx, uint8_t SPI_IT);

/**
 * @brief Clears the SPIx CRC Error (CRCERR) interrupt pending bit.
 * @param[in] SPIx: where x can be 1 or 2 to select the SPI peripheral.
 * @param[in] SPI_IT: specifies the SPI interrupt pending bit to clear.
 *            This parameter can be one of the following values: \n
 *              SPI_IT_TXE: Transmit buffer empty interrupt; \n
 *              SPI_IT_RXNE: Receive buffer not empty interrupt; \n
 *              SPI_IT_OVR: Overrun interrupt; \n
 *              SPI_IT_UDR: Underrun Fault interrupt; \n
 *              SPI_IT_ERR: Underrun Fault interrupt; \n
 *              SPI_IT_TX:  Transmit FIFO available interrupt; \n
 *              SPI_IT_RXFULL: RX FIFO Full interrupt flag bit.
 */
void SPI_ClearITPendingBit(SPI_TypeDef *SPIx, uint8_t SPI_IT);

#ifdef __cplusplus
}
#endif
#endif