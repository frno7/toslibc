// SPDX-License-Identifier: MIT
/*
 * This file is based on klibc.
 */

#include <string.h>

void *memmove(void *dst, const void *src, size_t n)
{
	const char *p = src;
	char *q = dst;

	if (q < p) {
		while (n--) {
			*q++ = *p++;
		}
	} else {
		p += n;
		q += n;
		while (n--) {
			*--q = *--p;
		}
	}

	return dst;
}
