// SPDX-License-Identifier: GPL-2.0

#ifndef TEST_FILE_H
#define TEST_FILE_H

#include <errno.h>
#include <sys/stat.h>

#include "internal/types.h"

static inline bool file_exists(const char *path)
{
	struct stat sb;

	return stat(path, &sb) != -1 || errno != ENOENT;
}

#endif /* TEST_FILE_H */
