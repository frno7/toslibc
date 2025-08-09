#!/bin/bash

set -e
set -o pipefail

gen_sndh_ld()
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
		*(.text._sndh)
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

	.sndh.title : {
		KEEP (*(.sndh.title));
	}

	.sndh.tune.count : {
		KEEP (*(.sndh.tune.count));
	}

	.sndh.tune.names : {
		KEEP (*(.sndh.tune.names));
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
	gen_sndh_ld >"$1".tmp
	mv "$1".tmp "$1"
else
	gen_sndh_ld
fi
