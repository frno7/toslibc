#!/bin/bash

set -e
set -o pipefail

gen_prg_ld()
{
	cat <<EOF
/* SPDX-License-Identifier: GPL-2.0 */

ENTRY(_start)

SEARCH_DIR($libdir)
INPUT(libc.a)

PHDRS
{
	text PT_LOAD FLAGS(7);
}

SECTIONS
{
	.text : ALIGN(4) {
		*(.text._start)
		*(.text)
		*(.text.*)
		. = ALIGN(4);
	} :text = 0

	.data : ALIGN(4) {
		*(.data)
		*(.data.*)
		*(.rodata)
		*(.rodata.*)
		. = ALIGN(4);
	} = 0

	.bss : ALIGN(4) {
		*(.bss)
		*(.bss.*)
		*(COMMON)
		. = ALIGN(4);
	}

	/DISCARD/ : {
		*(.comment)
		*(.note.*)
	}
}
EOF
}

if [ $# = 1 ]
then
	gen_prg_ld >"$1".tmp
	mv "$1".tmp "$1"
else
	gen_prg_ld
fi
