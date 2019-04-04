
/* The functions in this file are only meant to support Dhrystone on an
 * embedded RV32 system and are obviously incorrect in general. */


#include "core.h"
#include "dhry.h"
long csr_cycle(void)
{
  return get_cycle_value();
}

long csr_instret(void)
{
  return get_instret_value();
}

long time(void)
{
  return get_mtime_value() / get_tmr_freq();
}

