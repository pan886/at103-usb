/**
 * @file at103_iic.c
 * @brief This file provides all the iic firmware functions.
 * 
 * @author yaobin
 * @version 1.0
 * @date 2022-05-02
 * @copyright Copyright (c) 2022 Icore, Inc
 */

#include "at103_i2c.h"

#ifdef I2C_MODULE_ENABLED

#define IS_I2C_ALL_PERIPH(PERIPH) (((PERIPH) == I2C1) || \
                                   ((PERIPH) == I2C2))

#define IS_I2C_MASTERORSLAVEMODE(I2C_MODE) (((I2C_MODE) == I2C_Mode_Master) || \
                                            ((I2C_MODE) == I2C_Mode_Slave))

#define IS_I2C_STANDARDORFASTMODE(I2C_SPEEDMODE) (((I2C_SPEEDMODE) == I2C_SpeedMode_Standard) || \
                                                  ((I2C_SPEEDMODE) == I2C_SpeedMode_Fast))

#define IS_I2C_ADDRESSMODE(ADDRESSMODE) (((ADDRESSMODE) == I2C_AddressMode_7bit) || \
                                         ((ADDRESSMODE) == I2C_AddressMode_10bit))

#define IS_I2C_DMAReq(REQ) (((REQ) == I2C_DMAReq_Tx) || \
                            ((REQ) == I2C_DMAReq_Rx))

#define IS_I2C_IT(IT) (((IT) == I2C_IT_RXUNDER) || ((IT) == I2C_IT_RXOVER) || \
                       ((IT) == I2C_IT_RXFULL) || ((IT) == I2C_IT_TXOVER) || \
                       ((IT) == I2C_IT_TXEMPTY) || ((IT) == I2C_IT_RDREQ) || \
                       ((IT) == I2C_IT_TXABRT) || ((IT) == I2C_IT_RXDONE) || \
                       ((IT) == I2C_IT_ACTIVITY) || ((IT) == I2C_IT_STOP) || \
                       ((IT) == I2C_IT_START) || ((IT) == I2C_IT_GENCALL) || \
                       ((IT) == I2C_IT_RESTART) || ((IT) == I2C_IT_MSTONHOLD) || \
                       ((IT) == I2C_IT_SCLSTUCKATLOW))

#define IS_I2C_STATUS(STATUS) (((STATUS) == I2C_STATUS_ACTIVITY) || ((STATUS) == I2C_STATUS_TFNF) || \
                               ((STATUS) == I2C_STATUS_TFE) || ((STATUS) == I2C_STATUS_RFNE) || \
                               ((STATUS) == I2C_STATUS_RFF) || ((STATUS) == I2C_STATUS_MST_ACTIVITY) || \
                               ((STATUS) == I2C_STATUS_SLV_ACTIVITY) || ((STATUS) == I2C_STATUS_MST_HOLD_TX_FIFO_EMPTY) || \
                               ((STATUS) == I2C_STATUS_MST_HOLD_RX_FIFO_FULL) || ((STATUS) == I2C_STATUS_SLV_HOLD_TX_FIFO_EMPTY) || \
                               ((STATUS) == I2C_STATUS_SLV_HOLD_RX_FIFO_FULL))

#define IS_I2C_TX_ABRT_SOURCE(SOURCE) (((SOURCE) == I2C_TX_ABRT_DEVICE_WRITE) || \
                                       ((SOURCE) == I2C_TX_ABRT_DEVICE_SLVADDR_NOACK) || \
                                       ((SOURCE) == I2C_TX_ABRT_DEVICE_NOACK) || \
                                       ((SOURCE) == I2C_TX_ABRT_SDA_STUCK_AT_LOW) || \
                                       ((SOURCE) == I2C_TX_ABRT_USER_ABRT) || \
                                       ((SOURCE) == I2C_TX_ABRT_SLVRD_INTX) || \
                                       ((SOURCE) == I2C_TX_ABRT_SLV_ARBLOST) || \
                                       ((SOURCE) == I2C_TX_ABRT_SLVFLUSH_TXFIFO) || \
                                       ((SOURCE) == I2C_TX_ARBT_LOST) || \
                                       ((SOURCE) == I2C_TX_ABRT_MASTER_DIS) || \
                                       ((SOURCE) == I2C_TX_ABRT_10B_RD_NORSTRT) || \
                                       ((SOURCE) == I2C_TX_ABRT_SBYTE_NORSTRT) || \
                                       ((SOURCE) == I2C_TX_ABRT_HS_NORSTRT) || \
                                       ((SOURCE) == I2C_TX_ABRT_GCALL_READ) || \
                                       ((SOURCE) == I2C_TX_ABRT_GCALL_NOACK) || \
                                       ((SOURCE) == I2C_TX_ABRT_TXDATA_NOACK) || \
                                       ((SOURCE) == I2C_TX_ABRT_10ADDR2_NOACK) || \
                                       ((SOURCE) == I2C_TX_ABRT_10ADDR1_NOACK) || \
                                       ((SOURCE) == I2C_TX_ABRT_7B_ADDR_NOACK))

#define IS_I2C_DIRECTION(DIRECTION) (((DIRECTION) == I2C_Direction_Transmitter) || \
                                     ((DIRECTION) == I2C_Direction_Receiver))

#define IS_I2C_ENABLE_STATUS(ENABLESTATUS) (((ENABLESTATUS) == I2C_SLV_RX_DATA_LOST) || \
                                            ((ENABLESTATUS) == I2C_SLV_DISABLED_WHILE_BUSY) || \
                                            ((ENABLESTATUS) == I2C_ENABLE))

#define IS_I2C_STOPINT_CONFIG(I2C_STOP_CONFIG) (((I2C_STOP_CONFIG) == I2C_StopActive_IfMasterActive) || \
                                                ((I2C_STOP_CONFIG) == I2C_StopActive_MasterAlways) || \
                                                ((I2C_STOP_CONFIG) == I2C_StopActive_IfSlaveAddressed) || \
                                                ((I2C_STOP_CONFIG) == I2C_StopActive_SlaveAlways))

void I2C_DeInit(I2C_TypeDef *I2Cx)
{
    /* Check the parameters */
    assert_param(IS_I2C_ALL_PERIPH(I2Cx));

    if (I2Cx == I2C1) {
        /* Enable I2C1 reset state */
        RCC_APB1PeriphResetCmd(RCC_APB1Periph_I2C1, ENABLE);
        /* Release I2C1 from reset state */
        RCC_APB1PeriphResetCmd(RCC_APB1Periph_I2C1, DISABLE);
    } else {
        /* Enable I2C2 reset state */
        RCC_APB1PeriphResetCmd(RCC_APB1Periph_I2C2, ENABLE);
        /* Release I2C2 from reset state */
        RCC_APB1PeriphResetCmd(RCC_APB1Periph_I2C2, DISABLE);
    }
}

void I2C_Init(I2C_TypeDef *I2Cx, I2C_InitTypeDef *I2C_InitStruct)
{
    RCC_ClocksTypeDef RCC_ClocksStatus;
    uint32_t          I2CCLK = 0;

    /* Check the parameters */
    assert_param(IS_I2C_ALL_PERIPH(I2Cx));

    /*Configure I2C Mode*/
    if (I2C_InitStruct->I2C_Mode == I2C_Mode_Master) {
        I2Cx->CON.SLAVE_DISABLE = Bit_SET;
        I2Cx->CON.MASTER_MODE   = Bit_SET;
        I2Cx->TAR.SPECIAL       = Bit_RESET;
    }
    if (I2C_InitStruct->I2C_Mode == I2C_Mode_Slave) {
        I2Cx->CON.SLAVE_DISABLE = Bit_RESET;
        I2Cx->CON.MASTER_MODE   = Bit_RESET;
        I2Cx->TAR.SPECIAL       = Bit_RESET;
    }
    if (I2C_InitStruct->I2C_Mode == I2C_Mode_GeneralCall) {
        I2Cx->CON.SLAVE_DISABLE             = Bit_SET;
        I2Cx->CON.MASTER_MODE               = Bit_SET;
        I2Cx->TAR.SPECIAL                   = Bit_SET;
        I2Cx->TAR.GC_OR_START               = Bit_RESET;
        I2Cx->ACK_GENERAL_CALL.ACK_GEN_CALL = Bit_SET;
    }
    /*Configure I2C Bus Clock Speed*/
    I2Cx->CON.SPEED = I2C_InitStruct->I2C_SpeedMode;

    /* Configure Address Mode*/
    if (I2C_InitStruct->I2C_AddressMode == I2C_AddressMode_7bit) {
        I2Cx->CON._10BITADDR_MASTER = Bit_RESET;
        I2Cx->CON._10BITADDR_SLAVE  = Bit_RESET;
    } else {
        if (I2C_InitStruct->I2C_Mode == I2C_Mode_Master)
            I2Cx->CON._10BITADDR_MASTER = Bit_SET;
        else
            I2Cx->CON._10BITADDR_SLAVE = Bit_SET;
    }
    /* configure target Address or Slave Address*/
    I2Cx->TAR.TAR = I2C_InitStruct->I2C_TargetAddress;
    I2Cx->SAR.SAR = I2C_InitStruct->I2C_SlaveAddress;

    /* configure I2C Restart */

    I2Cx->CON.RESTART_EN = Bit_SET;

    /* configure clock speed */
    RCC_GetClocksFreq(&RCC_ClocksStatus);
    I2CCLK = RCC_ClocksStatus.PCLK1_Frequency;
    switch (I2Cx->CON.SPEED) {
    case I2C_SpeedMode_Standard:
        I2Cx->SS_SCL_HCNT.SS_SCL_HCNT = (I2CCLK / 1000000) * (1000.0 / I2C_InitStruct->I2C_ClockSpeed / 2);
        I2Cx->SS_SCL_LCNT.SS_SCL_LCNT = (I2CCLK / 1000000) * (1000.0 / I2C_InitStruct->I2C_ClockSpeed / 2);
        break;
    case I2C_SpeedMode_Fast:
        I2Cx->FS_SCL_HCNT.FS_SCL_HCNT = (I2CCLK / 1000000) * (1000.0 / (I2C_InitStruct->I2C_ClockSpeed / 2));
        I2Cx->FS_SCL_LCNT.FS_SCL_LCNT = (I2CCLK / 1000000) * (1000.0 / (I2C_InitStruct->I2C_ClockSpeed / 2));
        break;
    default:
        break;
    }

    I2Cx->SCL_STUCK_AT_LOW_TIMEOUT.SCL_STUCK_AT_LOW_TIMEOUT = (I2CCLK / 1000) * 25;
    I2Cx->SDA_STUCK_AT_LOW_TIMEOUT.SDA_STUCK_AT_LOW_TIMEOUT = (I2CCLK / 1000) * 25;
}

void I2C_StructInit(I2C_InitTypeDef *I2C_InitStruct)
{
    I2C_InitStruct->I2C_Mode          = I2C_Mode_Master;
    I2C_InitStruct->I2C_AddressMode   = I2C_AddressMode_7bit;
    I2C_InitStruct->I2C_TargetAddress = 0x50;
    I2C_InitStruct->I2C_SlaveAddress  = 0;
    I2C_InitStruct->I2C_SpeedMode     = I2C_SpeedMode_Standard;
    I2C_InitStruct->I2C_ClockSpeed    = 400;
}

void I2C_Cmd(I2C_TypeDef *I2Cx, FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_I2C_ALL_PERIPH(I2Cx));
    assert_param(IS_FUNCTIONAL_STATE(NewState));
    if (NewState != DISABLE) {
        /* Enable the selected I2C peripheral */
        I2Cx->ENABLE.ENABLE = Bit_SET;
    } else {
        /* Disable the selected I2C peripheral */
        I2Cx->ENABLE.ENABLE = Bit_RESET;
    }
}

void I2C_DMACmd(I2C_TypeDef *I2Cx, uint8_t I2C_DMAReq, FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_I2C_ALL_PERIPH(I2Cx));
    assert_param(IS_FUNCTIONAL_STATE(NewState));
    assert_param(IS_I2C_DMAReq(I2C_DMAReq));
    if (I2C_DMAReq & I2C_DMAReq_Tx) {
        I2Cx->DMA_CR.TDMAE = (NewState != DISABLE) ? Bit_SET : Bit_RESET;
    }
    if (I2C_DMAReq & I2C_DMAReq_Rx) {
        I2Cx->DMA_CR.RDMAE = (NewState != DISABLE) ? Bit_SET : Bit_RESET;
    }
}

void I2C_DMADataLevelConfig(I2C_TypeDef *I2Cx, uint8_t I2C_DMAReq, uint8_t DataLevel)
{
    /* Check the parameters */
    assert_param(IS_I2C_ALL_PERIPH(I2Cx));
    assert_param(IS_I2C_DMAReq(I2C_DMAReq));

    if (I2C_DMAReq & I2C_DMAReq_Tx) {
        I2Cx->DMA_TDLR.DMATDL = DataLevel;
    }

    if (I2C_DMAReq & I2C_DMAReq_Rx) {
        I2Cx->DMA_RDLR.DMARDL = DataLevel;
    }
}
void I2C_RxDMADataLevelConfig(I2C_TypeDef *I2Cx, uint8_t RxDataLevel)
{
    /* Check the parameters */
    assert_param(IS_I2C_ALL_PERIPH(I2Cx));
    I2Cx->DMA_RDLR.DMARDL = RxDataLevel;
}

void I2C_GenerateSTART(I2C_TypeDef *I2Cx, FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_I2C_ALL_PERIPH(I2Cx));
    assert_param(IS_FUNCTIONAL_STATE(NewState));
    if (NewState != DISABLE) {
        /* Generate a START/RESTART condition */
        I2Cx->DATA_CMD.RESTART = 1;
    } else {
        /* Disable the START/RESTART condition generation */
        I2Cx->DATA_CMD.RESTART = 0;
    }
}

void I2C_GenerateSTOP(I2C_TypeDef *I2Cx, FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_I2C_ALL_PERIPH(I2Cx));
    assert_param(IS_FUNCTIONAL_STATE(NewState));
    if (NewState != DISABLE) {
        /* Generate a STOP condition */
        I2Cx->DATA_CMD.STOP = 1;
    } else {
        /* Disable the STOP condition generation */
        I2Cx->DATA_CMD.STOP = 0;
    }
}

void I2C_TxBlock(I2C_TypeDef *I2Cx, FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_I2C_ALL_PERIPH(I2Cx));
    assert_param(IS_FUNCTIONAL_STATE(NewState));
    if (NewState != DISABLE) {
        /* Generate a STOP condition */
        I2Cx->ENABLE.TX_CMD_BLOCK = Bit_SET;
    } else {
        /* Disable the STOP condition generation */
        I2Cx->ENABLE.TX_CMD_BLOCK = Bit_RESET;
    }
}

void I2C_TxAbort(I2C_TypeDef *I2Cx, FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_I2C_ALL_PERIPH(I2Cx));
    assert_param(IS_FUNCTIONAL_STATE(NewState));
    if (NewState != DISABLE) {
        /* Generate a STOP condition */
        I2Cx->ENABLE.ABORT = Bit_SET;
    } else {
        /* Disable the STOP condition generation */
        I2Cx->ENABLE.ABORT = Bit_RESET;
    }
}

void I2C_FifoThresHoldLevelConfig(I2C_TypeDef *I2Cx, uint8_t TxFifoThresHoldLevel, uint8_t RxFifoThresHoldLevel)
{
    /* Check the parameters */
    assert_param(IS_I2C_ALL_PERIPH(I2Cx));

    I2Cx->RX_TL.RX_TL = RxFifoThresHoldLevel;
    I2Cx->TX_TL.TX_TL = TxFifoThresHoldLevel;
}

void I2C_StopInterruptConfig(I2C_TypeDef *I2Cx, uint32_t StopCondition)
{
    /* Check the parameters */
    assert_param(IS_I2C_ALL_PERIPH(I2Cx));
    assert_param(IS_I2C_STOPINT_CONFIG(StopCondition));
    switch (StopCondition) {
    case I2C_StopActive_IfMasterActive:
        I2Cx->CON.STOP_DET_IF_MASTER_ACTIVE = Bit_SET;
        break;
    case I2C_StopActive_MasterAlways:
        I2Cx->CON.STOP_DET_IF_MASTER_ACTIVE = Bit_RESET;
        break;
    case I2C_StopActive_IfSlaveAddressed:
        I2Cx->CON.STOP_DET_IF_MASTER_ACTIVE = Bit_SET;
        break;
    case I2C_StopActive_SlaveAlways:
        I2Cx->CON.STOP_DET_IF_MASTER_ACTIVE = Bit_RESET;
        break;
    default:
        break;
    }
}

void I2C_RxFifoFullHold(I2C_TypeDef *I2Cx, FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_I2C_ALL_PERIPH(I2Cx));
    assert_param(IS_FUNCTIONAL_STATE(NewState));
    if (NewState != DISABLE) {
        I2Cx->CON.RX_FIFO_FULL_HLD_CTRL = Bit_SET;
    } else {
        I2Cx->CON.RX_FIFO_FULL_HLD_CTRL = Bit_RESET;
    }
}

void I2C_TxEmptyCtrl(I2C_TypeDef *I2Cx, FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_I2C_ALL_PERIPH(I2Cx));
    assert_param(IS_FUNCTIONAL_STATE(NewState));
    if (NewState != DISABLE) {
        I2Cx->CON.TX_EMPTY_CTRL = Bit_SET;
    } else {
        I2Cx->CON.TX_EMPTY_CTRL = Bit_RESET;
    }
}

void I2C_ITConfig(I2C_TypeDef *I2Cx, uint32_t I2C_IT, FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_I2C_ALL_PERIPH(I2Cx));
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    if (NewState != DISABLE) {

        I2Cx->INTR_MASK.value |= I2C_IT;
    } else {

        I2Cx->INTR_MASK.value &= ~(I2C_IT);
    }
}

ITStatus I2C_GetITStatus(I2C_TypeDef *I2Cx, uint32_t I2C_IT)
{
    ITStatus bitstatus = RESET;
    /* Check the parameters */
    assert_param(IS_I2C_ALL_PERIPH(I2Cx));

    /* Check the status of the specified I2C flag */
    if (I2Cx->RAW_INTR_STAT.value & I2C_IT) {
        /* I2C_IT is set */
        bitstatus = SET;
    } else {
        /* I2C_IT is reset */
        bitstatus = RESET;
    }
    /* Return the I2C_IT status */
    return bitstatus;
}

void I2C_ClearITPendingBit(I2C_TypeDef *I2Cx, uint32_t I2C_IT)
{
    uint8_t  ITPendingBit = 0;
    uint32_t IT_Mask      = 0x4000;
    /* Check the parameters */
    assert_param(IS_I2C_ALL_PERIPH(I2Cx));

    if (I2C_IT == I2C_IT_ALL) {
        ITPendingBit = I2Cx->CLR_INTR.value;
        return;
    }

    while (IT_Mask) {
        switch (I2C_IT & IT_Mask) {
        case I2C_IT_RXUNDER:
            ITPendingBit = I2Cx->CLR_RX_UNDER.value;
            break;
        case I2C_IT_RXOVER:
            ITPendingBit = I2Cx->CLR_RX_OVER.value;
            break;
        case I2C_IT_TXOVER:
            ITPendingBit = I2Cx->CLR_TX_OVER.value;
            break;
        case I2C_IT_RDREQ:
            ITPendingBit = I2Cx->CLR_RD_REQ.value;
            break;
        case I2C_IT_TXABRT:
            ITPendingBit = I2Cx->CLR_TX_ABRT.value;
            break;
        case I2C_IT_RXDONE:
            ITPendingBit = I2Cx->CLR_RX_DONE.value;
            break;
        case I2C_IT_ACTIVITY:
            ITPendingBit = I2Cx->CLR_ACTIVITY.value;
            break;
        case I2C_IT_STOP:
            ITPendingBit = I2Cx->CLR_STOP_DET.value;
            break;
        case I2C_IT_START:
            ITPendingBit = I2Cx->CLR_START_DET.value;
            break;
        case I2C_IT_GENCALL:
            ITPendingBit = I2Cx->CLR_GEN_CALL.value;
            break;
        default:
            break;
        }

        IT_Mask >>= 1;
    }
}
void I2C_RestartConfig(I2C_TypeDef *I2Cx, FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_I2C_ALL_PERIPH(I2Cx));

    if (NewState != DISABLE) {
        I2Cx->CON.RESTART_EN = Bit_RESET;
    } else {
        I2Cx->CON.RESTART_EN = Bit_SET;
    }
}

void I2C_SDASetupTimeConfigure(I2C_TypeDef *I2Cx, uint8_t SetupTime)
{
    /* Check the parameters */
    assert_param(IS_I2C_ALL_PERIPH(I2Cx));

    I2Cx->SDA_SETUP.SDA_SETUP = SetupTime + 1;
}

void I2C_TxSDAHoldTimeConfigure(I2C_TypeDef *I2Cx, uint8_t HoldTime)
{
    /* Check the parameters */
    assert_param(IS_I2C_ALL_PERIPH(I2Cx));

    I2Cx->SDA_HOLD.SDA_TX_HOLD = HoldTime;
}

void II2C_RxSDAHoldTimeConfigure(I2C_TypeDef *I2Cx, uint8_t HoldTime)
{
    /* Check the parameters */
    assert_param(IS_I2C_ALL_PERIPH(I2Cx));

    I2Cx->SDA_HOLD.SDA_RX_HOLD = HoldTime;
}
I2C_TxAbort_Status I2C_GetTxAbortSource(I2C_TypeDef *I2Cx, uint32_t TxAbort)
{
    I2C_TxAbort_Status bitstatus = RESET;
    /* Check the parameters */
    assert_param(IS_I2C_ALL_PERIPH(I2Cx));
    assert_param(IS_I2C_TX_ABRT_SOURCE(TxAbort));
    /* Check the status of the specified I2C flag */
    if (I2Cx->TX_ABRT_SOURCE.value & TxAbort) {
        /* I2C_IT is set */
        bitstatus = SET;
    } else {
        /* I2C_IT is reset */
        bitstatus = RESET;
    }
    /* Return the I2C_IT status */
    return bitstatus;
}

I2C_ModuleEnable_Status I2C_GetModuleEnableStatus(I2C_TypeDef *I2Cx, uint8_t EnableStatus)
{
    I2C_ModuleEnable_Status bitstatus = RESET;
    /* Check the parameters */
    assert_param(IS_I2C_ALL_PERIPH(I2Cx));
    assert_param(IS_I2C_ENABLE_STATUS(EnableStatus));
    /* Check the status of the specified I2C flag */
    if (I2Cx->ENABLE_STATUS.value & EnableStatus) {
        /* I2C_Enable_Status is set */
        bitstatus = SET;
    } else {
        /* I2C_Enable_Status is reset */
        bitstatus = RESET;
    }
    /* Return the I2C_Enable_Status status */
    return bitstatus;
}

I2C_BusStatus I2C_GetBusStatus(I2C_TypeDef *I2Cx, uint32_t I2CStatus)
{
    I2C_BusStatus bitstatus = RESET;
    /* Check the parameters */
    assert_param(IS_I2C_ALL_PERIPH(I2Cx));
    assert_param(IS_I2C_STATUS(I2CStatus));
    /* Check the status of the specified I2C  */
    if (I2Cx->STATUS.value & I2CStatus) {
        /* I2C Status is set */
        bitstatus = SET;
    } else {
        /* I2C Status is reset */
        bitstatus = RESET;
    }
    /* Return the I2C status */
    return bitstatus;
}

void I2C_SendData(I2C_TypeDef *I2Cx, uint8_t Data)
{
    /* Check the parameters */
    assert_param(IS_I2C_ALL_PERIPH(I2Cx));
    if (I2Cx->DATA_CMD.CMD == I2C_Direction_Receiver)
        I2Cx->DATA_CMD.CMD = I2C_Direction_Transmitter;
    I2Cx->DATA_CMD.DAT = Data;
    while (I2C_GetBusStatus(I2Cx, I2C_STATUS_TFE) == RESET)
        ;
}

void I2C_SendlastData(I2C_TypeDef *I2Cx, uint8_t Data)
{
    /* Check the parameters */
    assert_param(IS_I2C_ALL_PERIPH(I2Cx));
    if (I2Cx->DATA_CMD.CMD == I2C_Direction_Receiver)
        I2Cx->DATA_CMD.CMD = I2C_Direction_Transmitter;
    I2Cx->DATA_CMD.value = Data | 0x200;
    while (I2C_GetBusStatus(I2Cx, I2C_STATUS_TFE) == RESET)
        ;
}

void I2C_MasterPollSend(I2C_TypeDef *I2Cx, uint8_t *Data, uint32_t length)
{
    while (length-- > 1) {
        I2C_SendData(I2Cx, *Data++);
    }
    I2C_SendlastData(I2Cx, *Data);
}

uint8_t I2C_ReceiveData(I2C_TypeDef *I2Cx)
{
    /* Check the parameters */
    assert_param(IS_I2C_ALL_PERIPH(I2Cx));
    if (I2Cx->DATA_CMD.CMD == I2C_Direction_Transmitter)
        I2Cx->DATA_CMD.CMD = I2C_Direction_Receiver;
    while (I2C_GetBusStatus(I2Cx, I2C_STATUS_RFNE) == RESET)
        ;
    return ((uint8_t)I2Cx->DATA_CMD.DAT);
}

void I2C_SlavePollRecv(I2C_TypeDef *I2Cx, uint8_t *Data, uint32_t Length)
{
    while (I2C_GetBusStatus(I2Cx, I2C_STATUS_RFNE) != RESET) {
        *Data++ = (uint8_t)I2Cx->DATA_CMD.DAT;
        if (--Length == 0)
            break;
    }
}

uint8_t I2C_ReceiveLastData(I2C_TypeDef *I2Cx)
{
    I2Cx->DATA_CMD.value |= 0x300;
    while (I2C_GetBusStatus(I2Cx, I2C_STATUS_RFNE) == RESET)
        ;
    return I2Cx->DATA_CMD.DAT;
}

void I2C_MasterPollRecv(I2C_TypeDef *I2Cx, uint8_t *Data, uint32_t Length)
{
    while (Length-- > 1) {
        *Data++ = I2C_ReceiveData(I2Cx);
    }
    *Data = I2C_ReceiveLastData(I2Cx);
}

uint8_t I2C_GetTxFifoVaildDataLevel(I2C_TypeDef *I2Cx)
{
    /* Check the parameters */
    assert_param(IS_I2C_ALL_PERIPH(I2Cx));
    return (uint8_t)I2Cx->TXFLR.TXFLR;
}

uint8_t I2C_GetRxFifoVaildDataLevel(I2C_TypeDef *I2Cx)
{
    /* Check the parameters */
    assert_param(IS_I2C_ALL_PERIPH(I2Cx));
    return (uint8_t)I2Cx->RXFLR.RXFLR;
}

uint16_t I2C_GetTxflushCnt(I2C_TypeDef *I2Cx)
{
    /* Check the parameters */
    assert_param(IS_I2C_ALL_PERIPH(I2Cx));
    return (uint16_t)I2Cx->TX_ABRT_SOURCE.TX_FLUSH_CNT;
}

#endif
