// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (C) 2025 Fredrik Noring
 */

#ifndef TOSLIBC_TOOL_RELOCATION_H
#define TOSLIBC_TOOL_RELOCATION_H

#include "file.h"

size_t append_relocations_text_data(struct file *tf, struct file *ef);

#endif /* TOSLIBC_TOOL_RELOCATION_H */
