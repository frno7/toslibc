// SPDX-License-Identifier: GPL-2.0

#include <stdio.h>
#include <stdlib.h>

#include <tos/gemdos.h>

#include <internal/build-assert.h>
#include <internal/macro.h>

#include <test/suite.h>

#define TEST_DECLARE(identifier)					\
	const char *identifier();
TEST_SUITE(TEST_DECLARE)

static const struct {
	const char *name;
	const char *(*test)();
} suite[] = {
#define TEST_ENTRY(identifier)						\
	{ #identifier, identifier },
TEST_SUITE(TEST_ENTRY)
};

#define SUITE_SIZE (__ARRAY_SIZE(suite))

int main(int argc, char *argv[])
{
	size_t failed = 0;

	BUILD_BUG_ON(SUITE_SIZE != 5);

	printf("TOS/libc %zu test suite\r\n\r\n", SUITE_SIZE);

	for (size_t i = 0; i < SUITE_SIZE; i++) {
		printf("%2zu %-30s", 1 + i, suite[i].name);

		const char *failure = suite[i].test();
		if (!failure) {
			puts("OK\r\n");
			continue;
		}

		failed++;
		puts("FAILED\r\n");
		printf(": %s\r\n", failure);
	}

	if (failed)
		printf("\r\n%zu of %zu failed\r\n", failed, SUITE_SIZE);
	else
		printf("\r\n%zu of %zu succeeded\r\n", SUITE_SIZE, SUITE_SIZE);

	puts("Press enter to exit\r\n");
	gemdos_cnecin();

	return EXIT_SUCCESS;
}
