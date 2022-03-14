/**
 * @file at103_it.c
 * @brief This file provides template for all exceptions handler
 *        and peripherals interrupt service routine.
 * @author zhangsheng (zhangsheng@zhangsheng.ic.com)
 * @version 1.0
 * @date 2022-02-13
 * @copyright Copyright (c) 2022 Icore, Inc
 */

#include "at103_it.h"

/* Private variables ---------------------------------------------------------*/
extern uint8_t      TxBuffer1[];
extern uint8_t      TxBuffer2[];
extern uint8_t      RxBuffer1[];
extern uint8_t      RxBuffer2[];
extern __IO uint8_t TxCounter1;
extern __IO uint8_t TxCounter2;
extern __IO uint8_t RxCounter1;
extern __IO uint8_t RxCounter2;
extern uint8_t      NbrOfDataToTransfer1;
extern uint8_t      NbrOfDataToTransfer2;
extern uint8_t      NbrOfDataToRead1;
extern uint8_t      NbrOfDataToRead2;

/**
 * @brief This function handles USARTy global interrupt request.
 */
void USARTy_IRQHandler(void)
{
    USART_InterruptID ID = NO_PENDING;

    ID = USART_GetInterruptID(USARTy);

    switch (ID) {

    case THR_EMPTY:

        if (USART_GetLineFlagStatus(USARTy, USART_FLAG_LSR_THRE) != RESET) {

            /* Write one byte to the transmit data register */
            USART_SendData(USARTy, TxBuffer1[TxCounter1++]);
            if (TxCounter1 == NbrOfDataToTransfer1) {
                /* Disable the USARTy Transmit interrupt */
                USART_ITConfig(USARTy, USART_IT_IER_THRI, DISABLE);
            }
        }
        break;
    case CHARACTER_TIMEOUT:
    case RCV_AVALIABLE:

        if (USART_GetLineFlagStatus(USARTy, USART_FLAG_LSR_DR) != RESET) {
            /* Read one byte from the receive data register */

            RxBuffer1[RxCounter1++] = USART_ReceiveData(USARTy);

            if (RxCounter1 == NbrOfDataToRead1) {
                /* Disable the USARTy Receive interrupt */
                USART_ITConfig(USARTy, USART_IT_IER_RDI, DISABLE);
            }
        }
        break;
    default:

        break;
    }
}

/**
 * @brief This function handles USARTz global interrupt request.
 */
void USARTz_IRQHandler(void)
{
    USART_InterruptID ID = NO_PENDING;

    ID = USART_GetInterruptID(USARTz);

    switch (ID) {

    case THR_EMPTY:

        if (USART_GetLineFlagStatus(USARTz, USART_FLAG_LSR_THRE) != RESET) {
            /* Write one byte to the transmit data register */
            USART_SendData(USARTz, TxBuffer2[TxCounter2++]);

            if (TxCounter2 == NbrOfDataToTransfer2) {
                /* Disable the USARTz Transmit interrupt */
                USART_ITConfig(USARTz, USART_IT_IER_THRI, DISABLE);
            }
        }

        break;
    case CHARACTER_TIMEOUT:
    case RCV_AVALIABLE:

        if (USART_GetLineFlagStatus(USARTz, USART_FLAG_LSR_DR) != RESET) {
            /* Read one byte from the receive data register */
            RxBuffer2[RxCounter2++] = USART_ReceiveData(USARTz);

            if (RxCounter2 == NbrOfDataToRead1) {
                /* Disable the USARTz Receive interrupt */
                USART_ITConfig(USARTz, USART_IT_IER_RDI, DISABLE);
            }
        }
        break;
    default:

        break;
    }
}

void IllegalInstruction_Handler(void)
{
    while (1) {
        /* error debug */
    }
}

void ECALL_InstructionExecuted_Handler(void)
{
    debug("Software Interrupt !!!\r\n");
    while (1) {
        /* swi */
    }
}
