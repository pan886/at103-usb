/**
 * @file at103_flash.c
 * @brief This file provides all the FLASH firmware functions.
 * @author Dong Qin (dong.qin@timesintelli.com)
 * @version 1.0
 * @date 2022-05-13
 * @copyright Copyright (c) 2022 Timesintelli, Inc
 */

#include "at103_flash.h"
#ifdef FLASH_MODULE_ENABLED

#define FlashOperationTimeout ((uint32_t)0x000B0000)

// ns, these parameters have more margin, if you need faster Flash access, you can update it according to umc flash spec
#define TimeRC               ((uint32_t)42)
#define TimeRW               ((uint32_t)125)
#define TimePGH              ((uint32_t)26)
#define TimePROG             ((uint32_t)9000)
#define TimePGS              ((uint32_t)25000)
#define TimeNVS              ((uint32_t)6000)
#define TimeRCV_PROG         ((uint32_t)6000)
#define TimeRCV_ERASE_SECTOR ((uint32_t)70000)
#define TimeRCV_ERASE_CHIP   ((uint32_t)70000)
#define TimeCEC              ((uint32_t)12000000)
#define TimeSEC              ((uint32_t)5000000)

#define IS_FLASH_PREFETCHBUFFER_STATE(STATE) (((STATE) == FLASH_PrefetchBuffer_Enable) || \
                                              ((STATE) == FLASH_PrefetchBuffer_Disable))

#define IS_FLASH_ADDRESS(ADDRESS) (((ADDRESS) >= FLASH_BASE) && ((ADDRESS) < FLASH_BASE + (uint32_t)0x40400))

#define IS_FLASH_CLEAR_FLAG(FLAG) (((FLAG) == FLASH_FLAG_BUS_ERR) || ((FLAG) == FLASH_FLAG_UNAL_ERR))
#define IS_FLASH_GET_FLAG(FLAG)   (((FLAG) == FLASH_FLAG_BUS_ERR) || ((FLAG) == FLASH_FLAG_UNAL_ERR))

#define IS_FLASH_WRPROT_PAGE(PAGE) (((PAGE) < (uint32_t)0x100) && (((PAGE) & (uint32_t)0xFF) != (uint32_t)0))

void FLASH_UpdateLatency(void)
{
    uint32_t sys_clk      = 0;
    uint32_t ahb_clk      = 0;
    uint32_t pll_div_pre  = 0;
    uint32_t pllsource    = 0;
    uint32_t pll_div_out  = 0;
    uint32_t pll_div_loop = 0;
    uint32_t presc        = 0;
    uint32_t ahb_tick     = 0;

    switch (RCC->CLK_CONFIG.SWS_BIT) {
    case 0x00: /* HSI used as system clock */
        sys_clk = HSI_VALUE;
        break;
    case 0x01: /* HSE used as system clock */
        sys_clk = HSE_VALUE;
        break;
    case 0x02: /* PLL used as system clock */
        pll_div_pre  = RCC->PLL_CONFIG.PLL_DIV_PRE_BIT;
        pll_div_out  = RCC->PLL_CONFIG.PLL_DIV_OUT_BIT;
        pll_div_loop = RCC->PLL_CONFIG.PLL_DIV_LOOP_BIT;
        pllsource    = RCC->CLK_CONFIG.PLL_SRC_BIT;
        if (0x00 == pllsource) {
            /* HSI oscillator clock divided by 2 selected as PLL clock entry */
            sys_clk = (HSI_VALUE >> 1) * (pll_div_loop / (pll_div_pre * pll_div_out));
        } else {
            if (RCC->CLK_CONFIG.PLL_XTEPRE_BIT) {
                /* HSE oscillator clock divided by 2 selected as PLL clock entry */
                sys_clk = (HSE_VALUE >> 1) * (pll_div_loop / (pll_div_pre * pll_div_out));
            } else {
                /* HSE oscillator clock  selected as PLL clock entry */
                sys_clk = (HSE_VALUE) * (pll_div_loop / (pll_div_pre * pll_div_out));
            }
        }
        break;
    default:
        sys_clk = HSI_VALUE;
        break;
    }

    presc    = RCC->CLK_CONFIG.AHB_PRESCL_BIT;
    ahb_clk  = sys_clk / (presc + 1);
    ahb_tick = ((uint32_t)0x3B9ACA00 + (ahb_clk >> 1)) / ahb_clk;

    FLASH->CFG0.value = ((((ahb_tick >> 1) + TimePROG) / ahb_tick) << 16) |
                        ((((ahb_tick >> 1) + TimeRC) / ahb_tick) << 8) |
                        (((ahb_tick >> 1) + TimeRW) / ahb_tick);
    FLASH->CFG1.SEC   = ((ahb_tick >> 1) + TimeSEC) / ahb_tick;
    FLASH->CFG2.CEC   = ((ahb_tick >> 1) + TimeCEC) / ahb_tick;
    FLASH->CFG3.value = ((((ahb_tick >> 1) + TimePGS) / ahb_tick) << 16) |
                        (((ahb_tick >> 1) + TimePGH) / ahb_tick);
    FLASH->CFG4.value = ((((ahb_tick >> 1) + TimeRCV_PROG) / ahb_tick) << 16) |
                        (((ahb_tick >> 1) + TimeNVS) / ahb_tick);
    FLASH->CFG5.value = ((((ahb_tick >> 1) + TimeRCV_ERASE_CHIP) / ahb_tick) << 16) |
                        (((ahb_tick >> 1) + TimeRCV_ERASE_SECTOR) / ahb_tick);
}

void FLASH_PrefetchBufferCmd(uint32_t FLASH_PrefetchBuffer)
{
    assert_param(IS_FLASH_PREFETCHBUFFER_STATE(FLASH_PrefetchBuffer));
    FLASH->CTL.PREFETCH = FLASH_PrefetchBuffer;
}

FlagStatus FLASH_GetPrefetchBufferStatus(void)
{
    FlagStatus bitstatus = RESET;

    if (FLASH->CTL.PREFETCH == (uint32_t)FLASH_PrefetchBuffer_Enable) {
        bitstatus = SET;
    } else {
        bitstatus = RESET;
    }
    /* Return the new state of FLASH Prefetch Buffer Status (SET or RESET) */
    return bitstatus;
}

void FLASH_Unlock(void)
{
    FLASH->LOCK = 0x0;
}

void FLASH_Lock(void)
{
    FLASH->LOCK = 0xF;
}

FLASH_Status FLASH_ErasePage(uint32_t Page_Address)
{
    FLASH_Status status = FLASH_COMPLETE;
    /* Check the parameters */
    assert_param(IS_FLASH_ADDRESS(Page_Address));

    /* Wait for last operation to be completed */
    status = FLASH_WaitForLastOperation(FlashOperationTimeout);
    if (status == FLASH_COMPLETE) {
        FLASH->CMD.ERASE_SEC_ADDR = (Page_Address - FLASH_BASE) & 0xFFC00;
        FLASH->CMD.CODE           = 0x555;

        /* Wait for last operation to be completed */
        status = FLASH_WaitForLastOperation(FlashOperationTimeout);
    }
    /* Return the Program Status */
    return status;
}

FLASH_Status FLASH_EraseAllPages(void)
{
    FLASH_Status status = FLASH_COMPLETE;

    /* Wait for last operation to be completed */
    status = FLASH_WaitForLastOperation(FlashOperationTimeout);
    if (status == FLASH_COMPLETE) {
        FLASH->CMD.ERASE_SEC_ADDR = 0x00000;
        FLASH->CMD.CODE           = 0xAAA;
        /* Wait for last operation to be completed */
        status = FLASH_WaitForLastOperation(FlashOperationTimeout);
        if (status == FLASH_COMPLETE) {
            FLASH->CMD.ERASE_SEC_ADDR = 0x20000;
            FLASH->CMD.CODE           = 0xAAA;
            /* Wait for last operation to be completed */
            status = FLASH_WaitForLastOperation(FlashOperationTimeout);
            if (status == FLASH_COMPLETE) {
                FLASH->CMD.ERASE_SEC_ADDR = 0x40000;
                FLASH->CMD.CODE           = 0xAAA;
                /* Wait for last operation to be completed */
                status = FLASH_WaitForLastOperation(FlashOperationTimeout);
            }
        }
    }
    /* Return the Program Status */
    return status;
}

FLASH_Status FLASH_ProgramDoubleWord(uint32_t Address, uint64_t Data)
{
    FLASH_Status status = FLASH_COMPLETE;

    /* Wait for last operation to be completed */
    status = FLASH_WaitForLastOperation(FlashOperationTimeout);

    if (status == FLASH_COMPLETE) {
        if (Address & 0x7) {
            status = FLASH_ERROR_UNAL;
        } else {
            *(__IO uint64_t *)Address = Data;
            /* Wait for last operation to be completed */
            status = FLASH_WaitForLastOperation(FlashOperationTimeout);
        }
    }

    /* Return the Program Status */
    return status;
}

FLASH_Status FLASH_EnableWriteProtection(uint32_t FLASH_Pages)
{
    FLASH_Status status = FLASH_COMPLETE;

    /* Check the parameters */
    assert_param(IS_FLASH_WRPROT_PAGE(FLASH_Pages));

    /* Wait for last operation to be completed */
    status = FLASH_WaitForLastOperation(FlashOperationTimeout);
    if (status == FLASH_COMPLETE) {
        if (FLASH_Pages & FLASH_WRProt_Pages0to31) {
            FLASH->SEC_P0 = 0xFFFFFFFF;
        }
        if (FLASH_Pages & FLASH_WRProt_Pages32to63) {
            FLASH->SEC_P1 = 0xFFFFFFFF;
        }
        if (FLASH_Pages & FLASH_WRProt_Pages64to95) {
            FLASH->SEC_P2 = 0xFFFFFFFF;
        }
        if (FLASH_Pages & FLASH_WRProt_Pages96to127) {
            FLASH->SEC_P3 = 0xFFFFFFFF;
        }
        if (FLASH_Pages & FLASH_WRProt_Pages128to159) {
            FLASH->SEC_P4 = 0xFFFFFFFF;
        }
        if (FLASH_Pages & FLASH_WRProt_Pages160to191) {
            FLASH->SEC_P5 = 0xFFFFFFFF;
        }
        if (FLASH_Pages & FLASH_WRProt_Pages192to223) {
            FLASH->SEC_P6 = 0xFFFFFFFF;
        }
        if (FLASH_Pages & FLASH_WRProt_Pages224to255) {
            FLASH->SEC_P7 = 0xFFFFFFFF;
        }
    }
    return status;
}

FlagStatus FLASH_GetFlagStatus(uint32_t FLASH_FLAG)
{
    FlagStatus bitstatus = RESET;

    /* Check the parameters */
    assert_param(IS_FLASH_GET_FLAG(FLASH_FLAG));

    if (FLASH_FLAG == FLASH_FLAG_BUS_ERR) {
        if (FLASH->STS0.BUSERR != (uint32_t)RESET) {
            bitstatus = SET;
        } else {
            bitstatus = RESET;
        }
    } else {
        if (FLASH->STS0.UNALERR != (uint32_t)RESET) {
            bitstatus = SET;
        } else {
            bitstatus = RESET;
        }
    }
    /* Return the new state of FLASH_FLAG (SET or RESET) */
    return bitstatus;
}

void FLASH_ClearFlag(uint32_t FLASH_FLAG)
{
    assert_param(IS_FLASH_CLEAR_FLAG(FLASH_FLAG));
    FLASH->ERR_CLR.BUSERR_CLEAR = 1;
    FLASH->ERR_CLR.BUSERR_CLEAR = 0;
}

FLASH_Status FLASH_GetStatus(void)
{
    FLASH_Status flashstatus = FLASH_COMPLETE;

    if (FLASH->STS0.BUSERR == 1) {
        flashstatus = FLASH_ERROR_BUS;
    } else {
        if (FLASH->STS0.UNALERR == 1) {
            flashstatus = FLASH_ERROR_UNAL;
        } else {
            if (FLASH->STS0.CURR_STATUS == 0x0) {
                flashstatus = FLASH_COMPLETE;
            } else {
                flashstatus = FLASH_BUSY;
            }
        }
    }
    /* Return the Flash Status */
    return flashstatus;
}

FLASH_Status FLASH_WaitForLastOperation(uint32_t Timeout)
{
    FLASH_Status status = FLASH_COMPLETE;

    /* Check for the Flash Status */
    status = FLASH_GetStatus();
    /* Wait for a Flash operation to complete or a TIMEOUT to occur */
    while ((status == FLASH_BUSY) && (Timeout != 0x00)) {
        status = FLASH_GetStatus();
        Timeout--;
    }
    if (Timeout == 0x00) {
        status = FLASH_TIMEOUT;
    }
    /* Return the operation status */
    return status;
}

#endif