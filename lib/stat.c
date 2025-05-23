// SPDX-License-Identifier: LGPL-2.1
/*
 * Copyright (C) 2020 Fredrik Noring
 */

#include <errno.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

#include <tos/error.h>
#include <tos/gemdos.h>

#include "internal/macro.h"

static off_t filesize(const char *pathname)
{
	const int fd = open(pathname, O_RDONLY);
	if (fd == -1)
		return -1;

	const off_t size = lseek(fd, 0, SEEK_END);
	if (size == -1) {
		preserve (errno) {
			close(fd);
		}
		return -1;
	}

	if (close(fd))
		return -1;

	return size;
}

int stat(const char *pathname, struct stat *statbuf)
{
	const int32_t attr = gemdos_fattrib(pathname, FA_INQUIRE, 0);
	if (attr < 0) {
		errno = errno_for_tos_error(-attr);

		return -1;
	}

	const off_t size = FA_ISDIR(attr) ? 0 : filesize(pathname);
	if (size == -1)
		return -1;

	*statbuf = (struct stat) {
		.st_mode = FA_ISDIR(attr) ? S_IFDIR : S_IFREG,
		.st_size = size,
	};

	return 0;
}
