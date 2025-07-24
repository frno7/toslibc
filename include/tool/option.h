// SPDX-License-Identifier: GPL-2.0

#ifndef TOSLIBC_TOOL_OPTION_H
#define TOSLIBC_TOOL_OPTION_H

extern struct option_ {
	const char *input;
	const char *output;
} option;

void parse_options(int argc, char **argv);

#endif /* TOSLIBC_TOOL_OPTION_H */
