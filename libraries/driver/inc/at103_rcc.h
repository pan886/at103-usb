/**
 * @file at103_rcc.h
 * @brief  This file contains all the functions prototypes for the RCC firmware 
 *         library.
 * @author zhangsheng (zhangsheng@zhangsheng.ic@gmail.com)
 * @version 1.0
 * @date 2022-01-13
 * @copyright Copyright (c) 2022 Icore, Inc
 */

#ifndef _RCC_H_
#define _RCC_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "at103.h"

/**
 * @brief HSE_configuration
 */
#define RCC_HSE_OFF     ((uint32_t)0x00000001)
#define RCC_HSE_ON      ((uint32_t)0x00000002)
#define RCC_HSE_Bypass  ((uint32_t)0x00000003)
#define IS_RCC_HSE(HSE) (((HSE) == RCC_HSE_OFF) || ((HSE) == RCC_HSE_ON) || \
                         ((HSE) == RCC_HSE_Bypass))

/**
 * @brief PLL Lock Timeout value 
 */
#define PLL_LOCK_TIMEOUT ((uint32_t)0x0000FFFF)

/**
 * @brief APB1_peripheral 
 */
#define RCC_APB1Periph_TIM2        ((uint32_t)0x00000001)
#define RCC_APB1Periph_TIM3        ((uint32_t)0x00000002)
#define RCC_APB1Periph_TIM4        ((uint32_t)0x00000004)
#define RCC_APB1Periph_RTC         ((uint32_t)0x00000008)
#define RCC_APB1Periph_WDT         ((uint32_t)0x00000010)
#define RCC_APB1Periph_SPI2        ((uint32_t)0x00000020)
#define RCC_APB1Periph_UART2       ((uint32_t)0x00000040)
#define RCC_APB1Periph_UART3       ((uint32_t)0x00000080)
#define RCC_APB1Periph_I2C1        ((uint32_t)0x00000100)
#define RCC_APB1Periph_I2C2        ((uint32_t)0x00000200)
#define RCC_APB1Periph_CAN         ((uint32_t)0x00000400)
#define RCC_APB1Periph_PMU         ((uint32_t)0x00000800)
#define IS_RCC_APB1_PERIPH(PERIPH) ((((PERIPH)&0xFFFFF000) == 0x00) && ((PERIPH) != 0x00))

/**
 * @brief APB2_peripheral
 */
#define RCC_APB2Periph_AFIO        ((uint32_t)0x00010000)
#define RCC_APB2Periph_ITCL        ((uint32_t)0x00020000)
#define RCC_APB2Periph_GPIOA       ((uint32_t)0x00040000)
#define RCC_APB2Periph_GPIOB       ((uint32_t)0x00080000)
#define RCC_APB2Periph_GPIOC       ((uint32_t)0x00100000)
#define RCC_APB2Periph_GPIOD       ((uint32_t)0x00200000)
#define RCC_APB2Periph_GPIOE       ((uint32_t)0x00400000)
#define RCC_APB2Periph_ADC1        ((uint32_t)0x00800000)
#define RCC_APB2Periph_ADC2        ((uint32_t)0x01000000)
#define RCC_APB2Periph_TIM1        ((uint32_t)0x02000000)
#define RCC_APB2Periph_SPI1        ((uint32_t)0x04000000)
#define RCC_APB2Periph_UART1       ((uint32_t)0x08000000)
#define RCC_APB2Periph_EFC         ((uint32_t)0x10000000)
#define IS_RCC_APB2_PERIPH(PERIPH) ((((PERIPH)&0xE000FFFF) == 0x00) && ((PERIPH) != 0x00))

/**
 * @brief APB1_PeriphRest
 */
#define RCC_APB1PeriphRest_TIM2         ((uint32_t)0x00000001)
#define RCC_APB1PeriphRest_TIM3         ((uint32_t)0x00000002)
#define RCC_APB1PeriphRest_TIM4         ((uint32_t)0x00000004)
#define RCC_APB1PeriphRest_RTC          ((uint32_t)0x00000008)
#define RCC_APB1PeriphRest_WDT          ((uint32_t)0x00000010)
#define RCC_APB1PeriphRest_SPI2         ((uint32_t)0x00000020)
#define RCC_APB1PeriphRest_UART2        ((uint32_t)0x00000040)
#define RCC_APB1PeriphRest_UART3        ((uint32_t)0x00000080)
#define RCC_APB1PeriphRest_I2C1         ((uint32_t)0x00000100)
#define RCC_APB1PeriphRest_I2C2         ((uint32_t)0x00000200)
#define RCC_APB1PeriphRest_CAN          ((uint32_t)0x00000400)
#define RCC_APB1PeriphRest_PMU          ((uint32_t)0x00000800)
#define IS_RCC_APB1_REST_PERIPH(PERIPH) ((((PERIPH)&0xFFFFF000) == 0x00) && ((PERIPH) != 0x00))
/**
 * @brief APB2_PeriphRest
 */
#define RCC_APB2PeriphRest_AFIO         ((uint32_t)0x00010000)
#define RCC_APB2PeriphRest_ITCL         ((uint32_t)0x00020000)
#define RCC_APB2PeriphRest_GPIOA        ((uint32_t)0x00040000)
#define RCC_APB2PeriphRest_GPIOB        ((uint32_t)0x00080000)
#define RCC_APB2PeriphRest_GPIOC        ((uint32_t)0x00100000)
#define RCC_APB2PeriphRest_GPIOD        ((uint32_t)0x00200000)
#define RCC_APB2PeriphRest_GPIOE        ((uint32_t)0x00400000)
#define RCC_APB2PeriphRest_ADC1         ((uint32_t)0x00800000)
#define RCC_APB2PeriphRest_ADC2         ((uint32_t)0x01000000)
#define RCC_APB2PeriphRest_TIM1         ((uint32_t)0x02000000)
#define RCC_APB2PeriphRest_SPI1         ((uint32_t)0x04000000)
#define RCC_APB2PeriphRest_UART1        ((uint32_t)0x08000000)
#define RCC_APB2PeriphRest_RCC          ((uint32_t)0x10000000)
#define IS_RCC_APB2_REST_PERIPH(PERIPH) ((((PERIPH)&0xE000FFFF) == 0x00) && ((PERIPH) != 0x00))
/**
 * @brief AHB_peripheral
 */
#define RCC_AHBPeriph_USB         ((uint32_t)0x00000001)
#define RCC_AHBPeriph_CRC         ((uint32_t)0x00000002)
#define RCC_AHBPeriph_DMA         ((uint32_t)0x00000004)
#define RCC_AHBPeriph_EFC         ((uint32_t)0x00000008)
#define IS_RCC_AHB_PERIPH(PERIPH) (((PERIPH) == RCC_AHBPeriph_USB) || ((PERIPH) == RCC_AHBPeriph_CRC) || \
                                   ((PERIPH) == RCC_AHBPeriph_DMA) || ((PERIPH) == RCC_AHBPeriph_EFC))

/**
 * @brief RCC_Interrupt_source
 */
#define RCC_IT_LSIRDY    ((uint8_t)0x01)
#define RCC_IT_PLLRDY    ((uint8_t)0x10)
#define RCC_IT_HSESTOP   ((uint8_t)0x80)
#define IS_RCC_EN_IT(IT) (((IT) == RCC_IT_LSIRDY) || ((IT) == RCC_IT_PLLRDY))
#define IS_RCC_IT(IT)    (((IT) == RCC_IT_LSIRDY) || ((IT) == RCC_IT_PLLRDY) || ((IT) == RCC_IT_HSESTOP))

/**
 * @brief RCC_FLAG 
 */
#define RCC_FLAG_PORRST   ((uint8_t)0x00)
#define RCC_FLAG_WDTRST   ((uint8_t)0x01)
#define RCC_FLAG_SFTRST   ((uint8_t)0x02)
#define IS_RCC_FLAG(FLAG) (((FLAG) == RCC_FLAG_PORRST) || ((FLAG) == RCC_FLAG_SFTRST) || \
                           ((FLAG) == RCC_FLAG_WDTRST))

typedef struct
{
    uint32_t SYSCLK_Frequency; /**< return SYSCLK clock frequency expressed in Hz */
    uint32_t HCLK_Frequency;   /**< return AHBCLK clock frequency expressed in Hz */
    uint32_t PCLK1_Frequency;  /**< return APBCLK1 clock frequency expressed in Hz */
    uint32_t PCLK2_Frequency;  /**< return APBCLK2 clock frequency expressed in Hz */
    uint32_t ADCCLK_Frequency; /**< return ADCCLK clock frequency expressed in Hz */
    uint32_t USART_Frequencey; /**< return USART clock frequency expressed in Hz */
    uint32_t USB_Frequencey;   /**< return USART clock frequency expressed in Hz */
} RCC_ClocksTypeDef;

/**
 * @brief AHB_clock_source
 */
#define RCC_SYSCLK_Div1   ((uint32_t)0x00000000)
#define RCC_SYSCLK_Div2   ((uint32_t)0x00000001)
#define RCC_SYSCLK_Div4   ((uint32_t)0x00000003)
#define RCC_SYSCLK_Div8   ((uint32_t)0x00000007)
#define RCC_SYSCLK_Div16  ((uint32_t)0x0000000F)
#define RCC_SYSCLK_Div64  ((uint32_t)0x0000003F)
#define RCC_SYSCLK_Div128 ((uint32_t)0x0000007F)
#define RCC_SYSCLK_Div256 ((uint32_t)0x000000FF)
#define RCC_SYSCLK_Div512 ((uint32_t)0x000001FF)
#define IS_RCC_HCLK(HCLK) (((HCLK) == RCC_SYSCLK_Div1) || ((HCLK) == RCC_SYSCLK_Div2) || \
                           ((HCLK) == RCC_SYSCLK_Div4) || ((HCLK) == RCC_SYSCLK_Div8) || \
                           ((HCLK) == RCC_SYSCLK_Div16) || ((HCLK) == RCC_SYSCLK_Div64) || \
                           ((HCLK) == RCC_SYSCLK_Div128) || ((HCLK) == RCC_SYSCLK_Div256) || \
                           ((HCLK) == RCC_SYSCLK_Div512))

/**
 * @brief APB_clock_source
 */
#define RCC_HCLK_Div1     ((uint32_t)0x00000000)
#define RCC_HCLK_Div2     ((uint32_t)0x00000001)
#define RCC_HCLK_Div4     ((uint32_t)0x00000003)
#define RCC_HCLK_Div8     ((uint32_t)0x00000007)
#define RCC_HCLK_Div16    ((uint32_t)0x0000000F)
#define IS_RCC_PCLK(PCLK) (((PCLK) == RCC_HCLK_Div1) || ((PCLK) == RCC_HCLK_Div2) || \
                           ((PCLK) == RCC_HCLK_Div4) || ((PCLK) == RCC_HCLK_Div8) || \
                           ((PCLK) == RCC_HCLK_Div16))

/**
 * @brief  PLL_entry_clock_source
 */
#define RCC_PLLSource_HSI_Div2    ((uint32_t)0x00000000)
#define RCC_PLLSource_HSE_Div1    ((uint32_t)0x00000008)
#define RCC_PLLSource_HSE_Div2    ((uint32_t)0x00000009)
#define IS_RCC_PLL_SOURCE(SOURCE) (((SOURCE) == RCC_PLLSource_HSI_Div2) || \
                                   ((SOURCE) == RCC_PLLSource_HSE_Div1) || \
                                   ((SOURCE) == RCC_PLLSource_HSE_Div2))

/**
 * @brief System_clock_source
 */
#define RCC_SYSCLKSource_HSI         ((uint32_t)0x00000000)
#define RCC_SYSCLKSource_HSE         ((uint32_t)0x00000001)
#define RCC_SYSCLKSource_PLLCLK      ((uint32_t)0x00000002)
#define IS_RCC_SYSCLK_SOURCE(SOURCE) (((SOURCE) == RCC_SYSCLKSource_HSI) || \
                                      ((SOURCE) == RCC_SYSCLKSource_HSE) || \
                                      ((SOURCE) == RCC_SYSCLKSource_PLLCLK))

/**
 * @brief clock source to mco pin
 */
#define RCC_MCO_NoClock     ((uint8_t)0x00)
#define RCC_MCO_SYSCLK      ((uint8_t)0x04)
#define RCC_MCO_HSI         ((uint8_t)0x05)
#define RCC_MCO_HSE         ((uint8_t)0x06)
#define RCC_MCO_PLLCLK_Div2 ((uint8_t)0x07)
#define IS_RCC_MCO(MCO)     (((MCO) == RCC_MCO_NoClock) || ((MCO) == RCC_MCO_HSI) || \
                         ((MCO) == RCC_MCO_SYSCLK) || ((MCO) == RCC_MCO_HSE) || \
                         ((MCO) == RCC_MCO_PLLCLK_Div2))

/**
 * @brief Resets the RCC clock configuration to the default reset state.
 */
void RCC_DeInit(void);

/**
 * @brief Configures the PLL clock source and multiplication factor.
 * @param[in] RCC_PLLSource: specifies the PLL entry clock source.
 * @param[in] RCC_PLLLoop: specifies the PLL multiplication factor.
 * @param[in] RCC_PLLDivOut: specifies the PLL Div_out factor.
 * @param[in] RCC_PLLDivPre: specifies the PLL Div_Pre factor.
 */
void RCC_PLLConfig(uint32_t RCC_PLLSource, uint32_t RCC_PLLLoop, uint32_t RCC_PLLDivOut, uint32_t RCC_PLLDivPre);

/**
 * @brief Enables or disables the PLL.
 * @param[in] NewState: new state of the PLL. This parameter can be: ENABLE or DISABLE.
 */
void RCC_PLLCmd(FunctionalState NewState);

/**
 * @brief Enables or disables the specified RCC interrupts.
 * @param[in] RCC_IT: specifies the RCC interrupt sources to be enabled or disabled.
 * @param[in] NewState: new state of the specified RCC interrupts.
 */
void RCC_ITConfig(uint8_t RCC_IT, FunctionalState NewState);

/**
 * @brief Configures the system clock (SYSCLK).
 * @param[in] RCC_SYSCLKSource specifies the clock source used as system clock.
 */
void RCC_SYSCLKConfig(uint32_t RCC_SYSCLKSource);

/**
 * @brief Returns the frequencies of different on chip clocks.
 * @param[in] RCC_Clocks: pointer to a RCC_ClocksTypeDef structure which will hold
 *            the clocks frequencies.
 */
void RCC_GetClocksFreq(RCC_ClocksTypeDef *RCC_Clocks);

/**
 * @brief Configures the External High Speed oscillator (HSE).
 * @param[in] RCC_HSE: specifies the new state of the HSE.
 */
void RCC_HSEConfig(uint32_t RCC_HSE);

/**
 * @brief Configures the AHB clock (HCLK).
 * @param[in] RCC_SYSCLK: defines the AHB clock divider.
 */
void RCC_HCLKConfig(uint32_t RCC_SYSCLK);

/**
 * @brief Configures the APB1 clock (PCLK1).
 * @param[in] RCC_HCLK: defines the APB1 clock divider.
 */
void RCC_PCLK1Config(uint32_t RCC_HCLK);

/**
 * @brief Configures the APB2 clock (PCLK1).
 * @param[in] RCC_HCLK: defines the APB1 clock divider.
 */
void RCC_PCLK2Config(uint32_t RCC_HCLK);

/**
 * @brief Enables or disables the Internal High Speed oscillator (HSI). 
 * @param[in] NewState: new state of the HSI. This parameter can be: ENABLE or DISABLE.
 */
void RCC_HSICmd(FunctionalState NewState);

/**
 * @brief Enables or disables the AHB peripheral clock.
 * @param[in] RCC_AHBPeriph: specifies the AHB peripheral to gates its clock.
 * @param[in] NewState: new state of the specified peripheral clock.
 */
void RCC_AHBPeriphClockCmd(uint32_t RCC_AHBPeriph, FunctionalState NewState);

/**
 * @brief Forces or releases AHB peripheral reset.
 * @param[in] RCC_AHBPeriph: specifies the AHB peripheral to reset.
 * @param[in] NewState: new state of the specified peripheral reset.
 */
void RCC_AHBPeriphResetCmd(uint32_t RCC_AHBPeriph, FunctionalState NewState);

/**
 * @brief Enables or disables the APB2 peripheral clock.
 * @param[in] RCC_APB2Periph: specifies the APB2 peripheral to gates its clock.
 * @param[in] NewState: new state of the specified peripheral clock.
 */
void RCC_APB2PeriphClockCmd(uint32_t RCC_APB2Periph, FunctionalState NewState);

/**
 * @brief Forces or releases APB2 peripheral reset.
 * @param[in] RCC_APB2Periph: specifies the APB2 peripheral to reset.
 * @param[in] NewState: new state of the specified peripheral reset.
 */
void RCC_APB2PeriphResetCmd(uint32_t RCC_APB2Periph, FunctionalState NewState);

/**
 * @brief Enables or disables the APB1 peripheral clock. 
 * @param[in] RCC_APB1Periph: specifies the APB1 peripheral to gates its clock.
 * @param[in] NewState: new state of the specified peripheral clock.
 */
void RCC_APB1PeriphClockCmd(uint32_t RCC_APB1Periph, FunctionalState NewState);

/**
 * @brief Forces or releases APB1 peripheral reset. 
 * @param[in] RCC_APB1Periph: specifies the APB1 peripheral to reset.
 * @param[in] NewState: new state of the specified peripheral reset.
 */
void RCC_APB1PeriphResetCmd(uint32_t RCC_APB1Periph, FunctionalState NewState);

/**
 * @brief Configures the USB OTG FS clock (OTGFSCLK).
 * @param[in] RCC_OTGFSCLKSource: specifies the USB OTG FS clock source.
 */
void RCC_OTGFSCLKConfig(uint32_t RCC_OTGFSCLKSource);

/**
 * @brief Configures the ADC clock (ADCCLK).
 * @param[in] RCC_PCLK2: defines the ADC clock divider. 
 */
void RCC_ADCCLKConfig(uint32_t RCC_PCLK2);

/**
 * @brief Configures the UART clock (UARTCLK).
 * @param[in] RCC_UartDivMain: defines the uart main divider. 
 * @param[in] RCC_UartDivNum: defines the uart num divider. 
 */
void RCC_UartClkConfig(uint32_t RCC_UartDivMain, uint32_t RCC_UartDivNum);

/**
 * @brief Selects the clock source to output on MCO pin.
 * @param[in] RCC_MCO: specifies the clock source to output.
 */
void RCC_MCOConfig(uint8_t RCC_MCO);

/**
 * @brief Clears the RCC's interrupt pending bits.
 * @param[in] RCC_IT: specifies the interrupt pending bit to clear.
 */
void RCC_ClearITPendingBit(uint8_t RCC_IT);

/**
 * @brief Waits for PLL Lock
 * @return ErrorStatus: An ErrorStatus enumuration value.
 */
ErrorStatus RCC_WaitForPllLock(void);

/**
 * @brief Checks whether the specified RCC flag is set or not.
 * @param[in] RCC_FLAG: specifies the flag to check.
 * @return FlagStatus The new state of RCC_FLAG (SET or RESET).
 */
FlagStatus RCC_GetFlagStatus(uint8_t RCC_FLAG);

/**
 * @brief Checks whether the specified RCC interrupt has occurred or not. 
 * @param[in] RCC_IT: specifies the RCC interrupt source to check.
 * @return ITStatus  The new state of RCC_IT (SET or RESET).
 */
ITStatus RCC_GetITStatus(uint8_t RCC_IT);

/**
 * @brief Adjusts the Internal High Speed oscillator (HSI) calibration value.
 * @param[in] Ctune: Coarse tuning control words of oscillator.
 * @param[in] Ftune: Fine tuning control words of oscillator frequency.
 */
void RCC_AdjustHSICalibrationValue(uint8_t Ctune, uint8_t Ftune);

/**
 * @brief Enables or disables the Clock Security System.
 * @param[in] NewState: new state of the Clock Security System.
 */
void RCC_ClockSecuritySystemCmd(FunctionalState NewState);

/**
 * @brief RCC Global Reset
 */
void RCC_GlobaLReset(void);

#ifdef __cplusplus
}
#endif

#endif