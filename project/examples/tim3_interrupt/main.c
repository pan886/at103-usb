/**
 * @file main.c
 * @brief TIM Interrupt example use TIM3:Use TIM3 timer interrupt to control LED flip
 * @author Liyouchaung (lycdemeng@163.com)
 * @version 1.0
 * @date 2022-03-31
 * @copyright Copyright (c) Icore, Inc
 */

#include "at103.h"
#include "at103_it.h"
/* Private variables ---------------------------------------------------------*/
USART_InitTypeDef       USART_InitStructure;
GPIO_InitTypeDef        GPIO_InitStructure;
TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
NVIC_InitTypeDef        NVIC_InitStructure;

void delay()
{
    uint16_t i, j;
    for (i = 0; i < 0xfff; i++)
        for (j = 0; j < 0x1ff; j++)
            ;
}

void main(void)
{
    pll_init();
    sys_io_init();
    uart_init(UART_BOOT_PORT, UART_PARITY_NONE, UART_STOPBITS_1, UART_DATABITS_8, UART_BOOT_BD);

    /* NVIC_IRQ configuration */
    NVIC_InitStructure.NVIC_IRQChannel                   = TIM3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    __enable_irq();

    /* GPIO configuration */
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_2 | GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    /*initialization pin PE2 PE3 PE4 PE5 PE6*/
    GPIO_Init(GPIOE, &GPIO_InitStructure);
    /* TIM3 clock enable */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

    /* Time Base configuration */
    TIM_TimeBaseStructure.TIM_Period        = 4999;
    TIM_TimeBaseStructure.TIM_Prescaler     = 7199;
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode   = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
    /* Enables TIM3 interrupt */
    TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);

    /* TIM3 counter enable */
    TIM_Cmd(TIM3, ENABLE);

    while (1) {
        GPIO_SetBits(GPIOE, GPIO_Pin_2);
        delay();
        GPIO_ResetBits(GPIOE, GPIO_Pin_2);
        delay();
    }
}

#ifdef USE_FULL_ASSERT

/**
 * @brief Reports the name of the source file and the source line number
 *        where the assert_param error has occurred.
 * @param[in] file: pointer to the source file name
 * @param[in] line: assert_param error line source number
 */
void assert_failed(uint8_t *file, uint32_t line)
{
    /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

    debug("Wrong parameters value: file %s on line %d\r\n", file, line);
    /* Infinite loop */
    while (1) {
    }
}

#endif