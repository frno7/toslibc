// SPDX-License-Identifier: LGPL-2.1
/*
 * Copyright (C) 2025 Fredrik Noring
 */

#include <errno.h>
#include <tos/error.h>
#include <sys/stat.h>

#include <tos/gemdos.h>

int mkdir(const char *pathname, mode_t mode)
{
	const int32_t r = gemdos_dcreate(pathname);

	if (r < 0) {
		errno = errno_for_tos_error(-r);

		return -1;
	}

	return 0;
}
