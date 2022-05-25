/**
 * @file      at103_it.c
 * @brief     This file provides template for SPI interrupt service routine.
 * @author    luoming (luoming@i-core.cn)
 * @version   1.0
 * @date      2022-04-15
 * @copyright Copyright (c) 2022 i-core, Inc
 */

#include "at103_it.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define BufferSize 32

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
extern __IO uint8_t spiyTxIdx;
extern __IO uint8_t spiyRxIdx;
extern __IO uint8_t spizTxIdx;
extern __IO uint8_t spizRxIdx;
extern uint8_t      itSPIy_Buffer_Tx[BufferSize];
extern uint8_t      itSPIz_Buffer_Tx[BufferSize];
extern uint8_t      itSPIy_Buffer_Rx[BufferSize], itSPIz_Buffer_Rx[BufferSize];

/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
 * @brief  This function handles SPI1 global interrupt request.
 * @param  None
 * @retval None
 */
void SPIy_IRQHandler(void)
{
    if (SPI_GetITStatus(SPIy, SPI_IT_TXE) != RESET) {
        /* Send SPI_MASTER data */
        SPI_SendData(SPIy, itSPIy_Buffer_Tx[spiyTxIdx++]);
        /* Disable SPI_MASTER TXE interrupt */
        if (spiyTxIdx == BufferSize) {
            SPI_ITConfig(SPIy, SPI_IT_TXE, DISABLE);
        }
    } else if (SPI_GetITStatus(SPIy, SPI_IT_RXNE) != RESET) {
        itSPIy_Buffer_Rx[spiyRxIdx++] = SPI_ReceiveData(SPIy);
    }
    SPI_ClearITPendingBit(SPIy, SPI_IT_ALL);
}

/**
 * @brief  This function handles SPI2 global interrupt request.
 * @param  None
 * @retval None
 */
void SPIz_IRQHandler(void)
{
    if (SPI_GetITStatus(SPIz, SPI_IT_TXE) != RESET) {
        /* Send SPI_MASTER data */
        SPI_SendData(SPIz, itSPIz_Buffer_Tx[spizTxIdx++]);
        /* Disable SPI_MASTER TXE interrupt */
        if (spizTxIdx == BufferSize) {
            SPI_ITConfig(SPIz, SPI_IT_TXE, DISABLE);
        }
    } else if (SPI_GetITStatus(SPIz, SPI_IT_RXNE) != RESET) {
        itSPIz_Buffer_Rx[spizRxIdx++] = SPI_ReceiveData(SPIz);
    }
    SPI_ClearITPendingBit(SPIz, SPI_IT_ALL);
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
