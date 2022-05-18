/**
 * @file at103_crc.c
 * @brief This file provides all the CRC firmware functions.
 * @author pantianwen (pantianwen@163.com)
 * @version 1.0
 * @date 2022-05-18
 * @copyright Copyright (c) 2022 Icore, Inc
 */

#include "at103_crc.h"
#ifdef CRC_MODULE_ENABLED

void CRC_ResetDR(void)
{
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_CRC, DISABLE);
}

uint16_t CRC_CalcCRC(uint32_t Data)
{
    CRC->CRC_IN = Data;

    return (CRC->CRC_OUT.COUT);
}

uint16_t CRC_GetCRC(void)
{
    return (CRC->CRC_OUT.COUT);
}

void CRC_Clear_Value(void)
{
    CRC->CRC_CTL.CLEAR = 1;
}

void CRC_Input_Inversion(void)
{
    CRC->CRC_CTL.INVOUT = 0x1;
}

void CRC_Seed_Config(uint16_t value)
{
    CRC->CRCSEED.SEED = value;
}
#endif