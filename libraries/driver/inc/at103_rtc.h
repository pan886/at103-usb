/**
 * @file at103_rtc.h
 * @brief This file contains all the functions prototypes for the RTC firmware library. \n
 *        How to use this driver? \n
 * (+) At first,use RTC_SelClk(...) to select the clock of RTC. \n
 * (+) Then,use RTC_SetAlarm(...) to config the alarma and use RTC_SetTimeDate(...) set the real time. \n
 * (+) Use RTC_SetSecINT(...) to set the interrput output. 
 * @author pantianwen (pantianwen@163.com)
 * @version 1.0
 * @date 2022-04-30
 * @copyright Copyright (c) 2022 Icore, Inc
 */
#ifndef _AT103_RTC_H
#define _AT103_RTC_H

#ifdef __cplusplus
extern "C" {
#endif

#include "at103.h"

#define RTC_LSE ((uint16_t)0x0001) /**< Select LSE clock. */
#define RTC_LSI ((uint16_t)0x0002) /**< Select LSI clock. */
#define RTC_HSE ((uint16_t)0x0003) /**< Select HSE clock. */

#define RTC_IT_ALR1 ((uint16_t)0x0001) /**< Alarm1 interrupt. */
#define RTC_IT_ALR2 ((uint16_t)0x0002) /**< Alarm2 interrupt. */

#define BKP_TamperPinLevel_High         ((uint16_t)0x0000) /**< Tamper Pin active high level.*/
#define BKP_TamperPinLevel_Low          ((uint16_t)0x0001) /**< Tamper Pin active low level.*/
#define BKP_TamperPinLevel_Both_edge    ((uint16_t)0x0002) /**< Tamper Pin active Both edge.*/
#define BKP_TamperPinLevel_Pos_Over_Deb ((uint16_t)0x0003) /**< Tamper Pin active Rising edge. */
#define BKP_TamperPinLevel_Neg_Over_Deb ((uint16_t)0x0004) /**< Tamper Pin active Falling edge.*/

#define BKP_DR1  ((uint16_t)0x0001) /**< Backup data register1.*/
#define BKP_DR2  ((uint16_t)0x0002) /**< Backup data register2.*/
#define BKP_DR3  ((uint16_t)0x0003) /**< Backup data register3.*/
#define BKP_DR4  ((uint16_t)0x0004) /**< Backup data register4.*/
#define BKP_DR5  ((uint16_t)0x0005) /**< Backup data register5.*/
#define BKP_DR6  ((uint16_t)0x0006) /**< Backup data register6.*/
#define BKP_DR7  ((uint16_t)0x0007) /**< Backup data register7.*/
#define BKP_DR8  ((uint16_t)0x0008) /**< Backup data register8.*/
#define BKP_DR9  ((uint16_t)0x0009) /**< Backup data register9.*/
#define BKP_DR10 ((uint16_t)0x000A) /**< Backup data register10.*/

#define RTC_TAMP ((uint16_t)0x0001) /**< Tamper interrupt */

#define RTC_PERIOD_NOOUTPUT  ((uint16_t)0x0000) /*!< Not output. */
#define RTC_PERIOD_1_sec     ((uint16_t)0x0001) /*!< Period of alarm2 interrupt 1 sec. */
#define RTC_PERIOD_1_sec_2   ((uint16_t)0x0002) /*!< Period of alarm2 interrupt 1/2 sec. */
#define RTC_PERIOD_1_sec_4   ((uint16_t)0x0003) /*!< Period of alarm2 interrupt 1/4 sec. */
#define RTC_PERIOD_1_sec_8   ((uint16_t)0x0004) /*!< Period of alarm2 interrupt 1/8 sec. */
#define RTC_PERIOD_1_sec_16  ((uint16_t)0x0005) /*!< Period of alarm2 interrupt 1/16 sec.*/
#define RTC_PERIOD_1_sec_32  ((uint16_t)0x0006) /*!< Period of alarm2 interrupt 1/32 sec. */
#define RTC_PERIOD_1_sec_64  ((uint16_t)0x0007) /*!< Period of alarm2 interrupt 1/64 sec. */
#define RTC_PERIOD_1_sec_128 ((uint16_t)0x0008) /*!< Period of alarm2 interrupt 1/128 sec. */
#define RTC_PERIOD_1_min     ((uint16_t)0x0009) /*!< Period of alarm2 interrupt 1 min.*/

/**
 * @brief Selection of week.
 */
typedef enum {
    Sunday = 0,
    Monday,
    Tuesday,
    Wednesday,
    Thursday,
    Friday,
    Saturday
} RTC_WeekTypeDef;

/**
 * @brief Selection of month.
 */
typedef enum {
    January   = 0x1,
    February  = 0x2,
    March     = 0x3,
    April     = 0x4,
    May       = 0x5,
    June      = 0x6,
    July      = 0x7,
    August    = 0x8,
    September = 0x9,
    October   = 0x10,
    November  = 0x11,
    December  = 0x12
} RTC_MonthTypeDef;

/**
 * @brief RTC time and date  structure.
 */
typedef struct
{
    uint8_t          sec;   /**< second. */
    uint8_t          min;   /**< minute. */
    uint8_t          hour;  /**< hour. */
    uint8_t          day;   /**< day. */
    RTC_WeekTypeDef  week;  /**< week.*/
    RTC_MonthTypeDef month; /**< month.*/
    uint8_t          year;  /**< year.*/
} RTC_TimeTypeDef;

/**
 * @brief Configuration of trim mode.
 */
typedef enum {
    every_60sec = 0x0,
    every_30sec,
    every_15sec,
    every_6sec,
} RTC_TrimmodeTypeDef;

/**
 * @brief Configuration of VLD.
 */
typedef enum {
    VLD_ON_60sec = 0,
    VLD_ON_30sec,
    VLD_ON_15sec,
    VLD_ON_6sec,
} RTC_VldmodeTypeDef;

/**
 * @brief Trim init structure definition.
 */
typedef struct
{
    uint8_t             compensate; /**< control register of the clock error compensate function */
    RTC_TrimmodeTypeDef trim_mode;  /**< controls the timing of clock adjustment.*/
    RTC_VldmodeTypeDef  VLD_mode;   /**< Setting for VLD_ON_O for trimming function */
    uint8_t             enable;     /**< Trimming enable control */
    uint8_t             VLD_EN;     /**< Validation enable control*/
} RTC_TrimTypeDef;

/**
 * @brief Waits until last write operation on RTC registers has finished.
 */
void RTC_WaitForLastTask(void);

/**
 * @brief Select the clock of RTC.
 * @param[in] RTCCLK  Can be RTC_LSE,RTC_HSE,RTC_LSI.
 */
void RTC_SelClk(uint8_t RTCCLK);

/**
 * @brief Set the time and date of RTC.
 * @param[in] time RTC alarm new value.
 */
void RTC_SetTimeDate(RTC_TimeTypeDef time);

/**
 * @brief Set the alarm of RTC.
 * @param[in] time the structure of time and date.
 */
void RTC_SetAlarm(RTC_TimeTypeDef time);

/**
 * @brief Set the periodical tick interrupt.
 * @param[in] RTC_Period Can be RTC_PERIOD_1_sec \n
 * RTC_PERIOD_1_sec_2 \n
 * RTC_PERIOD_1_sec_4 \n
 * RTC_PERIOD_1_sec_8 \n 
 * RTC_PERIOD_1_sec_16 \n
 * RTC_PERIOD_1_sec_32 \n
 * RTC_PERIOD_1_sec_64 \n
 * RTC_PERIOD_1_sec_128 \n
 * RTC_PERIOD_1_min.
 */
void RTC_SetSecINT(uint16_t RTC_Period);

/**
 * @brief Clears the RTC's interrupt pending bits.
 * @param[in] RTC_IT specifies the interrupt pending bit to clear.
 */
void RTC_ClearITPendingBit(uint16_t RTC_IT);

/**
 * @brief Clears the tamper's interrupt pending bits.
 */
void BKP_TamperClearIT(void);

/**
 * @brief Software reset to reset BBU Core.
 */
void BKP_SwRst(void);

/**
 * @brief Initialize tamper pin.
 */
void BKP_TamperInit(void);

/**
 * @brief Enables or disables the Tamper Pin activation.
 * @param[in] NewState  new state of the Tamper Pin activation.
 * This parameter can be: ENABLE or DISABLE.
 */
void BKP_TamperPinCmd(FunctionalState NewState);

/**
 * @brief Configures the Tamper Pin active level.
 * @param[in] BKP_TamperPinLevel  specifies the Tamper Pin active level.
 * 
 */
void BKP_TamperPinLevelConfig(uint16_t BKP_TamperPinLevel);

/**
 * @brief Writes user data to the specified Data Backup Register.
 * @param[in] BKP_DR  specifies the Data Backup Register.
 * @param[in] Data 
 */
void BKP_WriteBackupRegister(uint16_t BKP_DR, uint16_t Data);

/**
 * @brief Reads data from the specified Data Backup Register.
 * @param[in] BKP_DR  specifies the Data Backup Register.
 * @return uint16_t:The content of the specified Data Backup Register.
 */
uint16_t BKP_ReadBackupRegister(uint16_t BKP_DR);

/**
 * @brief Configuration of RTC output.
 */
void BKP_RTCOutputConfig(void);

/**
 * @brief    Sets RTC Clock Calibration value.
 * @param[in] trim specifies the RTC Clock Calibration structure.
 */
void BKP_SetCalValue(RTC_TrimTypeDef trim);

/**
 * @brief Clear tampering count.
 */
void BKP_TamperClearCounter(void);

/**
 * @brief Read the tampering event counter.
 * @return uint8_t  tampering event counter.
 */
uint8_t BKP_TamperGetCounter(void);

/**
 * @brief Enable tampering detector interrupt.
 */
void BKP_TamperIrqEnable(void);

/**
 * @brief Checks whether the Tamper Pin Event flag is set or not.
 * @return ITStatus  The new state of the Tamper Pin Event flag (SET or RESET).
 */
ITStatus BKP_GetITStatus(void);

#ifdef __cplusplus
}
#endif

#endif