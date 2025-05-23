// SPDX-License-Identifier: GPL-2.0

#include <string.h>

#include "test/test.h"

static int sign(int n)
{
	return !n ? 0 : n > 0 ? 1 : -1;
}

#define TEST_STRCMP(s1, s2, expect)					\
({									\
	int result = strcmp(s1, s2);					\
	if (sign(result) != sign(expect))				\
		return test_error("strcmp(\"%s\", \"%s\") = %d != %d",	\
				s1, s2, sign(result), sign(expect));	\
})

const char *test_strcmp()
{
	TEST_STRCMP("foo", "foo",  0);
	TEST_STRCMP("foo", "bar",  1);
	TEST_STRCMP("bar", "foo", -1);

	return NULL;
}
