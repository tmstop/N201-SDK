/*
 * Copyright (c) 2018-2019 Nuclei System Technology, Inc
 * All rights reserved.
 */

#include "platform.h"
#include <unistd.h>


void uart_init(unsigned int baudrate)
{
  GPIO_REG(GPIO_IOF_SEL) &= ~IOF0_UART0_MASK;
  GPIO_REG(GPIO_IOF_EN) |= IOF0_UART0_MASK;
  UART0_REG(UART_REG_DIV) = get_cpu_freq() / baudrate - 1;
  UART0_REG(UART_REG_TXCTRL) |= UART_TXEN;
  UART0_REG(UART_REG_RXCTRL) |= UART_RXEN;

	
}



void uart_putc(unsigned char c)
{
    while (UART0_REG(UART_REG_TXFIFO) & 0x80000000) ;
    UART0_REG(UART_REG_TXFIFO) = c;
}

int uart_puts(const unsigned char *s)
{
	int len = 0;

	while (*s) {
		uart_putc(*s);

		if (*s == '\n')
			uart_putc('\r');
		s++;
		len++;
	}
	return len;
}

int outbyte(unsigned char c)
{
	uart_putc(c);
	if (c =='\n')
		uart_putc('\r');
	return c;
}
