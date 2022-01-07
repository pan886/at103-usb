/*----------------------------------------------------------------------------*/
/*                                                                            */
/*     pyright (C) 2019 TimesIntelli Co., Ltd. All rights reserved.           */
/*                                                                            */
/*----------------------------------------------------------------------------*/
#ifndef _UART_H_
#define _UART_H_

#include "uart_reg.h"
#include "at103.h"

#define BOOT_ROM_DEBUG
/*************************************************************************************************************/
// macro definition
/*************************************************************************************************************/
#define UART0 (0)
#define UART1 (1)
#define UART2 (0)
#define UART3 (3)

#ifdef CONFIG_FPGA_VERSION
#define UART_BOOT_PORT  UART2
#define UART_DEBUG_PORT UART2
#else
#define UART_BOOT_PORT  UART2
#define UART_DEBUG_PORT UART2
#endif

/* IO MUX select. offset: 0xA0 */
#define AFIO_PIN_REMAP_CTL              (AFIO_BASE_ADDR + 0x0)
#define AFIO_PIN_REMAP_CTL_UART2_MASK   1
#define AFIO_PIN_REMAP_CTL_UART2_SHIFT  3
#define AFIO_PIN_REMAP_CTL_UART2        0
#define AFIO_PIN_REMAP_CTL_TIMER2_MASK  0x3
#define AFIO_PIN_REMAP_CTL_TIMER2_SHIFT 8
#define AFIO_PIN_REMAP_CTL_TIMER2       2

#define AFIO_GPIOA_FUNC_SEL       (AFIO_BASE_ADDR + 0x80)
#define AFIO_GPIOA_SEL_BITS_MASK  0x3
#define AFIO_GPIOA_UART2_TX_SHIFT 0x4
#define AFIO_GPIOA_UART2_RX_SHIFT 0x6
#define AFIO_GPIOA_UART2_TX_SEL   0x1
#define AFIO_GPIOA_UART2_RX_SEL   0x1

#ifndef CONFIG_SIM
#define UART_BOOT_BD UART_BD_115200
#else
#define UART_BOOT_BD UART_BD_4000000
#endif

#define tgtdebug(fmt, args...) debug("[TARG]" fmt, ##args)

#define SYS_ASIC_PLL (144 * 1000 * 1000)
#define PLL_CLK      SYS_ASIC_PLL
//#define SYS_AXI_CLK		(PLL_CLK >> 1)  		/* default divider in CLK_CFG */
#define SYS_AHB_CLK  (PLL_CLK)                /* default divider in CLK_CFG */
#define SYS_APB_CLK  (PLL_CLK >> 1)           /* default divider in CLK_CFG */
#define SYS_UART_CLK ((SYS_APB_CLK * 4) / 25) /* default divider in CLK_CFG */

#define MCU_PLL_CFG              (RCC_BASE_ADDR)
#define PLL_CFG_PLL_EN_BIT_SHIFT 31
#define PLL_CFG_PLL_EN_MASK      0x1
#define PLL_CFG_PLL_EN           0x1
#define PLL_CFG_PLL_DIS          0x0

#define MCU_CLK_CFG       (RCC_BASE_ADDR + 0x10)
#define CLK_CFG_SWS_SHIFT 1
#define CLK_CFG_SWS_MASK  0x11
#define CLK_CFG_SWS_HSI   0x0
#define CLK_CFG_SWS_HSE   0x1
#define CLK_CFG_SWS_PLL   0x2

#define CLK_CFG_PLLSRC_BIT_SHIFT 3
#define CLK_CFG_PLLSRC_MASK      0x1
#define CLK_CFG_PLLSRC_HSI       0x0
#define CLK_CFG_PLLSRC_HSE       0x1

#define CLK_CFG_PLLTXPRE_BIT_SHIFT 0
#define CLK_CFG_PLLTXPR_MASK       0x1
#define CLK_CFG_PLLTXPR_HSEDIV1    0x0
#define CLK_CFG_PLLTXPR_HSEDIV2    0x1

#define CLK_PLL_LOCK_BIT_SHIFT    31
#define CLK_PLL_LOCK_MASK         0x1
#define CLK_PLL_LOCK_STATUS_READY 0x1

#define AHB_CLKEN             (RCC_BASE_ADDR + 0x14)
#define PCLK_USB_EN_BIT_SHIFT 0
#define PCLK_CRC_EN_BIT_SHIFT 1
#define PCLK_DMA_EN_BIT_SHIFT 2
#define PCLK_EFC_EN_BIT_SHIFT 3
#define AHB_CLKEN_EN          1
#define AHB_CLKEN_DIS         0

#define APB_CLKEN               (RCC_BASE_ADDR + 0x18)
#define PCLK_WDT_EN_BIT_SHIFT   4
#define PCLK_TIM1_EN_BIT_SHIFT  25
#define PCLK_DEBUG_EN_BIT_SHIFT 30
#define PCLK_UART2_EN_BIT_SHIFT 6
#define APB_CLKEN_EN            1
#define APB_CLKEN_DIS           0

#define MCU_OSC_CFG               (RCC_BASE_ADDR + 0x24)
#define OSC_CFG_XTAL_EN_BIT_SHIFT 9
#define OSC_CFG_XTAL_EN_EN        1
#define OSC_CFG_XTAL_EN_DIS       0

#define MCU_CLK_DIV_CFG       (RCC_BASE_ADDR + 0x28)
#define CLK_CFG_UARTDIV_SHIFT 0
#define CLK_CFG_UARTDIV_MASK  0xFF

/* APB Reset Control Registers */
#define MCU_APB_SRST (RCC_BASE_ADDR + 0x30)
/* offset: 0x0014, block reset */
#define UART_RESET_BIT_SHIFT  6 /* UART2 */
#define WDT_RESET_BIT_SHIFT   4
#define TIMER_RESET_BIT_SHIFT 25

/* offset: 0x10, Global Reset Control register. */
#define MCU_GLB_RST (RCC_BASE_ADDR + 0x10)

#define MCU_GLB_RST_ENABLE 0xAA55A5A5

/* offset: 0x40, RCC Boot Mode register. */
#define RCC_BOOT_MODE (RCC_BASE_ADDR + 0x40)

/* IO MUX select. offset: 0xA0 */
#define AFIO_PIN_REMAP_CTL              (AFIO_BASE_ADDR + 0x0)
#define AFIO_PIN_REMAP_CTL_UART2_MASK   1
#define AFIO_PIN_REMAP_CTL_UART2_SHIFT  3
#define AFIO_PIN_REMAP_CTL_UART2        0
#define AFIO_PIN_REMAP_CTL_TIMER2_MASK  0x3
#define AFIO_PIN_REMAP_CTL_TIMER2_SHIFT 8
#define AFIO_PIN_REMAP_CTL_TIMER2       2

#define AFIO_GPIOA_FUNC_SEL       (AFIO_BASE_ADDR + 0x80)
#define AFIO_GPIOA_SEL_BITS_MASK  0x3
#define AFIO_GPIOA_UART2_TX_SHIFT 0x4
#define AFIO_GPIOA_UART2_RX_SHIFT 0x6
#define AFIO_GPIOA_UART2_TX_SEL   0x1
#define AFIO_GPIOA_UART2_RX_SEL   0x1
/*EFLASH*/
#define EFLASH_CFG3_CTL (EFLASH_BASE_ADDR + 0x10)

/*************************************************************************************************************/
// function prototype declaration
/*************************************************************************************************************/
void    uart_init(int port, uint8_t parity, uint8_t stop_bits, uint8_t data_bits, uint32_t bd);
void    debug(char *fmt, ...);
void    uart_putc(int port, uint8_t c);
void    uart_wait_xmit(int port);
uint8_t uart_getc(int port);
//uint8_t uart_getc_waitms(uint8_t port, uint32_t ms);
int  uart_getSingleChar(uint8_t port);
void uart_reinit(uint8_t port, uint32_t bd);
void debug_test(void);
void sys_io_init(void);
void sys_apbclk_en(uint32_t periph_shift, uint8_t eable);
void sys_ahbclk_en(uint32_t periph_shift, uint8_t eable);
void pll_init(void);
#endif
