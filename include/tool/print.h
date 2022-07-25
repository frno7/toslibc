// SPDX-License-Identifier: GPL-2.0

#ifndef TOOL_PRINT_H
#define TOOL_PRINT_H

#include <stdarg.h>

#include "tool/tool.h"

void pr_info(const char *fmt, ...);

void pr_warn(const char *msg, ...);

void NORETURN pr_fatal_error(const char *fmt, ...);

void NORETURN pr_fatal_errno(const char *fmt, ...);

#endif /* TOOL_PRINT_H */
