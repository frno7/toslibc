// SPDX-License-Identifier: LGPL-2.1
/*
 * Copyright (C) 2025 Fredrik Noring
 */

#include <errno.h>
#include <tos/error.h>
#include <unistd.h>

#include <tos/gemdos.h>

int rmdir(const char *pathname)
{
	const int32_t r = gemdos_ddelete(pathname);

	if (r < 0) {
		errno = errno_for_tos_error(-r);

		return -1;
	}

	return r;
}
