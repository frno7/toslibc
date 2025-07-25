// SPDX-License-Identifier: GPL-2.0

#ifndef TOSLIBC_TOOL_TOOL_H
#define TOSLIBC_TOOL_TOOL_H

extern char progname[];

#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))

#define NORETURN __attribute__((__noreturn__))

#define PRG_HEADER_SIZE 28

#endif /* TOSLIBC_TOOL_TOOL_H */
