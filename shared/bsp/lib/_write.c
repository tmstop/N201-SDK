/* See LICENSE of license details. */

#include <stdint.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>

#include "platform.h"
#include "uart.h"
#include "stub.h"
int _write(int fd, const unsigned char *buf, int size)
{
	extern int outbyte(unsigned char c);

    int i;
    for (i = 0; i < size; i++)
    	outbyte(buf[i]);

    return i;
}

