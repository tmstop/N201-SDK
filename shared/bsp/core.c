/*
 * Copyright (c) 2018-2019 Nuclei System Technology, Inc
 * All rights reserved.
 *
 */
#include <unistd.h>
#include "platform.h"

uint32_t mtime_lo(void)
{
  return TMR_REG(TMR_MTIME_OFFSET);
}


uint32_t mtime_hi(void)
{
  return TMR_REG(TMR_MTIME_OFFSET + 4);
}

uint64_t get_mtime_value()
{
  while (1) {
    uint32_t hi = mtime_hi();
    uint32_t lo = mtime_lo();
    if (hi == mtime_hi())
      return ((uint64_t)hi << 32) | lo;
  }
}

uint32_t get_tmr_freq()
{
  return TMR_FREQ;
}

uint64_t get_instret_value()
{
  while (1) {
    uint32_t hi = read_csr(minstreth);
    uint32_t lo = read_csr(minstret);
    if (hi == read_csr(minstreth))
      return ((uint64_t)hi << 32) | lo;
  }
}

uint64_t get_cycle_value()
{
  while (1) {
    uint32_t hi = read_csr(mcycleh);
    uint32_t lo = read_csr(mcycle);
    if (hi == read_csr(mcycleh))
      return ((uint64_t)hi << 32) | lo;
  }
}

uint32_t __attribute__((noinline)) measure_cpu_freq(size_t n)
{
  uint32_t start_mtime, delta_mtime;
  uint32_t mtime_freq = get_tmr_freq();

  // Don't start measuruing until we see an mtime tick
  uint32_t tmp = mtime_lo();
  do {
    start_mtime = mtime_lo();
  } while (start_mtime == tmp);

  uint32_t start_mcycle = read_csr(mcycle);

  do {
    delta_mtime = mtime_lo() - start_mtime;
  } while (delta_mtime < n);

  uint32_t delta_mcycle = read_csr(mcycle) - start_mcycle;

  return (delta_mcycle / delta_mtime) * mtime_freq
         + ((delta_mcycle % delta_mtime) * mtime_freq) / delta_mtime;
}

uint32_t get_cpu_freq()
{
  uint32_t cpu_freq;

  // warm up
  measure_cpu_freq(1);
  // measure for real
  cpu_freq = measure_cpu_freq(100);

  return cpu_freq;
}

void stop_mcycle_minstret()
{
  //#define CSR_MCOUNTINHIBIT 0x320
  set_csr(0x320,0x5);
}


void start_mcycle_minstret()
{
  //#define CSR_MCOUNTINHIBIT 0x320
  write_csr(0x320,0x0);
}



  
void eclic_init ( uint32_t num_irq )
{

  typedef volatile uint32_t vuint32_t;

  //clear cfg register 
  ECLIC_REG8(ECLIC_CFG_OFFSET) = 0x0;

  //clear minthresh register 
  ECLIC_REG8(ECLIC_MTH_OFFSET) = 0x0;

  //clear all IP/IE/ATTR/CTRL bits for all interrupt sources
  vuint32_t * ptr;

  vuint32_t * base = (vuint32_t*)(ECLIC_ADDR_BASE + ECLIC_INT_IP_OFFSET);
  vuint32_t * upper = (vuint32_t*)(base + num_irq*4);

  for (ptr = base; ptr < upper; ptr=ptr + 4){
    *ptr = 0;
  }
}


void eclic_enable_interrupt (uint32_t source) {
    ECLIC_REG8((uint32_t)ECLIC_INT_IE_OFFSET + source*4) = 0x1;
}

void eclic_disable_interrupt (uint32_t source){
    ECLIC_REG8(ECLIC_INT_IE_OFFSET + source*4) = 0x0;
}

void eclic_set_pending(uint32_t source){
    ECLIC_REG8(ECLIC_INT_IP_OFFSET + source*4) = 0x1;
}

void eclic_clear_pending(uint32_t source){
    ECLIC_REG8(ECLIC_INT_IP_OFFSET + source*4) = 0x0;
}

void eclic_set_intctrl (uint32_t source, uint8_t intctrl){
    ECLIC_REG8(ECLIC_INT_CTRL_OFFSET + source*4) = intctrl;
}

uint8_t eclic_get_intctrl  (uint32_t source){
  return ECLIC_REG8(ECLIC_INT_CTRL_OFFSET + source*4);
}

void eclic_set_intattr (uint32_t source, uint8_t intattr){
    ECLIC_REG8(ECLIC_INT_ATTR_OFFSET + source*4) = intattr;
}

uint8_t eclic_get_intattr  (uint32_t source){
  return ECLIC_REG8(ECLIC_INT_ATTR_OFFSET + source*4);
}

void eclic_set_cliccfg (uint8_t cliccfg){
    ECLIC_REG8(ECLIC_CFG_OFFSET) = cliccfg;
}

uint8_t eclic_get_cliccfg  (){
  return ECLIC_REG8(ECLIC_CFG_OFFSET);
}

void eclic_set_mth (uint8_t mth){
    ECLIC_REG8(ECLIC_MTH_OFFSET) = mth;
}

uint8_t eclic_get_mth  (){
  return ECLIC_REG8(ECLIC_MTH_OFFSET);
}

//sets nlbits 
void eclic_set_nlbits(uint8_t nlbits) {
  //shift nlbits to correct position
  uint8_t nlbits_shifted = nlbits << ECLIC_CFG_NLBITS_LSB;

  //read the current cliccfg 
  uint8_t old_cliccfg = eclic_get_cliccfg();
  uint8_t new_cliccfg = (old_cliccfg & (~ECLIC_CFG_NLBITS_MASK)) | (ECLIC_CFG_NLBITS_MASK & nlbits_shifted); 

  eclic_set_cliccfg(new_cliccfg);
}

//get nlbits 
uint8_t eclic_get_nlbits() {
  //extract nlbits
  uint8_t nlbits = eclic_get_cliccfg();
  nlbits = (nlbits & ECLIC_CFG_NLBITS_MASK) >> ECLIC_CFG_NLBITS_LSB;
  return nlbits;
}

//sets an interrupt level based encoding of nlbits and ECLICINTCTLBITS
void eclic_set_int_level(uint32_t source, uint8_t level) {
  //extract nlbits
  uint8_t nlbits = eclic_get_nlbits();
  if (nlbits > ECLICINTCTLBITS) {
    nlbits = ECLICINTCTLBITS; 
  }

  //shift level right to mask off unused bits
  level = level >> (8-nlbits);
  //shift level into correct bit position
  level = level << (8-nlbits);
 
  //write to clicintctrl
  uint8_t current_intctrl = eclic_get_intctrl(source);
  //shift intctrl left to mask off unused bits
  current_intctrl = current_intctrl << nlbits;
  //shift intctrl into correct bit position
  current_intctrl = current_intctrl >> nlbits;

  eclic_set_intctrl(source, (current_intctrl | level));

}

//gets an interrupt level based encoding of nlbits
uint8_t eclic_get_int_level(uint32_t source) {
  //extract nlbits
  uint8_t nlbits = eclic_get_nlbits();
  if (nlbits > ECLICINTCTLBITS) {
    nlbits = ECLICINTCTLBITS; 
  }

  uint8_t intctrl = eclic_get_intctrl(source);

  //shift intctrl
  intctrl = intctrl >> (8-nlbits);
  //shift intctrl
  uint8_t level = intctrl << (8-nlbits);

  return level;
}

void eclic_mode_enable() {
  uint32_t mtvec_value = read_csr(mtvec);
  mtvec_value = mtvec_value & 0xFFFFFFC0;
  mtvec_value = mtvec_value | 0x00000003;
  write_csr(mtvec,mtvec_value);
}


//sets an interrupt as edge trigger
void eclic_set_int_posedge_trig(uint32_t source) {
  uint8_t current_intattr = eclic_get_intattr(source);
  current_intattr = current_intattr | 0x00000002;
  current_intattr = current_intattr & (~0x00000004);
  eclic_set_intattr(source, current_intattr);
}

void eclic_set_int_negedge_trig(uint32_t source) {
  uint8_t current_intattr = eclic_get_intattr(source);
  current_intattr = current_intattr | 0x00000002;
  current_intattr = current_intattr | 0x00000004;
  eclic_set_intattr(source, current_intattr);
}

void eclic_set_int_level_trig(uint32_t source) {
  uint8_t current_intattr = eclic_get_intattr(source);
  current_intattr = current_intattr & (~0x00000002);
  eclic_set_intattr(source, current_intattr);
}



