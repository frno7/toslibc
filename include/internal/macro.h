// SPDX-License-Identifier: GPL-2.0

#ifndef INTERNAL_MACRO_H
#define INTERNAL_MACRO_H

/* Macro definitions from the Linux kernel. */

#define STR(x) #x
#define XSTR(x) STR(x)

#define CONCAT__(a, b) a ## b
#define CONCATENATE(a, b) CONCAT__(a, b)

#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))

/* Indirect macros required for expanded argument pasting, eg. __LINE__. */
#define ___PASTE(a,b) a##b
#define __PASTE(a,b) ___PASTE(a,b)

#define __UNIQUE_ID(prefix) __PASTE(__PASTE(__UNIQUE_ID_, prefix), __COUNTER__)

/*
 * This returns a constant expression while determining if an argument is
 * a constant expression, most importantly without evaluating the argument.
 * Glory to Martin Uecker <Martin.Uecker@med.uni-goettingen.de>
 */
#define __is_constexpr(x) \
	(sizeof(int) == sizeof(*(8 ? ((void *)((long)(x) * 0l)) : (int *)8)))

#define __no_side_effects(x, y) \
		(__is_constexpr(x) && __is_constexpr(y))

#define NORETURN __attribute__((__noreturn__))

#define __mode(x) __attribute__((__mode__(x)))

#endif /* INTERNAL_MACRO_H */
