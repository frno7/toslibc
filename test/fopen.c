// SPDX-License-Identifier: GPL-2.0

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

#include "internal/macro.h"

#include "test/test.h"

#define TEST_TEXT_FILE "test.txt"

const char *test___fopen_mode_flags()
{
	static const struct {
		const char *mode;
		int flags;
	} mode_flags[] = {
		/*
		 * Test that meaningless modes fail.
		 */

		{ "",    -1                            },
		{ "b",   -1                            },
		{ "rw",  -1                            },
		{ "ra",  -1                            },
		{ "wa",  -1                            },
		{ "ar",  -1                            },
		{ "aw",  -1                            },
		{ "+r",  -1                            },
		{ "+w",  -1                            },
		{ "+a",  -1                            },

		/*
		 * Test that all characters except "arw+" are ignored,
		 * and that the proper modes are mapped to flags correctly.
		 */

		{ "r",   O_RDONLY                      },
		{ "br",  O_RDONLY                      },
		{ "rb",  O_RDONLY                      },

		{ "w",   O_WRONLY | O_CREAT | O_TRUNC  },
		{ "bw",  O_WRONLY | O_CREAT | O_TRUNC  },
		{ "wb",  O_WRONLY | O_CREAT | O_TRUNC  },

		{ "a",   O_WRONLY | O_CREAT | O_APPEND },
		{ "ba",  O_WRONLY | O_CREAT | O_APPEND },
		{ "ab",  O_WRONLY | O_CREAT | O_APPEND },

		{ "r+",  O_RDWR                        },
		{ "br+", O_RDWR                        },
		{ "rb+", O_RDWR                        },
		{ "r+b", O_RDWR                        },

		{ "w+",  O_RDWR   | O_CREAT | O_TRUNC  },

		{ "a+",  O_RDWR   | O_CREAT | O_APPEND },
	};

	for (size_t i = 0; i < __ARRAY_SIZE(mode_flags); i++) {
		const int flags = __fopen_mode_flags(mode_flags[i].mode);

		if (flags != mode_flags[i].flags)
			return test_error("mode \"%s\" = %x != %x",
				mode_flags[i].mode, flags, mode_flags[i].flags);
	}

	return NULL;
}

const char *test_fopen_r()
{
	FILE *file = fopen(TEST_TEXT_FILE, "r");
	if (!file)
		return test_error("fopen(\"%s\", \"r\") errno %d\n",
				TEST_TEXT_FILE, errno);

	const off_t offset = ftell(file);
	if (offset != 0)
		return test_error("ftell(...) = %zd != 0 errno %d\n",
				offset, errno);

	if (fclose(file))
		return test_error("fclose(...) errno %d\n", errno);

	return NULL;
}

const char *test_fopen_a()
{
	FILE *file = fopen(TEST_TEXT_FILE, "a");
	if (!file)
		return test_error("fopen(\"%s\", \"a\") errno %d\n",
				TEST_TEXT_FILE, errno);

	const off_t offset = ftell(file);
	if (offset != 34)
		return test_error("ftell(...) = %zd != 35 errno %d\n",
				offset, errno);

	if (fclose(file))
		return test_error("fclose(...) errno %d\n", errno);

	return NULL;
}
