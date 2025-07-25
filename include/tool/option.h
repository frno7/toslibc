// SPDX-License-Identifier: GPL-2.0

#ifndef TOSLIBC_TOOL_OPTION_H
#define TOSLIBC_TOOL_OPTION_H

#include <stdbool.h>

extern struct option_ {
	const char *input;
	const char *output;
	bool strip;
} option;

static inline bool option_symbols()
{
	return !option.strip;
}

void parse_options(int argc, char **argv);

#endif /* TOSLIBC_TOOL_OPTION_H */
