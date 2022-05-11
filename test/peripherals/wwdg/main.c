/**
 * @file main.c
 * @brief WWDG test case.
 * @author pantianwen (pantianwen@163.com)
 * @version 1.0
 * @date 2022-05-12
 * @copyright Copyright (c) 2022 Icore, Inc
 */

#include "at103.h"
#include "unity.h"

void    WWDG_Init(void);
uint8_t interrupt_counter = 0;

void test_func()
{
    uint32_t Wwdg_enable   = 0;
    uint32_t Wwdg_int_mode = 0;
    uint32_t Wwdg_rst_mode = 0;
    uint32_t Wwdg_high     = 0;
    uint32_t Wwdg_low      = 0;
    uint32_t reset_pluse   = 0;
    uint32_t WWDG_counter  = 0;
    uint32_t Timeout_range = 0;

    /*enable the WDT*/
    WWDG_Enable();
    Wwdg_enable = WWDG->TCR.ENANBLE;
    TEST_ASSERT_MESSAGE(Wwdg_enable == 1, "WWDG enable function test fail!");
    /*set the mode of WDT*/
    WWDG_SetMode(INTERRUPT_MODE);
    Wwdg_int_mode = WWDG->TCR.ACTION;
    TEST_ASSERT_MESSAGE(Wwdg_int_mode == 1, "WWDG interrupt mode test fail!");
    /*set the prescale divisor*/
    WWDG_SetPrescaler(WWDG_Higher_Prescaler_2, WWDG_Lower_Prescaler_1);
    Wwdg_high = WWDG->TCR.PSC_DIV_HIGH;
    Wwdg_low  = WWDG->TCR.PSC_DIV_LOW;
    TEST_ASSERT_MESSAGE((Wwdg_high == WWDG_Higher_Prescaler_2) && (Wwdg_low == WWDG_Lower_Prescaler_1), "WWDG interrupt mode test fail!");
    /*set the length of reset pluse*/
    WWDG_Set_Reset_Pulselength(0xffff);
    reset_pluse = WWDG->PLR.value;
    TEST_ASSERT_MESSAGE(reset_pluse == 0xffff, "WWDG reset pluse test fail!");
    /*Reloads WWDG counter*/
    WWDG_SetReload(0x3FFFFFF);
    WWDG_counter = WWDG->LDR;
    TEST_ASSERT_MESSAGE(WWDG_counter == 0x3FFFFFF, "WWDG counter test fail!");
    /*set the timeout range*/
    WWDG_Set_Timeout_range(Counter_Cycles_64);
    Timeout_range = WWDG->TCR.TIMEOUT_RANGE;
    TEST_ASSERT_MESSAGE(Timeout_range == Counter_Cycles_64, "WWDG timeout range test fail!");
    /*Restart the WWDG counter.*/
    WWDG_ReloadCounter();

    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
    NVIC_InitStructure.NVIC_IRQChannel                   = WWDG_IRQn;
    NVIC_Init(&NVIC_InitStructure);
    __enable_irq();

    if (interrupt_counter == 0) {
        NopDelay(100000);
        TEST_ASSERT_MESSAGE(interrupt_counter == 0, "WWDG Interrupt function test fail!");
    };
    debug("WWDG Interrupt function test pass !!!\r\n");

    WWDG_SetMode(RESET_MODE);
    Wwdg_rst_mode = WWDG->TCR.ACTION;
    TEST_ASSERT_MESSAGE(Wwdg_rst_mode == 0, "WWDG reset function test fail!");
    debug("WWDG reset function test pass !!!\r\n");
}

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
    debug("WWDG test start!\r\n");
    unity_test();

    while (1) {
    }
}

void WWDG_Init()
{
    /*enable the WDT*/
    WWDG_Enable();
    /*set the mode of WDT*/
    WWDG_SetMode(INTERRUPT_MODE);
    /*set the prescale divisor*/
    WWDG_SetPrescaler(WWDG_Higher_Prescaler_2, WWDG_Lower_Prescaler_1);
    /*set the length of reset pluse*/
    WWDG_Set_Reset_Pulselength(0xffff);
    /*Reloads WWDG counter*/
    WWDG_SetReload(0x3FFFFFF);
    /*set the timeout range*/
    WWDG_Set_Timeout_range(Counter_Cycles_64);
    /*Restart the WWDG counter.*/
    WWDG_ReloadCounter();
}

void WWDG_IRQHandler()
{
    debug("enter WDT interrupt!\n");
    WWDG_ClearFlag();
    interrupt_counter++;
}