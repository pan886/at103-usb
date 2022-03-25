/**
 * @file at103_gpio.c
 * @brief This file provides all the GPIO firmware functions.
 * @author Dong Qin (dong.qin@timesintelli.com)
 * @version 1.0
 * @date 2022-03-14
 * @copyright Copyright (c) 2022 Timesintelli, Inc
 */

#include "at103_gpio.h"

#ifdef GPIO_MODULE_ENABLED

void GPIO_DeInit(GPIO_TypeDef *GPIOx)
{
    /* Check the parameters */
    assert_param(IS_GPIO_ALL_PERIPH(GPIOx));

    if (GPIOx == GPIOA) {
        RCC_APB2PeriphResetCmd(RCC_APB2Periph_GPIOA, ENABLE);
        RCC_APB2PeriphResetCmd(RCC_APB2Periph_GPIOA, DISABLE);
    } else if (GPIOx == GPIOB) {
        RCC_APB2PeriphResetCmd(RCC_APB2Periph_GPIOB, ENABLE);
        RCC_APB2PeriphResetCmd(RCC_APB2Periph_GPIOB, DISABLE);
    } else if (GPIOx == GPIOC) {
        RCC_APB2PeriphResetCmd(RCC_APB2Periph_GPIOC, ENABLE);
        RCC_APB2PeriphResetCmd(RCC_APB2Periph_GPIOC, DISABLE);
    } else if (GPIOx == GPIOD) {
        RCC_APB2PeriphResetCmd(RCC_APB2Periph_GPIOD, ENABLE);
        RCC_APB2PeriphResetCmd(RCC_APB2Periph_GPIOD, DISABLE);
    } else if (GPIOx == GPIOE) {
        RCC_APB2PeriphResetCmd(RCC_APB2Periph_GPIOE, ENABLE);
        RCC_APB2PeriphResetCmd(RCC_APB2Periph_GPIOE, DISABLE);
    }
}

void GPIO_AFIODeInit(void)
{
    RCC_APB2PeriphResetCmd(RCC_APB2Periph_AFIO, ENABLE);
    RCC_APB2PeriphResetCmd(RCC_APB2Periph_AFIO, DISABLE);
}

void GPIO_Init(GPIO_TypeDef *GPIOx, GPIO_InitTypeDef *GPIO_InitStruct)
{
    uint32_t      pinpos     = 0x00;
    uint32_t      gpio_group = 0x00;
    uint32_t      afio_func  = 0x00;
    __IO uint32_t func_reg;
    __IO uint32_t dr_reg;

    /* Check the parameters */
    assert_param(IS_GPIO_ALL_PERIPH(GPIOx));
    assert_param(IS_GPIO_MODE(GPIO_InitStruct->GPIO_Mode));
    assert_param(IS_GPIO_PIN(GPIO_InitStruct->GPIO_Pin));
    assert_param(IS_GPIO_SPEED(GPIO_InitStruct->GPIO_Speed));
#if 1
    switch ((uint32_t)GPIOx) {
    case (uint32_t)GPIOA:
        gpio_group = 0;
        break;
    case (uint32_t)GPIOB:
        gpio_group = 1;
        break;
    case (uint32_t)GPIOC:
        gpio_group = 2;
        break;
    case (uint32_t)GPIOD:
        gpio_group = 3;
        break;
    case (uint32_t)GPIOE:
        gpio_group = 4;
        break;
    }

    /* GPIO direction setting */
    afio_func = (((uint32_t)GPIO_InitStruct->GPIO_Mode) & ((uint32_t)0xF0)) >> 4;
    if (afio_func == 0x00) {
        // GPIO mode
        GPIOx->CTRL = 0x00;
        if ((((uint32_t)GPIO_InitStruct->GPIO_Mode) & ((uint32_t)0x01)) == 0) {
            // input mode
            GPIOx->DIR &= ~GPIO_InitStruct->GPIO_Pin;
            AFIO->GPIO[gpio_group].IC |= ((uint32_t)GPIO_InitStruct->GPIO_Pin << 16);
        } else {
            // output mode
            GPIOx->DIR |= GPIO_InitStruct->GPIO_Pin;
            AFIO->GPIO[gpio_group].IC &= ~(((uint32_t)GPIO_InitStruct->GPIO_Pin) << 16);
        }
    } else if (afio_func == 0x1) {
        // AF mode
        AFIO->GPIO[gpio_group].IC |= (((uint32_t)GPIO_InitStruct->GPIO_Pin) << 16);
    } else if (afio_func == 0x3) {
        // AIN mode
        AFIO->GPIO[gpio_group].IC &= ~(((uint32_t)GPIO_InitStruct->GPIO_Pin) << 16);
    }

    /* Function-selection and IO-speed Settings */
    dr_reg   = AFIO->GPIO[gpio_group].DR;
    func_reg = AFIO->GPIO_FUNC_SEL[gpio_group];
    for (pinpos = 0x00; pinpos < 0x10; pinpos++) {
        if ((((GPIO_InitStruct->GPIO_Pin) >> pinpos) & 0x1) != 0x0) {
            func_reg &= ~(0x03 << (pinpos << 1));
            dr_reg &= ~(0x03 << (pinpos << 1));
            func_reg |= (afio_func & 0x03) << (pinpos << 1);
            dr_reg |= (((uint32_t)GPIO_InitStruct->GPIO_Speed) & 0x03) << (pinpos << 1);
        }
    }
    AFIO->GPIO_FUNC_SEL[gpio_group] = func_reg;
    AFIO->GPIO[gpio_group].DR       = dr_reg;

    /* Pull-up and Pull-down Settings */
    if ((((uint32_t)GPIO_InitStruct->GPIO_Mode) & ((uint32_t)0x08)) != 0) {
        AFIO->GPIO[gpio_group].PC |= ((uint32_t)GPIO_InitStruct->GPIO_Pin << 16);
    } else {
        AFIO->GPIO[gpio_group].PC &= ~((uint32_t)GPIO_InitStruct->GPIO_Pin << 16);
    }
    if ((((uint32_t)GPIO_InitStruct->GPIO_Mode) & ((uint32_t)0x04)) != 0) {
        AFIO->GPIO[gpio_group].PC |= (uint32_t)GPIO_InitStruct->GPIO_Pin;
    } else {
        AFIO->GPIO[gpio_group].PC &= ~((uint32_t)GPIO_InitStruct->GPIO_Pin);
    }

    /* Open-drain and Push-pull Settings */
    if ((((uint32_t)GPIO_InitStruct->GPIO_Mode) & ((uint32_t)0x02)) != 0) {
        AFIO->GPIO[gpio_group].OD |= (uint32_t)GPIO_InitStruct->GPIO_Pin;
    } else {
        AFIO->GPIO[gpio_group].OD &= ~((uint32_t)GPIO_InitStruct->GPIO_Pin);
    }
#endif
}

void GPIO_StructInit(GPIO_InitTypeDef *GPIO_InitStruct)
{
    /* Reset GPIO init structure parameters values */
    GPIO_InitStruct->GPIO_Pin   = GPIO_Pin_All;
    GPIO_InitStruct->GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitStruct->GPIO_Mode  = GPIO_Mode_IN_FLOATING;
}

void GPIO_PinRemapConfig(uint32_t GPIO_Remap, FunctionalState NewState)
{
    __IO uint32_t temp_reg = 0U;

    /* Check the parameters */
    assert_param(IS_GPIO_REMAP(GPIO_Remap));
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    temp_reg = GPIO_Remap;
    AFIO->PIN_REMAP_CTRL.value |= temp_reg;
    if (NewState != ENABLE) {
        AFIO->PIN_REMAP_CTRL.value &= ~temp_reg;
    }
}

uint8_t GPIO_ReadInputDataBit(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin)
{
    uint8_t bitstatus = 0x00;

    /* Check the parameters */
    assert_param(IS_GPIO_ALL_PERIPH(GPIOx));
    assert_param(IS_GET_GPIO_PIN(GPIO_Pin));

    if ((GPIOx->EXT & GPIO_Pin) != (uint32_t)Bit_RESET) {
        bitstatus = (uint8_t)Bit_SET;
    } else {
        bitstatus = (uint8_t)Bit_RESET;
    }
    return bitstatus;
}

uint16_t GPIO_ReadInputData(GPIO_TypeDef *GPIOx)
{
    /* Check the parameters */
    assert_param(IS_GPIO_ALL_PERIPH(GPIOx));

    return ((uint16_t)GPIOx->EXT);
}

uint8_t GPIO_ReadOutputDataBit(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin)
{
    uint8_t bitstatus = 0x00;

    /* Check the parameters */
    assert_param(IS_GPIO_ALL_PERIPH(GPIOx));
    assert_param(IS_GET_GPIO_PIN(GPIO_Pin));

    if ((GPIOx->DATA & GPIO_Pin) != (uint32_t)Bit_RESET) {
        bitstatus = (uint8_t)Bit_SET;
    } else {
        bitstatus = (uint8_t)Bit_RESET;
    }
    return bitstatus;
}

uint16_t GPIO_ReadOutputData(GPIO_TypeDef *GPIOx)
{
    /* Check the parameters */
    assert_param(IS_GPIO_ALL_PERIPH(GPIOx));

    return ((uint16_t)GPIOx->DATA);
}

void GPIO_ResetBits(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin)
{
    /* Check the parameters */
    assert_param(IS_GPIO_ALL_PERIPH(GPIOx));
    assert_param(IS_GPIO_PIN(GPIO_Pin));

    GPIOx->DATA &= ~GPIO_Pin;
}

void GPIO_SetBits(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin)
{
    /* Check the parameters */
    assert_param(IS_GPIO_ALL_PERIPH(GPIOx));
    assert_param(IS_GPIO_PIN(GPIO_Pin));

    GPIOx->DATA |= GPIO_Pin;
}

void GPIO_WriteBit(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin, BitAction BitVal)
{
    /* Check the parameters */
    assert_param(IS_GPIO_ALL_PERIPH(GPIOx));
    assert_param(IS_GET_GPIO_PIN(GPIO_Pin));
    assert_param(IS_GPIO_BIT_ACTION(BitVal));

    if (BitVal != Bit_RESET) {
        GPIOx->DATA |= GPIO_Pin;
    } else {
        GPIOx->DATA &= ~GPIO_Pin;
    }
}

void GPIO_Write(GPIO_TypeDef *GPIOx, uint16_t PortVal)
{
    /* Check the parameters */
    assert_param(IS_GPIO_ALL_PERIPH(GPIOx));

    GPIOx->DATA = PortVal;
}

void GPIO_Debounce_Enable(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin)
{
    /* Check the parameters */
    assert_param(IS_GPIO_ALL_PERIPH(GPIOx));
    assert_param(IS_GPIO_PIN(GPIO_Pin));

    GPIOx->DB |= GPIO_Pin;
}

void GPIO_Debounce_Disable(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin)
{
    /* Check the parameters */
    assert_param(IS_GPIO_ALL_PERIPH(GPIOx));
    assert_param(IS_GPIO_PIN(GPIO_Pin));

    GPIOx->DB &= ~GPIO_Pin;
}

void GPIO_EXTILineConfig(uint8_t GPIO_PortSource, uint8_t GPIO_PinSource)
{
    uint32_t tmp = 0x00;

    /* Check the parameters */
    assert_param(IS_GPIO_EXTI_PORT_SOURCE(GPIO_PortSource));
    assert_param(IS_GPIO_PIN_SOURCE(GPIO_PinSource));

    tmp = ((uint32_t)0x0F) << (0x04 * (GPIO_PinSource & (uint8_t)0x07));
    AFIO->EXTICR[GPIO_PinSource >> 0x03] &= ~tmp;
    AFIO->EXTICR[GPIO_PinSource >> 0x03] |= (((uint32_t)GPIO_PortSource) << (0x04 * (GPIO_PinSource & (uint8_t)0x07)));
}

#endif