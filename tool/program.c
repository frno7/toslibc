// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (C) 2025 Fredrik Noring
 */

#include "tool/file.h"
#include "tool/option.h"
#include "tool/prg.h"
#include "tool/print.h"
#include "tool/program.h"
#include "tool/relocation.h"
#include "tool/section.h"
#include "tool/symbol.h"
#include "tool/tool.h"

#define PRG_HEADER_SIZE 28

struct program {
	struct link_header header;

	size_t size;
};

static struct program append_prg_header(struct file *tf, struct file *ef)
{
	const struct prg_header prg_header = {
		.magic       = prg_header_magic,
		.text_size   = section_size(ef, ".text", text_section),
		.data_size   = section_size(ef, ".data", data_section),
		.bss_size    = section_size(ef, ".bss",  bss_section),
		.symbol_size = option_symbols() ?
					symbol_size(ef, symtab_section) : 0,
	};
	const struct link_header link_header = {
		.size = sizeof(struct prg_header)
	};
	const struct program program = {
		.header = link_header,
		.size = sizeof(prg_header) +
			prg_header.text_size +
			prg_header.data_size +
			prg_header.symbol_size +
			relocation_size(ef, &link_header)
	};

	BUILD_BUG_ON(sizeof(struct prg_header) != PRG_HEADER_SIZE);

	file_append(tf, (const void *)&prg_header, sizeof(prg_header));

	return program;
}

void link_program(struct file *tf, struct file *ef)
{
	const struct program program = append_prg_header(tf, ef);

	append_sections_text_data(tf, ef);

	if (option_symbols())
		append_symbols(tf, ef);

	append_relocations_text_data(tf, ef, &program.header);

	if (tf->size != program.size)
		pr_fatal_error("%s: size mismatch %zu != %zu",
			ef->path, tf->size, program.size);
}
