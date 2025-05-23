// SPDX-License-Identifier: GPL-2.0

#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>

#include <test/test.h>

#define TEST_WRITE_FILE "fwrite.tmp"

static bool file_exists(const char *path)
{
	struct stat sb;

	if (stat(path, &sb) == -1 && errno == ENOENT)
		return false;

	return true;
}

const char *test_fwrite()
{
	if (file_exists(TEST_WRITE_FILE))
		return "" TEST_WRITE_FILE " already exists";

	FILE *file = fopen(TEST_WRITE_FILE, "w");
	if (!file)
		return test_error("fopen(\"%s\", \"w\") errno %d\n",
				TEST_WRITE_FILE, errno);

	const size_t w = fwrite("foobar", 2, 3, file);
	if (w != 3)
		return test_error("fwrite(...) = %zd != 3 errno %d\n", w, errno);

	const off_t offset = ftell(file);
	if (offset != 6)
		return test_error("ftell(...) = %zd != 6 errno %d\n",
				offset, errno);

	const int err = fseek(file, 0, SEEK_SET);
	if (err == -1)
		return test_error("fseek(...) errno %d\n", errno);

	char buf[strlen("foobar") + 1] = { };

	const size_t r = fread(buf, 3, 2, file);
	if (r != 2)
		return test_error("fread(...) = %zd != 2 errno %d\n", r, errno);

	if (fclose(file))
		return test_error("fclose(...) errno %d\n", errno);

	if (unlink(TEST_WRITE_FILE) == -1)
		return test_error("unlink(\"%s\") errno %d\n",
				TEST_WRITE_FILE, errno);

	if (file_exists(TEST_WRITE_FILE))
		return "" TEST_WRITE_FILE " not unlinked";

	if (strcmp(buf, "foobar") != 0)
		return "strcmp(...) != 0";

	return NULL;
}
