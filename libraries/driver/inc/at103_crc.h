/**
 * @file at103_crc.h
 * @brief This file contains all the functions prototypes for the CRC firmware library.
 * @author pantianwen (pantianwen@163.com)
 * @version 1.0
 * @date 2022-05-17
 * @copyright Copyright (c) 2022 Icore, Inc
 */
#ifndef AT103_CRC_H
#define AT103_CRC_H

#include "at103.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Resets the CRC Data register (DR).
 */
void CRC_ResetDR(void);

/**
  * @brief Computes the 32-bit CRC of a given data word(32-bit).
  * @param[in] Data data word(16-bit) to compute its CRC.
  * @return uint16_t  
  */
uint16_t CRC_CalcCRC(uint32_t Data);

/**
 * @brief Returns the current CRC value.
 * @return uint16_t  :16-bit CRC
 */
uint16_t CRC_GetCRC(void);

/**
 * @brief clear CRC value.
 */
void CRC_Clear_Value(void);

/**
 * @brief CRC input inversion.
 */
void CRC_Input_Inversion(void);

/**
 * @brief config CRC seed value. 
 * @param[in] value
 */
void CRC_Seed_Config(uint16_t value);

#endif