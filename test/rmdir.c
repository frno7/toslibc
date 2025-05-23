// SPDX-License-Identifier: GPL-2.0

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

#include "test/file.h"
#include "test/test.h"

#define TEST_RMDIR_DIR "rmdir.tmp"

const char *test_rmdir()
{
	if (file_exists(TEST_RMDIR_DIR))
		return TEST_RMDIR_DIR " already exists";

	if (mkdir(TEST_RMDIR_DIR, 0777) == -1)
		return test_error("mkdir(\"%s\", 0777) errno %d\n",
				TEST_RMDIR_DIR, errno);

	if (!file_exists(TEST_RMDIR_DIR))
		return TEST_RMDIR_DIR " does not exist";

	if (rmdir(TEST_RMDIR_DIR) == -1)
		return test_error("rmdir(\"%s\") errno %d\n",
				TEST_RMDIR_DIR, errno);

	if (file_exists(TEST_RMDIR_DIR))
		return TEST_RMDIR_DIR " not rmdired";

	if (rmdir(TEST_RMDIR_DIR) != -1 || errno != ENOENT)
		return test_error("rmdir(\"%s\") errno %d\n",
				TEST_RMDIR_DIR, errno);

	return NULL;
}
