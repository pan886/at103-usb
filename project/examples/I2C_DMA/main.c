/**
 * @file main.c
 * @brief I2C example
 * @author Yaobin (ybb@163.com)
 * @version 1.0
 * @date 2022-01-07
 * @copyright Copyright (c) 2022 ICORE, Inc
 */
#include "at103.h"
#include <string.h>
#include "at103_nvic.h"
#include "at103_dma.h"
/* Private macro -------------------------------------------------------------*/
#define ZeroMemory(dp) memset(dp, 0, sizeof(dp))
#define BUF_SIZE       64

#define I2C_Master             I2C1
#define I2C_Slave_AckAddress   (0x50)
#define RCC_I2C_Master         RCC_APB1Periph_I2C1
#define Master_Send_DMAChannel DMA_Channel1

/* Private variables ---------------------------------------------------------*/
uint8_t MasterSendbuf[BUF_SIZE] = {0, 0, // page address
                                   2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 0xaa,
                                   0, 1, 2, 3, 4, 5, 6, 0xaa, 8, 9, 10, 11, 12, 13, 14, 0xff,
                                   0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 0xff,
                                   0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};

uint8_t MasterRecvbuf[BUF_SIZE];

/* Private function prototypes -----------------------------------------------*/
static uint8_t DataComp(const void *Src, const void *Dst, uint32_t length);
static void    I2C_Master_Init(void);
static void    I2C_MasterDMASend_Init(void);

/* Private functions ---------------------------------------------------------*/
static uint8_t DataCompare(const void *Src, const void *Dst, uint32_t length)
{
    while (*(uint8_t *)Src++ == *(uint8_t *)Dst++)
        if (--length == 0)
            return 0;
    debug("Err Data is 0x%x offest is %d \n", *(uint8_t *)Src, length);
    return length;
}
static void I2C_Master_Init()
{
    /* I2C_Master GPIO Pin configuration ----------------------------------------------*/
    GPIO_InitTypeDef IIC_Pin = {
        GPIO_Pin_6 | GPIO_Pin_7,
        GPIO_Speed_10MHz,
        GPIO_Mode_AF,
    };
    GPIO_Init(GPIOB, &IIC_Pin);
    /* I2C_Master Module configuration ----------------------------------------------*/
    I2C_DeInit(I2C_Master);
    RCC_APB1PeriphClockCmd(RCC_I2C_Master, ENABLE);
    I2C_InitTypeDef Master_Conf = {
        I2C_Mode_Master,
        I2C_SpeedMode_Standard,
        245,                  // clock speed
        I2C_Slave_AckAddress, // I2C2 address
        0,                    // ack address as slave
        I2C_AddressMode_7bit, // 7bit address mode
    };
    I2C_ClearITPendingBit(I2C_Master, I2C_IT_ALL);
    I2C_ITConfig(I2C_Master, I2C_IT_ALL, DISABLE);

    I2C_Init(I2C_Master, &Master_Conf);
    I2C_DMADataLevelConfig(I2C_Master, I2C_DMAReq_Tx, 8);
    // I2C_DMADataLevelConfig(I2C_Master, I2C_DMAReq_Rx, 8);
    I2C_DMACmd(I2C_Master, I2C_DMAReq_Tx, ENABLE); //| I2C_DMAReq_Rx
    DMA_Cmd(Master_Send_DMAChannel, ENABLE);
    I2C_Cmd(I2C_Master, ENABLE);
}

static void I2C_MasterDMASend_Init()
{
    DMA_InitTypeDef DMA_InitStructure;
    DMA_Cmd(Master_Send_DMAChannel, DISABLE);
    DMA_DeInit(Master_Send_DMAChannel);
    DMA_StructInit(&DMA_InitStructure);
    DMA_InitStructure.DMA_PeripheralBaseAddr  = (uint32_t) & (I2C_Master->DATA_CMD);
    DMA_InitStructure.DMA_MemoryBaseAddr      = (uint32_t)MasterSendbuf;
    DMA_InitStructure.DMA_DIR                 = DMA_DIR_PeripheralDST;
    DMA_InitStructure.DMA_BufferSize          = BUF_SIZE;
    DMA_InitStructure.DMA_PeripheralInc       = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc           = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize  = DMA_PeripheralDataSize_Byte;
    DMA_InitStructure.DMA_MemoryDataSize      = DMA_MemoryDataSize_Byte;
    DMA_InitStructure.DMA_Mode                = DMA_Mode_Normal;
    DMA_InitStructure.DMA_Priority            = DMA_Priority_High;
    DMA_InitStructure.DMA_M2M                 = DMA_M2M_Disable;
    DMA_InitStructure.DMA_PeripheralHandshake = DMA_PeripheralHandshake_I2C1_TX;
    DMA_Init(Master_Send_DMAChannel, &DMA_InitStructure);
}

/*  Master : At103_I2C1
 *  Slave  : m24256
 */
static void I2C_DMA_WriteRead(void)
{
    ZeroMemory(MasterRecvbuf);

    debug(
        "Master : At103_I2C1\n"
        "Slave  : m24256\n"
        "Master DMA Write  \n");

    I2C_MasterDMASend_Init();
    I2C_Master_Init();

    /* Wait for transfer complete ------------------------------*/
    while (DMA_GetFlagStatus(DMA_FLAG_GL1) == RESET) {
        NopDelay(0xffff);
    }
    if (DMA_GetFlagStatus(DMA_FLAG_TE1) == SET) {
        debug("Master DMA Transfer error!!!\r\n");
    }
    if (DMA_GetFlagStatus(DMA_FLAG_TC1) == SET) {
        debug("Master DMA Transfer complete!!!\r\n");
    }
    DMA_ClearFlag(DMA_FLAG_GL1);
    /* Wait for receive complete ------------------------------*/
    I2C_GenerateSTOP(I2C_Master, ENABLE);
    NopDelay(0xffff);
    I2C_SendData(I2C_Master, 0x00);
    I2C_SendData(I2C_Master, 0x00);
    // MasterRecvbuf[1] = 0x01;
    I2C_MasterPollRecv(I2C_Master, MasterRecvbuf + 2, BUF_SIZE - 2);
    /* Data Compare -------------------------------------------*/
    if (DataCompare(MasterSendbuf, MasterRecvbuf, BUF_SIZE)) {
        debug("Failed to Verify the data received from slave!!!\r\n");
    } else
        debug("Succceed to Verify the data received from slave!!!\r\n");
}

void main(void)
{
    RCC_ClocksTypeDef RCC_ClocksStatus;
    /* System Module configuration ----------------------------*/
    pll_init();
    GPIO_AFIODeInit();
    //Debug_UartIO_Init();
    sys_io_init();
    /* Debug print init ----------------------------------------*/
    uart_init(UART_DEBUG_PORT, UART_PARITY_NONE, UART_STOPBITS_1, UART_DATABITS_8, UART_BOOT_BD);
    debug("debug print ok!!!\r\n");
    /* test start ----------------------------------------------*/
    I2C_DMA_WriteRead();

    while (1) {
    }
}
