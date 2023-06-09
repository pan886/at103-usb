/**
 * @file at103_crc.h
 * @brief This file contains all the functions prototypes for the CRC firmware library. \n
 *         How to use this driver? \n
 * (+) At first, use RCC_AHBPeriphClockCmd(...) to enable the CRC clock. \n
 * (+) Then use CRC_SeedConfig(...) to config the the initial value of crc. \n
 * (+) Finally, use CRC_CalCRC(...) to get the crc value. 
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

#define CRC_Reverse     ((uint8_t)0x01) /**< CRC input reverse.*/
#define CRC_Not_Reverse ((uint8_t)0x00) /**< CRC input not reverse.*/

/**
 * @brief Resets the CRC Data register (DR).
 */
void CRC_ResetDR(void);

/**
  * @brief Computes the 32-bit CRC of a given data word(32-bit).
  * @param[in] Data data word(16-bit) to compute its CRC.
  * @return uint16_t 16-bit CRC.
  */
uint16_t CRC_CalcCRC(uint32_t Data);

/**
 * @brief Returns the current CRC value.
 * @return uint16_t 16-bit CRC.
 */
uint16_t CRC_GetCRC(void);

/**
 * @brief Clear CRC value.
 */
void CRC_ClearValue(void);

/**
 * @brief CRC input inversion.
 * @param[in] invout can be 0x1 0r 0x0;
 */
void CRC_InputControl(uint8_t invout);

/**
 * @brief Config CRC seed value. 
 * @param[in] value  The initial value of crc before CRC computation.
 */
void CRC_SeedConfig(uint16_t value);

/**
 * @brief  Computes the 16-bit CRC of a given buffer of data word(32-bit).
 * @param[in] pBuffer Pointer to the buffer containing the data to be computed.
 * @param[in] BufferLength Length of the buffer to be computed.		
 * @return uint16_t 16-bit CRC.
 */
uint16_t CRC_CalcBlockCRC(uint32_t pBuffer[], uint32_t BufferLength);

#ifdef __cplusplus
}
#endif

#endif