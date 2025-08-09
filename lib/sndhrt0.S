// SPDX-License-Identifier: LGPL-2.1
/*
 * Copyright (C) 2025 Fredrik Noring
 */

#include <tos/stddef.S>

#define SNDH_SECTION_TEXT_SIZE	-16
#define SNDH_SECTION_DATA_SIZE	-12
#define SNDH_SECTION_BSS_SIZE	-8
#define SNDH_SECTION_REL_SIZE	-4

	.section ".text._sndh", "ax"

	.globl	_sndh
	.type	_sndh,@function
_sndh:

	.globl	_sndh_play
	.type	_sndh_play,@function
_sndh_play:
	jmp sndh_play

	.globl	_sndh_exit
	.type	_sndh_exit,@function
_sndh_exit:
	jmp sndh_exit

	.globl	_sndh_init
	.type	_sndh_init,@function
_sndh_init:
	lea	_sndh(pc),a0
	tst.l	SNDH_SECTION_REL_SIZE(a0) /* Check if already relocated. */
	bne.s	_relocate
	jmp	sndh_init

/*
 * The relocation section begins with a longword offset to the first
 * longword location to fixup, or zero meaning no fixups. Then follows
 * a stream of bytes:
 *
 * 	    0: end of list
 * 	    1: advance 254 bytes
 * 	2-254: advance this many bytes, and fixup the longword there
 *
 * The relocation codes overlap the BSS section, so clear all codes after use.
 */
	.globl	_relocate
	.type	_relocate,@function
_relocate:
	move.l	a0,a1		/* a1 is location to fixup. */
	move.l	a0,d0		/* d0 is address for fixup. */
	clr.l	d1		/* d1 is 2-254 byte advancement. */
	clr.l	SNDH_SECTION_REL_SIZE(a1) /* Clear to indicate relocated. */
	add.l	SNDH_SECTION_TEXT_SIZE(a1),a0
	add.l	SNDH_SECTION_DATA_SIZE(a1),a0 /* a0 is relocation section. */

	/* Fixup first longword, if longword offset is nonzero. */
	tst.l	(a0)
	beq.s	_3
	add.l	(a0),a1		/* Advance to first location for fixup. */
	clr.l	(a0)+		/* Clear for BSS overlap. */
	add.l	d0,(a1)		/* Fixup longword. */

_1:	move.b	(a0),d1		/* Read byte with offset to advance. */
	beq.s	_3		/* Zero means end of stream. */
	clr.b	(a0)+		/* Clear for BSS overlap. */

	cmp.b	#1,d1
	bne.s	_2
	lea	254(a1),a1
	bra.s	_1

_2:	add.l	d1,a1		/* Advance to next longword for fixup. */
	add.l	d0,(a1)		/* Fixup longword. */
	bra.s	_1

_3:	jmp	sndh_init
