/*
 * Copyright (c) 2018-2019 Nuclei System Technology, Inc
 * All rights reserved.
 *
 */

#ifndef __N201SOC_H__
#define __N201SOC_H__

#ifdef __cplusplus
extern "C" {
#endif

 /****************************************************************************
 * Platform definitions
 *****************************************************************************/


#define GPIO_ADDR_BASE              0x10012000
#define UART0_ADDR_BASE             0x10013000
#define SPI0_ADDR_BASE              0x10014000
#define I2C_ADDR_BASE               0x10042000

    // The ECLIC and TMR base address for N201 core is fixed
#define SOC_TMR_ADDR_BASE              0x02000000
#define SOC_ECLIC_ADDR_BASE             0x0C000000

// IOF Mappings
#define IOF0_UART0_MASK             0x00030000
#define IOF_UART0_RX   (16u)
#define IOF_UART0_TX   (17u)

#define IOF0_I2C_MASK               0x00003000
#define IOF_I2C_SDA (12u)
#define IOF_I2C_SCL (13u)


// Interrupt Numbers
#define SOC_ECLIC_NUM_INTERRUPTS 16


// The TMR frequency is just the RTC frequency as 32.768KHz
#define SOC_TMR_FREQ 32768

// Helper functions
#define _REG8(p, i)             (*(volatile uint8_t *) ((p) + (i)))
#define _REG32(p, i)            (*(volatile uint32_t *) ((p) + (i)))
#define _REG64(p, i)            (*(volatile uint64_t *) ((p) + (i)))
#define _REG32P(p, i)           ((volatile uint32_t *) ((p) + (i)))


#define GPIO_REG(offset)        _REG32(GPIO_ADDR_BASE, offset)
#define SPI0_REG(offset)        _REG32(SPI0_ADDR_BASE, offset)
#define UART0_REG(offset)       _REG32(UART0_ADDR_BASE, offset)
#define I2C_REG8(offset)        _REG8(I2C_ADDR_BASE, offset)

// Misc

#define NUM_GPIO 32




#ifdef __cplusplus
}
#endif

#endif	/* __N201SOC_H__ */
