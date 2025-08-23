// SPDX-License-Identifier: LGPL-2.1

#ifndef TOSLIBC_ASM_INTERRUPT_H
#define TOSLIBC_ASM_INTERRUPT_H

/*
 * The compiler generates function entry and exit sequences suitable for
 * use in an interrupt handler when this attribute is present. Registers
 * are saved and the rte instruction exits.
 */
#define INTERRUPT __attribute__((__interrupt__))

#endif /* TOSLIBC_ASM_INTERRUPT_H */
