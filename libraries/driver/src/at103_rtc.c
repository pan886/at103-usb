/**
 * @file at103_rtc.c
 * @brief This file provides all the RTC firmware functions.
 * @author pantianwen (pantianwen@163.com)
 * @version 1.0
 * @date 2022-04-15
 * @copyright Copyright (c) 2022 Icore, Inc
 */
#include "at103_rtc.h"

#ifdef RTC_MODULE_ENABLED

#define IS_RTC_IT(IT) ((IT == RTC_IT_ALR1) || (IT == RTC_IT_ALR2))

#define IS_RTC_PERIOD(RTC_Period) ((RTC_Period == RTC_PERIOD_NOOUTPUT) || (RTC_Period == RTC_PERIOD_1_sec) || \
                                   (RTC_Period == RTC_PERIOD_1_sec_2) || (RTC_Period == RTC_PERIOD_1_sec_4) || \
                                   (RTC_Period == RTC_PERIOD_1_sec_8) || (RTC_Period == RTC_PERIOD_1_sec_16) || \
                                   (RTC_Period == RTC_PERIOD_1_sec_32) || (RTC_Period == RTC_PERIOD_1_sec_64) || \
                                   (RTC_Period == RTC_PERIOD_1_sec_128) || (RTC_Period == RTC_PERIOD_1_min))

#define IS_BKP_DR(DR) (((DR) == BKP_DR1) || ((DR) == BKP_DR2) || ((DR) == BKP_DR3) || \
                       ((DR) == BKP_DR4) || ((DR) == BKP_DR5) || ((DR) == BKP_DR6) || \
                       ((DR) == BKP_DR7) || ((DR) == BKP_DR8) || ((DR) == BKP_DR9) || \
                       ((DR) == BKP_DR10))

#define IS_BKP_TAMPER_PIN_LEVEL(LEVEL) (((LEVEL) == BKP_TamperPinLevel_High) || \
                                        ((LEVEL) == BKP_TamperPinLevel_Low) || \
                                        ((LEVEL) == BKP_TamperPinLevel_Both_edge) || \
                                        ((LEVEL) == BKP_TamperPinLevel_Pos_Over_Deb) || \
                                        ((LEVEL) == BKP_TamperPinLevel_Neg_Over_Deb))

void RTC_WaitForLastTask(void)
{
    while (RTC->RTC_PEND_WR.PEND_WR == 1)
        ;
}

void RTC_SelClk(uint8_t RTCCLK)
{
    RTC_WaitForLastTask();
    RTC->OSC32K_CONTROL_1.XTAL32K_EN = 0x1; //Enables the crystal driver
    RTC_WaitForLastTask();
    RTC->OSC32K_CONTROL_1.RTC_SEL = RTCCLK;
    RTC_WaitForLastTask();
}

void RTC_SetTimeDate(RTC_TimeTypeDef time)
{
    RTC->RTC_WRSTA_A7.WRSTA = 0x1;

    RTC->SECOND.SEC = time.sec;
    RTC_WaitForLastTask();
    RTC->MINUTE.MIN = time.min;
    RTC_WaitForLastTask();
    RTC->HOUR.H12_24 = 0x1;
    RTC_WaitForLastTask();

    if (time.hour >= 0x20) {
        RTC->HOUR.H20_PA = 1;
        RTC_WaitForLastTask();
        RTC->HOUR.HOUR19 = time.hour - 0x20;
    } else {
        RTC->HOUR.H20_PA = 0;
        RTC_WaitForLastTask();
        RTC->HOUR.HOUR19 = time.hour;
    }
    RTC_WaitForLastTask();

    RTC->WEEK.WK = time.week;
    RTC_WaitForLastTask();
    RTC->DAY.DY = time.day;
    RTC_WaitForLastTask();
    RTC->CEN_MONTH.MONTH = time.month;
    RTC_WaitForLastTask();
    RTC->YEAR.YR = time.year;
    RTC_WaitForLastTask();
    RTC->RTC_WRSTP_A7.WRSTP = 0x1;
}

void RTC_SetAlarm(RTC_TimeTypeDef time)
{
    RTC->ALM1_SEC.SEC = time.sec;
    RTC_WaitForLastTask();
    RTC->ALM1_MIN.MIN = time.min;
    RTC_WaitForLastTask();
    RTC->ALM1_HOUR.H12_24 = 0x0;
    RTC_WaitForLastTask();
    RTC->ALM1_HOUR.HOUR19 = time.hour;
    RTC_WaitForLastTask();
    RTC->ALM1_WEEK.WEEK = time.week;
    RTC_WaitForLastTask();
    RTC->ALM1_DAY.DAY = time.day;
    RTC_WaitForLastTask();
    RTC->ALM1_MONTH.MON = time.month;
    RTC_WaitForLastTask();
    RTC->ALM1_YEAR.YEAR = time.year;
    RTC_WaitForLastTask();
    RTC->ALM1_MASK.value = 0x6;
    RTC_WaitForLastTask();
    RTC->ALM_EN.ALM1_EN = 0x1;
    RTC_WaitForLastTask();
    RTC->IRQ_MASK.value = 0x2;
    RTC_WaitForLastTask();
}

void RTC_SetSecINT(uint16_t RTC_Period)
{
    assert_param(IS_RTC_PERIOD(RTC_Period));
    RTC->ALM2_SET.ALM2_PDSET = RTC_Period;
    RTC_WaitForLastTask();
    RTC->ALM_EN.ALM2_EN = 0x1;
    RTC_WaitForLastTask();
    RTC->IRQ_MASK.value = 0x0;
    RTC_WaitForLastTask();
}

void RTC_ClearITPendingBit(uint16_t RTC_IT)
{
    assert_param(IS_RCC_IT(RTC_IT));
    RTC->RTC_IRQ_CLR.value = RTC_IT;
    RTC_WaitForLastTask();
}

void BKP_TamperClearIT(void)
{
    RTC->TAMP_IRQ_CLR.CLR = 0x1;
    RTC_WaitForLastTask();
}

void BKP_SwRst()
{
    RTC->BBU_SOFT_RST.PWRUP_TIMER_CFG = 0x1;
    RTC_WaitForLastTask();
}

void BKP_TamperInit()
{
    GPIO_InitTypeDef GPIOC_struct1;
    GPIOC_struct1.GPIO_Mode = GPIO_Mode_AF;
    GPIOC_struct1.GPIO_Pin  = GPIO_Pin_13;

    /*initialization pin PC13*/
    GPIO_Init(GPIOC, &GPIOC_struct1);
    RTC->PC13_IO_CONTRL.TAMPER_RTC_IS = 0x1;
    RTC_WaitForLastTask();
    RTC->PC13_IO_CONTRL.TAMPER_RTC_IE = 0x1;
    RTC_WaitForLastTask();
    RTC->PC13_IO_CONTRL.TAMPER_RTC_PE = 0x1;
    RTC_WaitForLastTask();
    RTC->PC13_IO_CONTRL.TAMPER_RTC_PS = 0x1;
    RTC_WaitForLastTask();
}

void BKP_TamperIrqEnable(void)
{
    RTC->TAMP_IRQ_EN.ENA = 0x1;
    RTC_WaitForLastTask();
}

uint8_t BKP_TamperGetCounter()
{
    return RTC->TAMP_COUNT.CNT;
}

void BKP_TamperClearCounter()
{
    RTC->TAMP_COUNT_CLR.CLR = 0x1;
    RTC_WaitForLastTask();
}

ITStatus BKP_GetITStatus(void)
{
    return RTC->TAMP_IRQ_STA.STA;
}

void BKP_TamperPinCmd(FunctionalState NewState)
{
    assert_param(IS_FUNCTIONAL_STATE(NewState));
    RTC->TAMP_CTRL.ENA = NewState;
    RTC_WaitForLastTask();
}

void BKP_TamperPinLevelConfig(uint16_t BKP_TamperPinLevel)
{
    assert_param(IS_BKP_TAMPER_PIN_LEVEL(BKP_TamperPinLevel));
    RTC->TAMP_CONFIG.DET_MODE = BKP_TamperPinLevel;
    RTC_WaitForLastTask();
}

void BKP_WriteBackupRegister(uint16_t BKP_DR, uint16_t Data)
{
    /* Check the parameters */
    assert_param(IS_BKP_DR(BKP_DR));
    RTC->BKP_DR[2 * (BKP_DR - 1)].BKP = Data >> 8;
    while (RTC->RTC_PEND_WR.PEND_WR == 1)
        ;
    RTC->BKP_DR[2 * (BKP_DR - 1) + 1].BKP = Data & 0xff;
    while (RTC->RTC_PEND_WR.PEND_WR == 1)
        ;
}

uint16_t BKP_ReadBackupRegister(uint16_t BKP_DR)
{
    /* Check the parameters */
    assert_param(IS_BKP_DR(BKP_DR));
    return ((RTC->BKP_DR[2 * (BKP_DR - 1)].BKP) << 8) | (RTC->BKP_DR[2 * (BKP_DR - 1) + 1].BKP);
    while (RTC->RTC_PEND_WR.PEND_WR == 1)
        ;
}

void BKP_RTCOutputConfig()
{
    RTC->PC13_IO_CONTRL.TAMPER_RTC_IS = 0x1;
    RTC_WaitForLastTask();
    RTC->PC13_IO_CONTRL.TAMPER_SEL = 0x1;
    RTC_WaitForLastTask();
    RTC->PC13_IO_CONTRL.TAMPER_RTC_OE = 0x1;
    RTC_WaitForLastTask();
    RTC->PC13_IO_CONTRL.TAMPER_RTC_IE = 0x0;
    RTC_WaitForLastTask();
    RTC->PC13_IO_CONTRL.TAMPER_RTC_DR = 0x3;
    RTC_WaitForLastTask();
    RTC->ALM_EN.ALM1_EN = 0x1;
    RTC_WaitForLastTask();
    RTC->IRQ_MASK.ALM1_IRQ_MASK = 0;
    RTC_WaitForLastTask();
}

void BKP_SetRTCCalibrationValue(RTC_TrimTypeDef trim)
{
    RTC->TRIM_EN.ENABLE = trim.enable;
    RTC_WaitForLastTask();
    RTC->VLD_EN.ENABLE = trim.VLD_EN;
    RTC_WaitForLastTask();
    RTC->TRIM_MODE.TRIM_MODE = trim.trim_mode;
    RTC_WaitForLastTask();
    RTC->TRIM_MODE.VLD_MODE = trim.VLD_mode;
    RTC_WaitForLastTask();
    RTC->TRIM.COMPENSATE = trim.compensate;
    RTC_WaitForLastTask();
}

#endif