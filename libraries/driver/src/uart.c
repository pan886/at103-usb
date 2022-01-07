/*----------------------------------------------------------------------------*/
/*                                                                            */
/*     pyright (C) 2019 TimesIntelli Co., Ltd. All rights reserved.           */
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "uart.h"

/*************************************************************************************************************/
// macro definition
/*************************************************************************************************************/
#define MAX_UINT_DIGITS      10                          // 2^32 = 4294967296, which has 10 digits
#define UART_RESET_BIT_SHIFT 6                           /* UART2 */
#define WDT_LDR_REG          (WDT_CTRL_BASE_ADDR + 0x00) // R/W, 0x00000000, Watchdog Timer Load Register
#define WDT_CRR_REG          (WDT_CTRL_BASE_ADDR + 0x04) // W,    0x00000000, Watchdog Timer Counter Restart Register
#define WDT_TCR_REG          (WDT_CTRL_BASE_ADDR + 0x08) // R/W, 0x000001DE, Watchdog Timer Control Register
#define WDT_PLR_REG          (WDT_CTRL_BASE_ADDR + 0x0C) // R/W, 0x000000FF, Watchdog Timer Reset Pulse Length Register
#define WDT_CNT_REG          (WDT_CTRL_BASE_ADDR + 0x10) // R,     0xFFFFFFFF, Watchdog Timer Counter Register
#define WDT_ISR_REG          (WDT_CTRL_BASE_ADDR + 0x14) // R/W, 0x00000000, Watchdog Timer Interrupt Status Register

/*************************************************************************************************************/
// global variable definition
/*************************************************************************************************************/
static const uint32_t g_uart_num_to_dec[MAX_UINT_DIGITS] = {
    1, 10, 100, 1000, 10000, 100000, 1000000, 10000000, 100000000, 1000000000};
/*****************************************************************************/
//  Function:  void uart_init(int port, uint8_t parity, uint8_t stop_bits, uint8_t data_bits, uint32_t bd)
//
//  Parameters:
//      	port 		: uart index
//					= UART0
//
//		parity	: parity set
//					= UART_PARITY_NONE
//					= UART_PARITY_ODD
//					= UART_PARITY_EVEN
//
//		stop_bits	: stop bit set
//					= UART_STOPBITS_1
//					= UART_STOPBITS_1_5
//					= UART_STOPBITS_2
//
//		data_bits	: data bits set
//					= UART_DATABITS_5
//					= UART_DATABITS_6
//					= UART_DATABITS_7
//					= UART_DATABITS_8
//
//		bd	: baudrate
//
//  Return:
//		none
//
//  Description:
//      uart x initialize
//
/*****************************************************************************/
static uint32_t current_bd;

void sys_apbclk_en(uint32_t periph_shift, uint8_t eable)
{
    uint32_t mask = (1UL << periph_shift);
    uint32_t tmp_val;
    tmp_val = readl(APB_CLKEN);
    if (eable) {
        tmp_val |= mask;
    } else {
        tmp_val &= ~(mask);
    }
    writel(tmp_val, APB_CLKEN);
}
void sys_ahbclk_en(uint32_t periph_shift, uint8_t eable)
{
    uint32_t mask = (1UL << periph_shift);
    uint32_t tmp_val;
    tmp_val = readl(AHB_CLKEN);
    if (eable) {
        tmp_val |= mask;
    } else {
        tmp_val &= ~(mask);
    }
    writel(tmp_val, AHB_CLKEN);
}

void pll_init(void)
{
    volatile uint32_t clk_cfg_reg = 0;

    clk_cfg_reg = readl(MCU_OSC_CFG) | (OSC_CFG_XTAL_EN_EN << OSC_CFG_XTAL_EN_BIT_SHIFT); /*xtal en*/
    writel(clk_cfg_reg, MCU_OSC_CFG);                                                     /*xtal en*/

    clk_cfg_reg = readl(MCU_PLL_CFG);
    clk_cfg_reg &= ~(PLL_CFG_PLL_EN_MASK << PLL_CFG_PLL_EN_BIT_SHIFT); /* PLL disable. */
    writel(clk_cfg_reg, MCU_PLL_CFG);

    clk_cfg_reg = readl(MCU_CLK_CFG);
    clk_cfg_reg |= CLK_CFG_PLLTXPR_MASK << CLK_CFG_PLLTXPRE_BIT_SHIFT;    /*HSEDIV =2*/
    clk_cfg_reg &= ~(CLK_CFG_PLLTXPR_MASK << CLK_CFG_PLLTXPRE_BIT_SHIFT); /*HSEDIV =1*/
    writel(clk_cfg_reg, MCU_CLK_CFG);

    clk_cfg_reg = readl(MCU_PLL_CFG);
    clk_cfg_reg |= PLL_CFG_PLL_EN_MASK << PLL_CFG_PLL_EN_BIT_SHIFT; /* PLL eable. */
    writel(clk_cfg_reg, MCU_PLL_CFG);

    /* 
	 * For i103, ?M OSC is used. PLL default value after reset is 144M. 
	 *  PLL default value 144M is fine and no need to change it in BootROM.
	 */
    /* Wait for PLL to be ready. */
    while (((readl(MCU_CLK_CFG) >> CLK_PLL_LOCK_BIT_SHIFT) & CLK_PLL_LOCK_MASK) != CLK_PLL_LOCK_STATUS_READY)
        ;

    /*clk = 8M delay 100us*/
    NOP_DELAY(80);
    /* Clock source switch from OSC to PLL. Keep all the original value except the switch control bit. */
    clk_cfg_reg = readl(MCU_CLK_CFG);
    clk_cfg_reg |= CLK_CFG_SWS_PLL << CLK_CFG_SWS_SHIFT; /* PLL as input. */
    writel(clk_cfg_reg, MCU_CLK_CFG);

    sys_apbclk_en(PCLK_WDT_EN_BIT_SHIFT, 1);
    sys_apbclk_en(PCLK_TIM1_EN_BIT_SHIFT, 1);
    sys_apbclk_en(PCLK_UART2_EN_BIT_SHIFT, 1);
    sys_ahbclk_en(PCLK_CRC_EN_BIT_SHIFT, 1);

    uint32_t tmp_val;
    tmp_val = readl(WDT_TCR_REG) & (~(1 << 9));
    writel(tmp_val, WDT_TCR_REG);

    clk_cfg_reg = 0x407d0;
    writel(clk_cfg_reg, EFLASH_CFG3_CTL);
}

static void sys_rst(uint32_t periph_shit)
{
    uint32_t mask = (1UL << periph_shit);
    uint32_t tmp_val;
    // reset
    tmp_val = readl(MCU_APB_SRST) | mask;
    writel(tmp_val, MCU_APB_SRST);
    NOP_DELAY(500);
    // release
    tmp_val = readl(MCU_APB_SRST) & ~mask;
    writel(tmp_val, MCU_APB_SRST);
    /* CPU may run much faster than peripheral. Wait some CPU cycles after reset release. */
    NOP_DELAY(50);
}

void uart_init(int port, uint8_t parity, uint8_t stop_bits, uint8_t data_bits, uint32_t bd)
{
    uint32_t          uart_ip_div, uart_pll_div;
    volatile uint32_t reg_val;

    sys_rst(UART_RESET_BIT_SHIFT);

    // disable all interrupt
    writel(0, UART_IER_REG(port));

    uint32_t tmp_val;
    tmp_val = (UART_FIFO_EN << 6) | (UART_INT_ID_CH_TIMEOUT << 0);
    writel(tmp_val, UART_IIR_REG(port));

    // set Rx Trigger Level & Tx FIFO Reset & Rx FIFO Reset & FIFO Mode Enable
    writel(UART_FCR_VAL(1, 1), UART_FCR_REG(port));

    // set Parity & Data bits & Stop bits
    tmp_val = (UART_DLAB_EN << 7) | (parity << 3) | (stop_bits << 2) | data_bits;
    writel(tmp_val, UART_LCR_REG(port));

    // Set BaudRate
    uart_pll_div = 0xE4; /* 5'b11100, 3'b100. 144M /(4 +(28/32)) = 29.538M*/
    switch (bd) {        /* uart_ip_div = 29.538M / 16 / baud_rate*/
    case UART_BD_50:
        //uart_pll_div = 0xb4; /* 5'b10110, 3'b100. */
        uart_ip_div = 0x903b;
        break;
    case UART_BD_300:
        uart_ip_div = 0x180a;
        break;
    case UART_BD_1200:
        uart_ip_div = 0x0602;
        break;
    case UART_BD_4800:
        uart_ip_div = 0x0181;
        break;
    case UART_BD_9600:
        uart_ip_div = 0xc0;
        break;
    case UART_BD_19200:
        uart_ip_div = 0x60;
        break;
    case UART_BD_38400:
        uart_ip_div = 0x30;
        break;
    case UART_BD_57600:
        uart_ip_div = 0x20;
        break;
    case UART_BD_115200:
        uart_ip_div = 0x10;
        break;
    case UART_BD_230400:
        uart_ip_div = 0x08;
        break;
    case UART_BD_460800:
        uart_ip_div = 0x04;
        break;
    case UART_BD_921600:
        //uart_pll_div = 0x0a; /* 5'b00001, 3'b010. */
        uart_ip_div = 0x02;
        break;
    case UART_BD_1000000:
        uart_pll_div = 0x42; /* 5'b01000, 3'b010. */
        uart_ip_div  = 0x04;
        break;
    case UART_BD_2000000:
        uart_pll_div = 0x42; /* 5'b01000, 3'b010. */
        uart_ip_div  = 0x02;
        break;
    case UART_BD_3000000:
        uart_pll_div = 0x03; /* 5'b00000, 3'b011. */
        uart_ip_div  = 0x0001;
        break;
    case UART_BD_3500000:
        uart_pll_div = 0x92; /* 5'b10010, 3'b010. */
        uart_ip_div  = 0x01;
        break;
    case UART_BD_4000000:
        uart_pll_div = 0x42; /* 5'b01000, 3'b010. */
        uart_ip_div  = 0x01;
        break;
    default:
        uart_pll_div = 0xE4;
        uart_ip_div  = 0x0010;
        break;
    }
    reg_val = readl(MCU_CLK_DIV_CFG);
    tmp_val = (reg_val & (~CLK_CFG_UARTDIV_MASK)) | (uart_pll_div << CLK_CFG_UARTDIV_SHIFT);
    writel(tmp_val, MCU_CLK_DIV_CFG);

    writel(uart_ip_div & 0xFF, UART_DLL_REG(port));
    writel((uart_ip_div >> 8) & 0xFF, UART_DLH_REG(port));

    tmp_val = (UART_DLAB_DIS << 7) | (parity << 3) | (stop_bits << 2) | data_bits;
    writel(tmp_val, UART_LCR_REG(port));

    /* TODO: double check the purpose of the codes below. Jianle, 2019/01/23. */
    //if(bd >= UART_BD_921600)
    //    writel(UART_FLOWCTRL_HW,UART_MCR_REG(port));
    //else
    writel(UART_FLOWCTRL_NONE, UART_MCR_REG(port));

    current_bd = bd;
}

/*uart reinit for any uart setting changes*/
void uart_reinit(uint8_t port, uint32_t bd)
{
    uart_wait_xmit(port);
    if (current_bd != bd)
        uart_init(port, UART_PARITY_NONE, UART_STOPBITS_1, UART_DATABITS_8, bd);
}

/*****************************************************************************/
//  Function:  void uart_putc(uint8_t port, uint8_t c)
//
//  Parameters:
//      	port 		: uart index
//					= UART0
//
//		c		: byte to be send to the transmit fifo
//
//  Return:
//		none
//
//  Description:
//      send a byte to the transmission FIFO
//
/*****************************************************************************/
void uart_putc(int port, uint8_t c)
{
    uint32_t times = 0;

    while ((readl(UART_LSR_REG(port)) & LSR_THRE) == 0) {
        if (times++ >= (1UL << 23)) {
            return;
        }
    }

    writel(c, UART_THR_REG(port));
}

void uart_debug_putc(int port, uint8_t c)
{
    if (c == '\n')
        uart_putc(port, '\r');
    uart_putc(port, c);
}

void uart_wait_xmit(int port)
{
    volatile uint32_t times = 0;
    while ((readl(UART_LSR_REG(port)) & LSR_TEMT) == 0) {
        if (times++ >= (1UL << 23)) {
            return;
        }
    }
}

// uint8_t uart_getc_waitms(uint8_t port, uint32_t ms)
// {

//     uint32_t           ret   = 0xFFFFFFFF;
//     unsigned long long start = get_timer(0);

//     do {
//         if ((readl(UART_LSR_REG(port)) & LSR_RDR) == 0) {
//             udelay(POLL_STATUS_GRANULARITY_US);
//             continue;
//         } else {
//             ret = readl(UART_RBR_REG(port));
//             break;
//         }
//     } while (get_timer(start) < ms);
//     return (ret & 0xFF);
// }
/*****************************************************************************/
//  Function:  uint8_t uart_getc(uint8_t port)
//
//  Parameters:
//      	port 		: uart index
//					= UART0
//
//  Return:
//		byte to be received from the receive fifo
//
//  Description:
//      receive a byte from the receive FIFO
//
/*****************************************************************************/
uint8_t uart_getc(int port)
{
    uint32_t times = 0;

    while ((readl(UART_LSR_REG(port)) & LSR_RDR) == 0) {
        if (times++ >= (1UL << 23)) {
            return 0xFF;
        }
    }
    return (readl(UART_RBR_REG(port)) & 0xFF);
}

int uart_getSingleChar(uint8_t port)
{
    if ((readl(UART_LSR_REG(port)) & LSR_RDR) == 0)
        return -1;

    return (int)readl(UART_RBR_REG(port));
}

#if 0
 void debug(char *fmt, ...)
{
	uint32_t j, num;
	int8_t *p_buf, *p_next;
	uint32_t dword, temp, ch, flag, sftBits;
	uint32_t *p_para;
	uint32_t zeroFlag;
	int8_t *p_str;
	int port = UART_DEBUG_PORT;

	if(g_print_dis){ 
		return;
	}

	num = 0;
	p_buf = (int8_t *)fmt;
	p_para = (uint32_t *)(&fmt);

	do
	{
		if (*p_buf == '%')
		{
			p_next = p_buf + 1;
			num++;
			if ((*p_next == 'x') || (*p_next == 'X'))
			{
				if (*p_next == 'X') zeroFlag = 0x29;
				else				zeroFlag = 0x30;

				dword = p_para[num];
				flag = 0;
				sftBits = 28;
				for(j = 0; j < 8; j++)
				{
					temp = (dword&(0xFUL << sftBits )) >> sftBits;

					if (temp > 9)	ch = temp + 0x37;
					else			ch = temp + 0x30;

					if ((flag == 1) || (j == 7) || (ch != zeroFlag))
					{
						flag = 1;
						uart_debug_putc(port, (uint8_t)ch);
					}

					sftBits -= 4;
				}
			}
			else if ((*p_next == 'd') || (*p_next == 'D'))
			{
				dword = p_para[num];
				sftBits = 8;
				flag = 0;

				if (dword)
				{
					do
					{
						ch = '0';
						while (dword >= g_uart_num_to_dec[sftBits])
						{
							ch++;
							dword -= g_uart_num_to_dec[sftBits];
							flag = 1;
						}

						if (flag)
						{
							uart_debug_putc(port, (uint8_t)ch);
						}
					} while (sftBits--);
				}
				else
				{
					uart_debug_putc(port, '0');
				}
			}
			else if ((*p_next == 's') ||(*p_next == 'S'))
			{
				j = 0;
				p_str = (int8_t *)(p_para[num]);

				while (*p_str)
				{
					uart_debug_putc(port, (uint8_t)*p_str);
					p_str++;			
				}
			}
			else if ((*p_next == 'c') || (*p_next == 'C'))
			{
				//p_str = (int8_t *)(p_para[num]);

				//uart_debug_putc(port, *p_str);
			}
			else
			{
				num--;
				p_buf -= 2;

				uart_debug_putc(port, '%');
			}

			p_buf += 2;
			continue;
		}

		uart_debug_putc(port, (uint8_t)*p_buf);

		p_buf++;
	}while(*p_buf);
}

#else

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
    int port = UART_DEBUG_PORT;

    // Preconditions
    if (str == NULL)
        return;

    while (*str) {
        uart_debug_putc(port, *str);
        str++;
    }
    return;
}

void v_uart_debug(char *fmt, va_list args)
{
    uint32_t j;
    int8_t * p_buf, *p_next;
    uint32_t dword, temp, ch, flag, sftBits;
    char     buffer[MAX_UINT_DIGITS + sizeof((char)'\0') + sizeof((char)'-')];
    int      len_modifier = 0;
    int      zero_fill    = 0;
    int      port         = UART_DEBUG_PORT;

    // check disable bit from otp to disable uart debug print
    /*     if (g_print_dis) {
        return;
    } */

    p_buf = (int8_t *)fmt;

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

void sys_io_init(void)
{
    /* BOOT UART port is not its default function. Init it here. */
    /* uart 2   tx :PA2   rx :PA3*/
    volatile uint32_t tmp_value;
    tmp_value = readl(AFIO_GPIOA_FUNC_SEL);
    tmp_value &= ~(AFIO_GPIOA_SEL_BITS_MASK << AFIO_GPIOA_UART2_TX_SHIFT);
    tmp_value |= AFIO_GPIOA_UART2_TX_SEL << AFIO_GPIOA_UART2_TX_SHIFT;
    tmp_value &= ~(AFIO_GPIOA_SEL_BITS_MASK << AFIO_GPIOA_UART2_RX_SHIFT);
    tmp_value |= AFIO_GPIOA_UART2_RX_SEL << AFIO_GPIOA_UART2_RX_SHIFT;
    writel(tmp_value, AFIO_GPIOA_FUNC_SEL);
    /*uart2 PIN REMAP CF0 */
    tmp_value = readl(AFIO_PIN_REMAP_CTL);
    tmp_value &= ~(AFIO_PIN_REMAP_CTL_UART2_MASK << AFIO_PIN_REMAP_CTL_UART2_SHIFT);
    /*timer2 PIN REMAP CF10 */
    tmp_value &= ~(AFIO_PIN_REMAP_CTL_TIMER2_MASK << AFIO_PIN_REMAP_CTL_TIMER2_SHIFT);
    tmp_value |= AFIO_PIN_REMAP_CTL_TIMER2 << AFIO_PIN_REMAP_CTL_TIMER2_SHIFT;
    writel(tmp_value, AFIO_PIN_REMAP_CTL);
}
#endif
