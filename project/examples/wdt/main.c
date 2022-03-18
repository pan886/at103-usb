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

__IO uint32_t interrupt_flag;

GPIO_InitTypeDef GPIOE_struct1;
GPIO_InitTypeDef GPIOE_struct2;
GPIO_InitTypeDef GPIOA_struct;
void             delay(void);
void             main(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;
    AFIO_TypeDef     AFIO_InitStructure;
    uint32_t         PE_state   = 0;
    uint8_t          PE14_state = 0;

    pll_init();
    sys_io_init();
    uart_init(UART_BOOT_PORT, UART_PARITY_NONE, UART_STOPBITS_1, UART_DATABITS_8, UART_BOOT_BD);
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
    interrupt_flag                                       = WWDG_IRQn;
    NVIC_InitStructure.NVIC_IRQChannel                   = interrupt_flag;
    NVIC_Init(&NVIC_InitStructure);
    __enable_irq();

    /*set the GPIO mode :Push-pull output */
    GPIOE_struct1.GPIO_Mode  = GPIO_Mode_Out_PP;
    GPIOE_struct1.GPIO_Pin   = GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6;
    GPIOE_struct1.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_Init(GPIOE, &GPIOE_struct1);
    /*light up the LED*/
    GPIO_SetBits(GPIOE, GPIO_Pin_2);
    GPIO_SetBits(GPIOE, GPIO_Pin_3);
    GPIO_SetBits(GPIOE, GPIO_Pin_4);
    GPIO_SetBits(GPIOE, GPIO_Pin_5);
    GPIO_SetBits(GPIOE, GPIO_Pin_6);

    delay();
    /*enable the WDT*/
    WWDG_Enable();
    /*set the mode of WDT*/
    WWDG_SetMode(INTERRUPT_MODE);
    /*set the prescale divisor*/
    WWDG_SetPrescaler(WWDG_Higher_Prescaler_2, WWDG_Lower_Prescaler_1);
    /*set the length of reset pluse*/
    WWDG_Set_Reset_Pulselength(0xffff);
    /*Reloads WWDG counter*/
    WWDG_SetReload(0xAAAAA);
    /*set the timeout range*/
    WWDG_Set_Timeout_range(Counter_Cycles_64);
    /*Restart the WWDG counter.*/
    WWDG_ReloadCounter();

    while (1) {
    }
}

void WWDG_IRQHandler()
{
    /* the interrupt status of the WDT*/
    WWDG_ClearFlag();
    debug("enter WDT interrupt!\n");
    /*change the GPIO mode,turn off the led */
    GPIOE_struct1.GPIO_Mode = GPIO_Mode_IPD;
    GPIO_Init(GPIOE, &GPIOE_struct1);
}

void delay()
{
    uint16_t i, j;
    for (i = 0; i < 0xffff; i++)
        for (j = 0; j < 0x1ff; j++)
            ;
}