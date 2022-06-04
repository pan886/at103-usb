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

#define I2C_Master I2C1

#define I2C_TargetAddress (0x50)

#define RCC_I2C_Master RCC_APB1Periph_I2C1

/* Private variables ---------------------------------------------------------*/
uint8_t Mastersendbuf[BUF_SIZE] = {
    0xcc, 0x55, 0xaa, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 0xff,
    1, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
    1, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
    1, 2, 3, 4, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 0xff};
uint8_t         Masterrecvbuf[BUF_SIZE];
static uint8_t *mastersendbufpointer = Mastersendbuf;
/* Private function prototypes -----------------------------------------------*/
static uint8_t DataCompare(const void *Src, const void *Dst, uint32_t length);
static void    I2C_Master_Init(void);

/* Private functions ---------------------------------------------------------*/
static uint8_t DataCompare(const void *Src, const void *Dst, uint32_t length)
{
    while (*(uint8_t *)Src++ == *(uint8_t *)Dst++)
        if (--length == 0)
            return 0;
    debug("Err Data is 0x%x \n", *(uint8_t *)Src);
    return length;
}
static void I2C_Master_Init(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;
    /* I2C_Master GPIO Pin configuration ----------------------------------------------*/
    GPIO_InitTypeDef IIC_Pin = {
        GPIO_Pin_6 | GPIO_Pin_7,
        GPIO_Speed_10MHz,
        GPIO_Mode_AF,
    };
    GPIO_Init(GPIOB, &IIC_Pin);
    /* NVIC  init ---------------------------------------------------------------*/
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
    NVIC_InitStructure.NVIC_IRQChannel                   = I2C1_IRQn;
    NVIC_Init(&NVIC_InitStructure);
    /* I2C_Master Module configuration ----------------------------------------------*/
    I2C_InitTypeDef Master_Conf = {
        I2C_Mode_Master,
        I2C_SpeedMode_Standard,
        245,                  // clock speed
        I2C_TargetAddress,    // I2C2 target addr as master
        0,                    // ack address as slave
        I2C_AddressMode_7bit, // 7 bit address mode
    };
    I2C_DeInit(I2C_Master);
    RCC_APB1PeriphClockCmd(RCC_I2C_Master, ENABLE);

    /* Clear All Intrrupt        ----------------------------------------------*/
    I2C_ClearITPendingBit(I2C_Master, I2C_IT_ALL);
    I2C_ITConfig(I2C_Master, I2C_IT_ALL, DISABLE);

    I2C_ITConfig(I2C_Master, I2C_IT_TXEMPTY, ENABLE);
    I2C_Init(I2C_Master, &Master_Conf);
    I2C_Cmd(I2C_Master, ENABLE);
}
/* Master : At103_I2C1
 * Slave  : m24256
 */

static void I2C_IntWriteRead(void)
{
    ZeroMemory(Masterrecvbuf);
    debug(
        "Master : At103_I2C1\n"
        "Slave  : m24256\n");
    I2C_Master_Init();
    I2C_SendData(I2C_Master, 0x00);
    I2C_SendData(I2C_Master, 0x00);
    debug("Master is sending  data!!!\r\n");
    /* Enable System Intrrupt ----------------------------------------------*/
    __enable_irq();
    /* Test Start ----------------------------------------------*/
    debug("Master is sending  data!!!\r\n");

    while (mastersendbufpointer < Mastersendbuf + 63) {
    }
    __disable_irq();
    NopDelay(0xffff);

    /* Wait for receive complete ------------------------------*/
    I2C_SendData(I2C_Master, 0x00);
    I2C_SendData(I2C_Master, 0x00);
    debug("Master is searching address!!!\r\n");
    I2C_MasterPollRecv(I2C_Master, Masterrecvbuf, BUF_SIZE);

    if (DataCompare(Mastersendbuf, Masterrecvbuf, BUF_SIZE)) {
        debug("Failed to Verify the data received from slave!!!\r\n");
    } else
        debug("Succceed to Verify the data received from slave!!!\r\n");

    // for (int i = 0; i < 64; i++)

    // {
    //     debug("%x %x \n", Mastersendbuf[i], Masterrecvbuf[i]);
    // }
}

void main(void)
{
    RCC_ClocksTypeDef RCC_ClocksStatus;
    /* System Module configuration ----------------------------------------------*/
    pll_init();
    GPIO_AFIODeInit();
    sys_io_init();
    /* Debug print init ---------------------------------------------------------*/
    uart_init(UART_DEBUG_PORT, UART_PARITY_NONE, UART_STOPBITS_1, UART_DATABITS_8, UART_BOOT_BD);
    debug("debug print ok!!!\r\n");
    /* use interrupt write and read---------------------------------------------------------*/
    I2C_IntWriteRead();

    while (1) {
    }
}
void I2C1_IRQHandler(void)
{

    if (I2C_GetITStatus(I2C_Master, I2C_IT_TXEMPTY)) {
        /* Transfer empty */

        I2C_SendData(I2C_Master, *mastersendbufpointer++);

        if (mastersendbufpointer > Mastersendbuf + 62) {
            I2C_SendlastData(I2C_Master, *mastersendbufpointer++);
            I2C_ITConfig(I2C_Master, I2C_IT_ALL, DISABLE);
        }
    }

    // clear all interrupt
    I2C_ClearITPendingBit(I2C_Master, I2C_IT_ALL);
}