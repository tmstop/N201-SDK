#ifndef _STUB_H
#define _STUB_H

#include <stdint.h>
#include <unistd.h>

void write_hex(int fd, unsigned long int hex);

static inline int _stub(int err)
{
  return -1;
}


#endif /* _STUB_H */
