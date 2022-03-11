/**
 * @file main.c
 * @brief interrupt controller test case.
 * @author Dong Qin (dong.qin@timesintelli.com)
 * @version 1.0
 * @date 2022-03-01
 * @copyright Copyright (c) 2022 Timesintelli, Inc
 *
 * Test function    : Trigger all interrupts in turn.
 * Board Version    ：AiP32rv001_EVB_V01
 * Hardware setup   : Without.
 */

#include "at103.h"
#include "at103_it.h"
#include "unity.h"

__IO uint32_t interrupt_flag;
__IO uint32_t error_flag;

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

void test_func(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;
    uint32_t         temp;
    error_flag = 0;
    /* interrupt function test */
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
    interrupt_flag                                       = WWDG_IRQn;
    __enable_irq();
    for (uint32_t i = 0; i < 0x1000; i++) {
        NVIC_InitStructure.NVIC_IRQChannel = interrupt_flag;
        NVIC_Init(&NVIC_InitStructure);
        temp = interrupt_flag;
        NVIC_ForceEnable(interrupt_flag);
        NopDelay(10000);
        if (interrupt_flag == (temp + 1)) {
            debug("IRQ[0x%02x]The interrupt function completes execution !!!\r\n", temp);
        } else {
            TEST_ASSERT_MESSAGE(0, "Interrupt controller function test fail !!!");
        }
        if (interrupt_flag == (USB_DMA_IRQn + 1)) {
            break;
        }
    }
    interrupt_flag                     = WWDG_IRQn;
    NVIC_InitStructure.NVIC_IRQChannel = interrupt_flag;
    NVIC_Init(&NVIC_InitStructure);
    NVIC_DisableIRQ(WWDG_IRQn);
    NVIC_ForceEnable(WWDG_IRQn);
    NopDelay(10000);
    TEST_ASSERT_MESSAGE(interrupt_flag == WWDG_IRQn, "Interrupt function test fail: NVIC_DisableIRQ");
    NVIC_EnableIRQ(WWDG_IRQn);
    NopDelay(10000);
    __disable_irq();
    TEST_ASSERT_MESSAGE(interrupt_flag == WWDG_IRQn + 1, "Interrupt function test fail: NVIC_EnableIRQ");

    debug("Interrupt function test pass !!!\r\n");
}

/*****************************************************************************/
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
    debug("Interrupt controller test start!\r\n");
    unity_test();
    while (1) {
        /* test end */
    }
}
