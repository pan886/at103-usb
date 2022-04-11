/**
 * @file main.c
 * @brief dma memory to memory transfer test case.
 * @author Dong Qin (dong.qin@timesintelli.com)
 * @version 1.0
 * @date 2022-03-18
 * @copyright Copyright (c) 2022 Timesintelli, Inc
 *
 * Test function    : Repeat all channels with different configuration parameters to transfer data.
 * Board Version    ：AiP32rv001_EVB_V01
 * Hardware setup   : Without.
 */

#include "at103.h"
#include "at103_it.h"
#include "unity.h"

#define BUF_SIZE 100

static int8_t Buffercmp(const uint32_t *pBuffer0, const uint32_t *pBuffer1, uint32_t BufferLength);
static void   test_func(void);

__IO uint32_t   DMA_TransferDoneFlag = 0;
static uint32_t src_buf[BUF_SIZE];
static uint32_t dst_buf[BUF_SIZE];

void unity_test(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_func);
    UNITY_END();
}

void main(void)
{
    pll_init();
    sys_io_init();
    uart_init(UART_BOOT_PORT, UART_PARITY_NONE, UART_STOPBITS_1, UART_DATABITS_8, UART_BOOT_BD);
    debug("DMA m2m transfer test start!\r\n");
    unity_test();
    while (1) {
        /* test end */
    }
}

static void test_func(void)
{
    NVIC_InitTypeDef     NVIC_InitStructure;
    DMA_InitTypeDef      DMA_InitStructure;
    DMA_Channel_TypeDef *DMA_Channelx;
    uint16_t             buf_size;

    RCC_APB2PeriphClockCmd(RCC_APB2PeriphRest_NVIC, ENABLE);

    /* Enable DMA IRQ Channel */
    NVIC_InitStructure.NVIC_IRQChannel                   = DMA_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    __enable_irq();

    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA, ENABLE);
    DMA_StructInit(&DMA_InitStructure);

    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)src_buf;
    DMA_InitStructure.DMA_MemoryBaseAddr     = (uint32_t)dst_buf;
    DMA_InitStructure.DMA_Priority           = DMA_Priority_High;
    DMA_InitStructure.DMA_M2M                = DMA_M2M_Enable;
    DMA_InitStructure.DMA_PeripheralInc      = DMA_PeripheralInc_Enable;
    DMA_InitStructure.DMA_MemoryInc          = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_Mode               = DMA_Mode_Normal;

    DMA_TransferDoneFlag = 0;

    for (uint32_t i = 0; i < 0x100; i++) {
        if ((i & 0x1) != 0) {
            DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
        } else {
            DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
        }
        if ((i & 0x6) == 0) {
            DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;
            DMA_InitStructure.DMA_MemoryDataSize     = DMA_MemoryDataSize_Word;
            DMA_InitStructure.DMA_BufferSize         = BUF_SIZE;
        } else if ((i & 0x6) == 0x02) {
            DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
            DMA_InitStructure.DMA_MemoryDataSize     = DMA_MemoryDataSize_HalfWord;
            DMA_InitStructure.DMA_BufferSize         = BUF_SIZE << 1;
        } else {
            DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
            DMA_InitStructure.DMA_MemoryDataSize     = DMA_MemoryDataSize_Byte;
            DMA_InitStructure.DMA_BufferSize         = BUF_SIZE << 2;
        }
        switch (i & 0x38) {
        case 0x0:
            DMA_Channelx = DMA_Channel1;
            break;
        case 0x8:
            DMA_Channelx = DMA_Channel2;
            break;
        case 0x10:
            DMA_Channelx = DMA_Channel3;
            break;
        case 0x18:
            DMA_Channelx = DMA_Channel4;
            break;
        case 0x20:
            DMA_Channelx = DMA_Channel5;
            break;
        case 0x28:
            DMA_Channelx = DMA_Channel6;
            break;
        case 0x30:
            DMA_Channelx = DMA_Channel7;
            break;
        case 0x38:
            DMA_Channelx = DMA_Channel8;
            break;
        }
        DMA_DeInit(DMA_Channelx);
        DMA_Init(DMA_Channelx, &DMA_InitStructure);
        /* Enable DMA Channelx Transfer interrupt */
        DMA_ITConfig(DMA_Channelx, DMA_IT_TC | DMA_IT_TE, ENABLE);
        /* Init dma transfer buffer data */
        for (uint32_t i = 0; i < BUF_SIZE; i++) {
            src_buf[i] = i;
            dst_buf[i] = ~i;
        }
        /* Enable DMA Channel transfer */
        DMA_Cmd(DMA_Channelx, ENABLE);
        /* Wait for transmission to complete */
        NopDelay(100000);
        TEST_ASSERT_MESSAGE(DMA_TransferDoneFlag == i + 1, "DMA interrupt did't fire.");
        DMA_ITConfig(DMA_Channelx, DMA_IT_TC | DMA_IT_TE, DISABLE);
        /* Check if the transmitted and received data are equal */
        TEST_ASSERT_MESSAGE(Buffercmp(src_buf, dst_buf, BUF_SIZE) == 0, "DMA transfer data verify FAIL.");
        debug(".");
    }
    debug("\r\nDone.\r\n");
}

static int8_t Buffercmp(const uint32_t *pBuffer0, const uint32_t *pBuffer1, uint32_t BufferLength)
{
    while (BufferLength--) {
        if (*pBuffer0 != *pBuffer1) {
            return -1;
        }
        pBuffer0++;
        pBuffer1++;
    }
    return 0;
}

#ifdef USE_FULL_ASSERT
void assert_failed(uint8_t *file, uint32_t line)
{
    debug("Wrong parameters value: file %s on line %d\r\n", file, line);
    TEST_ASSERT_MESSAGE(0, "DMA test FAIL.");
}
#endif
