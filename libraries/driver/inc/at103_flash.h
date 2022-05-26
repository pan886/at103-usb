/**
 * @file at103_flash.h
 * @brief This file contains all the functions prototypes for the FLASH firmware library. \n
 *        How to use this driver? \n
 * (+) At first, use FLASH_Unlock(...) to unlock the flash. \n
 * (+) Then, use FLASH_EraseAllPages(...) to erase the entire FLASH.
 *     Use FLASH_ErasePage(...) to specified FLASH page. \n
 * (+) Use FLASH_ProgramDoubleWord(...) to programs a double word at a specified address. \n
 * (+) Of course, you can use FLASH_GetStatus(...) to get the state of the FLASH at any time. \n
 * (+) Finally, use FLASH_Lock(...) to lock the flash.
 * @author Dong Qin (dong.qin@timesintelli.com)
 * @version 1.0
 * @date 2022-05-12
 * @copyright Copyright (c) 2022 Timesintelli, Inc
 */
#ifndef __AT103_FLASH_H
#define __AT103_FLASH_H

#ifdef __cplusplus
extern "C" {
#endif

#include "at103.h"

/**
 * @brief FLASH Status.
 */
typedef enum {
    FLASH_BUSY = 1,   /**< FLASH busy. */
    FLASH_ERROR_UNAL, /**< FLASH program address unalign error. */
    FLASH_ERROR_BUS,  /**< FLASH bus transfer error. */
    FLASH_COMPLETE,   /**< FLASH idle. */
    FLASH_TIMEOUT     /**< FLASH operation time out. */
} FLASH_Status;

#define FLASH_PrefetchBuffer_Enable  ((uint32_t)0x1) /**< FLASH Prefetch Buffer Enable. */
#define FLASH_PrefetchBuffer_Disable ((uint32_t)0x0) /**< FLASH Prefetch Buffer Disable. */

#define FLASH_FLAG_BUS_ERR  ((uint32_t)0x0) /**< FLASH bus transfer error flag. */
#define FLASH_FLAG_UNAL_ERR ((uint32_t)0x1) /**< FLASH program address unalign error flag. */

#define FLASH_WRProt_Pages0to31    ((uint32_t)0x01) /**< Write protection of page 0 to 31. */
#define FLASH_WRProt_Pages32to63   ((uint32_t)0x02) /**< Write protection of page 32 to 63. */
#define FLASH_WRProt_Pages64to95   ((uint32_t)0x04) /**< Write protection of page 64 to 95. */
#define FLASH_WRProt_Pages96to127  ((uint32_t)0x08) /**< Write protection of page 96 to 127. */
#define FLASH_WRProt_Pages128to159 ((uint32_t)0x10) /**< Write protection of page 128 to 159. */
#define FLASH_WRProt_Pages160to191 ((uint32_t)0x20) /**< Write protection of page 160 to 191. */
#define FLASH_WRProt_Pages192to223 ((uint32_t)0x40) /**< Write protection of page 192 to 223. */
#define FLASH_WRProt_Pages224to255 ((uint32_t)0x80) /**< Write protection of page 224 to 255. */

/**
 * @brief Update the code latency value, if AHB frequency is changed, that it must be called to ensure correct Flash access settings.
 */
void FLASH_UpdateLatency(void);

/**
 * @brief Enables or disables the Prefetch Buffer.
 * @param[in] FLASH_PrefetchBuffer specifies the Prefetch buffer status.
 *   This parameter can be one of the following values: \n
 *     FLASH_PrefetchBuffer_Enable: FLASH Prefetch Buffer Enable; \n
 *     FLASH_PrefetchBuffer_Disable: FLASH Prefetch Buffer Disable; \n
 */
void FLASH_PrefetchBufferCmd(uint32_t FLASH_PrefetchBuffer);

/**
 * @brief Checks whether the FLASH Prefetch Buffer status is set or not.
 * @return FlagStatus FLASH Prefetch Buffer Status (SET or RESET).
 */
FlagStatus FLASH_GetPrefetchBufferStatus(void);

/**
 * @brief Unlocks the FLASH Program Erase Controller.
 */
void FLASH_Unlock(void);

/**
 * @brief Locks the FLASH Program Erase Controller.
 */
void FLASH_Lock(void);

/**
 * @brief Erases a specified FLASH page.
 * @param[in] Page_Address The page address to be erased.
 * @return FLASH_Status The returned value can be: FLASH_BUSY, FLASH_ERROR_UNAL,
 *         FLASH_ERROR_BUS, FLASH_COMPLETE or FLASH_TIMEOUT.
 */
FLASH_Status FLASH_ErasePage(uint32_t Page_Address);

/**
 * @brief Erases all FLASH pages.
 * @return FLASH_Status The returned value can be: FLASH_BUSY, FLASH_ERROR_UNAL,
 *         FLASH_ERROR_BUS, FLASH_COMPLETE or FLASH_TIMEOUT.
 */
FLASH_Status FLASH_EraseAllPages(void);

/**
 * @brief Programs a double word at a specified address.
 * @param[in] Address specifies the address to be programmed, must be 8 bytes aligned.
 * @param[in] Data specifies the uint64_t data to be programmed.
 * @return FLASH_Status The returned value can be: FLASH_BUSY, FLASH_ERROR_UNAL,
 *         FLASH_ERROR_BUS, FLASH_COMPLETE or FLASH_TIMEOUT.
 */
FLASH_Status FLASH_ProgramDoubleWord(uint32_t Address, uint64_t Data);

/**
 * @brief Write protects the desired pages.
 * @param[in] FLASH_Pages specifies the address of the pages to be write protected.
 * @return FLASH_Status The returned value can be: FLASH_BUSY, FLASH_ERROR_UNAL,
 *         FLASH_ERROR_BUS, FLASH_COMPLETE or FLASH_TIMEOUT.
 */
FLASH_Status FLASH_EnableWriteProtection(uint32_t FLASH_Pages);

/**
 * @brief Checks whether the specified FLASH flag is set or not.
 * @param[in] FLASH_FLAG specifies the FLASH flag to check.
 * @return FlagStatus FLASH Prefetch Buffer Status (SET or RESET).
 */
FlagStatus FLASH_GetFlagStatus(uint32_t FLASH_FLAG);

/**
 * @brief Clears the FLASH's pending flags.
 * @param[in] FLASH_FLAG specifies the FLASH flag to clear.
 */
void FLASH_ClearFlag(uint32_t FLASH_FLAG);

/**
 * @brief Returns the FLASH Status.
 * @return FLASH_Status The returned value can be: FLASH_BUSY, FLASH_ERROR_UNAL,
 *         FLASH_ERROR_BUS, FLASH_COMPLETE or FLASH_TIMEOUT.
 */
FLASH_Status FLASH_GetStatus(void);

/**
 * @brief Waits for a Flash operation to complete or a TIMEOUT to occur.
 * @param[in] Timeout FLASH programming Timeout
 * @return FLASH_Status The returned value can be: FLASH_BUSY, FLASH_ERROR_UNAL,
 *         FLASH_ERROR_BUS, FLASH_COMPLETE or FLASH_TIMEOUT.
 */
FLASH_Status FLASH_WaitForLastOperation(uint32_t Timeout);

#ifdef __cplusplus
}
#endif

#endif