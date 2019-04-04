/*
 * Copyright (c) 2018-2019 Nuclei System Technology, Inc
 * All rights reserved.
 *
 */


#include "platform.h"
#include <stdio.h>
#include <unistd.h>

#include "stdatomic.h"
extern void reset_vector(void);

void __platform_init(void)
{
	/* Do your platform low-level initial */
}

void c_startup(void)
{
#define MEMCPY(des, src, n)     __builtin_memcpy ((des), (src), (n))
#define MEMSET(s, c, n)         __builtin_memset ((s), (c), (n))
	/* Data section initialization */
	extern char  _edata, _end;
	unsigned int size;

	extern char __data_lmastart, __data_start;

	/* Copy data section from LMA to VMA */
	size = &_edata - &__data_start;
	MEMCPY(&__data_start, &__data_lmastart, size);

	/* Clear bss section */
	size = &_end - &_edata;
	MEMSET(&_edata, 0, size);
}

void system_init(void)
{
	/*
	 * Do your system reset handling here
	 */

	/*
	 * Do your own SoC init
	 */


	/* Initial UART port */
	uart_init(115200);
	
	printf("**************************************\n");
	printf("**************************************\n");
	printf("*            	N201 SOC             *\n");
	printf("Core freq at %d Hz\n", get_cpu_freq());
	printf("*                                    *\n");
	printf("**************************************\n");
	printf("**************************************\n");
	printf("\n\n\nAfter ECLIC mode enabled, the mtvec value is %x \n\n\n", read_csr(mtvec));
	
}
