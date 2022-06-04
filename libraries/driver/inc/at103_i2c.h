/**
 * @file at103_i2c.h
 * @brief This file contains all the functions prototypes for the i2c firmware library. \n
 *         How to use this driver? \n
 * (+) At first, use I2C_DeInit(…) to reset the required I2C. \n
 * (+) Then, use I2C_Init(…) with I2C initialization structure to initialize a I2C. \n
 * For details on the I2C initialization structure, see “I2C_InitTypeDef”. \n
 * (+) You can use I2C_ITConfig(…) to enable the required I2C interrupts and you need to implement the corresponding service function: I2cx_IRQHandler. \n
 * (+) Finally, use I2C_DMACmd(…) to start a I2C channel transfer.
 * @author ybb (ybb@163.com)
 * @version 1.0
 * @date 2022-05-27
 * @copyright Copyright (c) 2022 Icore, Inc
 */

#ifndef __AT103_I2C_H
#define __AT103_I2C_H

#ifdef __cplusplus
extern "C" {
#endif

#include "at103.h"

typedef FlagStatus I2C_BusStatus;
typedef FlagStatus I2C_ModuleEnable_Status;
typedef FlagStatus I2C_TxAbort_Status;

/**
 * @brief I2C init structure definition
 */

typedef struct
{
    uint8_t  I2C_Mode;          /**< Specifies the I2C mode.  This parameter can be Master_Mode or Slave_Mode.*/
    uint32_t I2C_SpeedMode;     /**< Specifies the I2C Speed Mode. This parameter must be Standardmode or Fastmode. */
    uint32_t I2C_ClockSpeed;    /**< Specifies the clock frequency. This parameter must be set to a value lower than 1000kHz. */
    uint16_t I2C_TargetAddress; /**< Specifies the target address.This parameter can be a 7-bit or 10-bit address. */
    uint16_t I2C_SlaveAddress;  /**< Specifies the slave address.This parameter can be a 7-bit or 10-bit address. */
    uint16_t I2C_AddressMode;   /**< Specifies if 7-bit or 10-bit address is acknowledged. */
} I2C_InitTypeDef;

#define I2C_Mode_Master      ((uint8_t)0x00) /**< Specifies I2C mode Master Mode. */
#define I2C_Mode_Slave       ((uint8_t)0x01) /**< Specifies I2C mode Slave  Mode. */
#define I2C_Mode_GeneralCall ((uint8_t)0x02) /**< Specifies I2C mode GenerCall Mode. */

#define I2C_SpeedMode_Standard ((uint8_t)0x01) /**< Specifies I2C speed mode Standard Mode. */
#define I2C_SpeedMode_Fast     ((uint8_t)0x02) /**< Specifies I2C speed mode Fast Mode. */

#define I2C_AddressMode_7bit  ((uint8_t)0x00) /**< Specifies I2C address mode 7_bit. */
#define I2C_AddressMode_10bit ((uint8_t)0x01) /**< Specifies I2C address mode 10_bit. */

#define I2C_DMAReq_Tx ((uint8_t)0x01) /**< Specifies I2C DMA Direction Transmit. */
#define I2C_DMAReq_Rx ((uint8_t)0x02) /**< Specifies I2C DMA Direction Receive. */

#define I2C_IT_RXUNDER       ((uint32_t)0x00000001) /**< rx fifo under intrrrupt. */
#define I2C_IT_RXOVER        ((uint32_t)0x00000002) /**< rx fifo over intrrrupt. */
#define I2C_IT_RXFULL        ((uint32_t)0x00000004) /**< rx fifo full intrrrupt. */
#define I2C_IT_TXOVER        ((uint32_t)0x00000008) /**< tx fifo over intrrrupt. */
#define I2C_IT_TXEMPTY       ((uint32_t)0x00000010) /**< tx fifo empty intrrrupt. */
#define I2C_IT_RDREQ         ((uint32_t)0x00000020) /**< read request intrrrupt. */
#define I2C_IT_TXABRT        ((uint32_t)0x00000040) /**< tx abort intrrrupt. */
#define I2C_IT_RXDONE        ((uint32_t)0x00000080) /**< rx done intrrrupt. */
#define I2C_IT_ACTIVITY      ((uint32_t)0x00000100) /**< bus activity intrrrupt. */
#define I2C_IT_STOP          ((uint32_t)0x00000200) /**< stop signal intrrrupt. */
#define I2C_IT_START         ((uint32_t)0x00000400) /**< start or restart signal intrrrupt. */
#define I2C_IT_GENCALL       ((uint32_t)0x00000800) /**< general call intrrrupt. */
#define I2C_IT_RESTART       ((uint32_t)0x00001000) /**< restart signal intrrrupt as addressed slave. */
#define I2C_IT_MSTONHOLD     ((uint32_t)0x00002000) /**< master hold bus intrrrupt. */
#define I2C_IT_SCLSTUCKATLOW ((uint32_t)0x00004000) /**< lines stuck at low interrupt. */
#define I2C_IT_ALL           ((uint32_t)0x00007FFF) /**< Specifies all interrupt. */

#define I2C_STATUS_ACTIVITY               ((uint32_t)0x00000001) /**< I2C bus status activity. */
#define I2C_STATUS_TFNF                   ((uint32_t)0x00000002) /**< transmit fifo not full. */
#define I2C_STATUS_TFE                    ((uint32_t)0x00000004) /**< transmit fifo empty. */
#define I2C_STATUS_RFNE                   ((uint32_t)0x00000008) /**< receive fifo not empty. */
#define I2C_STATUS_RFF                    ((uint32_t)0x00000010) /**< receive fifo full. */
#define I2C_STATUS_MST_ACTIVITY           ((uint32_t)0x00000020) /**< master status activity. */
#define I2C_STATUS_SLV_ACTIVITY           ((uint32_t)0x00000040) /**< slave status activity. */
#define I2C_STATUS_MST_HOLD_TX_FIFO_EMPTY ((uint32_t)0x00000080) /**< master hold bus because tx fifo empty. */
#define I2C_STATUS_MST_HOLD_RX_FIFO_FULL  ((uint32_t)0x00000100) /**< master hold bus because rx fifo full. */
#define I2C_STATUS_SLV_HOLD_TX_FIFO_EMPTY ((uint32_t)0x00000200) /**< slave hold bus because tx fifo empty. */
#define I2C_STATUS_SLV_HOLD_RX_FIFO_FULL  ((uint32_t)0x00000400) /**< slave hold bus because rx fifo full. */

#define I2C_TX_ABRT_DEVICE_WRITE         ((uint32_t)0x00100000) /**< master transfer device id. */
#define I2C_TX_ABRT_DEVICE_SLVADDR_NOACK ((uint32_t)0x00080000) /**< master transfer device id and no slave acknowledged it. */
#define I2C_TX_ABRT_DEVICE_NOACK         ((uint32_t)0x00040000) /**< master transfer device id and no slave acknowledged the id. */
#define I2C_TX_ABRT_SDA_STUCK_AT_LOW     ((uint32_t)0x00020000) /**< SDA stuck at low. */
#define I2C_TX_ABRT_USER_ABRT            ((uint32_t)0x00010000) /**< master detecte the transfer abort. */
#define I2C_TX_ABRT_SLVRD_INTX           ((uint32_t)0x00008000) /**< user write a 1(read) in CMD when module acknowledged slave transmit request. */
#define I2C_TX_ABRT_SLV_ARBLOST          ((uint32_t)0x00004000) /**< slave lost bus while transmitting data to a remote master. */
#define I2C_TX_ABRT_SLVFLUSH_TXFIFO      ((uint32_t)0x00002000) /**< slave flush old tx fifo. */
#define I2C_TX_ARBT_LOST                 ((uint32_t)0x00001000) /**< I2C module has lost arbitration. */
#define I2C_TX_ABRT_MASTER_DIS           ((uint32_t)0x00000800) /**< User tries to initiate a Master operation with the Master mode disabled. */
#define I2C_TX_ABRT_10B_RD_NORSTRT       ((uint32_t)0x00000400) /**< The restart is disabled and the master sends a read command in 10-bit addressing mode. */
#define I2C_TX_ABRT_SBYTE_NORSTRT        ((uint32_t)0x00000200) /**< The restart is disabled and the user is trying to send a START Byte. */
#define I2C_TX_ABRT_HS_NORSTRT           ((uint32_t)0x00000100) /**< The restart is disabled and the user is trying to use the master to transfer data in High Speed mode. */
#define I2C_TX_ABRT_SBYTE_ACKDET         ((uint32_t)0x00000080) /**< Master has sent a START Byte and the START Byte was acknowledged. */
#define I2C_TX_ABRT_HS_ACKDET            ((uint32_t)0x00000040) /**< Master is in High Speed mode and the High Speed Master code was acknowledged. */
#define I2C_TX_ABRT_GCALL_READ           ((uint32_t)0x00000020) /**< I2C in master mode sent a General Call but the user programmed the byte following the General Call to be a read from the bus is set to 1. */
#define I2C_TX_ABRT_GCALL_NOACK          ((uint32_t)0x00000010) /**< I2C in master mode sent a General Call and no slave on the bus acknowledged the General Call. */
#define I2C_TX_ABRT_TXDATA_NOACK         ((uint32_t)0x00000008) /**< This is a master-mode only bit. Master has received an acknowledgement for the address, but when it sent data byte(s) following the address, it did not receive an acknowledge from the remote slave(s). */
#define I2C_TX_ABRT_10ADDR2_NOACK        ((uint32_t)0x00000004) /**< Master is in 10-bit address mode and the second address byte of the 10-bit address was not acknowledged by any slave. */
#define I2C_TX_ABRT_10ADDR1_NOACK        ((uint32_t)0x00000002) /**< Master is in 7-bit addressing mode and the address sent was not acknowledged by any slave. */
#define I2C_TX_ABRT_7B_ADDR_NOACK        ((uint32_t)0x00000001) /**< Master is in 7-bit addressing mode and the address sent was not acknowledged by any slave. */

#define I2C_Direction_Transmitter ((uint8_t)0x00) /**< I2C Direction Transmitter. */
#define I2C_Direction_Receiver    ((uint8_t)0x01) /**< I2C Direction Receiver. */

#define I2C_SLV_RX_DATA_LOST        ((uint8_t)0x04) /**< I2C Enable Status slave rx data lost. */
#define I2C_SLV_DISABLED_WHILE_BUSY ((uint8_t)0x02) /**< I2C Enable Status slave disabled while busy. */
#define I2C_ENABLE                  ((uint8_t)0x01) /**< I2C Enable Status enable. */

#define I2C_StopActive_IfMasterActive   ((uint8_t)0x01) /**< Issues the STOP interrupt only when the master is active. */
#define I2C_StopActive_MasterAlways     ((uint8_t)0x02) /**< Issues the STOP irrespective of whether the master is active. */
#define I2C_StopActive_IfSlaveAddressed ((uint8_t)0x04) /**< Issues the STOP interrupt only when it is addressed. */
#define I2C_StopActive_SlaveAlways      ((uint8_t)0x08) /**< Issues the STOP irrespective of whether it’s addressed or not. */

/**
 * @brief Deinitializes the I2Cx peripheral registers to their default reset values.
 * @param[in] I2Cx: where x can be 1 or 2 to select the I2C peripheral.
 *  
 */
void I2C_DeInit(I2C_TypeDef *I2Cx);

/**
 * @brief Initializes the I2Cx peripheral according to the specified parameters in the I2C_InitStruct.
 * @param[in] I2Cx: where x can be 1 or 2 to select the I2C peripheral.
 * @param[in] I2C_InitStruct: pointer to a I2C_InitTypeDef structure that contains the configuration information for the specified I2C peripheral.
 *  
 */
void I2C_Init(I2C_TypeDef *I2Cx, I2C_InitTypeDef *I2C_InitStruct);

/**
 * @brief Fills each I2C_InitStruct member with its default value.
 * @param[in] I2C_InitStruct: pointer to an I2C_InitTypeDef structure which will be initialized.
 *  
 */
void I2C_StructInit(I2C_InitTypeDef *I2C_InitStruct);

/**
 * @brief Enables or disables the specified I2C peripheral.
 * @param[in] I2Cx: where x can be 1 or 2 to select the I2C peripheral.
 * @param[in] NewState: new state of the I2Cx peripheral. \n
 *   This parameter can be: ENABLE or DISABLE.
 *  
 */
void I2C_Cmd(I2C_TypeDef *I2Cx, FunctionalState NewState);

/**
 * @brief Enables or disables the specified I2C Tx DMA requests.
 * @param[in] I2Cx: where x can be 1 or 2 to select the I2C peripheral.
 * @param[in] I2C_DMAReq: specifies the I2C DMA transfer request to be enabled or disabled. \n
 *   This parameter can be any combination of the following values: \n
 *      I2C_DMAReq_Tx: Tx buffer DMA transfer request. \n
 *      I2C_DMAReq_Rx: Rx buffer DMA transfer request. \n
 * @param[in] NewState: new state of the I2C DMA transfer. \n
 *   This parameter can be: ENABLE or DISABLE.
 *  
 */
void I2C_DMACmd(I2C_TypeDef *I2Cx, uint8_t I2C_DMAReq, FunctionalState NewState);

/**
 * @brief Configure Tx DMA Data Level
 * @param[in] I2Cx: where x can be 1 or 2 to select the I2C peripheral.
 * @param[in] I2C_DMAReq: specifies the I2C DMA transfer request to be enabled or disabled. \n
 *   This parameter can be any combination of the following values: \n
 *     I2C_DMAReq_Tx: Tx buffer DMA transfer request. \n
 *     I2C_DMAReq_Rx: Rx buffer DMA transfer request.
 * @param[in] DataLevel: Transmit Data Level
 * This bit field controls the level at which a DMA request is made by the transmit logic. \n
 * It is equal to the watermark level; that is, the dma_tx_req signal is generated \n
 * when the number of valid data entries in the transmit FIFO is equal to or below this field value, and TDMAE = 1.
 *  
 */
void I2C_DMADataLevelConfig(I2C_TypeDef *I2Cx, uint8_t I2C_DMAReq, uint8_t DataLevel);

/**
 * @brief Generates I2Cx communication start or restart condition.
 * @param[in] I2Cx: where x can be 1 or 2 to select the I2C peripheral.
 * @param[in] NewState: new state of the I2C START condition generation. \n
 *     This parameter can be: ENABLE or DISABLE.
 *  
 */
void I2C_GenerateSTART(I2C_TypeDef *I2Cx, FunctionalState NewState);

/**
 * @brief Generates I2Cx communication stop condition.
 * @param[in] I2Cx: where x can be 1 or 2 to select the I2C peripheral.
 * @param[in] NewState: new state of the I2C STOP condition generation. \n
 *   This parameter can be: ENABLE or DISABLE.
 *  
 */
void I2C_GenerateSTOP(I2C_TypeDef *I2Cx, FunctionalState NewState);

/**
 * @brief In Master Mode Blocks the transmission of data on I2C bus.
 * @param[in] I2Cx: where x can be 1 or 2 to select the I2C peripheral.
 * @param[in] NewState: new state of the I2C Transmit. \n
 * This parameter can be: ENABLE or DISABLE. \n
 *   In Master mode \n
 *   Enable: Blocks the transmission of data on I2C bus even if Tx FIFO has data to transmit. \n
 *   Disable: The transmission of data starts on I2C bus automatically, as soon as the first data is available in the Tx FIFO. \n
 *   [Note] To block the execution of Master commands, set the TX_CMD_BLOCK bit only when Tx FIFO is empty ( STATUS[2]=1) \n
 *   and the master is in the Idle state ( STATUS[5] == 0). \n 
 *   Any further commands put in the Tx FIFO are not executed until TX_CMD_BLOCK bit is unset.
 *  
 */
void I2C_TxBlock(I2C_TypeDef *I2Cx, FunctionalState NewState);

/**
 * @brief Generate the the transfer abort.
 * @param[in] I2Cx: where x can be 1 or 2 to select the I2C peripheral.
 * @param[in] NewState: new state of the I2C Transmit. \n
 * This parameter can be: ENABLE or DISABLE. \n
 *   Disable: ABORT not initiated or ABORT done. \n 
 *   Enable: ABORT operation in progress.
 *  
 */
void I2C_TxAbort(I2C_TypeDef *I2Cx, FunctionalState NewState);

/**
 * @brief This bit controls whether i2c should hold the bus \n 
 * when the Rx FIFO is physically full to its RX_BUFFER_DEPTH, \n 
 * If the configuration parameter IC_RX_FULL_HLD_BUS_EN is enabled and \n 
 * bit 9 of the IC_CON register (RX_FIFO_FULL_HLD_CTRL) is programmed to HIGH, \n 
 * then the RX_OVER interrupt never occurs, because the Rx FIFO never overflows.
 * @param[in] I2Cx: where x can be 1 or 2 to select the I2C peripheral.
 * @param[in] NewState: new state of the RxFifoFullHold.
 *  
 */
void I2C_RxFifoFullHold(I2C_TypeDef *I2Cx, FunctionalState NewState);

/**
 * @brief When TX_EMPTY_CTRL = 0, \n
 * This bit is set to 1 when the transmit buffer is at or below the threshold value set in the IC_TX_TL register. \n 
 * When TX_EMPTY_CTRL = 1: This bit is set to 1 when the transmit buffer is at or below the threshold value set in the IC_TX_TL register \n 
 * and the transmission of the address/data from the internal shift register for the most recently popped command is completed. \n 
 * It is automatically cleared by hardware when the buffer level goes above the threshold. \n 
 * When IC_ENABLE[0] is set to 0, the TX FIFO is flushed and held in reset. \n 
 * There the TX FIFO looks like it has no data within it, so this bit is set to 1, \n
 * provided there is activity in the master or slave state machines. \n 
 * When there is no longer any activity, then with ic_en=0, this bit is set to 0.
 * @param[in] I2Cx: where x can be 1 or 2 to select the I2C peripheral.
 * @param[in] NewState: new state of the RxFifoFullHold.
 *  
 */
void I2C_TxEmptyCtrl(I2C_TypeDef *I2Cx, FunctionalState NewState);

/**
 * @brief Configure FIFO Threshold Level
 * @param[in] I2Cx: where x can be 1 or 2 to select the I2C peripheral.
 * @param[in] TxFifoThresHoldLevel: Tx FIFO Threshold Level. \n
 * Transmit FIFO Threshold Level Controls the level of entries (or below) \n
 * that trigger the TX_EMPTY interrupt (bit 4 in  RAW_INTR_STAT register). \n
 * The valid range is 0-255, with the additional restriction \n 
 * that it may not be set to value larger than the depth of the buffer. \n 
 * If an attempt is made to do that, the actual value set will be the maximum depth of the buffer. \n 
 * A value of 0 sets the threshold for 0 entries, and a value of 255 sets the threshold for 255 entries.
 * @param[in] RxFifoThresHoldLevel: Rx FIFO Threshold Level. \n
 * Receive FIFO Threshold Level Controls the level of entries (or above) \n 
 * that triggers the RX_FULL interrupt (bit 2 in  RAW_INTR_STAT register). \n 
 * The valid range is 0-255, with the additional restriction \n 
 * that hardware does not allow this value to be set to a value larger than the depth of the buffer. \n 
 * If an attempt is made to do that, the actual value set will be the maximum depth of the buffer. \n 
 * A value of 0 sets the threshold for 1 entry, and a value of 255 sets the threshold for 256 entries.
 *  
 */
void I2C_FifoThresHoldLevelConfig(I2C_TypeDef *I2Cx, uint8_t TxFifoThresHoldLevel, uint8_t RxFifoThresHoldLevel);

/**
 * @brief Configure Stop Interrupt Condition
 * @param[in] I2Cx: where x can be 1 or 2 to select the I2C peripheral.
 * @param[in] StopCondition: Specifies stop condition
 *  
 */
void I2C_StopInterruptConfig(I2C_TypeDef *I2Cx, uint32_t StopCondition);

/**
 * @brief Enables or disables the specified I2C interrupts.
 * @param[in] I2Cx: where x can be 1 or 2 to select the I2C peripheral.
 * @param[in] I2C_IT: specifies the I2C interrupts sources to be enabled or disabled. \n
 *   This parameter can be any combination of the following values: \n
 *       I2C_IT_RXUNDER: rx fifo under intrrrupt. \n
 *       I2C_IT_RXOVER: rx fifo over intrrrupt. \n
 *       I2C_IT_RXFULL: rx fifo full intrrrupt. \n
 *       I2C_IT_TXOVER: tx fifo over intrrrupt. \n
 *       I2C_IT_TXEMPTY: tx fifo empty intrrrupt. \n
 *       I2C_IT_RDREQ: read request intrrrupt. \n
 *       I2C_IT_TXABRT: tx abort intrrrupt. \n
 *       I2C_IT_RXDONE: rx done intrrrupt. \n
 *       I2C_IT_ACTIVITY: bus activity intrrrupt. \n
 *       I2C_IT_STOP: stop signal intrrrupt. \n
 *       I2C_IT_START: start or restart signal intrrrupt. \n
 *       I2C_IT_GENCALL: general call intrrrupt. \n
 *       I2C_IT_RESTART: restart signal intrrrupt as addressed slave. \n
 *       I2C_IT_MSTONHOLD: master hold bus intrrrupt. \n
 *       I2C_IT_SCLSTUCKATLOW: lines stuck at low interrupt. \n
 *       I2C_IT_ALL: Specifies all interrupt.
 * @param NewState: new state of the specified I2C interrupts. \n
 *   This parameter can be: ENABLE or DISABLE.
 *  
 */
void I2C_ITConfig(I2C_TypeDef *I2Cx, uint32_t I2C_IT, FunctionalState NewState);

/**
 * @brief Checks whether the specified I2C interrupt has occurred or not.
 * @param[in] I2Cx: where x can be 1 or 2 to select the I2C peripheral.
 * @param[in] I2C_IT: specifies the interrupt source to check. 
 *  This parameter can be any combination of the following values: \n
 *       I2C_IT_RXUNDER: rx fifo under intrrrupt. \n
 *       I2C_IT_RXOVER: rx fifo over intrrrupt. \n
 *       I2C_IT_RXFULL: rx fifo full intrrrupt. \n
 *       I2C_IT_TXOVER: tx fifo over intrrrupt. \n
 *       I2C_IT_TXEMPTY: tx fifo empty intrrrupt. \n
 *       I2C_IT_RDREQ: read request intrrrupt. \n
 *       I2C_IT_TXABRT: tx abort intrrrupt. \n
 *       I2C_IT_RXDONE: rx done intrrrupt. \n
 *       I2C_IT_ACTIVITY: bus activity intrrrupt. \n
 *       I2C_IT_STOP: stop signal intrrrupt. \n
 *       I2C_IT_START: start or restart signal intrrrupt. \n
 *       I2C_IT_GENCALL: general call intrrrupt. \n
 *       I2C_IT_RESTART: restart signal intrrrupt as addressed slave. \n
 *       I2C_IT_MSTONHOLD: master hold bus intrrrupt. \n
 *       I2C_IT_SCLSTUCKATLOW: lines stuck at low interrupt. \n
 *       I2C_IT_ALL: Specifies all interrupt.
 * @return  ITStatus: The new state of I2C_IT (SET or RESET).
 */
ITStatus I2C_GetITStatus(I2C_TypeDef *I2Cx, uint32_t I2C_IT);

/**
 * @brief Clears the I2Cx’s interrupt pending bits.
 * @param[in] I2Cx: where x can be 1 or 2 to select the I2C peripheral.
 * @param[in] I2C_IT: specifies the interrupt pending bit to clear. \n
 *  This parameter can be any combination of the following values: \n
 *       I2C_IT_RXUNDER: rx fifo under intrrrupt. \n
 *       I2C_IT_RXOVER: rx fifo over intrrrupt. \n
 *       I2C_IT_RXFULL: rx fifo full intrrrupt. \n
 *       I2C_IT_TXOVER: tx fifo over intrrrupt. \n
 *       I2C_IT_TXEMPTY: tx fifo empty intrrrupt. \n
 *       I2C_IT_RDREQ: read request intrrrupt. \n
 *       I2C_IT_TXABRT: tx abort intrrrupt. \n
 *       I2C_IT_RXDONE: rx done intrrrupt. \n
 *       I2C_IT_ACTIVITY: bus activity intrrrupt. \n
 *       I2C_IT_STOP: stop signal intrrrupt. \n
 *       I2C_IT_START: start or restart signal intrrrupt. \n
 *       I2C_IT_GENCALL: general call intrrrupt. \n
 *       I2C_IT_RESTART: restart signal intrrrupt as addressed slave. \n
 *       I2C_IT_MSTONHOLD: master hold bus intrrrupt. \n
 *       I2C_IT_SCLSTUCKATLOW: lines stuck at low interrupt. \n
 *       I2C_IT_ALL: Specifies all interrupt.
 *   
 */
void I2C_ClearITPendingBit(I2C_TypeDef *I2Cx, uint32_t I2C_IT);

/**
 * @brief Sends a data byte through the I2Cx peripheral.
 * @param[in] I2Cx: where x can be 1 or 2 to select the I2C peripheral.
 * @param[in] Data: Byte to be transmitted.
 *  
 */
void I2C_SendData(I2C_TypeDef *I2Cx, uint8_t Data);

/**
 * @brief Sends a data byte through the I2Cx peripheral.
 * @param[in] I2Cx: where x can be 1 or 2 to select the I2C peripheral. 
 * @param[in] Data: The last Byte to be transmitted.
 *  
 */
void I2C_SendlastData(I2C_TypeDef *I2Cx, uint8_t Data);

/**
 * @brief Sends some bytes through the I2Cx peripheral.
 * @param[in] I2Cx: where x can be 1 or 2 to select the I2C peripheral.
 * @param[in] Data: Byte to be transmitted.
 * @param[in] Length the length of data 
 *  
 */
void I2C_MasterPollSend(I2C_TypeDef *I2Cx, uint8_t *Data, uint32_t Length);

/**
 * @brief Returns the most recent received data by the I2Cx peripheral.
 * @param[in] I2Cx: where x can be 1 or 2 to select the I2C peripheral.
 * @return  uint8_t: The value of the received data.
 */
uint8_t I2C_ReceiveData(I2C_TypeDef *I2Cx);

/**
 * @brief Returns the most recent received data by the I2Cx peripheral.
 * @param[in] I2Cx: where x can be 1 or 2 to select the I2C peripheral.
 * @return  uint8_t: The value of the received data.
 */
uint8_t I2C_ReceiveLastData(I2C_TypeDef *I2Cx);

/**
 * @brief Returns the most recent received data by the I2Cx peripheral when I2C as master.
 * @param[in] I2Cx: where x can be 1 or 2 to select the I2C peripheral.
 * @param[in] Data: specifies the data which will be received.
 * @param[in] length: specifies the length of data.
 *  
 */
void I2C_MasterPollRecv(I2C_TypeDef *I2Cx, uint8_t *Data, uint32_t Length);

/**
 * @brief Returns the most recent received data by the I2Cx peripheral when I2C as slave.
 * @param[in] I2Cx: where x can be 1 or 2 to select the I2C peripheral.
 * @param[in] Data: specifies the data which will be received.
 * @param[in] length: specifies the length of data.
 *  
 */
void I2C_SlavePollRecv(I2C_TypeDef *I2Cx, uint8_t *Data, uint32_t Length);

/**
 * @brief Configure Restart enable or disable.
 * @param[in] I2Cx: where x can be 1 or 2 to select the I2C peripheral.
 * @param[in] NewState: new state of the Restart state.
 *  
 */
void I2C_RestartConfig(I2C_TypeDef *I2Cx, FunctionalState NewState);

/**
 * @brief Configure Restart enable or disable.
 * @param[in] I2Cx: where x can be 1 or 2 to select the I2C peripheral.
 * @param[in]  SetupTime: SDA Setup Time.
 *  
 */
void I2C_SDASetupTimeConfigure(I2C_TypeDef *I2Cx, uint8_t SetupTime);

/**
 * @brief Configure Restart enable or disable.
 * @param[in] I2Cx: where x can be 1 or 2 to select the I2C peripheral.
 * @param[in] HoldTime: SDA Transmit Hold Time.
 *  
 */
void I2C_TxSDAHoldTimeConfigure(I2C_TypeDef *I2Cx, uint8_t HoldTime);

/**
 * @brief Configure SDA Rx Hold Time.
 * @param[in] I2Cx: where x can be 1 or 2 to select the I2C peripheral.
 * @param[in] HoldTime: SDA Receive Hold Time.
 *  
 */
void II2C_RxSDAHoldTimeConfigure(I2C_TypeDef *I2Cx, uint8_t HoldTime);

/**
 * @brief Get Tx Abort Source Status.
 * @param[in] I2Cx: where x can be 1 or 2 to select the I2C peripheral.
 * @param[in] TxAbort: specifies the I2C Tx Abort Source. \n
 *    This parameter can be one of the following values: \n
 *       I2C_TX_ABRT_DEVICE_WRITE: master transfer device id. \n
 *       I2C_TX_ABRT_DEVICE_SLVADDR_NOACK: master transfer device id and no slave acknowledged it. \n
 *       I2C_TX_ABRT_DEVICE_NOACK: master transfer device id and no slave acknowledged the id. \n
 *       I2C_TX_ABRT_SDA_STUCK_AT_LOW: SDA stuck at low. \n
 *       I2C_TX_ABRT_USER_ABRT: master detecte the transfer abort. \n
 *       I2C_TX_ABRT_SLVRD_INTX: user write a 1(read) in CMD when module acknowledged slave transmit request. \n
 *       I2C_TX_ABRT_SLV_ARBLOST: slave lost bus while transmitting data to a remote master. \n
 *       I2C_TX_ABRT_SLVFLUSH_TXFIFO: slave flush old tx fifo. \n
 *       I2C_TX_ARBT_LOST: I2C module has lost arbitration. \n
 *       I2C_TX_ABRT_MASTER_DIS: User tries to initiate a Master operation with the Master mode disabled. \n
 *       I2C_TX_ABRT_10B_RD_NORSTRT: The restart is disabled and the master sends a read command in 10-bit addressing mode. \n
 *       I2C_TX_ABRT_SBYTE_NORSTRT: The restart is disabled and the user is trying to send a START Byte. \n
 *       I2C_TX_ABRT_HS_NORSTRT: The restart is disabled and the user is trying to use the master to transfer data in High Speed mode. \n
 *       I2C_TX_ABRT_SBYTE_ACKDET: Master has sent a START Byte and the START Byte was acknowledged. \n
 *       I2C_TX_ABRT_HS_ACKDET: Master is in High Speed mode and the High Speed Master code was acknowledged. \n
 *       I2C_TX_ABRT_GCALL_READ: I2C in master mode sent a General Call but the user programmed the byte following the General Call to be a read from the bus is set to 1. \n
 *       I2C_TX_ABRT_GCALL_NOACK: I2C in master mode sent a General Call and no slave on the bus acknowledged the General Call. \n
 *       I2C_TX_ABRT_TXDATA_NOACK: This is a master-mode only bit. Master has received an acknowledgement for the address, but when it sent data byte(s) following the address, it did not receive an acknowledge from the remote slave(s). \n
 *       I2C_TX_ABRT_10ADDR2_NOACK: Master is in 10-bit address mode and the second address byte of the 10-bit address was not acknowledged by any slave. \n
 *       I2C_TX_ABRT_10ADDR1_NOACK: Master is in 7-bit addressing mode and the address sent was not acknowledged by any slave. \n
 *       I2C_TX_ABRT_7B_ADDR_NOACK: Master is in 7-bit addressing mode and the address sent was not acknowledged by any slave.
 * @return  I2C_TXABORT_STATUS The new state of I2C_TxAbort_Status (SET or RESET).
 * 
 */
I2C_TxAbort_Status I2C_GetTxAbortSource(I2C_TypeDef *I2Cx, uint32_t TxAbort);

/**
 * @brief Checks whether the specified I2C Enable Status has occurred or not.
 * @param[in] I2Cx: where x can be 1 or 2 to select the I2C peripheral.
 * @param[in] EnableStatus: specifies the status to check. \n 
 * This parameter can be any combination of the following values: \n
 *    I2C_SLV_RX_DATA_LOST: Slave Received Data Lost. \n
 *    I2C_SLV_DISABLED_WHILE_BUSY: Slave Disabled While Busy (Transmit, Receive). \n
 *    I2C_ENABLE: Module Enable.             
 * @return  The new state of I2C_ModuleEnable_Status (SET or RESET).
 */
I2C_ModuleEnable_Status I2C_GetModuleEnableStatus(I2C_TypeDef *I2Cx, uint8_t EnableStatus);

/**
 * @brief Checks whether the specified I2C status has occurred or not.
 * @param[in] I2Cx: where x can be 1 or 2 to select the I2C peripheral.
 * @param[in] I2CStatus: specifies the status source to check. \n
 * This parameter can be one of the following values: \n
 *       I2C_STATUS_ACTIVITY: I2C Activity Status. \n
 *       I2C_STATUS_TFNF: Transmit FIFO Not Full. \n
 *       I2C_STATUS_TFE: Transmit FIFO Completely Empty. \n
 *       I2C_STATUS_RFNE: Receive FIFO Not Empty. \n
 *       I2C_STATUS_RFF: Receive FIFO Completely Full. \n
 *       I2C_STATUS_MST_ACTIVITY: Master FSM Activity Status. \n
 *       I2C_STATUS_SLV_ACTIVITY: Slave FSM Activity Status. \n
 *       I2C_STATUS_MST_HOLD_TX_FIFO_EMPTY: The BUS hold when the master holds the bus  \n
 *     because of the Tx FIFO being empty, and the the previous transferred command does not have the Stop bit set. \n 
 *       I2C_STATUS_MST_HOLD_RX_FIFO_FULL: The BUS Hold in Master mode due to Rx FIFO is Full and additional byte has been received. \n
 *       I2C_STATUS_SLV_HOLD_TX_FIFO_EMPTY:  The BUS Hold in Slave mode for the Read request when the Tx FIFO is empty. \n 
 *     The Bus is in hold until the Tx FIFO has data to Transmit for the read request. \n
 *       I2C_STATUS_SLV_HOLD_RX_FIFO_FULL:  The BUS Hold in Slave mode due to the Rx FIFO being Full and an additional byte being received.
 * @return  The new state of I2C_BusStatus (SET or RESET).
 */
I2C_BusStatus I2C_GetBusStatus(I2C_TypeDef *I2Cx, uint32_t I2CStatus);

/**
 * @brief Transmit FIFO Leve.
 * @param[in] I2Cx: where x can be 1 or 2 to select the I2C peripheral.
 * @return  uint8_t: the number of valid data entries in the transmit FIFO.
 */
uint8_t I2C_GetTxFifoVaildDataLevel(I2C_TypeDef *I2Cx);

/**
 * @brief Receive FIFO Level.
 * @param[in] I2Cx: where x can be 1 or 2 to select the I2C peripheral.
 * @return   uint8_t: the number of valid data entries in the receive FIFO.
 */
uint8_t I2C_GetRxFifoVaildDataLevel(I2C_TypeDef *I2Cx);

/**
 * @brief This function indicates the number of Tx FIFO data commands that are flushed due to TX_ABRT interrupt. 
 * It is cleared whenever I2C is disabled.
 * @param[in] I2Cx: where x can be 1 or 2 to select the I2C peripheral.
 * @return   uint16_t: the number of Tx FIFO data commands that are flushed due to TX_ABRT interrupt.
 */
uint16_t I2C_GetTxflushCnt(I2C_TypeDef *I2Cx);

#ifdef __cplusplus
}
#endif

#endif