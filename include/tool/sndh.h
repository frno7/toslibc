// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (C) 2025 Fredrik Noring
 */

#ifndef TOSLIBC_TOOL_SNDH_H
#define TOSLIBC_TOOL_SNDH_H

void link_sndh(struct file *tf, struct file *ef);

bool has_sndh_extension(const char *path);

#endif /* TOSLIBC_TOOL_SNDH_H */
