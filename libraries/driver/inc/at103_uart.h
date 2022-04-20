/**
 * @file at103_uart.h
 * @brief This file contains all the functions prototypes for the USART
 *        firmware library.
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
    uint32_t USART_BaudRate; /**< This member configures the USART communication baud rate. */

    uint16_t USART_WordLength; /**< Specifies the number of data bits transmitted or received in a frame. */

    uint16_t USART_StopBits; /**< Specifies the number of stop bits transmitted. */

    uint16_t USART_Parity; /**< Specifies the parity mode. */

    uint16_t USART_Mode; /**< Specifies wether the Receive or Transmit mode is enabled or disabled. */

    uint16_t USART_HardwareFlowControl; /**< Specifies wether the hardware flow control mode is enabled
                                           or disabled. */
} USART_InitTypeDef;

/**
 * @brief USART_Interrupt_Type
 */
typedef enum {
    MODEM_STATUS      = 0x00,
    NO_PENDING        = 0x01,
    THR_EMPTY         = 0x02,
    RCV_AVALIABLE     = 0x04,
    RCV_LINE_STATUS   = 0x06,
    BUSY_DETECT       = 0x07,
    CHARACTER_TIMEOUT = 0x0c
} USART_InterruptID;

/**
 * @brief USART_FIFO_STATE
 */
#define USART_FIFO_DISABLES       ((uint16_t)0x00000000)
#define USART_FIFO_ENABLES        ((uint16_t)0x000000C0)
#define IS_USART_FIFO_MODE(STATE) (((STATE) == USART_FIFO_DISABLES) || \
                                   ((STATE) == USART_FIFO_ENABLES))

/**
 * @brief USART_Exported_Constants
 */
#define IS_USART_ALL_PERIPH(PERIPH) (((PERIPH) == USART1) || \
                                     ((PERIPH) == USART2) || \
                                     ((PERIPH) == USART3))
/**
 * @brief USART_Word_Length
 */
#define USART_WordLength_5b ((uint16_t)0x0000)
#define USART_WordLength_6b ((uint16_t)0x0001)
#define USART_WordLength_7b ((uint16_t)0x0002)
#define USART_WordLength_8b ((uint16_t)0x0003)

#define IS_USART_WORD_LENGTH(LENGTH) (((LENGTH) == USART_WordLength_5b) || \
                                      ((LENGTH) == USART_WordLength_6b) || \
                                      ((LENGTH) == USART_WordLength_7b) || \
                                      ((LENGTH) == USART_WordLength_8b))

/**
 * @brief USART_Stop_Bits
 */
#define USART_StopBits_1            ((uint8_t)0x00)
#define USART_StopBits_1_5          ((uint8_t)0x04)
#define USART_StopBits_2            ((uint8_t)0x04)
#define IS_USART_STOPBITS(STOPBITS) (((STOPBITS) == USART_StopBits_1) || \
                                     ((STOPBITS) == USART_StopBits_1_5) || \
                                     ((STOPBITS) == USART_StopBits_2))
/**
 * @brief USART_Parity
 */
#define USART_Parity_No         ((uint8_t)0x00)
#define USART_Parity_Even       ((uint8_t)0x18)
#define USART_Parity_Odd        ((uint8_t)0x08)
#define IS_USART_PARITY(PARITY) (((PARITY) == USART_Parity_No) || \
                                 ((PARITY) == USART_Parity_Even) || \
                                 ((PARITY) == USART_Parity_Odd))

/**
 * @brief USART_Hardware_Flow_Control
 */
#define USART_HardwareFlowControl_None    ((uint16_t)0x00000)
#define USART_HardwareFlowControl_RTS_CTS ((uint16_t)0x00022)
#define IS_USART_HARDWARE_FLOW_CONTROL(CONTROL) \
    (((CONTROL) == USART_HardwareFlowControl_None) || \
     ((CONTROL) == USART_HardwareFlowControl_RTS_CTS))

/**
 * @brief USART_Interrupt_definition
 */
#define USART_IT_IER_RDI       0x01 /**< Receiver data interrupt. */
#define USART_IT_IER_THRI      0x02 /**< Transmitter holding register int. */
#define USART_IT_IER_RLSI      0x04 /**< Receiver line status interrupt. */
#define USART_IT_IER_MSI       0x08 /**< Modem status interrupt. */
#define IS_USART_CONFIG_IT(IT) (((IT) == USART_IT_IER_RDI) || ((IT) == USART_IT_IER_THRI) || \
                                ((IT) == USART_IT_IER_RLSI) || ((IT) == USART_IT_IER_MSI))

/**
 * @brief USART_Interrupt_Flags
 */
#define USART_FLAG_IIR_MSI  0x00 /**< Modem status interrupt. */
#define USART_FLAG_IIR_THRI 0x02 /**< Transmitter holding register empty. */
#define USART_FLAG_IIR_RDI  0x04 /**< Receiver data interrupt. */
#define USART_FLAG_IIR_CHTO 0x0C /**< Receiver data interrupt. */
#define USART_FLAG_IIR_RLSI 0x06 /**< Receiver line status interrupt. */
#define IS_USART_FLAG(FLAG) (((FLAG) == USART_FLAG_IIR_MSI) || ((FLAG) == USART_FLAG_IIR_THRI) || \
                             ((FLAG) == USART_FLAG_IIR_RDI) || ((FLAG) == USART_FLAG_IIR_CHTO) || \
                             ((FLAG) == USART_FLAG_IIR_RLSI))

/**
 * @brief USART_Line Status_Flags
 */
#define USART_FLAG_LSR_DR       0x01 /**< Data ready */
#define USART_FLAG_LSR_OE       0x02 /**< Overrun */
#define USART_FLAG_LSR_PE       0x04 /**< Parity error */
#define USART_FLAG_LSR_FE       0x08 /**< Framing error */
#define USART_FLAG_LSR_BI       0x10 /**< Break */
#define USART_FLAG_LSR_THRE     0x20 /**< Xmit holding register empty */
#define USART_FLAG_LSR_TEMT     0x40 /**< Xmitter empty */
#define USART_FLAG_LSR_ERR      0x80 /**< Error */
#define IS_USART_LSR_FLAG(FLAG) (((FLAG) == USART_FLAG_LSR_DR) || ((FLAG) == USART_FLAG_LSR_OE) || \
                                 ((FLAG) == USART_FLAG_LSR_PE) || ((FLAG) == USART_FLAG_LSR_FE) || \
                                 ((FLAG) == USART_FLAG_LSR_BI) || ((FLAG) == USART_FLAG_LSR_THRE) || \
                                 ((FLAG) == USART_FLAG_LSR_TEMT) || ((FLAG) == USART_FLAG_LSR_ERR))

#define IS_USART_CLEAR_FLAG(FLAG) ((((FLAG) & (uint16_t)0x0000) == 0x0000) && ((FLAG) != 0x0000))

/**
 * @brief USART_Modem_Status_Flags
 */
#define USART_FLAG_MSR_DCD      0x80 /**< Data Carrier Detect */
#define USART_FLAG_MSR_RI       0x40 /**< Ring Indicator */
#define USART_FLAG_MSR_DSR      0x20 /**< Data Set Ready */
#define USART_FLAG_MSR_CTS      0x10 /**< Clear to Send */
#define USART_FLAG_MSR_DDCD     0x08 /**< Delta DCD */
#define USART_FLAG_MSR_TERI     0x04 /**< Trailing edge ring indicator */
#define USART_FLAG_MSR_DDSR     0x02 /**< Delta DSR */
#define USART_FLAG_MSR_DCTS     0x01 /**< Delta CTS */
#define IS_USART_MSR_FLAG(FLAG) (((FLAG) == USART_FLAG_MSR_DCD) || ((FLAG) == USART_FLAG_MSR_RI) || \
                                 ((FLAG) == USART_FLAG_MSR_DSR) || ((FLAG) == USART_FLAG_MSR_CTS) || \
                                 ((FLAG) == USART_FLAG_MSR_DDCD) || ((FLAG) == USART_FLAG_MSR_TERI) || \
                                 ((FLAG) == USART_FLAG_MSR_DDSR) || ((FLAG) == USART_FLAG_MSR_DCTS))

#define IS_USART_BAUDRATE(BAUDRATE) (((BAUDRATE) > 0) && ((BAUDRATE) < 0x0044AA21))

/**
 * @brief USART_DMA_Mode
 */
#define USART_DMA_Mode0        0x00000000
#define USART_DMA_Mode1        0x00000008
#define IS_USART_DMAMode(MODE) (((MODE) == USART_DMA_Mode0) || ((MODE) == USART_DMA_Mode1))

/**
 * @brief USART_DMA_Requests 
 */
#define USART_DMAReq_Tx         ((uint16_t)0x00000004)
#define USART_DMAReq_Rx         ((uint16_t)0x00000002)
#define IS_USART_DMAREQ(DMAREQ) ((((DMAREQ) & (uint16_t)0xFFFFFFF9) == 0x00000000) && ((DMAREQ) != (uint16_t)0x00000000))

/**
 * @brief USART_IrDA_Low_Power 
 */
#define USART_IrDAMode_LowPower  ((uint16_t)0x0080)
#define USART_IrDAMode_Normal    ((uint16_t)0x0000)
#define IS_USART_IRDA_MODE(MODE) (((MODE) == USART_IrDAMode_LowPower) || \
                                  ((MODE) == USART_IrDAMode_Normal))

/**
 * @brief USART_FIFO_Control
 */
#define USART_FIFO_FCR_FIFIOE              0x01 /**< FIFO Enable (or FIFOE) */
#define USART_FIFO_FCR_RFIFOR              0x02 /**< RCVR FIFO Reset (or RFIFOR) */
#define USART_FIFO_FCR_XFIFOR              0x04 /**< XMIT FIFO Reset (or XFIFOR) */
#define USART_FIFO_FCR_DMAM                0x08 /**< DMA Mode (or DMAM) */
#define USART_FIFO_FCR_TET                 0x30 /**< TX Empty Trigger (or TET) */
#define USART_FIFO_FCR_RT                  0xC0 /**< RCVR Trigger (or RT) */
#define IS_USART_FIFO_FCR_CONTROL(CONTROL) (((CONTROL) == USART_FIFO_FCR_FIFIOE) || ((CONTROL) == USART_FIFO_FCR_RFIFOR) || \
                                            ((CONTROL) == USART_FIFO_FCR_XFIFOR) || ((CONTROL) == USART_FIFO_FCR_DMAM) || \
                                            ((CONTROL) == USART_FIFO_FCR_TET) || ((CONTROL) == USART_FIFO_FCR_RT))

/**
 * @brief USART_Modem_Control
 */
#define USART_Modem_MCR_DTR                 0x01 /**< Data Terminal Ready */
#define USART_Modem_MCR_RTS                 0x02 /**< Request to Send */
#define USART_Modem_MCR_OUT1                0x04 /**< OUT1 */
#define USART_Modem_MCR_OUT2                0x08 /**< OUT2 */
#define USART_Modem_MCR_LoopBack            0x10 /**< LoopBack Bit */
#define USART_Modem_MCR_AFCE                0x20 /**< Auto Flow Control Enable */
#define USART_Modem_MCR_SIRE                0x40 /**< SIR Mode Enable */
#define IS_USART_Modem_MCR_CONTROL(CONTROL) (((CONTROL) == USART_Modem_MCR_DTR) || ((CONTROL) == USART_Modem_MCR_RTS) || \
                                             ((CONTROL) == USART_Modem_MCR_OUT1) || ((CONTROL) == USART_Modem_MCR_OUT2) || \
                                             ((CONTROL) == USART_Modem_MCR_LoopBack) || ((CONTROL) == USART_Modem_MCR_AFCE) || \
                                             ((CONTROL) == USART_Modem_MCR_SIRE))

/**
 * @brief Deinitializes the USARTx peripheral registers to their default reset values.
 * @param[in] USARTx: Select the USART or the USART peripheral. 
 */
void USART_DeInit(USART_TypeDef *USARTx);

/**
 * @brief Initializes the USARTx peripheral according to the specified
 *        parameters in the USART_InitStruct .
 * @param[in] USARTx: Select the USART or the UART peripheral. 
 * @param[in] USART_InitStruct pointer to a USART_InitTypeDef structure
 *            that contains the configuration information for the specified USART 
 *            peripheral.
 */
void USART_Init(USART_TypeDef *USARTx, USART_InitTypeDef *USART_InitStruct);

/**
 * @brief Fills each USART_InitStruct member with its default value.
 * @param[in] USART_InitStruct: pointer to a USART_InitTypeDef structure
 *            which will be initialized.
 */
void USART_StructInit(USART_InitTypeDef *USART_InitStruct);

/**
 * @brief Enables or disables the specified USART interrupts.
 * @param[in] USARTx: Select the USART or the UART peripheral. 
 * @param[in] USART_IT: specifies the USART interrupt sources to be enabled or disabled.
 * @param[in] NewState: new state of the specified USARTx interrupts.
 */
void USART_ITConfig(USART_TypeDef *USARTx, uint16_t USART_IT, FunctionalState NewState);

/**
 * @brief Enables or disables the USART DMA interface.
 * @param[in] USARTx: Select the USART or the UART peripheral. 
 * @param[in] USART_DMAReq: specifies the DMA request.
 * @param[in] NewState: new state of the DMA Request sources.
 */
void USART_DMACmd(USART_TypeDef *USARTx, uint16_t USART_DMAReq, FunctionalState NewState);

/**
 * @brief Enables or disables the USART FIFO interface.
 * @param[in] USARTx: Select the USART peripheral.
 * @param[in] NewState: new state of the USART FIFO. 
 */
void USART_FIFOCmd(USART_TypeDef *USARTx, FunctionalState NewState);

/**
 * @brief Enables or disables the FIFO mode.
 * @param[in] USARTx: Select the USART or the UART peripheral.  
 * @param[in] USART_FIFO: FIFO mode.
 */
void USART_FIFOConfig(USART_TypeDef *USARTx, uint16_t USART_FIFO);

/**
 * @brief Sets the address of the USART node.
 * @param[in] USARTx: Select the USART or the UART peripheral. 
 * @param[in] USART_Address: Indicates the address of the USART node.
 */
void USART_SetAddress(USART_TypeDef *USARTx, uint8_t USART_Address);

/**
 * @brief Transmits single data through the USARTx peripheral.
 * @param[in] USARTx: Select the USART or the UART peripheral. 
 * @param[in] Data: the data to transmit.
 */
void USART_SendData(USART_TypeDef *USARTx, uint16_t Data);

/**
 * @brief Configures the USART's IrDA interface.
 * @param[in] USARTx: Select the USART or the UART peripheral. 
 * @param[in] USART_IrDAMode: specifies the IrDA mode.
 */
void USART_IrDAConfig(USART_TypeDef *USARTx, uint16_t USART_IrDAMode);

/**
 * @brief Enables or disables the USART's IrDA interface.
 * @param[in] USARTx: Select the USART or the UART peripheral. 
 * @param[in] NewState: new state of the IrDA mode.
 */
void USART_IrDACmd(USART_TypeDef *USARTx, FunctionalState NewState);

/**
 * @brief Transmits break characters.
 * @param[in] USARTx: Select the USART or the UART peripheral. 
 */
void USART_SendBreak(USART_TypeDef *USARTx);

/**
 * @brief Get the Specified USART interrupt ID.
 * @param[in] USARTx: Select the USART or the UART peripheral. 
 * @return Interrupt ID.  
 */
USART_InterruptID USART_GetInterruptID(USART_TypeDef *USARTx);

/**
 * @brief Returns the most recent received data by the USARTx peripheral.
 * @param[in] USARTx: Select the USART or the UART peripheral. 
 * @return The received data.
 */
uint16_t USART_ReceiveData(USART_TypeDef *USARTx);

/**
 * @brief Checks whether the specified USART Line flag is set or not.
 * @param[in] USARTx: Select the USART or the UART peripheral.
 * @param[in] USART_FLAG: specifies the flag to check.
 * @return The new state of USART_FLAG (SET or RESET). 
 */
FlagStatus USART_GetLineFlagStatus(USART_TypeDef *USARTx, uint16_t USART_FLAG);

/**
 * @brief Checks whether the specified USART Modem flag is set or not.
 * @param[in] USARTx: Select the USART or the UART peripheral.
 * @param[in] USART_FLAG: specifies the flag to check.
 * @return The new state of USART_FLAG (SET or RESET).  
 */
FlagStatus USART_GetModemFlagStatus(USART_TypeDef *USARTx, uint16_t USART_FLAG);

/**
 * @brief Clears the USARTx's pending flags.
 * @param[in] USARTx: Select the USART or the UART peripheral.
 * @param[in] USART_FLAG: specifies the flag to clear.
 */
void USART_ClearFlag(USART_TypeDef *USARTx, uint16_t USART_FLAG);

/**
 * @brief Checks whether the specified USART interrupt has occurred or not. 
 * @param[in] USARTx: Select the USART or the UART peripheral.
 * @param[in] USART_IT: specifies the USART interrupt source to check.
 * @return The new state of USART_IT (SET or RESET).  
 */
ITStatus USART_GetITStatus(USART_TypeDef *USARTx, uint16_t USART_IT);

/**
 * @brief Clears the USARTx's interrupt pending bits.
 * @param[in] USARTx: Select the USART or the UART peripheral. 
 * @param[in] USART_IT: specifies the interrupt pending bit to clear.
 */
void USART_ClearITPendingBit(USART_TypeDef *USARTx, uint16_t USART_IT);

#ifdef __cplusplus
}
#endif

#endif