/**
 * @file main.c
 * @brief TIM1 DMA example:memory to Peripheral transfer example .
 * @author Liyouchuang (lycdemeng@163.com)
 * @version 1.0
 * @date 2022-04-04
 * @copyright Copyright (c) 2022 Icore, Inc
 */

#include "at103.h"
#include "at103_it.h"
/* Private variables ---------------------------------------------------------*/
uint16_t      SRC_Buffer[3]                 = {0, 0, 0};
uint16_t      TimerPeriod                   = 0;
__IO uint32_t DMA_Channel1_TransferDoneFlag = 0;

void main(void)
{
    pll_init();
    sys_io_init();
    uart_init(UART_BOOT_PORT, UART_PARITY_NONE, UART_STOPBITS_1, UART_DATABITS_8, UART_BOOT_BD);
    /* TIM1, GPIOA and GPIOB clock enable */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1 | RCC_APB2Periph_GPIOE, ENABLE);
    /* DMA clock enable */
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA, ENABLE);

    GPIO_InitTypeDef GPIO_InitStructure;
    /* TIM1 REMAP */
    GPIO_PinRemapConfig(GPIO_FullRemap_TIM1, ENABLE);
    /* GPIOA Configuration: Channel 1 as alternate function push-pull */
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOE, &GPIO_InitStructure);

    /* GPIOB Configuration: Channel 1N as alternate function push-pull */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
    GPIO_Init(GPIOE, &GPIO_InitStructure);

    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel                   = DMA_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    __enable_irq();

    DMA_InitTypeDef DMA_InitStructure;
    /* DMA Channel1 Config */
    DMA_DeInit(DMA_Channel1);
    // DMA_StructInit(&DMA_InitStructure);
    DMA_InitStructure.DMA_PeripheralBaseAddr  = (uint32_t)TIM1->TIM_CCR1.value;
    DMA_InitStructure.DMA_MemoryBaseAddr      = (uint32_t)SRC_Buffer;
    DMA_InitStructure.DMA_DIR                 = DMA_DIR_PeripheralDST;
    DMA_InitStructure.DMA_BufferSize          = 3;
    DMA_InitStructure.DMA_PeripheralInc       = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc           = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize  = DMA_PeripheralDataSize_Word;
    DMA_InitStructure.DMA_MemoryDataSize      = DMA_MemoryDataSize_Word;
    DMA_InitStructure.DMA_Mode                = DMA_Mode_Circular;
    DMA_InitStructure.DMA_Priority            = DMA_Priority_High;
    DMA_InitStructure.DMA_M2M                 = DMA_M2M_Disable;
    DMA_InitStructure.DMA_PeripheralHandshake = DMA_PeripheralHandshake_TIM1_CH1;
    DMA_Init(DMA_Channel1, &DMA_InitStructure);
    /* Enable DMA Channel1 Transfer interrupt */
    DMA_ITConfig(DMA_Channel1, DMA_IT_TC | DMA_IT_TE, ENABLE);

    /* TIM1 DMA Transfer example -------------------------------------------------
  TIM1CLK = SystemCoreClock, Prescaler = 0, TIM1 counter clock = SystemCoreClock
  SystemCoreClock is set to 72 MHz for Low-density, Medium-density, High-density
  and Connectivity line devices and to 24 MHz for Low-Density Value line and
  Medium-Density Value line devices.

  The objective is to configure TIM1 channel 3 to generate complementary PWM
  signal with a frequency equal to 17.57 KHz:
     - TIM1_Period = (SystemCoreClock / 17570) - 1
  and a variable duty cycle that is changed by the DMA after a specific number of
  Update DMA request.

  The number of this repetitive requests is defined by the TIM1 Repetition counter,
  each 3 Update Requests, the TIM1 Channel 3 Duty Cycle changes to the next new 
  value defined by the SRC_Buffer .
  
  The waveform can be output by oscilloscope. */

    /* Compute the value to be set in ARR register to generate signal frequency at 17.57 Khz */
    TimerPeriod = (72000000 / 17570) - 1;
    /* Compute CCR1 value to generate a duty cycle at 50% */
    SRC_Buffer[0] = (uint16_t)(((uint32_t)5 * (TimerPeriod - 1)) / 10);
    /* Compute CCR1 value to generate a duty cycle at 37.5% */
    SRC_Buffer[1] = (uint16_t)(((uint32_t)375 * (TimerPeriod - 1)) / 1000);
    /* Compute CCR1 value to generate a duty cycle at 25% */
    SRC_Buffer[2] = (uint16_t)(((uint32_t)25 * (TimerPeriod - 1)) / 100);

    /* TIM1 Peripheral Configuration --------------------------------------------*/
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    /* Time Base configuration */
    TIM_TimeBaseStructure.TIM_Prescaler         = 0;
    TIM_TimeBaseStructure.TIM_CounterMode       = TIM_CounterMode_Up;
    TIM_TimeBaseStructure.TIM_Period            = TimerPeriod;
    TIM_TimeBaseStructure.TIM_ClockDivision     = 0;
    TIM_TimeBaseStructure.TIM_RepetitionCounter = 2;
    TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);

    debug("TIM_Period  = %x\r\n", TimerPeriod);
    debug("SRC_Buffer0  = %x\r\n", SRC_Buffer[0]);
    debug("SRC_Buffer1  = %x\r\n", SRC_Buffer[1]);
    debug("SRC_Buffer2  = %x\r\n", SRC_Buffer[2]);

    TIM_OCInitTypeDef TIM_OCInitStructure;
    /* Channel 1 Configuration in PWM mode */
    TIM_OCInitStructure.TIM_OCMode       = TIM_OCMode_PWM2;
    TIM_OCInitStructure.TIM_OutputState  = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
    TIM_OCInitStructure.TIM_Pulse        = SRC_Buffer[0];
    TIM_OCInitStructure.TIM_OCPolarity   = TIM_OCPolarity_Low;
    TIM_OCInitStructure.TIM_OCNPolarity  = TIM_OCNPolarity_Low;
    TIM_OCInitStructure.TIM_OCIdleState  = TIM_OCIdleState_Set;
    TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset;

    TIM_OC1Init(TIM1, &TIM_OCInitStructure);

    /* TIM1 counter enable */
    TIM_Cmd(TIM1, ENABLE);
    /* TIM1 Update DMA Request enable */
    TIM_DMACmd(TIM1, TIM_DMA_Update | TIM_DMA_CC1, ENABLE);

    /* TIM1 PWM Output Enable */
    TIM_CtrlPWMOutputs(TIM1, ENABLE);
    debug("DMA transfer data length: %d\r\n", DMA_GetCurrDataCounter(DMA_Channel1));
    /* the value of memory to Peripheral */
    debug("DMA_memory_to_Peripheral = %x\r\n", TIM1->TIM_CCR1.value);

    DMA_Channel1_TransferDoneFlag = 0;
    /* Enable DMA Channel transfer */
    DMA_Cmd(DMA_Channel1, ENABLE);
    while (DMA_Channel1_TransferDoneFlag == 0) {
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