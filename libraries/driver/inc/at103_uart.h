/**
 * @file at103_uart.h
 * @brief This file contains all the functions prototypes for the USART firmware library.
 *        How to use this driver? \n
 * (+) At first, use USART_DeInit(...) to reset requires USARTx. \n
 * (+) Then, use USART_Init(...) with USART Init Structure to initialize USARTx.
 *     For details on the USART Init Structure definition, see "USART_InitTypeDef". \n
 * (+) Finally, use USART_ITConfig(...) to enable USARTx Receive and Transmit interrupts, and you need to implement the corresponding USARTx service function: USARTx_IRQHandler.
 *     For details, you can see USART_IER register. \n
 * (+) Of course, you can use USART_GetLineFlagStatus(...) to get the state of the USARTx at any time, this is helpful in determining whether the specified USART flag is set or not.
 * @author zhangsheng (zhangsheng@zhangsheng.ic.com)
 * @version 1.0
 * @date 2022-01-27
 * @copyright Copyright (c) 2022 Icore, Inc
 */
#ifndef __AT103_UART_H
#define __AT103_UART_H

#ifdef __cplusplus
extern "C" {
#endif

#include "at103.h"

/**
 * @brief USART Init Structure definition  
 */
typedef struct
{
    uint32_t USART_BaudRate;            /**< This member configures the USART communication baud rate. */
    uint16_t USART_WordLength;          /**< Specifies the number of data bits transmitted or received in a frame. */
    uint16_t USART_StopBits;            /**< Specifies the number of stop bits transmitted. */
    uint16_t USART_Parity;              /**< Specifies the parity mode. */
    uint16_t USART_Mode;                /**< Specifies wether the Receive or Transmit mode is enabled or disabled. */
    uint16_t USART_HardwareFlowControl; /**< Specifies wether the hardware flow control mode is enabled or disabled. */
} USART_InitTypeDef;

/**
 * @brief USART_Interrupt_Type
 */
typedef enum {
    MODEM_STATUS      = 0x00, /**< Interrupt ID: Modem status. */
    NO_PENDING        = 0x01, /**< Interrupt ID: no interrupt pending. */
    THR_EMPTY         = 0x02, /**< Interrupt ID: THR empty. */
    RCV_AVALIABLE     = 0x04, /**< Interrupt ID: received data available. */
    RCV_LINE_STATUS   = 0x06, /**< Interrupt ID: receiver line status. */
    BUSY_DETECT       = 0x07, /**< Interrupt ID: busy detect. */
    CHARACTER_TIMEOUT = 0x0c  /**< Interrupt ID: character timeout. */
} USART_InterruptID;

#define LCR_DLAB                ((uint16_t)0x0080) /**< LCR register Divisor Latch Access Bit. */
#define IIR_FCR_IIR_FIFO_ENABLE ((uint16_t)0x00C0) /**< IIR register FIFOs Enabled (or FIFOSE). */
#define IIR_FCR_FCR_FIFOE       ((uint16_t)0x0001) /**< FCR register FIFO Enable (or FIFOE). */

#define USART_FIFO_DISABLES ((uint16_t)0x00000000) /**< USART FIFO STATE: DISABLE. */
#define USART_FIFO_ENABLES  ((uint16_t)0x000000C0) /**< USART FIFO STATE: ENABLE. */

#define USART_WordLength_5b ((uint16_t)0x0000) /**< USART Word Length: 5 data bits per character. */
#define USART_WordLength_6b ((uint16_t)0x0001) /**< USART Word Length: 6 data bits per character. */
#define USART_WordLength_7b ((uint16_t)0x0002) /**< USART Word Length: 7 data bits per character. */
#define USART_WordLength_8b ((uint16_t)0x0003) /**< USART Word Length: 8 data bits per character. */

#define USART_StopBits_1   ((uint8_t)0x00) /**< USART Stop Bits: 1 stop bit. */
#define USART_StopBits_1_5 ((uint8_t)0x04) /**< USART Stop Bits: 1.5 stop bits. */
#define USART_StopBits_2   ((uint8_t)0x04) /**< USART Stop Bits: 2 stop bits. */

#define USART_Parity_No   ((uint8_t)0x00) /**< USART Parity: No Parity. */
#define USART_Parity_Even ((uint8_t)0x18) /**< USART Parity: Even Parity. */
#define USART_Parity_Odd  ((uint8_t)0x08) /**< USART Parity: Odd Parity. */

#define USART_HardwareFlowControl_None    ((uint16_t)0x00000) /**< USART Hardware Flow Control: None. */
#define USART_HardwareFlowControl_RTS_CTS ((uint16_t)0x00022) /**< USART Hardware Flow Control: RTS_CTS. */

#define USART_IT_IER_RDI  0x01 /**< Receiver data interrupt. */
#define USART_IT_IER_THRI 0x02 /**< Transmitter holding register int. */
#define USART_IT_IER_RLSI 0x04 /**< Receiver line status interrupt. */
#define USART_IT_IER_MSI  0x08 /**< Modem status interrupt. */

#define USART_FLAG_IIR_MSI  0x00 /**< Modem status interrupt. */
#define USART_FLAG_IIR_THRI 0x02 /**< Transmitter holding register empty. */
#define USART_FLAG_IIR_RDI  0x04 /**< Receiver data interrupt. */
#define USART_FLAG_IIR_CHTO 0x0C /**< Receiver data interrupt. */
#define USART_FLAG_IIR_RLSI 0x06 /**< Receiver line status interrupt. */

#define USART_FLAG_LSR_DR   0x0001 /**< Data ready. */
#define USART_FLAG_LSR_OE   0x0002 /**< Overrun. */
#define USART_FLAG_LSR_PE   0x0004 /**< Parity error. */
#define USART_FLAG_LSR_FE   0x0008 /**< Framing error. */
#define USART_FLAG_LSR_BI   0x0010 /**< Break. */
#define USART_FLAG_LSR_THRE 0x0020 /**< Xmit holding register empty. */
#define USART_FLAG_LSR_TEMT 0x0040 /**< Xmitter empty. */
#define USART_FLAG_LSR_ERR  0x0080 /**< Error. */

#define USART_FLAG_MSR_DCD  0x80 /**< Data Carrier Detect. */
#define USART_FLAG_MSR_RI   0x40 /**< Ring Indicator. */
#define USART_FLAG_MSR_DSR  0x20 /**< Data Set Ready. */
#define USART_FLAG_MSR_CTS  0x10 /**< Clear to Send. */
#define USART_FLAG_MSR_DDCD 0x08 /**< Delta DCD. */
#define USART_FLAG_MSR_TERI 0x04 /**< Trailing edge ring indicator. */
#define USART_FLAG_MSR_DDSR 0x02 /**< Delta DSR. */
#define USART_FLAG_MSR_DCTS 0x01 /**< Delta CTS. */

#define USART_DMA_Mode0 0x00000000 /**< USART DMA Mode0. */
#define USART_DMA_Mode1 0x00000008 /**< USART DMA Mode1. */

#define USART_DMAReq_Tx ((uint16_t)0x00000004) /**< USART DMA TX Requests. */
#define USART_DMAReq_Rx ((uint16_t)0x00000002) /**< USART DMA RX Requests. */

#define USART_IrDAMode_LowPower ((uint16_t)0x00FF) /**< USART IrDA Mode LowPower. */
#define USART_IrDAMode_Normal   ((uint16_t)0x0000) /**< USART IrDA Mode Normal. */

#define USART_FIFO_FCR_FIFIOE 0x01 /**< FIFO Enable (or FIFOE). */
#define USART_FIFO_FCR_RFIFOR 0x02 /**< RCVR FIFO Reset (or RFIFOR). */
#define USART_FIFO_FCR_XFIFOR 0x04 /**< XMIT FIFO Reset (or XFIFOR). */
#define USART_FIFO_FCR_DMAM   0x08 /**< DMA Mode (or DMAM). */
#define USART_FIFO_FCR_TET    0x30 /**< TX Empty Trigger (or TET). */
#define USART_FIFO_FCR_RT     0xC0 /**< RCVR Trigger (or RT). */

#define USART_Modem_MCR_DTR      0x01 /**< Data Terminal Ready. */
#define USART_Modem_MCR_RTS      0x02 /**< Request to Send. */
#define USART_Modem_MCR_OUT1     0x04 /**< OUT1. */
#define USART_Modem_MCR_OUT2     0x08 /**< OUT2. */
#define USART_Modem_MCR_LoopBack 0x10 /**< LoopBack Bit. */
#define USART_Modem_MCR_AFCE     0x20 /**< Auto Flow Control Enable. */
#define USART_Modem_MCR_SIRE     0x40 /**< SIR Mode Enable. */

/**
 * @brief Deinitializes the USARTx peripheral registers to their default reset values.
 * @param[in] USARTx Select the USART or the USART peripheral. 
 */
void USART_DeInit(USART_TypeDef *USARTx);

/**
 * @brief Initializes the USARTx peripheral according to the specified
 *        parameters in the USART_InitStruct .
 * @param[in] USARTx Select the USART or the UART peripheral. 
 * @param[in] USART_InitStruct pointer to a USART_InitTypeDef structure
 *            that contains the configuration information for the specified USART peripheral.
 */
void USART_Init(USART_TypeDef *USARTx, USART_InitTypeDef *USART_InitStruct);

/**
 * @brief Fills each USART_InitStruct member with its default value.
 * @param[in] USART_InitStruct pointer to a USART_InitTypeDef structure
 *            which will be initialized.
 */
void USART_StructInit(USART_InitTypeDef *USART_InitStruct);

/**
 * @brief Enables or disables the specified USART interrupts.
 * @param[in] USARTx Select the USART or the UART peripheral. 
 * @param[in] USART_IT specifies the USART interrupt sources to be enabled or disabled.
 * @param[in] NewState new state of the specified USARTx interrupts.
 */
void USART_ITConfig(USART_TypeDef *USARTx, uint16_t USART_IT, FunctionalState NewState);

/**
 * @brief Enables or disables the USART DMA interface.
 * @param[in] USARTx Select the USART or the UART peripheral. 
 * @param[in] USART_DMAReq specifies the DMA request.
 * @param[in] NewState new state of the DMA Request sources.
 */
void USART_DMACmd(USART_TypeDef *USARTx, uint16_t USART_DMAReq, FunctionalState NewState);

/**
 * @brief Enables or disables the USART FIFO interface.
 * @param[in] USARTx Select the USART peripheral.
 * @param[in] NewState new state of the USART FIFO. 
 */
void USART_FIFOCmd(USART_TypeDef *USARTx, FunctionalState NewState);

/**
 * @brief Enables or disables the FIFO mode.
 * @param[in] USARTx Select the USART or the UART peripheral.  
 * @param[in] USART_FIFO FIFO mode.
 */
void USART_FIFOConfig(USART_TypeDef *USARTx, uint16_t USART_FIFO);

/**
 * @brief Sets the address of the USART node.
 * @param[in] USARTx Select the USART or the UART peripheral. 
 * @param[in] USART_Address Indicates the address of the USART node.
 */
void USART_SetAddress(USART_TypeDef *USARTx, uint8_t USART_Address);

/**
 * @brief Transmits single data through the USARTx peripheral.
 * @param[in] USARTx Select the USART or the UART peripheral. 
 * @param[in] Data the data to transmit.
 */
void USART_SendData(USART_TypeDef *USARTx, uint16_t Data);

/**
 * @brief Configures the USART's IrDA interface.
 * @param[in] USARTx Select the USART or the UART peripheral. 
 * @param[in] USART_IrDAMode specifies the IrDA mode.
 */
void USART_IrDAConfig(USART_TypeDef *USARTx, uint16_t USART_IrDAMode);

/**
 * @brief Enables or disables the USART's IrDA interface.
 * @param[in] USARTx Select the USART or the UART peripheral. 
 * @param[in] NewState new state of the IrDA mode.
 */
void USART_IrDACmd(USART_TypeDef *USARTx, FunctionalState NewState);

/**
 * @brief Transmits break characters.
 * @param[in] USARTx Select the USART or the UART peripheral. 
 */
void USART_SendBreak(USART_TypeDef *USARTx);

/**
 * @brief Get the Specified USART interrupt ID.
 * @param[in] USARTx Select the USART or the UART peripheral. 
 * @return Interrupt ID.  
 */
USART_InterruptID USART_GetInterruptID(USART_TypeDef *USARTx);

/**
 * @brief Returns the most recent received data by the USARTx peripheral.
 * @param[in] USARTx Select the USART or the UART peripheral. 
 * @return The received data.
 */
uint16_t USART_ReceiveData(USART_TypeDef *USARTx);

/**
 * @brief Checks whether the specified USART Line flag is set or not.
 * @param[in] USARTx Select the USART or the UART peripheral.
 * @param[in] USART_FLAG specifies the flag to check.
 * @return The new state of USART_FLAG (SET or RESET). 
 */
FlagStatus USART_GetLineFlagStatus(USART_TypeDef *USARTx, uint16_t USART_FLAG);

/**
 * @brief Checks whether the specified USART Modem flag is set or not.
 * @param[in] USARTx Select the USART or the UART peripheral.
 * @param[in] USART_FLAG specifies the flag to check.
 * @return The new state of USART_FLAG (SET or RESET).  
 */
FlagStatus USART_GetModemFlagStatus(USART_TypeDef *USARTx, uint16_t USART_FLAG);

/**
 * @brief Clears the USARTx's pending flags.
 * @param[in] USARTx Select the USART or the UART peripheral.
 * @param[in] USART_FLAG specifies the flag to clear.
 */
void USART_ClearFlag(USART_TypeDef *USARTx, uint16_t USART_FLAG);

/**
 * @brief Checks whether the specified USART interrupt has occurred or not. 
 * @param[in] USARTx Select the USART or the UART peripheral.
 * @param[in] USART_IT specifies the USART interrupt source to check.
 * @return The new state of USART_IT (SET or RESET).  
 */
ITStatus USART_GetITStatus(USART_TypeDef *USARTx, uint16_t USART_IT);

/**
 * @brief Clears the USARTx's interrupt pending bits.
 * @param[in] USARTx Select the USART or the UART peripheral. 
 * @param[in] USART_IT specifies the interrupt pending bit to clear.
 */
void USART_ClearITPendingBit(USART_TypeDef *USARTx, uint16_t USART_IT);

#ifdef __cplusplus
}
#endif

#endif