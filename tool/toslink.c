// SPDX-License-Identifier: GPL-2.0
/*
 * Link programs for Atari TOS
 *
 * Copyright (C) 2022 Fredrik Noring
 */

#include <stdio.h>
#include <stdlib.h>

#include "tool/file.h"
#include "tool/option.h"
#include "tool/prg.h"
#include "tool/print.h"
#include "tool/relocation.h"
#include "tool/section.h"
#include "tool/symbol.h"
#include "tool/tool.h"
#include "tool/verify.h"

char progname[] = "toslink";

static size_t append_header(struct file *tf, struct file *ef)
{
	const struct prg_header prg_header = {
		.magic       = prg_header_magic,
		.text_size   = section_size(ef, text_section),
		.data_size   = section_size(ef, data_section),
		.bss_size    = section_size(ef, bss_section),
		.symbol_size = symbol_size(ef, symtab_section)
	};

	BUILD_BUG_ON(sizeof(struct prg_header) != PRG_HEADER_SIZE);

	file_append(tf, (const void *)&prg_header, sizeof(prg_header));

	return sizeof(prg_header) +
	       prg_header.text_size +
	       prg_header.data_size +
	       prg_header.symbol_size;
}

static void link_program(struct file *tf, struct file *ef)
{
	size_t size = append_header(tf, ef);

	append_sections_text_data(tf, ef);
	append_symbols(tf, ef);
	size += append_relocations_text_data(tf, ef);

	if (tf->size != size)
		pr_fatal_error("%s: size mismatch %zu != %zu",
			ef->path, tf->size, size);
}

int main(int argc, char **argv)
{
	parse_options(argc, argv);

	struct file ef = file_read(option.input);
	struct file tf = { };

	verify_elf(&ef);
	link_program(&tf, &ef);

	if (!file_rename(option.output, &tf))
		pr_fatal_errno(option.output);

	if (!file_write(tf, 0755))
		pr_fatal_errno(tf.path);

	file_free(tf);
	file_free(ef);

	return EXIT_SUCCESS;
}
