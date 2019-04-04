/*
 * Copyright (c) 2018-2019 Nuclei System Technology, Inc
 * All rights reserved.
 */

#include "core.h"




###############################################
###############################################
// Interrupt Vector Entry Table
//
	.section .init


    .weak  eclic_ssip_handler 
    .weak  eclic_msip_handler 

    .weak  eclic_utip_handler 
    .weak  eclic_stip_handler 
    .weak  eclic_mtip_handler 

    .weak  eclic_ueip_handler 
    .weak  eclic_seip_handler 
    .weak  eclic_meip_handler 

    .weak  eclic_imecci_handler
    .weak  eclic_bwei_handler
    .weak  eclic_pmovi_handler
    .weak  eclic_irq19_handler

    .weak  eclic_irq20_handler
    .weak  eclic_irq21_handler
    .weak  eclic_irq22_handler
    .weak  eclic_irq23_handler

    .weak  eclic_irq24_handler
    .weak  eclic_irq25_handler
    .weak  eclic_irq26_handler
    .weak  eclic_irq27_handler

    .weak  eclic_irq28_handler
    .weak  eclic_irq29_handler
    .weak  eclic_irq30_handler
    .weak  eclic_irq31_handler

    .weak  eclic_irq32_handler
    .weak  eclic_irq33_handler
    .weak  eclic_irq34_handler
    .weak  eclic_irq35_handler

    .weak  eclic_irq36_handler
    .weak  eclic_irq37_handler
    .weak  eclic_irq38_handler
    .weak  eclic_irq39_handler

    .weak  eclic_irq40_handler
    .weak  eclic_irq41_handler
    .weak  eclic_irq42_handler
    .weak  eclic_irq43_handler

    .weak  eclic_irq44_handler
    .weak  eclic_irq45_handler
    .weak  eclic_irq46_handler
    .weak  eclic_irq47_handler

    .weak  eclic_irq48_handler
    .weak  eclic_irq49_handler
    .weak  eclic_irq50_handler
    .weak  eclic_irq51_handler

    .weak  eclic_irq52_handler
    .weak  eclic_irq53_handler
    .weak  eclic_irq54_handler
    .weak  eclic_irq55_handler

    .weak  eclic_irq56_handler
    .weak  eclic_irq57_handler
    .weak  eclic_irq58_handler
    .weak  eclic_irq59_handler

    .weak  eclic_irq50_handler
    .weak  eclic_irq61_handler
    .weak  eclic_irq62_handler
    .weak  eclic_irq63_handler

    .weak  eclic_irq64_handler
    .weak  eclic_irq65_handler
    .weak  eclic_irq66_handler
    .weak  eclic_irq67_handler

    .weak  eclic_irq68_handler
    .weak  eclic_irq69_handler
    .weak  eclic_irq70_handler

vector_base:
    j _start 
    .align 2
    .word  eclic_ssip_handler 
    .word  0 
    .word  eclic_msip_handler 

    .word  eclic_utip_handler 
    .word  eclic_stip_handler 
    .word  0 
    .word  eclic_mtip_handler 

    .word  eclic_ueip_handler 
    .word  eclic_seip_handler 
    .word  0 
    .word  eclic_meip_handler 

    .word  0
    .word  0
    .word  0
    .word  0

    .word  eclic_imecci_handler
    .word  eclic_bwei_handler
    .word  eclic_pmovi_handler
    .word  eclic_irq19_handler

    .word  eclic_irq20_handler
    .word  eclic_irq21_handler
    .word  eclic_irq22_handler
    .word  eclic_irq23_handler

    .word  eclic_irq24_handler
    .word  eclic_irq25_handler
    .word  eclic_irq26_handler
    .word  eclic_irq27_handler

    .word  eclic_irq28_handler
    .word  eclic_irq29_handler
    .word  eclic_irq30_handler
    .word  eclic_irq31_handler

    .word  eclic_irq32_handler
    .word  eclic_irq33_handler
    .word  eclic_irq34_handler
    .word  eclic_irq35_handler

    .word  eclic_irq36_handler
    .word  eclic_irq37_handler
    .word  eclic_irq38_handler
    .word  eclic_irq39_handler

    .word  eclic_irq40_handler
    .word  eclic_irq41_handler
    .word  eclic_irq42_handler
    .word  eclic_irq43_handler

    .word  eclic_irq44_handler
    .word  eclic_irq45_handler
    .word  eclic_irq46_handler
    .word  eclic_irq47_handler

    .word  eclic_irq48_handler
    .word  eclic_irq49_handler
    .word  eclic_irq50_handler
    .word  eclic_irq51_handler

    .word  eclic_irq52_handler
    .word  eclic_irq53_handler
    .word  eclic_irq54_handler
    .word  eclic_irq55_handler

    .word  eclic_irq56_handler
    .word  eclic_irq57_handler
    .word  eclic_irq58_handler
    .word  eclic_irq59_handler

    .word  eclic_irq50_handler
    .word  eclic_irq61_handler
    .word  eclic_irq62_handler
    .word  eclic_irq63_handler

    .word  eclic_irq64_handler
    .word  eclic_irq65_handler
    .word  eclic_irq66_handler
    .word  eclic_irq67_handler

    .word  eclic_irq68_handler
    .word  eclic_irq69_handler
    .word  eclic_irq70_handler


	.global _start
	.type _start,@function

_start:
	/* Set the the NMI base to share with mtvec by setting CSR_MMISC_CTL */
    li t0, (0x1 << 9);
    csrs CSR_MMISC_CTL, t0

    /* Add the cycle/instret enable by default to make it counting */
    li t0, 0x0;
    csrw CSR_MCOUNTINHIBIT, t0

	/* Intial the CSR MTVT for the interrupts vector table base addr*/
    la t0, vector_base
    csrw CSR_MTVT, t0

	/* Intial the CSR MIRQ_ENTRY and enable it for the INTERRUPT common code entry point base addr*/
    la t0, irq_entry
    csrw CSR_MIRQ_ENTRY, t0
    csrs CSR_MIRQ_ENTRY, 0x1

	/* Intial the CSR MTVEC for the Trap ane NMI base addr*/
    la t0, trap_entry
    csrw CSR_MTVEC, t0

	/* Initialize global pointer */
	.option push
	.option norelax
	la gp, __global_pointer$
	.option pop

	/* Initialize stack pointer */
	la t0, _stack
	mv sp, t0


#ifdef __riscv_flen
	/* Enable FPU */
	li t0, MSTATUS_FS
	csrrs t0, mstatus, t0

	/* Initialize FCSR */
	fscsr zero
#endif


	/* Do system low level setup. It must be a leaf function */
	call __platform_init

	/* System reset handler */
	call reset_handler

	/* Infinite loop, if returned accidently */
1:	j 1b

	.weak __platform_init
__platform_init:
	ret



	.global default_irq_entry
	.align 2

default_irq_entry:
1:	j 1b

	.weak trap_handler
trap_handler:
1:	j 1b



###############################################
###############################################
# Disable Interrupt
#
.macro DISABLE_MIE
  csrc CSR_MSTATUS, MSTATUS_MIE  
.endm


###############################################
###############################################
#Save caller registers
.macro SAVE_CONTEXT

  STORE x1,  0*REGBYTES(sp)
  STORE x5,  1*REGBYTES(sp)
  STORE x6,  2*REGBYTES(sp)
  STORE x7,  3*REGBYTES(sp)
  STORE x10, 4*REGBYTES(sp)
  STORE x11, 5*REGBYTES(sp)
  STORE x12, 6*REGBYTES(sp)
  STORE x13, 7*REGBYTES(sp)
  STORE x14, 8*REGBYTES(sp)
  STORE x15, 9*REGBYTES(sp)
  STORE x16, 10*REGBYTES(sp)
  STORE x17, 11*REGBYTES(sp)
  STORE x28, 12*REGBYTES(sp)
  STORE x29, 13*REGBYTES(sp)
  STORE x30, 14*REGBYTES(sp)
  STORE x31, 15*REGBYTES(sp)
.endm


###############################################
###############################################
#restore caller registers
.macro RESTORE_CONTEXT
  LOAD x1,  0*REGBYTES(sp)
  LOAD x5,  1*REGBYTES(sp)
  LOAD x6,  2*REGBYTES(sp)
  LOAD x7,  3*REGBYTES(sp)
  LOAD x10, 4*REGBYTES(sp)
  LOAD x11, 5*REGBYTES(sp)
  LOAD x12, 6*REGBYTES(sp)
  LOAD x13, 7*REGBYTES(sp)
  LOAD x14, 8*REGBYTES(sp)
  LOAD x15, 9*REGBYTES(sp)
  LOAD x16, 10*REGBYTES(sp)
  LOAD x17, 11*REGBYTES(sp)
  LOAD x28, 12*REGBYTES(sp)
  LOAD x29, 13*REGBYTES(sp)
  LOAD x30, 14*REGBYTES(sp)
  LOAD x31, 15*REGBYTES(sp)

.endm

###############################################
###############################################
#restore caller registers
.macro RESTORE_CONTEXT_EXCPT_X5
  LOAD x1,  0*REGBYTES(sp)
  LOAD x6,  2*REGBYTES(sp)
  LOAD x7,  3*REGBYTES(sp)
  LOAD x10, 4*REGBYTES(sp)
  LOAD x11, 5*REGBYTES(sp)
  LOAD x12, 6*REGBYTES(sp)
  LOAD x13, 7*REGBYTES(sp)
  LOAD x14, 8*REGBYTES(sp)
  LOAD x15, 9*REGBYTES(sp)
  LOAD x16, 10*REGBYTES(sp)
  LOAD x17, 11*REGBYTES(sp)
  LOAD x28, 12*REGBYTES(sp)
  LOAD x29, 13*REGBYTES(sp)
  LOAD x30, 14*REGBYTES(sp)
  LOAD x31, 15*REGBYTES(sp)

.endm

###############################################
###############################################
#restore caller registers
.macro RESTORE_CONTEXT_ONLY_X5
  LOAD x5,  1*REGBYTES(sp)
.endm

###############################################
###############################################
# Save the mepc and mstatus
#
.macro SAVE_EPC_STATUS
  csrr x5, CSR_MEPC  
  STORE x5,  16*REGBYTES(sp)
  csrr x5, CSR_MSTATUS  
  STORE x5,  17*REGBYTES(sp)
  csrr x5, CSR_MXSTATUS  
  STORE x5,  18*REGBYTES(sp)
.endm

###############################################
###############################################
# Restore the mepc and mstatus
#
.macro RESTORE_EPC_STATUS
  LOAD x5,  16*REGBYTES(sp)
  csrw CSR_MEPC, x5  
  LOAD x5,  17*REGBYTES(sp)
  csrw CSR_MSTATUS, x5  
  LOAD x5,  18*REGBYTES(sp)
  csrw CSR_MXSTATUS, x5  
.endm



###############################################
###############################################
// Trap entry point
//
  .section      .text.trap	
  .align 6// In ECLIC mode, the trap entry must be 64bytes aligned
  .global trap_entry
.weak trap_entry
trap_entry:
  // Allocate the stack space
  addi sp, sp, -19*REGBYTES

  // Save the caller saving registers (context)
  SAVE_CONTEXT
  // Save the MEPC/Mstatus/Mxstatus reg
  SAVE_EPC_STATUS

     // Set the function argument
  csrr a0, mcause
  mv a1, sp
     // Call the function
  call trap_handler

  // Restore the MEPC/Mstatus/Mxstatus reg
  RESTORE_EPC_STATUS
  // Restore the caller saving registers (context)
  RESTORE_CONTEXT

  // De-allocate the stack space
  addi sp, sp, 19*REGBYTES
  // Return to regular code
  mret


###############################################
###############################################
// IRQ entry point
//
  .section      .text.irq	
  .align 2
  .global irq_entry
.weak irq_entry
irq_entry: // -------------> This label will be set to MTVT2 register
  // Allocate the stack space
  addi sp, sp, -19*REGBYTES

  SAVE_CONTEXT// Save 16 regs

  //------This special CSR read operation, which is actually use mcause as operand to directly store it to memory
  csrrwi  x0, CSR_PUSHMCAUSE, 16
  //------This special CSR read operation, which is actually use mepc as operand to directly store it to memory
  csrrwi  x0, CSR_PUSHMEPC, 17
  //------This special CSR read operation, which is actually use mxstatus as operand to directly store it to memory
  csrrwi  x0, CSR_PUSHMXSTATUS, 18
 
service_loop:
  //------This special CSR read/write operation, which is actually Claim the ECLIC to find its pending highest
  // ID, if the ID is not 0, then automatically enable the mstatus.MIE, and jump to its vector-entry-label, and
  // update the link register 
  csrrw ra, CSR_MINTSEL_JAL, ra 
  
  RESTORE_CONTEXT_EXCPT_X5

  #---- Critical section with interrupts disabled -----------------------
  DISABLE_MIE # Disable interrupts 

  LOAD x5,  18*REGBYTES(sp)
  csrw CSR_MXSTATUS, x5  
  LOAD x5,  17*REGBYTES(sp)
  csrw CSR_MEPC, x5  
  LOAD x5,  16*REGBYTES(sp)
  csrw CSR_MCAUSE, x5  


  RESTORE_CONTEXT_ONLY_X5

  // De-allocate the stack space
  addi sp, sp, 19*REGBYTES
  // Return to regular code
  mret







