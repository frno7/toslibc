// SPDX-License-Identifier: GPL-2.0

#include <string.h>

#include <test/test.h>
#include <internal/macro.h>

#define TEST_STRCHR(s, c, expect)					\
({									\
	char *result = strchr(s, c);					\
	if (result != (expect < 0 ? NULL : &s[expect]))			\
		return test_error("strchr(\"%s\", '%c') = \"%s\" != \"%s\"",\
				s, c, result, &s[expect]);		\
})

const char *test_strchr()
{
	TEST_STRCHR("",    '.', -1);
	TEST_STRCHR("foo", 'o',  1);
	TEST_STRCHR("foo", 'x', -1);

	return NULL;
}
