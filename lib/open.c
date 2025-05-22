// SPDX-License-Identifier: LGPL-2.1
/*
 * Copyright (C) 2020 Fredrik Noring
 */

#include <errno.h>
#include <fcntl.h>
#include <unistd.h>

#include <tos/error.h>

#include <tos/gemdos.h>

#include "internal/macro.h"

#define GEMDOS_FLAG_MASK (0x7)	/* Only bits 2..0 maps to gemdos_fopen. */

int open(const char *pathname, int flags, ...)
{
	const int32_t fd = (flags & O_TRUNC) ?
		gemdos_fcreate(pathname, 0) :
		gemdos_fopen(pathname, flags & GEMDOS_FLAG_MASK);

	if (fd < 0) {
		errno = errno_for_tos_error(-fd);

		return -1;
	}

	if ((flags & O_APPEND) && lseek(fd, 0, SEEK_END) == -1) {
		preserve (errno) {
			close(fd);
		}
		return -1;
	}

	return fd;
}
