/*
 * Copyright (c) 2018-2019 Nuclei System Technology, Inc
 * All rights reserved.
 *
 */

#include <stdio.h>
#include "platform.h"

void default_irq_handler(void)
{
	printf("Default interrupt handler\n");
}



void msip_irq_handler  (void) __attribute__((weak, alias("default_irq_handler")));
void mtip_irq_handler  (void) __attribute__((weak, alias("default_irq_handler")));
void bwei_irq_handler  (void) __attribute__((weak, alias("default_irq_handler")));

void gpio16_irq_handler(void) __attribute__((weak, alias("default_irq_handler")));
void gpio17_irq_handler(void) __attribute__((weak, alias("default_irq_handler")));
void gpio18_irq_handler(void) __attribute__((weak, alias("default_irq_handler")));
void gpio19_irq_handler(void) __attribute__((weak, alias("default_irq_handler")));
void gpio20_irq_handler(void) __attribute__((weak, alias("default_irq_handler")));
void gpio21_irq_handler(void) __attribute__((weak, alias("default_irq_handler")));
void gpio22_irq_handler(void) __attribute__((weak, alias("default_irq_handler")));
void gpio23_irq_handler(void) __attribute__((weak, alias("default_irq_handler")));
void gpio24_irq_handler(void) __attribute__((weak, alias("default_irq_handler")));
void gpio25_irq_handler(void) __attribute__((weak, alias("default_irq_handler")));
void gpio26_irq_handler(void) __attribute__((weak, alias("default_irq_handler")));
void gpio27_irq_handler(void) __attribute__((weak, alias("default_irq_handler")));
void gpio28_irq_handler(void) __attribute__((weak, alias("default_irq_handler")));
void gpio29_irq_handler(void) __attribute__((weak, alias("default_irq_handler")));
void gpio30_irq_handler(void) __attribute__((weak, alias("default_irq_handler")));
void gpio31_irq_handler(void) __attribute__((weak, alias("default_irq_handler")));


void eclic_entry_msip (void) { msip_irq_handler  ();}
//void eclic_entry_mtip (void) { mtip_irq_handler  ();}
void eclic_entry_bwei (void) { bwei_irq_handler  ();}

void eclic_entry_irq19(void) { gpio16_irq_handler();}
void eclic_entry_irq20(void) { gpio17_irq_handler();}
void eclic_entry_irq21(void) { gpio18_irq_handler();}
void eclic_entry_irq22(void) { gpio19_irq_handler();}
void eclic_entry_irq23(void) { gpio20_irq_handler();}
void eclic_entry_irq24(void) { gpio21_irq_handler();}
void eclic_entry_irq25(void) { gpio22_irq_handler();}
void eclic_entry_irq26(void) { gpio23_irq_handler();}
void eclic_entry_irq27(void) { gpio24_irq_handler();}
void eclic_entry_irq28(void) { gpio25_irq_handler();}
void eclic_entry_irq29(void) { gpio26_irq_handler();}
void eclic_entry_irq30(void) { gpio27_irq_handler();}
void eclic_entry_irq31(void) { gpio28_irq_handler();}
void eclic_entry_irq32(void) { gpio29_irq_handler();}
void eclic_entry_irq33(void) { gpio30_irq_handler();}
void eclic_entry_irq34(void) { gpio31_irq_handler();}


