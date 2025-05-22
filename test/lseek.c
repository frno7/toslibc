// SPDX-License-Identifier: GPL-2.0

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <test/test.h>
#include <internal/macro.h>

#define TEST_TEXT_FILE "test.txt"

static const char *test_seek(int fd, off_t offset, int whence,
	off_t total, const char *expect)
{
	char buf[strlen(expect) + 1] = { };

	const off_t off = lseek(fd, offset, whence);
	if (off == (off_t)-1)
		return test_error("lseek(%d, %zd, %d) = %zd",
			fd, offset, whence, off);

	if (off != total)
		return test_error("lseek(%d, %zd, %d) = %zd != %zd total",
			fd, offset, whence, off, total);

	ssize_t r = read(fd, buf, __ARRAY_SIZE(buf) - 1);
	if (r != __ARRAY_SIZE(buf) - 1)
		return test_error("read(%d, \"%s\", %zd) != %zd",
			fd, buf, __ARRAY_SIZE(buf) - 1, r);

	if (strcmp(expect, buf) != 0)
		return test_error("strcmp(\"%s\", \"%s\") failed", expect, buf);

	return 0;
}

#define TEST_SEEK(fd, offset, whence, total, expect)			\
({									\
	const char *failure = test_seek(fd, offset, whence, total, expect);\
	if (failure)							\
		return failure;						\
})

const char *test_lseek()
{
	int fd = open(TEST_TEXT_FILE, O_RDONLY);
	if (fd == -1)
		return test_error("open(\"%s\", O_RDONLY) errno %d\n",
				TEST_TEXT_FILE, errno);

	TEST_SEEK(fd,  2, SEEK_SET,  2, "dazzling");
	TEST_SEEK(fd,  1, SEEK_CUR, 11, "demonstration");
	TEST_SEEK(fd, -6, SEEK_END, 28, "text");

	if (close(fd) != 0)
		return test_error("close(%d) errno %d\n", fd, errno);

	return NULL;
}
