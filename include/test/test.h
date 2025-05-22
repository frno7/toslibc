// SPDX-License-Identifier: GPL-2.0

#ifndef TEST_TEST_H
#define TEST_TEST_H

#include <stdio.h>

#include <internal/types.h>

#define test_error(format, ...)						\
({									\
	static char msg[256];						\
	snprintf(msg, sizeof(msg), format, __VA_ARGS__);		\
	msg;								\
})

#endif /* TEST_TEST_H */
