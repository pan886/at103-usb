/**
 * @file main.c
 * @brief dma memory to memory transfer example in interrupt mode.
 * @author Dong Qin (dong.qin@timesintelli.com)
 * @version 1.0
 * @date 2022-01-07
 * @copyright Copyright (c) 2022 Timesintelli, Inc
 */

#include "at103.h"
#include "at103_it.h"

#define BUF_SIZE 100

static int8_t Buffercmp(const uint32_t *pBuffer0, const uint32_t *pBuffer1, uint32_t BufferLength);

__IO uint32_t   DMA_Channel1_TransferDoneFlag = 0;
static uint32_t src_buf[BUF_SIZE];
static uint32_t dst_buf[BUF_SIZE];

void main(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;
    DMA_InitTypeDef  DMA_InitStructure;

    pll_init();
    sys_io_init();
    uart_init(UART_BOOT_PORT, UART_PARITY_NONE, UART_STOPBITS_1, UART_DATABITS_8, UART_BOOT_BD);

    RCC_APB2PeriphClockCmd(RCC_APB2PeriphRest_NVIC, ENABLE);

    /* Enable DMA IRQ Channel */
    NVIC_InitStructure.NVIC_IRQChannel                   = DMA_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    __enable_irq();

    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA, ENABLE);

    DMA_DeInit(DMA_Channel1);
    DMA_StructInit(&DMA_InitStructure);
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)src_buf;
    DMA_InitStructure.DMA_MemoryBaseAddr     = (uint32_t)dst_buf;
    DMA_InitStructure.DMA_DIR                = DMA_DIR_PeripheralSRC;
    DMA_InitStructure.DMA_BufferSize         = BUF_SIZE;
    DMA_InitStructure.DMA_PeripheralInc      = DMA_PeripheralInc_Enable;
    DMA_InitStructure.DMA_MemoryInc          = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;
    DMA_InitStructure.DMA_MemoryDataSize     = DMA_MemoryDataSize_Word;
    DMA_InitStructure.DMA_Mode               = DMA_Mode_Normal;
    DMA_InitStructure.DMA_Priority           = DMA_Priority_High;
    DMA_InitStructure.DMA_M2M                = DMA_M2M_Enable;
    DMA_Init(DMA_Channel1, &DMA_InitStructure);

    /* Enable DMA Channel1 Transfer interrupt */
    DMA_ITConfig(DMA_Channel1, DMA_IT_TC | DMA_IT_TE, ENABLE);

    debug("DMA transfer data length: %d\r\n", DMA_GetCurrDataCounter(DMA_Channel1));

    /* Init dma transfer buffer data */
    for (uint32_t i = 0; i < BUF_SIZE; i++) {
        src_buf[i] = i;
        dst_buf[i] = ~i;
    }

    DMA_Channel1_TransferDoneFlag = 0;
    /* Enable DMA Channel transfer */
    DMA_Cmd(DMA_Channel1, ENABLE);

    /* Wait for transmission to complete */
    while (DMA_Channel1_TransferDoneFlag == 0) {
    }

    /* Check if the transmitted and received data are equal */
    if (Buffercmp(src_buf, dst_buf, BUF_SIZE) == 0) {
        debug("DMA transfer data verify PASS.\r\n");
    } else {
        debug("DMA transfer data verify FAIL.\r\n");
    }

    while (1) {
    }
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
    /* Infinite loop */
    while (1) {
    }
}
#endif
