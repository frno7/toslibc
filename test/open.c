// SPDX-License-Identifier: GPL-2.0

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

#include "test/test.h"

#define TEST_TEXT_FILE "test.txt"

static off_t tell(int fd)
{
	return lseek(fd, 0, SEEK_CUR);
}

const char *test_open_rdonly()
{
	int fd = open(TEST_TEXT_FILE, O_RDONLY);
	if (fd == -1)
		return test_error("open(\"%s\", O_RDONLY) errno %d\n",
				TEST_TEXT_FILE, errno);

	const off_t offset = tell(fd);
	if (offset != 0)
		return test_error("tell(%d) = %zd != 0 errno %d\n",
				fd, offset, errno);

	if (close(fd))
		return test_error("close(%d) errno %d\n", fd, errno);

	return NULL;
}

const char *test_open_wronly_append()
{
	int fd = open(TEST_TEXT_FILE, O_WRONLY | O_APPEND);
	if (fd == -1)
		return test_error("open(\"%s\", O_WRONLY | O_APPEND) errno %d\n",
				TEST_TEXT_FILE, errno);

	const off_t offset = tell(fd);
	if (offset != 34)
		return test_error("tell(%d) = %zd != 35 errno %d\n",
				fd, offset, errno);

	if (close(fd))
		return test_error("close(%d) errno %d\n", fd, errno);

	return NULL;
}
