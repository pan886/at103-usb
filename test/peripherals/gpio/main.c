/**
 * @file main.c
 * @brief GPIO test case.
 * 
 * @author pantianwen (pantianwen@163.com)
 * @version 1.0
 * @date 2022-03-24
 * @copyright Copyright (c) 2022 Icore, Inc
 * Test function    : test all api in turn.
 * Board Version    ：AiP32rv001_EVB_V01
 * Hardware setup   : Without.
 */

#include "at103.h"
#include "unity.h"

void test_func(void)
{

    GPIO_InitTypeDef GPIOE_struct;
    uint32_t         GPIOE_PC            = 0;
    uint32_t         GPIOE_Func          = 0;
    uint32_t         GPIOE_OD            = 0;
    uint32_t         GPIOE_PP            = 0;
    uint32_t         GPIOE_DR            = 0;
    uint32_t         GPIOE_outputdata    = 0;
    uint32_t         GPIOE_outputdatabit = 0;
    uint32_t         GPIOE_inputdata     = 0;
    uint32_t         Pinmap_SPI          = 0;
    uint32_t         debounce            = 0;
    uint32_t         exti_data           = 0;
    uint32_t         GPIOE_inputdatabit  = 0;
    GPIOE_struct.GPIO_Pin                = GPIO_Pin_All;
    GPIOE_struct.GPIO_Mode               = GPIO_Mode_IPD;
    GPIOE_struct.GPIO_Speed              = GPIO_Speed_2MHz;
    GPIO_Init(GPIOE, &GPIOE_struct);
    /*pull down/up and float in test*/
    GPIOE_PC = *((uint32_t *)(AFIO_BASE + 0x48));
    debug("GPIOE_PC = %x\n", GPIOE_PC);
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

    /*read GPIO input test*/
    GPIOE_inputdata = GPIO_ReadInputData(GPIOE);
    TEST_ASSERT_MESSAGE(GPIOE_inputdata == 0xff83, "GPIO input data test fail");
    debug("GPIOE_inputdata = %x\n", GPIOE_inputdata);
    GPIOE_inputdatabit = GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_0);
    debug("GPIOE_inputdatabit = %x\n", GPIOE_inputdatabit);
    TEST_ASSERT_MESSAGE(GPIOE_inputdatabit == 0x1, "GPIO input data bit test fail");

    /*GPIO Analog function test*/
    GPIOE_struct.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_Init(GPIOE, &GPIOE_struct);
    GPIOE_Func = *((uint32_t *)(AFIO_BASE + 0x90));
    TEST_ASSERT_MESSAGE(GPIOE_Func == 0xffffffff, "GPIO Analog function test fail");
    debug("GPIOE_Func = %x\n", GPIOE_Func);

    /*GPIO Alternate function test*/
    GPIOE_struct.GPIO_Mode = GPIO_Mode_AF;
    GPIO_Init(GPIOE, &GPIOE_struct);
    GPIOE_Func = *((uint32_t *)(AFIO_BASE + 0x90));
    TEST_ASSERT_MESSAGE(GPIOE_Func == 0x55555555, "GPIO Alternate function test fail");
    debug("GPIOE_Func = %x\n", GPIOE_Func);

    /*GPIO Open-Drain mode test*/
    GPIOE_struct.GPIO_Mode = GPIO_Mode_Out_OD;
    GPIO_Init(GPIOE, &GPIOE_struct);
    GPIOE_OD = *((uint32_t *)(AFIO_BASE + 0x44));
    TEST_ASSERT_MESSAGE(GPIOE_OD == 0xffff, "GPIO Open-Drain mode test fail");
    debug("GPIOE_OD = %x\n", GPIOE_OD);

    /*GPIO push-pull mode test*/
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

    /*read GPIO output test*/
    GPIOE_struct.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOE, &GPIOE_struct);
    GPIO_SetBits(GPIOE, GPIO_Pin_All);
    GPIOE_outputdata = GPIO_ReadOutputData(GPIOE);
    debug("GPIOE_outputdata = %x\n", GPIOE_outputdata);
    TEST_ASSERT_MESSAGE(GPIOE_outputdata == 0xffff, "setbits test fail");
    GPIOE_outputdatabit = GPIO_ReadOutputDataBit(GPIOE, GPIO_Pin_0);
    debug("GPIOE_outputdatabit = %x\n", GPIOE_outputdatabit);
    TEST_ASSERT_MESSAGE(GPIOE_outputdatabit == 0x1, "GPIO output databit test fail");
    GPIO_ResetBits(GPIOE, GPIO_Pin_All);
    GPIOE_outputdata = GPIO_ReadOutputData(GPIOE);
    TEST_ASSERT_MESSAGE(GPIOE_outputdata == 0x0, "resetbits test fail");
    debug("GPIOE_outputdata = %x\n", GPIOE_outputdata);

    /*debounce enable and disable test*/
    GPIO_Debounce_Enable(GPIOE, GPIO_Pin_All);
    debounce = *((uint32_t *)(GPIOE_BASE + 0x30));
    debug("debounce = %x\n", debounce);
    TEST_ASSERT_MESSAGE(debounce == 0xffff, "debounce enable test fail");
    GPIO_Debounce_Disable(GPIOE, GPIO_Pin_All);
    debounce = *((uint32_t *)(GPIOE_BASE + 0x30));
    debug("debounce = %x\n", debounce);
    TEST_ASSERT_MESSAGE(debounce == 0x0, "debounce disable test fail");

    /*exti test*/
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOE, GPIO_PinSource0);
    exti_data = *((uint32_t *)(AFIO_BASE + 0x70));
    debug("exti_data = %x\n", exti_data);
    TEST_ASSERT_MESSAGE(exti_data == 0x4, "exti test fail");

    /*GPIO pinmap test*/
    GPIO_PinRemapConfig(GPIO_Remap_SPI1, ENABLE);
    GPIO_PinRemapConfig(GPIO_Remap_I2C1, ENABLE);
    GPIO_PinRemapConfig(GPIO_Remap_USART1, ENABLE);
    GPIO_PinRemapConfig(GPIO_PartialRemap_USART3, ENABLE);
    GPIO_PinRemapConfig(GPIO_FullRemap_USART3, ENABLE);
    GPIO_PinRemapConfig(GPIO_PartialRemap_TIM1, ENABLE);
    GPIO_PinRemapConfig(GPIO_FullRemap_TIM1, ENABLE);
    GPIO_PinRemapConfig(GPIO_PartialRemap1_TIM2, ENABLE);
    GPIO_PinRemapConfig(GPIO_PartialRemap2_TIM2, ENABLE);
    GPIO_PinRemapConfig(GPIO_FullRemap_TIM2, ENABLE);
    GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3, ENABLE);
    GPIO_PinRemapConfig(GPIO_FullRemap_TIM3, ENABLE);
    GPIO_PinRemapConfig(GPIO_Remap_TIM4, ENABLE);
    GPIO_PinRemapConfig(GPIO_Remap1_CAN, ENABLE);
    GPIO_PinRemapConfig(GPIO_Remap2_CAN, ENABLE);
    GPIO_PinRemapConfig(GPIO_Remap_PD01, ENABLE);
    GPIO_PinRemapConfig(GPIO_Remap_SPI2, ENABLE);

    Pinmap_SPI = *((uint32_t *)(AFIO_BASE));
    debug("Pinmap_SPI = %x\n", Pinmap_SPI);
    TEST_ASSERT_MESSAGE(Pinmap_SPI == 0x1fff7, "GPIO pinmap test fail");

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
    debug("GPIO test start!\r\n");
    unity_test();
    while (1) {
    }
}
