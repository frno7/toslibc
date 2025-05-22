// SPDX-License-Identifier: MIT
/*
 * This file is based on klibc.
 */

#include <string.h>

char *strchr(const char *s, int c)
{
	while (*s != (char)c) {
		if (!*s)
			return NULL;
		s++;
	}

	return (char *)s;
}
