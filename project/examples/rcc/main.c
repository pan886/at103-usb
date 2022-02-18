/**
 * @file main.c
 * @brief rcc module example.
 * 
 * @author zhangsheng (zhangsheng@zhangsheng.ic@gmail.com)
 * @version 1.0
 * @date 2022-01-17
 * @copyright Copyright (c) 2022 Icore, Inc
 */

#include "at103.h"

/* Private variables ---------------------------------------------------------*/
static RCC_ClocksTypeDef Clocks;
ErrorStatus              PllLockStatus;
uint8_t                  ErrorCnt;
/* Private macro -------------------------------------------------------------*/
#define CFG_SYSCLK_FREQ 36000000
#define CFG_AHB_FREQ    36000000
#define CFG_APB1_FREQ   18000000
#define CFG_APB2_FREQ   18000000
#define CFG_USB_FREQ    12000000
#define CFG_ADC_FREQ    9000000

#define CLK_CFGR_TRIM_Mask ((uint32_t)0x000FFF00)

/* Private function prototypes -----------------------------------------------*/
void SetSysClockTo36(void);
void SetSysClockTo144(void);
void SetSysClockTo72(void);
void RccMcoInit(void);
void RCCInterruptInit(void);
void RCCInterruptCallBack(void);

/* Private functions ---------------------------------------------------------*/

void main(void)
{
    pll_init();
    sys_io_init();
    uart_init(UART_BOOT_PORT, UART_PARITY_NONE, UART_STOPBITS_1, UART_DATABITS_8, UART_BOOT_BD);

    /* Clear ErrorCnt */
    ErrorCnt = 0;

    /* Mco GPIO Init */
    RccMcoInit();

    /* Enable RCC Interrupt. */
    RCCInterruptInit();

    /* Change System Clock to 36Mhz,AHB 36Mhz,APB1=APB2 18Hz,ADC 9Mhz,USB 12Mhz */
    SetSysClockTo36();

    /* Get Frequence */
    RCC_GetClocksFreq(&Clocks);

    /* Switch to 144Mhz,Use HSE*/
    SetSysClockTo144();

    /* Switch to 72Mhz,USE HSE or HSI*/
    SetSysClockTo72();

    /* MCO Output */
    RCC_MCOConfig(RCC_MCO_SYSCLK);

    /* Forces Uart2 Reset */
    RCC_APB1PeriphResetCmd(RCC_APB1PeriphRest_UART2, ENABLE);

    /* Release Uart2 Reset */
    RCC_APB1PeriphResetCmd(RCC_APB1PeriphRest_UART2, DISABLE);

    /* Reconfig Uart Port*/
    uart_init(UART_BOOT_PORT, UART_PARITY_NONE, UART_STOPBITS_1, UART_DATABITS_8, UART_BOOT_BD);

    /* 72Mhz / (2+ 14 / 32) = 29.46Mhz */
    RCC_UartClkConfig(2, 14);

    if (Clocks.SYSCLK_Frequency != CFG_SYSCLK_FREQ ||
        Clocks.USB_Frequencey != CFG_USB_FREQ ||
        Clocks.ADCCLK_Frequency != CFG_ADC_FREQ ||
        Clocks.PCLK1_Frequency != CFG_APB1_FREQ ||
        Clocks.PCLK2_Frequency != CFG_APB2_FREQ || ErrorCnt > 0) {
        debug("RCC Test Fail!!!\n");
    } else {
        debug("RCC Test Sucess!!!\n");
    }

    /* Main Loop*/
    while (1) {
    }
}

/**
 * @brief Config MCO GPIO
 */
void RccMcoInit(void)
{
    volatile uint32_t tmp_value;
    /*PA8 AF Select*/
    tmp_value = readl(AFIO_GPIOA_FUNC_SEL);
    tmp_value &= ~(3 << 16);
    tmp_value |= 0x01 << 16;
    writel(tmp_value, AFIO_GPIOA_FUNC_SEL);

    /*Remap Time1 Ch1  To 0x03 Not Use PA8*/
    tmp_value = readl(AFIO_PIN_REMAP_CTL);
    tmp_value &= ~(3 << 6);
    tmp_value |= 3 << 6;
    writel(tmp_value, AFIO_PIN_REMAP_CTL);
}

/**
  * @brief  Sets System clock frequency to 36MHz and configure HCLK, PCLK2 
  *         and PCLK1 prescalers,Use HSE. 
  * @param  None
  * @retval None
  */
void SetSysClockTo36(void)
{
    /* SYSCLK, HCLK, PCLK2 and PCLK1 configuration -----------------------------*/
    /* RCC system reset(for debug purpose) */
    RCC_DeInit();

    /* Enable HSE */
    RCC_HSEConfig(RCC_HSE_ON);

    /* PLLCLK = ((8MHz/2) *36)/(4*1) = 36 MHz */
    RCC_PLLConfig(RCC_PLLSource_HSE_Div2, 0x24, 0x04, 0x01);

    /* Enable PLL */
    RCC_PLLCmd(ENABLE);

    /* Wait For PLL Lock */
    PllLockStatus = RCC_WaitForPllLock();

    if (PllLockStatus != ERROR) {
        /* HCLK = SYSCLK */
        RCC_HCLKConfig(RCC_SYSCLK_Div1);

        /* PCLK2 = HCLK/2 */
        RCC_PCLK2Config(RCC_HCLK_Div2);

        /* PCLK1 = HCLK/2 */
        RCC_PCLK1Config(RCC_HCLK_Div2);

        /* Select PLL as system clock source */
        RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);

        /* USB OTG FS CLK = PLLCLK/( 2 + 1)=36/3=12Mhz, should be 12Mhz */
        RCC_OTGFSCLKConfig(2);

        /* ADC CLK = PCLK2/(1+1)=18/2=9Mhz,should not exceed 14MHz*/
        RCC_ADCCLKConfig(1);
    } else { /* PLL Lock Fail */
        ErrorCnt++;
    }
}

/**
  * @brief  Sets System clock frequency to 144MHz and configure HCLK, PCLK2 
  *         and PCLK1 prescalers,Use HSE. 
  * @param  None
  * @retval None
  */
void SetSysClockTo144(void)
{
    /* SYSCLK, HCLK, PCLK2 and PCLK1 configuration -----------------------------*/
    /* RCC system reset(for debug purpose) */
    RCC_DeInit();

    /* Enable HSE */
    RCC_HSEConfig(RCC_HSE_ON);

    /* PLLCLK = ( ( 8MHz  ) * 36 ) / ( 2 * 1 ) = 144 MHz */
    RCC_PLLConfig(RCC_PLLSource_HSE_Div1, 0x24, 0x02, 0x01);

    /* Enable PLL */
    RCC_PLLCmd(ENABLE);

    /* Wait For Pll Ready */
    PllLockStatus = RCC_WaitForPllLock();

    if (PllLockStatus != ERROR) {
        /* HCLK = SYSCLK */
        RCC_HCLKConfig(RCC_SYSCLK_Div1);

        /* PCLK2 = HCLK */
        RCC_PCLK2Config(RCC_HCLK_Div2);

        /* PCLK1 = HCLK */
        RCC_PCLK1Config(RCC_HCLK_Div2);

        /* Select PLL as system clock source */
        RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
    } else { /* PLL Lock Fail */
        ErrorCnt++;
    }
}

/**
  * @brief  Sets System clock frequency to 72MHz and configure HCLK, PCLK2 
  *         and PCLK1 prescalers,Use HSI. 
  * @param  None
  * @retval None
  */
void SetSysClockTo72(void)
{
    /* SYSCLK, HCLK, PCLK2 and PCLK1 configuration -----------------------------*/
    /* RCC system reset(for debug purpose) */
    RCC_DeInit();

    /* Enable HSI */
    //RCC_HSICmd(ENABLE);

    /* Enable HSE */
    RCC_HSEConfig(RCC_HSE_ON);

    /* PLLCLK = ( ( 8MHz/2  ) * 36 ) / ( 2 * 1 ) = 72 MHz */
    //RCC_PLLConfig(RCC_PLLSource_HSI_Div2, 0x24, 0x02, 0x01);

    RCC_PLLConfig(RCC_PLLSource_HSE_Div2, 0x24, 0x02, 0x01);

    /* Enable PLL */
    RCC_PLLCmd(ENABLE);

    /* Wait For Pll Ready */
    PllLockStatus = RCC_WaitForPllLock();

    if (PllLockStatus != ERROR) {
        /* HCLK = SYSCLK */
        RCC_HCLKConfig(RCC_SYSCLK_Div1);

        /* PCLK2 = HCLK */
        RCC_PCLK2Config(RCC_HCLK_Div2);

        /* PCLK1 = HCLK */
        RCC_PCLK1Config(RCC_HCLK_Div2);

        /* Select PLL as system clock source */
        RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);

    } else { /* PLL Lock Fail */
        ErrorCnt++;
    }
}

/**
 * @brief ENable NVIC & RCC Interrupt
 */
void RCCInterruptInit(void)
{
    /* NVIC Config*/
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
    NVIC_InitStructure.NVIC_IRQChannel                   = RCC_IRQn;

    NVIC_Init(&NVIC_InitStructure);

    /* Core Interrupt enable*/
    __enable_irq();

    /* RCC Irq Enable*/
}

/**
 * @brief RCC Interrupt Call Back Function
 */
void RCCInterruptCallBack(void)
{
    if (RCC_GetITStatus(RCC_IT_PLLRDY) != RESET) {
        /* PLL Ready Interrupt*/
        RCC_ClearITPendingBit(RCC_IT_PLLRDY);
    }

    if (RCC_GetITStatus(RCC_IT_HSESTOP) != RESET) {
        /* HSE Stop Interrupt*/
        RCC_ClearITPendingBit(RCC_IT_HSESTOP);
    }

    if (RCC_GetITStatus(RCC_IT_LSIRDY) != RESET) {
        /* LSI Ready Interrupt*/
        RCC_ClearITPendingBit(RCC_IT_LSIRDY);
    }
}

void RCC_IRQHandler(void)
{
    /* exec callback function */
    RCCInterruptCallBack();
}

/**
 * @brief   Reports the name of the source file and the source line number
 * @param[in] file: pointer to the source file name
 * @param[in] line: assert_param error line source number 
 */
void assert_failed(uint8_t *file, uint32_t line)
{
    debug("Wrong parameters value: file %s on line %d\r\n", file, line);

    while (1) {
    }
}