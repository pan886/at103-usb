/**
 * @file at103.h
 * @brief Device Peripheral Access Layer Header File.
 *        This file contains all the peripheral register's definitions, bits definitions for AT103 
 *        The file is the unique include file that the application programmer is using in the C source code, usually in main.c. 
 * @author Dong Qin (dong.qin@timesintelli.com)
 * @version 1.0
 * @date 2022-01-07
 * @copyright Copyright (c) 2022 Timesintelli, Inc
 */

#ifndef __AT103_H
#define __AT103_H

#ifdef __cplusplus
extern "C" {
#endif

#include "at103_memmap.h"

typedef enum {
    RESET = 0,
    SET   = !RESET,
} FlagStatus;
typedef FlagStatus ITStatus;

typedef enum {
    DISABLE = 0,
    ENABLE  = !DISABLE,
} FunctionalState;

#define IS_FUNCTIONAL_STATE(STATE) (((STATE) == DISABLE) || ((STATE) == ENABLE))

typedef enum {
    ERROR   = 0,
    SUCCESS = !ERROR,
} ErrorStatus;

/**
 * @brief nop delay.
 * @param[in] time delay time.
 */
static void NopDelay(uint32_t time)
{
    for (volatile uint32_t i = 0; i < time; i++) {
        __NOP();
    }
}

/**
 * @brief Instruction Cache Peripheral Interface.
 */
typedef struct
{
    union {
        struct {
            __OM uint32_t EN_BIT : 1;    /**< Instruction cache enable/disable bit */
            __OM          uint32_t : 31; /**< Reserved */
        };
    } ENABLE;
    union {
        struct {
            __OM uint32_t FLUSH_BIT : 1; /**< Instruction cache flush bit */
            __OM          uint32_t : 31; /**< Reserved */
        };
    } FLUSH;
    union {
        struct {
            __OM uint32_t EN_BIT : 1;    /**< Instruction cache performance counter enable bit */
            __OM          uint32_t : 31; /**< Reserved */
        };
    } PREF_CNT_EN;
    union {
        struct {
            __OM uint32_t CLR_BIT : 1;   /**< Instruction cache performance counter clear bit */
            __OM          uint32_t : 31; /**< Reserved */
        };
    } PERF_CNT_CLR;
    union {
        struct {
            __OM uint32_t REQ_EN_BIT : 1; /**< Instruction cache select flush request bit */
            __OM          uint32_t : 31;  /**< Reserved */
        };
    } SEL_FLUSH_REQ;
    __OM uint32_t SEL_FLUSH_ID; /**< ID of cachline to be flushed. */
    union {
        struct {
            __IM uint32_t IS_ENABLE_BIT : 1;     /**< Instruction cache enable/bypass status. */
            __IM uint32_t IS_FLUSH_BIT : 1;      /**< Instruction cache flush status. */
            __IM uint32_t IS_LINE_FLUSH_BIT : 1; /**< Selected cachline status. */
            __IM          uint32_t : 29;         /**< Reserved */
        };
        __IM uint32_t value;
    } STATUS;
    __IM uint32_t HIT_CNT;   /**< Number of instruction cache hit count. */
    __IM uint32_t TRANS_CNT; /**< Number of instruction request count. */
} ICACHE_TypeDef;

/**
 * @brief Interrupt Controller Interface.
 */
typedef struct
{
    __IOM uint32_t IRQ_INTEN_L;       /**< This register specifies the interrupt enable bits for lower 32 interrupt sources. */
    __IOM uint32_t IRQ_INTEN_H;       /**< This register enables the upper 32 interrupt sources. */
    __IOM uint32_t IRQ_INTMASK_L;     /**< This register masks the lower 32 interrupt sources. */
    __IOM uint32_t IRQ_INTMASK_H;     /**< This register masks the upper 32 interrupt sources. */
    __IOM uint32_t IRQ_INTFORCE_L;    /**< This register specifies the interrupt force bits for the lower 32 interrupt sources. */
    __IOM uint32_t IRQ_INTFORCE_H;    /**< This register specifies the interrupt force bits for the upper 32 interrupt sources. */
    __IM uint32_t  IRQ_RAWSTATUS_L;   /**< This register specifies the raw status of lower 32 interrupt sources. */
    __IM uint32_t  IRQ_RAWSTATUS_H;   /**< This register specifies the raw status of the upper 32 interrupt sources */
    __IM uint32_t  IRQ_STATUS_L;      /**< This register specifies the interrupt Status of the lower 32 interrupt sources. */
    __IM uint32_t  IRQ_STATUS_H;      /**< This register specifies the interrupt status of the upper 32 interrupt sources. */
    __IM uint32_t  IRQ_MASKSTATUS_L;  /**< This register specifies the interrupt mask status of the lower 32 interrupt sources. */
    __IM uint32_t  IRQ_MASKSTATUS_H;  /**< This register specifies the interrupt mask status of the upper 32 interrupt sources. */
    __IM uint32_t  IRQ_FINALSTATUS_L; /**< This register specifies the interrupt final status of the lower 32 interrupt sources. */
    __IM uint32_t  IRQ_FINALSTATUS_H; /**< This register specifies the interrupt final status of the upper 32 interrupt sources. */
    __IM uint32_t  RESERVED1[40];     /**< Reserved */
    union {
        struct {
            __IOM uint32_t IRQ_PLEVEL_BIT : 4; /**< This register specifies the IRQ system priority level. */
            __IM           uint32_t : 28;      /**< Reserved */
        };
        __IOM uint32_t value;
    } IRQ_PLEVEL;
    union {
        struct {
            __IOM uint32_t IRQ_INTERNAL_PLEVEL_BIT : 4; /**< This register specifies the internal IRQ system priority level. */
            __IM           uint32_t : 28;               /**< Reserved */
        };
        __IOM uint32_t value;
    } IRQ_INTERNAL_PLEVEL;
    __IM uint32_t  ICTL_VERSION_ID; /**< This register specifies the component version. */
    __IOM uint32_t RESERVED2;       /**< Reserved */
    union {
        struct {
            __IOM uint32_t PROIORITY_BIT : 4; /**< This register specifies the IRQ Individual Interrupt n Priority Level. */
            __IM           uint32_t : 28;     /**< Reserved */
        };
        __IOM uint32_t value;
    } IRQ_PR[64];
    __IOM uint32_t RESERVED3; /**< Reserved */
} NVIC_TypeDef;

/** @addtogroup Peripheral declaration
  * @{
  */
#define ICACHE ((ICACHE_TypeDef *)ICACHE_BASE)
#define NVIC   ((NVIC_TypeDef *)NVIC_BASE)

/** system level driver */
#include "at103_icache.h"

/** at103 sdk config */
#include "at103_conf.h"

/** peripherals driver */
#ifdef NVIC_MODULE_ENABLED
#include "at103_nvic.h"
#endif /* NVIC_MODULE_ENABLED */
#ifdef USART_MODULE_ENABLED
#include "uart.h"
#endif /* USART_MODULE_ENABLED */

#ifdef USE_FULL_ASSERT

/**
  * @brief  The assert_param macro is used for function's parameters check.
  * @param  expr: If expr is false, it calls assert_failed function which reports 
  *         the name of the source file and the source line number of the call 
  *         that failed. If expr is true, it returns no value.
  * @retval None
  */
#define assert_param(expr) ((expr) ? (void)0 : assert_failed((uint8_t *)__FILE__, __LINE__))
/* Exported functions ------------------------------------------------------- */
void assert_failed(uint8_t *file, uint32_t line);
#else
#define assert_param(expr) ((void)0)
#endif /* USE_FULL_ASSERT */

#ifdef __cplusplus
}
#endif

#endif