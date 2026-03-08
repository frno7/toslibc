// SPDX-License-Identifier: LGPL-2.1
/*
 * Copyright (C) 2019 Fredrik Noring
 */

#ifndef TOS_TRAP_S
#define TOS_TRAP_S

#include <tos/stddef.S>

/*
 * Push trap arguments in front of the rts address, and convert
 * 32-bit words to 16-bit words for TOS. This method is reentrant.
 */

#define TRAP_ARGS(args)							\
	args(, 	        0, 0, 0, 0, 0, 0, 0, 0, 0, 0)			\
	args(w,	        1, 2, 0, 0, 0, 0, 0, 0, 0, 0)			\
	args(l,	        1, 4, 0, 0, 0, 0, 0, 0, 0, 0)			\
	args(ww,        2, 2, 2, 0, 0, 0, 0, 0, 0, 0)			\
	args(wl,        2, 2, 4, 0, 0, 0, 0, 0, 0, 0)			\
	args(lw,        2, 4, 2, 0, 0, 0, 0, 0, 0, 0)			\
	args(wwl,       3, 2, 2, 4, 0, 0, 0, 0, 0, 0)			\
	args(wll,       3, 2, 4, 4, 0, 0, 0, 0, 0, 0)			\
	args(lww,       3, 4, 2, 2, 0, 0, 0, 0, 0, 0)			\
	args(lwl,       3, 4, 2, 4, 0, 0, 0, 0, 0, 0)			\
	args(lll,       3, 4, 4, 4, 0, 0, 0, 0, 0, 0)			\
	args(wwwl,      4, 2, 2, 2, 4, 0, 0, 0, 0, 0)			\
	args(wlll,      4, 2, 4, 4, 4, 0, 0, 0, 0, 0)			\
	args(llww,      4, 4, 4, 2, 2, 0, 0, 0, 0, 0)			\
	args(wlwww,     5, 2, 4, 2, 2, 2, 0, 0, 0, 0)			\
	args(wwwwww,    6, 2, 2, 2, 2, 2, 2, 0, 0, 0)			\
	args(llwwwww,   7, 4, 4, 2, 2, 2, 2, 2, 0, 0)			\
	args(llwwwwwlw, 9, 4, 4, 2, 2, 2, 2, 2, 4, 2)

#define TRAP_ARG_SIZE(s, n, a, b, c, d, e, f, g, h, i)			\
	.equ	TRAP_ARG_SIZE_##s,(a+b+c+d+e+f+g+h+i);
TRAP_ARGS(TRAP_ARG_SIZE)

/**
 * trap_push_arg - convert and push an argument for traps
 * @s: stack offset
 * @n: number of stack arguments
 * @w: argument, if any
 * @o: accumulated argument offset
 */
.macro trap_push_arg t s n w o
#if __SIZEOF_INT__ == 2
.if	\w == 2
	move.w	\s+(\t)+(4-\w)(sp),-(sp)
.elseif	\w == 4
	move.l	\s+(\t)(sp),-(sp)
.endif
#elif __SIZEOF_INT__ == 4
.if	\w == 2
	move.w	\s+(4*(\n))-(\o)+(4-(\w))(sp),-(sp)
.elseif	\w == 4
	move.l	\s+(4*(\n))-(\o)(sp),-(sp)
.endif
#else
#error "__SIZEOF_INT__ neither 2 nor 4?"
#endif
.endm

/**
 * trap_push_args - convert and push arguments for traps
 * @s: stack offset
 * @n: number of stack arguments
 * @a: 1st argument, if any
 * @b: 2nd argument, if any
 * @c: 3rd argument, if any
 * @d: 4th argument, if any
 * @e: 5th argument, if any
 * @f: 6th argument, if any
 * @g: 7th argument, if any
 * @h: 8th argument, if any
 * @i: 9th argument, if any
 */
.macro trap_push_args t s n a b c d e f g h i
	trap_push_arg \t \s \n \i 0
	trap_push_arg \t \s \n \h \i%4
	trap_push_arg \t \s \n \g \i%4+\h%4
	trap_push_arg \t \s \n \f \i%4+\h%4+\g%4
	trap_push_arg \t \s \n \e \i%4+\h%4+\g%4+\f%4
	trap_push_arg \t \s \n \d \i%4+\h%4+\g%4+\f%4+\e%4
	trap_push_arg \t \s \n \c \i%4+\h%4+\g%4+\f%4+\e%4+\d%4
	trap_push_arg \t \s \n \b \i%4+\h%4+\g%4+\f%4+\e%4+\d%4+\c%4
	trap_push_arg \t \s \n \a \i%4+\h%4+\g%4+\f%4+\e%4+\d%4+\c%4+\b%4
.endm

#define TRAP_ARG_MACRO(t_, n_, a_, b_, c_, d_, e_, f_, g_, h_, i_)	\
.macro trap_push_args_##t_ s;						\
	trap_push_args TRAP_ARG_SIZE_##t_ \s n_ a_ b_ c_ d_ e_ f_ g_ h_ i_; \
.endm;
TRAP_ARGS(TRAP_ARG_MACRO)

.macro trap_opcode trap_ opcode_ t_=0
	move.w	#\opcode_,-(sp)
	trap	#\trap_
.if 2+\t_ <= 8
	addq.l	#2+\t_,sp
.else
	lea	2+\t_(sp),sp
.endif
.endm

#endif /* TOS_TRAP_S */
