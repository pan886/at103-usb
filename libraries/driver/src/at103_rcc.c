/**
 * @file at103_rcc.c
 * @brief This file provides all the RCC firmware functions.
 * 
 * @author zhangsheng (zhangsheng@zhangsheng.ic@gmail.com)
 * @version 1.0
 * @date 2022-01-13
 * @copyright Copyright (c) 2022 Icore, Inc
 */
#include "at103_rcc.h"
#ifdef RCC_MODULE_ENABLED

#define IS_RCC_HSE(HSE) (((HSE) == RCC_HSE_OFF) || ((HSE) == RCC_HSE_ON) || \
                         ((HSE) == RCC_HSE_Bypass))

#define IS_RCC_APB1_PERIPH(PERIPH) ((((PERIPH)&0xFFFFF000) == 0x00) && ((PERIPH) != 0x00))

#define IS_RCC_APB2_PERIPH(PERIPH) ((((PERIPH)&0xE000FFFF) == 0x00) && ((PERIPH) != 0x00))

#define IS_RCC_APB1_REST_PERIPH(PERIPH) ((((PERIPH)&0xFFFFF000) == 0x00) && ((PERIPH) != 0x00))

#define IS_RCC_APB2_REST_PERIPH(PERIPH) ((((PERIPH)&0xE000FFFF) == 0x00) && ((PERIPH) != 0x00))

#define IS_RCC_AHB_PERIPH(PERIPH) (((PERIPH) == RCC_AHBPeriph_USB) || ((PERIPH) == RCC_AHBPeriph_CRC) || \
                                   ((PERIPH) == RCC_AHBPeriph_DMA) || ((PERIPH) == RCC_AHBPeriph_EFC))

#define IS_RCC_EN_IT(IT) (((IT) == RCC_IT_LSIRDY) || ((IT) == RCC_IT_PLLRDY))
#define IS_RCC_IT(IT)    (((IT) == RCC_IT_LSIRDY) || ((IT) == RCC_IT_PLLRDY) || ((IT) == RCC_IT_HSESTOP))

#define IS_RCC_FLAG(FLAG) (((FLAG) == RCC_FLAG_PORRST) || ((FLAG) == RCC_FLAG_SFTRST) || \
                           ((FLAG) == RCC_FLAG_WDTRST))

#define IS_RCC_HCLK(HCLK) (((HCLK) == RCC_SYSCLK_Div1) || ((HCLK) == RCC_SYSCLK_Div2) || \
                           ((HCLK) == RCC_SYSCLK_Div4) || ((HCLK) == RCC_SYSCLK_Div8) || \
                           ((HCLK) == RCC_SYSCLK_Div16) || ((HCLK) == RCC_SYSCLK_Div64) || \
                           ((HCLK) == RCC_SYSCLK_Div128) || ((HCLK) == RCC_SYSCLK_Div256) || \
                           ((HCLK) == RCC_SYSCLK_Div512))

#define IS_RCC_PCLK(PCLK) (((PCLK) == RCC_HCLK_Div1) || ((PCLK) == RCC_HCLK_Div2) || \
                           ((PCLK) == RCC_HCLK_Div4) || ((PCLK) == RCC_HCLK_Div8) || \
                           ((PCLK) == RCC_HCLK_Div16))

#define IS_RCC_PLL_SOURCE(SOURCE) (((SOURCE) == RCC_PLLSource_HSI_Div2) || \
                                   ((SOURCE) == RCC_PLLSource_HSE_Div1) || \
                                   ((SOURCE) == RCC_PLLSource_HSE_Div2))

#define IS_RCC_SYSCLK_SOURCE(SOURCE) (((SOURCE) == RCC_SYSCLKSource_HSI) || \
                                      ((SOURCE) == RCC_SYSCLKSource_HSE) || \
                                      ((SOURCE) == RCC_SYSCLKSource_PLLCLK))

#define IS_RCC_MCO(MCO) (((MCO) == RCC_MCO_NoClock) || ((MCO) == RCC_MCO_HSI) || \
                         ((MCO) == RCC_MCO_SYSCLK) || ((MCO) == RCC_MCO_HSE) || \
                         ((MCO) == RCC_MCO_PLLCLK_Div2))

void RCC_DeInit(void)
{
    /* Enable HSI */
    RCC->RC8M_CONFIG.RC8M_EN_BIT = 1;
    /* Reset MCO,AHB,APB1,APB2,Select HSI as Sysclock Source */
    RCC->CLK_CONFIG.value = 0x80022008;

    /* Power down PLL*/
    RCC->PLL_CONFIG.PLL_EN_BIT = 0;

    /* Rest PLL multi factor & div factor*/
    RCC->PLL_CONFIG.value = 0x10208901;

    /* HSE disable,bypass disable */
    RCC->OSC_CTRL.value = 0x2D;

    /* Reset Rcc interrupt */
    RCC->INT.value = 0x00;
}

ErrorStatus RCC_WaitForPllLock(void)
{
    __IO uint32_t StartUpCounter = 0;

    ErrorStatus status    = ERROR;
    FlagStatus  HSEStatus = RESET;
    /* Wait till pll is ready and if Time out is reached exit */
    do {
        HSEStatus = RCC->CLK_CONFIG.PLL_LOCK_BIT;
        StartUpCounter++;
    } while ((StartUpCounter != PLL_LOCK_TIMEOUT) && (HSEStatus == RESET));

    if (HSEStatus != RESET) {
        status = SUCCESS;
    } else {
        status = ERROR;
    }
    return (status);
}

void RCC_PLLCmd(FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    if (NewState != DISABLE) {
        RCC->PLL_CONFIG.PLL_EN_BIT = 1;
    } else {
        RCC->PLL_CONFIG.PLL_EN_BIT = 0;
    }
}

void RCC_PLLConfig(uint32_t RCC_PLLSource, uint32_t RCC_PLLLoop, uint32_t RCC_PLLDivOut, uint32_t RCC_PLLDivPre)
{
    uint32_t clktemp = 0;
    /* Check the parameters */
    assert_param(IS_RCC_PLL_SOURCE(RCC_PLLSource));

    /* Select the pll input source*/
    clktemp = RCC->CLK_CONFIG.value;
    clktemp &= 0xfffffff6;
    RCC->CLK_CONFIG.value = clktemp | RCC_PLLSource;

    /* config the pll divider*/
    RCC->PLL_CONFIG.PLL_DIV_LOOP_BIT = RCC_PLLLoop;
    RCC->PLL_CONFIG.PLL_DIV_OUT_BIT  = RCC_PLLDivOut;
    RCC->PLL_CONFIG.PLL_DIV_PRE_BIT  = RCC_PLLDivPre;
}

void RCC_SYSCLKConfig(uint32_t RCC_SYSCLKSource)
{
    uint32_t tempreg = 0;
    /* Check the parameters */
    assert_param(IS_RCC_SYSCLK_SOURCE(RCC_SYSCLKSource));

    RCC->CLK_CONFIG.SWS_BIT = RCC_SYSCLKSource;
}

void RCC_HSEConfig(uint32_t RCC_HSE)
{
    /* Check the parameters */
    assert_param(IS_RCC_HSE(RCC_HSE));

    /* Reset HSEON and HSEBYP bits before configuring the HSE ------------------*/
    /* Reset HSEON bit */
    RCC->OSC_CTRL.XTAL_EN_BIT = 0;
    /* Reset HSEBYP bit */
    RCC->OSC_CTRL.XTAL_BYP_BIT = 0;
    /* Configure HSE (RCC_HSE_OFF is already covered by the code section above) */
    switch (RCC_HSE) {
    case RCC_HSE_ON:
        /* Set HSEON bit */
        RCC->OSC_CTRL.XTAL_EN_BIT = 1;
        break;
    case RCC_HSE_Bypass:
        /* Set HSEBYP and HSEON bits */
        RCC->OSC_CTRL.XTAL_EN_BIT  = 1;
        RCC->OSC_CTRL.XTAL_BYP_BIT = 1;
        break;

    default:
        break;
    }
}

void RCC_HCLKConfig(uint32_t RCC_SYSCLK)
{
    uint32_t tempreg = 0x00;
    /* Check the parameters */
    IS_RCC_SYSCLK_SOURCE(RCC_SYSCLK);

    RCC->CLK_CONFIG.AHB_PRESCL_BIT = RCC_SYSCLK;
}

void RCC_PCLK1Config(uint32_t RCC_HCLK)
{
    uint32_t tempreg = 0x00;
    /* Check the parameters */
    IS_RCC_SYSCLK_SOURCE(RCC_HCLK);

    RCC->CLK_CONFIG.APB1_PRESCL_BIT = RCC_HCLK;
}

void RCC_PCLK2Config(uint32_t RCC_HCLK)
{
    uint32_t tempreg = 0x00;
    /* Check the parameters */
    IS_RCC_SYSCLK_SOURCE(RCC_HCLK);

    RCC->CLK_CONFIG.APB2_PRESCL_BIT = RCC_HCLK;
}

void RCC_HSICmd(FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    if (NewState != DISABLE) {
        RCC->RC8M_CONFIG.RC8M_EN_BIT = 1;
    } else {
        RCC->RC8M_CONFIG.RC8M_EN_BIT = 0;
    }
}

void RCC_GetClocksFreq(RCC_ClocksTypeDef *RCC_Clocks)
{
    uint32_t pll_div_pre = 0, pllsource = 0, pll_div_out = 0, pll_div_loop = 0;
    uint32_t presc          = 0;
    uint32_t usart_div_main = 0, usart_div_num = 0;
    /* Get SYSCLK source -------------------------------------------------------*/
    uint32_t temp = RCC->CLK_CONFIG.SWS_BIT;

    switch (temp) {
    case 0x00: /* HSI used as system clock */

        RCC_Clocks->SYSCLK_Frequency = HSI_VALUE;
        break;
    case 0x01: /* HSE used as system clock */

        RCC_Clocks->SYSCLK_Frequency = HSE_VALUE;
        break;
    case 0x02: /* PLL used as system clock */

        pll_div_pre  = RCC->PLL_CONFIG.PLL_DIV_PRE_BIT;
        pll_div_out  = RCC->PLL_CONFIG.PLL_DIV_OUT_BIT;
        pll_div_loop = RCC->PLL_CONFIG.PLL_DIV_LOOP_BIT;
        pllsource    = RCC->CLK_CONFIG.PLL_SRC_BIT;

        if (0x00 == pllsource) {
            /* HSI oscillator clock divided by 2 selected as PLL clock entry */
            RCC_Clocks->SYSCLK_Frequency = (HSI_VALUE >> 1) * (pll_div_loop / (pll_div_pre * pll_div_out));
        } else {
            if (RCC->CLK_CONFIG.PLL_XTEPRE_BIT) {
                /* HSE oscillator clock divided by 2 selected as PLL clock entry */
                RCC_Clocks->SYSCLK_Frequency = (HSE_VALUE >> 1) * (pll_div_loop / (pll_div_pre * pll_div_out));
            } else {
                /* HSE oscillator clock  selected as PLL clock entry */
                RCC_Clocks->SYSCLK_Frequency = (HSE_VALUE) * (pll_div_loop / (pll_div_pre * pll_div_out));
            }
        }
        break;
    default:
        RCC_Clocks->SYSCLK_Frequency = HSI_VALUE;
        break;
    }

    /* Compute HCLK, PCLK1, PCLK2 and ADCCLK clocks frequencies ----------------*/
    /* Get HCLK prescaler */
    presc = RCC->CLK_CONFIG.AHB_PRESCL_BIT;
    /* HCLK clock frequency */
    RCC_Clocks->HCLK_Frequency = RCC_Clocks->SYSCLK_Frequency / (presc + 1);
    /* Get PCLK1 prescaler */
    presc = RCC->CLK_CONFIG.APB1_PRESCL_BIT;
    /* PCLK1 clock frequency */
    RCC_Clocks->PCLK1_Frequency = RCC_Clocks->HCLK_Frequency / (presc + 1);
    /* Get PCLK2 prescaler */
    presc = RCC->CLK_CONFIG.APB2_PRESCL_BIT;
    /* PCLK2 clock frequency */
    RCC_Clocks->PCLK2_Frequency = RCC_Clocks->HCLK_Frequency / (presc + 1);
    /* Get ADCCLK prescaler */
    presc = RCC->CLK_DIV_CFG.ADC_CLK_DIV_BIT;
    /* ADCCLK clock frequency */
    RCC_Clocks->ADCCLK_Frequency = RCC_Clocks->PCLK2_Frequency / (presc + 1);
    /* Get USBPHY prescaler */
    presc = RCC->CLK_DIV_CFG.USB_PHY_CLK_DIV_BIT;
    /* USART clock frequency */
    RCC_Clocks->USB_Frequencey = RCC_Clocks->SYSCLK_Frequency / (presc + 1);
    /* Get USART prescaler */
    usart_div_main = RCC->CLK_DIV_CFG.UART_CLK_DIV_MAIN_BIT;
    usart_div_num  = RCC->CLK_DIV_CFG.UART_CLK_DIV_NUM_BIT;
    ;
    /* USART clock frequency */
    RCC_Clocks->USART_Frequencey = ((RCC_Clocks->SYSCLK_Frequency / 1000 * 32) / (usart_div_main * 32 + usart_div_num)) * 1000;
}

void RCC_AHBPeriphClockCmd(uint32_t RCC_AHBPeriph, FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_FUNCTIONAL_STATE(NewState));
    assert_param(IS_RCC_AHB_PERIPH(RCC_AHBPeriph));

    if (NewState != DISABLE) {
        RCC->AHBCLKEN.value |= RCC_AHBPeriph;
    } else {
        RCC->AHBCLKEN.value &= ~RCC_AHBPeriph;
    }
}

void RCC_AHBPeriphResetCmd(uint32_t RCC_AHBPeriph, FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_FUNCTIONAL_STATE(NewState));
    assert_param(IS_RCC_AHB_PERIPH(RCC_AHBPeriph));

    if (NewState != DISABLE) {
        RCC->AHB_SRST.value |= RCC_AHBPeriph;
    } else {
        RCC->AHB_SRST.value &= ~RCC_AHBPeriph;
    }
}

void RCC_APB1PeriphClockCmd(uint32_t RCC_APB1Periph, FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_FUNCTIONAL_STATE(NewState));
    assert_param(IS_RCC_APB1_PERIPH(RCC_APB1Periph));

    if (NewState != DISABLE) {
        RCC->APBCLKEN.value |= RCC_APB1Periph;
    } else {
        RCC->APBCLKEN.value &= ~RCC_APB1Periph;
    }
}

void RCC_APB2PeriphClockCmd(uint32_t RCC_APB2Periph, FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_FUNCTIONAL_STATE(NewState));
    assert_param(IS_RCC_APB2_PERIPH(RCC_APB2Periph));

    if (NewState != DISABLE) {
        RCC->APBCLKEN.value |= RCC_APB2Periph;
    } else {
        RCC->APBCLKEN.value &= ~RCC_APB2Periph;
    }
}

void RCC_APB1PeriphResetCmd(uint32_t RCC_APB1Periph, FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_FUNCTIONAL_STATE(NewState));
    assert_param(IS_RCC_APB1_REST_PERIPH(RCC_APB1Periph));

    if (NewState != DISABLE) {
        RCC->APB_SRST.value |= RCC_APB1Periph;
    } else {
        RCC->APB_SRST.value &= ~RCC_APB1Periph;
    }
}

void RCC_APB2PeriphResetCmd(uint32_t RCC_APB2Periph, FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_FUNCTIONAL_STATE(NewState));
    assert_param(IS_RCC_APB2_REST_PERIPH(RCC_APB2Periph));

    if (NewState != DISABLE) {
        RCC->APB_SRST.value |= RCC_APB2Periph;
    } else {
        RCC->APB_SRST.value &= ~RCC_APB2Periph;
    }
}

void RCC_OTGFSCLKConfig(uint32_t RCC_OTGPHY_CLKDiv)
{

    RCC->CLK_DIV_CFG.USB_PHY_CLK_DIV_BIT = RCC_OTGPHY_CLKDiv;
}

void RCC_ADCCLKConfig(uint32_t RCC_ADC_Div)
{
    RCC->CLK_DIV_CFG.ADC_CLK_DIV_BIT = RCC_ADC_Div;
}

void RCC_UartClkConfig(uint32_t RCC_UartDivMain, uint32_t RCC_UartDivNum)
{
    RCC->CLK_DIV_CFG.UART_CLK_DIV_MAIN_BIT = RCC_UartDivMain;
    RCC->CLK_DIV_CFG.UART_CLK_DIV_NUM_BIT  = RCC_UartDivNum;
}

void RCC_MCOConfig(uint8_t RCC_MCO)
{
    /* Check the parameters */
    assert_param(IS_RCC_MCO(RCC_MCO));

    RCC->CLK_CONFIG.MCO_SEL_BIT = RCC_MCO;
}

void RCC_ITConfig(uint8_t RCC_IT, FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_RCC_EN_IT(RCC_IT));
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    if (NewState != DISABLE) {
        RCC->INT.value |= RCC_IT << 8;
    } else {
        RCC->INT.value &= ~(RCC_IT << 8);
    }
}

FlagStatus
RCC_GetFlagStatus(uint8_t RCC_FLAG)
{
    uint32_t   tmp       = 0;
    uint32_t   statusreg = 0;
    FlagStatus bitstatus = RESET;
    /* Check the parameters */
    assert_param(IS_RCC_FLAG(RCC_FLAG));

    statusreg = RCC->MCU_RESET_STATUS.value;
    if ((statusreg & RCC_FLAG) != RESET) {
        bitstatus = SET;
    } else {
        bitstatus = RESET;
    }
    return bitstatus;
}

ITStatus RCC_GetITStatus(uint8_t RCC_IT)
{
    ITStatus bitstatus = RESET;
    /* Check the parameters */
    assert_param(IS_RCC_IT(RCC_IT));

    if ((RCC->INT.value & RCC_IT) != (uint32_t)RESET) {
        bitstatus = SET;
    } else {
        bitstatus = RESET;
    }
    return bitstatus;
}

void RCC_ClearITPendingBit(uint8_t RCC_IT)
{
    /* Check the parameters */
    assert_param(IS_RCC_IT(RCC_IT));

    RCC->INT.value |= RCC_IT << 16;
}

void RCC_AdjustHSICalibrationValue(uint8_t Ctune, uint8_t Ftune)
{
    RCC->RC8M_CONFIG.FTUNE_BIT = Ftune;
    RCC->RC8M_CONFIG.CTUNE_BIT = Ctune;
}

void RCC_ClockSecuritySystemCmd(FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    if (NewState != DISABLE) {
        RCC->CLK_CONFIG.CSSON_BIT = 1;
    } else {
        RCC->CLK_CONFIG.CSSON_BIT = 0;
    }
}

void RCC_GlobaLReset(void)
{
    RCC->GLB_SW_RESET = 0xAA55A5A5;
}

#endif