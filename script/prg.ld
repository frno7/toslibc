/* SPDX-License-Identifier: GPL-2.0 */

ENTRY(_start)

PHDRS
{
	text PT_LOAD FLAGS(7);
}

SECTIONS
{
	.text : ALIGN(4) {
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
