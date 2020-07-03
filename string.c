/* string.c
 * Heiher <r@hev.cc>
 */

#include "string.h"

int
ultostr (char *str, unsigned long val)
{
	unsigned int i;

	for (i=0; i<16; i++) {
		unsigned int v = val & 0xfu;

		str[15 - i] = '0' + ((9 < v) ? (v + 7) : v);
		val >>= 4;
	}
	str[16] = '\0';

	return 0;
}

void *
memcpy (void *dst, void *src, unsigned long size)
{
	unsigned long i;

	for (i = 0; i < size; i ++)
		*(unsigned char *)(dst + i) = *(unsigned char *)(src + i);
}
