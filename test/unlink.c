// SPDX-License-Identifier: GPL-2.0

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

#include "test/file.h"
#include "test/test.h"

#define TEST_UNLINK_FILE "unlink.tmp"

const char *test_unlink()
{
	if (file_exists(TEST_UNLINK_FILE))
		return TEST_UNLINK_FILE " already exists";

	int fd = open(TEST_UNLINK_FILE, O_CREAT | O_WRONLY);
	if (fd == -1)
		return test_error("open(\"%s\", O_CREAT | O_WRONLY) errno %d\n",
				TEST_UNLINK_FILE, errno);

	if (close(fd))
		return test_error("close(%d) errno %d\n", fd, errno);

	if (!file_exists(TEST_UNLINK_FILE))
		return TEST_UNLINK_FILE " does not exist";

	if (unlink(TEST_UNLINK_FILE) == -1)
		return test_error("unlink(\"%s\") errno %d\n",
				TEST_UNLINK_FILE, errno);

	if (file_exists(TEST_UNLINK_FILE))
		return TEST_UNLINK_FILE " not unlinked";

	if (unlink(TEST_UNLINK_FILE) != -1 || errno != ENOENT)
		return test_error("unlink(\"%s\") errno %d\n",
				TEST_UNLINK_FILE, errno);

	return NULL;
}
