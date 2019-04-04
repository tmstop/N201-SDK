/*
 * Copyright (c) 2018-2019 Nuclei System Technology, Inc
 * All rights reserved.
 */

#include <stdio.h>
#include "platform.h"

typedef struct {
	union {
		struct {
			long x1;
			long x4;
			long x5;
			long x6;
			long x7;
			long x10;
			long x11;
			long x12;
			long x13;
			long x14;
			long x15;
			long x16;
			long x17;
			long x28;
			long x29;
			long x30;
			long x31;
		};
		long caller_regs[17];
	};
	long mepc;
	long mstatus;
	long mxstatus;
} SAVED_CONTEXT;



__attribute__((weak)) void syscall_handler(long n, long a0, long a1, long a2, long a3)
{
	printf("syscall #%ld (a0:0x%lx,a1:0x%lx, a2:0x%lx, a3:0x%lx)\n", n, a0, a1, a2, a3);
}

__attribute__((weak)) void sv_syscall_handler(long n, long a0, long a1, long a2, long a3)
{
	printf("sv syscall #%ld (a0:0x%lx,a1:0x%lx, a2:0x%lx, a3:0x%lx)\n", n, a0, a1, a2, a3);
}

__attribute__((weak)) void user_syscall_handler(long n, long a0, long a1, long a2, long a3)
{
	printf("user syscall #%ld (a0:0x%lx,a1:0x%lx, a2:0x%lx, a3:0x%lx)\n", n, a0, a1, a2, a3);
}

__attribute__((weak)) long except_handler(long cause, long epc, long *reg)
{
	/* Unhandled Trap */
	printf("Unhandled Trap : mcause = 0x%x, mepc = 0x%x\n", (unsigned int)cause, (unsigned int)epc);

	return epc;
}

__attribute__((weak)) long nmi_handler(long cause, long epc, long *reg)
{
	/* NMI handler, to be updated by user */
	printf("Unhandled NMI : mcause = 0x%x, mepc = 0x%x\n", (unsigned int)cause, (unsigned int)epc);

	return epc;
}

void trap_handler(unsigned long mcause, SAVED_CONTEXT *context)
{
	/* Do your trap handling */
    if ((!(mcause & MCAUSE_INT)) && ((mcause & MCAUSE_CAUSE) == TRAP_NMI)) {
		/* NMI handler */
		context->mepc = nmi_handler(mcause, context->mepc, context->caller_regs);
	} else if (!(mcause & MCAUSE_INT) && ((mcause & MCAUSE_CAUSE) == TRAP_M_ECALL)) {
		/* Machine Syscal call */
		syscall_handler(context->x17, context->x10, context->x11, context->x12, context->x13);
		context->mepc += 4; // Here we update the save EPC to EPC+4, then after return from this exception
                            //   back to the regular code, then it start from the next instruction which caused
                            //   this system call
	} else if (!(mcause & MCAUSE_INT) && ((mcause & MCAUSE_CAUSE) == TRAP_S_ECALL)) {
		/* Supervisor Syscal call */
		sv_syscall_handler(context->x17, context->x10, context->x11, context->x12, context->x13);
		context->mepc += 4;
	} else if (!(mcause & MCAUSE_INT) && ((mcause & MCAUSE_CAUSE) == TRAP_U_ECALL)) {
		/* User Syscal call */
		user_syscall_handler(context->x17, context->x10, context->x11, context->x12, context->x13);
		context->mepc += 4;
	}else {
		/* Unhandled Trap */
		context->mepc = except_handler(mcause, context->mepc, context->caller_regs);
	}
}
