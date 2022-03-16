/**
 * @file at103_debug.c
 * @brief This file provides all the debug firmware functions.
 *
 * @author zhangsheng (zhangsheng@zhangsheng.ic.com)
 * @version 1.0
 * @date 2022-02-27
 * @copyright Copyright (c) 2022 Icore, Inc
 */
#include "at103_uart.h"
#include "at103_rcc.h"
#include <stdarg.h>

#define MAX_UINT_DIGITS 10 // 2^32 = 4294967296, which has 10 digits
static const uint32_t g_uart_num_to_dec[MAX_UINT_DIGITS] = {
    1, 10, 100, 1000, 10000, 100000, 1000000, 10000000, 100000000, 1000000000};
#define TRUE  1
#define FALSE 0

void uart_init(USART_TypeDef *port, uint8_t parity, uint8_t stop_bits, uint8_t data_bits, uint32_t bd)
{
    USART_InitTypeDef USART_InitStruct;
    USART_InitStruct.USART_BaudRate            = bd;
    USART_InitStruct.USART_WordLength          = data_bits;
    USART_InitStruct.USART_StopBits            = stop_bits;
    USART_InitStruct.USART_Parity              = parity;
    USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_Init(port, &USART_InitStruct);
}

void uart_putc(USART_TypeDef *port, uint8_t c)
{
    uint32_t times = 0;

    USART_SendData(port, c);

    while (USART_GetLineFlagStatus(port, USART_FLAG_LSR_THRE) == RESET) {
        if (times++ >= (1UL << 23)) {
            return;
        }
    }
}

void uart_debug_putc(USART_TypeDef *port, uint8_t c)
{
    if (c == '\n')
        uart_putc(port, '\r');
    uart_putc(port, c);
}

void uart_wait_xmit(USART_TypeDef *port)
{
    volatile uint32_t times = 0;

    while (USART_GetLineFlagStatus(port, USART_FLAG_LSR_TEMT) == RESET) {
        if (times++ >= (1UL << 23)) {
            return;
        }
    }
}

uint8_t uart_getc(USART_TypeDef *port)
{
    uint32_t times = 0;

    while (USART_GetLineFlagStatus(port, USART_FLAG_LSR_DR) == RESET) {
        if (times++ >= (1UL << 23)) {
            return 0xFF;
        }
    }

    return (USART_ReceiveData(port) & 0x7F);
}

int uart_getSingleChar(USART_TypeDef *port)
{

    while (USART_GetLineFlagStatus(port, USART_FLAG_LSR_DR) == RESET) {
        return -1;
    }

    return (USART_ReceiveData(port) & 0x7F);
}

static int uint_to_str(uint32_t Val, char *str, uint8_t len)
{
    if (len <= sizeof('\0'))
        return 0;
    if (str == NULL)
        return 0;
    uint32_t quotient;
    uint8_t  Divider    = MAX_UINT_DIGITS - 1;
    uint8_t  FirstDigit = 0;

    *str   = '0';
    str[1] = '\0';
    if (Val == 0)
        return 1;
    len--;
    do {
        quotient = Val / g_uart_num_to_dec[Divider];
        if (quotient)
            FirstDigit = TRUE;
        if (FirstDigit) {
            *str = '0' + (char)quotient;
            if ((len--) == 0) // if not enough available characters in the string then...
            {
                *str = '\0';
                return 0;
            }
            str++;
            Val %= g_uart_num_to_dec[Divider];
        }
    } while (Divider--);
    *str = '\0';
    return 1;
}

static void uart_debug_puts(char *str)
{

    // Preconditions
    if (str == NULL)
        return;

    while (*str) {
        uart_debug_putc(UART_DEBUG_PORT, *str);
        str++;
    }
    return;
}

void v_uart_debug(char *fmt, va_list args)
{
    uint32_t j;
    int8_t  *p_buf, *p_next;
    uint32_t dword, temp, ch, flag, sftBits;
    char     buffer[MAX_UINT_DIGITS + sizeof((char)'\0') + sizeof((char)'-')];
    int      len_modifier = 0;
    int      zero_fill    = 0;

    USART_TypeDef *port = UART_DEBUG_PORT;
    p_buf               = (int8_t *)fmt;

    do {
        if (*p_buf == '%') {
            zero_fill    = 0;
            len_modifier = 0;
            p_next       = p_buf + 1;
            if (*p_next == '0') {
                zero_fill = 1;
                p_next++;
                p_buf++;
            }
            if ('0' < *p_next && *p_next <= '8') {
                len_modifier = *p_next - '0';
                p_next++;
                p_buf++;
            }
            if ((*p_next == 'x') || (*p_next == 'X')) {
                int8_t hex_start = *p_next - ('x' - 'a');

                dword   = va_arg(args, unsigned int); /* p_para[num]; */
                flag    = 0;
                sftBits = 28;
                for (j = 0; j < 8; j++) {
                    temp = (dword & (0xF << sftBits)) >> sftBits;

                    if (temp >= 10)
                        ch = temp - 10 + hex_start;
                    else
                        ch = temp + '0';

                    if (temp || flag || j == 7 || len_modifier >= 8 - j || (temp == 0 && zero_fill && len_modifier >= 8 - j)) {
                        if (!flag && temp == 0 && j != 7 && !zero_fill)
                            ch = ' ';
                        else
                            flag++;
                        uart_debug_putc(port, (uint8_t)ch);
                    }

                    sftBits -= 4;
                }
            } else if ((*p_next == 'd') || (*p_next == 'D')) {
                int32_t  Val = va_arg(args, unsigned int); /* (int32_t)p_para[num]; */
                uint32_t uVal;
                if (Val < 0) {
                    uVal      = (uint32_t)(-Val);
                    buffer[0] = '-';
                    if (uint_to_str(uVal, &buffer[1], sizeof(buffer) - 1) == 1)
                        uart_debug_puts(buffer);
                } else {
                    uVal = (uint32_t)Val;
                    if (uint_to_str(uVal, buffer, sizeof(buffer)) == 1)
                        uart_debug_puts(buffer);
                }
            } else if ((*p_next == 'u') || (*p_next == 'U')) {
                uint32_t val = va_arg(args, unsigned int);
                if (uint_to_str(val, buffer, sizeof(buffer)) == 1)
                    uart_debug_puts(buffer);
            } else if ((*p_next == 's') || (*p_next == 'S')) {
                uart_debug_puts(va_arg(args, char *));
            } else if ((*p_next == 'c') || (*p_next == 'C')) {
                uart_debug_putc(port, va_arg(args, int));
            } else if (*p_next == '%') {
                uart_debug_putc(port, '%');
            } else {
                p_buf -= 2;
                uart_debug_putc(port, '%');
            }

            p_buf += 2;
            continue;
        }

        uart_debug_putc(port, (uint8_t)*p_buf);

        p_buf++;
    } while (*p_buf);
}

void debug(char *fmt, ...)
{
    va_list args;

    va_start(args, fmt);
    v_uart_debug(fmt, args);
    va_end(args);
}

void debug_test(void)
{
    debug("%%x 0 8 d 88 dd 888 ddd 8888 dddd 88888 ddddd 888888 dddddd:\n"
          "   %x %x %x %x %x %x %x %x %x %x %x %x %x\n",
          0, 0x8, 0xd, 0x88, 0xdd, 0x888, 0xddd, 0x8888, 0xdddd, 0x88888, 0xddddd, 0x888888, 0xdddddd);
    debug("%%0nx 0  8 0d   88 00dd    888 000ddd     8888 0000dddd, a8888 9dddd cc8888 abcd0123:\n"
          "     %0x %2x %02x %4x %04x %6x %06x %8x %08x, %0x %02x %2x %4x\n",
          0, 0x8, 0xd, 0x88, 0xdd, 0x888, 0xddd, 0x8888, 0xdddd,
          0xa8888, 0x9dddd, 0xcc8888, 0xabcd0123);
    debug("%%d 0 9 -99 789 -6789 56789 456789 -3456789 23456789 -123456789 1234567890:\n"
          "   %d %d %d %d %d %d %d %d %d %d %d\n",
          0, 9, -99, 789, -6789, 56789, 456789, -3456789, 23456789, -123456789, 1234567890);
    debug("%%u 0 1 2 -1 -2 -3456789:\n"
          "   %u %u %u %u %u %u\n",
          0, 1, 2, -1, -2, -3456789);
    debug("%%s hello world:\n"
          "   %s\n",
          "hello world");
    debug("%%c a b c d e g z k:\n"
          "   %c %c %c %c %c %c %c %c\n",
          'a', 'b', 'c', 'd', 'e', 'g', 'z', 'k');
    debug("mix %% %s%d%u%c%x\n", "good to know", -98765, 1234, '=', 0xc1d04000);
}

void pll_init(void)
{
    ErrorStatus PllLockStatus = ERROR;

    /* SYSCLK, HCLK, PCLK2 and PCLK1 configuration -----------------------------*/
    /* RCC system reset(for debug purpose) */
    RCC_DeInit();

    /* Enable HSE */
    RCC_HSEConfig(RCC_HSE_ON);

    /* PLLCLK = ( ( 8MHz  ) * 36 ) / ( 2 * 1 ) = 144 MHz */
    RCC_PLLConfig(RCC_PLLSource_HSE_Div1, 0x24, 0x02, 0x01);

    /* Enable PLL */
    RCC_PLLCmd(ENABLE);

    /* Wait For Pll Ready */
    PllLockStatus = RCC_WaitForPllLock();

    if (PllLockStatus != ERROR) {
        /* HCLK = SYSCLK */
        RCC_HCLKConfig(RCC_SYSCLK_Div1);

        /* PCLK2 = HCLK */
        RCC_PCLK2Config(RCC_HCLK_Div2);

        /* PCLK1 = HCLK */
        RCC_PCLK1Config(RCC_HCLK_Div2);

        /* Select PLL as system clock source */
        RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);

        RCC_UartClkConfig(2, 16);
    }
}

void sys_io_init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    /* Remap USART2 use PA2 PA3 */
    GPIO_PinRemapConfig(GPIO_FullRemap_TIM2, ENABLE);
    GPIO_PinRemapConfig(GPIO_Remap_USART2, DISABLE);

    /* Configure USART as AF mode */
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_2 | GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
}