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

/**
 * @brief WWDG Peripheral Interface
 */
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
            __IOM          uint32_t : 19;
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
} WWDG_TypeDef;

/**
 * @brief External Interrupt/Event Controller
 */
typedef struct {
    __IOM uint32_t EMR;         /**< EXTEvent mask. */
    __IOM uint32_t RESERVED[2]; /**< Reserved. */
    __IOM uint32_t IMR;         /**< EXT Interruprt mask */
    __IOM uint32_t PR;          /**< Interruprt status.*/
} EXTI_TypeDef;

/**
 * @brief DMA Peripheral Channels Interface
 */
typedef struct {
    union {
        struct {
            __IOM uint32_t ADDR;          /** Source Address*/
            __IOM          uint32_t : 32; /** Reserved */
        };
    } SAR; /** Channel x Source Address Register*/
    union {
        struct {
            __IOM uint32_t ADDR;          /** Destination Address*/
            __IOM          uint32_t : 32; /** Reserved */
        };
    } DAR; /** Channel x Destination Address Register*/
    union {
        struct {
            __IOM          uint32_t : 2;  /** Reserved */
            __IOM uint32_t LOC : 30;      /** Starting Address In Memory of next*/
            __IOM          uint32_t : 32; /** Reserved */
        };
        __IOM uint32_t value[2];
    } LLP; /** Channel x Linked List Pointer Register*/
    union {
        struct {
            __IOM uint32_t INT_EN : 1;         /** Interrupt Enable Bit*/
            __IOM uint32_t DST_TR_WIDTH : 3;   /** Destination Transfer Width*/
            __IOM uint32_t SRC_TR_WIDTH : 3;   /** Source Transfer Width*/
            __IOM uint32_t DINC : 2;           /** Destination Address Increment*/
            __IOM uint32_t SINC : 2;           /** Source Address Increment*/
            __IOM uint32_t DST_MSIZE : 3;      /** Destination Burst Transaction Length*/
            __IOM uint32_t SRC_MSIZE : 3;      /** Source Burst Transaction Length*/
            __IOM uint32_t SRC_GATHER_EN : 1;  /** Source gather enable bit*/
            __IOM uint32_t DST_SCATTER_EN : 1; /** Destination scatter enable bit*/
            __IOM          uint32_t : 1;       /** Reserved */
            __IOM uint32_t TT_FC : 3;          /** Transfer Type and Flow Control*/
            __IOM uint32_t DMS : 2;            /** Destination Master Select*/
            __IOM uint32_t SMS : 2;            /** Source Master Select*/
            __IOM uint32_t LLP_DST_EN : 1;     /** Destination block chaining enable bit*/
            __IOM uint32_t LLP_SRC_EN : 1;     /** Source block chaining enable bit*/
            __IOM          uint32_t : 3;       /** Reserved */
            __IOM uint32_t BLOCK_TS : 12;      /** Block Transfer Size*/
            __IOM uint32_t DONE : 1;           /** Done bit*/
            __IOM          uint32_t : 19;      /** Reserved */
        };
        __IOM uint32_t value[2];
    } CTL;                      /** Channel x Control Register*/
    __IOM uint32_t RESERVED[8]; /** Reserved */
    union {
        struct {
            __IOM          uint32_t : 5;   /** Reserved */
            __IOM uint32_t CH_PRIOR : 3;   /** Channel priority*/
            __IOM uint32_t CH_SUSP : 1;    /** Channel Suspend*/
            __IOM uint32_t FIFO_EMPTY : 1; /** Indicates if there is data left in the channel FIFO*/
            __IOM uint32_t HS_SEL_DST : 1; /** Destination Software or Hardware Handshaking Select*/
            __IOM uint32_t HS_SEL_SRC : 1; /** Source Software or Hardware Handshaking Select*/
            __IOM          uint32_t : 6;   /** Reserved */
            __IOM uint32_t DST_HS_POL : 1; /** Destination Handshaking Interface Polarity*/
            __IOM uint32_t SRC_HS_POL : 1; /** Source Handshaking Interface Polarity*/
            __IOM uint32_t MAX_ABRST : 10; /** Maximum AMBA Burst Length*/
            __IOM uint32_t RELOAD : 2;     /** Automatic Source Reload and Automatic Destination Reload*/
            __IOM uint32_t FCMODE : 1;     /** Flow Control Mode*/
            __IOM uint32_t FIFO_MODE : 1;  /** FIFO Mode Select*/
            __IOM uint32_t PROTCTL : 3;    /** Protection Control bits*/
            __IOM          uint32_t : 2;   /** Reserved */
            __IOM uint32_t SRC_PER : 4;    /** Source hardware handshaking interface*/
            __IOM uint32_t DST_PER : 4;    /** Destination hardware handshaking interface*/
            __IOM          uint32_t : 17;  /** Reserved */
        };
        __IOM uint32_t value[2];
    } CFG; /** Channel x Configuration Register*/
    union {
        struct {
            __IOM uint32_t SGI : 20;      /** Source gather interval*/
            __IOM uint32_t SGC : 12;      /** Source gather count*/
            __IOM          uint32_t : 32; /** Reserved*/
        };
        __IOM uint32_t value[2];
    } SGR; /** Channel x Source Gather Register*/
    union {
        struct {
            __IOM uint32_t DSI : 20;      /** Destination scatter interval*/
            __IOM uint32_t DSC : 12;      /** Destination scatter count*/
            __IOM          uint32_t : 32; /** Reserved*/
        };
        __IOM uint32_t value[2];
    } DSR; /** Channel x Destination Scatter Register*/
} DMA_Channel_TypeDef;

/**
 * @brief DMA Peripheral Interface
 */
typedef struct {
    union {
        struct {
            __IM uint32_t value : 8;     /** Raw Status for Tfr Interrupt*/
            __IM          uint32_t : 24; /** Reserved*/
            __IM          uint32_t : 32; /** Reserved*/
        };
    } RAW_TFR; /** Raw Status for Tfr Interrupt*/
    union {
        struct {
            __IM uint32_t value : 8;     /** Raw Status for Block Interrupt*/
            __IM          uint32_t : 24; /** Reserved*/
            __IM          uint32_t : 32; /** Reserved*/
        };
    } RAW_BLOCK; /** Raw Status for Block Interrupt*/
    union {
        struct {
            __IM uint32_t value : 8;     /** Raw Status for SrcTran Interrupt*/
            __IM          uint32_t : 24; /** Reserved*/
            __IM          uint32_t : 32; /** Reserved*/
        };
    } RAW_SRC_TRAN; /** Raw Status for SrcTran Interrupt*/
    union {
        struct {
            __IM uint32_t value : 8;     /** Raw Status for DstTran Interrupt*/
            __IM          uint32_t : 24; /** Reserved*/
            __IM          uint32_t : 32; /** Reserved*/
        };
    } RAW_DST_TRAN; /** Raw Status for DstTran Interrupt*/
    union {
        struct {
            __IM uint32_t value : 8;     /** Raw Status for Err Interrupt*/
            __IM          uint32_t : 24; /** Reserved*/
            __IM          uint32_t : 32; /** Reserved*/
        };
    } RAW_ERR; /** Raw Status for Err Interrupt*/
    union {
        struct {
            __IM uint32_t value : 8;     /** Status for Tfr Interrupt*/
            __IM          uint32_t : 24; /** Reserved*/
            __IM          uint32_t : 32; /** Reserved*/
        };
    } STATUSES_TFR; /** Status for Tfr Interrupt*/
    union {
        struct {
            __IM uint32_t value : 8;     /** Status for Block Interrupt*/
            __IM          uint32_t : 24; /** Reserved*/
            __IM          uint32_t : 32; /** Reserved*/
        };
    } STATUSES_BLOCK; /** Status for Block Interrupt*/
    union {
        struct {
            __IM uint32_t value : 8;     /** Status for SrcTran Interrupt*/
            __IM          uint32_t : 24; /** Reserved*/
            __IM          uint32_t : 32; /** Reserved*/
        };
    } STATUSES_SRC_TRAN; /** Status for SrcTran Interrupt*/
    union {
        struct {
            __IM uint32_t value : 8;     /** Status for DstTran Interrupt*/
            __IM          uint32_t : 24; /** Reserved*/
            __IM          uint32_t : 32; /** Reserved*/
        };
    } STATUSES_DST_TRAN; /** Status for DstTran Interrupt*/
    union {
        struct {
            __IM uint32_t value : 8;     /** Status for Err Interrupt*/
            __IM          uint32_t : 24; /** Reserved*/
            __IM          uint32_t : 32; /** Reserved*/
        };
    } STATUSES_ERR; /** Status for Err Interrupt*/

    union {
        struct {
            __IOM uint32_t INT_MASK : 8;    /** Mask for Tfr Interrupt*/
            __OM uint32_t  INT_MASK_WE : 8; /** Interrupt Mask Write Enable*/
            __IOM          uint32_t : 16;   /** Reserved*/
            __IOM          uint32_t : 32;   /** Reserved*/
        };
        __IOM uint32_t value[2];
    } MASK_TFR; /** Mask for Tfr Interrupt*/
    union {
        struct {
            __IOM uint32_t INT_MASK : 8;    /** Mask for Block Interrupt*/
            __OM uint32_t  INT_MASK_WE : 8; /** Interrupt Mask Write Enable*/
            __IOM          uint32_t : 16;   /** Reserved*/
            __IOM          uint32_t : 32;   /** Reserved*/
        };
        __IOM uint32_t value[2];
    } MASK_BLOCK; /** Mask for Block Interrupt*/
    union {
        struct {
            __IOM uint32_t INT_MASK : 8;    /** Mask for SrcTran Interrupt*/
            __OM uint32_t  INT_MASK_WE : 8; /** Interrupt Mask Write Enable*/
            __IOM          uint32_t : 16;   /** Reserved*/
            __IOM          uint32_t : 32;   /** Reserved*/
        };
        __IOM uint32_t value[2];
    } MASK_SRC_TRAN; /** Mask for SrcTran Interrupt*/
    union {
        struct {
            __IOM uint32_t INT_MASK : 8;    /** Mask for DstTran Interrupt*/
            __OM uint32_t  INT_MASK_WE : 8; /** Interrupt Mask Write Enable*/
            __IOM          uint32_t : 16;   /** Reserved*/
            __IOM          uint32_t : 32;   /** Reserved*/
        };
        __IOM uint32_t value[2];
    } MASK_DST_TRAN; /** Mask for DstTran Interrupt*/
    union {
        struct {
            __IOM uint32_t INT_MASK : 8;    /** Mask for Err Interrupt*/
            __OM uint32_t  INT_MASK_WE : 8; /** Interrupt Mask Write Enable*/
            __IOM          uint32_t : 16;   /** Reserved*/
            __IOM          uint32_t : 32;   /** Reserved*/
        };
        __IOM uint32_t value[2];
    } MASK_ERR; /** Mask for Err Interrupt*/

    union {
        struct {
            __OM uint32_t INT : 8;       /** Clear for Tfr Interrupt*/
            __OM          uint32_t : 24; /** Reserved*/
            __OM          uint32_t : 32; /** Reserved*/
        };
        __OM uint32_t value[2];
    } CLEAR_TFR; /** Clear for Tfr Interrupt*/
    union {
        struct {
            __OM uint32_t INT : 8;       /** Clear for Block Interrupt*/
            __OM          uint32_t : 24; /** Reserved*/
            __OM          uint32_t : 32; /** Reserved*/
        };
        __OM uint32_t value[2];
    } CLEAR_BLOCK; /** Clear for Block Interrupt*/

    union {
        struct {
            __OM uint32_t INT : 8;       /** Clear for SrcTran Interrupt*/
            __OM          uint32_t : 24; /** Reserved*/
            __OM          uint32_t : 32; /** Reserved*/
        };
        __OM uint32_t value[2];
    } CLEAR_SRC_TRAN; /** Clear for SrcTran Interrupt*/

    union {
        struct {
            __OM uint32_t INT : 8;       /** Clear for DstTran Interrupt*/
            __OM          uint32_t : 24; /** Reserved*/
            __OM          uint32_t : 32; /** Reserved*/
        };
        __OM uint32_t value[2];
    } CLEAR_DST_TRAN; /** Clear for DstTran Interrupt*/

    union {
        struct {
            __OM uint32_t INT : 8;       /** Clear for Err Interrupt*/
            __OM          uint32_t : 24; /** Reserved*/
            __OM          uint32_t : 32; /** Reserved*/
        };
        __OM uint32_t value[2];
    } CLEAR_ERR; /** Clear for Err Interrupt*/

    union {
        struct {
            __IM uint32_t TFR : 1;       /** OR of the contents of StatusTfr register*/
            __IM uint32_t BLOCK : 1;     /** OR of the contents of StatusBlock register*/
            __IM uint32_t SRCT : 1;      /** OR of the contents of StatusSrcTran register*/
            __IM uint32_t DSTT : 1;      /** OR of the contents of StatusDst register*/
            __IM uint32_t ERR : 1;       /** OR of the contents of StatusErr register*/
            __IM          uint32_t : 27; /** Reserved*/
            __IM          uint32_t : 32; /** Reserved*/
        };
        __IOM uint32_t value[2];
    } STATUS_INT; /** Status for each interrupt type*/

    union {
        struct {
            __IOM uint32_t SRC_REQ : 8;    /** Source transaction request*/
            __IM uint32_t  SRC_REQ_WE : 8; /** Source transaction request write enable*/
            __IOM          uint32_t : 16;  /** Reserved*/
            __IOM          uint32_t : 32;  /** Reserved*/
        };
        __IOM uint32_t value[2];
    } REQ_SRC_REG; /** Source Software Transaction Request Register*/

    union {
        struct {
            __IOM uint32_t DST_REQ : 8;    /** Destination transaction request*/
            __IM uint32_t  DST_REQ_WE : 8; /** Destination transaction request write enable*/
            __IOM          uint32_t : 16;  /** Reserved*/
            __IOM          uint32_t : 32;  /** Reserved*/
        };
        __IOM uint32_t value[2];
    } REQ_DST_REG; /** Destination Software Transaction Request Register*/

    union {
        struct {
            __IOM uint32_t SRC_SGLREQ : 8;    /** Source single transaction request*/
            __IM uint32_t  SRC_SGLREQ_WE : 8; /** Source single transaction request write enable*/
            __IOM          uint32_t : 16;     /** Reserved*/
            __IOM          uint32_t : 32;     /** Reserved*/
        };
        __IOM uint32_t value[2];
    } REQ_SGLSRC_REG; /** Single Source Transaction Request Register*/

    union {
        struct {
            __IOM uint32_t DST_SGLREQ : 8;    /** Destination single transaction request*/
            __IM uint32_t  DST_SGLREQ_WE : 8; /** Destination single transaction request write enable*/
            __IOM          uint32_t : 16;     /** Reserved*/
            __IOM          uint32_t : 32;     /** Reserved*/
        };
        __IOM uint32_t value[2];
    } REQ_SGLDST_REG; /** Single Destination Transaction Request Register*/

    union {
        struct {
            __IOM uint32_t SRC_LSTREQ : 8;    /** Source last transaction request*/
            __IM uint32_t  SRC_LSTREQ_WE : 8; /** Source last transaction request write enable*/
            __IOM          uint32_t : 16;     /** Reserved*/
            __IOM          uint32_t : 32;     /** Reserved*/
        };
        __IOM uint32_t value[2];
    } REQ_LSTSRC_REG; /** Last Source Transaction Request Register*/

    union {
        struct {
            __IOM uint32_t DST_LSTREQ : 8;    /** Destination last transaction request*/
            __IM uint32_t  DST_LSTREQ_WE : 8; /** Destination last transaction request write enable*/
            __IOM          uint32_t : 16;     /** Reserved*/
            __IOM          uint32_t : 32;     /** Reserved*/
        };
        __IOM uint32_t value[2];
    } REQ_LSTDST_REG; /** Last Destination Transaction Request Register*/

    union {
        struct {
            __IOM uint32_t DMA_EN : 1;    /** DW_ahb_dmac Enable bit*/
            __IOM          uint32_t : 31; /** Reserved*/
            __IOM          uint32_t : 32; /** Reserved*/
        };
        __IOM uint32_t value[2];
    } DMA_CFG_REG; /** DMA Configuration Register*/

    union {
        struct {
            __IOM uint32_t CH_EN : 8;     /** Enables/Disables the channel*/
            __OM uint32_t  CH_EN_WE : 8;  /** Channel enable write enable*/
            __IOM          uint32_t : 16; /** Reserved*/
            __IOM          uint32_t : 32; /** Reserved*/
        };
        __IOM uint32_t value[2];
    } CH_EN_REG;                   /**  DMA Channel Enable Register*/
    __IM uint32_t DMA_ID_REG[2];   /**  DMA ID Register*/
    __IM uint32_t DMA_TEST_REG[2]; /**  DMA Test Register*/
    __IM uint32_t RESERVED[5];     /** Reserved*/
    union {
        struct {
            __IM uint32_t DTW : 3;
            __IM uint32_t STW : 3;
            __IM uint32_t STAT_DST : 1;
            __IM uint32_t STAT_SRC : 1;
            __IM uint32_t DST_SCA_EN : 1;
            __IM uint32_t SRC_GAT_EN : 1;
            __IM uint32_t LOCK_EN : 1;
            __IM uint32_t MULTI_BLK_EN : 1;
            __IM uint32_t CTL_WB_EN : 1;
            __IM uint32_t HC_LLP : 1;
            __IM uint32_t FC : 2;
            __IM uint32_t MAX_MULT_SIZE : 3;
            __IM uint32_t DMS : 3;
            __IM uint32_t LMS : 3;
            __IM uint32_t SMS : 3;
            __IM uint32_t FIFO_DEPTH : 3;
            __IM          uint32_t : 1; /** Reserved*/
        };
        __IM uint32_t value;
    } DMA_CHN_PARAMS[8]; /**  DMA Channels Component Parameters */
    union {
        struct {
            __IM uint32_t CH0 : 4;
            __IM uint32_t CH1 : 4;
            __IM uint32_t CH2 : 4;
            __IM uint32_t CH3 : 4;
            __IM uint32_t CH4 : 4;
            __IM uint32_t CH5 : 4;
            __IM uint32_t CH6 : 4;
            __IM uint32_t CH7 : 4;
        };
        __IM uint32_t value;
    } DMA_CHN_MULTI_BLK_TYP_PARAMS; /**  DMA Channels Multi Block Type Component Parameters */
    union {
        struct {
            __IM uint32_t CH0 : 4;
            __IM uint32_t CH1 : 4;
            __IM uint32_t CH2 : 4;
            __IM uint32_t CH3 : 4;
            __IM uint32_t CH4 : 4;
            __IM uint32_t CH5 : 4;
            __IM uint32_t CH6 : 4;
            __IM uint32_t CH7 : 4;
        };
        __IM uint32_t value;
    } DMA_CHN_MAX_BLK_SIZE_PARAMS; /**  DMA Channels Max Block Size Component Parameters */
    union {
        struct {
            __IM uint32_t DMA_BIG_ENDIAN : 1;
            __IM uint32_t INTR_IO : 2;
            __IM uint32_t MABRST : 1;
            __IM          uint32_t : 4;
            __IM uint32_t NUM_CHANNELS : 3;
            __IM uint32_t NUM_MASTER_INT : 2;
            __IM uint32_t S_HDATA_WIDTH : 2;
            __IM uint32_t M4_HDATA_WIDTH : 2;
            __IM uint32_t M3_HDATA_WIDTH : 2;
            __IM uint32_t M2_HDATA_WIDTH : 2;
            __IM uint32_t M1_HDATA_WIDTH : 2;
            __IM uint32_t NUM_HS_INT : 5;
            __IM uint32_t ADD_ENCODED_PARAMS : 1;
            __IM uint32_t STATIC_ENDIAN_SELECT : 1;
            __IM          uint32_t : 2;
        };
        __IM uint32_t value;
    } DMA_PARAMS; /**  DMA Component Parameters */
    __IM uint32_t DMA_COMP_ID[2];
} DMA_TypeDef;

/**
 * @brief ADC Peripheral Interface
 */
typedef struct
{
    union {
        struct
        {
            __IOM uint32_t AWD : 1;   /**< Analog watchdog flag */
            __IOM uint32_t EOC : 1;   /**< End of conversion */
            __IOM uint32_t JEOC : 1;  /**< Injected channel end of conversion*/
            __IOM uint32_t JSTRT : 1; /**< Injected channel Start flag*/
            __IOM uint32_t STRT : 1;  /**< Regular channel Start flag*/
            __IOM          uint32_t : 27;
        };
        __IOM uint32_t value;

    } SR;

    union {
        struct
        {
            __IOM uint32_t AWDCH : 5;
            __IOM uint32_t EOCIE : 1;
            __IOM uint32_t AWDIE : 1;
            __IOM uint32_t JEOCIE : 1;
            __IOM uint32_t SCAN : 1;
            __IOM uint32_t AWDSGL : 1;
            __IOM uint32_t JAUTO : 1;
            __IOM uint32_t DISCEN : 1;
            __IOM uint32_t JDISCEN : 1;
            __IOM uint32_t DISCNUM : 3;
            __IOM uint32_t DUALMOD : 4;
            __IOM          uint32_t : 2;
            __IOM uint32_t JAWDEN : 1;
            __IOM uint32_t AWDEN : 1;
            __IOM          uint32_t : 8;
        };
        __IOM uint32_t value;
    } CR1;

    union {
        struct
        {
            __IOM uint32_t ADON : 1;
            __IOM uint32_t CONT : 1;
            __IOM uint32_t CAL : 1;
            __IOM uint32_t RSTCAL : 1;
            __IOM          uint32_t : 4;
            __IOM uint32_t DMA : 1;
            __IOM          uint32_t : 2;
            __IOM uint32_t ALIGN : 1;
            __IOM uint32_t JEXTSEL : 3;
            __IOM uint32_t JEXTTRIG : 1;
            __IOM          uint32_t : 1;
            __IOM uint32_t EXTSEL : 3;
            __IOM uint32_t EXTTRIG : 1;
            __IOM uint32_t JSWSTART : 1;
            __IOM uint32_t SWSTART : 1;
            __IOM uint32_t TSVREFE : 1;
            __IOM          uint32_t : 8;
        };
        __IOM uint32_t value;
    } CR2;

    union {
        __IOM uint32_t value;
        struct
        {
            __IOM uint32_t SMP10 : 3;
            __IOM uint32_t SMP11 : 3;
            __IOM uint32_t SMP12 : 3;
            __IOM uint32_t SMP13 : 3;
            __IOM uint32_t SMP14 : 3;
            __IOM uint32_t SMP15 : 3;
            __IOM uint32_t SMP16 : 3;
            __IOM uint32_t SMP17 : 3;
            __IOM          uint32_t : 8;
        };

    } SMPR1;

    union {
        struct
        {
            __IOM uint32_t SMP0 : 3;
            __IOM uint32_t SMP1 : 3;
            __IOM uint32_t SMP2 : 3;
            __IOM uint32_t SMP3 : 3;
            __IOM uint32_t SMP4 : 3;
            __IOM uint32_t SMP5 : 3;
            __IOM uint32_t SMP6 : 3;
            __IOM uint32_t SMP7 : 3;
            __IOM uint32_t SMP8 : 3;
            __IOM uint32_t SMP9 : 3;
            __IOM          uint32_t : 2;
        };
        __IOM uint32_t value;
    } SMPR2;

    union {
        struct
        {
            __IOM uint32_t JOFR : 12;
            __IOM          uint32_t : 20;
        };
        __IOM uint32_t value;
    } JOFR1;

    union {
        struct
        {
            __IOM uint32_t JOFR : 12;
            __IOM          uint32_t : 20;
        };
        __IOM uint32_t value;
    } JOFR2;

    union {
        struct
        {
            __IOM uint32_t JOFR : 12;
            __IOM          uint32_t : 20;
        };
        __IOM uint32_t value;
    } JOFR3;

    union {
        struct
        {
            __IOM uint32_t JOFR : 12;
            __IOM          uint32_t : 20;
        };
        __IOM uint32_t value;
    } JOFR4;

    union {
        struct
        {
            __IOM uint32_t HT : 12;
            __IOM          uint32_t : 20;
        };
        __IOM uint32_t value;
    } HTR;

    union {
        struct
        {
            __IOM uint32_t LT : 12;
            __IOM          uint32_t : 20;
        };
        __IOM uint32_t value;
    } LTR;

    union {
        struct
        {
            __IOM uint32_t SQ13 : 5;
            __IOM uint32_t SQ14 : 5;
            __IOM uint32_t SQ15 : 5;
            __IOM uint32_t SQ16 : 5;
            __IOM uint32_t L : 4;
            __IOM          uint32_t : 8;
        };
        __IOM uint32_t value;
    } SQR1;

    union {
        struct
        {
            __IOM uint32_t SQ7 : 5;
            __IOM uint32_t SQ8 : 5;
            __IOM uint32_t SQ9 : 5;
            __IOM uint32_t SQ10 : 5;
            __IOM uint32_t SQ11 : 5;
            __IOM uint32_t SQ12 : 5;
            __IOM          uint32_t : 2;
        };
        __IOM uint32_t value;
    } SQR2;

    union {
        struct
        {
            __IOM uint32_t SQ1 : 5;
            __IOM uint32_t SQ2 : 5;
            __IOM uint32_t SQ3 : 5;
            __IOM uint32_t SQ4 : 5;
            __IOM uint32_t SQ5 : 5;
            __IOM uint32_t SQ6 : 5;
            __IOM          uint32_t : 2;
        };
        __IOM uint32_t value;
    } SQR3;

    union {
        struct
        {
            __IOM uint32_t JSQ1 : 5;
            __IOM uint32_t JSQ2 : 5;
            __IOM uint32_t JSQ3 : 5;
            __IOM uint32_t JSQ4 : 5;
            __IOM uint32_t L : 2;
            __IOM          uint32_t : 10;
        };
        __IOM uint32_t value;

    } JSQR;

    union {
        struct
        {
            __IOM uint32_t JDATA : 16;
            __IOM          uint32_t : 16;
        };
        __IOM uint32_t value;
    } JDR1;

    union {
        struct
        {
            __IOM uint32_t JDATA : 16;
            __IOM          uint32_t : 16;
        };
        __IOM uint32_t value;
    } JDR2;

    union {
        struct
        {
            __IOM uint32_t JDATA : 16;
            __IOM          uint32_t : 16;
        };
        __IOM uint32_t value;
    } JDR3;

    union {
        struct
        {
            __IOM uint32_t JDATA : 16;
            __IOM          uint32_t : 16;
        };
        __IOM uint32_t value;
    } JDR4;

    union {
        struct
        {
            __IOM uint32_t DATA : 16;
            __IOM uint32_t ADC2DATA : 16;
        };
        __IOM uint32_t value;
    } DR;

    union {
        struct
        {
            __IOM uint32_t GCMP : 1;
            __IOM uint32_t ANA_EN : 1;
            __IOM uint32_t RSTN : 1;
            __IOM uint32_t RESERVED : 1;
            __IOM uint32_t DISH : 1;
            __IOM uint32_t OFFSET : 6;
            __IOM uint32_t RESO : 2;
            __IOM          uint32_t : 19;
        };
        __IOM uint32_t value;
    } ANA;

} ADC_TypeDef;

/**
 * @brief FLASH Controller Interface
 */
typedef struct {
    union {
        struct
        {
            __IOM          uint32_t : 2;
            __IOM uint32_t LVCTL : 1;
            __IOM uint32_t VRED0 : 1;
            __IOM uint32_t VRED1 : 1;
            __IOM uint32_t PREFETCH : 1;
            __IOM          uint32_t : 24;
        };
        __IOM uint32_t value;
    } CTL;
    union {
        struct
        {
            __IOM uint32_t TRW : 8;
            __IOM uint32_t TRC : 8;
            __IOM uint32_t TPROG : 16;
        };
        __IOM uint32_t value;
    } CFG0;
    union {
        __IOM uint32_t SEC;
    } CFG1;
    union {
        __IOM uint32_t CEC;
    } CFG2;
    union {
        struct
        {
            __IOM uint32_t TPGH : 16;
            __IOM uint32_t TPGS : 16;
        };
        __IOM uint32_t value;
    } CFG3;
    union {
        struct
        {
            __IOM uint32_t TNVS : 16;
            __IOM uint32_t TRCV_PROG : 16;
        };
        __IOM uint32_t value;
    } CFG4;
    union {
        struct
        {
            __IOM uint32_t TRCV_ERASE_SECTOR : 16;
            __IOM uint32_t TRCV_ERASE_CHIP : 16;
        };
        __IOM uint32_t value;
    } CFG5;
    union {
        struct
        {
            __IOM uint32_t TWUP : 16;
            __IOM          uint32_t : 16;
        };
        __IOM uint32_t value;
    } CFG6;
    union {
        struct
        {
            __OM uint32_t  CODE : 12;
            __IOM uint32_t ERASE_SEC_ADDR : 20;
        };
        __IOM uint32_t value;
    } CMD;
    __OM uint32_t LOCK;
    union {
        struct
        {
            __IOM uint32_t OPST : 1;
            __IOM uint32_t BUSERR : 1;
            __IOM uint32_t PRAM_LOCK : 1;
            __IOM uint32_t PROG_MAIN_LOCK : 1;
            __IOM uint32_t PROG_NVR_LOCK : 1;
            __IOM uint32_t ERASE_LOCK : 1;
            __IOM uint32_t INIT_STATUS : 1;
            __IOM uint32_t UNALERR : 1;
            __IOM uint32_t CURR_STATUS : 2;
            __IOM          uint32_t : 22;
        };
        __IOM uint32_t value;
    } STS0;
    __IOM uint32_t SEC_P0;
    __IOM uint32_t SEC_P1;
    __IOM uint32_t SEC_P2;
    __IOM uint32_t SEC_P3;
    union {
        struct
        {
            __OM uint32_t BUSERR_CLEAR : 1;
            __IOM         uint32_t : 31;
        };
        __IOM uint32_t value;
    } ERR_CLR;
    __IOM uint32_t SEC_P4;
    __IOM uint32_t SEC_P5;
    __IOM uint32_t SEC_P6;
    __IOM uint32_t SEC_P7;
} FLASH_TypeDef;

typedef struct
{

    uint32_t CRC_IN;
    uint32_t RESERVED[16380];
    union {
        struct
        {
            uint32_t COUT : 16;
            uint32_t RESERVED : 16;
        };
        __IOM uint32_t value;
    } CRC_OUT;

    union {
        struct
        {
            uint32_t INVOUT : 1;
            uint32_t CLEAR : 1;
            uint32_t RESERVED : 30;
        };
        __IOM uint32_t value;
    } CRC_CTL;

    union {
        struct
        {
            uint32_t SEED : 16;
            uint32_t RESERVED : 16;
        };
        __IOM uint32_t value;
    } CRCSEED;

} CRC_TypeDef;

/** Peripheral declaration */
#define ICACHE       ((ICACHE_TypeDef *)ICACHE_BASE)
#define RCC          ((RCC_TypeDef *)RCC_BASE)
#define NVIC         ((NVIC_TypeDef *)NVIC_BASE)
#define AFIO         ((AFIO_TypeDef *)AFIO_BASE)
#define EXTI         ((EXTI_TypeDef *)EXTI_BASE)
#define GPIOA        ((GPIO_TypeDef *)GPIOA_BASE)
#define GPIOB        ((GPIO_TypeDef *)GPIOB_BASE)
#define GPIOC        ((GPIO_TypeDef *)GPIOC_BASE)
#define GPIOD        ((GPIO_TypeDef *)GPIOD_BASE)
#define GPIOE        ((GPIO_TypeDef *)GPIOE_BASE)
#define USART1       ((USART_TypeDef *)USART1_BASE)
#define USART2       ((USART_TypeDef *)USART2_BASE)
#define USART3       ((USART_TypeDef *)USART3_BASE)
#define DMA          ((DMA_TypeDef *)DMA_BASE)
#define DMA_Channel1 ((DMA_Channel_TypeDef *)DMA_Channel1_BASE)
#define DMA_Channel2 ((DMA_Channel_TypeDef *)DMA_Channel2_BASE)
#define DMA_Channel3 ((DMA_Channel_TypeDef *)DMA_Channel3_BASE)
#define DMA_Channel4 ((DMA_Channel_TypeDef *)DMA_Channel4_BASE)
#define DMA_Channel5 ((DMA_Channel_TypeDef *)DMA_Channel5_BASE)
#define DMA_Channel6 ((DMA_Channel_TypeDef *)DMA_Channel6_BASE)
#define DMA_Channel7 ((DMA_Channel_TypeDef *)DMA_Channel7_BASE)
#define DMA_Channel8 ((DMA_Channel_TypeDef *)DMA_Channel8_BASE)
#define WWDG         ((WWDG_TypeDef *)WWDG_BASE)
#define ADC1         ((ADC_TypeDef *)ADC1_BASE)
#define ADC2         ((ADC_TypeDef *)ADC2_BASE)
#define FLASH        ((FLASH_TypeDef *)FLASH_R_BASE)
#define CRC          ((CRC_TypeDef *)CRC_BASE)
/** system level driver */
#include "at103_icache.h"

/** at103 sdk config */
#include "at103_conf.h"

/** peripherals driver */
#ifdef DMA_MODULE_ENABLED
#include "at103_dma.h"
#endif /* DMA_MODULE_ENABLED */

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

#ifdef WWDG_MODULE_ENABLED
#include "at103_wwdg.h"
#endif

#ifdef ADC_MODULE_ENABLED
#include "at103_adc.h"
#endif /* ADC_MODULE_ENABLED */

#ifdef FLASH_MODULE_ENABLED
#include "at103_flash.h"
#endif /* FLASH_MODULE_ENABLED */

#ifdef CRC_MODULE_ENABLED
#include "at103_crc.h"
#endif /* CRC_MODULE_ENABLED */

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