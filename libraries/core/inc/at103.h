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

#define HSI_VALUE ((uint32_t)8000000) /**< Value of the Internal oscillator in Hz. */
#define HSE_VALUE ((uint32_t)8000000) /**< Value of the External oscillator in Hz. */

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
            __OM uint32_t CLR_BIT : 1;   /**< Instruction cache performance counter clear bit. */
            __OM          uint32_t : 31; /**< Reserved */
        };
    } PERF_CNT_CLR;
    union {
        struct {
            __OM uint32_t REQ_EN_BIT : 1; /**< Instruction cache select flush request bit. */
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

/**
 * @brief USART Peripheral Interface.
 */
typedef struct
{
    union {
        union {
            struct
            {
                __IM uint32_t DATA : 9;      /**< Data byte received on the serial input */
                __IM          uint32_t : 23; /**< Reserved. */
            };
            __IM uint32_t value;
        } RBR;
        union {
            struct
            {
                __OM uint32_t DATA : 9;      /**< Data to be transmitted on the serial output port */
                __IM          uint32_t : 23; /**< Reserved. */
            };
            __OM uint32_t value;
        } THR;
        union {
            struct
            {
                __IOM uint32_t DIV_L : 8;     /**< Low 8 bits of a 16bit divisor. */
                __IM           uint32_t : 24; /**< Reserved. */
            };
            __IOM uint32_t value;
        } DLL;
    } BRB_THR_DLL;
    union {
        union {
            struct
            {
                __IOM uint32_t DIV_H : 8;     /**< Upper 8-bits of a 16-bit divisor. */
                __IM           uint32_t : 24; /**< Reserved. */
            };
            __IOM uint32_t value;
        } DLH;
        union {
            struct
            {
                __IOM uint32_t ERBFI : 1;     /**< Enable Received Data Available Interrupt. */
                __IOM uint32_t ETBEI : 1;     /**< Enable Transmit Holding Register Empty Interrupt. */
                __IOM uint32_t ELSI : 1;      /**< Enable Receiver Line Status Interrupt. */
                __IOM uint32_t EDSSI : 1;     /**< Enable Modem Status Interrupt. */
                __IM           uint32_t : 3;  /**< Reserved. */
                __IOM uint32_t PTIME : 1;     /**< Enable programmable THRE Interrupt. */
                __IM           uint32_t : 24; /**< Reserved. */
            };
            __IOM uint32_t value;
        } IER;
    } DLH_IER;
    union {
        union {
            struct
            {
                __IM uint32_t INTERRUPT_ID : 4; /**< Indicates the highest priority pending interrupt. */
                __IM          uint32_t : 2;     /**< Reserved. */
                __IM uint32_t FIFO_ENABLE : 2;  /**< Indicate whether the FIFOs are enabled or disabled. */
                __IM          uint32_t : 24;    /**< Reserved. */
            };
            __IM uint32_t value;
        } IIR;
        union {
            struct
            {
                __OM uint32_t FIFOE : 1;     /**< Enables/Disables the transmit (XMIT) and receive (RCVR) FIFOs. */
                __OM uint32_t RFIFOR : 1;    /**< Resets the control portion of the receive FIFO and treats the FIFO as empty. */
                __OM uint32_t XFIFOR : 1;    /**< Resets the control portion of the transmit FIFO and treats the FIFO as empty. */
                __OM uint32_t DMAM : 1;      /**< DMA Mode (or DMAM). */
                __OM uint32_t TET : 2;       /**< Select the empty threshold level. */
                __OM uint32_t RT : 2;        /**< select the trigger level in the receiver FIFO . */
                __IM          uint32_t : 24; /**< Reserved. */
            };
            __OM uint32_t value;
        } FCR;
    } IIR_FCR;
    union {
        struct
        {
            __IOM uint32_t DLS : 2;       /**< Data length Select.*/
            __IOM uint32_t STOP : 1;      /**< Number of stop bits select. */
            __IOM uint32_t PEN : 1;       /**< Parity Enable. */
            __IOM uint32_t EPS : 1;       /**< Enable/Disable parity. */
            __IOM uint32_t SP : 1;        /**< Force parity value. */
            __IOM uint32_t BREAK : 1;     /**< Break Control Bit. */
            __IOM uint32_t DLAB : 1;      /**< Divisor Latch Access Bit. */
            __IOM          uint32_t : 24; /**< Reserved. */
        };
        __IOM uint32_t value;
    } LCR;
    union {
        struct
        {
            __IOM uint32_t DTR : 1;       /**< Control the Data Terminal Ready (dtr_n) output*/
            __IOM uint32_t RTS : 1;       /**< Control the Request to Send (rts_n) output. */
            __IOM uint32_t OUT1 : 1;      /**< Control the user-designated Output1. */
            __IOM uint32_t OUT2 : 1;      /**< Control the user-designated Output2. */
            __IOM uint32_t LOOP_BACK : 1; /**< Put the UART into a diagnostic mode for test purposes. */
            __IOM uint32_t AFCE : 1;      /**< Auto Flow Control Enable. */
            __IOM uint32_t SIRE : 1;      /**< SIR Mode Enable. */
            __IM           uint32_t : 25; /**< Reserved. */
        };
        __IOM uint32_t value;
    } MCR;
    union {
        struct
        {
            __IM uint32_t DR : 1;        /**< Data Ready bit. */
            __IM uint32_t OE : 1;        /**< Indicate the occurrence of an overrun error. */
            __IM uint32_t PE : 1;        /**< Indicate the occurrence of a parity error. */
            __IM uint32_t FE : 1;        /**< Indicate the occurrence of a framing error. */
            __IM uint32_t BI : 1;        /**< Indicate the detection of a break sequence on the serial input data.*/
            __IM uint32_t THRE : 1;      /**< Transmit Holding Register Empty bit. */
            __IM uint32_t TEMT : 1;      /**< Transmitter Empty bit. */
            __IM uint32_t RFE : 1;       /**< Receiver FIFO Error bit. */
            __IM uint32_t ADDR_RCVD : 1; /**< Address Received Bit.*/
            __IM          uint32_t : 23; /**< Reserved .*/
        };
        __IM uint32_t value;
    } LSR;
    union {
        struct
        {
            __IM uint32_t DCTS : 1;      /**< Delta Clear to Send. */
            __IM uint32_t DDSR : 1;      /**< Delta Data Set Ready. */
            __IM uint32_t TERI : 1;      /**< Trailing Edge of Ring Indicator. */
            __IM uint32_t DDCD : 1;      /**< Delta Data Carrier Detect. */
            __IM uint32_t CTS : 1;       /**< Clear to Send. */
            __IM uint32_t DSR : 1;       /**< Data Set Ready. */
            __IM uint32_t RI : 1;        /**< Ring Indicator. */
            __IM uint32_t DCD : 1;       /**< Data Carrier Detect. */
            __IM          uint32_t : 24; /**< Reserved .*/
        };
        __IM uint32_t value;
    } MSR;
    union {
        struct
        {
            __IOM uint32_t DATA : 8;      /**< Temporary storage space. */
            __IM           uint32_t : 24; /**< Reserved .*/
        };
        __IOM uint32_t value;
    } SCR;
    union {
        struct
        {
            __IOM uint32_t LPDLL : 8;     /**< Low Power Divisor Latch Low Register. */
            __IM           uint32_t : 24; /**< Reserved. */
        };
        __IOM uint32_t value;
    } LPDLL;
    union {
        struct
        {
            __IOM uint32_t LPDLH : 8;     /**< Low Power Divisor Latch High Register. */
            __IM           uint32_t : 24; /**< Reserved. */
        };
        __IOM uint32_t value;
    } LPDLH;
    __IM uint32_t RESERVED1[2]; /**< Reserved. */
    union {
        struct
        {
            __IOM uint32_t DATA : 9;      /**< Shadow register for the RBR. */
            __IM           uint32_t : 23; /**< Reserved. */
        };
        __IOM uint32_t value;
    } SRBRn_STHRn[16];
    union {
        struct
        {
            __IOM uint32_t ENABLE_BIT : 1; /**< This register is use to enable a FIFO access mode for testing. */
            __IM           uint32_t : 31;  /**< Reserved. */
        };
    } FAR;
    union {
        struct
        {
            __IM uint32_t DATA : 8;      /**< Data at the top of the transmit FIFO. */
            __IM          uint32_t : 24; /**< Reserved. */
        };
        __IM uint32_t value;
    } TFR;
    union {
        struct
        {
            __OM uint32_t DATA : 8;      /**< Receive FIFO Write Data. */
            __OM uint32_t RFPE : 1;      /**< Receive FIFO Parity Error Detection Enable bit. */
            __OM uint32_t RFFE : 1;      /**< Receive FIFO Framing Error Detection Enable bit. */
            __IM          uint32_t : 22; /**< Reserved. */
        };
        __OM uint32_t value;
    } RFW;
    __IM uint32_t RESERVED2[10]; /**< Reserved. */
    union {
        struct
        {
            __IOM uint32_t ENABLE : 1;    /**< Halt Transmission enable/disabled. */
            __IM           uint32_t : 31; /**< Reserved. */
        };
    } HTX;
    union {
        struct
        {
            __IOM uint32_t SOFT_ACK : 1;  /**< DMA Software Acknowledge. */
            __IM           uint32_t : 31; /**< Reserved. */
        };
    } DMASA;
    __IM uint32_t RESERVED3[5]; /**< Reserved. */
    union {
        struct
        {
            __IOM uint32_t DIV : 4;       /**< Fractional part of divisor. */
            __IM           uint32_t : 28; /**< Reserved. */
        };
        __IOM uint32_t value;
    } DLF;
    union {
        struct
        {
            __IOM uint32_t ADDRESS : 8;   /**< Address matching register during receive mode. */
            __IM           uint32_t : 24; /**< Reserved. */
        };
        __IOM uint32_t value;
    } RAR;
    union {
        struct
        {
            __IOM uint32_t ADRESS : 8;    /**< Address matching register during transmit mode. */
            __IM           uint32_t : 24; /**< Reserved. */
        };
        __IOM uint32_t value;
    } TAR;
    union {
        struct
        {
            __IOM uint32_t DLS_E : 1;         /**< Extension for DLS,use to enable 9-bit data. */
            __IOM uint32_t ADDR_MATCH : 1;    /**< Address Match Mode. */
            __IOM uint32_t SEND_ADDR : 1;     /**< Send address control bit. */
            __IOM uint32_t TRANSMIT_MODE : 1; /**< Transmit mode control bit.  */
            __IM           uint32_t : 29;     /**< Reserved. */
        };
        __IOM uint32_t value;
    } LCR_EXT;
} USART_TypeDef;
/**
 * @brief RCC Peripheral Interface
 */
typedef struct
{
    union {
        struct
        {
            __IOM uint32_t PLL_DIV_PRE_BIT : 6;  /**< Pre-divider of the reference clock. */
            __IOM uint32_t PLL_DIV_LOOP_BIT : 8; /**< Loop-divider of PLL. */
            __IOM uint32_t PLL_DIV_OUT_BIT : 6;  /**< Output-divider of PLL. */
            __IOM uint32_t PLL_SW_VCO_BIT : 3;   /**< The VCO oscillating frequency adjust signals. */
            __IOM uint32_t PLL_SW_ICP_BIT : 2;   /**< Current of charge pump. */
            __IOM uint32_t PLL_SW_LP_BIT : 1;    /**< Low power mode enable signal. */
            __IOM uint32_t PLL_SW_LOCK_BIT : 1;  /**< The bypass signal for PLL lock detection. */
            __IOM uint32_t PLL_SW_VREG_BIT : 2;  /**< Output voltage of internal regulator. */
            __IM           uint32_t : 2;         /**< Reserved. */
            __IOM uint32_t PLL_EN_BIT : 1;       /**< Enable signal of PLL .*/
        };
        __IOM uint32_t value;
    } PLL_CONFIG;
    __IOM uint32_t RESERVED1; /**<Reserved. */
    union {
        struct
        {
            __IOM uint32_t RC8M_EN_BIT : 1;      /**< Oscillator enable control signal. */
            __IOM uint32_t RC8M_IOP_SEL_BIT : 3; /**< Inner reference voltage select. */
            __IOM uint32_t FREQ_SEL_BIT : 4;     /**< Target frequency select. */
            __IOM uint32_t CTUNE_BIT : 6;        /**< Coarse tuning control words of oscillator. */
            __IOM uint32_t FTUNE_BIT : 6;        /**< Fine tuning control words of oscillator. */
            __IM           uint32_t : 12;        /**< Reserved. */
        };
        __IOM uint32_t value;
    } RC8M_CONFIG;
    union {
        struct
        {
            __IOM uint32_t RC8M_REF_CNT_BIT : 16; /**< Target counting value of target frequency. */
            __IOM uint32_t RC8M_TRIM_EN_BIT : 1;  /**< Frequency trimming enable bit. */
            __IM           uint32_t : 15;         /**< Reserved. */
        };
        __IOM uint32_t value;
    } RC8M_TRIM_CONFIG;
    union {
        struct
        {
            __IOM uint32_t PLL_XTEPRE_BIT : 1;  /**< HSE divider for PLL entry. */
            __IOM uint32_t SWS_BIT : 2;         /**< System clock Switch. */
            __IOM uint32_t PLL_SRC_BIT : 1;     /**< PLL intput source select bit. */
            __IOM uint32_t AHB_PRESCL_BIT : 9;  /**< AHB prescl divider. */
            __IOM uint32_t APB1_PRESCL_BIT : 4; /**< APB1 prescl divider. */
            __IOM uint32_t APB2_PRESCL_BIT : 4; /**< APB2 prescl divider. */
            __IM           uint32_t : 2;        /**< Reserved. */
            __IOM uint32_t MCO_SEL_BIT : 3;     /**< MCO source select. */
            __IOM uint32_t CSSON_BIT : 1;       /**< Clock security system enable bit. */
            __IM           uint32_t : 4;        /**< Reserved. */
            __IM uint32_t  PLL_LOCK_BIT : 1;    /**< PLL lcok indicator, active high. */
        };
        __IOM uint32_t value;
    } CLK_CONFIG;
    union {
        struct
        {

            __IOM uint32_t USB_EN_BIT : 1; /**< USB clock enable bit. */
            __IOM uint32_t CRC_EN_BIT : 1; /**< CRC clock enable bit. */
            __IOM uint32_t DMA_EN_BIT : 1; /**< DMA clock enable bit. */
            __IOM uint32_t EFC_EN_BIT : 1; /**< EFC clock enable bit. */
            __IM           uint32_t : 28;  /**< Reserved.*/
        };
        __IOM uint32_t value;
    } AHBCLKEN;
    union {
        struct
        {
            __IOM uint32_t TIM2_EN_BIT : 1;   /**< TIM2 clock enable bit. */
            __IOM uint32_t TIM3_EN_BIT : 1;   /**< TIM3 clock enable bit. */
            __IOM uint32_t TIM4_EN_BIT : 1;   /**< TIM4 clock enable bit. */
            __IOM uint32_t RTC_EN_BIT : 1;    /**< RTC clock enable bit. */
            __IOM uint32_t WDT_EN_BIT : 1;    /**< WDT clock enable bit. */
            __IOM uint32_t SPI2_EN_BIT : 1;   /**< SPI2 clock enable bit .*/
            __IOM uint32_t UART2_EN_BIT : 1;  /**< UART2 clock enable bit .*/
            __IOM uint32_t UART3_EN_BIT : 1;  /**< UART3 clock enable bit .*/
            __IOM uint32_t I2C1_EN_BIT : 1;   /**< I2C1 clock enable bit. */
            __IOM uint32_t I2C2_EN_BIT : 1;   /**< I2C2 clock enable bit. */
            __IOM uint32_t CAN_EN_BIT : 1;    /**< CAN clock enable bit. */
            __IOM uint32_t PMU_EN_BIT : 1;    /**< PMU clock enable bit. */
            __IM           uint32_t : 4;      /**< Reserved. */
            __IOM uint32_t AFIO_EN_BIT : 1;   /**< AFIO clock enable bit. */
            __IOM uint32_t ITCL_EN_BIT : 1;   /**< ITCL clock enable bit. */
            __IOM uint32_t GPIOA_EN_BIT : 1;  /**< GPIOA clock enable bit. */
            __IOM uint32_t GPIOB_EN_BIT : 1;  /**< GPIOB clock enable bit. */
            __IOM uint32_t GPIOC_EN_BIT : 1;  /**< GPIOC clock enable bit. */
            __IOM uint32_t GPIOD_EN_BIT : 1;  /**< GPIOD clock enable bit. */
            __IOM uint32_t GPIOE_EN_BIT : 1;  /**< GPIOE clock enable bit. */
            __IOM uint32_t ADC1_EN_BIT : 1;   /**< ADC1 clock enable bit. */
            __IOM uint32_t ADC2_EN_BIT : 1;   /**< ADC2 clock enable bit. */
            __IOM uint32_t TIM1_EN_BIT : 1;   /**< TIM1 clock enable bit. */
            __IOM uint32_t SPI1_EN_BIT : 1;   /**< SPI1 clock enable bit. */
            __IOM uint32_t UART1_EN_BIT : 1;  /**< UART1 clock enable bit. */
            __IOM uint32_t EFC_EN_BIT : 1;    /**< EFC clock enable bit. */
            __IM           uint32_t : 1;      /**< Reserved. */
            __IOM uint32_t DEBUG_EN_BIT : 1;  /**< DEBUG clock enable bit. */
            __IOM uint32_t ICACHE_EN_BIT : 1; /**< ICACHE clock enable bit. */
        };
        __IOM uint32_t value;
    } APBCLKEN;
    union {
        struct
        {
            __IM uint32_t TRIM_DONE_BIT : 1;       /**< Frequency trimming done status. */
            __IM uint32_t TRIM_FREQ_FTUNE_BIT : 6; /**< Ftune value generated by triming block*/
            __IM uint32_t TRIM_FREQ_CTUNE_BIT : 6; /**< Ctune value generated by triming block*/
            __IM          uint32_t : 19;           /**< Reserved*/
        };
        __IOM uint32_t value;
    } RC8M_TRIM_OUT;
    __IOM uint32_t RESERVED2; /**<Reserved. */
    union {
        struct
        {
            __IOM uint32_t XTAL32M_BIT : 8;  /**< Operating current control. */
            __IOM uint32_t XTAL_BYP_BIT : 1; /**< External active crystal input selectable signal bit*/
            __IOM uint32_t XTAL_EN_BIT : 1;  /**< Crystal enable bit*/
            __IM           uint32_t : 22;    /**< Reserved. */
        };
        __IOM uint32_t value;
    } OSC_CTRL;
    union {
        struct
        {
            __IOM uint32_t UART_CLK_DIV_MAIN_BIT : 3; /**< UART clock main dividor. */
            __IOM uint32_t UART_CLK_DIV_NUM_BIT : 5;  /**< UART clock num dividor. */
            __IOM uint32_t UART_CLK_DIV_RST_BIT : 1;  /**< UART clock divider reset bit. */
            __IOM uint32_t USB_PHY_CLK_DIV_BIT : 4;   /**< USB phy dividor. */
            __IOM uint32_t ADC_CLK_DIV_BIT : 4;       /**< ADC clock dividor. */
            __IM           uint32_t : 15;             /**< Reserved. */
        };
        __IOM uint32_t value;
    } CLK_DIV_CFG;
    __IOM uint32_t RESERVED3; /**< Reserved. */
    union {
        struct
        {
            __IOM uint32_t TIM2_SRST_BIT : 1;   /**< TIM2 sreset bit. */
            __IOM uint32_t TIM3_SRST_BIT : 1;   /**< TIM3 sreset bit. */
            __IOM uint32_t TIM4_SRST_BIT : 1;   /**< TIM4 sreset bit. */
            __IOM uint32_t RTC_SRST_BIT : 1;    /**< RCC sreset bit. */
            __IOM uint32_t WDT_SRST_BIT : 1;    /**< WDT sreset bit. */
            __IOM uint32_t SPI2_SRST_BIT : 1;   /**< SPI2 sreset bit. */
            __IOM uint32_t UART2_SRST_BIT : 1;  /**< UART2 sreset bit. */
            __IOM uint32_t UART3_SRST_BIT : 1;  /**< UART3 sreset bit. */
            __IOM uint32_t I2C1_SRST_BIT : 1;   /**< I2C1 sreset bit. */
            __IOM uint32_t I2C2_SRST_BIT : 1;   /**< I2C2 sreset bit. */
            __IOM uint32_t CAN_SRST_BIT : 1;    /**< CAN sreset bit. */
            __IOM uint32_t PMU_SRST_BIT : 1;    /**< PMU sreset bit. */
            __IM           uint32_t : 4;        /**< Reserved. */
            __IOM uint32_t AFIO_SRST_BIT : 1;   /**< AFIO sreset bit. */
            __IOM uint32_t ITCL_SRST_BIT : 1;   /**< ITCL sreset bit. */
            __IOM uint32_t GPIOA_SRST_BIT : 1;  /**< GPIOA sreset bit. */
            __IOM uint32_t GPIOB_SRST_BIT : 1;  /**< GPIOB sreset bit. */
            __IOM uint32_t GPIOC_SRST_BIT : 1;  /**< GPIOC sreset bit. */
            __IOM uint32_t GPIOD_SRST_BIT : 1;  /**< GPIOD sreset bit. */
            __IOM uint32_t GPIOE_SRST_BIT : 1;  /**< GPIOE sreset bit. */
            __IOM uint32_t ADC1_SRST_BIT : 1;   /**< ADC1 sreset bit. */
            __IOM uint32_t ADC2_SRST_BIT : 1;   /**< ADC2 sreset bit. */
            __IOM uint32_t TIM1_SRST_BIT : 1;   /**< TIM1 sreset bit. */
            __IOM uint32_t SPI1_SRST_BIT : 1;   /**< SPI1 sreset bit. */
            __IOM uint32_t UART1_SRST_BIT : 1;  /**< UART1 sreset bit. */
            __IOM uint32_t EFC_SRST_BIT : 1;    /**< EFC sreset bit. */
            __IOM uint32_t RCC_SRST_BIT : 1;    /**< RCC sreset bit. */
            __IOM uint32_t DEBUG_SRST_BIT : 1;  /**< DEBUG sreset bit. */
            __IOM uint32_t ICACHE_SRST_BIT : 1; /**< ICACHE sreset bit. */
        };
        __IOM uint32_t value;
    } APB_SRST;
    union {
        struct
        {
            __IOM uint32_t USB_SRST_BIT : 1; /**< USB srst bit. */
            __IOM uint32_t CRC_SRST_BIT : 1; /**< CRC srst bit. */
            __IOM uint32_t DMA_SRST_BIT : 1; /**< DMA srst bit. */
            __IOM uint32_t EFC_SRST_BIT : 1; /**< EFC srst bit. */
            __IM           uint32_t : 28;    /**< Reserved. */
        };
        __IOM uint32_t value;
    } AHB_SRST;
    __IOM uint32_t RESERVED4[2]; /**< Reserved. */
    union {
        struct
        {
            __IM uint32_t MODE_BIT : 2;  /**< Boot0 &Boot1 pin status. */
            __IM          uint32_t : 30; /**< Reserved. */
        };
        __IOM uint32_t value;
    } BOOT_MODE;
    __IOM uint32_t GLB_SW_RESET; /**< Global Reset Register. */
    union {
        struct
        {
            __IM uint32_t STATUS : 2;    /**< Mcu reset status. */
            __IM          uint32_t : 30; /**< Reserved. */
        };
        __IM uint32_t value;
    } MCU_RESET_STATUS;
    union {
        struct
        {
            __IM uint32_t  LSI_RDY_INT_STATUS_BIT : 1;    /**< LSI (RC32K) clock ready interrupt status. */
            __IM           uint32_t : 3;                  /**< Reserved. */
            __IM uint32_t  PLL_RDY_INT_STATUS_BIT : 1;    /**< PLL output clock ready interrupt status. */
            __IM           uint32_t : 2;                  /**< Reserved. */
            __IM uint32_t  HSE_STOP_INT_STATUS_BIT : 1;   /**< HSE (OSC8M) clock stop interrupt status. */
            __IOM uint32_t LSI_RDY_INT_EN_BIT : 1;        /**< LSI (RC32K) ready intterupt enable bit. */
            __IM           uint32_t : 3;                  /**< Reserved. */
            __IOM uint32_t PLL_RDY_INT_EN_BIT : 1;        /**< PLL output clock interrupt enable bit. */
            __IM           uint32_t : 3;                  /**< Reserved. */
            __OM uint32_t  LSI_RDY_INT_STATUS_CLEAR : 1;  /**< LSI (RC32K) clock ready interrupt clear. */
            __IM           uint32_t : 3;                  /**< Reserved. */
            __IOM uint32_t PLL_RDY_INT_STATUS_CLEAR : 1;  /**< PLL output clock ready interrupt clear. */
            __IM           uint32_t : 2;                  /**< Reserved. */
            __IM uint32_t  HSE_STOP_INT_STATUS_CLEAR : 1; /**< HSE (OSC8M) clock stop interrupt clear. */
            __IOM          uint32_t : 8;                  /**< Reserved. */
        };
        __IOM uint32_t value;
    } INT;
    __IOM uint32_t RESERVED5[4]; /**<Reserved. */
    union {
        struct
        {
            __IOM uint32_t HS0_SEL_BIT : 2;  /**< DMA handshack 0 select bits. */
            __IOM uint32_t HS1_SEL_BIT : 2;  /**< DMA handshack 1 select bits. */
            __IOM uint32_t HS2_SEL_BIT : 2;  /**< DMA handshack 2 select bits. */
            __IOM uint32_t HS3_SEL_BIT : 2;  /**< DMA handshack 3 select bits. */
            __IOM uint32_t HS4_SEL_BIT : 2;  /**< DMA handshack 4 select bits. */
            __IOM uint32_t HS5_SEL_BIT : 2;  /**< DMA handshack 5 select bits. */
            __IOM uint32_t HS6_SEL_BIT : 2;  /**< DMA handshack 6 select bits. */
            __IOM uint32_t HS7_SEL_BIT : 2;  /**< DMA handshack 7 select bits. */
            __IOM uint32_t HS8_SEL_BIT : 2;  /**< DMA handshack 8 select bits. */
            __IOM uint32_t HS9_SEL_BIT : 2;  /**< DMA handshack 9 select bits. */
            __IOM uint32_t HS10_SEL_BIT : 2; /**< DMA handshack 10 select bits. */
            __IOM uint32_t HS11_SEL_BIT : 2; /**< DMA handshack 11 select bits. */
            __IOM uint32_t HS12_SEL_BIT : 2; /**< DMA handshack 12 select bits. */
            __IOM uint32_t HS13_SEL_BIT : 2; /**< DMA handshack 13 select bits. */
            __IOM uint32_t HS14_SEL_BIT : 2; /**< DMA handshack 14 select bits. */
            __IOM uint32_t HS15_SEL_BIT : 2; /**< DMA handshack 15 select bits. */
        };
        __IOM uint32_t value;
    } DMA_HS_SEL;
    union {
        struct
        {
            __IOM uint32_t HPORT_USB_BIT : 3; /**< HPROT port of USB AHB interface control bit. */
            __IM           uint32_t : 19;     /**< Reserved. */
        };
        __IOM uint32_t value;
    } HPROT_USB;
    union {
        struct
        {
            __IOM uint32_t DATA_IN_BIT : 8;  /**< Register Configuration Interface Input data. */
            __IOM uint32_t READ_EN_BIT : 1;  /**< Register Configuration Interface Read enable, Rising edge active. */
            __IOM uint32_t REG_ADDR_BIT : 6; /**< Register Configuration Interface Input/output operation address. */
            __IOM uint32_t RSTN_BIT : 1;     /**< Register Configuration Interface Reset signal, low activate. */
            __IOM uint32_t WRITE_EN_BIT : 1; /**< Register Configuration Interface Write enable, Rising edge active. */
            __IM uint32_t  DATA_OUT_BIT : 8; /**< Register Configuration Interface Output data. */
            __IM           uint32_t : 7;     /**< Reserved. */
        };
        __IOM uint32_t value;
    } USBPHY_CFG;
    union {
        struct
        {
            __IOM uint32_t CLK_MODE_BIT : 1;   /**< USB reference clock bit. */
            __IOM uint32_t PLL_EN_BIT : 1;     /**< USB Pll enable bit. */
            __IOM uint32_t VBUSVAILD_BIT : 1;  /**< Control the input  vbusvalid of USB controller. */
            __IOM uint32_t AVALID_BIT : 1;     /**< Control the input  avalid of USB controller. */
            __IOM uint32_t SESSEND_BIT : 1;    /**< Control the input  sessend of USB controller. */
            __IM           uint32_t : 1;       /**< Reserved. */
            __IOM uint32_t IDDIG_BIT : 1;      /**< Control the input  IDDIG of USB controller. */
            __IOM uint32_t REFCLK_SEL_BIT : 1; /**< Select divided PLL clock. */
            __IM           uint32_t : 24;      /**< Reserved. */
        };
        __IOM uint32_t value;
    } USBPHY_CTRL;
    __IOM uint32_t RESERVED6[8]; /**< Reserved. */
} RCC_TypeDef;

/**
 * @brief GPIO Peripheral Interface
 */
typedef struct
{
    __IOM uint32_t DATA; /**< Data of Input/Output register. */
    __IOM uint32_t DIR;  /**< Direction of data flow register. */
    __IOM uint32_t CTRL; /**< Control mode select register. */
    __IOM uint32_t EXT;  /**< Port data extend register. */
    __IOM uint32_t IEN;  /**< Interrupt enable register. */
    __IOM uint32_t IS;   /**< Interrupt sense register. */
    __IOM uint32_t IBE;  /**< Interrupt both edge register. */
    __IOM uint32_t IEV;  /**< Interrupt event register. */
    __IM uint32_t  RIS;  /**< Raw interrupt status register. */
    __IOM uint32_t IM;   /**< Interrupt mask register.  */
    __IOM uint32_t MIS;  /**< Masked interrupt status register. */
    __OM uint32_t  IC;   /**< Interrupt clear register. */
    __IOM uint32_t DB;   /**< Delete Bounce flag. */
    __IOM uint32_t DFG;  /**< Define Filter-ed Glitch. */
    __IOM uint32_t IG;   /**< Specify Interrupt Group. */
} GPIO_TypeDef;

/**
 * @brief AFIO Peripheral Interface
 */
typedef struct
{
    union {
        struct
        {
            __IOM uint32_t SPI1_REMAP : 1;
            __IOM uint32_t I2C1_REMAP : 1;
            __IOM uint32_t USART1_REMAP : 1;
            __IOM uint32_t USART2_REMAP : 1;
            __IOM uint32_t USART3_REMAP : 2;
            __IOM uint32_t TIM1_REMAP : 2;
            __IOM uint32_t TIM2_REMAP : 2;
            __IOM uint32_t TIM3_REMAP : 2;
            __IOM uint32_t TIM4_REMAP : 1;
            __IOM uint32_t CAN_REMAP : 2;
            __IOM uint32_t PD01_REMAP : 1;
            __IOM uint32_t SPI2_REMAP : 1;
            __IOM          uint32_t : 15;
        };
        __IOM uint32_t value;
    } PIN_REMAP_CTRL;

    struct {
        __IOM uint32_t OD; /** <Open-Drain output mode control of GPIOA ports. */
        __IOM uint32_t PC; /**< Pull resistor selection signal for GPIO ports.*/
        __IOM uint32_t IC; /**< Input mode controlling signal */
        __IOM uint32_t DR; /**< bit[2n+1:2n]: Driving strength select for GPIO bit[n]. */
    } GPIO[5];
    union {
        struct
        {
            __IOM uint32_t OSCOUT : 5;    /**< IO configuration for OSC_OUT pin as GPIO port. */
            __IOM uint32_t OSCIN : 5;     /**< IO configuration for OSC_IN pin as GPIO port. */
            __IOM          uint32_t : 22; /**< Reserved. */
        };
        __IOM uint32_t value;
    } OSC_GPIO_CTRL;
    __IOM uint32_t RESERVED1[6];     /**< Reserved. */
    __IOM uint32_t EXTICR[2];        /**< bit[4n+3:4n] EXTI 15-0 select. */
    __IOM uint32_t RESERVED2[2];     /**< Reserved. */
    __IOM uint32_t GPIO_FUNC_SEL[5]; /**< bit[2n+1:2n]:  function selection for GPIOA. */
    union {
        struct
        {
            __IOM uint32_t IREN : 3;      /**< USART IO function select. */
            __IOM          uint32_t : 29; /**< Reserved. */
        };
        __IOM uint32_t value;
    } USART_IREN;
} AFIO_TypeDef;

typedef struct
{

    __IOM uint32_t LDR;

    union {
        struct
        {
            __OM uint32_t COUNTERRESTART : 8; /**< This register is used to restart the WDT counter. As a safety feature to prevent accidental restarts.*/
            __OM          uint32_t : 24;
        };
        __OM uint32_t value;
    } CRR;

    union {
        struct
        {
            __IOM uint32_t PSC_DIV_LOW : 4;  /**< The bits (3:0) further divide 4 higher bits (7:4) divided-clock by following prescale divisors .*/
            __IOM uint32_t PSC_DIV_HIGH : 4; /**< 4 Higher Bits of the Prescale Divisor The 4 bits (7:4) divide the system clock by following prescale divisors. */
            __IOM uint32_t ACTION : 1;
            __IOM uint32_t ENANBLE : 1;
            __IOM uint32_t TIMEOUT_RANGE : 3;
        };
        __IOM uint32_t value;
    } TCR;

    union {

        struct
        {
            __IOM uint32_t LENGTH : 16; /**< Timer Reset Pulse Length Register. */
            __IOM          uint32_t : 16;
        };
        __IOM uint32_t value;
    } PLR;

    __IM uint32_t CNT; /**< Current value of the timer counter. */

    union {
        struct
        {

            __IOM uint32_t INTSTAT : 1; /**< interrupt status of the WDT.*/
            __IOM          uint32_t : 31;
        };
        __IOM uint32_t value;
    } ISR;
} WDT_TypeDef;

/**
 * @brief External Interrupt/Event Controller
 */
typedef struct {
    __IOM uint32_t EMR;         /**< EXTEvent mask. */
    __IOM uint32_t RESERVED[2]; /**< Reserved. */
    __IOM uint32_t IMR;         /**< EXT Interruprt mask */
    __IOM uint32_t PR;          /**< Interruprt status.*/
} EXTI_TypeDef;

/** Peripheral declaration */
#define ICACHE ((ICACHE_TypeDef *)ICACHE_BASE)
#define RCC    ((RCC_TypeDef *)RCC_BASE)
#define NVIC   ((NVIC_TypeDef *)NVIC_BASE)
#define AFIO   ((AFIO_TypeDef *)AFIO_BASE)
#define EXTI   ((EXTI_TypeDef *)EXTI_BASE)
#define GPIOA  ((GPIO_TypeDef *)GPIOA_BASE)
#define GPIOB  ((GPIO_TypeDef *)GPIOB_BASE)
#define GPIOC  ((GPIO_TypeDef *)GPIOC_BASE)
#define GPIOD  ((GPIO_TypeDef *)GPIOD_BASE)
#define GPIOE  ((GPIO_TypeDef *)GPIOE_BASE)
#define USART1 ((USART_TypeDef *)USART1_BASE)
#define USART2 ((USART_TypeDef *)USART2_BASE)
#define USART3 ((USART_TypeDef *)USART3_BASE)
#define WWDG   ((WDT_TypeDef *)WWDG_BASE)

/** system level driver */
#include "at103_icache.h"

/** at103 sdk config */
#include "at103_conf.h"

/** peripherals driver */
#ifdef NVIC_MODULE_ENABLED
#include "at103_nvic.h"
#endif /* NVIC_MODULE_ENABLED */

#ifdef USART_MODULE_ENABLED
#include "at103_debug.h"
#include "at103_uart.h"
#endif /* USART_MODULE_ENABLED */

#ifdef RCC_MODULE_ENABLED
#include "at103_rcc.h"
#endif /* RCC_MODULE_ENABLED */

#ifdef EXTI_MODULE_ENABLED
#include "at103_exti.h"
#endif /* EXTI_MODULE_ENABLED */

#ifdef GPIO_MODULE_ENABLED
#include "at103_gpio.h"
#endif /* GPIO_MODULE_ENABLED */

#ifdef WDT_MODULE_ENABLED
#include "at103_wdt.h"
#endif
/**
 * @brief  The assert_param macro is used for function's parameters check.
 * @param  expr: If expr is false, it calls assert_failed function which reports
 *         the name of the source file and the source line number of the call
 *         that failed. If expr is true, it returns no value.
 * @retval None
 */
#ifdef USE_FULL_ASSERT
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