/**
 * @file at103_rcc.h
 * @brief This file contains all the functions prototypes for the RCC firmware library.
 *        How to use this driver? \n
 * (+) Use RCC_DeInit(...) to resets the RCC clock configuration to the default reset state. \n
 * (+) Use RCC_GetClocksFreq(...) to returns the frequencies of different on chip clocks.
 *     For details on the RCC Clocks structure, see "RCC_ClocksTypeDef" \n
 * (+) Use RCC_AHBPeriphClockCmd(...) or RCC_APB1PeriphClockCmd(...) or RCC_APB2PeriphClockCmd(...) to enable peripheral clock. \n
 * (+) Use RCC_AHBPeriphResetCmd(...) or RCC_APB1PeriphResetCmd(...) or RCC_APB2PeriphResetCmd(...) to reset peripheral.
 * @author zhangsheng (zhangsheng@zhangsheng.ic@gmail.com)
 * @version 1.0
 * @date 2022-01-13
 * @copyright Copyright (c) 2022 Icore, Inc
 */

#ifndef __AT103_RCC_H_
#define __AT103_RCC_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "at103.h"

#define RCC_HSE_OFF    ((uint32_t)0x00000001) /**< RCC HSE oscillator disable. */
#define RCC_HSE_ON     ((uint32_t)0x00000002) /**< RCC HSE oscillator enable. */
#define RCC_HSE_Bypass ((uint32_t)0x00000003) /**< RCC HSE oscillator bypassed. */

#define PLL_LOCK_TIMEOUT ((uint32_t)0x0000FFFF) /**< PLL Lock Timeout value. */

#define RCC_APB1Periph_TIM2  ((uint32_t)0x00000001) /**< APB1 peripheral: TIM2. */
#define RCC_APB1Periph_TIM3  ((uint32_t)0x00000002) /**< APB1 peripheral: TIM3. */
#define RCC_APB1Periph_TIM4  ((uint32_t)0x00000004) /**< APB1 peripheral: TIM4. */
#define RCC_APB1Periph_RTC   ((uint32_t)0x00000008) /**< APB1 peripheral: RTC. */
#define RCC_APB1Periph_WDT   ((uint32_t)0x00000010) /**< APB1 peripheral: WDT. */
#define RCC_APB1Periph_SPI2  ((uint32_t)0x00000020) /**< APB1 peripheral: SPI2. */
#define RCC_APB1Periph_UART2 ((uint32_t)0x00000040) /**< APB1 peripheral: UART2. */
#define RCC_APB1Periph_UART3 ((uint32_t)0x00000080) /**< APB1 peripheral: UART3. */
#define RCC_APB1Periph_I2C1  ((uint32_t)0x00000100) /**< APB1 peripheral: I2C1. */
#define RCC_APB1Periph_I2C2  ((uint32_t)0x00000200) /**< APB1 peripheral: I2C2. */
#define RCC_APB1Periph_CAN   ((uint32_t)0x00000400) /**< APB1 peripheral: CAN. */
#define RCC_APB1Periph_PMU   ((uint32_t)0x00000800) /**< APB1 peripheral: PMU. */

#define RCC_APB2Periph_AFIO  ((uint32_t)0x00010000) /**< APB2 peripheral clock enable: AFIO. */
#define RCC_APB2Periph_ITCL  ((uint32_t)0x00020000) /**< APB2 peripheral clock enable: ITCL. */
#define RCC_APB2Periph_GPIOA ((uint32_t)0x00040000) /**< APB2 peripheral clock enable: GPIOA. */
#define RCC_APB2Periph_GPIOB ((uint32_t)0x00080000) /**< APB2 peripheral clock enable: GPIOB. */
#define RCC_APB2Periph_GPIOC ((uint32_t)0x00100000) /**< APB2 peripheral clock enable: GPIOC. */
#define RCC_APB2Periph_GPIOD ((uint32_t)0x00200000) /**< APB2 peripheral clock enable: GPIOD. */
#define RCC_APB2Periph_GPIOE ((uint32_t)0x00400000) /**< APB2 peripheral clock enable: GPIOE. */
#define RCC_APB2Periph_ADC1  ((uint32_t)0x00800000) /**< APB2 peripheral clock enable: ADC1. */
#define RCC_APB2Periph_ADC2  ((uint32_t)0x01000000) /**< APB2 peripheral clock enable: ADC2. */
#define RCC_APB2Periph_TIM1  ((uint32_t)0x02000000) /**< APB2 peripheral clock enable: TIM1. */
#define RCC_APB2Periph_SPI1  ((uint32_t)0x04000000) /**< APB2 peripheral clock enable: SPI1. */
#define RCC_APB2Periph_UART1 ((uint32_t)0x08000000) /**< APB2 peripheral clock enable: UART1. */
#define RCC_APB2Periph_EFC   ((uint32_t)0x10000000) /**< APB2 peripheral clock enable: EFC. */

#define RCC_APB1PeriphRest_TIM2  ((uint32_t)0x00000001) /**< APB1 peripheral reset: TIM2. */
#define RCC_APB1PeriphRest_TIM3  ((uint32_t)0x00000002) /**< APB1 peripheral reset: TIM3. */
#define RCC_APB1PeriphRest_TIM4  ((uint32_t)0x00000004) /**< APB1 peripheral reset: TIM4. */
#define RCC_APB1PeriphRest_RTC   ((uint32_t)0x00000008) /**< APB1 peripheral reset: RTC. */
#define RCC_APB1PeriphRest_WDT   ((uint32_t)0x00000010) /**< APB1 peripheral reset: WDT. */
#define RCC_APB1PeriphRest_SPI2  ((uint32_t)0x00000020) /**< APB1 peripheral reset: SPI2. */
#define RCC_APB1PeriphRest_UART2 ((uint32_t)0x00000040) /**< APB1 peripheral reset: UART2. */
#define RCC_APB1PeriphRest_UART3 ((uint32_t)0x00000080) /**< APB1 peripheral reset: UART3. */
#define RCC_APB1PeriphRest_I2C1  ((uint32_t)0x00000100) /**< APB1 peripheral reset: I2C1. */
#define RCC_APB1PeriphRest_I2C2  ((uint32_t)0x00000200) /**< APB1 peripheral reset: I2C2. */
#define RCC_APB1PeriphRest_CAN   ((uint32_t)0x00000400) /**< APB1 peripheral reset: CAN. */
#define RCC_APB1PeriphRest_PMU   ((uint32_t)0x00000800) /**< APB1 peripheral reset: PMU. */

#define RCC_APB2PeriphRest_AFIO  ((uint32_t)0x00010000) /**< APB2 peripheral reset: AFIO. */
#define RCC_APB2PeriphRest_NVIC  ((uint32_t)0x00020000) /**< APB2 peripheral reset: NVIC. */
#define RCC_APB2PeriphRest_GPIOA ((uint32_t)0x00040000) /**< APB2 peripheral reset: GPIOA. */
#define RCC_APB2PeriphRest_GPIOB ((uint32_t)0x00080000) /**< APB2 peripheral reset: GPIOB. */
#define RCC_APB2PeriphRest_GPIOC ((uint32_t)0x00100000) /**< APB2 peripheral reset: GPIOC. */
#define RCC_APB2PeriphRest_GPIOD ((uint32_t)0x00200000) /**< APB2 peripheral reset: GPIOD. */
#define RCC_APB2PeriphRest_GPIOE ((uint32_t)0x00400000) /**< APB2 peripheral reset: GPIOE. */
#define RCC_APB2PeriphRest_ADC1  ((uint32_t)0x00800000) /**< APB2 peripheral reset: ADC1. */
#define RCC_APB2PeriphRest_ADC2  ((uint32_t)0x01000000) /**< APB2 peripheral reset: ADC2. */
#define RCC_APB2PeriphRest_TIM1  ((uint32_t)0x02000000) /**< APB2 peripheral reset: TIM1. */
#define RCC_APB2PeriphRest_SPI1  ((uint32_t)0x04000000) /**< APB2 peripheral reset: SPI1. */
#define RCC_APB2PeriphRest_UART1 ((uint32_t)0x08000000) /**< APB2 peripheral reset: UART1. */
#define RCC_APB2PeriphRest_RCC   ((uint32_t)0x10000000) /**< APB2 peripheral reset: RCC. */

#define RCC_AHBPeriph_USB ((uint32_t)0x00000001) /**< AHB peripheral clock enable: USB. */
#define RCC_AHBPeriph_CRC ((uint32_t)0x00000002) /**< AHB peripheral clock enable: CRC. */
#define RCC_AHBPeriph_DMA ((uint32_t)0x00000004) /**< AHB peripheral clock enable: DMA. */
#define RCC_AHBPeriph_EFC ((uint32_t)0x00000008) /**< AHB peripheral clock enable: EFC. */

#define RCC_IT_LSIRDY  ((uint8_t)0x01) /**< LSI (RC32K) clock ready interrupt status. */
#define RCC_IT_PLLRDY  ((uint8_t)0x10) /**< PLL output clock ready interrupt status. */
#define RCC_IT_HSESTOP ((uint8_t)0x80) /**< HSE (OSC8M) clock stop interrupt status. */

#define RCC_FLAG_PORRST ((uint8_t)0x00) /**< MCU boot from POR reset. */
#define RCC_FLAG_WDTRST ((uint8_t)0x01) /**< MCU boot from WDT reset. */
#define RCC_FLAG_SFTRST ((uint8_t)0x02) /**< MCU boot from SFT reset. */

/**
 * @brief RCC Clock Frequency definition. 
 */
typedef struct
{
    uint32_t SYSCLK_Frequency; /**< return SYSCLK clock frequency expressed in Hz. */
    uint32_t HCLK_Frequency;   /**< return AHBCLK clock frequency expressed in Hz. */
    uint32_t PCLK1_Frequency;  /**< return APBCLK1 clock frequency expressed in Hz. */
    uint32_t PCLK2_Frequency;  /**< return APBCLK2 clock frequency expressed in Hz. */
    uint32_t ADCCLK_Frequency; /**< return ADCCLK clock frequency expressed in Hz. */
    uint32_t USART_Frequencey; /**< return USART clock frequency expressed in Hz. */
    uint32_t USB_Frequencey;   /**< return USB clock frequency expressed in Hz. */
} RCC_ClocksTypeDef;

#define RCC_SYSCLK_Div1   ((uint32_t)0x00000000) /**< AHB system clock Prescaler: Div1. */
#define RCC_SYSCLK_Div2   ((uint32_t)0x00000001) /**< AHB system clock Prescaler: Div2. */
#define RCC_SYSCLK_Div4   ((uint32_t)0x00000003) /**< AHB system clock Prescaler: Div4. */
#define RCC_SYSCLK_Div8   ((uint32_t)0x00000007) /**< AHB system clock Prescaler: Div8. */
#define RCC_SYSCLK_Div16  ((uint32_t)0x0000000F) /**< AHB system clock Prescaler: Div16. */
#define RCC_SYSCLK_Div64  ((uint32_t)0x0000003F) /**< AHB system clock Prescaler: Div64. */
#define RCC_SYSCLK_Div128 ((uint32_t)0x0000007F) /**< AHB system clock Prescaler: Div128. */
#define RCC_SYSCLK_Div256 ((uint32_t)0x000000FF) /**< AHB system clock Prescaler: Div256. */
#define RCC_SYSCLK_Div512 ((uint32_t)0x000001FF) /**< AHB system clock Prescaler: Div512. */

#define RCC_HCLK_Div1  ((uint32_t)0x00000000) /**< APB high-speed clock Prescaler: Div1. */
#define RCC_HCLK_Div2  ((uint32_t)0x00000001) /**< APB high-speed clock Prescaler: Div2. */
#define RCC_HCLK_Div4  ((uint32_t)0x00000003) /**< APB high-speed clock Prescaler: Div4. */
#define RCC_HCLK_Div8  ((uint32_t)0x00000007) /**< APB high-speed clock Prescaler: Div8. */
#define RCC_HCLK_Div16 ((uint32_t)0x0000000F) /**< APB high-speed clock Prescaler: Div16. */

#define RCC_PLLSource_HSI_Div2 ((uint32_t)0x00000000) /**< PLL entry clock source: HSI oscillator clock / 2 selected as PLL input clock. */
#define RCC_PLLSource_HSE_Div1 ((uint32_t)0x00000008) /**< PLL entry clock source: HSE oscillator clock selected as PLL input clock. */
#define RCC_PLLSource_HSE_Div2 ((uint32_t)0x00000009) /**< PLL entry clock source: HSE oscillator clock / 2 selected as PLL input clock. */

#define RCC_SYSCLKSource_HSI    ((uint32_t)0x00000000) /**< RCC system_clock_source: HSI. */
#define RCC_SYSCLKSource_HSE    ((uint32_t)0x00000001) /**< RCC system_clock_source: HSE. */
#define RCC_SYSCLKSource_PLLCLK ((uint32_t)0x00000002) /**< RCC system_clock_source: PLLCLK. */

#define RCC_MCO_NoClock     ((uint8_t)0x00) /**< Microcontroller clock output: No clock. */
#define RCC_MCO_SYSCLK      ((uint8_t)0x04) /**< Microcontroller clock output: System clock (SYSCLK) selected. */
#define RCC_MCO_HSI         ((uint8_t)0x05) /**< Microcontroller clock output: HSI clock selected. */
#define RCC_MCO_HSE         ((uint8_t)0x06) /**< Microcontroller clock output: HSE clock selected. */
#define RCC_MCO_PLLCLK_Div2 ((uint8_t)0x07) /**< Microcontroller clock output: PLL clock divided by 2 selected. */

/**
 * @brief Resets the RCC clock configuration to the default reset state.
 */
void RCC_DeInit(void);

/**
 * @brief Configures the PLL clock source and multiplication factor.
 * @param[in] RCC_PLLSource specifies the PLL entry clock source.
 * @param[in] RCC_PLLLoop specifies the PLL multiplication factor.
 * @param[in] RCC_PLLDivOut specifies the PLL Div_out factor.
 * @param[in] RCC_PLLDivPre specifies the PLL Div_Pre factor.
 */
void RCC_PLLConfig(uint32_t RCC_PLLSource, uint32_t RCC_PLLLoop, uint32_t RCC_PLLDivOut, uint32_t RCC_PLLDivPre);

/**
 * @brief Enables or disables the PLL.
 * @param[in] NewState new state of the PLL. This parameter can be: ENABLE or DISABLE.
 */
void RCC_PLLCmd(FunctionalState NewState);

/**
 * @brief Enables or disables the specified RCC interrupts.
 * @param[in] RCC_IT specifies the RCC interrupt sources to be enabled or disabled.
 * @param[in] NewState new state of the specified RCC interrupts.
 */
void RCC_ITConfig(uint8_t RCC_IT, FunctionalState NewState);

/**
 * @brief Configures the system clock (SYSCLK).
 * @param[in] RCC_SYSCLKSource specifies the clock source used as system clock.
 */
void RCC_SYSCLKConfig(uint32_t RCC_SYSCLKSource);

/**
 * @brief Returns the frequencies of different on chip clocks.
 * @param[out] RCC_Clocks pointer to a RCC_ClocksTypeDef structure which will hold
 *             the clocks frequencies.
 */
void RCC_GetClocksFreq(RCC_ClocksTypeDef *RCC_Clocks);

/**
 * @brief Configures the External High Speed oscillator (HSE).
 * @param[in] RCC_HSE specifies the new state of the HSE.
 */
void RCC_HSEConfig(uint32_t RCC_HSE);

/**
 * @brief Configures the AHB clock (HCLK).
 * @param[in] RCC_SYSCLK defines the AHB clock divider.
 */
void RCC_HCLKConfig(uint32_t RCC_SYSCLK);

/**
 * @brief Configures the APB1 clock (PCLK1).
 * @param[in] RCC_HCLK defines the APB1 clock divider.
 */
void RCC_PCLK1Config(uint32_t RCC_HCLK);

/**
 * @brief Configures the APB2 clock (PCLK1).
 * @param[in] RCC_HCLK defines the APB1 clock divider.
 */
void RCC_PCLK2Config(uint32_t RCC_HCLK);

/**
 * @brief Enables or disables the Internal High Speed oscillator (HSI).
 * @param[in] NewState new state of the HSI. This parameter can be: ENABLE or DISABLE.
 */
void RCC_HSICmd(FunctionalState NewState);

/**
 * @brief Enables or disables the AHB peripheral clock.
 * @param[in] RCC_AHBPeriph specifies the AHB peripheral to gates its clock.
 * @param[in] NewState new state of the specified peripheral clock.
 */
void RCC_AHBPeriphClockCmd(uint32_t RCC_AHBPeriph, FunctionalState NewState);

/**
 * @brief Forces or releases AHB peripheral reset.
 * @param[in] RCC_AHBPeriph specifies the AHB peripheral to reset.
 * @param[in] NewState new state of the specified peripheral reset.
 */
void RCC_AHBPeriphResetCmd(uint32_t RCC_AHBPeriph, FunctionalState NewState);

/**
 * @brief Enables or disables the APB2 peripheral clock.
 * @param[in] RCC_APB2Periph specifies the APB2 peripheral to gates its clock.
 * @param[in] NewState new state of the specified peripheral clock.
 */
void RCC_APB2PeriphClockCmd(uint32_t RCC_APB2Periph, FunctionalState NewState);

/**
 * @brief Forces or releases APB2 peripheral reset.
 * @param[in] RCC_APB2Periph specifies the APB2 peripheral to reset.
 * @param[in] NewState new state of the specified peripheral reset.
 */
void RCC_APB2PeriphResetCmd(uint32_t RCC_APB2Periph, FunctionalState NewState);

/**
 * @brief Enables or disables the APB1 peripheral clock.
 * @param[in] RCC_APB1Periph specifies the APB1 peripheral to gates its clock.
 * @param[in] NewState new state of the specified peripheral clock.
 */
void RCC_APB1PeriphClockCmd(uint32_t RCC_APB1Periph, FunctionalState NewState);

/**
 * @brief Forces or releases APB1 peripheral reset.
 * @param[in] RCC_APB1Periph specifies the APB1 peripheral to reset.
 * @param[in] NewState new state of the specified peripheral reset.
 */
void RCC_APB1PeriphResetCmd(uint32_t RCC_APB1Periph, FunctionalState NewState);

/**
 * @brief Configures the USB OTG FS clock (OTGFSCLK).
 * @param[in] RCC_OTGFSCLKSource specifies the USB OTG FS clock source.
 */
void RCC_OTGFSCLKConfig(uint32_t RCC_OTGFSCLKSource);

/**
 * @brief Configures the ADC clock (ADCCLK).
 * @param[in] RCC_PCLK2 defines the ADC clock divider.
 */
void RCC_ADCCLKConfig(uint32_t RCC_PCLK2);

/**
 * @brief Configures the UART clock (UARTCLK).
 * @param[in] RCC_UartDivMain defines the uart main divider.
 * @param[in] RCC_UartDivNum defines the uart num divider.
 */
void RCC_UartClkConfig(uint32_t RCC_UartDivMain, uint32_t RCC_UartDivNum);

/**
 * @brief Selects the clock source to output on MCO pin.
 * @param[in] RCC_MCO specifies the clock source to output.
 */
void RCC_MCOConfig(uint8_t RCC_MCO);

/**
 * @brief Clears the RCC's interrupt pending bits.
 * @param[in] RCC_IT specifies the interrupt pending bit to clear.
 */
void RCC_ClearITPendingBit(uint8_t RCC_IT);

/**
 * @brief Waits for PLL Lock
 * @return ErrorStatus: An ErrorStatus enumuration value.
 */
ErrorStatus RCC_WaitForPllLock(void);

/**
 * @brief Checks whether the specified RCC flag is set or not.
 * @param[in] RCC_FLAG specifies the flag to check.
 * @return FlagStatus: The new state of RCC_FLAG (SET or RESET).
 */
FlagStatus RCC_GetFlagStatus(uint8_t RCC_FLAG);

/**
 * @brief Checks whether the specified RCC interrupt has occurred or not.
 * @param[in] RCC_IT specifies the RCC interrupt source to check.
 * @return ITStatus: The new state of RCC_IT (SET or RESET).
 */
ITStatus RCC_GetITStatus(uint8_t RCC_IT);

/**
 * @brief Adjusts the Internal High Speed oscillator (HSI) calibration value.
 * @param[in] Ctune Coarse tuning control words of oscillator.
 * @param[in] Ftune Fine tuning control words of oscillator frequency.
 */
void RCC_AdjustHSICalibrationValue(uint8_t Ctune, uint8_t Ftune);

/**
 * @brief Enables or disables the Clock Security System.
 * @param[in] NewState new state of the Clock Security System.
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