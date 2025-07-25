// SPDX-License-Identifier: GPL-2.0
/*
 * DRI and GST symbols table
 *
 * Copyright (C) 2025 Fredrik Noring
 */

#ifndef TOSLIBC_TOOL_SYMBOL_H
#define TOSLIBC_TOOL_SYMBOL_H

#include <stdint.h>

#include "tool/elf32.h"
#include "tool/file.h"

uint32_t symbol_size(const struct file *f,
	bool (*section)(Elf_Shdr *shdr, Elf_Ehdr *ehdr));

void append_symbols(struct file *tf, struct file *ef);

#endif /* TOSLIBC_TOOL_SYMBOL_H */
