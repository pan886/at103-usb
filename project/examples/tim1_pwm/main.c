/**
 * @file main.c
 * @brief TIM PWM example:Using timer interrupt generated PWM to control LED brightness.
 * Connect GND to The negative of LED;
 * Connect PE9(TIM1_CH1) to The positive of LED;
 * @author Liyouchuang (lycdemeng@163.com)
 * @version 1.0
 * @date 2022-04-04
 * @copyright Copyright (c) 2022 Icore, Inc
 */

#include "at103.h"

/* Private variables ---------------------------------------------------------*/
TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
TIM_OCInitTypeDef       TIM_OCInitStructure;
USART_InitTypeDef       USART_InitStructure;
GPIO_InitTypeDef        GPIO_InitStructure;

void main(void)
{
    uint16_t pwmvalue = 0;
    uint8_t  dir      = 1;
    pll_init();
    sys_io_init();
    uart_init(UART_BOOT_PORT, UART_PARITY_NONE, UART_STOPBITS_1, UART_DATABITS_8, UART_BOOT_BD);

    /* Internal clock to provide the clock source for TIM1 */
    TIM_InternalClockConfig(TIM1);

    /* TIM1 REMAP */
    GPIO_PinRemapConfig(GPIO_FullRemap_TIM1, ENABLE);
    /* GPIO configuration */
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOE, &GPIO_InitStructure);

    /* TIM1, GPIOE clock enable */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1 | RCC_APB2Periph_GPIOE, ENABLE);

    /* Time Base configuration */
    TIM_TimeBaseStructure.TIM_Prescaler     = 0;
    TIM_TimeBaseStructure.TIM_CounterMode   = TIM_CounterMode_Up;
    TIM_TimeBaseStructure.TIM_Period        = 899;
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);
    /* Enables TIM1 interrupt */
    TIM_ITConfig(TIM1, TIM_IT_Update, ENABLE);

    /* Channel 3 Configuration in PWM mode */
    TIM_OCInitStructure.TIM_OCMode      = TIM_OCMode_PWM2;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_OCPolarity  = TIM_OCPolarity_High;
    TIM_OC1Init(TIM1, &TIM_OCInitStructure);

    /* Enable the preloaded register of TIM1 on CCR1 */
    TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);

    /* Enable the ARR Preload */
    TIM_ARRPreloadConfig(TIM1, ENABLE);

    /* TIM1 counter enable */
    TIM_Cmd(TIM1, ENABLE);

    /* TIM1 PWM Output Enable */
    TIM_CtrlPWMOutputs(TIM1, ENABLE);

    while (1) {

        if (dir) {
            pwmvalue++;
        } else {
            pwmvalue--;
        }

        if (pwmvalue > 899) {
            dir = 0;
        }
        if (pwmvalue == 0) {
            dir = 1;
        }
        /* SET CCR1 count */
        TIM_SetCompare1(TIM1, pwmvalue);
        debug("PWMOutput = %x\r\n", pwmvalue);
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