// SPDX-License-Identifier: GPL-2.0

#include <string.h>

#include "test/test.h"

#define TEST_MEMMOVE(s, src, dst, len, expect)				\
({									\
	char buf[] = s;							\
	void *result = memmove(&buf[dst], &buf[src], len);		\
	if (result != &buf[dst] || strcmp(buf, expect) != 0)		\
		return test_error("memmove(\"%s\"[%d], \"%s\"[%d], %d) = \"%s\" != \"%s\"",\
				&buf[dst], dst, &s[src], src, len, buf, expect);\
})

const char *test_memmove()
{
	TEST_MEMMOVE("abcdefghij", 0, 2, 4, "ababcdghij");
	TEST_MEMMOVE("abcdefghij", 2, 0, 4, "cdefefghij");

	return NULL;
}
