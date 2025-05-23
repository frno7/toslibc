// SPDX-License-Identifier: GPL-2.0

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>

#include <test/test.h>

#define TEST_TEXT_FILE  "test.txt"
#define TEST_NOENT_FILE "noent.tmp"
#define TEST_CUR_DIR    "."

const char *test_stat_noent()
{
	struct stat sb;

	if (stat(TEST_NOENT_FILE, &sb) != -1)
		return test_error("stat(\"%s\", &sb)\n", TEST_NOENT_FILE);

	if (errno != ENOENT)
		return test_error("errno = %d != %d\n", errno, ENOENT);

	return NULL;
}

const char *test_stat_reg()
{
	struct stat sb;

	if (stat(TEST_TEXT_FILE, &sb) == -1)
		return test_error("stat(\"%s\", &sb) errno %d\n",
				TEST_TEXT_FILE, errno);

	if ( S_ISSOCK(sb.st_mode) ||
	     S_ISLNK(sb.st_mode)  ||
	    !S_ISREG(sb.st_mode)  ||
	     S_ISBLK(sb.st_mode)  ||
	     S_ISDIR(sb.st_mode)  ||
	     S_ISCHR(sb.st_mode)  ||
	     S_ISFIFO(sb.st_mode))
		return test_error("st_mode = %x != %x\n", sb.st_mode, S_IFREG);

	if (sb.st_size != 34)
		return test_error("st_size = %zd != 34\n", sb.st_size);

	return NULL;
}

const char *test_stat_dir()
{
	struct stat sb;

	if (stat(TEST_CUR_DIR, &sb) == -1)
		return test_error("stat(\"%s\", &sb) errno %d\n",
				TEST_CUR_DIR, errno);

	if ( S_ISSOCK(sb.st_mode) ||
	     S_ISLNK(sb.st_mode)  ||
	     S_ISREG(sb.st_mode)  ||
	     S_ISBLK(sb.st_mode)  ||
	    !S_ISDIR(sb.st_mode)  ||
	     S_ISCHR(sb.st_mode)  ||
	     S_ISFIFO(sb.st_mode))
		return test_error("st_mode = %x != %x\n", sb.st_mode, S_IFDIR);

	return NULL;
}
