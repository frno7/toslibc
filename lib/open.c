// SPDX-License-Identifier: LGPL-2.1
/*
 * Copyright (C) 2020 Fredrik Noring
 */

#include <errno.h>
#include <fcntl.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <unistd.h>

#include <tos/error.h>
#include <tos/gemdos.h>

#include "internal/macro.h"

#define GEMDOS_FLAG_MASK (0x7)	/* Only bits 2..0 maps to gemdos_fopen. */

static bool invalid_flags(int flags)
{
	const bool wr = (flags & O_WRONLY) || (flags & O_RDWR);

	return ((flags & O_CREAT)  && !wr) ||
	       ((flags & O_APPEND) && !wr);
}

static bool file_exists(const char *path)
{
	struct stat sb;

	return stat(path, &sb) != -1 || errno != ENOENT;
}

int open(const char *pathname, int flags, ...)
{
	int fd;

	if (invalid_flags(flags)) {
		errno = EINVAL;
		return -1;
	}

	if (flags & O_CREAT) {
		fd = gemdos_fopen(pathname, flags & GEMDOS_FLAG_MASK);
		if (fd == -TOS_EFILNF)
			fd = gemdos_fcreate(pathname, 0);
	} else if (flags & O_TRUNC) {
		if (!file_exists(pathname)) {
			errno = ENOENT;
			return -1;
		}
		fd = gemdos_fcreate(pathname, 0);
	} else
		fd = gemdos_fopen(pathname, flags & GEMDOS_FLAG_MASK);

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
