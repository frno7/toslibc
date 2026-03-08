// SPDX-License-Identifier: GPL-2.0

#ifndef TOSLIBC_TOOL_PRINT_H
#define TOSLIBC_TOOL_PRINT_H

#include <stdarg.h>

#include "internal/macro.h"

#include "tool/tool.h"

void pr_info(const char *fmt, ...);

void pr_warn(const char *msg, ...);

void pr_error(const char *msg, ...);

void __NORETURN pr_fatal_error(const char *fmt, ...);

void __NORETURN pr_fatal_errno(const char *fmt, ...);

#endif /* TOSLIBC_TOOL_PRINT_H */
