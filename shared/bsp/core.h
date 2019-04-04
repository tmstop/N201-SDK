#ifndef __CORE_H__
#define __CORE_H__

#include "n201soc.h"
#define MSTATUS_UIE         	0x00000001
#define MSTATUS_SIE         	0x00000002
#define MSTATUS_HIE         	0x00000004
#define MSTATUS_MIE         	0x00000008
#define MSTATUS_UPIE        	0x00000010
#define MSTATUS_SPIE        	0x00000020
#define MSTATUS_HPIE        	0x00000040
#define MSTATUS_MPIE        	0x00000080
#define MSTATUS_SPP         	0x00000100
#define MSTATUS_HPP         	0x00000600
#define MSTATUS_MPP         	0x00001800
#define MSTATUS_FS          	0x00006000
#define MSTATUS_XS          	0x00018000
#define MSTATUS_MPRV        	0x00020000
#define MSTATUS_PUM         	0x00040000
#define MSTATUS_MXR		0x00080000
#define MSTATUS_VM          	0x1F000000
#define MSTATUS32_SD        	0x80000000
#define MSTATUS64_SD        	0x8000000000000000

#if __riscv_xlen == 64
#define MCAUSE_INT          	0x8000000000000000UL
#define MCAUSE_CAUSE        	0x0000000000000FFFUL
#else
#define MCAUSE_INT          	0x80000000UL
#define MCAUSE_CAUSE        	0x00000FFFUL
#endif

#define IRQ_S_SOFT          	1
#define IRQ_H_SOFT          	2
#define IRQ_M_SOFT          	3
#define IRQ_S_TIMER         	5
#define IRQ_H_TIMER         	6
#define IRQ_M_TIMER         	7
#define IRQ_S_EXT           	9
#define IRQ_H_EXT           	10
#define IRQ_M_EXT           	11
#define IRQ_COP             	12
#define IRQ_HOST            	13

/* Machine mode MCAUSE */
#define TRAP_M_I_ACC_FAULT    	1   /* Instruction access fault */
#define TRAP_M_L_ACC_FAULT   	5   /* Data load access fault */
#define TRAP_M_S_ACC_FAULT   	7   /* Data store access fault */
#define TRAP_U_ECALL        	8
#define TRAP_S_ECALL        	9
#define TRAP_H_ECALL        	10
#define TRAP_M_ECALL        	11
#define TRAP_M_I_PAGE_FAULT	12  /* Instruction page fault */
#define TRAP_M_L_PAGE_FAULT	13  /* Data load page fault */
#define TRAP_M_S_PAGE_FAULT	15  /* Data store page fault */
#define TRAP_M_STACKOVF     	32
#define TRAP_M_STACKUDF     	33
#define TRAP_NMI            	0xFFF

/* Supervisor mode SCAUSE */
#define TRAP_S_I_ACC_FAULT    	1   /* Instruction access fault */
#define TRAP_S_L_ACC_FAULT   	5   /* Data load access fault */
#define TRAP_S_S_ACC_FAULT   	7   /* Data store access fault */
#define TRAP_S_I_PAGE_FAULT	12  /* Instruction page fault */
#define TRAP_S_L_PAGE_FAULT	13  /* Data load page fault */
#define TRAP_S_S_PAGE_FAULT	15  /* Data store page fault */

#define MIP_SSIP            	(1 << IRQ_S_SOFT)
#define MIP_HSIP            	(1 << IRQ_H_SOFT)
#define MIP_MSIP            	(1 << IRQ_M_SOFT)
#define MIP_STIP            	(1 << IRQ_S_TIMER)
#define MIP_HTIP            	(1 << IRQ_H_TIMER)
#define MIP_MTIP            	(1 << IRQ_M_TIMER)
#define MIP_SEIP            	(1 << IRQ_S_EXT)
#define MIP_HEIP            	(1 << IRQ_H_EXT)
#define MIP_MEIP            	(1 << IRQ_M_EXT)

/* MILMB and MDLMB */
#define	MILMB_IEN		(0x1UL)
#define	MDLMB_DEN		(0x1UL)

#if __riscv_xlen == 64
# define SLL32    		sllw
# define STORE    		sd
# define LOAD     		ld
# define LWU      		lwu
# define LOG_REGBYTES 		3
#else
# define SLL32    		sll
# define STORE    		sw
# define LOAD     		lw
# define LWU      		lw
# define LOG_REGBYTES		2
#endif
#define REGBYTES		(1 << LOG_REGBYTES)

#if __riscv_flen == 64
# define FPSTORE		fsd
# define FPLOAD			fld
# define LOG_FPREGBYTES		3
#else
# define FPSTORE		fsw
# define FPLOAD			flw
# define LOG_FPREGBYTES		2
#endif
#define FPREGBYTES              (1 << LOG_FPREGBYTES)

#define STR(S)			#S
#define XSTR(S)			STR(S)

#define PUSH			XSTR(STORE)
#define POP			XSTR(LOAD)
#define REGSIZE			XSTR(REGBYTES)
#define FPPUSH			XSTR(FPSTORE)
#define FPPOP			XSTR(FPLOAD)
#define FPREGSIZE		XSTR(FPREGBYTES)

#ifndef __ASSEMBLER__
#ifdef __GNUC__

#define read_csr(reg) ({ unsigned long __tmp; \
  asm volatile ("csrr %0, " #reg : "=r"(__tmp)); \
  __tmp; })

#define write_csr(reg, val) ({ \
  if (__builtin_constant_p(val) && (unsigned long)(val) < 32) \
    asm volatile ("csrw " #reg ", %0" :: "i"(val)); \
  else \
    asm volatile ("csrw " #reg ", %0" :: "r"(val)); })

#define swap_csr(reg, val) ({ unsigned long __tmp; \
  if (__builtin_constant_p(val) && (unsigned long)(val) < 32) \
    asm volatile ("csrrw %0, " #reg ", %1" : "=r"(__tmp) : "i"(val)); \
  else \
    asm volatile ("csrrw %0, " #reg ", %1" : "=r"(__tmp) : "r"(val)); \
  __tmp; })

#define set_csr(reg, bit) ({ unsigned long __tmp; \
  if (__builtin_constant_p(bit) && (unsigned long)(bit) < 32) \
    asm volatile ("csrrs %0, " #reg ", %1" : "=r"(__tmp) : "i"(bit)); \
  else \
    asm volatile ("csrrs %0, " #reg ", %1" : "=r"(__tmp) : "r"(bit)); \
  __tmp; })

#define clear_csr(reg, bit) ({ unsigned long __tmp; \
  if (__builtin_constant_p(bit) && (unsigned long)(bit) < 32) \
    asm volatile ("csrrc %0, " #reg ", %1" : "=r"(__tmp) : "i"(bit)); \
  else \
    asm volatile ("csrrc %0, " #reg ", %1" : "=r"(__tmp) : "r"(bit)); \
  __tmp; })

#define rdtime() read_csr(time)
#define rdcycle() read_csr(cycle)
#define rdinstret() read_csr(instret)

#endif
#endif

#define CSR_FFLAGS 0x1
#define CSR_FRM 0x2
#define CSR_FCSR 0x3
#define CSR_CYCLE 0xc00
#define CSR_TIME 0xc01
#define CSR_INSTRET 0xc02
#define CSR_HPMCOUNTER3 0xc03
#define CSR_HPMCOUNTER4 0xc04
#define CSR_HPMCOUNTER5 0xc05
#define CSR_HPMCOUNTER6 0xc06
#define CSR_HPMCOUNTER7 0xc07
#define CSR_HPMCOUNTER8 0xc08
#define CSR_HPMCOUNTER9 0xc09
#define CSR_HPMCOUNTER10 0xc0a
#define CSR_HPMCOUNTER11 0xc0b
#define CSR_HPMCOUNTER12 0xc0c
#define CSR_HPMCOUNTER13 0xc0d
#define CSR_HPMCOUNTER14 0xc0e
#define CSR_HPMCOUNTER15 0xc0f
#define CSR_HPMCOUNTER16 0xc10
#define CSR_HPMCOUNTER17 0xc11
#define CSR_HPMCOUNTER18 0xc12
#define CSR_HPMCOUNTER19 0xc13
#define CSR_HPMCOUNTER20 0xc14
#define CSR_HPMCOUNTER21 0xc15
#define CSR_HPMCOUNTER22 0xc16
#define CSR_HPMCOUNTER23 0xc17
#define CSR_HPMCOUNTER24 0xc18
#define CSR_HPMCOUNTER25 0xc19
#define CSR_HPMCOUNTER26 0xc1a
#define CSR_HPMCOUNTER27 0xc1b
#define CSR_HPMCOUNTER28 0xc1c
#define CSR_HPMCOUNTER29 0xc1d
#define CSR_HPMCOUNTER30 0xc1e
#define CSR_HPMCOUNTER31 0xc1f
#define CSR_SSTATUS 0x100
#define CSR_SIE 0x104
#define CSR_STVEC 0x105
#define CSR_SSCRATCH 0x140
#define CSR_SEPC 0x141
#define CSR_SCAUSE 0x142
#define CSR_SBADADDR 0x143
#define CSR_SIP 0x144
#define CSR_SPTBR 0x180
#define CSR_MSTATUS 0x300
#define CSR_MISA 0x301
#define CSR_MEDELEG 0x302
#define CSR_MIDELEG 0x303
#define CSR_MIE 0x304
#define CSR_MTVEC 0x305
#define CSR_MCOUNTEREN 0x306
#define CSR_MSCRATCH 0x340
#define CSR_MEPC 0x341
#define CSR_MCAUSE 0x342
#define CSR_MBADADDR 0x343
#define CSR_MTVAL 0x343
#define CSR_MIP 0x344
#define CSR_TSELECT 0x7a0
#define CSR_TDATA1 0x7a1
#define CSR_TDATA2 0x7a2
#define CSR_TDATA3 0x7a3
#define CSR_DCSR 0x7b0
#define CSR_DPC 0x7b1
#define CSR_DSCRATCH 0x7b2
#define CSR_MCYCLE 0xb00
#define CSR_MINSTRET 0xb02
#define CSR_MHPMCOUNTER3 0xb03
#define CSR_MHPMCOUNTER4 0xb04
#define CSR_MHPMCOUNTER5 0xb05
#define CSR_MHPMCOUNTER6 0xb06
#define CSR_MHPMCOUNTER7 0xb07
#define CSR_MHPMCOUNTER8 0xb08
#define CSR_MHPMCOUNTER9 0xb09
#define CSR_MHPMCOUNTER10 0xb0a
#define CSR_MHPMCOUNTER11 0xb0b
#define CSR_MHPMCOUNTER12 0xb0c
#define CSR_MHPMCOUNTER13 0xb0d
#define CSR_MHPMCOUNTER14 0xb0e
#define CSR_MHPMCOUNTER15 0xb0f
#define CSR_MHPMCOUNTER16 0xb10
#define CSR_MHPMCOUNTER17 0xb11
#define CSR_MHPMCOUNTER18 0xb12
#define CSR_MHPMCOUNTER19 0xb13
#define CSR_MHPMCOUNTER20 0xb14
#define CSR_MHPMCOUNTER21 0xb15
#define CSR_MHPMCOUNTER22 0xb16
#define CSR_MHPMCOUNTER23 0xb17
#define CSR_MHPMCOUNTER24 0xb18
#define CSR_MHPMCOUNTER25 0xb19
#define CSR_MHPMCOUNTER26 0xb1a
#define CSR_MHPMCOUNTER27 0xb1b
#define CSR_MHPMCOUNTER28 0xb1c
#define CSR_MHPMCOUNTER29 0xb1d
#define CSR_MHPMCOUNTER30 0xb1e
#define CSR_MHPMCOUNTER31 0xb1f
#define CSR_MUCOUNTEREN 0x320
#define CSR_MSCOUNTEREN 0x321
#define CSR_MHPMEVENT3 0x323
#define CSR_MHPMEVENT4 0x324
#define CSR_MHPMEVENT5 0x325
#define CSR_MHPMEVENT6 0x326
#define CSR_MHPMEVENT7 0x327
#define CSR_MHPMEVENT8 0x328
#define CSR_MHPMEVENT9 0x329
#define CSR_MHPMEVENT10 0x32a
#define CSR_MHPMEVENT11 0x32b
#define CSR_MHPMEVENT12 0x32c
#define CSR_MHPMEVENT13 0x32d
#define CSR_MHPMEVENT14 0x32e
#define CSR_MHPMEVENT15 0x32f
#define CSR_MHPMEVENT16 0x330
#define CSR_MHPMEVENT17 0x331
#define CSR_MHPMEVENT18 0x332
#define CSR_MHPMEVENT19 0x333
#define CSR_MHPMEVENT20 0x334
#define CSR_MHPMEVENT21 0x335
#define CSR_MHPMEVENT22 0x336
#define CSR_MHPMEVENT23 0x337
#define CSR_MHPMEVENT24 0x338
#define CSR_MHPMEVENT25 0x339
#define CSR_MHPMEVENT26 0x33a
#define CSR_MHPMEVENT27 0x33b
#define CSR_MHPMEVENT28 0x33c
#define CSR_MHPMEVENT29 0x33d
#define CSR_MHPMEVENT30 0x33e
#define CSR_MHPMEVENT31 0x33f
#define CSR_MVENDORID 0xf11
#define CSR_MARCHID 0xf12
#define CSR_MIMPID 0xf13
#define CSR_MHARTID 0xf14
#define CSR_CYCLEH 0xc80
#define CSR_TIMEH 0xc81
#define CSR_INSTRETH 0xc82
#define CSR_HPMCOUNTER3H 0xc83
#define CSR_HPMCOUNTER4H 0xc84
#define CSR_HPMCOUNTER5H 0xc85
#define CSR_HPMCOUNTER6H 0xc86
#define CSR_HPMCOUNTER7H 0xc87
#define CSR_HPMCOUNTER8H 0xc88
#define CSR_HPMCOUNTER9H 0xc89
#define CSR_HPMCOUNTER10H 0xc8a
#define CSR_HPMCOUNTER11H 0xc8b
#define CSR_HPMCOUNTER12H 0xc8c
#define CSR_HPMCOUNTER13H 0xc8d
#define CSR_HPMCOUNTER14H 0xc8e
#define CSR_HPMCOUNTER15H 0xc8f
#define CSR_HPMCOUNTER16H 0xc90
#define CSR_HPMCOUNTER17H 0xc91
#define CSR_HPMCOUNTER18H 0xc92
#define CSR_HPMCOUNTER19H 0xc93
#define CSR_HPMCOUNTER20H 0xc94
#define CSR_HPMCOUNTER21H 0xc95
#define CSR_HPMCOUNTER22H 0xc96
#define CSR_HPMCOUNTER23H 0xc97
#define CSR_HPMCOUNTER24H 0xc98
#define CSR_HPMCOUNTER25H 0xc99
#define CSR_HPMCOUNTER26H 0xc9a
#define CSR_HPMCOUNTER27H 0xc9b
#define CSR_HPMCOUNTER28H 0xc9c
#define CSR_HPMCOUNTER29H 0xc9d
#define CSR_HPMCOUNTER30H 0xc9e
#define CSR_HPMCOUNTER31H 0xc9f
#define CSR_MCYCLEH 0xb80
#define CSR_MINSTRETH 0xb82
#define CSR_MHPMCOUNTER3H 0xb83
#define CSR_MHPMCOUNTER4H 0xb84
#define CSR_MHPMCOUNTER5H 0xb85
#define CSR_MHPMCOUNTER6H 0xb86
#define CSR_MHPMCOUNTER7H 0xb87
#define CSR_MHPMCOUNTER8H 0xb88
#define CSR_MHPMCOUNTER9H 0xb89
#define CSR_MHPMCOUNTER10H 0xb8a
#define CSR_MHPMCOUNTER11H 0xb8b
#define CSR_MHPMCOUNTER12H 0xb8c
#define CSR_MHPMCOUNTER13H 0xb8d
#define CSR_MHPMCOUNTER14H 0xb8e
#define CSR_MHPMCOUNTER15H 0xb8f
#define CSR_MHPMCOUNTER16H 0xb90
#define CSR_MHPMCOUNTER17H 0xb91
#define CSR_MHPMCOUNTER18H 0xb92
#define CSR_MHPMCOUNTER19H 0xb93
#define CSR_MHPMCOUNTER20H 0xb94
#define CSR_MHPMCOUNTER21H 0xb95
#define CSR_MHPMCOUNTER22H 0xb96
#define CSR_MHPMCOUNTER23H 0xb97
#define CSR_MHPMCOUNTER24H 0xb98
#define CSR_MHPMCOUNTER25H 0xb99
#define CSR_MHPMCOUNTER26H 0xb9a
#define CSR_MHPMCOUNTER27H 0xb9b
#define CSR_MHPMCOUNTER28H 0xb9c
#define CSR_MHPMCOUNTER29H 0xb9d
#define CSR_MHPMCOUNTER30H 0xb9e
#define CSR_MHPMCOUNTER31H 0xb9f


#define CSR_MNVEC          0x7C3
#define CSR_MIRQ_ENTRY     0x7EC
#define CSR_MINTSEL_JAL    0x7ED
#define CSR_PUSHMCAUSE     0x7EE
#define CSR_PUSHMEPC       0x7EF
#define CSR_PUSHMXSTATUS   0x7EB
#define CSR_WFE            0x810
#define CSR_SLEEPVALUE     0x811
#define CSR_TXEVT          0x812
#define CSR_MMISC_CTL      0x7d0
#define CSR_MXSTATUS       0x7c4
#define CSR_MCOUNTINHIBIT  0x320 

#define CSR_MSAVESTATUS    0x7D6
#define CSR_MSAVEEPC1      0x7D7
#define CSR_MSAVECAUSE1    0x7D8
#define CSR_MSAVEEPC2      0x7D9
#define CSR_MSAVECAUSE2    0x7DA
#define CSR_MSAVEDCAUSE1   0x7DB
#define CSR_MSAVEDCAUSE2   0x7DC
#define CSR_MMSC_CFG       0xFC2

#define CSR_MSCRATCHCSW    0x348
#define CSR_MSCRATCHCSWL   0x349
#define CSR_MTVT           0x307
#define CSR_MNXTI          0x345
#define CSR_MINTSTATUS     0x346
#define CSR_MDCAUSE        0x7C9
#define CSR_MCOUNTERINTEN  0x306
#define CSR_MCOUNTEROVF    0x7D4


  // Need to know the following info from the soc.h, so include soc.h here
  //   SOC_TMR_FREQ           : what is the frequency for TMR to count (by N201 core's input signal mtime_toggle_a) 
  //   SOC_ECLIC_NUM_INTERRUPTS: how much of the ECLIC external interrupts supported
  //   SOC_TMR_ADDR_BASE 
  //   SOC_ECLIC_ADDR_BASE
#include "n201soc.h"
#define TMR_FREQ   SOC_TMR_FREQ
#define ECLIC_NUM_INTERRUPTS (SOC_ECLIC_NUM_INTERRUPTS + 19)
#define TMR_ADDR_BASE    SOC_TMR_ADDR_BASE 
#define ECLIC_ADDR_BASE   SOC_ECLIC_ADDR_BASE

#define TMR_REG(offset)     _REG32(TMR_ADDR_BASE, offset)
#define TMR_REG64(offset)   _REG64(TMR_ADDR_BASE, offset)
#define ECLIC_REG(offset)    _REG32(ECLIC_ADDR_BASE, offset)
#define ECLIC_REG8(offset)   _REG8(ECLIC_ADDR_BASE, offset)

#define TMR_MTIMECMP_OFFSET 0x8
#define TMR_MTIME_OFFSET 0x0
#define TMR_MSIP_OFFSET 0xFFC


#define ECLICINTCTLBITS  3 // N201 core have the ECLICINTCTLBITS bits as 3 bits fixed

//ECLIC memory map
//   Offset
//  0x0000       1B          RW        cliccfg
#define ECLIC_CFG_OFFSET            0x0
//  0x0004       4B          R         clicinfo   
#define ECLIC_INFO_OFFSET           0x4
//  0x000B       1B          RW        mintthresh 
#define ECLIC_MTH_OFFSET         0xB
//
//  0x1000+4*i   1B/input    RW        clicintip[i]
#define ECLIC_INT_IP_OFFSET            0x1000
//  0x1001+4*i   1B/input    RW        clicintie[i]
#define ECLIC_INT_IE_OFFSET            0x1001
//  0x1002+4*i   1B/input    RW        clicintattr[i]
#define ECLIC_INT_ATTR_OFFSET          0x1002

#define ECLIC_INT_ATTR_SHV              0x01
#define ECLIC_INT_ATTR_TRIG_LEVEL       0x00
#define ECLIC_INT_ATTR_TRIG_EDGE        0x02
#define ECLIC_INT_ATTR_TRIG_POS         0x00
#define ECLIC_INT_ATTR_TRIG_NEG         0x04

//  0x1003+4*i   1B/input    RW        clicintctl[i]
#define ECLIC_INT_CTRL_OFFSET          0x1003
//
//  ...
//

#define ECLIC_CFG_NLBITS_MASK          0x1E
#define ECLIC_CFG_NLBITS_LSB     (1u)

#define ECLIC_IRQ_ID_MSIP        3
#define ECLIC_IRQ_ID_MTIP        7
#define ECLIC_IRQ_ID_BWEI        17


#ifndef __ASSEMBLER__
#include <unistd.h>
/*
 * Exported functions
 */
extern uint32_t get_tmr_freq();
extern uint32_t mtime_lo(void);
extern uint32_t mtime_hi(void);
extern uint64_t get_mtime_value();
extern uint64_t get_instret_value();
extern uint64_t get_cycle_value();  
extern uint32_t get_cpu_freq();
extern uint32_t __attribute__((noinline)) measure_cpu_freq(size_t n);
extern void stop_mcycle_minstret();
extern void start_mcycle_minstret();

extern void eclic_init ( uint32_t num_irq );
extern void eclic_enable_interrupt (uint32_t source);
extern void eclic_disable_interrupt (uint32_t source);
extern void eclic_set_pending(uint32_t source);
extern void eclic_clear_pending(uint32_t source);
extern void eclic_set_intctrl (uint32_t source, uint8_t intctrl);
extern uint8_t eclic_get_intctrl  (uint32_t source);
extern void eclic_set_intattr (uint32_t source, uint8_t intattr);
extern uint8_t eclic_get_intattr  (uint32_t source);
extern void eclic_set_cliccfg (uint8_t cliccfg);
extern uint8_t eclic_get_cliccfg ();
extern void eclic_set_mth (uint8_t mth);
extern uint8_t eclic_get_mth();
extern void eclic_set_nlbits(uint8_t nlbits);
extern uint8_t eclic_get_nlbits();
extern void eclic_set_int_level(uint32_t source, uint8_t level);
extern uint8_t eclic_get_int_level(uint32_t source);
extern void eclic_mode_enable();
extern void eclic_set_int_posedge_trig(uint32_t source);
extern void eclic_set_int_negedge_trig(uint32_t source);
extern void eclic_set_int_level_trig(uint32_t source);
#endif /* __ASSEMBLER__ */


#endif	// __CORE_H__
