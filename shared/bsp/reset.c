/*
 * Copyright (c) 2018-2019 Nuclei System Technology, Inc
 * All rights reserved.
 */

#ifdef CFG_GCOV
#include <stdlib.h>
#endif

#include "platform.h"

extern void c_startup(void);
extern void system_init(void);
extern void stop_mcycle_minstret(void);
extern void __libc_init_array(void);
extern void __libc_fini_array(void);

__attribute__((weak)) void reset_handler(void)
{
	extern int main(void);
	eclic_init(70);     //num  70 is n22 have  
  	eclic_mode_enable();

    eclic_set_nlbits(3); // N201 only support 8 levels (3bits wide encoding)

    // Set the internal local interrupts to be as posedge-triggered
    eclic_set_int_posedge_trig(ECLIC_IRQ_ID_BWEI );
       
    // Set the internal local interrupts to have highest levels (3bits encoded value 7)
    eclic_set_int_level(ECLIC_IRQ_ID_BWEI , 7<<(8-3));

    // Enable the internal local interrupts
    eclic_enable_interrupt (ECLIC_IRQ_ID_BWEI );
    
	/*
	 * Initialize LMA/VMA sections.
	 * Relocation for any sections that need to be copied from LMA to VMA.
	 */
	c_startup();

	/* Call platform specific hardware initialization */
	
	system_init();

    /* Add the cycle/instret disable by default to save power, only use them when needed to measure the cycle/instret */
//	stop_mcycle_minstret();
	
	/* Do global constructors */
	__libc_init_array();
	
#ifdef CFG_GCOV
	/* Register global destructors to be called upon exit */
	atexit(__libc_fini_array);

	/* Entry function */
	exit(main());
#else
	/* Entry function */
	main();
#endif
}

/*
 * When compiling C++ code with static objects, the compiler inserts
 * a call to __cxa_atexit() with __dso_handle as one of the arguments.
 * The dummy versions of these symbols should be provided.
 */
void __cxa_atexit(void (*arg1)(void*), void* arg2, void* arg3)
{
}

void*   __dso_handle = (void*) &__dso_handle;
