// SPDX-License-Identifier: GPL-2.0

#include <errno.h>
#include <stdio.h>
#include <string.h>

#include "test/test.h"

#define TEST_TEXT_FILE "test.txt"

const char *test_fread()
{
	FILE *file = fopen(TEST_TEXT_FILE, "r");
	if (!file)
		return test_error("fopen(\"%s\", \"r\") errno %d\n",
				TEST_TEXT_FILE, errno);

	char buf[2 * 19] = { };

	const size_t r = fread(buf, 2, 19, file);
	if (r != 17)
		return test_error("fread(...) = %zd != 17 errno %d\n", r, errno);

	if (fclose(file))
		return test_error("fclose(...) errno %d\n", errno);

	if (strcmp(buf, "A dazzling demonstration of text.\n") != 0)
		return "strcmp(...) != 0";

	return NULL;
}
