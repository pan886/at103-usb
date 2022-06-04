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

#define I2C_Master            I2C1
#define I2C_Slave1            M24256
#define I2C_Slave1_AckAddress (0x50)
#define RCC_I2C_Master        RCC_APB1Periph_I2C1

/* Private variables ---------------------------------------------------------*/
uint8_t Sendbuf[BUF_SIZE] = {
    0xff, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 0xff,
    0xff, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 0xff,
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
uint8_t  Verifybuf[BUF_SIZE];
uint8_t *tempbufbp = NULL;
uint8_t *testbufbp = NULL;

/* Private function prototypes -----------------------------------------------*/
static uint8_t DataCompare(const void *Src, const void *Dst, uint32_t length);
static void    I2C_Master_Init(void);
static void    I2C_MasterPollWriteRead(void);

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
    /* I2C_Master GPIO Pin configuration ----------------------------------------------*/
    GPIO_InitTypeDef IIC_Pin = {
        GPIO_Pin_6 | GPIO_Pin_7,
        GPIO_Speed_10MHz,
        GPIO_Mode_AF,
    };
    GPIO_Init(GPIOB, &IIC_Pin);
    /* I2C_Master Module configuration ----------------------------------------------*/
    I2C_InitTypeDef Master_Conf = {
        I2C_Mode_Master,        // master mode
        I2C_SpeedMode_Standard, // standard speed mode
        245,                    // clock speed
        I2C_Slave1_AckAddress,  // m24256 addr
        0,                      // ack address as slave
        I2C_AddressMode_7bit,   // 7 bit address mode
    };
    I2C_DeInit(I2C_Master);
    RCC_APB1PeriphClockCmd(RCC_I2C_Master, ENABLE);

    /* Clear All Intrrupt        ----------------------------------------------*/
    I2C_ClearITPendingBit(I2C_Master, I2C_IT_ALL);
    I2C_ITConfig(I2C_Master, I2C_IT_ALL, DISABLE);

    I2C_Init(I2C_Master, &Master_Conf);
    I2C_Cmd(I2C_Master, ENABLE);
}
/*  Master : At103_I2C1
 *  Slave  : M24256
 */
static void I2C_MasterPollWriteRead(void)
{
    debug("Master : At103_I2C1\n"
          "Slave  : M24256\n"
          "Breaf  : Master Poll Write and Poll Read \r\n");
    /* I2C Master Configure ----------------------------------------------*/
    I2C_Master_Init();

    /* Enable System Intrrupt ----------------------------------------------*/
    __enable_irq();

    /* Test Start ----------------------------------------------*/
    debug("At103 is sending eeprom page address!!!\r\n");
    I2C_SendData(I2C_Master, 0x00);
    I2C_SendData(I2C_Master, 0x00);

    debug("At103 is sending data!!!\r\n");
    I2C_MasterPollSend(I2C_Master, Sendbuf, sizeof(Sendbuf));

    NopDelay(0xffff);
    ZeroMemory(Verifybuf);

    debug("At103 is sending eeprom page address!!!\r\n");
    I2C_SendData(I2C_Master, 0x00);
    I2C_SendData(I2C_Master, 0x00);

    debug("At103 is reading data!!!\r\n");
    I2C_MasterPollRecv(I2C_Master, Verifybuf, sizeof(Verifybuf));
    NopDelay(0xffff);
    if (DataCompare(Sendbuf, Verifybuf, BUF_SIZE)) {
        debug("Verify Failed!!!\r\n");
    }

    else
        debug("Verify OK!!!\r\n");

    I2C_ITConfig(I2C_Master, I2C_IT_ALL, DISABLE);
    __disable_irq();
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
    /* Write and read ---------------------------------------------------------*/
    I2C_MasterPollWriteRead();

    while (1) {
    }
}
