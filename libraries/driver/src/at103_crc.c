/**
 * @file at103_crc.c
 * @brief This file provides all the CRC firmware functions.
 * @author pantianwen (pantianwen@163.com)
 * @version 1.0
 * @date 2022-05-17
 * @copyright Copyright (c) 2022 Icore, Inc
 */

#include "at103_crc.h"

/**
 * @brief Resets the CRC Data register (DR).
 */
void CRC_ResetDR(void)
{
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_CRC, DISABLE);
}

/**
  * @brief Computes the 32-bit CRC of a given data word(32-bit).
  * @param[in] Data data word(16-bit) to compute its CRC.
  * @return uint16_t  
  */
uint16_t CRC_CalcCRC(uint32_t Data)
{
    CRC->CRC_IN = Data;

    return (CRC->CRC_OUT.COUT);
}

/**
 * @brief Returns the current CRC value.
 * @return uint16_t  :16-bit CRC
 */
uint16_t CRC_GetCRC(void)
{
    return (CRC->CRC_OUT.COUT);
}

/**
 * @brief clear CRC value.
 */
void CRC_Clear_Value(void)
{
    CRC->CRC_CTL.CLEAR = 1;
}

/**
 * @brief CRC input inversion.
 */
void CRC_Input_Inversion(void)
{
    CRC->CRC_CTL.INVOUT = 0x1;
}

/**
 * @brief config CRC seed value. 
 * @param[in] value
 */
void CRC_Seed_Config(uint16_t value)
{
    CRC->CRCSEED.SEED = value;
}