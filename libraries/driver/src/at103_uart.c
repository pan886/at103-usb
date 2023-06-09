/**
 * @file at103_uart.c
 * @brief This file provides all the USART firmware functions.
 * @author zhangsheng (zhangsheng@zhangsheng.ic.com)
 * @version 1.0
 * @date 2022-01-27
 * @copyright Copyright (c) 2022 Icore, Inc
 */

#include "at103_uart.h"
#ifdef USART_MODULE_ENABLED

#define IS_USART_FIFO_MODE(STATE) (((STATE) == USART_FIFO_DISABLES) || \
                                   ((STATE) == USART_FIFO_ENABLES))

#define IS_USART_ALL_PERIPH(PERIPH) (((PERIPH) == USART1) || \
                                     ((PERIPH) == USART2) || \
                                     ((PERIPH) == USART3))

#define IS_USART_WORD_LENGTH(LENGTH) (((LENGTH) == USART_WordLength_5b) || \
                                      ((LENGTH) == USART_WordLength_6b) || \
                                      ((LENGTH) == USART_WordLength_7b) || \
                                      ((LENGTH) == USART_WordLength_8b))

#define IS_USART_STOPBITS(STOPBITS) (((STOPBITS) == USART_StopBits_1) || \
                                     ((STOPBITS) == USART_StopBits_1_5) || \
                                     ((STOPBITS) == USART_StopBits_2))

#define IS_USART_PARITY(PARITY) (((PARITY) == USART_Parity_No) || \
                                 ((PARITY) == USART_Parity_Even) || \
                                 ((PARITY) == USART_Parity_Odd))

#define IS_USART_HARDWARE_FLOW_CONTROL(CONTROL) \
    (((CONTROL) == USART_HardwareFlowControl_None) || \
     ((CONTROL) == USART_HardwareFlowControl_RTS_CTS))

#define IS_USART_CONFIG_IT(IT) (((IT) == USART_IT_IER_RDI) || ((IT) == USART_IT_IER_THRI) || \
                                ((IT) == USART_IT_IER_RLSI) || ((IT) == USART_IT_IER_MSI))

#define IS_USART_FLAG(FLAG) (((FLAG) == USART_FLAG_IIR_MSI) || ((FLAG) == USART_FLAG_IIR_THRI) || \
                             ((FLAG) == USART_FLAG_IIR_RDI) || ((FLAG) == USART_FLAG_IIR_CHTO) || \
                             ((FLAG) == USART_FLAG_IIR_RLSI))

#define IS_USART_LSR_FLAG(FLAG) (((FLAG) == USART_FLAG_LSR_DR) || ((FLAG) == USART_FLAG_LSR_OE) || \
                                 ((FLAG) == USART_FLAG_LSR_PE) || ((FLAG) == USART_FLAG_LSR_FE) || \
                                 ((FLAG) == USART_FLAG_LSR_BI) || ((FLAG) == USART_FLAG_LSR_THRE) || \
                                 ((FLAG) == USART_FLAG_LSR_TEMT) || ((FLAG) == USART_FLAG_LSR_ERR))

#define IS_USART_CLEAR_FLAG(FLAG) ((((FLAG) & (uint16_t)0xFFFF) == 0x0000) && ((FLAG) != 0x0000))

#define IS_USART_MSR_FLAG(FLAG) (((FLAG) == USART_FLAG_MSR_DCD) || ((FLAG) == USART_FLAG_MSR_RI) || \
                                 ((FLAG) == USART_FLAG_MSR_DSR) || ((FLAG) == USART_FLAG_MSR_CTS) || \
                                 ((FLAG) == USART_FLAG_MSR_DDCD) || ((FLAG) == USART_FLAG_MSR_TERI) || \
                                 ((FLAG) == USART_FLAG_MSR_DDSR) || ((FLAG) == USART_FLAG_MSR_DCTS))

#define IS_USART_BAUDRATE(BAUDRATE) (((BAUDRATE) > 0) && ((BAUDRATE) < 0x0044AA21))

#define IS_USART_DMAMode(MODE) (((MODE) == USART_DMA_Mode0) || ((MODE) == USART_DMA_Mode1))

#define IS_USART_DMAREQ(DMAREQ) ((((DMAREQ) & (uint16_t)0xFFFFFFF9) == 0x00000000) && ((DMAREQ) != (uint16_t)0x00000000))

#define IS_USART_IRDA_MODE(MODE) (((MODE) == USART_IrDAMode_LowPower) || \
                                  ((MODE) == USART_IrDAMode_Normal))

#define IS_USART_FIFO_FCR_CONTROL(CONTROL) (((CONTROL) == USART_FIFO_FCR_FIFIOE) || ((CONTROL) == USART_FIFO_FCR_RFIFOR) || \
                                            ((CONTROL) == USART_FIFO_FCR_XFIFOR) || ((CONTROL) == USART_FIFO_FCR_DMAM) || \
                                            ((CONTROL) == USART_FIFO_FCR_TET) || ((CONTROL) == USART_FIFO_FCR_RT))

#define IS_USART_Modem_MCR_CONTROL(CONTROL) (((CONTROL) == USART_Modem_MCR_DTR) || ((CONTROL) == USART_Modem_MCR_RTS) || \
                                             ((CONTROL) == USART_Modem_MCR_OUT1) || ((CONTROL) == USART_Modem_MCR_OUT2) || \
                                             ((CONTROL) == USART_Modem_MCR_LoopBack) || ((CONTROL) == USART_Modem_MCR_AFCE) || \
                                             ((CONTROL) == USART_Modem_MCR_SIRE))

void USART_DeInit(USART_TypeDef *USARTx)
{
    /* Check the parameters */
    assert_param(IS_USART_ALL_PERIPH(USARTx));

    if (USARTx == USART1) {
        RCC_APB2PeriphResetCmd(RCC_APB2PeriphRest_UART1, ENABLE);
        RCC_APB2PeriphResetCmd(RCC_APB2PeriphRest_UART1, DISABLE);
    } else if (USARTx == USART2) {
        RCC_APB1PeriphResetCmd(RCC_APB1PeriphRest_UART2, ENABLE);
        RCC_APB1PeriphResetCmd(RCC_APB1PeriphRest_UART2, DISABLE);
    } else if (USARTx == USART3) {
        RCC_APB1PeriphResetCmd(RCC_APB1PeriphRest_UART3, ENABLE);
        RCC_APB1PeriphResetCmd(RCC_APB1PeriphRest_UART3, DISABLE);
    }
}

void USART_Init(USART_TypeDef *USARTx, USART_InitTypeDef *USART_InitStruct)
{
    uint32_t tmpreg1           = 0x00;
    uint32_t tmpreg2           = 0x00;
    uint32_t uartclock         = 0x00;
    uint32_t integerdivider    = 0x00;
    uint32_t fractionaldivider = 0x00;

    RCC_ClocksTypeDef RCC_ClocksStatus;
    /* Check the parameters */
    assert_param(IS_USART_ALL_PERIPH(USARTx));
    assert_param(IS_USART_BAUDRATE(USART_InitStruct->USART_BaudRate));
    assert_param(IS_USART_WORD_LENGTH(USART_InitStruct->USART_WordLength));
    assert_param(IS_USART_STOPBITS(USART_InitStruct->USART_StopBits));
    assert_param(IS_USART_PARITY(USART_InitStruct->USART_Parity));
    assert_param(IS_USART_HARDWARE_FLOW_CONTROL(USART_InitStruct->USART_HardwareFlowControl));

    /* Get Current USART clock*/
    RCC_GetClocksFreq(&RCC_ClocksStatus);
    uartclock = RCC_ClocksStatus.USART_Frequencey;

    /* Determine the integer part */
    integerdivider = ((25 * uartclock) / (4 * USART_InitStruct->USART_BaudRate));
    tmpreg2        = (integerdivider / 100);
    tmpreg1        = (integerdivider / 100) << 4;
    /* Determine the fractional part */
    fractionaldivider = integerdivider - (100 * (tmpreg1 >> 4));

    tmpreg1 = ((fractionaldivider * 16 + 50) / 100) & 0x0f;

    /* Set boudrate. */
    USARTx->DLF.DIV = tmpreg1;

    /* DLAB bit enable to set boudrate. */
    USARTx->LCR.DLAB              = 1;
    USARTx->BRB_THR_DLL.DLL.DIV_L = (uint8_t)(tmpreg2);
    USARTx->DLH_IER.DLH.DIV_H     = (uint8_t)(tmpreg2 >> 8);

    /* DLAB bit disable to set boudrate. */
    USARTx->LCR.DLAB = 0;

    tmpreg1 = USARTx->LCR.value;
    /* Clear LCR reg. */
    tmpreg1 = 0x0;

    tmpreg1 |= USART_InitStruct->USART_WordLength |
               USART_InitStruct->USART_Parity |
               USART_InitStruct->USART_StopBits;

    USARTx->LCR.value = tmpreg1;

    USARTx->MCR.RTS  = 0;
    USARTx->MCR.AFCE = 0;
    USARTx->MCR.value |= USART_InitStruct->USART_HardwareFlowControl;
    /* FIFO mode set,by default is disabled. */
    USARTx->IIR_FCR.FCR.value = 0x00;
    /* Disable all interrupt. */
    USARTx->DLH_IER.IER.value = 0x00;
}

void USART_StructInit(USART_InitTypeDef *USART_InitStruct)
{
    /* USART_InitStruct members default value */
    USART_InitStruct->USART_BaudRate            = 9600;
    USART_InitStruct->USART_WordLength          = USART_WordLength_8b;
    USART_InitStruct->USART_StopBits            = USART_StopBits_1;
    USART_InitStruct->USART_Parity              = USART_Parity_No;
    USART_InitStruct->USART_HardwareFlowControl = USART_HardwareFlowControl_None;
}

void USART_SendData(USART_TypeDef *USARTx, uint16_t Data)
{
    /* Check the parameters */
    assert_param(IS_USART_ALL_PERIPH(USARTx));

    USARTx->BRB_THR_DLL.THR.value = (Data & (uint16_t)0x01ff);
}

uint16_t USART_ReceiveData(USART_TypeDef *USARTx)
{
    /* Check the parameters */
    assert_param(IS_USART_ALL_PERIPH(USARTx));

    return (uint16_t)(USARTx->BRB_THR_DLL.RBR.value);
}

void USART_ITConfig(USART_TypeDef *USARTx, uint16_t USART_IT, FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_USART_ALL_PERIPH(USARTx));
    assert_param(IS_USART_CONFIG_IT(USART_IT));

    if (NewState != DISABLE) {

        USARTx->DLH_IER.IER.value |= USART_IT;

    } else {

        USARTx->DLH_IER.IER.value &= ~USART_IT;
    }
}

void USART_DMACmd(USART_TypeDef *USARTx, uint16_t USART_DMAReq, FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_USART_ALL_PERIPH(USARTx));
    assert_param(IS_USART_DMAREQ(USART_DMAReq));
    assert_param(IS_FUNCTIONAL_STATE(NewState));
    if (NewState != DISABLE) {

        USARTx->IIR_FCR.FCR.value |= USART_DMAReq;
    } else {

        USARTx->IIR_FCR.FCR.value &= ~USART_DMAReq;
    }
}

void USART_FIFOConfig(USART_TypeDef *USARTx, uint16_t USART_FIFO)
{
    uint16_t tmpfcr = 0;
    /* Check the parameters */
    assert_param(IS_USART_ALL_PERIPH(USARTx));
    assert_param(IS_USART_FIFO_FCR_CONTROL(USART_FIFO));
    tmpfcr = USARTx->IIR_FCR.FCR.value;
    /* Reset the FIFO_ENABLE and FIFOE Bits */
    tmpfcr &= (uint16_t)(~((uint16_t)(IIR_FCR_IIR_FIFO_ENABLE | IIR_FCR_FCR_FIFOE)));
    tmpfcr |= USART_FIFO;
    /* Write to USARTx FCR */
    USARTx->IIR_FCR.FCR.value = tmpfcr;
}

void USART_SetAddress(USART_TypeDef *USARTx, uint8_t USART_Address)
{
    /* Check the parameters */
    assert_param(IS_USART_ALL_PERIPH(USARTx));

    USARTx->TAR.ADRESS = USART_Address;
}

void USART_IrDAConfig(USART_TypeDef *USARTx, uint16_t USART_IrDAMode)
{
    uint16_t tmplcr = 0;
    /* Check the parameters */
    assert_param(IS_USART_ALL_PERIPH(USARTx));
    assert_param(IS_USART_IRDA_MODE(USART_IrDAMode));
    tmplcr = USARTx->LCR.value;
    /* Reset the DLAB Bits */
    tmplcr &= (uint16_t)(~((uint16_t)LCR_DLAB));
    tmplcr |= USART_IrDAMode;
    /* Write to USARTx LCR */
    USARTx->LCR.value = tmplcr;
}

void USART_IrDACmd(USART_TypeDef *USARTx, FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_USART_ALL_PERIPH(USARTx));

    if (NewState != DISABLE) {

        USARTx->MCR.SIRE = 1;

    } else {

        USARTx->MCR.SIRE = 0;
    }
}

void USART_FIFOCmd(USART_TypeDef *USARTx, FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_USART_ALL_PERIPH(USARTx));
    assert_param(IS_FUNCTIONAL_STATE(NewState));
    // assert_param(IS_USART_FIFO_FCR_FLAG(NewState));
    if (NewState != DISABLE) {

        USARTx->IIR_FCR.FCR.FIFOE = 1;

    } else {

        USARTx->IIR_FCR.FCR.FIFOE = 0;
    }
}

void USART_SendBreak(USART_TypeDef *USARTx)
{
    /* Check the parameters */
    assert_param(IS_USART_ALL_PERIPH(USARTx));

    USARTx->LCR.BREAK = 1;
}

FlagStatus USART_GetLineFlagStatus(USART_TypeDef *USARTx, uint16_t USART_FLAG)
{
    FlagStatus bitstatus = RESET;
    /* Check the parameters */
    assert_param(IS_USART_ALL_PERIPH(USARTx));
    assert_param(IS_USART_LSR_FLAG(USART_FLAG));

    if ((USARTx->LSR.value & USART_FLAG) != (uint16_t)RESET) {

        bitstatus = SET;

    } else {

        bitstatus = RESET;
    }

    return bitstatus;
}

FlagStatus USART_GetModemFlagStatus(USART_TypeDef *USARTx, uint16_t USART_FLAG)
{
    FlagStatus bitstatus = RESET;
    /* Check the parameters */
    assert_param(IS_USART_ALL_PERIPH(USARTx));
    assert_param(IS_USART_MSR_FLAG(USART_FLAG));

    if ((USARTx->MSR.value & USART_FLAG) != (uint16_t)RESET) {

        bitstatus = SET;

    } else {

        bitstatus = RESET;
    }

    return bitstatus;
}

void USART_ClearFlag(USART_TypeDef *USARTx, uint16_t USART_FLAG)
{
    uint16_t tmpclear = 0;
    /* Check the parameters */
    assert_param(IS_USART_ALL_PERIPH(USARTx));
    assert_param(IS_USART_CLEAR_FLAG(USART_FLAG));
    tmpclear = USARTx->LSR.value;
    tmpclear &= ~USART_FLAG;
}

USART_InterruptID USART_GetInterruptID(USART_TypeDef *USARTx)
{
    /* Check the parameters */
    assert_param(IS_USART_ALL_PERIPH(USARTx));

    return USARTx->IIR_FCR.IIR.INTERRUPT_ID;
}

ITStatus USART_GetITStatus(USART_TypeDef *USARTx, uint16_t USART_IT)
{
    ITStatus bitstatus = RESET;
    /* Check the parameters */
    assert_param(IS_USART_ALL_PERIPH(USARTx));
    assert_param(IS_USART_CONFIG_IT(USART_IT));

    if (USARTx->IIR_FCR.IIR.INTERRUPT_ID == USART_IT) {

        bitstatus = SET;

    } else {

        bitstatus = RESET;
    }

    return bitstatus;
}

void USART_ClearITPendingBit(USART_TypeDef *USARTx, uint16_t USART_IT)
{
    /* Check the parameters */
    assert_param(IS_USART_ALL_PERIPH(USARTx));
    assert_param(IS_USART_CONFIG_IT(USART_IT));
    /* Clear the IT pending Bit */
    USARTx->DLH_IER.IER.value = ~USART_IT;
}

#endif