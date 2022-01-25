/**
 * @file main.c
 * @brief interrupt controller test
 * @author Dong Qin (dong.qin@timesintelli.com)
 * @version 1.0
 * @date 2022-01-07
 * @copyright Copyright (c) 2022 Timesintelli, Inc
 */

#include "at103.h"
#include "at103_it.h"

__IO uint32_t interrupt_flag;

/**
 * @brief Software forcibly triggers an interrupt.
 */
static void NVIC_ForceEnable(IRQn_Type IRQn)
{
    __IO uint32_t reg;
    reg = NVIC->IRQ_INTFORCE_L;
    reg |= 0x01 << IRQn;
    NVIC->IRQ_INTFORCE_L = reg;
}

void main(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;
    uint32_t         temp;

    pll_init();
    sys_io_init();
    uart_init(UART_BOOT_PORT, UART_PARITY_NONE, UART_STOPBITS_1, UART_DATABITS_8, UART_BOOT_BD);

    /* interrupt function test */
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
    interrupt_flag                                       = WWDG_IRQn;
    __enable_irq();
    while (1) {
        NVIC_InitStructure.NVIC_IRQChannel = interrupt_flag;
        NVIC_Init(&NVIC_InitStructure);
        temp = interrupt_flag;
        NVIC_ForceEnable(interrupt_flag);
        NopDelay(10000);
        if (interrupt_flag == (temp + 1)) {
            debug("IRQ[0x%02x]The interrupt function completes execution !!!\r\n", temp);
        } else {
            debug("Interrupt function test fail !!!\r\n");
            break;
        }
        if (interrupt_flag == (USB_DMA_IRQn + 1)) {
            debug("Interrupt function test pass !!!\r\n");
            break;
        }
    }
    __disable_irq();
    /* Triggering software interrupt */
    __SWI();
}
