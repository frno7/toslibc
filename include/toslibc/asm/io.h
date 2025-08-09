// SPDX-License-Identifier: LGPL-2.1

#ifndef TOSLIBC_ASM_IO_H
#define TOSLIBC_ASM_IO_H

#include <stdint.h>

#define barrier()	__sync_synchronize()

#define DEFINE_IORD(type, name)						\
static inline type name(const uint32_t addr)				\
{									\
	const volatile type *__addr = (const volatile type *)addr;	\
									\
	barrier();							\
	const type value = *__addr;					\
	barrier();							\
									\
	return value;							\
}

DEFINE_IORD(uint8_t,  iord8);
DEFINE_IORD(uint16_t, iord16);
DEFINE_IORD(uint32_t, iord32);

#define DEFINE_IOWR(type, name)						\
static inline void name(type value, uint32_t addr)			\
{									\
	volatile type *__addr = (volatile type *)addr;			\
									\
	barrier();							\
	*__addr = value;						\
	barrier();							\
}

DEFINE_IOWR(uint8_t,  iowr8);
DEFINE_IOWR(uint16_t, iowr16);
DEFINE_IOWR(uint32_t, iowr32);

#endif /* TOSLIBC_ASM_IO_H */
