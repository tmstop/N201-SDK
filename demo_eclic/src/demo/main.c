/*
 * Copyright (c) 2018-2019 Nuclei System Technology, Inc
 * All rights reserved.
 *
 */

/*
 * This demo program shows how to active machine timer and Button (by GPIO) related interrupts (through ECLIC).
 *
 * - The Machine Mtime interrupt handler:
 *   The machine mtime interrupt period is set to 0.5 seconds. If the interrupt
 *   happens then the handler will:
 *   -- Print out the message to remind it is in the mtime handler.
 *   -- Invert one GPIO output value, which is connected to a LED light on the reference FPGA board.
 *
 * - The Button handler:
 *   The Button interrupt will be triggered if the GPIO input value is high. If the interrupt
 *   happens then the handler will:
 *   -- Print out the message to remind it is in the GPIO handler.
 *   -- Invert one GPIO output value, which is connected to a LED light on the reference FPGA board.
 *
 * - The main function:
 *   The main function will firstly print out some message, and then wait the user to input some characters,
 *   and then enable the machine timer interrupt and GPIO interrupts, lastly the it will enter into a while(1)
 *   endless loop.
 *   Note: the different interrupts are set with different leves, and the interrupt
 *   with higher level can preempt interrupt with lower level. The level order is: 
 *   Button2 > Button1 > Machine Timer
 *
 *
 */

#include <stdio.h>
#include <unistd.h>
#include "platform.h"
#include "stdatomic.h"


#define RED_LED_GPIO_ID   22
#define GREEN_LED_GPIO_ID 19
#define BLUE_LED_GPIO_ID  21
#define BUTTON_1_GPIO_ID  30
#define BUTTON_2_GPIO_ID  31
#define BITBANG_GPIO_ID   13

#define BUTTON_1_HANDLER eclic_irq56_handler // the gpio30_irq_handler is the handler named registered in the bsp/interrupts.c
#define BUTTON_2_HANDLER eclic_irq57_handler // the gpio31_irq_handler is the handler named registered in the bsp/interrupts.c

#define ECLIC_IRQ_ID_MTIP 7
#define ECLIC_IRQ_ID_DEVICE_BUTTON_1 56
#define ECLIC_IRQ_ID_DEVICE_BUTTON_2 57

void wait_seconds(unsigned long n)
{
  unsigned long start_mtime, delta_mtime;
  // Don't start measuruing until we see an mtime tick
  unsigned long tmp = mtime_lo();
  do {
    start_mtime = mtime_lo();
  } while (start_mtime == tmp);

  do {
    delta_mtime = mtime_lo() - start_mtime;
  } while (delta_mtime < (n * TMR_FREQ));

  printf("-----------------Waited %ld seconds.\n", n);
}

/*Entry Point for Machine Timer Interrupt Handler*/
void eclic_mtip_handler(){

  printf ("%s","Begin mtime handler\n");
  GPIO_REG(GPIO_OUTPUT_VAL) ^= (0x1 << RED_LED_GPIO_ID);
 //GPIO_REG(GPIO_OUTPUT_VAL) ^=   (0x1 << BLUE_LED_GPIO_ID) ;
  uint64_t now = TMR_REG64(TMR_MTIME_OFFSET);
  uint64_t then = now + 0.5 * TMR_FREQ;// Here we set 1 second, but we need to wait 5 cycles to get out from this handler, so the blink will not toggle as 1 cycle
  TMR_REG64(TMR_MTIMECMP_OFFSET) = then;
  wait_seconds(5);// Wait for a while
  
  printf ("%s","End mtime handler\n");

}

static void _putc(char c) {
  while ((int32_t) UART0_REG(UART_REG_TXFIFO) < 0);
  UART0_REG(UART_REG_TXFIFO) = c;
}
int _getc(char * c){
  int32_t val = (int32_t) UART0_REG(UART_REG_RXFIFO);
  if (val > 0) {
    *c =  val & 0xFF;
    return 1;
  }
  return 0;
}
char * read_instructions_msg= " \
\n\
 ";
const char * printf_instructions_msg= " \
\n\
\n\
\n\
\n\
This is printf function printed:  \n\
\n\
!! Here We Go, Nuclei N201 RISC-V Core !! \n\
\n\
     ######    ###    #####   #####          #     #\n\
     #     #    #    #     # #     #         #     #\n\
     #     #    #    #       #               #     #\n\
     ######     #     #####  #        #####  #     #\n\
     #   #      #          # #                #   #\n\
     #    #     #    #     # #     #           # #\n\
     #     #   ###    #####   #####             #\n\
\n\
 ";

void BUTTON_1_HANDLER(void) {

  printf ("%s","----Begin button1 handler\n");

  // Green LED On
  GPIO_REG(GPIO_OUTPUT_VAL) ^= (1 << GREEN_LED_GPIO_ID);

  // Clear the GPIO Pending interrupt by writing 1.
  wait_seconds(5);// Wait for a while
  GPIO_REG(GPIO_RISE_IP) = (0x1 << BUTTON_1_GPIO_ID);

  printf ("%s","----End button1 handler\n");
};


void BUTTON_2_HANDLER(void) {

  printf ("%s","--------Begin button2 handler\n");

  // Blue LED On
  GPIO_REG(GPIO_OUTPUT_VAL) ^= (1 << BLUE_LED_GPIO_ID);
  wait_seconds(5);// Wait for a while
  GPIO_REG(GPIO_RISE_IP) = (0x1 << BUTTON_2_GPIO_ID);

  printf ("%s","--------End button2 handler\n");

};

void config_eclic_irqs (){

  // Have to enable the interrupt at the ECLIC level.
  eclic_enable_interrupt (ECLIC_IRQ_ID_MTIP);
  eclic_enable_interrupt (ECLIC_IRQ_ID_DEVICE_BUTTON_1);
  eclic_enable_interrupt (ECLIC_IRQ_ID_DEVICE_BUTTON_2);

  eclic_set_nlbits(3); // N201 only support 8 levels (3bits wide encoding)
  //  The level order is: Button2 > Button1 > Machine Timer
  eclic_set_int_level(ECLIC_IRQ_ID_MTIP           , 1<<(8-3));
  eclic_set_int_level(ECLIC_IRQ_ID_DEVICE_BUTTON_1, 2<<(8-3));
  eclic_set_int_level(ECLIC_IRQ_ID_DEVICE_BUTTON_2, 3<<(8-3));

 } 


void setup_mtime (){
 
    // Set the machine timer to go off in 2 seconds.
  uint64_t  now =TMR_REG64(TMR_MTIME_OFFSET);// (uint64_t*) (TMR_ADDR_BASE + 0);//TMR_REG64(TMR_MTIME_OFFSET);
  uint64_t then = now + 2 * TMR_FREQ;
  TMR_REG64(TMR_MTIMECMP_OFFSET) = then;
}

int main(void)
{
  
  // Set up the GPIOs such that the Button GPIO can be used as Inputs (raising edge sensitive)
  GPIO_REG(GPIO_OUTPUT_EN)  &= ~((0x1 << BUTTON_1_GPIO_ID) | (0x1 << BUTTON_2_GPIO_ID));
  GPIO_REG(GPIO_PULLUP_EN)  &= ~((0x1 << BUTTON_1_GPIO_ID) | (0x1 << BUTTON_2_GPIO_ID));
  GPIO_REG(GPIO_INPUT_EN)   |=  ((0x1 << BUTTON_1_GPIO_ID) | (0x1 << BUTTON_2_GPIO_ID));
  GPIO_REG(GPIO_RISE_IE) |= (1 << BUTTON_1_GPIO_ID);
  GPIO_REG(GPIO_RISE_IE) |= (1 << BUTTON_2_GPIO_ID);


  // Set up the GPIOs such that the LED GPIO can be used as Outputs
  GPIO_REG(GPIO_INPUT_EN)    &= ~((0x1<< RED_LED_GPIO_ID) | (0x1<< GREEN_LED_GPIO_ID) | (0x1 << BLUE_LED_GPIO_ID)) ;
  GPIO_REG(GPIO_OUTPUT_EN)   |=  ((0x1<< RED_LED_GPIO_ID)| (0x1<< GREEN_LED_GPIO_ID) | (0x1 << BLUE_LED_GPIO_ID)) ;
  GPIO_REG(GPIO_OUTPUT_VAL)  |=   (0x1 << RED_LED_GPIO_ID) ;
  GPIO_REG(GPIO_OUTPUT_VAL)  &=  ~((0x1<< BLUE_LED_GPIO_ID) | (0x1<< GREEN_LED_GPIO_ID)) ;

  
   //Print the message
  printf ("%s",printf_instructions_msg);
 

  printf ("%s","\nPlease enter any letter from keyboard to continue!\n");

  char c;
  // Check for user input
  while(1){
    if (_getc(&c) != 0){
      
       printf ("%s","I got an input, it is\n\r");
       break;
    }
  }
  _putc(c);
  printf ("\n\r");
  printf ("%s","\nThank you for supporting Nuclei-N201, you will see the blink soon on the board!\n");

 
  config_eclic_irqs();
  setup_mtime();
  
  // Enable interrupts in general.
  set_csr(mstatus, MSTATUS_MIE);

  // For Bit-banging 
  
  uint32_t bitbang_mask = 0;
  bitbang_mask = (1 << BITBANG_GPIO_ID);

  while (1){
    // For Bit-banging with Atomics demo since the A extension is supported at N201.
//    GPIO_REG(GPIO_OUTPUT_VAL) ^=   (0x1 << BLUE_LED_GPIO_ID) ;
    
    atomic_fetch_xor_explicit(&GPIO_REG(GPIO_OUTPUT_VAL), bitbang_mask, memory_order_relaxed);
   
  }

  return 0;

}
