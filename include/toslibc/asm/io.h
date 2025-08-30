// SPDX-License-Identifier: LGPL-2.1

#ifndef TOSLIBC_ASM_IO_H
#define TOSLIBC_ASM_IO_H

#include <stdint.h>

#define barrier()	__sync_synchronize()

#define ASM_IO_DEFINE_IORD(size)					\
static inline uint##size##_t iord##size(const uint32_t addr)		\
{									\
	const volatile uint##size##_t *addr__ =				\
		(const volatile uint##size##_t *)addr;			\
									\
	barrier();							\
	const uint##size##_t value = *addr__;				\
	barrier();							\
									\
	return value;							\
}

ASM_IO_DEFINE_IORD(8)   /* iord8  */
ASM_IO_DEFINE_IORD(16)  /* iord16 */
ASM_IO_DEFINE_IORD(32)  /* iord32 */

#define ASM_IO_DEFINE_IOWR(size)					\
static inline void iowr##size(uint##size##_t value, uint32_t addr)	\
{									\
	volatile uint##size##_t *addr__ =				\
		(volatile uint##size##_t *)addr;			\
									\
	barrier();							\
	*addr__ = value;						\
	barrier();							\
}

ASM_IO_DEFINE_IOWR(8)   /* iowr8  */
ASM_IO_DEFINE_IOWR(16)  /* iowr16 */
ASM_IO_DEFINE_IOWR(32)  /* iowr32 */

#define ASM_IO_DEFINE_WR_OP(name, op, size)				\
static inline void io##name##size(uint##size##_t val, uint32_t addr)	\
{									\
	barrier();							\
	__asm__ __volatile__						\
		(#op " %0,%1" :: "di" (val),				\
			"m" (*(uint##size##_t *)addr) : "memory");	\
	barrier();							\
}

ASM_IO_DEFINE_WR_OP( or,  orb, 8)   /* ioor8   */
ASM_IO_DEFINE_WR_OP(and, andb, 8)   /* ioand8  */
ASM_IO_DEFINE_WR_OP(xor, eorb, 8)   /* ioxor8  */

ASM_IO_DEFINE_WR_OP( or,  orw, 16)  /* ioor16  */
ASM_IO_DEFINE_WR_OP(and, andw, 16)  /* ioand16 */
ASM_IO_DEFINE_WR_OP(xor, eorw, 16)  /* ioxor16 */

ASM_IO_DEFINE_WR_OP( or,  orl, 32)  /* ioor32  */
ASM_IO_DEFINE_WR_OP(and, andl, 32)  /* ioand32 */
ASM_IO_DEFINE_WR_OP(xor, eorl, 32)  /* ioxor32 */

#endif /* TOSLIBC_ASM_IO_H */
