// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (C) 2025 Fredrik Noring
 */

#ifndef TOSLIBC_TOOL_PROGRAM_H
#define TOSLIBC_TOOL_PROGRAM_H

#include <stdint.h>

struct program {
	struct program_header {
		size_t size;
	} header;

	size_t size;
};

void link_program(struct file *tf, struct file *ef);

#endif /* TOSLIBC_TOOL_PROGRAM_H */
