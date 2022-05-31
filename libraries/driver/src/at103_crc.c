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

#define IS_CRC_DATA_INVOUT(INVOUT) (((INVOUT) == CRC_Reverse) || \
                                    ((ALIGN) == CRC_Not_Reverse))
void CRC_ResetDR(void)
{
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_CRC, DISABLE);
}

uint16_t CRC_CalcCRC(uint32_t Data)
{
    uint32_t input_data = 0;
    input_data          = (Data & 0xff) << 24 | ((Data & 0xff00) >> 8) << 16 | ((Data & 0xff0000) >> 16) << 8 | (Data & 0xff000000) >> 24;
    CRC->CRC_IN         = input_data;

    return (CRC->CRC_OUT.COUT);
}

uint16_t CRC_GetCRC(void)
{
    return (CRC->CRC_OUT.COUT);
}

void CRC_ClearValue(void)
{
    CRC->CRC_CTL.CLEAR = 1;
}

void CRC_InputControl(uint8_t invout)
{
    assert_param(IS_CRC_DATA_INVOUT(INVOUT));
    CRC->CRC_CTL.INVOUT = invout;
}

void CRC_SeedConfig(uint16_t value)
{
    CRC->CRCSEED.SEED = value;
}

uint16_t CRC_CalcBlockCRC(uint32_t pBuffer[], uint32_t BufferLength)
{
    uint32_t index = 0;
    uint32_t input = 0;
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_CRC, DISABLE);
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_CRC, ENABLE);
    CRC_ClearValue();

    for (index = 0; index < BufferLength; index++) {
        input = (pBuffer[index] & 0xff) << 24 | ((pBuffer[index] & 0xff00) >> 8) << 16 | ((pBuffer[index] & 0xff0000) >> 16) << 8 | (pBuffer[index] & 0xff000000) >> 24;
        ;
        CRC->CRC_IN = input;
    }
    return (CRC->CRC_OUT.COUT);
}
#endif