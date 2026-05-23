// SPDX-License-Identifier: LGPL-2.1
/*
 * Copyright (C) 2020 Fredrik Noring
 */

#include <stdio.h>
#include <string.h>
#include <unistd.h>

int puts(const char *s)
{
	const size_t length = strlen(s);
	const ssize_t ws = write(STDOUT_FILENO, s, length);
	const ssize_t wn = write(STDOUT_FILENO, "\r\n", 2);

	return ws + wn == length + 2 ? ws + wn : EOF;
}
