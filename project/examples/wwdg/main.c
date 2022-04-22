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

#define LED_ON  0x1
#define LED_OFF 0x0

void set_led(uint8_t led_state);
void WWDG_Init(void);
void main(void)
{
    GPIO_InitTypeDef GPIOE_struct1;
    NVIC_InitTypeDef NVIC_InitStructure;
    pll_init();
    sys_io_init();
    uart_init(UART_BOOT_PORT, UART_PARITY_NONE, UART_STOPBITS_1, UART_DATABITS_8, UART_BOOT_BD);
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
    NVIC_InitStructure.NVIC_IRQChannel                   = WWDG_IRQn;
    NVIC_Init(&NVIC_InitStructure);
    __enable_irq();

    /*set the GPIO mode :Push-pull output */
    GPIOE_struct1.GPIO_Mode  = GPIO_Mode_Out_PP;
    GPIOE_struct1.GPIO_Pin   = GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6;
    GPIOE_struct1.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_Init(GPIOE, &GPIOE_struct1);

    /*light up the LED*/
    set_led(LED_ON);
    /*Initialize the WWDG configuration */
    WWDG_Init();

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

void set_led(uint8_t led_state)
{
    if (led_state == LED_ON) {
        GPIO_SetBits(GPIOE, GPIO_Pin_2);
        GPIO_SetBits(GPIOE, GPIO_Pin_3);
        GPIO_SetBits(GPIOE, GPIO_Pin_4);
        GPIO_SetBits(GPIOE, GPIO_Pin_5);
        GPIO_SetBits(GPIOE, GPIO_Pin_6);
    } else {
        GPIO_ResetBits(GPIOE, GPIO_Pin_2);
        GPIO_ResetBits(GPIOE, GPIO_Pin_3);
        GPIO_ResetBits(GPIOE, GPIO_Pin_4);
        GPIO_ResetBits(GPIOE, GPIO_Pin_5);
        GPIO_ResetBits(GPIOE, GPIO_Pin_6);
    }
}

void WWDG_IRQHandler()
{
    /* the interrupt status of the WDT*/
    WWDG_ClearFlag();
    debug("enter WDT interrupt!\n");
    /*change the GPIO mode,turn off the led */
    set_led(LED_OFF);
}