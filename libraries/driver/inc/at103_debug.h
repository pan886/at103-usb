/**
 * @file at103_debug.h
 * @brief This file contains all the functions prototypes for the debug library.
 *
 * @author zhangsheng (zhangsheng@zhangsheng.ic.com)
 * @version 1.0
 * @date 2022-02-27
 * @copyright Copyright (c) 2022 Icore, Inc
 */

#ifndef __AT103_DEBUG_H
#define __AT103_DEBUG_H

#define NULL             ((void *)0)
#define UART_BOOT_PORT   USART2
#define UART_DEBUG_PORT  USART2
#define UART_PARITY_NONE USART_Parity_No
#define UART_STOPBITS_1  USART_StopBits_1
#define UART_DATABITS_8  USART_WordLength_8b
#define UART_BOOT_BD     115200

#define AFIO_BASE_ADDR 0x40010000 /* AFIO */
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

void    uart_init(USART_TypeDef *port, uint8_t parity, uint8_t stop_bits, uint8_t data_bits, uint32_t bd);
void    debug(char *fmt, ...);
void    uart_putc(USART_TypeDef *port, uint8_t c);
void    uart_wait_xmit(USART_TypeDef *port);
uint8_t uart_getc(USART_TypeDef *port);
int     uart_getSingleChar(USART_TypeDef *port);
void    debug_test(void);
void    sys_io_init(void);
void    pll_init(void);

#endif