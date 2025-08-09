// SPDX-License-Identifier: LGPL-2.1

#ifndef TOSLIBC_ASM_MATH_H
#define TOSLIBC_ASM_MATH_H

#include <stdint.h>

#define DIV_ROUND_CLOSEST_U32(x_, divisor_)				\
({									\
	const typeof(divisor_) __d = divisor_;				\
	const uint32_t _tmp = (x_) + (__d / 2);				\
	_tmp / __d;							\
})

#endif /* TOSLIBC_ASM_MATH_H */
