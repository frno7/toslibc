// SPDX-License-Identifier: LGPL-2.1
/*
 * Copyright (C) 2020 Fredrik Noring
 */

#include <errno.h>
#include <fcntl.h>

#include <tos/error.h>

#include <tos/gemdos.h>

int open(const char *pathname, int flags, ...)
{
	const int f = flags & ~(O_CREAT | O_TRUNC);
	const int32_t fd = (f == flags ?
		gemdos_fopen(pathname, f) :
		gemdos_fcreate(pathname, 0));

	if (fd < 0) {
		errno = errno_for_tos_error(-fd);

		return -1;
	}

	return fd;
}
