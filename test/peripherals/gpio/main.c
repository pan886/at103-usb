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
#include "unity.h"

__IO uint32_t interrupt_flag;
__IO uint32_t error_flag;

/**
 * @brief Software forcibly triggers an interrupt.
 */
// static void NVIC_ForceEnable(IRQn_Type IRQn)
// {
//     __IO uint32_t reg;
//     reg = NVIC->IRQ_INTFORCE_L;
//     reg |= 0x01 << IRQn;
//     NVIC->IRQ_INTFORCE_L = reg;
// }

void test_func(void)
{
    debug("GPIO  pass !!!\r\n");
    GPIO_InitTypeDef GPIOE_struct;
    uint32_t         GPIOE_PC;
    uint32_t         GPIOE_Func;
    uint32_t         GPIOE_OD;
    uint32_t         GPIOE_PP;
    uint32_t         GPIOE_DR;
    uint32_t         GPIOE_outputdata;
    uint32_t         GPIOE_inputdata;
    uint32_t         Pinmap_SPI;
    GPIOE_struct.GPIO_Pin   = GPIO_Pin_All; //GPIO_Pin_15;
    GPIOE_struct.GPIO_Mode  = GPIO_Mode_IPD;
    GPIOE_struct.GPIO_Speed = GPIO_Speed_2MHz;

    // GPIO_Init(GPIOE, &GPIOE_struct);
    // NVIC_InitTypeDef NVIC_InitStructure;
    // uint32_t         temp;
    // error_flag = 0;
    // /* interrupt function test */
    // NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    // NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 0;
    // NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
    // interrupt_flag                                       = WWDG_IRQn;
    // __enable_irq();
    // for (uint32_t i = 0; i < 0x1000; i++) {
    //     NVIC_InitStructure.NVIC_IRQChannel = interrupt_flag;
    //     NVIC_Init(&NVIC_InitStructure);
    //     temp = interrupt_flag;
    //     NVIC_ForceEnable(interrupt_flag);
    //     NopDelay(10000);
    //     if (interrupt_flag == (temp + 1)) {
    //         debug("IRQ[0x%02x]The interrupt function completes execution !!!\r\n", temp);
    //     } else {
    //         TEST_ASSERT_MESSAGE(0, "Interrupt controller function test fail !!!");
    //     }
    //     if (interrupt_flag == (USB_DMA_IRQn + 1)) {
    //         break;
    //     }
    // }
    // interrupt_flag                     = WWDG_IRQn;
    // NVIC_InitStructure.NVIC_IRQChannel = interrupt_flag;
    // NVIC_Init(&NVIC_InitStructure);
    // NVIC_DisableIRQ(WWDG_IRQn);
    // NVIC_ForceEnable(WWDG_IRQn);
    // NopDelay(10000);
    // TEST_ASSERT_MESSAGE(interrupt_flag == WWDG_IRQn, "Interrupt function test fail: NVIC_DisableIRQ");
    // NVIC_EnableIRQ(WWDG_IRQn);
    // NopDelay(10000);
    // __disable_irq();
    // TEST_ASSERT_MESSAGE(interrupt_flag == WWDG_IRQn + 1, "Interrupt function test fail: NVIC_EnableIRQ");

    /*pull down/up and float in test*/
#if 1
    GPIOE_PC = *((uint32_t *)(AFIO_BASE + 0x48));
    TEST_ASSERT_MESSAGE(GPIOE_PC == 0xffff0000, "GPIO pulldown test fail");
    GPIOE_struct.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOE, &GPIOE_struct);
    GPIOE_PC = *((uint32_t *)(AFIO_BASE + 0x48));
    TEST_ASSERT_MESSAGE(GPIOE_PC == 0xffffffff, "GPIO pullup test fail");
    GPIOE_struct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOE, &GPIOE_struct);
    GPIOE_PC = *((uint32_t *)(AFIO_BASE + 0x48));
    TEST_ASSERT_MESSAGE(GPIOE_PC == 0x0, "GPIO float in test fail");
    debug("GPIOE_data = %x\n", GPIOE_PC);
    GPIOE_inputdata = GPIO_ReadInputData(GPIOE);
    TEST_ASSERT_MESSAGE(GPIOE_inputdata == 0xffff, "GPIO input data test fail");
    debug("GPIOE_inputdata = %x\n", GPIOE_inputdata);

    /*GPIO Analog function test*/
    GPIOE_struct.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_Init(GPIOE, &GPIOE_struct);
    GPIOE_Func = *((uint32_t *)(AFIO_BASE + 0x90));
    TEST_ASSERT_MESSAGE(GPIOE_Func == 0xffffffff, "GPIO Analog function test fail");
    debug("GPIOE_Func = %x\n", GPIOE_Func);

    GPIOE_struct.GPIO_Mode = GPIO_Mode_AF;
    GPIO_Init(GPIOE, &GPIOE_struct);
    GPIOE_Func = *((uint32_t *)(AFIO_BASE + 0x90));
    TEST_ASSERT_MESSAGE(GPIOE_Func == 0x55555555, "GPIO Alternate function test fail");
    debug("GPIOE_Func = %x\n", GPIOE_Func);

    GPIOE_struct.GPIO_Mode = GPIO_Mode_Out_OD;
    GPIO_Init(GPIOE, &GPIOE_struct);
    GPIOE_OD = *((uint32_t *)(AFIO_BASE + 0x44));
    TEST_ASSERT_MESSAGE(GPIOE_OD == 0xffff, "GPIO Open-Drain mode test fail");
    debug("GPIOE_OD = %x\n", GPIOE_OD);

    GPIOE_struct.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOE, &GPIOE_struct);
    GPIOE_PP = *((uint32_t *)(AFIO_BASE + 0x44));
    TEST_ASSERT_MESSAGE(GPIOE_PP == 0x0, "GPIO push-pull mode test fail");
    debug("GPIOE_PP = %x\n", GPIOE_PP);

    /*GPIO speed test*/
    GPIOE_struct.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_Init(GPIOE, &GPIOE_struct);
    GPIOE_DR = *((uint32_t *)(AFIO_BASE + 0x50));
    TEST_ASSERT_MESSAGE(GPIOE_DR == 0x55555555, "GPIO speed test fail");
    debug("GPIOE_DR = %x\n", GPIOE_DR);

    GPIOE_struct.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOE, &GPIOE_struct);
    GPIO_SetBits(GPIOE, GPIO_Pin_All);

    GPIOE_outputdata = GPIO_ReadOutputData(GPIOE);
    debug("GPIOE_outputdata = %x\n", GPIOE_outputdata);
    TEST_ASSERT_MESSAGE(GPIOE_outputdata == 0xffff, "setbits test fail");
    GPIO_ResetBits(GPIOE, GPIO_Pin_All);
    GPIOE_outputdata = GPIO_ReadOutputData(GPIOE);
    TEST_ASSERT_MESSAGE(GPIOE_outputdata == 0x0, "resetbits test fail");
    debug("GPIOE_outputdata = %x\n", GPIOE_outputdata);

#endif

    // GPIO_PinRemapConfig(GPIO_Remap_SPI1, ENABLE);
    // Pinmap_SPI = *((uint32_t *)(AFIO_BASE));
    // debug("Pinmap_SPI = %x\n", Pinmap_SPI);
    debug("GPIO function test pass !!!\r\n");
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
    debug("GPIO function test pass !!!\r\n"); /* test end */
    //debug("Interrupt controller test start!\r\n");

    unity_test();
    while (1) {
    }
}
