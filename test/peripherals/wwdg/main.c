/**
 * @file main.c
 * @brief This example provides an interrupt implementation of the watchdog 
 * First, light up the led at the beginning of the program, then start the watchdog, 
 * write the value in the counter, enter the interrupt when the countdown ends and 
 * turn off the led in the interrupt service function 
 * @author pantianwen (pantianwen@163.com)
 * @version 1.0
 * @date 2022-03-17
 * @copyright Copyright (c) 2022 Icore, Inc
 */

#include "at103.h"
#include "unity.h"

void    WWDG_Init(void);
uint8_t interrupt_counter = 0;
uint8_t reset_counter     = 100;
void    test_func()
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

    reset_counter = 200;

    GPIO_InitTypeDef GPIOE_struct1;
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
    reset_counter = 200;
    NopDelay(100000);
    TEST_ASSERT_MESSAGE(reset_counter == 200, "WWDG reset function test fail!");
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