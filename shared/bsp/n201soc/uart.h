/*
 * Copyright (c) 2018-2019 Nuclei System Technology, Inc
 * All rights reserved.
 */
#ifndef __UART_H__
#define __UART_H__

/* Register offsets */
#define UART_REG_TXFIFO         0x00
#define UART_REG_RXFIFO         0x04
#define UART_REG_TXCTRL         0x08
#define UART_REG_RXCTRL         0x0c
#define UART_REG_IE             0x10
#define UART_REG_IP             0x14
#define UART_REG_DIV            0x18

/* TXCTRL register */
#define UART_TXEN               0x1
#define UART_TXWM(x)            (((x) & 0xffff) << 16)

/* RXCTRL register */
#define UART_RXEN               0x1
#define UART_RXWM(x)            (((x) & 0xffff) << 16)

/* IP register */
#define UART_IP_TXWM            0x1
#define UART_IP_RXWM            0x2

/*
 * Exported functions
 */
extern void uart_init(unsigned int baudrate);
extern void uart_putc(unsigned char c);
extern int uart_puts(const unsigned char *s);
extern int outbyte(unsigned char c);


#endif	// __UART_H__
