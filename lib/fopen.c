// SPDX-License-Identifier: LGPL-2.1
/*
 * Copyright (C) 2025 Fredrik Noring
 */

#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>

#include "internal/macro.h"
#include "internal/build-assert.h"

static const char *mode_skip(const char *mode)
{
	while (*mode && !strchr("rwa+", *mode))
		mode++;		/* Ignore characters such as "bcemx". */

	return mode;
}

static void mode_skip2(const char **a, const char **b)
{
	*a = mode_skip(*a);
	*b = mode_skip(*b);
}

static bool equivalent_modes(const char *a, const char *b)
{
	mode_skip2(&a, &b);

	while (*a && *a == *b) {
		a++;
		b++;

		mode_skip2(&a, &b);
	}

	return !*a && !*b;
}

int __fopen_mode_flags(const char *mode)
{
	static const struct {
		const char *mode;
		int flags;
	} mode_flags[] = {
		{ "r",  O_RDONLY                      },
		{ "w",  O_WRONLY | O_CREAT | O_TRUNC  },
		{ "a",  O_WRONLY | O_CREAT | O_APPEND },
		{ "r+", O_RDWR                        },
		{ "w+", O_RDWR   | O_CREAT | O_TRUNC  },
		{ "a+", O_RDWR   | O_CREAT | O_APPEND },
	};

	for (size_t i = 0; i < __ARRAY_SIZE(mode_flags); i++)
		if (equivalent_modes(mode, mode_flags[i].mode))
			return mode_flags[i].flags;

	errno = EINVAL;

	return -1;
}

FILE *fopen(const char *pathname, const char *mode)
{
	const int flags = __fopen_mode_flags(mode);
	if (flags == -1)
		return NULL;

	const int fd = open(pathname, flags);
	if (fd == -1)
		return NULL;

	/*
	 * Cast the file descriptor (fd) integer to a pseudo-pointer,
	 * as the size of the pointer is at least as great as the integer.
	 * For the Atari ST, the pointer is 32 bits, and the integer is
	 * either 16 or 32 bits; however TOS/libc requires 32-bit integers.
	 * This avoids mallocing or otherwise storing the fd only to have
	 * a FILE pointer to it.
	 */
	BUILD_BUG_ON(sizeof(FILE *) < sizeof(fd));
	return (FILE *)fd;
}
