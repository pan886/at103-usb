/**
 * @file main.c
 * @brief Light up the running lights and get input for PE port 
 * @author pantianwen (pantianwen@163.com)
 * @version 1.0
 * @date 2022-03-16
 * @copyright Copyright (c) 2022 Icore, Inc
 */

#include "at103.h"
GPIO_InitTypeDef GPIOE_struct1;
GPIO_InitTypeDef GPIOE_struct2;
uint32_t         PE_state, PE14_state;
void             GPIO_test(void);
void             main(void)
{
    pll_init();
    sys_io_init();
    uart_init(UART_BOOT_PORT, UART_PARITY_NONE, UART_STOPBITS_1, UART_DATABITS_8, UART_BOOT_BD);
    /*Push-pull output mode */
    GPIOE_struct1.GPIO_Mode  = GPIO_Mode_Out_PP;
    GPIOE_struct1.GPIO_Pin   = GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6;
    GPIOE_struct1.GPIO_Speed = GPIO_Speed_2MHz;
    /*initialization pin PE2 PE3 PE4 PE5 PE6*/
    GPIO_Init(GPIOE, &GPIOE_struct1);

    GPIOE_struct2.GPIO_Mode = GPIO_Mode_IPD;
    GPIOE_struct2.GPIO_Pin  = GPIO_Pin_14;
    /*initialization pin PE14 */
    GPIO_Init(GPIOE, &GPIOE_struct2);

    while (1) {
        /*get the state of PE*/
        PE_state = GPIO_ReadInputData(GPIOE);
        /*get the state of PE14*/
        PE14_state = GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_14);

        debug("PE_state = %x\r\n", PE_state);

        debug("PE14_state = %x\r\n", PE14_state);

        /*running water lamp,PE2,PE3,PE4,PE5*/
        GPIO_test();
    }
}

void GPIO_Toggle(GPIO_TypeDef *GPIOx, uint16_t PortVal)
{
    GPIOx->DATA ^= PortVal;
}
void delay()
{
    uint16_t i, j;
    for (i = 0; i < 0xfff; i++)
        for (j = 0; j < 0x1ff; j++)
            ;
}
void GPIO_test()
{
    GPIO_Toggle(GPIOE, GPIO_Pin_2);
    delay();
    GPIO_Toggle(GPIOE, GPIO_Pin_3);
    delay();
    GPIO_Toggle(GPIOE, GPIO_Pin_4);
    delay();
    GPIO_Toggle(GPIOE, GPIO_Pin_5);
    delay();
    GPIO_Toggle(GPIOE, GPIO_Pin_6);
    delay();
    GPIO_Toggle(GPIOE, GPIO_Pin_5);
    delay();
    GPIO_Toggle(GPIOE, GPIO_Pin_4);
    delay();
    GPIO_Toggle(GPIOE, GPIO_Pin_3);
    delay();
    GPIO_Toggle(GPIOE, GPIO_Pin_2);
    delay();
}