/**
 * @file at103_it.c
 * @brief This file provides template for all exceptions handler
 *        and peripherals interrupt service routine.
 * @author liyouchaung (lycdemeng@163.com)
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
void USART1_IRQHandler(void)
{
    USART_InterruptID ID = NO_PENDING;

    ID = USART_GetInterruptID(USART1);

    switch (ID) {

    case THR_EMPTY:

        if (USART_GetLineFlagStatus(USART1, USART_FLAG_LSR_THRE) != RESET) {

            /* Write one byte to the transmit data register */
            USART_SendData(USART1, TxBuffer1[TxCounter1++]);
            if (TxCounter1 == NbrOfDataToTransfer1) {
                /* Disable the USART1 Transmit interrupt */
                USART_ITConfig(USART1, USART_IT_IER_THRI, DISABLE);
            }
        }
        break;
    case CHARACTER_TIMEOUT:
    case RCV_AVALIABLE:

        if (USART_GetLineFlagStatus(USART1, USART_FLAG_LSR_DR) != RESET) {
            /* Read one byte from the receive data register */

            RxBuffer1[RxCounter1++] = USART_ReceiveData(USART1);

            if (RxCounter1 == NbrOfDataToRead1) {
                /* Disable the USART1 Receive interrupt */
                USART_ITConfig(USART1, USART_IT_IER_RDI, DISABLE);
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
void USART3_IRQHandler(void)
{
    USART_InterruptID ID = NO_PENDING;

    ID = USART_GetInterruptID(USART3);

    switch (ID) {

    case THR_EMPTY:

        if (USART_GetLineFlagStatus(USART3, USART_FLAG_LSR_THRE) != RESET) {
            /* Write one byte to the transmit data register */
            USART_SendData(USART3, TxBuffer2[TxCounter2++]);

            if (TxCounter2 == NbrOfDataToTransfer2) {
                /* Disable the USART3 Transmit interrupt */
                USART_ITConfig(USART3, USART_IT_IER_THRI, DISABLE);
            }
        }

        break;
    case CHARACTER_TIMEOUT:
    case RCV_AVALIABLE:

        if (USART_GetLineFlagStatus(USART3, USART_FLAG_LSR_DR) != RESET) {
            /* Read one byte from the receive data register */
            RxBuffer2[RxCounter2++] = USART_ReceiveData(USART3);

            if (RxCounter2 == NbrOfDataToRead1) {
                /* Disable the USART3 Receive interrupt */
                USART_ITConfig(USART3, USART_IT_IER_RDI, DISABLE);
            }
        }
        break;
    default:

        break;
    }
}
